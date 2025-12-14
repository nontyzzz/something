#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stage.h"
#include "gacha.h"
#include "weapon.h"
#include "character.h"

CombatCharacter current_player;

/* Check if player owns a character (character IDs are 1..4) */
static int has_character(const PlayerData *p, int char_id) {
    for (int i = 0; i < p->inv_count; i++) {
        if (p->inventory[i] == char_id) return 1;
    }
    return 0;
}

/* Character Selection Menu - Only allow selecting owned characters */
static void character_selection_menu(PlayerData *p) {
    while (1) {
        printf("\n===== SELECT CHARACTER =====\n");

        int count = 0;
        int choices[4] = {0};

        if (has_character(p, 1)) { choices[count++] = 1; printf("%d) Saber\n", count); }
        else { printf("- Saber (Locked - Not Owned)\n"); }

        if (has_character(p, 2)) { choices[count++] = 2; printf("%d) Ishtar\n", count); }
        else { printf("- Ishtar (Locked - Not Owned)\n"); }

        if (has_character(p, 3)) { choices[count++] = 3; printf("%d) Gilgamesh\n", count); }
        else { printf("- Gilgamesh (Locked - Not Owned)\n"); }

        if (has_character(p, 4)) { choices[count++] = 4; printf("%d) Welt Yang\n", count); }
        else { printf("- Welt Yang (Locked - Not Owned)\n"); }

        if (count == 0) {
            printf("\nNo characters owned! Gacha from Character Banner first.\n");
            printf("Press Enter to continue...");
            while (getchar() != '\n') {}
            return;
        }

        printf("0) Back\n");
        printf("Choice: ");

        int choice = -1;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {}
            continue;
        }
        while (getchar() != '\n') {}

        if (choice == 0) return;
        if (choice < 1 || choice > count) continue;

        int selected_char = choices[choice - 1];
        current_player = create_character(selected_char);
        p->current_character = selected_char - 1;

        /* Apply saved equipped weapon to this character (if any) */
        if (p->equipped_weapon[p->current_character] > 0) {
            equip_weapon(&current_player, p->equipped_weapon[p->current_character]);
        }

        printf("Selected: %s\n", current_player.name);
        return;
    }
}

/* Sort weapons by rarity descending, then OP descending */
static int compare_weapons(const void *a, const void *b) {
    const Weapon *w1 = (const Weapon *)a;
    const Weapon *w2 = (const Weapon *)b;

    if (w1->rarity != w2->rarity) return w2->rarity - w1->rarity;

    int op1 = w1->atk + w1->spd + w1->def + (w1->hp / 5);
    int op2 = w2->atk + w2->spd + w2->def + (w2->hp / 5);
    return op2 - op1;
}

static void weapon_equip_menu(PlayerData *p) {
    while (1) {
        printf("\n===== EQUIP WEAPON =====\n");
        printf("Current Character: %s\n", current_player.name);
        printf("Owned Weapons:\n");

        Weapon compatible[MAX_INVENTORY];
        int comp_count = 0;

        for (int i = 0; i < p->inv_count; i++) {
            Weapon *w = get_weapon_by_id(p->inventory[i]);
            if (w && can_equip(&current_player, w)) {
                compatible[comp_count++] = *w;
            }
        }

        if (comp_count == 0) {
            printf("No compatible weapons owned!\n");
            printf("Press Enter to continue...");
            while (getchar() != '\n') {}
            return;
        }

        qsort(compatible, comp_count, sizeof(Weapon), compare_weapons);

        for (int i = 0; i < comp_count; i++) {
            int op = compatible[i].atk + compatible[i].spd + compatible[i].def + (compatible[i].hp / 5);
            printf("%d) %s [%d-stars OP:%d] (ATK:%d HP:%d DEF:%d SPD:%d)\n",
                   i + 1,
                   compatible[i].name,
                   compatible[i].rarity,
                   op,
                   compatible[i].atk,
                   compatible[i].hp,
                   compatible[i].def,
                   compatible[i].spd);
        }

        printf("0) Back\n");
        printf("Choice: ");

        int choice = -1;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {}
            continue;
        }
        while (getchar() != '\n') {}

        if (choice == 0) return;
        if (choice < 1 || choice > comp_count) continue;

        int weapon_id = compatible[choice - 1].id;

        equip_weapon(&current_player, weapon_id);
        p->equipped_weapon[p->current_character] = weapon_id;
        save_player_data(p);

        printf("Equipped: %s\n", compatible[choice - 1].name);
        printf("Press Enter to continue...");
        while (getchar() != '\n') {}
        return;
    }
}

static void main_menu(PlayerData *p) {
    /* Pick a sensible default current character (first owned), else Saber */
    int chosen = 0;
    for (int i = 1; i <= 4; i++) {
        if (has_character(p, i)) {
            current_player = create_character(i);
            p->current_character = i - 1;
            chosen = 1;
            break;
        }
    }
    if (!chosen) {
        current_player = create_character(1);
        p->current_character = 0;
    }

    /* Apply saved equipped weapon to current_player */
    if (p->equipped_weapon[p->current_character] > 0) {
        equip_weapon(&current_player, p->equipped_weapon[p->current_character]);
    }

    while (1) {
        printf("\n===== MAIN MENU =====\n");
        printf("Current: %s", current_player.name);

        if (current_player.equipped_weapon_id > 0) {
            Weapon *w = get_weapon_by_id(current_player.equipped_weapon_id);
            if (w) {
                int op = w->atk + w->spd + w->def + (w->hp / 5);
                printf(" + %s [%d-stars OP:%d]", w->name, w->rarity, op);
            } else {
                printf(" (No weapon equipped)");
            }
        } else {
            printf(" (No weapon equipped)");
        }

        printf("\nStats: ATK:%d HP:%d DEF:%d SPD:%d\n",
               current_player.atk, current_player.hp, current_player.def, current_player.spd);

        printf("\n1) Gacha\n");
        printf("2) Stages\n");
        printf("3) Select Character\n");
        printf("4) Equip Weapon\n");
        printf("5) Inventory\n");
        printf("6) Save & Exit\n");
        printf("Choice: ");

        int choice = -1;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {}
            continue;
        }
        while (getchar() != '\n') {}

        if (choice == 1) {
            banner_menu(p);

            /* Reload current_player in case gacha changed inventory */
            if (p->current_character >= 0 && p->current_character < 4) {
                current_player = create_character(p->current_character + 1);
                if (p->equipped_weapon[p->current_character] > 0) {
                    equip_weapon(&current_player, p->equipped_weapon[p->current_character]);
                }
            }
        } else if (choice == 2) {
            stage_selection_menu(p, &current_player);
        } else if (choice == 3) {
            character_selection_menu(p);
        } else if (choice == 4) {
            weapon_equip_menu(p);
        } else if (choice == 5) {
            printf("\n===== INVENTORY =====\n");
            printf("Items Owned: %d\n", p->inv_count);
            printf("Gems: %d\n", p->gems);

            for (int i = 0; i < p->inv_count && i < 20; i++) {
                Weapon *w = get_weapon_by_id(p->inventory[i]);
                if (w) {
                    int op = w->atk + w->spd + w->def + (w->hp / 5);
                    printf("%d) %s [%d-stars OP:%d]\n", i + 1, w->name, w->rarity, op);
                } else {
                    printf("%d) ID: %d\n", i + 1, p->inventory[i]);
                }
            }

            if (p->inv_count > 20) {
                printf("... and %d more items\n", p->inv_count - 20);
            }

            printf("Press Enter to continue...");
            while (getchar() != '\n') {}
        } else if (choice == 6) {
            save_player_data(p);
            printf("Game saved!\n");
            return;
        }
    }
}

int main(void) {
    gacha_init();

    PlayerData data;
    load_player_data(&data);

    printf("=== Welcome to Turn Based Gacha Game ===\n");
    printf("Loaded player data successfully.\n");

    main_menu(&data);
    return 0;
}
