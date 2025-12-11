#ifndef COMBAT_H
#define COMBAT_H

#include <stdio.h>
#include "weapon.h" // NOW CORRECTLY INCLUDED HERE

#define MAX_ENEMIES 3

//----------------------------------------------------
// STRUCTS
//----------------------------------------------------
typedef struct {
    char name[50];
    int atk, hp, def, spd;
    int maxHP;
    int hasWeaponSkill;
    int isBurning;
    int isStunned;
    // NEW: The complete Weapon struct to hold equipped weapon data
    Weapon equippedWeapon; 
} Character;

typedef struct {
    char name[50];
    int atk, hp, def, spd;
    int isBurning;
    int isStunned;
} Enemy;

//----------------------------------------------------
// FUNCTION DECLARATIONS (combat functions)
//----------------------------------------------------
void saveCharacter(Character *c);
void loadCharacter(Character *c);

int calculateDamage(int atk, int def);

int pickTarget(Enemy enemies[]);

void applyBurn(Character *c);
void applyBurnEnemy(Enemy *e);

void normalAttack(Character *c, Enemy *e);
void guardAction(Character *c);
void skill1(Character *c, Enemy *e); // fallback
void skill2(Character *c, Enemy *e); // fallback
void weaponSkill(Character *c, Enemy enemies[]);

void enemyAttack(Enemy *e, Character *c);

void takeTurn(Character *c, Enemy enemies[]);
void enemyTurns(Character *c, Enemy enemies[]);

void battle(Character *c);

#endif // COMBAT_H