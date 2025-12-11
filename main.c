#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "combat.h"    // Contains Character struct (which uses Weapon)
#include "character.h" // Contains CharacterType enum and createCharacter prototype
#include "gacha.h"     // Contains PlayerData struct and menu prototypes
#include "weapon.h"    // Contains Weapon and equipWeapon prototype

// FIX: This menu definition is now in main.c, correctly using the Weapon struct fields.
void inventory_menu(PlayerData *pdata) {
    // For simplicity, we assume the player always manages the first character's equipment
    if (pdata->characterInv[0] == 0) {
        printf("\nYou need at least one character (ID: 1-4) to manage equipment! Roll on the Gacha menu.\n");
        return;
    }
    
    // 1. Load base character stats 
    Character player = createCharacter(pdata->characterInv[0]); 
    
    // 2. Load saved stats (this loads the equipped weapon ID and re-creates the Weapon struct)
    loadCharacter(&player); 

    printf("\n=== INVENTORY MENU ===\n");
    printf("Current Character: %s\n", player.name);
    // FIX: Using the Weapon struct fields directly
    printf("Equipped Weapon: %s (ATK +%d, HP +%d)\n", 
           player.equippedWeapon.name, player.equippedWeapon.atk_base_boost, player.equippedWeapon.hp_base_boost);
    
    // 3. Call the primary equip function
    equipWeapon(&player, &pdata->weaponInv);
}


int main() {
    srand((unsigned int)time(NULL));

    PlayerData pdata;
    load_player_data(&pdata);

    // Initial character setup if none is loaded, ensuring CHAR_SABER is the default hero
    if (pdata.char_banner_first == 1 && pdata.characterInv[0] == 0) { // Only set default if new game and slot 0 is empty
        pdata.characterInv[0] = CHAR_SABER; // Default starting character ID
        Character starting_char = createCharacter(CHAR_SABER);
        saveCharacter(&starting_char); // Save the default character base stats
    }
    
    while (1) {
        printf("\n=== MAIN MENU ===\n1. Gacha\n2. Combat Demo\n3. Inventory (Equip Weapon)\n4. Exit\nChoice: ");
        int choice;
        if (scanf("%d", &choice) != 1) { while(getchar() != '\n'); continue; }

        if (choice == 1) {
            banner_menu(&pdata);
        } else if (choice == 2) {
            if (pdata.characterInv[0] == 0) {
                 printf("You need to pull a character first! Try the Gacha menu.\n");
                continue;
            }
            
            // FIX: Ensure correct CharacterType is used
            Character player = createCharacter(pdata.characterInv[0]); 
            loadCharacter(&player); 
            
            printf("Starting battle as %s (HP: %d ATK: %d DEF: %d SPD: %d). Equipped: %s\n",
                   player.name, player.hp, player.atk, player.def, player.spd, player.equippedWeapon.name);
            
            battle(&player);
            saveCharacter(&player); // Save stats after battle
            
            printf("\nPress Enter to continue...");
            getchar(); 
            getchar();
            
        } else if (choice == 3) {
            inventory_menu(&pdata);
        } else if (choice == 4) {
            save_player_data(&pdata);
            printf("\nExiting. Progress saved.\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}