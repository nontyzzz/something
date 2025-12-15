#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "combat.h"
#include "weapon.h"
#include "data.h"

#ifndef WEAPON_SKILL_COOLDOWN
#define WEAPON_SKILL_COOLDOWN 4
#endif

/* ===== INTERNAL: PLAYER EFFECTIVE STATS ===== */
static int get_effective_player_spd(const CombatState *state) {
    int spd = state->player.spd;

    if (state->player.status.spd_debuff_turns > 0) {
        spd = (spd * (100 - state->player.status.spd_debuff_percent)) / 100;
    }
    if (state->player.status.spd_buff_turns > 0) {
        spd = (spd * (100 + state->player.status.spd_buff_percent)) / 100;
    }
    if (state->player.status.slow_turns > 0) {
        spd = (spd * (100 - state->player.status.slow_percent)) / 100;
    }

    if (spd < 1) spd = 1;
    return spd;
}

static int get_effective_player_def(const CombatState *state) {
    int def = state->player.def;

    if (state->player.status.def_debuff_turns > 0) {
        def = (def * (100 - state->player.status.def_debuff_percent)) / 100;
    }
    if (state->player.status.def_buff_turns > 0) {
        def = (def * (100 + state->player.status.def_buff_percent)) / 100;
    }

    if (def < 0) def = 0;
    return def;
}

static int get_effective_enemy_spd(const Enemy *e) {
    int spd = e->spd;

    if (e->status.spd_debuff_turns > 0) {
        spd = (spd * (100 - e->status.spd_debuff_percent)) / 100;
    }
    if (e->status.spd_buff_turns > 0) {
        spd = (spd * (100 + e->status.spd_buff_percent)) / 100;
    }
    if (e->status.slow_turns > 0) {
        spd = (spd * (100 - e->status.slow_percent)) / 100;
    }

    if (spd < 1) spd = 1;
    return spd;
}

static int apply_player_atk_modifiers_to_damage(const CombatState *state, int damage) {
    if (state->player.status.atk_debuff_turns > 0) {
        damage = (damage * (100 - state->player.status.atk_debuff_percent)) / 100;
    }
    if (state->player.status.atk_buff_turns > 0) {
        damage = (damage * (100 + state->player.status.atk_buff_percent)) / 100;
    }
    if (damage < 0) damage = 0;
    return damage;
}

/* ===== DISPLAY HELPERS ===== */
static void print_player_effects(const CombatCharacter *player) {
    if (player->status.burn_turns > 0) {
        printf(" | Burn(%dt)", player->status.burn_turns);
    }
    if (player->status.stunned_turns > 0) {
        printf(" | Stun(%dt)", player->status.stunned_turns);
    }
    if (player->status.atk_debuff_turns > 0) {
        printf(" | ATK-%d%%(%dt)", player->status.atk_debuff_percent, player->status.atk_debuff_turns);
    }
    if (player->status.def_debuff_turns > 0) {
        printf(" | DEF-%d%%(%dt)", player->status.def_debuff_percent, player->status.def_debuff_turns);
    }
    if (player->status.spd_debuff_turns > 0) {
        printf(" | SPD-%d%%(%dt)", player->status.spd_debuff_percent, player->status.spd_debuff_turns);
    }
    if (player->status.atk_buff_turns > 0) {
        printf(" | ATK+%d%%(%dt)", player->status.atk_buff_percent, player->status.atk_buff_turns);
    }
    if (player->status.def_buff_turns > 0) {
        printf(" | DEF+%d%%(%dt)", player->status.def_buff_percent, player->status.def_buff_turns);
    }
    if (player->status.spd_buff_turns > 0) {
        printf(" | SPD+%d%%(%dt)", player->status.spd_buff_percent, player->status.spd_buff_turns);
    }
    if (player->status.slow_turns > 0) {
        printf(" | Slow(%dt)", player->status.slow_turns);
    }
    if (player->status.healing_reduction_turns > 0) {
        printf(" | Heal-%d%%(%dt)", player->status.healing_reduction_percent, player->status.healing_reduction_turns);
    }
    if (player->status.fire_resist_down_turns > 0) {
        printf(" | FireResDown-%d%%(%dt)", player->status.fire_resist_down_percent, player->status.fire_resist_down_turns);
    }
}

static void print_enemy_effects(const Enemy *enemy) {
    if (enemy->status.burn_turns > 0) {
        printf(" | Burn(%dt)", enemy->status.burn_turns);
    }
    if (enemy->status.stunned_turns > 0) {
        printf(" | Stun(%dt)", enemy->status.stunned_turns);
    }
    if (enemy->status.atk_debuff_turns > 0) {
        printf(" | ATK-%d%%(%dt)", enemy->status.atk_debuff_percent, enemy->status.atk_debuff_turns);
    }
    if (enemy->status.def_debuff_turns > 0) {
        printf(" | DEF-%d%%(%dt)", enemy->status.def_debuff_percent, enemy->status.def_debuff_turns);
    }
    if (enemy->status.spd_debuff_turns > 0) {
        printf(" | SPD-%d%%(%dt)", enemy->status.spd_debuff_percent, enemy->status.spd_debuff_turns);
    }
    if (enemy->status.atk_buff_turns > 0) {
        printf(" | ATK+%d%%(%dt)", enemy->status.atk_buff_percent, enemy->status.atk_buff_turns);
    }
    if (enemy->status.def_buff_turns > 0) {
        printf(" | DEF+%d%%(%dt)", enemy->status.def_buff_percent, enemy->status.def_buff_turns);
    }
    if (enemy->status.spd_buff_turns > 0) {
        printf(" | SPD+%d%%(%dt)", enemy->status.spd_buff_percent, enemy->status.spd_buff_turns);
    }
    if (enemy->status.slow_turns > 0) {
        printf(" | Slow(%dt)", enemy->status.slow_turns);
    }
    if (enemy->status.healing_reduction_turns > 0) {
        printf(" | Heal-%d%%(%dt)", enemy->status.healing_reduction_percent, enemy->status.healing_reduction_turns);
    }
    if (enemy->status.fire_resist_down_turns > 0) {
        printf(" | FireResDown-%d%%(%dt)", enemy->status.fire_resist_down_percent, enemy->status.fire_resist_down_turns);
    }
}

/* ===== INITIALIZATION ===== */
void init_combat(CombatState *state, CombatCharacter *player, Enemy enemies[], int num_enemies) {
    state->player = *player;
    state->player_current_hp = player->hp;

    state->player_max_hp_modified = 0;
    state->player_max_hp_modified_turns = 0;

    state->num_enemies = clamp(num_enemies, 1, MAX_ENEMIES);
    for (int i = 0; i < state->num_enemies; i++) {
        state->enemies[i] = enemies[i];
        state->eternal_warden_judgment_debuffs[i] = 0;
    }

    state->turn_count = 1;
    state->player_guarding = 0;

    state->skill1_cooldown = 0;
    state->skill2_cooldown = 0;
    state->weapon_skill_cooldown = 0;

    state->phoenix_rebirth_used = 0;
}

/* ===== HIT CHANCE CALCULATION ===== */
int calculate_hit_chance(int attacker_spd, int target_spd, Enemy *target_enemy) {
    int effective_target_spd = target_spd;

    if (target_enemy != NULL && target_enemy->status.spd_debuff_turns > 0) {
        effective_target_spd = (effective_target_spd * (100 - target_enemy->status.spd_debuff_percent)) / 100;
    }
    if (target_enemy != NULL && target_enemy->status.spd_buff_turns > 0) {
        effective_target_spd = (effective_target_spd * (100 + target_enemy->status.spd_buff_percent)) / 100;
    }
    if (target_enemy != NULL && target_enemy->status.slow_turns > 0) {
        effective_target_spd = (effective_target_spd * (100 - target_enemy->status.slow_percent)) / 100;
    }

    int chance = HIT_BASE_CHANCE + (int)((attacker_spd - effective_target_spd) * SPD_HIT_FACTOR);
    return clamp(chance, 0, 100);
}

static int calculate_enemy_hit_chance(int enemy_spd, const CombatState *state) {
    int attacker_spd = enemy_spd;
    int target_spd = get_effective_player_spd(state);

    int chance = HIT_BASE_CHANCE + (int)((attacker_spd - target_spd) * SPD_HIT_FACTOR);
    return clamp(chance, 0, 100);
}

/* ===== DEFENSE REDUCTION ===== */
int apply_def_reduction(int base_damage, int def) {
    int denominator = 100 + (int)(def * DEF_REDUCTION_FACTOR);
    return (base_damage * 100) / denominator;
}

int get_effective_enemy_damage(Enemy *e, int base_damage_percent) {
    int atk = e->atk;

    if (e->status.atk_debuff_turns > 0) {
        atk = (atk * (100 - e->status.atk_debuff_percent)) / 100;
    }
    if (e->status.atk_buff_turns > 0) {
        atk = (atk * (100 + e->status.atk_buff_percent)) / 100;
    }

    return (atk * base_damage_percent) / 100;
}

/* ===== DAMAGE APPLICATION ===== */
/* MODIFIED: Now returns 1 if hit, 0 if missed */
int apply_damage(CombatState *state, int target_enemy_idx, int damage) {
    if (target_enemy_idx < 0 || target_enemy_idx >= state->num_enemies) return 0;  /* Invalid target */

    Enemy *e = &state->enemies[target_enemy_idx];

    /* Do not attack dead enemies */
    if (e->current_hp <= 0) return 0;

    damage = apply_player_atk_modifiers_to_damage(state, damage);

    int attacker_spd = get_effective_player_spd(state);
    int hit_chance = calculate_hit_chance(attacker_spd, e->spd, e);

    if (!random_percent(hit_chance)) {
        printf("%s MISSED!\n", state->player.name);
        return 0;  /* MISSED */
    }

    int enemy_def = e->def;

    if (e->status.def_debuff_turns > 0) {
        enemy_def = (enemy_def * (100 - e->status.def_debuff_percent)) / 100;
    }
    if (e->status.def_buff_turns > 0) {
        enemy_def = (enemy_def * (100 + e->status.def_buff_percent)) / 100;
    }

    int reduced_damage = apply_def_reduction(damage, enemy_def);

    e->current_hp -= reduced_damage;
    if (e->current_hp < 0) e->current_hp = 0;

    printf("%s takes %d damage! (HP: %d/%d)\n", e->name, reduced_damage, e->current_hp, e->max_hp);
    return 1;  /* HIT */
}

/* MODIFIED: Now returns int (1 = hit, 0 = missed) */
int apply_player_damage(CombatState *state, int damage, Enemy *attacker) {
    /* Hit chance check for enemy attacks */
    if (attacker != NULL) {
        int enemy_spd = get_effective_enemy_spd(attacker);
        int hit_chance = calculate_enemy_hit_chance(enemy_spd, state);

        if (!random_percent(hit_chance)) {
            printf("%s MISSED!\n", attacker->name);
            return 0;  /* MISSED */
        }
    }

    if (state->player_guarding) {
        damage = (damage * (100 - GUARD_DAMAGE_REDUCTION)) / 100;
        printf("Guard! Reduced damage to %d\n", damage);
    }

    int player_def = get_effective_player_def(state);
    int reduced_damage = apply_def_reduction(damage, player_def);

    state->player_current_hp -= reduced_damage;
    if (state->player_current_hp < 0) state->player_current_hp = 0;

    int effective_max_hp = state->player.hp;

    if (state->player_max_hp_modified_turns > 0 && state->player_max_hp_modified > 0) {
        effective_max_hp = (effective_max_hp * (100 - state->player_max_hp_modified)) / 100;
    }

    for (int i = 0; i < state->num_enemies; i++) {
        if (state->eternal_warden_judgment_debuffs[i] > 0) {
            effective_max_hp = (effective_max_hp * (100 - state->eternal_warden_judgment_debuffs[i])) / 100;
            break;
        }
    }

    printf("%s takes %d damage! (HP: %d/%d)\n",
           state->player.name, reduced_damage, state->player_current_hp, effective_max_hp);
    
    return 1;  /* HIT */
}

/* ===== STATUS EFFECT APPLICATION (ENEMY TARGET) ===== */
void apply_burn(Enemy *e, int damage_per_turn, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.burn_turns = duration;
    e->status.burn_damage_per_turn = damage_per_turn;

    if (e->status.fire_resist_down_turns > 0) {
        e->status.burn_damage_per_turn =
            (damage_per_turn * (100 + e->status.fire_resist_down_percent)) / 100;

        printf("%s is burned! (%d damage/turn for %d turns) [Fire Resist Down Active!]\n",
               e->name, e->status.burn_damage_per_turn, duration);
    } else {
        printf("%s is burned! (%d damage/turn for %d turns)\n", e->name, damage_per_turn, duration);
    }
}

void apply_stun_effect(Enemy *e, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.stunned_turns = duration;
    printf("%s is stunned for %d turn(s)!\n", e->name, duration);
}

void apply_atk_debuff(Enemy *e, int percent, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.atk_debuff_turns = duration;
    e->status.atk_debuff_percent = percent;
    printf("%s ATK reduced by %d%% for %d turns!\n", e->name, percent, duration);
}

void apply_def_debuff(Enemy *e, int percent, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.def_debuff_turns = duration;
    e->status.def_debuff_percent = percent;
    printf("%s DEF reduced by %d%% for %d turns!\n", e->name, percent, duration);
}

void apply_spd_debuff(Enemy *e, int percent, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.spd_debuff_turns = duration;
    e->status.spd_debuff_percent = percent;
    printf("%s SPD reduced by %d%% for %d turns!\n", e->name, percent, duration);
}

void apply_slow_effect(Enemy *e, int percent, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.slow_turns = duration;
    e->status.slow_percent = percent;
    printf("%s is slowed by %d%% for %d turns!\n", e->name, percent, duration);
}

void apply_atk_buff(Enemy *e, int percent, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.atk_buff_turns = duration;
    e->status.atk_buff_percent = percent;
    printf("%s ATK increased by %d%% for %d turns!\n", e->name, percent, duration);
}

void apply_def_buff(Enemy *e, int percent, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.def_buff_turns = duration;
    e->status.def_buff_percent = percent;
    printf("%s DEF increased by %d%% for %d turns!\n", e->name, percent, duration);
}

void apply_spd_buff(Enemy *e, int percent, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.spd_buff_turns = duration;
    e->status.spd_buff_percent = percent;
    printf("%s SPD increased by %d%% for %d turns!\n", e->name, percent, duration);
}

void apply_healing_reduction(Enemy *e, int percent, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.healing_reduction_turns = duration;
    e->status.healing_reduction_percent = percent;
    printf("%s healing effectiveness reduced by %d%% for %d turns!\n", e->name, percent, duration);
}

void apply_max_hp_reduction(CombatState *state, int enemy_idx, int percent) {
    if (enemy_idx < 0 || enemy_idx >= state->num_enemies) return;
    state->eternal_warden_judgment_debuffs[enemy_idx] = percent;
    printf("Player max HP permanently reduced by %d%% for this fight!\n", percent);
}

void apply_fire_resistance_down(Enemy *e, int percent, int duration) {

    if (e == NULL || e->current_hp <= 0) return;

    e->status.fire_resist_down_turns = duration;
    e->status.fire_resist_down_percent = percent;
    printf("%s fire resistance reduced by %d%% for %d turns!\n", e->name, percent, duration);
}

/* ===== STATUS EFFECT APPLICATION (PLAYER TARGET) ===== */
static void apply_player_stun(CombatState *state, int duration) {
    state->player.status.stunned_turns = duration;
    printf("%s is stunned for %d turn(s)!\n", state->player.name, duration);
}

static void apply_player_burn(CombatState *state, int damage_per_turn, int duration) {
    state->player.status.burn_turns = duration;
    state->player.status.burn_damage_per_turn = damage_per_turn;

    if (state->player.status.fire_resist_down_turns > 0) {
        state->player.status.burn_damage_per_turn =
            (damage_per_turn * (100 + state->player.status.fire_resist_down_percent)) / 100;

        printf("%s is burned! (%d damage/turn for %d turns) [Fire Resist Down Active!]\n",
               state->player.name, state->player.status.burn_damage_per_turn, duration);
    } else {
        printf("%s is burned! (%d damage/turn for %d turns)\n",
               state->player.name, damage_per_turn, duration);
    }
}

static void apply_player_slow(CombatState *state, int percent, int duration) {
    state->player.status.slow_turns = duration;
    state->player.status.slow_percent = percent;
    printf("%s is slowed by %d%% for %d turns!\n", state->player.name, percent, duration);
}

static void apply_player_atk_debuff(CombatState *state, int percent, int duration) {
    state->player.status.atk_debuff_turns = duration;
    state->player.status.atk_debuff_percent = percent;
    printf("%s ATK reduced by %d%% for %d turns!\n", state->player.name, percent, duration);
}

static void apply_player_def_debuff(CombatState *state, int percent, int duration) {
    state->player.status.def_debuff_turns = duration;
    state->player.status.def_debuff_percent = percent;
    printf("%s DEF reduced by %d%% for %d turns!\n", state->player.name, percent, duration);
}

static void apply_player_spd_debuff(CombatState *state, int percent, int duration) {
    state->player.status.spd_debuff_turns = duration;
    state->player.status.spd_debuff_percent = percent;
    printf("%s SPD reduced by %d%% for %d turns!\n", state->player.name, percent, duration);
}

static void apply_player_healing_reduction(CombatState *state, int percent, int duration) {
    state->player.status.healing_reduction_turns = duration;
    state->player.status.healing_reduction_percent = percent;
    printf("%s healing effectiveness reduced by %d%% for %d turns!\n", state->player.name, percent, duration);
}

static void apply_player_fire_resistance_down(CombatState *state, int percent, int duration) {
    state->player.status.fire_resist_down_turns = duration;
    state->player.status.fire_resist_down_percent = percent;

    if (state->player.status.burn_turns > 0) {
        state->player.status.burn_damage_per_turn =
            (state->player.status.burn_damage_per_turn * (100 + percent)) / 100;
    }

    printf("%s fire resistance reduced by %d%% for %d turns!\n", state->player.name, percent, duration);
}

/* ===== PLAYER BUFF/HEAL HELPERS ===== */
static void apply_player_atk_buff(CombatState *state, int percent, int duration) {
    state->player.status.atk_buff_turns = duration;
    state->player.status.atk_buff_percent = percent;
    printf("%s ATK increased by %d%% for %d turns!\n", state->player.name, percent, duration);
}

static void apply_player_def_buff(CombatState *state, int percent, int duration) {
    state->player.status.def_buff_turns = duration;
    state->player.status.def_buff_percent = percent;
    printf("%s DEF increased by %d%% for %d turns!\n", state->player.name, percent, duration);
}

static void apply_player_spd_buff(CombatState *state, int percent, int duration) {
    state->player.status.spd_buff_turns = duration;
    state->player.status.spd_buff_percent = percent;
    printf("%s SPD increased by %d%% for %d turns!\n", state->player.name, percent, duration);
}

static void heal_player_percent(CombatState *state, int percent) {
    int heal = (state->player.hp * percent) / 100;

    if (state->player.status.healing_reduction_turns > 0) {
        heal = (heal * (100 - state->player.status.healing_reduction_percent)) / 100;
    }

    state->player_current_hp += heal;
    if (state->player_current_hp > state->player.hp) state->player_current_hp = state->player.hp;

    printf("%s heals for %d HP! (HP: %d/%d)\n", state->player.name, heal, state->player_current_hp, state->player.hp);
}

/* ===== CHARACTER SKILL APPLICATION ===== */
static void apply_character_skill(CombatState *state, int skill_num) {
    const CharacterSkillData *sd = get_character_skill_data(state->player.id, skill_num);
    if (sd == NULL) {
        printf("Unknown character skill!\n");
        return;
    }

    /* Self effects always apply */
    if (sd->self_heal_percent > 0) {
        heal_player_percent(state, sd->self_heal_percent);
    }
    if (sd->self_atk_buff_percent > 0 && sd->self_atk_buff_duration > 0) {
        apply_player_atk_buff(state, sd->self_atk_buff_percent, sd->self_atk_buff_duration);
    }
    if (sd->self_def_buff_percent > 0 && sd->self_def_buff_duration > 0) {
        apply_player_def_buff(state, sd->self_def_buff_percent, sd->self_def_buff_duration);
    }
    if (sd->self_spd_buff_percent > 0 && sd->self_spd_buff_duration > 0) {
        apply_player_spd_buff(state, sd->self_spd_buff_percent, sd->self_spd_buff_duration);
    }

    /* No damage / no enemy effects */
    if (sd->damage_percent <= 0) return;

    int dmg = (state->player.atk * sd->damage_percent) / 100;

    if (sd->target_type == SKILL_TARGET_SINGLE_ENEMY) {
        int target = 0;
        if (state->num_enemies > 1) {
            printf("Target enemy (0-%d): ", state->num_enemies - 1);
            if (scanf("%d", &target) != 1) {
                while (getchar() != '\n') {}
                return;
            }
        }
        target = clamp(target, 0, state->num_enemies - 1);

        int hit = apply_damage(state, target, dmg);
        if (!hit) return;

        Enemy *e = &state->enemies[target];

        /* Stun */
        if (sd->stun_chance > 0 && sd->stun_duration > 0 && random_percent(sd->stun_chance)) {
            apply_stun_effect(e, sd->stun_duration);
        }

        /* Burn */
        if (sd->burn_chance > 0 && sd->burn_damage_percent > 0 && sd->burn_duration > 0 && random_percent(sd->burn_chance)) {
            int burn = (state->player.atk * sd->burn_damage_percent) / 100;
            apply_burn(e, burn, sd->burn_duration);
        }

        /* Debuffs */
        if (sd->atk_debuff_percent > 0 && sd->atk_debuff_duration > 0) {
            apply_atk_debuff(e, sd->atk_debuff_percent, sd->atk_debuff_duration);
        }
        if (sd->def_debuff_percent > 0 && sd->def_debuff_duration > 0) {
            apply_def_debuff(e, sd->def_debuff_percent, sd->def_debuff_duration);
        }
        if (sd->spd_debuff_percent > 0 && sd->spd_debuff_duration > 0) {
            if (sd->spd_debuff_chance > 0) {
                if (random_percent(sd->spd_debuff_chance)) {
                    apply_spd_debuff(e, sd->spd_debuff_percent, sd->spd_debuff_duration);
                }
            } else {
                apply_spd_debuff(e, sd->spd_debuff_percent, sd->spd_debuff_duration);
            }
        }

    } else if (sd->target_type == SKILL_TARGET_ALL_ENEMIES) {
        for (int i = 0; i < state->num_enemies; i++) {
            int hit = apply_damage(state, i, dmg);
            if (!hit) continue;

            Enemy *e = &state->enemies[i];

            /* Stun (per target) */
            if (sd->stun_chance > 0 && sd->stun_duration > 0 && random_percent(sd->stun_chance)) {
                apply_stun_effect(e, sd->stun_duration);
            }

            /* Burn (per target) */
            if (sd->burn_chance > 0 && sd->burn_damage_percent > 0 && sd->burn_duration > 0 && random_percent(sd->burn_chance)) {
                int burn = (state->player.atk * sd->burn_damage_percent) / 100;
                apply_burn(e, burn, sd->burn_duration);
            }

            /* Debuffs (per target) */
            if (sd->atk_debuff_percent > 0 && sd->atk_debuff_duration > 0) {
                apply_atk_debuff(e, sd->atk_debuff_percent, sd->atk_debuff_duration);
            }
            if (sd->def_debuff_percent > 0 && sd->def_debuff_duration > 0) {
                apply_def_debuff(e, sd->def_debuff_percent, sd->def_debuff_duration);
            }
            if (sd->spd_debuff_percent > 0 && sd->spd_debuff_duration > 0) {
                if (sd->spd_debuff_chance > 0) {
                    if (random_percent(sd->spd_debuff_chance)) {
                        apply_spd_debuff(e, sd->spd_debuff_percent, sd->spd_debuff_duration);
                    }
                } else {
                    apply_spd_debuff(e, sd->spd_debuff_percent, sd->spd_debuff_duration);
                }
            }
        }
    }
}



/* ===== TURN UPDATE ===== */
void update_combat_turn(CombatState *state) {
    state->player_guarding = 0;

    if (state->player.status.burn_turns > 0) {
        state->player_current_hp -= state->player.status.burn_damage_per_turn;
        if (state->player_current_hp < 0) state->player_current_hp = 0;

        state->player.status.burn_turns--;

        printf("%s takes %d burn damage! (HP: %d/%d)\n",
               state->player.name,
               state->player.status.burn_damage_per_turn,
               state->player_current_hp,
               state->player.hp);
    }

    if (state->player.status.stunned_turns > 0) state->player.status.stunned_turns--;
    if (state->player.status.atk_debuff_turns > 0) state->player.status.atk_debuff_turns--;
    if (state->player.status.def_debuff_turns > 0) state->player.status.def_debuff_turns--;
    if (state->player.status.spd_debuff_turns > 0) state->player.status.spd_debuff_turns--;
    if (state->player.status.slow_turns > 0) state->player.status.slow_turns--;
    if (state->player.status.healing_reduction_turns > 0) state->player.status.healing_reduction_turns--;
    if (state->player.status.fire_resist_down_turns > 0) state->player.status.fire_resist_down_turns--;

    if (state->player.status.atk_buff_turns > 0) state->player.status.atk_buff_turns--;
    if (state->player.status.def_buff_turns > 0) state->player.status.def_buff_turns--;
    if (state->player.status.spd_buff_turns > 0) state->player.status.spd_buff_turns--;

    if (state->player_max_hp_modified_turns > 0) {
        state->player_max_hp_modified_turns--;
        if (state->player_max_hp_modified_turns == 0) {
            state->player_max_hp_modified = 0;
        }
    }

    for (int i = 0; i < state->num_enemies; i++) {
        Enemy *e = &state->enemies[i];

        if (e->current_hp <= 0) continue;

        if (e->status.burn_turns > 0) {
            e->current_hp -= e->status.burn_damage_per_turn;
            if (e->current_hp < 0) e->current_hp = 0;

            e->status.burn_turns--;

            printf("%s takes %d burn damage! (HP: %d/%d)\n",
                   e->name,
                   e->status.burn_damage_per_turn,
                   e->current_hp,
                   e->max_hp);
        }

        if (e->status.stunned_turns > 0) e->status.stunned_turns--;

        if (e->status.atk_debuff_turns > 0) e->status.atk_debuff_turns--;
        if (e->status.def_debuff_turns > 0) e->status.def_debuff_turns--;
        if (e->status.spd_debuff_turns > 0) e->status.spd_debuff_turns--;

        if (e->status.atk_buff_turns > 0) e->status.atk_buff_turns--;
        if (e->status.def_buff_turns > 0) e->status.def_buff_turns--;
        if (e->status.spd_buff_turns > 0) e->status.spd_buff_turns--;

        if (e->status.slow_turns > 0) e->status.slow_turns--;

        if (e->status.healing_reduction_turns > 0) e->status.healing_reduction_turns--;
        if (e->status.fire_resist_down_turns > 0) e->status.fire_resist_down_turns--;
    }

    if (state->skill1_cooldown > 0) state->skill1_cooldown--;
    if (state->skill2_cooldown > 0) state->skill2_cooldown--;
    if (state->weapon_skill_cooldown > 0) state->weapon_skill_cooldown--;

    state->turn_count++;
}

/* ===== DISPLAY COMBAT STATE ===== */
void display_combat_state(const CombatState *state) {
    printf("\n===== TURN %d =====\n", state->turn_count);

    int effective_max_hp = state->player.hp;

    if (state->player_max_hp_modified_turns > 0 && state->player_max_hp_modified > 0) {
        effective_max_hp = (effective_max_hp * (100 - state->player_max_hp_modified)) / 100;
    }

    for (int i = 0; i < state->num_enemies; i++) {
        if (state->eternal_warden_judgment_debuffs[i] > 0) {
            effective_max_hp = (effective_max_hp * (100 - state->eternal_warden_judgment_debuffs[i])) / 100;
            break;
        }
    }

    printf("%s (HP: %d/%d)", state->player.name, state->player_current_hp, effective_max_hp);
    print_player_effects(&state->player);
    printf("\n");

    for (int i = 0; i < state->num_enemies; i++) {
        printf("Enemy %d: %s (HP: %d/%d)",
               i,
               state->enemies[i].name,
               state->enemies[i].current_hp,
               state->enemies[i].max_hp);
        print_enemy_effects(&state->enemies[i]);
        printf("\n");
    }

    printf("=====================\n\n");
}

/* ===== ENEMY SKILL APPLICATION ===== */
/* MODIFIED: Check if attack hit before applying status effects */
void apply_enemy_skill(CombatState *state, int enemy_idx, int skill_num) {
    if (enemy_idx < 0 || enemy_idx >= state->num_enemies) return;

    Enemy *e = &state->enemies[enemy_idx];
    EnemySkill *skill = get_enemy_skill(e->type, skill_num);

    if (skill == NULL) {
        printf("%s attempted unknown skill!\n", e->name);
        return;
    }

    printf("%s uses %s!\n", e->name, skill->name);

    /* Damage and hit check */
    int damage = 0;
    int attack_hit = 1;  /* Assume hit if no damage component */
    
    if (skill->damage_percent > 0) {
        damage = get_effective_enemy_damage(e, skill->damage_percent);
        attack_hit = apply_player_damage(state, damage, e);  /* Returns 1 if hit, 0 if miss */
    }

    /* ONLY APPLY EFFECTS IF ATTACK HIT */
    if (!attack_hit) {
        printf("All effects avoided!\n");
        return;  /* Exit early if attack missed */
    }

    /* Drain (only if attack hit) */
    if (skill->drain_percent > 0 && damage > 0) {
        int heal_amount = (damage * skill->drain_percent) / 100;
        e->current_hp += heal_amount;
        if (e->current_hp > e->max_hp) e->current_hp = e->max_hp;
        printf("%s drains %d HP from the player!\n", e->name, heal_amount);
    }

    /* Status effects on player (only if attack hit) */
    if (skill->stun_chance > 0 && random_percent(skill->stun_chance)) {
        apply_player_stun(state, skill->stun_duration);
    }

    if (skill->burn_chance > 0 && random_percent(skill->burn_chance)) {
        int burn_damage = (e->atk * skill->burn_damage_percent) / 100;
        apply_player_burn(state, burn_damage, skill->burn_duration);
    }

    if (skill->slow_chance > 0 && random_percent(skill->slow_chance)) {
        apply_player_slow(state, skill->slow_percent, skill->slow_duration);
    }

    if (skill->atk_debuff_percent > 0) {
        apply_player_atk_debuff(state, skill->atk_debuff_percent, skill->atk_debuff_duration);
    }

    if (skill->def_debuff_percent > 0) {
        apply_player_def_debuff(state, skill->def_debuff_percent, skill->def_debuff_duration);
    }

    if (skill->spd_debuff_percent > 0) {
        apply_player_spd_debuff(state, skill->spd_debuff_percent, skill->spd_debuff_duration);
    }

    /* Self buffs always apply (enemy buffs self regardless of hit) */
    if (skill->atk_buff_percent > 0) {
        apply_atk_buff(e, skill->atk_buff_percent, skill->atk_buff_duration);
    }

    if (skill->def_buff_percent > 0) {
        apply_def_buff(e, skill->def_buff_percent, skill->def_buff_duration);
    }

    if (skill->spd_buff_percent > 0) {
        apply_spd_buff(e, skill->spd_buff_percent, skill->spd_buff_duration);
    }

    /* Self healing always applies */
    if (skill->heal_percent > 0) {
        int heal_amount = (e->max_hp * skill->heal_percent) / 100;
        e->current_hp += heal_amount;
        if (e->current_hp > e->max_hp) e->current_hp = e->max_hp;
        printf("%s heals for %d HP!\n", e->name, heal_amount);
    }

    /* Special effects (only if attack hit) */
    if (skill->max_hp_reduction_percent > 0) {
        if (e->type == BOSS_ETERNAL_WARDEN && skill_num == 2) {
            apply_max_hp_reduction(state, enemy_idx, skill->max_hp_reduction_percent);
        } else {
            state->player_max_hp_modified = skill->max_hp_reduction_percent;
            state->player_max_hp_modified_turns = 3;
            printf("Player max HP reduced by %d%% for %d turns!\n",
                   state->player_max_hp_modified, state->player_max_hp_modified_turns);
        }
    }

    if (skill->healing_reduction_percent > 0) {
        apply_player_healing_reduction(state, skill->healing_reduction_percent, skill->healing_reduction_duration);
    }

    if (skill->fire_resist_down_percent > 0) {
        apply_player_fire_resistance_down(state, skill->fire_resist_down_percent, skill->fire_resist_down_duration);
    }

    /* Hard-coded specials */
    if (e->type == BOSS_ETERNAL_WARDEN && skill_num == 1) {
        state->skill1_cooldown = 0;
        state->skill2_cooldown = 0;
        state->weapon_skill_cooldown = 0;
        printf("Player cooldowns have been reset!\n");
    }

    if (e->type == BOSS_ETERNAL_WARDEN && skill_num == 3) {
        if (random_percent(50)) {
            int dur = 2 + (rand() % 2);
            int pick = rand() % 4;

            if (pick == 0) {
                apply_player_stun(state, dur);
            } else if (pick == 1) {
                int burn_damage = (e->atk * 40) / 100;
                apply_player_burn(state, burn_damage, dur);
            } else if (pick == 2) {
                apply_player_def_debuff(state, 60, dur);
            } else {
                apply_player_atk_debuff(state, 50, dur);
            }
        } else {
            printf("No additional status effect applied.\n");
        }
    }
}

/* ===== MAIN COMBAT LOOP ===== */
int run_combat_loop(CombatState *state) {
    display_combat_state(state);

    while (1) {
        int all_dead = 1;
        for (int i = 0; i < state->num_enemies; i++) {
            if (state->enemies[i].current_hp > 0) {
                all_dead = 0;
                break;
            }
        }

        if (all_dead) {
            printf("\n=== VICTORY! ===\n");
            return 1;
        }

        if (state->player_current_hp <= 0) {
            printf("\n=== DEFEAT! ===\n");
            return 0;
        }

        if (state->player.status.stunned_turns > 0) {
            printf("\n%s is stunned and cannot act!\n", state->player.name);

            update_combat_turn(state);

            for (int i = 0; i < state->num_enemies; i++) {
                if (state->enemies[i].current_hp <= 0) continue;

                if (state->enemies[i].status.stunned_turns > 0) {
                    printf("%s is stunned!\n", state->enemies[i].name);
                    continue;
                }

                int action = enemy_get_action(&state->enemies[i]);

                if (action == 0) {
                    int dmg = get_effective_enemy_damage(&state->enemies[i], NORMAL_ATTACK_PERCENT);
                    printf("%s attacks %s!\n", state->enemies[i].name, state->player.name);
                    apply_player_damage(state, dmg, &state->enemies[i]);
                } else {
                    apply_enemy_skill(state, i, action);
                }

                if (state->player_current_hp <= 0) break;
            }

        /* FIXED: Update turn counters AFTER enemies have acted */
        update_combat_turn(state);

            display_combat_state(state);
            continue;
        }

        printf("\nPlayer Actions:\n");
        printf("1) Normal Attack (50%% ATK)\n");
        printf("2) Guard (35%% DMG reduction)\n");

        Skill s1 = get_character_skill(state->player.id, 1);
        Skill s2 = get_character_skill(state->player.id, 2);

        printf("3) %s (cooldown: %d) - %s\n", s1.name, state->skill1_cooldown, s1.description);
        printf("4) %s (cooldown: %d) - %s\n", s2.name, state->skill2_cooldown, s2.description);

        WeaponSkill *weapon_skill = get_weapon_skill(state->equipped_weapon_id);
        if (weapon_skill != NULL) {
            printf("5) %s (cooldown: %d) - %s\n",
                   weapon_skill->name, state->weapon_skill_cooldown, weapon_skill->description);
        } else {
            printf("5) (No weapon equipped)\n");
        }

        printf("6) Flee (escape from combat)\n");
        printf("Choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {}
            continue;
        }

        if (choice == 1) {
            int damage = (state->player.atk * NORMAL_ATTACK_PERCENT) / 100;

            printf("\n%s attacks!\n", state->player.name);
            printf("Select target (0-%d): ", state->num_enemies - 1);

            int target;
            if (scanf("%d", &target) != 1) {
                while (getchar() != '\n') {}
                continue;
            }

            apply_damage(state, clamp(target, 0, state->num_enemies - 1), damage);

        } else if (choice == 2) {
            state->player_guarding = 1;
            printf("%s takes a defensive stance!\n", state->player.name);

        } else if (choice == 3) {
            if (state->skill1_cooldown > 0) {
                printf("Skill on cooldown! (%d turns remaining)\n", state->skill1_cooldown);
                continue;
            }

            printf("\n%s uses %s!\n", state->player.name, s1.name);
            apply_character_skill(state, 1);

            state->skill1_cooldown = s1.cooldown;
        } else if (choice == 4) {
            if (state->skill2_cooldown > 0) {
                printf("Skill on cooldown! (%d turns remaining)\n", state->skill2_cooldown);
                continue;
            }

            printf("\n%s uses %s!\n", state->player.name, s2.name);
            apply_character_skill(state, 2);

            state->skill2_cooldown = s2.cooldown;
        } else if (choice == 5 && weapon_skill != NULL) {
            if (state->weapon_skill_cooldown > 0) {
                printf("Weapon skill on cooldown! (%d turns remaining)\n", state->weapon_skill_cooldown);
                continue;
            }

            printf("\n%s uses %s!\n", state->player.name, weapon_skill->name);

            if (weapon_skill->single_target) {
                printf("Select target (0-%d): ", state->num_enemies - 1);

                int target;
                if (scanf("%d", &target) != 1) {
                    while (getchar() != '\n') {}
                    continue;
                }

                target = clamp(target, 0, state->num_enemies - 1);

                int dmg = (state->player.atk * weapon_skill->damage_percent) / 100;
                apply_damage(state, target, dmg);

                if (weapon_skill->stun_chance > 0 && random_percent(weapon_skill->stun_chance)) {
                    apply_stun_effect(&state->enemies[target], 1);
                }
                if (weapon_skill->burn_chance > 0 && random_percent(weapon_skill->burn_chance)) {
                    int burn = (state->player.atk * weapon_skill->burn_damage_percent) / 100;
                    apply_burn(&state->enemies[target], burn, weapon_skill->burn_duration);
                }
                if (weapon_skill->atk_debuff_percent > 0) {
                    apply_atk_debuff(&state->enemies[target], weapon_skill->atk_debuff_percent, weapon_skill->atk_debuff_duration);
                }
                if (weapon_skill->def_debuff_percent > 0) {
                    apply_def_debuff(&state->enemies[target], weapon_skill->def_debuff_percent, weapon_skill->def_debuff_duration);
                }
                if (weapon_skill->spd_debuff_percent > 0) {
                    apply_spd_debuff(&state->enemies[target], weapon_skill->spd_debuff_percent, weapon_skill->spd_debuff_duration);
                }
            } else {
                printf("Attacking all enemies!\n");
                int dmg = (state->player.atk * weapon_skill->damage_percent) / 100;
                for (int i = 0; i < state->num_enemies; i++) {
                    int hit = apply_damage(state, i, dmg);

                    /* FIXED: Only apply effects if attack hit */
                    if (!hit) continue;

                    if (weapon_skill->stun_chance > 0 && random_percent(weapon_skill->stun_chance)) {
                        apply_stun_effect(&state->enemies[i], 1);
                    }
                    if (weapon_skill->burn_chance > 0 && random_percent(weapon_skill->burn_chance)) {
                        int burn = (state->player.atk * weapon_skill->burn_damage_percent) / 100;
                        apply_burn(&state->enemies[i], burn, weapon_skill->burn_duration);
                    }
                    if (weapon_skill->atk_debuff_percent > 0) {
                        apply_atk_debuff(&state->enemies[i], weapon_skill->atk_debuff_percent, weapon_skill->atk_debuff_duration);
                    }
                    if (weapon_skill->def_debuff_percent > 0) {
                        apply_def_debuff(&state->enemies[i], weapon_skill->def_debuff_percent, weapon_skill->def_debuff_duration);
                    }
                    if (weapon_skill->spd_debuff_percent > 0) {
                        apply_spd_debuff(&state->enemies[i], weapon_skill->spd_debuff_percent, weapon_skill->spd_debuff_duration);
                    }
                }
            }

            if (weapon_skill->spd_buff_percent > 0) {
                apply_player_spd_buff(state, weapon_skill->spd_buff_percent, weapon_skill->spd_buff_duration);
            }

            if (weapon_skill->heal_percent > 0) {
                heal_player_percent(state, weapon_skill->heal_percent);
            }

            state->weapon_skill_cooldown = WEAPON_SKILL_COOLDOWN;

        } else if (choice == 6) {
            int flee_chance = 50 + (get_effective_player_spd(state) / 2);
            flee_chance = clamp(flee_chance, 0, 100);

            printf("\n%s attempts to flee! (Success rate: %d%%)\n", state->player.name, flee_chance);

            if (random_percent(flee_chance)) {
                printf("%s successfully fled from combat!\n", state->player.name);
                display_combat_state(state);
                return 2;
            }

            printf("%s failed to flee! Enemies get a free turn!\n", state->player.name);
        } else {
            printf("Invalid action!\n");
            continue;
        }

        update_combat_turn(state);

        for (int i = 0; i < state->num_enemies; i++) {
            if (state->enemies[i].current_hp <= 0) continue;

            if (state->enemies[i].status.stunned_turns > 0) {
                printf("%s is stunned!\n", state->enemies[i].name);
                continue;
            }

            int action = enemy_get_action(&state->enemies[i]);

            if (action == 0) {
                int dmg = get_effective_enemy_damage(&state->enemies[i], NORMAL_ATTACK_PERCENT);
                printf("%s attacks %s!\n", state->enemies[i].name, state->player.name);
                apply_player_damage(state, dmg, &state->enemies[i]);
            } else {
                apply_enemy_skill(state, i, action);
            }

            if (state->player_current_hp <= 0) break;
        }

        /* FIXED: Update turn counters AFTER enemies have acted */
        update_combat_turn(state);

        display_combat_state(state);
    }
}