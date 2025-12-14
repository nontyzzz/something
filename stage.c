#include <stdio.h>
#include <string.h>

#include "stage.h"

Stage get_stage_definition(int stage_id) {
    Stage s;
    memset(&s, 0, sizeof(Stage));
    s.stage_id = stage_id;

    switch (stage_id) {
        case 1:
            s.num_enemies = 1;
            s.enemy_types[0] = ENEMY_SLIME;
            s.gem_reward = 100;
            break;
        case 2:
            s.num_enemies = 2;
            s.enemy_types[0] = ENEMY_SLIME;
            s.enemy_types[1] = ENEMY_SLIME;
            s.gem_reward = 150;
            break;
        case 3:
            s.num_enemies = 2;
            s.enemy_types[0] = ENEMY_BANDIT;
            s.enemy_types[1] = ENEMY_SLIME;
            s.gem_reward = 200;
            break;
        case 4:
            s.num_enemies = 2;
            s.enemy_types[0] = ENEMY_GORGON;
            s.enemy_types[1] = ENEMY_SLIME;
            s.gem_reward = 200;
            break;
        case 5:
            s.num_enemies = 2;
            s.enemy_types[0] = BOSS_SHADOW_KNIGHT;
            s.enemy_types[1] = ENEMY_BANDIT;
            s.gem_reward = 400;
            break;
        case 6:
            s.num_enemies = 3;
            s.enemy_types[0] = ENEMY_BANDIT;
            s.enemy_types[1] = ENEMY_BANDIT;
            s.enemy_types[2] = ENEMY_BEAST;
            s.gem_reward = 250;
            break;
        case 7:
            s.num_enemies = 3;
            s.enemy_types[0] = ENEMY_BEAST;
            s.enemy_types[1] = ENEMY_SLIME;
            s.enemy_types[2] = ENEMY_SLIME;
            s.gem_reward = 250;
            break;
        case 8:
            s.num_enemies = 3;
            s.enemy_types[0] = ENEMY_WRAITH;
            s.enemy_types[1] = ENEMY_GORGON;
            s.enemy_types[2] = ENEMY_BANDIT;
            s.gem_reward = 300;
            break;
        case 9:
            s.num_enemies = 3;
            s.enemy_types[0] = ENEMY_BEAST;
            s.enemy_types[1] = ENEMY_WRAITH;
            s.enemy_types[2] = ENEMY_SLIME;
            s.gem_reward = 300;
            break;
        case 10:
            s.num_enemies = 3;
            s.enemy_types[0] = BOSS_PHOENIX_LORD;
            s.enemy_types[1] = ENEMY_GORGON;
            s.enemy_types[2] = ENEMY_GORGON;
            s.gem_reward = 600;
            break;
        case 11:
            s.num_enemies = 3;
            s.enemy_types[0] = ENEMY_BANDIT;
            s.enemy_types[1] = ENEMY_BANDIT;
            s.enemy_types[2] = ENEMY_BANDIT;
            s.gem_reward = 350;
            break;
        case 12:
            s.num_enemies = 3;
            s.enemy_types[0] = ENEMY_GORGON;
            s.enemy_types[1] = ENEMY_BEAST;
            s.enemy_types[2] = ENEMY_WRAITH;
            s.gem_reward = 350;
            break;
        case 13:
            s.num_enemies = 3;
            s.enemy_types[0] = ENEMY_WRAITH;
            s.enemy_types[1] = ENEMY_WRAITH;
            s.enemy_types[2] = ENEMY_SLIME;
            s.gem_reward = 350;
            break;
        case 14:
            s.num_enemies = 3;
            s.enemy_types[0] = ENEMY_BEAST;
            s.enemy_types[1] = ENEMY_WRAITH;
            s.enemy_types[2] = ENEMY_GORGON;
            s.gem_reward = 400;
            break;
        case 15:
            s.num_enemies = 3;
            s.enemy_types[0] = BOSS_ETERNAL_WARDEN;
            s.enemy_types[1] = ENEMY_WRAITH;
            s.enemy_types[2] = ENEMY_WRAITH;
            s.gem_reward = 1000;
            break;
        default:
            s.num_enemies = 0;
            s.gem_reward = 0;
            break;
    }

    return s;
}

int run_stage(int stage_id, CombatCharacter *player, PlayerData *p) {
    Stage s = get_stage_definition(stage_id);
    if (s.num_enemies == 0) {
        printf("Invalid stage!\n");
        return 0;
    }

    printf("STAGE %d\n", stage_id);

    Enemy enemies[MAX_ENEMIES];
    for (int i = 0; i < s.num_enemies; i++) {
        enemies[i] = create_enemy(s.enemy_types[i]);
    }

    CombatState combat;
    init_combat(&combat, player, enemies, s.num_enemies);

    /* Make weapon skill logic available in combat */
    combat.equipped_weapon_id = p->equipped_weapon[p->current_character];

    int result = run_combat_loop(&combat);

    /* run_combat_loop return codes:
       1 = victory, 0 = defeat, 2 = fled */
    if (result == 1) {
        printf("Clearing stage %d!\n", stage_id);

        p->gems += s.gem_reward;

        if (stage_id > p->highest_stage_cleared) {
            p->highest_stage_cleared = stage_id;
        }

        save_player_data(p);
        printf("Received %d gems!\n", s.gem_reward);
        return 1;
    }

    if (result == 2) {
        printf("You fled from stage %d.\n", stage_id);
        /* No rewards, no progression on flee. */
        return 0;
    }

    printf("Stage %d failed!\n", stage_id);
    return 0;
}

void stage_selection_menu(PlayerData *p, CombatCharacter *player) {
    while (1) {
        printf("\nSTAGE SELECTION\n");
        printf("Highest Stage Cleared: %d\n", p->highest_stage_cleared);
        printf("Gems: %d\n", p->gems);
        printf("Select stage 1-15 or 0 to go back: ");

        int choice = -1;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {}
            continue;
        }
        while (getchar() != '\n') {}

        if (choice == 0) return;
        if (choice < 1 || choice > 15) continue;

        if (choice > p->highest_stage_cleared + 1) {
            printf("Stage not yet available!\n");
            continue;
        }

        run_stage(choice, player, p);

        printf("Press Enter to continue...");
        while (getchar() != '\n') {}
    }
}
