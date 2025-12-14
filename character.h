#ifndef CHARACTER_H
#define CHARACTER_H

#include "weapon.h"

/* ===== CHARACTER STATUS EFFECTS ===== */
typedef struct {
    /* Damage-over-time */
    int burn_turns;
    int burn_damage_per_turn;

    /* Player buffs */
    int def_buff_turns;
    int def_buff_percent;

    int spd_buff_turns;
    int spd_buff_percent;

    int atk_buff_turns;
    int atk_buff_percent;

    /* Player debuffs */
    int stunned_turns;

    int atk_debuff_turns;
    int atk_debuff_percent;

    int def_debuff_turns;
    int def_debuff_percent;

    int spd_debuff_turns;
    int spd_debuff_percent;

    int slow_turns;
    int slow_percent;

    int healing_reduction_turns;
    int healing_reduction_percent;

    int fire_resist_down_turns;
    int fire_resist_down_percent;
} CharacterStatus;

/* ===== CHARACTER STRUCT ===== */
typedef struct {
    char name[MAX_WEAPON_NAME];
    int id;
    WeaponType allowed_weapon_type;

    int atk, hp, def, spd;
    int equipped_weapon_id;

    CharacterStatus status;
} CombatCharacter;

/* ===== SKILL STRUCT ===== */
typedef struct {
    int id;
    char name[64];
    char description[256];
    int damage_percent;
    int cooldown;
} Skill;

/* ===== CHARACTER FUNCTIONS ===== */
CombatCharacter create_character(int char_id);
int can_equip(CombatCharacter *c, Weapon *w);
void equip_weapon(CombatCharacter *c, int weapon_id);
Skill get_character_skill(int char_id, int skill_num);

#endif
