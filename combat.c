#include "combat.h"
#include "character.h"
#include "weapon.h" // Needed for Weapon structs and initialization functions
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> 

//----------------------------------------------------
// SAVE & LOAD (Updated for new Character struct)
//----------------------------------------------------
void saveCharacter(Character *c) {
    FILE *fp = fopen("./character.txt", "w");
    if (!fp) { printf("Error saving!\n"); return; }

    // Save ALL current stats (which include weapon boosts) plus the equipped weapon ID
    fprintf(fp, "%s %d %d %d %d %d %d %d %d %d",
        c->name,
        c->atk,
        c->hp,
        c->def,
        c->spd,
        c->maxHP,
        c->hasWeaponSkill,
        c->isBurning,
        c->isStunned,
        c->equippedWeapon.id // Save the equipped weapon ID
    );

    fclose(fp);
    printf("Character saved to character.txt!\n");
}

void loadCharacter(Character *c) {
    FILE *fp = fopen("./character.txt", "r");
    
    int loaded_wpn_id = 0;
    
    if (!fp) {
        printf("character.txt not found. Using default stats.\n");
        // The calling function (main.c/inventory_menu) must call createCharacter first,
        // so we just ensure the weapon field is clean here if the file is missing.
        c->equippedWeapon = createDefaultWeapon(); 
        return;
    }

    int read_count = fscanf(fp, "%s %d %d %d %d %d %d %d %d %d",
        c->name,
        &c->atk,
        &c->hp,
        &c->def,
        &c->spd,
        &c->maxHP,
        &c->hasWeaponSkill,
        &c->isBurning,
        &c->isStunned,
        &loaded_wpn_id // Load the equipped weapon ID
    );

    if (read_count != 10) {
        printf("Incomplete data found in character.txt. Resetting equipped weapon.\n");
        // This is a minimal fix; ideal would be to reset all stats via createCharacter here
        c->equippedWeapon = createDefaultWeapon();
    } else {
        // Recreate the full Weapon struct based on the loaded ID
        c->equippedWeapon = createWeapon(loaded_wpn_id); 
        printf("Character loaded from character.txt! Equipped Weapon: %s (ID: %d)\n",
               c->equippedWeapon.name, c->equippedWeapon.id);
    }

    fclose(fp);
}

// --- Other combat functions remain here ---

int calculateDamage(int atk, int def) {
    int damage = (atk * 20 / (def + 20)) + (rand() % 5);
    return damage > 1 ? damage : 1;
}

int pickTarget(Enemy enemies[]) {
    // ... (Implementation unchanged)
    int target;

    while (1) {
        printf("\nChoose target:\n");

        for (int i = 0; i < MAX_ENEMIES; i++) {
            printf("[%d] %s (HP: %d)\n", i, enemies[i].name, enemies[i].hp);
        }

        printf("Enter target number: ");
        if (scanf("%d", &target) != 1) { while(getchar() != '\n'); continue; }

        if (target < 0 || target >= MAX_ENEMIES) {
            printf("Invalid target. Choose 0-%d.\n", MAX_ENEMIES - 1);
            continue;
        }

        if (enemies[target].hp <= 0) {
            printf("That enemy is dead. Pick another.\n");
            continue;
        }

        return target; // valid
    }
}

void applyBurn(Character *c) {
    if (c->isBurning > 0) {
        int burn = c->maxHP * 0.05;
        c->hp -= burn;
        c->isBurning--;
        printf("%s takes %d burn damage!\n", c->name, burn);
    }
}

void applyBurnEnemy(Enemy *e) {
    if (e->isBurning > 0) {
        int burn = 5;
        e->hp -= burn;
        e->isBurning--;
        printf("%s takes %d burn damage!\n", e->name, burn);
    }
}

void normalAttack(Character *c, Enemy *e) {
    int dmg = calculateDamage(c->atk, e->def);
    e->hp -= dmg;
    printf("%s attacks %s for %d damage!\n", c->name, e->name, dmg);
}

void guardAction(Character *c) {
    printf("%s is guarding! Damage reduced by 35%%.\n", c->name);
}

void skill1(Character *c, Enemy *e) {
    Enemy targets[MAX_ENEMIES] = {*e};
    useSkill1(c, targets, MAX_ENEMIES);
}

void skill2(Character *c, Enemy *e) {
    Enemy targets[MAX_ENEMIES] = {*e};
    useSkill2(c, targets, MAX_ENEMIES);
}

void weaponSkill(Character *c, Enemy enemies[]) {
    if (c->equippedWeapon.skill_id == 0) {
        printf("Weapon %s has no active skill. Using standard attack.\n", c->equippedWeapon.name);
        int t = pickTarget(enemies);
        if (t != -1) normalAttack(c, &enemies[t]);
        return;
    }

    printf("%s used Weapon Skill (ID: %d) from %s!\n", c->name, c->equippedWeapon.skill_id, c->equippedWeapon.name);
    
    // Example AoE implementation
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].hp > 0) {
            int dmg = calculateDamage((int)(c->atk * 0.5f), enemies[i].def);
            enemies[i].hp -= dmg;
            printf(" - %s took %d AoE damage\n", enemies[i].name, dmg);
        }
    }
}

void enemyAttack(Enemy *e, Character *c) {
    if (e->hp <= 0) return;

    if (e->isStunned) {
        printf("%s is stunned and cannot act!\n", e->name);
        e->isStunned = 0;
        return;
    }

    int dmg = calculateDamage(e->atk, c->def);
    c->hp -= dmg;
    printf("%s attacks %s for %d damage! HP: %d\n", e->name, c->name, c->hp);
}

void takeTurn(Character *c, Enemy enemies[]) {
    int choice;

    printf("\n-- Your Turn --\n");
    printf("1. Normal Attack\n");
    printf("2. Guard\n");
    printf("3. Skill 1\n");
    printf("4. Skill 2\n");
    printf("5. Weapon Skill (%s Skill ID: %d)\n", c->equippedWeapon.name, c->equippedWeapon.skill_id);
    printf("6. Flee\n");
    printf("Choose action: ");
    if (scanf("%d", &choice) != 1) { while(getchar() != '\n'); printf("Invalid input. Guarding.\n"); guardAction(c); return; }

    switch (choice) {
        case 1: {
            int target = pickTarget(enemies);
            if (target != -1) normalAttack(c, &enemies[target]);
        } break;

        case 2:
            guardAction(c);
            break;

        case 3:
            useSkill1(c, enemies, MAX_ENEMIES);
            break;

        case 4:
            useSkill2(c, enemies, MAX_ENEMIES);
            break;

        case 5:
            weaponSkill(c, enemies);
            break;

        case 6:
            printf("You fled!\n");
            exit(0);

        default:
            printf("Invalid action!\n");
            break;
    }
}

void enemyTurns(Character *c, Enemy enemies[]) {
    printf("\n-- Enemy Turn --\n");
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemyAttack(&enemies[i], c);
    }
}

void battle(Character *c) {
    Enemy enemies[MAX_ENEMIES] = {
        {"Slime", 10, 40, 2, 4, 0, 0},
        {"Wolf", 20, 60, 4, 8, 0, 0},
        {"Bandit", 25, 70, 5, 7, 0, 0}
    };
    
    for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].hp += 100;

    printf("\nStarting battle against 3 enemies!\n");
    printf("%s enters battle. Equipped: %s (ATK: %d, HP: %d/%d, DEF: %d)\n",
           c->name, c->equippedWeapon.name, c->atk, c->hp, c->maxHP, c->def);

    while (c->hp > 0) {
        applyBurn(c);
        for (int i = 0; i < MAX_ENEMIES; i++)
            applyBurnEnemy(&enemies[i]);

        int alive = 0;
        for (int i = 0; i < MAX_ENEMIES; i++)
            if (enemies[i].hp > 0)
                alive++;

        if (alive == 0) {
            printf("You won the battle!\n");
            return;
        }

        printf("\n--- Battle Status ---\n");
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].hp > 0) {
                printf("%s: HP %d, ATK %d, DEF %d %s%s\n",
                       enemies[i].name, enemies[i].hp, enemies[i].atk, enemies[i].def, 
                       enemies[i].isBurning > 0 ? "[BURN]" : "", enemies[i].isStunned ? "[STUN]" : "");
            }
        }
        printf("%s: HP %d/%d %s%s\n", c->name, c->hp, c->maxHP, c->isBurning > 0 ? "[BURN]" : "", c->isStunned ? "[STUN]" : "");

        int fastestEnemySPD = 0;
        for (int i = 0; i < MAX_ENEMIES; i++)
            if (enemies[i].hp > 0 && enemies[i].spd > fastestEnemySPD)
                fastestEnemySPD = enemies[i].spd;

        if (c->spd >= fastestEnemySPD) {
            takeTurn(c, enemies);
            enemyTurns(c, enemies);
        } else {
            enemyTurns(c, enemies);
            if (c->hp > 0)
                takeTurn(c, enemies);
        }
        
        if (c->hp <= 0) break;
    }

    printf("You died...\n");
}