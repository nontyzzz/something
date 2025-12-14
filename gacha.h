#ifndef GACHA_H
#define GACHA_H

#include "data.h"

/* ===== ITEM STRUCT ===== */
typedef struct {
    char name[MAX_WEAPON_NAME];
    int id;
    int star;
    ItemType type;
} Item;

/* ===== GACHA STRUCTS ===== */
typedef struct {
    char name[MAX_WEAPON_NAME];
    int id;
    Rarity rarity;
    WeaponType type;
    int atk, hp, def, spd;
} Weapon;

typedef struct {
    int gems;
    int char_banner_first;
    int pity_4star;
    int pity_5star;
    int inventory[MAX_INVENTORY];
    int inv_count;
    int highest_stage_cleared;
    int current_character;              /* FIX #5: Track selected character */
    int equipped_weapon[5];             /* FIX #4: Equipped weapon per character (IDs 1-4) */
    int character_level[5];             /* Future: Character levels */
} PlayerData;

/* ===== GACHA POOLS ===== */
extern Weapon weapons_2star[4];
extern Weapon weapons_3star[20];
extern Weapon weapons_4star[8];
extern Weapon weapons_5star[4];

typedef struct {
    char name[MAX_WEAPON_NAME];
    int id;
} Character;

extern Character characters[4];

/* ===== GACHA FUNCTIONS ===== */
void gacha_init(void);
void load_player_data(PlayerData *p);
void save_player_data(const PlayerData *p);
Item pull_single(int banner_type, PlayerData *p);
int has_item(const PlayerData *p, int id);
void banner_menu(PlayerData *p);
int calculate_op(Weapon *w);          /* FIX #7: Centralized OP calculation */

#endif
