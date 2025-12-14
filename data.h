#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* ===== CONSTANTS ===== */

#define MAX_INVENTORY 500
#define MAX_ENEMIES 3
#define MAX_SKILLS 3
#define MAX_WEAPON_NAME 64
#define MAX_ENEMY_NAME 64

/* Gacha constants */
#define GEM_COST_SINGLE 100
#define GEM_COST_TEN 1000
#define GEM_REFUND_DUPLICATE 50

/* Combat constants */
#define BURN_DAMAGE_PERCENT 40
#define HIT_BASE_CHANCE 85
#define SPD_HIT_FACTOR 0.5f
#define DEF_REDUCTION_FACTOR 0.5f
#define NORMAL_ATTACK_PERCENT 50
#define GUARD_DAMAGE_REDUCTION 35

/* ===== ENUMS ===== */

typedef enum {
    RARITY_2STAR = 2,
    RARITY_3STAR = 3,
    RARITY_4STAR = 4,
    RARITY_5STAR = 5
} Rarity;

typedef enum {
    TYPE_SWORD = 0,
    TYPE_BOW = 1,
    TYPE_SPEAR = 2,
    TYPE_STAFF = 3
} WeaponType;

typedef enum {
    ITEM_WEAPON = 0,
    ITEM_CHARACTER = 1
} ItemType;

typedef enum {
    STATUS_NONE = 0,
    STATUS_BURN = 1,
    STATUS_STUN = 2,
    STATUS_ATK_BUFF = 3,
    STATUS_ATK_DEBUFF = 4,
    STATUS_DEF_BUFF = 5,
    STATUS_DEF_DEBUFF = 6,
    STATUS_SPD_BUFF = 7,
    STATUS_SPD_DEBUFF = 8,
    STATUS_SLOW = 9
} StatusType;

typedef enum {
    ENEMY_SLIME = 0,
    ENEMY_BANDIT = 1,
    ENEMY_BEAST = 2,
    ENEMY_GORGON = 3,
    ENEMY_WRAITH = 4,
    BOSS_SHADOW_KNIGHT = 5,
    BOSS_PHOENIX_LORD = 6,
    BOSS_ETERNAL_WARDEN = 7
} EnemyType;

/* ===== UTILITY FUNCTIONS ===== */

int clamp(int val, int min, int max);
int random_range(int min, int max);
int random_percent(int percent);

#endif
