#ifndef STAGE_H
#define STAGE_H

#include "combat.h"

/* ===== STAGE STRUCT ===== */

typedef struct {
    int stage_id;
    EnemyType enemy_types[MAX_ENEMIES];
    int num_enemies;
    int gem_reward;
} Stage;

/* ===== STAGE FUNCTIONS ===== */

Stage get_stage_definition(int stage_id);
int get_gem_reward(int stage_id);
int run_stage(int stage_id, CombatCharacter *player, PlayerData *p);
void stage_selection_menu(PlayerData *p, CombatCharacter *player);

#endif
