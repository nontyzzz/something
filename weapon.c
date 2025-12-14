#include "weapon.h"

#include "gacha.h"

extern Weapon weapons_2star[4];
extern Weapon weapons_3star[20];
extern Weapon weapons_4star[8];
extern Weapon weapons_5star[4];

/* ===== WEAPON SKILL DEFINITIONS ===== */

/* 4-star weapon skills */
WeaponSkill skill_buster_sword = {
    .name = "Iron Cleave",
    .description = "Deal 170% ATK to a single enemy. 40% chance to stun for 1 turn.",
    .damage_percent = 170,
    .single_target = 1,
    .stun_chance = 40,
    .burn_chance = 0,
    .burn_damage_percent = 0,
    .burn_duration = 0,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 0,
    .def_debuff_duration = 0,
    .spd_buff_percent = 0,
    .spd_buff_duration = 0,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 0
};

WeaponSkill skill_rebellion = {
    .name = "Rebel Assault",
    .description = "Deal 140% ATK to a single enemy. If enemy HP < 50%, deal additional 60% ATK.",
    .damage_percent = 140,
    .single_target = 1,
    .stun_chance = 0,
    .burn_chance = 0,
    .burn_damage_percent = 0,
    .burn_duration = 0,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 0,
    .def_debuff_duration = 0,
    .spd_buff_percent = 0,
    .spd_buff_duration = 0,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 0
};

WeaponSkill skill_heilig_bogen = {
    .name = "Holy Arrow",
    .description = "Deal 160% ATK to a single enemy. Increase own SPD by 40% for 2 turns.",
    .damage_percent = 160,
    .single_target = 1,
    .stun_chance = 0,
    .burn_chance = 0,
    .burn_damage_percent = 0,
    .burn_duration = 0,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 0,
    .def_debuff_duration = 0,
    .spd_buff_percent = 40,
    .spd_buff_duration = 2,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 0
};

WeaponSkill skill_sunray = {
    .name = "Solar Volley",
    .description = "Deal 110% ATK to all enemies. 35% chance to inflict burn (40% ATK/turn, 2 turns).",
    .damage_percent = 110,
    .single_target = 0,
    .stun_chance = 0,
    .burn_chance = 35,
    .burn_damage_percent = 40,
    .burn_duration = 2,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 0,
    .def_debuff_duration = 0,
    .spd_buff_percent = 0,
    .spd_buff_duration = 0,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 0
};

WeaponSkill skill_heaven_splitter = {
    .name = "Heaven's Thrust",
    .description = "Deal 180% ATK to a single enemy. 50% chance to stun for 1 turn.",
    .damage_percent = 180,
    .single_target = 1,
    .stun_chance = 50,
    .burn_chance = 0,
    .burn_damage_percent = 0,
    .burn_duration = 0,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 0,
    .def_debuff_duration = 0,
    .spd_buff_percent = 0,
    .spd_buff_duration = 0,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 0
};

WeaponSkill skill_war_flaming_lance = {
    .name = "Warflame Impale",
    .description = "Deal 150% ATK to a single enemy. Always inflict burn (40% ATK/turn) for 3 turns.",
    .damage_percent = 150,
    .single_target = 1,
    .stun_chance = 0,
    .burn_chance = 100,
    .burn_damage_percent = 40,
    .burn_duration = 3,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 0,
    .def_debuff_duration = 0,
    .spd_buff_percent = 0,
    .spd_buff_duration = 0,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 0
};

WeaponSkill skill_celestial_scepter = {
    .name = "Celestial Drain",
    .description = "Deal 120% ATK to a single enemy. Reduce enemy DEF by 40% for 2 turns. Heal self for 15% max HP.",
    .damage_percent = 120,
    .single_target = 1,
    .stun_chance = 0,
    .burn_chance = 0,
    .burn_damage_percent = 0,
    .burn_duration = 0,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 40,
    .def_debuff_duration = 2,
    .spd_buff_percent = 0,
    .spd_buff_duration = 0,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 15
};

WeaponSkill skill_staff_of_explosion = {
    .name = "Explosive Surge",
    .description = "Deal 140% ATK to all enemies. 35% chance to stun each enemy for 1 turn.",
    .damage_percent = 140,
    .single_target = 0,
    .stun_chance = 35,
    .burn_chance = 0,
    .burn_damage_percent = 0,
    .burn_duration = 0,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 0,
    .def_debuff_duration = 0,
    .spd_buff_percent = 0,
    .spd_buff_duration = 0,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 0
};

/* 5-star weapon skills */
WeaponSkill skill_excalibur = {
    .name = "Sword of Promised Victory",
    .description = "Deal 180% ATK to all enemies. Reduce all ATK by 35% for 2 turns. Heal self for 25% max HP.",
    .damage_percent = 180,
    .single_target = 0,
    .stun_chance = 0,
    .burn_chance = 0,
    .burn_damage_percent = 0,
    .burn_duration = 0,
    .atk_debuff_percent = 35,
    .atk_debuff_duration = 2,
    .def_debuff_percent = 0,
    .def_debuff_duration = 0,
    .spd_buff_percent = 0,
    .spd_buff_duration = 0,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 25
};

WeaponSkill skill_maanna = {
    .name = "An Gal Ta Ki Gal",
    .description = "Deal 200% ATK to a single enemy. Increase own SPD by 60% for 3 turns. 40% chance to apply burn (40% ATK/turn, 3 turns).",
    .damage_percent = 200,
    .single_target = 1,
    .stun_chance = 0,
    .burn_chance = 40,
    .burn_damage_percent = 40,
    .burn_duration = 3,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 0,
    .def_debuff_duration = 0,
    .spd_buff_percent = 60,
    .spd_buff_duration = 3,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 0
};

WeaponSkill skill_ea = {
    .name = "Enuma Elish",
    .description = "Deal 190% ATK to all enemies. 50% chance to stun each enemy for 1 turn. Reduce all DEF by 30% for 2 turns.",
    .damage_percent = 190,
    .single_target = 0,
    .stun_chance = 50,
    .burn_chance = 0,
    .burn_damage_percent = 0,
    .burn_duration = 0,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 30,
    .def_debuff_duration = 2,
    .spd_buff_percent = 0,
    .spd_buff_duration = 0,
    .spd_debuff_percent = 0,
    .spd_debuff_duration = 0,
    .heal_percent = 0
};

WeaponSkill skill_star_of_eden = {
    .name = "Synthetic Blackhole",
    .description = "Deal 170% ATK to all enemies. Reduce all DEF by 50% for 3 turns. Reduce all SPD by 40% for 2 turns.",
    .damage_percent = 170,
    .single_target = 0,
    .stun_chance = 0,
    .burn_chance = 0,
    .burn_damage_percent = 0,
    .burn_duration = 0,
    .atk_debuff_percent = 0,
    .atk_debuff_duration = 0,
    .def_debuff_percent = 50,
    .def_debuff_duration = 3,
    .spd_buff_percent = 0,
    .spd_buff_duration = 0,
    .spd_debuff_percent = 40,
    .spd_debuff_duration = 2,
    .heal_percent = 0
};

/* ===== WEAPON LOOKUP FUNCTIONS ===== */

Weapon* get_weapon_by_id(int id) {
    if (id >= 211 && id <= 241) {
        int idx = 0;
        if (id == 211) idx = 0;
        else if (id == 221) idx = 1;
        else if (id == 231) idx = 2;
        else if (id == 241) idx = 3;
        return &weapons_2star[idx];
    }
    if (id >= 311 && id <= 345) {
        int idx = 0;
        if (id >= 311 && id <= 315) idx = id - 311;
        else if (id >= 321 && id <= 325) idx = 5 + (id - 321);
        else if (id >= 331 && id <= 335) idx = 10 + (id - 331);
        else if (id >= 341 && id <= 345) idx = 15 + (id - 341);
        return &weapons_3star[idx];
    }
    if (id >= 411 && id <= 442) {
        int idx = 0;
        if (id >= 411 && id <= 412) idx = id - 411;
        else if (id >= 421 && id <= 422) idx = 2 + (id - 421);
        else if (id >= 431 && id <= 432) idx = 4 + (id - 431);
        else if (id >= 441 && id <= 442) idx = 6 + (id - 441);
        return &weapons_4star[idx];
    }
    if (id >= 511 && id <= 541) {
        int idx = 0;
        if (id == 511) idx = 0;
        else if (id == 521) idx = 1;
        else if (id == 531) idx = 2;
        else if (id == 541) idx = 3;
        return &weapons_5star[idx];
    }
    return NULL;
}

WeaponSkill* get_weapon_skill(int weapon_id) {
    switch (weapon_id) {
        /* 4-star skills */
        case 411: return &skill_buster_sword;
        case 412: return &skill_rebellion;
        case 421: return &skill_heilig_bogen;
        case 422: return &skill_sunray;
        case 431: return &skill_heaven_splitter;
        case 432: return &skill_war_flaming_lance;
        case 441: return &skill_celestial_scepter;
        case 442: return &skill_staff_of_explosion;
        
        /* 5-star skills */
        case 511: return &skill_excalibur;
        case 521: return &skill_maanna;
        case 531: return &skill_ea;
        case 541: return &skill_star_of_eden;
        
        default: return NULL;
    }
}
