#include "character.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "weapon.h" // Needed for createDefaultWeapon

// forward declare pickTarget (from combat.c)
extern int pickTarget(Enemy enemies[]);
extern int calculateDamage(int atk, int def);
extern void saveCharacter(Character *c); 
extern Weapon createDefaultWeapon(); // Declared in weapon.h

// Create character presets
Character createCharacter(CharacterType type) {
    Character c;
    memset(&c, 0, sizeof(Character));

    switch (type) {
        case CHAR_SABER:
            strcpy(c.name, "Saber");
            c.atk = 40; c.hp = c.maxHP = 180; c.def = 12; c.spd = 15; c.hasWeaponSkill = 1;
            break;

        case CHAR_ISHTAR:
            strcpy(c.name, "Ishtar");
            c.atk = 55; c.hp = c.maxHP = 150; c.def = 8; c.spd = 18; c.hasWeaponSkill = 0;
            break;

        case CHAR_GILGAMESH:
            strcpy(c.name, "Gilgamesh");
            c.atk = 48; c.hp = c.maxHP = 170; c.def = 10; c.spd = 14; c.hasWeaponSkill = 1;
            break;

        case CHAR_WELT_YANG:
            strcpy(c.name, "Welt Yang");
            c.atk = 42; c.hp = c.maxHP = 160; c.def = 14; c.spd = 13; c.hasWeaponSkill = 1;
            break;

        case CHAR_DEFAULT: // FIX: CHAR_DEFAULT is correctly handled here
        default:
            strcpy(c.name, "Hero");
            c.atk = 30; c.hp = c.maxHP = 150; c.def = 10; c.spd = 12; c.hasWeaponSkill = 1;
            break;
    }

    c.isBurning = 0;
    c.isStunned = 0;
    
    // Initialize with the default unarmed weapon structure
    c.equippedWeapon = createDefaultWeapon();
    
    return c;
}

// --- Skill implementations remain here ---
void useSkill1(Character *c, Enemy *targets, int targetCount) {
    if (strcmp(c->name, "Saber") == 0) {
        // Avalon: heal 30% of max HP
        int heal = (int)(c->maxHP * 0.30f);
        c->hp += heal;
        if (c->hp > c->maxHP) c->hp = c->maxHP;
        printf("%s used Avalon and healed %d HP (HP: %d/%d)\n", c->name, heal, c->hp, c->maxHP);
        return;
    }

    if (strcmp(c->name, "Ishtar") == 0) {
        // Throw Gems: single target 300% dmg
        int t = pickTarget(targets);
        if (t == -1) return;
        int dmg = calculateDamage(c->atk * 3, targets[t].def);
        targets[t].hp -= dmg;
        printf("%s used Throw Gems on %s for %d damage!\n", c->name, targets[t].name, dmg);
        return;
    }

    if (strcmp(c->name, "Gilgamesh") == 0) {
        // Gate of Babylon: hit all enemies 75% and chance to random debuff (30% stun)
        printf("%s used Gate of Babylon!\n", c->name);
        for (int i = 0; i < targetCount; i++) {
            if (targets[i].hp <= 0) continue;
            int dmg = calculateDamage((int)(c->atk * 0.75f), targets[i].def);
            targets[i].hp -= dmg;
            printf(" - %s took %d damage\n", targets[i].name, dmg);
            int r = rand() % 100;
            if (r < 30) {
                targets[i].isStunned = 1;
                printf("   %s is debuffed (stunned)!\n", targets[i].name);
            }
        }
        return;
    }

    if (strcmp(c->name, "Welt Yang") == 0) {
        // Skill 1: deal 75% to all and decrease enemy speed by 3
        printf("%s used Cutting Gale!\n", c->name);
        for (int i = 0; i < targetCount; i++) {
            if (targets[i].hp <= 0) continue;
            int dmg = calculateDamage((int)(c->atk * 0.75f), targets[i].def);
            targets[i].hp -= dmg;
            targets[i].spd -= 3;
            if (targets[i].spd < 1) targets[i].spd = 1;
            printf(" - %s took %d damage and lost speed (SPD now %d)\n", targets[i].name, dmg, targets[i].spd);
        }
        return;
    }

    // fallback to original per-target power slash
    printf("%s used Power Slash (fallback)!\n", c->name);
    int t = pickTarget(targets);
    if (t == -1) return;
    int dmg = calculateDamage((int)(c->atk * 1.5f), targets[t].def);
    targets[t].hp -= dmg;
    printf("%s dealt %d damage to %s\n", c->name, dmg, targets[t].name);
}

void useSkill2(Character *c, Enemy *targets, int targetCount) {
    if (strcmp(c->name, "Saber") == 0) {
        // Saber Skill 2: AoE 75% (Holy Arc)
        printf("%s used Holy Arc (AOE 75%%)!\n", c->name);
        for (int i = 0; i < targetCount; i++) {
            if (targets[i].hp <= 0) continue;
            int dmg = calculateDamage((int)(c->atk * 0.75f), targets[i].def);
            targets[i].hp -= dmg;
            printf(" - %s took %d damage\n", targets[i].name, dmg);
        }
        return;
    }

    if (strcmp(c->name, "Ishtar") == 0) {
        printf("Ishtar's Skill 2 not implemented yet.\n");
        return;
    }

    if (strcmp(c->name, "Gilgamesh") == 0) {
        // Chains of Heaven: single target 200% and stun
        int t = pickTarget(targets);
        if (t == -1) return;
        int dmg = calculateDamage((int)(c->atk * 2.0f), targets[t].def);
        targets[t].hp -= dmg;
        targets[t].isStunned = 1;
        printf("%s used Chains of Heaven on %s for %d damage and stunned them!\n", c->name, targets[t].name, dmg);
        return;
    }

    if (strcmp(c->name, "Welt Yang") == 0) {
        printf("Welt Yang's Skill 2 not implemented yet.\n");
        return;
    }

    // fallback burning strike
    printf("%s used Burning Strike (fallback)!\n", c->name);
    int t = pickTarget(targets);
    if (t == -1) return;
    targets[t].isBurning = 3;
    printf("%s is now burning!\n", targets[t].name);
}