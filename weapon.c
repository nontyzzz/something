#include "weapon.h"
#include "combat.h" // Needed for the Character struct (and saveCharacter)
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Helper function to initialize a default (un-equipped) weapon
Weapon createDefaultWeapon() {
    Weapon wpn;
    strcpy(wpn.name, "Unarmed");
    wpn.id = 0;
    wpn.star_level = 0;
    wpn.type = WPN_TYPE_DEFAULT;
    wpn.atk_base_boost = 0;
    wpn.hp_base_boost = 0;
    wpn.def_base_boost = 0;
    wpn.skill_id = 0;
    return wpn;
}

// Function to create a weapon based on its ID
Weapon createWeapon(int id) {
    Weapon wpn = createDefaultWeapon();

    if (id >= 500 && id < 600) { // 5-Star Weapons
        wpn.star_level = WPN_RARITY_5;
        wpn.id = id;
        if (id == 511) { // Excalibur (Sword)
            strcpy(wpn.name, "Excalibur");
            wpn.type = WPN_TYPE_SWORD;
            wpn.atk_base_boost = 40; 
            wpn.hp_base_boost = 0;
            wpn.def_base_boost = 15;
            wpn.skill_id = 101; 
        } else if (id == 521) { // Maanna (Bow)
            strcpy(wpn.name, "Maanna");
            wpn.type = WPN_TYPE_BOW;
            wpn.atk_base_boost = 45;
            wpn.hp_base_boost = 0;
            wpn.def_base_boost = 0;
            wpn.skill_id = 102;
        }
    } else if (id >= 400 && id < 500) { // 4-Star Weapons
        wpn.star_level = WPN_RARITY_4;
        wpn.id = id;
        if (id == 411) { // Buster Sword (Sword)
            strcpy(wpn.name, "Buster Sword");
            wpn.type = WPN_TYPE_SWORD;
            wpn.atk_base_boost = 25; 
            wpn.hp_base_boost = 10;
        }
    } else if (id >= 300 && id < 400) { // 3-Star Weapons
        wpn.star_level = WPN_RARITY_3;
        wpn.id = id;
        if (id == 311) { // Emberfang (Sword)
            strcpy(wpn.name, "Emberfang");
            wpn.type = WPN_TYPE_SWORD;
            wpn.atk_base_boost = 10;
        }
    }
    
    return wpn;
}

// Adds a weapon to the inventory. Returns the index of the new weapon or -1 if full.
int addWeaponToInventory(WeaponInventory *inv, Weapon wpn) {
    if (inv->count >= MAX_WEAPON_INV) {
        printf("Weapon inventory is full! (%d/%d)\n", inv->count, MAX_WEAPON_INV);
        return -1;
    }

    inv->weapons[inv->count] = wpn;
    inv->count++;
    return inv->count - 1;
}

// Displays the stats of a single weapon
void displayWeaponStats(Weapon *wpn) {
    printf("  [Lvl %d] ATK +%d | HP +%d | DEF +%d", 
           wpn->star_level, wpn->atk_base_boost, wpn->hp_base_boost, wpn->def_base_boost);
    if (wpn->skill_id != 0) {
        printf(" | Skill ID: %d", wpn->skill_id);
    }
    printf("\n");
}

// Lists all weapons in the player's inventory
void listWeapons(WeaponInventory *inv) {
    printf("\n=== Weapon Inventory (%d/%d) ===\n", inv->count, MAX_WEAPON_INV);
    if (inv->count == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    // List Unarmed as option 0
    printf("  0. [0\u2605] Unarmed (ID: 0) [Lvl 0] ATK +0 | HP +0 | DEF +0\n");

    for (int i = 0; i < inv->count; i++) {
        Weapon *wpn = &inv->weapons[i];
        printf("%3d. [%d\u2605] %s (ID: %d) ", 
               i + 1, wpn->star_level, wpn->name, wpn->id);
        displayWeaponStats(wpn);
    }
    printf("================================\n");
}

// Equip Logic
void equipWeapon(Character *c, WeaponInventory *inv) {
    // 1. Unequip old weapon stats
    c->atk -= c->equippedWeapon.atk_base_boost;
    c->hp -= c->equippedWeapon.hp_base_boost;
    c->def -= c->equippedWeapon.def_base_boost;
    c->maxHP -= c->equippedWeapon.hp_base_boost;

    listWeapons(inv);
    
    int choice;
    printf("Enter weapon number to equip (1-%d) or 0 for Unarmed: ", inv->count);
    if (scanf("%d", &choice) != 1) { 
        while(getchar() != '\n'); 
        printf("Invalid input. Cancelling equip.\n"); 
        
        // Re-apply original weapon stats
        c->atk += c->equippedWeapon.atk_base_boost;
        c->hp += c->equippedWeapon.hp_base_boost;
        c->def += c->equippedWeapon.def_base_boost;
        c->maxHP += c->equippedWeapon.hp_base_boost;
        return;
    }
    while(getchar() != '\n');

    if (choice < 0 || choice > inv->count) {
        printf("Invalid choice. Cancelling equip.\n");
        
        // Re-apply original weapon stats
        c->atk += c->equippedWeapon.atk_base_boost;
        c->hp += c->equippedWeapon.hp_base_boost;
        c->def += c->equippedWeapon.def_base_boost;
        c->maxHP += c->equippedWeapon.hp_base_boost;
        return;
    }

    Weapon newWeapon;

    if (choice == 0) {
        newWeapon = createDefaultWeapon();
        printf("\nUnarmed equipped.\n");
    } else {
        newWeapon = inv->weapons[choice - 1];
    }
    
    // 2. Equip the new weapon
    c->equippedWeapon = newWeapon;
    
    // 3. Apply new weapon stats
    c->atk += newWeapon.atk_base_boost;
    c->hp += newWeapon.hp_base_boost;
    c->def += newWeapon.def_base_boost;
    c->maxHP += newWeapon.hp_base_boost; 

    // 4. Ensure current HP doesn't exceed maxHP
    if (c->hp > c->maxHP) c->hp = c->maxHP;

    printf("\nEquipped %s! %s's new stats (after boost):\n  ATK: %d, HP: %d/%d, DEF: %d, SPD: %d\n", 
           newWeapon.name, c->name, c->atk, c->hp, c->maxHP, c->def, c->spd);

    // Save the updated character stats (important for persistence)
    saveCharacter(c);
}