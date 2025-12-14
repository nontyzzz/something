#ifndef COMBAT_H
#define COMBAT_H

#include "enemy.h"

typedef struct {
    CombatCharacter player;
    Enemy enemies[MAX_ENEMIES];
    int num_enemies;
    int turn_count;
    int player_current_hp;
    int player_max_hp_modified;
    int player_max_hp_modified_turns;
    int player_guarding;
    int skill1_cooldown;
    int skill2_cooldown;
    int weapon_skill_cooldown;
    int equipped_weapon_id;
    int phoenix_rebirth_used;
    int eternal_warden_judgment_debuffs[MAX_ENEMIES];
} CombatState;

void init_combat(CombatState *state, CombatCharacter *player, Enemy enemies[], int num_enemies);

int calculate_hit_chance(int attacker_spd, int target_spd, Enemy *target_enemy);
int apply_def_reduction(int base_damage, int def);
int get_effective_enemy_damage(Enemy *e, int base_damage);

void apply_damage(CombatState *state, int target_enemy_idx, int damage);
int apply_player_damage(CombatState *state, int damage, Enemy *attacker);  /* MODIFIED: Now returns int */

void apply_burn(Enemy *e, int damage_per_turn, int duration);
void apply_stun_effect(Enemy *e, int duration);
void apply_atk_debuff(Enemy *e, int percent, int duration);
void apply_def_debuff(Enemy *e, int percent, int duration);
void apply_spd_debuff(Enemy *e, int percent, int duration);
void apply_slow_effect(Enemy *e, int percent, int duration);

void apply_atk_buff(Enemy *e, int percent, int duration);
void apply_def_buff(Enemy *e, int percent, int duration);
void apply_spd_buff(Enemy *e, int percent, int duration);

void apply_healing_reduction(Enemy *e, int percent, int duration);
void apply_max_hp_reduction(CombatState *state, int enemy_idx, int percent);
void apply_fire_resistance_down(Enemy *e, int percent, int duration);

void update_combat_turn(CombatState *state);
void display_combat_state(const CombatState *state);

void apply_enemy_skill(CombatState *state, int enemy_idx, int skill_num);
int run_combat_loop(CombatState *state);

#endif
