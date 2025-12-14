#include "gacha.h"
#include "weapon.h"

/* ===== WEAPON POOLS ===== */

Weapon weapons_2star[4] = {
    {"Iron Sword", 211, RARITY_2STAR, TYPE_SWORD, 80, 800, 80, 80},
    {"Wooden Bow", 221, RARITY_2STAR, TYPE_BOW, 80, 800, 80, 80},
    {"Iron Spear", 231, RARITY_2STAR, TYPE_SPEAR, 80, 800, 80, 80},
    {"Wooden Staff", 241, RARITY_2STAR, TYPE_STAFF, 80, 800, 80, 80}
};

Weapon weapons_3star[20] = {
    {"Emberfang", 311, RARITY_3STAR, TYPE_SWORD, 85, 1050, 120, 75},
    {"Ironleaf Rapier", 312, RARITY_3STAR, TYPE_SWORD, 90, 1000, 115, 85},
    {"Duskwind Cutter", 313, RARITY_3STAR, TYPE_SWORD, 95, 950, 110, 95},
    {"Stormvein Blade", 314, RARITY_3STAR, TYPE_SWORD, 100, 900, 105, 105},
    {"Frostthorn Saber", 315, RARITY_3STAR, TYPE_SWORD, 105, 850, 100, 115},
    {"Willowstrike", 321, RARITY_3STAR, TYPE_BOW, 120, 850, 80, 120},
    {"Ashenwind Bow", 322, RARITY_3STAR, TYPE_BOW, 125, 800, 75, 130},
    {"Shadowglint Recurve", 323, RARITY_3STAR, TYPE_BOW, 130, 750, 70, 140},
    {"Bramblethorn Arch", 324, RARITY_3STAR, TYPE_BOW, 125, 850, 80, 130},
    {"Duskhawk Longbow", 325, RARITY_3STAR, TYPE_BOW, 135, 700, 65, 145},
    {"Thornpiercer", 331, RARITY_3STAR, TYPE_SPEAR, 115, 950, 95, 110},
    {"Ashen Lance", 332, RARITY_3STAR, TYPE_SPEAR, 120, 900, 90, 115},
    {"Duskrend Spear", 333, RARITY_3STAR, TYPE_SPEAR, 125, 850, 85, 120},
    {"Brambletine", 334, RARITY_3STAR, TYPE_SPEAR, 130, 800, 80, 125},
    {"Skyspike Glaive", 335, RARITY_3STAR, TYPE_SPEAR, 135, 750, 75, 130},
    {"Shadowbark Staff", 341, RARITY_3STAR, TYPE_STAFF, 75, 1100, 130, 80},
    {"Mistveil Rod", 342, RARITY_3STAR, TYPE_STAFF, 80, 1150, 125, 75},
    {"Emberflare Cane", 343, RARITY_3STAR, TYPE_STAFF, 85, 1100, 120, 80},
    {"Thornspire Staff", 344, RARITY_3STAR, TYPE_STAFF, 90, 1050, 115, 85},
    {"Frostwind Scepter", 345, RARITY_3STAR, TYPE_STAFF, 95, 1000, 110, 90}
};

Weapon weapons_4star[8] = {
    {"Buster Sword", 411, RARITY_4STAR, TYPE_SWORD, 110, 1250, 140, 100},
    {"Rebellion", 412, RARITY_4STAR, TYPE_SWORD, 120, 1150, 130, 115},
    {"Heilig Bogen", 421, RARITY_4STAR, TYPE_BOW, 140, 1050, 110, 140},
    {"Sunray", 422, RARITY_4STAR, TYPE_BOW, 150, 950, 100, 155},
    {"Heaven Splitter", 431, RARITY_4STAR, TYPE_SPEAR, 150, 1050, 100, 130},
    {"War-Flaming Lance", 432, RARITY_4STAR, TYPE_SPEAR, 145, 1100, 105, 125},
    {"Celestial Scepter", 441, RARITY_4STAR, TYPE_STAFF, 100, 1350, 150, 100},
    {"Staff of Explosion", 442, RARITY_4STAR, TYPE_STAFF, 135, 1150, 130, 120}
};

Weapon weapons_5star[4] = {
    {"Excalibur", 511, RARITY_5STAR, TYPE_SWORD, 140, 1500, 170, 130},
    {"Maanna", 521, RARITY_5STAR, TYPE_BOW, 180, 1250, 130, 180},
    {"EA", 531, RARITY_5STAR, TYPE_SPEAR, 200, 1150, 110, 160},
    {"Star of Eden", 541, RARITY_5STAR, TYPE_STAFF, 160, 1400, 160, 150}
};

Character characters[4] = {
    {"Saber", 1},
    {"Ishtar", 2},
    {"Gilgamesh", 3},
    {"Welt Yang", 4}
};

/* ===== OP CALCULATION (FIX #7) ===== */

int calculate_op(Weapon *w) {
    if (!w) return 0;
    return w->atk + w->spd + w->def + (w->hp / 5);
}

/* ===== GACHA IMPLEMENTATION ===== */

void gacha_init(void) {
    srand((unsigned int)time(NULL));
}

void add_starter_weapons(PlayerData *p) {
    if (p->inv_count == 0) {
        printf("Adding starter weapons (all 2-stars)...\n");
        for (int i = 0; i < 4; i++) {
            if (p->inv_count < MAX_INVENTORY) {
                p->inventory[p->inv_count++] = weapons_2star[i].id;
                printf("Added: %s\n", weapons_2star[i].name);
            }
        }
    }
}

void sort_inventory(PlayerData *p) {
    if (p->inv_count <= 1) return;

    for (int i = 0; i < p->inv_count - 1; i++) {
        for (int j = 0; j < p->inv_count - i - 1; j++) {
            Weapon *w1 = get_weapon_by_id(p->inventory[j]);
            Weapon *w2 = get_weapon_by_id(p->inventory[j + 1]);
            if (!w1 || !w2) continue;

            int op1 = calculate_op(w1);
            int op2 = calculate_op(w2);

            if (w1->rarity > w2->rarity) {
                int temp = p->inventory[j];
                p->inventory[j] = p->inventory[j + 1];
                p->inventory[j + 1] = temp;
            } else if (w1->rarity == w2->rarity && op1 < op2) {
                int temp = p->inventory[j];
                p->inventory[j] = p->inventory[j + 1];
                p->inventory[j + 1] = temp;
            }
        }
    }
}

/* FIX #5 & #6: Updated load_player_data */
void load_player_data(PlayerData *p) {
    FILE *f = fopen("PlayerData.txt", "r");
    p->gems = 0;
    p->char_banner_first = 1;
    p->pity_4star = 0;
    p->pity_5star = 0;
    p->inv_count = 0;
    p->highest_stage_cleared = 0;

    /* FIX #5: Initialize current character */
    p->current_character = 0;

    /* FIX #4 & #6: Initialize equipped weapons to 0 */
    for (int i = 0; i < 5; i++) {
        p->equipped_weapon[i] = 0;
        p->character_level[i] = 1;
    }

    if (!f) {
        printf("No PlayerData.txt found. Starting new game.\n");
        add_starter_weapons(p);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "GEMS:", 5) == 0) {
            sscanf(line + 5, "%d", &p->gems);
        } else if (strncmp(line, "CHAR_BANNER_FIRST:", 18) == 0) {
            sscanf(line + 18, "%d", &p->char_banner_first);
        } else if (strncmp(line, "PITY_4STAR:", 11) == 0) {
            sscanf(line + 11, "%d", &p->pity_4star);
        } else if (strncmp(line, "PITY_5STAR:", 11) == 0) {
            sscanf(line + 11, "%d", &p->pity_5star);
        } else if (strncmp(line, "HIGHEST_STAGE:", 14) == 0) {
            sscanf(line + 14, "%d", &p->highest_stage_cleared);
        } else if (strncmp(line, "CURRENT_CHARACTER:", 18) == 0) {
            sscanf(line + 18, "%d", &p->current_character);
        } else if (strncmp(line, "EQUIPPED_WEAPONS:", 16) == 0) {
            char *tok = strtok(line + 16, ",\n");
            int idx = 1;
            while (tok && idx <= 4) {
                p->equipped_weapon[idx++] = atoi(tok);
                tok = strtok(NULL, ",\n");
            }
        } else if (strncmp(line, "ITEMS:", 6) == 0) {
            char *tok = strtok(line + 6, ",\n");
            while (tok && p->inv_count < MAX_INVENTORY) {
                p->inventory[p->inv_count++] = atoi(tok);
                tok = strtok(NULL, ",\n");
            }
        }
    }

    fclose(f);

    if (p->inv_count == 0) {
        add_starter_weapons(p);
    }
}

/* FIX #5 & #6: Updated save_player_data */
void save_player_data(const PlayerData *p) {
    FILE *f = fopen("PlayerData.txt", "w");
    if (!f) {
        printf("Error: could not save PlayerData.txt\n");
        return;
    }

    fprintf(f, "GEMS:%d\n", p->gems);
    fprintf(f, "CHAR_BANNER_FIRST:%d\n", p->char_banner_first);
    fprintf(f, "PITY_4STAR:%d\n", p->pity_4star);
    fprintf(f, "PITY_5STAR:%d\n", p->pity_5star);
    fprintf(f, "HIGHEST_STAGE:%d\n", p->highest_stage_cleared);

    /* FIX #5: Save current character selection */
    fprintf(f, "CURRENT_CHARACTER:%d\n", p->current_character);

    /* FIX #4 & #6: Save equipped weapons per character */
    fprintf(f, "EQUIPPED_WEAPONS:");
    for (int i = 1; i <= 4; i++) {
        fprintf(f, "%d", p->equipped_weapon[i]);
        if (i < 4) fprintf(f, ",");
    }
    fprintf(f, "\n");

    /* Inventory */
    fprintf(f, "ITEMS:");
    for (int i = 0; i < p->inv_count; i++) {
        fprintf(f, "%d", p->inventory[i]);
        if (i < p->inv_count - 1) fprintf(f, ",");
    }
    fprintf(f, "\n");

    fclose(f);
}

int has_item(const PlayerData *p, int id) {
    for (int i = 0; i < p->inv_count; i++) {
        if (p->inventory[i] == id) return 1;
    }
    return 0;
}

Item pull_single(int banner_type, PlayerData *p) {
    Item result;
    memset(&result, 0, sizeof(Item));

    if (p->pity_5star >= 99) {
        p->pity_5star = 0;
        p->pity_4star = 0;
        if (banner_type == 1) {
            int idx = rand() % 4;
            strcpy(result.name, weapons_5star[idx].name);
            result.id = weapons_5star[idx].id;
            result.type = ITEM_WEAPON;
            result.star = 5;
        } else {
            int idx = rand() % 4;
            strcpy(result.name, characters[idx].name);
            result.id = characters[idx].id;
            result.type = ITEM_CHARACTER;
            result.star = 5;
        }
        return result;
    }

    if (p->pity_4star >= 9) {
        p->pity_4star = 0;
        p->pity_5star++;
        int idx = rand() % 8;
        strcpy(result.name, weapons_4star[idx].name);
        result.id = weapons_4star[idx].id;
        result.type = ITEM_WEAPON;
        result.star = 4;
        return result;
    }

    float rng = (float)rand() / (float)RAND_MAX * 100.0f;

    if (rng < 1.0f) {
        p->pity_5star = 0;
        p->pity_4star = 0;
        if (banner_type == 1) {
            int idx = rand() % 4;
            strcpy(result.name, weapons_5star[idx].name);
            result.id = weapons_5star[idx].id;
            result.type = ITEM_WEAPON;
            result.star = 5;
        } else {
            int idx = rand() % 4;
            strcpy(result.name, characters[idx].name);
            result.id = characters[idx].id;
            result.type = ITEM_CHARACTER;
            result.star = 5;
        }
        return result;
    }

    if (rng < 10.0f) {
        p->pity_4star = 0;
        p->pity_5star++;
        int idx = rand() % 8;
        strcpy(result.name, weapons_4star[idx].name);
        result.id = weapons_4star[idx].id;
        result.type = ITEM_WEAPON;
        result.star = 4;
        return result;
    }

    p->pity_4star++;
    p->pity_5star++;
    int idx = rand() % 20;
    strcpy(result.name, weapons_3star[idx].name);
    result.id = weapons_3star[idx].id;
    result.type = ITEM_WEAPON;
    result.star = 3;
    return result;
}

void show_status(const PlayerData *p) {
    printf("\n===== PLAYER STATUS =====\n");
    printf("Gems: %d\n", p->gems);
    printf("4-stars Pity: %d/10\n", p->pity_4star);
    printf("5-stars Pity: %d/100\n", p->pity_5star);
    printf("Items Owned: %d\n", p->inv_count);
    printf("==========================\n");
}

void banner_menu(PlayerData *p) {
    while (1) {
        show_status(p);
        printf("\n=== GACHA MENU ===\n");
        printf("1) Weapon Banner\n");
        printf("2) Character Banner%s\n", p->char_banner_first ? " [FREE FIRST PULL]" : "");
        printf("3) Add 1000 Gems (Debug)\n");
        printf("4) Back to Main Menu\n");
        printf("Choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (choice == 4) return;

        if (choice == 3) {
            p->gems += 1000;
            save_player_data(p);
            printf("Added 1000 gems!\n");
            continue;
        }

        if (choice != 1 && choice != 2) continue;

        int is_free = (choice == 2 && p->char_banner_first);

        show_status(p);
        printf("\n=== %s ===\n", choice == 1 ? "WEAPON BANNER" : "CHARACTER BANNER");

        if (is_free) {
            printf("1) Single Pull (FREE - first time)\n");
        } else {
            printf("1) Single Pull (%d gems)\n", GEM_COST_SINGLE);
        }

        printf("2) 10-Pull (%d gems)\n", GEM_COST_TEN);
        printf("3) Back\n");
        printf("Choice: ");

        int pull_choice;
        if (scanf("%d", &pull_choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (pull_choice == 3) continue;
        if (pull_choice != 1 && pull_choice != 2) continue;

        int cost = is_free && pull_choice == 1 ? 0 : (pull_choice == 1 ? GEM_COST_SINGLE : GEM_COST_TEN);

        if (p->gems < cost) {
            printf("Not enough gems!\n");
            continue;
        }

        p->gems -= cost;
        int pulls = pull_choice == 1 ? 1 : 10;

        if (is_free && pull_choice == 1) {
            printf("\n--- FREE FIRST CHARACTER PULL ---\n");
            int idx = rand() % 4;
            Item it;
            strcpy(it.name, characters[idx].name);
            it.id = characters[idx].id;
            it.type = ITEM_CHARACTER;
            it.star = 5;
            p->char_banner_first = 0;
            p->pity_4star = 0;
            p->pity_5star = 0;
            printf("You pulled: %s [5-stars] (ID %d)\n", it.name, it.id);

            if (!has_item(p, it.id) && p->inv_count < MAX_INVENTORY) {
                p->inventory[p->inv_count++] = it.id;
                printf("NEW!\n");
            } else if (has_item(p, it.id)) {
                p->gems += GEM_REFUND_DUPLICATE;
                printf("DUPLICATE! +%d gems\n", GEM_REFUND_DUPLICATE);
            }
        } else if (pulls == 1) {
            printf("\n--- SINGLE PULL ---\n");
            Item it = pull_single(choice, p);
            printf("You pulled: %s [%d-stars] (ID %d)\n", it.name, it.star, it.id);

            if (!has_item(p, it.id) && p->inv_count < MAX_INVENTORY) {
                p->inventory[p->inv_count++] = it.id;
                printf("NEW!\n");
            } else if (has_item(p, it.id)) {
                p->gems += GEM_REFUND_DUPLICATE;
                printf("DUPLICATE! +%d gems\n", GEM_REFUND_DUPLICATE);
            }
        } else {
            printf("\n--- 10-PULL ---\n");
            for (int i = 0; i < 10; i++) {
                Item it = pull_single(choice, p);
                printf("%d) %s [%d-stars] (ID %d) ", i+1, it.name, it.star, it.id);

                if (!has_item(p, it.id) && p->inv_count < MAX_INVENTORY) {
                    p->inventory[p->inv_count++] = it.id;
                    printf("NEW\n");
                } else if (has_item(p, it.id)) {
                    p->gems += GEM_REFUND_DUPLICATE;
                    printf("DUPLICATE +%d\n", GEM_REFUND_DUPLICATE);
                } else {
                    printf("INV FULL\n");
                }
            }
        }

        sort_inventory(p);
        save_player_data(p);
        printf("\nProgress saved!\n");
        printf("Press Enter to continue...");
        while (getchar() != '\n');
    }
}
