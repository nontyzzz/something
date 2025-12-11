#ifndef WEAPON_H
#define WEAPON_H

#include <stdio.h>
#include "combat.h" // Needed for the Character struct declaration

#define MAX_WEAPON_INV 100 // Maximum number of weapons a player can hold

// Weapon Rarity
typedef enum {
    WPN_RARITY_3 = 3,
    WPN_RARITY_4 = 4,
    WPN_RARITY_5 = 5
} WeaponRarity;

// Weapon Types
typedef enum {
    WPN_TYPE_SWORD,
    WPN_TYPE_BOW,
    WPN_TYPE_CATALYST,
    WPN_TYPE_DEFAULT
} WeaponType;

// The core Weapon structure
typedef struct {
    char name[50];
    int id;
    int star_level;
    WeaponType type;
    
    // Base stat boosts (flat addition)
    int atk_base_boost; 
    int hp_base_boost; 
    int def_base_boost; 
    
    // Special weapon skill ID (0 if none)
    int skill_id;
} Weapon;

// Inventory structure to hold all weapons
typedef struct {
    Weapon weapons[MAX_WEAPON_INV];
    int count;
} WeaponInventory;

// Function declarations
Weapon createDefaultWeapon(); 
Weapon createWeapon(int id);
int addWeaponToInventory(WeaponInventory *inv, Weapon wpn);
void listWeapons(WeaponInventory *inv);
void displayWeaponStats(Weapon *wpn);

// The primary equip function now correctly uses the Inventory struct
void equipWeapon(Character *c, WeaponInventory *inv);

#endif // WEAPON_H