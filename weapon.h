#ifndef WEAPON_H
#define WEAPON_H

#include "gacha.h"

/* ===== WEAPON SKILL STRUCT ===== */

typedef struct {
    char name[100];
    char description[300];
    int damage_percent;
    int single_target;
    int stun_chance;
    int burn_chance;
    int burn_damage_percent;
    int burn_duration;
    int atk_debuff_percent;
    int atk_debuff_duration;
    int def_debuff_percent;
    int def_debuff_duration;
    int spd_buff_percent;
    int spd_buff_duration;
    int spd_debuff_percent;
    int spd_debuff_duration;
    int heal_percent;
} WeaponSkill;

/* ===== WEAPON FUNCTIONS ===== */

Weapon* get_weapon_by_id(int id);
WeaponSkill* get_weapon_skill(int weapon_id);

#endif
