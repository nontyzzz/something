#ifndef GACHA_H
#define GACHA_H

#include <stdio.h>
#include "weapon.h" // Includes WeaponInventory and MAX_WEAPON_INV

#define MAX_CHARACTER_INV 10 // Maximum number of unique characters a player can hold

typedef struct {
    char name[50];
    int id;
    float rate;
    char rarity[15];
    int star_level;
} Item;

typedef struct {
    int gems;
    int char_banner_first;
    int pity_4star;
    int pity_5star;
    WeaponInventory weaponInv; // NEW: Weapon inventory structure
    int characterInv[MAX_CHARACTER_INV]; // Assuming characters are still stored as IDs
} PlayerData;

void save_player_data(PlayerData *data);
void load_player_data(PlayerData *data);
void banner_menu(PlayerData *data);
void inventory_menu(PlayerData *data); // Declared for main.c visibility

#endif // GACHA_H