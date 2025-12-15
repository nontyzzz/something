#include <string.h>

#include "character.h"

/* ===== CHARACTER SKILL DATABASE =====
   IMPORTANT: The numeric fields MUST match the CharacterSkillData struct order in character.h.
*/
static const CharacterSkillData character_skills_db[] = {
    /* ===================== SABER (1) ===================== */
    {
        1, 1,
        "Avalon's Protection",
        "Heal self 40% max HP + increase own DEF by 50% for 2 turns",
        4, 0, SKILL_TARGET_SELF,

        /* self */
        40, 0, 0, 50, 2, 0, 0,

        /* enemy */
        0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0, 0
    },
    {
        1, 2,
        "Excalibur Strike",
        "Deal 120% ATK to all enemies + reduce all enemy ATK by 40% for 2 turns",
        4, 120, SKILL_TARGET_ALL_ENEMIES,

        /* self */
        0, 0, 0, 0, 0, 0, 0,

        /* enemy */
        0, 0,
        0, 0, 0,
        40, 2,
        0, 0,
        0, 0, 0
    },

    /* ===================== ISHTAR (2) ===================== */
    {
        2, 1,
        "Throw Gems",
        "Deal 180% ATK to single enemy + inflict burn (40% ATK/turn for 3 turns)",
        4, 180, SKILL_TARGET_SINGLE_ENEMY,

        /* self */
        0, 0, 0, 0, 0, 0, 0,

        /* enemy */
        0, 0,
        100, 40, 3,
        0, 0,
        0, 0,
        0, 0, 0
    },
    {
        2, 2,
        "Starlight Rain",
        "Deal 100% ATK to all enemies + increase own SPD by 60% for 2 turns + 50% chance to reduce each enemy SPD by 35% for 2 turns",
        4, 100, SKILL_TARGET_ALL_ENEMIES,

        /* self */
        0, 0, 0, 0, 0, 60, 2,

        /* enemy */
        0, 0,
        0, 0, 0,
        0, 0,
        0, 0,
        50, 35, 2
    },

    /* ===================== GILGAMESH (3) ===================== */
    {
        3, 1,
        "Gate of Babylon",
        "Deal 130% ATK to all enemies + 50% chance to stun each for 1 turn",
        4, 130, SKILL_TARGET_ALL_ENEMIES,

        /* self */
        0, 0, 0, 0, 0, 0, 0,

        /* enemy */
        50, 1,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 0, 0
    },
    {
        3, 2,
        "Chains of Heaven",
        "Deal 170% ATK to single enemy + reduce DEF by 60% for 3 turns + 60% chance to stun for 1 turn",
        4, 170, SKILL_TARGET_SINGLE_ENEMY,

        /* self */
        0, 0, 0, 0, 0, 0, 0,

        /* enemy */
        60, 1,
        0, 0, 0,
        0, 0,
        60, 3,
        0, 0, 0
    },

    /* ===================== WELT YANG (4) ===================== */
    {
        4, 1,
        "Gravity Suppression",
        "Deal 150% ATK to single enemy + reduce DEF by 70% for 3 turns",
        4, 150, SKILL_TARGET_SINGLE_ENEMY,

        /* self */
        0, 0, 0, 0, 0, 0, 0,

        /* enemy */
        0, 0,
        0, 0, 0,
        0, 0,
        70, 3,
        0, 0, 0
    },
    {
        4, 2,
        "Gravity Imprisonment",
        "Deal 110% ATK to all enemies + reduce all enemy SPD by 70% for 2 turns + 55% chance to stun each for 1 turn",
        4, 110, SKILL_TARGET_ALL_ENEMIES,

        /* self */
        0, 0, 0, 0, 0, 0, 0,

        /* enemy */
        55, 1,
        0, 0, 0,
        0, 0,
        0, 0,
        0, 70, 2
    }
};

const CharacterSkillData *get_character_skill_data(int char_id, int skill_num) {
    for (unsigned i = 0; i < sizeof(character_skills_db) / sizeof(character_skills_db[0]); i++) {
        if (character_skills_db[i].character_id == char_id && character_skills_db[i].skill_num == skill_num) {
            return &character_skills_db[i];
        }
    }
    return NULL;
}

CombatCharacter create_character(int char_id) {
    CombatCharacter c;
    memset(&c, 0, sizeof(CombatCharacter));

    /* Stats are ONLY from weapons - NOT base stats */
    c.atk = 0;
    c.hp = 0;
    c.def = 0;
    c.spd = 0;

    if (char_id == 1) {
        strcpy(c.name, "Saber");
        c.id = 1;
        c.allowed_weapon_type = TYPE_SWORD;
    } else if (char_id == 2) {
        strcpy(c.name, "Ishtar");
        c.id = 2;
        c.allowed_weapon_type = TYPE_BOW;
    } else if (char_id == 3) {
        strcpy(c.name, "Gilgamesh");
        c.id = 3;
        c.allowed_weapon_type = TYPE_SPEAR;
    } else if (char_id == 4) {
        strcpy(c.name, "Welt Yang");
        c.id = 4;
        c.allowed_weapon_type = TYPE_STAFF;
    }

    c.equipped_weapon_id = 0;
    return c;
}

int can_equip(CombatCharacter *c, Weapon *w) {
    if (!w) return 0;
    return w->type == c->allowed_weapon_type;
}

void equip_weapon(CombatCharacter *c, int weapon_id) {
    Weapon *w = get_weapon_by_id(weapon_id);
    if (w && can_equip(c, w)) {
        c->equipped_weapon_id = weapon_id;
        c->atk = w->atk;
        c->hp = w->hp;
        c->def = w->def;
        c->spd = w->spd;
    }
}

Skill get_character_skill(int char_id, int skill_num) {
    Skill s;
    memset(&s, 0, sizeof(Skill));

    const CharacterSkillData *sd = get_character_skill_data(char_id, skill_num);
    if (!sd) return s;

    s.id = sd->skill_num;
    strcpy(s.name, sd->name);
    strcpy(s.description, sd->description);
    s.damage_percent = sd->damage_percent;
    s.cooldown = sd->cooldown;

    return s;
}
