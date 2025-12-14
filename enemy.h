#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

/* ===== ENEMY STATUS EFFECTS ===== */
typedef struct {
    int burn_turns;
    int burn_damage_per_turn;
    int stunned_turns;
    int atk_debuff_turns;
    int atk_debuff_percent;
    int def_debuff_turns;
    int def_debuff_percent;
    int spd_debuff_turns;
    int spd_debuff_percent;
    
    /* NEW: Enemy offensive buffs */
    int atk_buff_turns;
    int atk_buff_percent;
    int def_buff_turns;
    int def_buff_percent;
    int spd_buff_turns;
    int spd_buff_percent;
    
    /* NEW: Debuffs specific to skills */
    int slow_turns;
    int slow_percent;
    int max_hp_reduction_turns;
    int max_hp_reduction_percent;
    int healing_reduction_turns;
    int healing_reduction_percent;
    int fire_resist_down_turns;
    int fire_resist_down_percent;
} EnemyStatus;

/* ===== ENEMY STRUCT ===== */
typedef struct {
    EnemyType type;
    char name[MAX_ENEMY_NAME];
    int atk, hp, max_hp, def, spd;
    int current_hp;
    int turn_count;
    EnemyStatus status;
} Enemy;

/* ===== ENEMY SKILL STRUCT ===== */
typedef struct {
    EnemyType enemy_type;
    int skill_num;
    char name[64];
    char description[256];
    int damage_percent;
    int stun_chance;
    int stun_duration;
    int burn_chance;
    int burn_damage_percent;
    int burn_duration;
    int slow_chance;
    int slow_percent;
    int slow_duration;
    int atk_debuff_percent;
    int atk_debuff_duration;
    int def_debuff_percent;
    int def_debuff_duration;
    int spd_debuff_percent;
    int spd_debuff_duration;
    int atk_buff_percent;
    int atk_buff_duration;
    int def_buff_percent;
    int def_buff_duration;
    int spd_buff_percent;
    int spd_buff_duration;
    int heal_percent;
    int drain_percent;
    int max_hp_reduction_percent;
    int healing_reduction_percent;
    int healing_reduction_duration;
    int fire_resist_down_percent;
    int fire_resist_down_duration;
} EnemySkill;

/* ===== ENEMY FUNCTIONS ===== */
Enemy create_enemy(EnemyType type);
int enemy_get_action(Enemy *e);
EnemySkill* get_enemy_skill(EnemyType type, int skill_num);

#endif