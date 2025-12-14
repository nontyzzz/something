#include "character.h"

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
    
    if (char_id == 1) { /* Saber - Support/Healer/Tank */
        if (skill_num == 1) {
            s.id = 1;
            strcpy(s.name, "Avalon's Protection");
            strcpy(s.description, "Heal self 20%% max HP + increase own DEF by 50%% for 2 turns");
            s.damage_percent = 0;
            s.cooldown = 4;
        } else if (skill_num == 2) {
            s.id = 2;
            strcpy(s.name, "Excalibur Strike");
            strcpy(s.description, "Deal 120%% ATK to all enemies + reduce all enemy ATK by 40%% for 2 turns");
            s.damage_percent = 120;
            s.cooldown = 4;
        }
    } else if (char_id == 2) { /* Ishtar - Speed/Burst DPS */
        if (skill_num == 1) {
            s.id = 3;
            strcpy(s.name, "Throw Gems");
            strcpy(s.description, "Deal 180%% ATK to single enemy + inflict burn (40%% ATK/turn for 3 turns)");
            s.damage_percent = 180;
            s.cooldown = 4;
        } else if (skill_num == 2) {
            s.id = 4;
            strcpy(s.name, "Starlight Rain");
            strcpy(s.description, "Deal 100%% ATK to all enemies + increase own SPD by 60%% for 2 turns + 50%% chance to slow all enemies (SPD -35%% for 2 turns)");
            s.damage_percent = 100;
            s.cooldown = 4;
        }
    } else if (char_id == 3) { /* Gilgamesh - Control/CC */
        if (skill_num == 1) {
            s.id = 5;
            strcpy(s.name, "Gate of Babylon");
            strcpy(s.description, "Deal 130%% ATK to all enemies + 50%% chance to stun each for 2 turns");
            s.damage_percent = 130;
            s.cooldown = 4;
        } else if (skill_num == 2) {
            s.id = 6;
            strcpy(s.name, "Chains of Heaven");
            strcpy(s.description, "Deal 170%% ATK to single enemy + reduce DEF by 60%% for 3 turns + 60%% chance to stun for 1 turn");
            s.damage_percent = 170;
            s.cooldown = 4;
        }
    } else if (char_id == 4) { /* Welt Yang - Debuffer/Control */
        if (skill_num == 1) {
            s.id = 7;
            strcpy(s.name, "Gravity Suppression");
            strcpy(s.description, "Deal 150%% ATK to single enemy + reduce DEF by 70%% for 3 turns");
            s.damage_percent = 150;
            s.cooldown = 4;
        } else if (skill_num == 2) {
            s.id = 8;
            strcpy(s.name, "Gravity Imprisonment");
            strcpy(s.description, "Deal 110%% ATK to all enemies + reduce all enemy SPD by 70%% for 2 turns + 55%% chance to stun each for 1 turn");
            s.damage_percent = 110;
            s.cooldown = 4;
        }
    }
    
    return s;
}
