/* enemy.c */
#include <stdlib.h>
#include <string.h>

#include "enemy.h"

/* ===== ENEMY SKILL DATABASE =====
   IMPORTANT: The numeric fields MUST match the EnemySkill struct order in enemy.h.
*/
static EnemySkill enemy_skills_db[] = {
    /* ===================== SLIME (2) ===================== */
    {
        ENEMY_SLIME, 1,
        "Ooze Spray",
        "Deal 60% ATK damage + 25% chance to slow",
        60,                 /* damage_percent */
        0, 0,               /* stun_chance, stun_duration */
        0, 0, 0,            /* burn_chance, burn_damage_percent, burn_duration */
        25, 30, 2,          /* slow_chance, slow_percent, slow_duration */
        0, 0,               /* atk_debuff_percent, atk_debuff_duration */
        0, 0,               /* def_debuff_percent, def_debuff_duration */
        0, 0,               /* spd_debuff_percent, spd_debuff_duration */
        0, 0,               /* atk_buff_percent, atk_buff_duration */
        0, 0,               /* def_buff_percent, def_buff_duration */
        0, 0,               /* spd_buff_percent, spd_buff_duration */
        0,                  /* heal_percent */
        0,                  /* drain_percent */
        0,                  /* max_hp_reduction_percent */
        0, 0,               /* healing_reduction_percent, healing_reduction_duration */
        0, 0                /* fire_resist_down_percent, fire_resist_down_duration */
    },
    {
        ENEMY_SLIME, 2,
        "Regenerate",
        "Heal self for 20% max HP",
        0,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        20,
        0,
        0,
        0, 0,
        0, 0
    },

    /* ===================== BANDIT (2) ===================== */
    {
        ENEMY_BANDIT, 1,
        "Quick Strike",
        "Deal 130% ATK damage",
        130,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        0,
        0, 0,
        0, 0
    },
    {
        ENEMY_BANDIT, 2,
        "Exploit Weakness",
        "Deal 150% ATK damage + reduce player DEF by 35% for 2 turns",
        150,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,               /* atk_debuff */
        35, 2,              /* def_debuff (FIXED) */
        0, 0,               /* spd_debuff */
        0, 0,               /* atk_buff */
        0, 0,               /* def_buff */
        0, 0,               /* spd_buff */
        0,                  /* heal */
        0,                  /* drain */
        0,                  /* max_hp_reduction */
        0, 0,               /* healing_reduction */
        0, 0                /* fire_resist_down */
    },

    /* ===================== BEAST (2) ===================== */
    {
        ENEMY_BEAST, 1,
        "Inferno Breath",
        "Deal 110% ATK damage + inflict burn for 3 turns",
        110,
        0, 0,
        100, 40, 3,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        0,
        0, 0,
        0, 0
    },
    {
        ENEMY_BEAST, 2,
        "Savage Roar",
        "Increase own ATK by 60% for 2 turns + reduce player ATK by 40% for 2 turns",
        0,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        40, 2,              /* atk_debuff */
        0, 0,
        0, 0,
        60, 2,              /* atk_buff */
        0, 0,
        0, 0,
        0,
        0,
        0,
        0, 0,
        0, 0
    },

    /* ===================== GORGON (2) ===================== */
    {
        ENEMY_GORGON, 1,
        "Petrifying Gaze",
        "Deal 95% ATK damage + 50% chance to stun for 1 turn",
        95,
        50, 1,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        0,
        0, 0,
        0, 0
    },
    {
        ENEMY_GORGON, 2,
        "Serpent Shield",
        "Reduce all incoming damage by 45% for 1 turn + heal self for 25% max HP",
        0,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,               /* def_debuff cleared */
        0, 0,
        0, 0,
        45, 1,              /* def_buff (FIXED) */
        0, 0,
        25,
        0,
        0,
        0, 0,
        0, 0
    },

    /* ===================== WRAITH (2) ===================== */
    {
        ENEMY_WRAITH, 1,
        "Life Drain",
        "Deal 120% ATK damage + steal 40% of damage dealt as healing",
        120,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        40,                 /* drain_percent */
        0,
        0, 0,
        0, 0
    },
    {
        ENEMY_WRAITH, 2,
        "Curse",
        "Reduce player max HP by 20% for 3 turns + reduce player healing by 50% for 3 turns",
        0,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        20,                 /* max_hp_reduction_percent */
        50, 3,              /* healing_reduction_percent, duration */
        0, 0
    },

    /* ===================== SHADOW KNIGHT (3) ===================== */
    {
        BOSS_SHADOW_KNIGHT, 1,
        "Dark Slash",
        "Deal 160% ATK damage + 40% chance to stun for 1 turn",
        160,
        40, 1,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        0,
        0, 0,
        0, 0
    },
    {
        BOSS_SHADOW_KNIGHT, 2,
        "Shadow Armor",
        "Reduce all incoming damage by 50% for 2 turns + increase own ATK by 50% for 2 turns",
        0,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,               /* def_debuff cleared (FIXED) */
        0, 0,
        50, 2,              /* atk_buff */
        50, 2,              /* def_buff */
        0, 0,
        0,
        0,
        0,
        0, 0,
        0, 0
    },
    {
        BOSS_SHADOW_KNIGHT, 3,
        "Void Execution",
        "Deal 200% ATK damage + reduce player DEF by 50% for 3 turns",
        200,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,               /* atk_debuff cleared */
        50, 3,              /* def_debuff (FIXED) */
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        0,
        0, 0,
        0, 0
    },

    /* ===================== PHOENIX LORD (3) ===================== */
    {
        BOSS_PHOENIX_LORD, 1,
        "Flame Inferno",
        "Deal 140% ATK damage + inflict burn for 4 turns",
        140,
        0, 0,
        100, 18, 4,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        0,
        0, 0,
        0, 0
    },
    {
        BOSS_PHOENIX_LORD, 2,
        "Rebirth Flame",
        "Special: When health drops below 35%, heal to 70% max HP + gain ATK+40% for 4 turns (triggers once)",
        0,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        40, 4,              /* atk_buff */
        0, 0,
        0, 0,
        70,                 /* heal_percent */
        0,
        0,
        0, 0,
        0, 0
    },
    {
        BOSS_PHOENIX_LORD, 3,
        "Immolation",
        "Deal 220% ATK damage + inflict burn + reduce player fire resistance (take 25% more burn damage) for 3 turns",
        220,
        0, 0,
        100, 40, 4,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        0,
        0, 0,
        25, 3               /* fire_resist_down */
    },

    /* ===================== ETERNAL WARDEN (3) ===================== */
    {
        BOSS_ETERNAL_WARDEN, 1,
        "Temporal Fracture",
        "Reduce player SPD by 60% for 3 turns + reset player skill cooldowns",
        0,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        60, 3,              /* spd_debuff */
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        0,
        0, 0,
        0, 0
    },
    {
        BOSS_ETERNAL_WARDEN, 2,
        "Judgment Strike",
        "Deal 200% ATK damage + 70% chance to stun for 2 turns + reduce player max HP by 25% permanently",
        200,
        70, 2,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        25,                 /* max_hp_reduction_percent */
        0, 0,
        0, 0
    },
    {
        BOSS_ETERNAL_WARDEN, 3,
        "Annihilation Protocol",
        "Deal 180% ATK damage + randomly apply (50% stun OR burn OR DEF down 60% OR ATK down 50%) for 2-3 turns",
        180,
        0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0,
        0,
        0,
        0, 0,
        0, 0
    }
};

#define NUM_SKILLS ((int)(sizeof(enemy_skills_db) / sizeof(enemy_skills_db[0])))

Enemy create_enemy(EnemyType type) {
    Enemy e;
    memset(&e, 0, sizeof(Enemy));

    e.type = type;
    e.turn_count = 0;

    /* Initialize status fields (safe even if already zeroed) */
    e.status.burn_turns = 0;
    e.status.burn_damage_per_turn = 0;
    e.status.stunned_turns = 0;

    e.status.atk_debuff_turns = 0; e.status.atk_debuff_percent = 0;
    e.status.def_debuff_turns = 0; e.status.def_debuff_percent = 0;
    e.status.spd_debuff_turns = 0; e.status.spd_debuff_percent = 0;

    e.status.atk_buff_turns = 0; e.status.atk_buff_percent = 0;
    e.status.def_buff_turns = 0; e.status.def_buff_percent = 0;
    e.status.spd_buff_turns = 0; e.status.spd_buff_percent = 0;

    e.status.slow_turns = 0; e.status.slow_percent = 0;

    e.status.max_hp_reduction_turns = 0; e.status.max_hp_reduction_percent = 0;
    e.status.healing_reduction_turns = 0; e.status.healing_reduction_percent = 0;
    e.status.fire_resist_down_turns = 0; e.status.fire_resist_down_percent = 0;

    switch (type) {
        case ENEMY_SLIME:
            strcpy(e.name, "Slime");
            e.atk = 60; e.hp = 400; e.def = 80; e.spd = 100;
            break;

        case ENEMY_BANDIT:
            strcpy(e.name, "Bandit");
            e.atk = 120; e.hp = 300; e.def = 70; e.spd = 150;
            break;

        case ENEMY_BEAST:
            strcpy(e.name, "Beast");
            e.atk = 100; e.hp = 600; e.def = 140; e.spd = 120;
            break;

        case ENEMY_GORGON:
            strcpy(e.name, "Gorgon");
            e.atk = 80; e.hp = 350; e.def = 120; e.spd = 130;
            break;

        case ENEMY_WRAITH:
            strcpy(e.name, "Wraith");
            e.atk = 110; e.hp = 450; e.def = 100; e.spd = 160;
            break;

        case BOSS_SHADOW_KNIGHT:
            strcpy(e.name, "Shadow Knight");
            e.atk = 140; e.hp = 1000; e.def = 200; e.spd = 180;
            break;

        case BOSS_PHOENIX_LORD:
            strcpy(e.name, "Phoenix Lord");
            e.atk = 160; e.hp = 900; e.def = 250; e.spd = 190;
            break;

        case BOSS_ETERNAL_WARDEN:
            strcpy(e.name, "Eternal Warden");
            e.atk = 180; e.hp = 1100; e.def = 300; e.spd = 200;
            break;

        default:
            strcpy(e.name, "Unknown");
            e.atk = 1; e.hp = 1; e.def = 0; e.spd = 1;
            break;
    }

    e.max_hp = e.hp;
    e.current_hp = e.hp;
    return e;
}

int enemy_get_action(Enemy *e) {
    /* Alternating pattern: odd turns = normal attack (0), even turns = skill (1-N) */
    e->turn_count++;

    if (e->turn_count % 2 == 1) {
        return 0; /* Normal attack */
    }

    int num_skills = 2;
    if (e->type == BOSS_SHADOW_KNIGHT || e->type == BOSS_PHOENIX_LORD || e->type == BOSS_ETERNAL_WARDEN) num_skills = 3;

    return 1 + (rand() % num_skills);
}

EnemySkill *get_enemy_skill(EnemyType type, int skill_num) {
    for (int i = 0; i < NUM_SKILLS; i++) {
        if (enemy_skills_db[i].enemy_type == type && enemy_skills_db[i].skill_num == skill_num) {
            return &enemy_skills_db[i];
        }
    }
    return NULL;
}
