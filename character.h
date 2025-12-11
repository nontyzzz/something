#ifndef CHARACTER_H
#define CHARACTER_H

#include "combat.h" // get Character and Enemy definitions

typedef enum {
    CHAR_DEFAULT,
    CHAR_SABER,
    CHAR_ISHTAR,
    CHAR_GILGAMESH,
    CHAR_WELT_YANG
} CharacterType;

// create a character preset
Character createCharacter(CharacterType type);

// skill entry points used by combat system
// targets is pointer to an array of Enemy, targetCount is usually MAX_ENEMIES
void useSkill1(Character *c, Enemy *targets, int targetCount);
void useSkill2(Character *c, Enemy *targets, int targetCount);

#endif // CHARACTER_H