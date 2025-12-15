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

/* ===== SKILL DISPLAY STRUCT ===== */
typedef struct {
    int id;
    char name[64];
    char description[256];
    int damage_percent;
    int cooldown;
} Skill;

/* ===== SKILL DATA (COMBAT LOGIC) ===== */
typedef enum {
    SKILL_TARGET_SELF = 0,
    SKILL_TARGET_SINGLE_ENEMY = 1,
    SKILL_TARGET_ALL_ENEMIES = 2
} SkillTargetType;

/*
    NOTE:
    - This struct is intentionally similar in spirit to EnemySkill/WeaponSkill:
      all numeric effect tuning lives in one place (character.c).
    - Any field set to 0 is treated as "no effect".
*/
typedef struct {
    int character_id;       /* matches CombatCharacter.id */
    int skill_num;          /* 1 or 2 */

    char name[64];
    char description[256];

    int cooldown;
    int damage_percent;     /* damage as % of player ATK */
    SkillTargetType target_type;

    /* Self effects */
    int self_heal_percent;          /* % of player max HP */
    int self_atk_buff_percent;
    int self_atk_buff_duration;
    int self_def_buff_percent;
    int self_def_buff_duration;
    int self_spd_buff_percent;
    int self_spd_buff_duration;

    /* Enemy effects (applied only if the hit lands, per target) */
    int stun_chance;
    int stun_duration;

    int burn_chance;
    int burn_damage_percent;        /* % of player ATK per turn */
    int burn_duration;

    int atk_debuff_percent;
    int atk_debuff_duration;

    int def_debuff_percent;
    int def_debuff_duration;

    /* Optional chance-gated SPD debuff (used by Ishtar skill 2) */
    int spd_debuff_chance;
    int spd_debuff_percent;
    int spd_debuff_duration;
} CharacterSkillData;

/* ===== CHARACTER FUNCTIONS ===== */
CombatCharacter create_character(int char_id);
int can_equip(CombatCharacter *c, Weapon *w);
void equip_weapon(CombatCharacter *c, int weapon_id);

/* UI / menu */
Skill get_character_skill(int char_id, int skill_num);

/* Combat logic */
const CharacterSkillData *get_character_skill_data(int char_id, int skill_num);

#endif
