//=================================================================
// The main program file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

// Projet includes
#include "globals.h"
#include "graphics.h"
#include "hardware.h"
#include "map.h"
#include "speech.h"
#include <math.h>


#define CITY_HIT_MARGIN 1
#define CITY_UPPER_BOUND (SIZE_Y - (LANDSCAPE_HEIGHT + MAX_BUILDING_HEIGHT))
#define YELLOW 0xFFFF00

// Helper function declarations
void playSound(char *wav);

/////////////////////////
// Struct of Player
/////////////////////////

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
int status_color;
int status = 0;
struct {
  int x, y;           // Current locations
  int px, py;         // Previous locations
  bool has_key;       // flag when obtained the key
  bool game_solved;   // flag when game is complete
  bool talked_to_npc; // flag when you've talked to npc
  bool talked_to_npc4;
  // extra stuff
  bool drop_Item; //yeet
  bool drop_Item2;
  bool speedMode;
  bool win_game;
  bool lose_game;
  bool special_skill;
  bool sacred_power;
  bool insta_kill;
  bool regular_skill;
  bool leave_fire;
  bool solve_quiz;
  bool walkwall;
  int health;
  int life;

  int steps;
  bool be_drunk;

  bool learn_1;
  bool learn_2;
  bool learn_3;
  bool learn_4;
  bool learn_5;

  // bool see_BOSS;

  // You will need to add more flags as needed

} Player;

struct {
  int x, y;           // Current locations
  int px, py;         // Previous locations
  // You will need to add more flags as needed

} Tree;
/////////////////////////
// Get Action
/////////////////////////

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define ATTACK_BUTTON 3
#define GO_LEFT 4
#define GO_RIGHT 5
#define GO_UP 6
#define GO_DOWN 7
#define GO_CENTER 8



int get_action(GameInputs inputs) {
  //******************
  // TODO: Implement
  //******************

  // 1. Check your action and menu button inputs and return the corresponding
  // action value
  if (inputs.b1) {
    return ATTACK_BUTTON;
  }
  if (inputs.b2) {
    return ACTION_BUTTON;
  }
  if (inputs.b3) {
      return MENU_BUTTON;
  }
  // 2. Check for your navigation switch inputs and return the corresponding
  // action value
  if (inputs.ns_up) {
    return GO_UP;
  }
  if (inputs.ns_right) {
    return GO_RIGHT;
  }
  if (inputs.ns_down) {
    return GO_DOWN;
  }
  if (inputs.ns_left) {
    return GO_LEFT;
  }
  if (inputs.ns_center) {
    return GO_CENTER;
  }
  // If no button is pressed, just return no action value
  return NO_ACTION;
}

/////////////////////////
// Helper Functions:
/////////////////////////
// Feel free to define any helper functions here for update game
void go_up() {
  Player.x = Player.px;
  Player.y = Player.py - 1;
  if (Player.learn_4) {
      Player.x = Player.px;
      Player.y = Player.py - 2;
  } else if (Player.be_drunk) {
    Player.x = Player.px;
    Player.y = Player.py + 1; 
    Player.steps++;
    if (Player.steps == 30) {
      Player.be_drunk = false;
      speech("You are sober", """");
    }
  } else if (Player.leave_fire) {
      add_fire(Player.x, Player.y + 1);
  }
}
void go_down() {
  Player.x = Player.px;
  Player.y = Player.py + 1;
  if (Player.learn_4) {
      Player.x = Player.px;
      Player.y = Player.py + 2;
  } else if (Player.be_drunk) {
    Player.x = Player.px;
    Player.y = Player.py - 1; 
    Player.steps++;
    if (Player.steps == 30) {
      Player.be_drunk = false;   
      speech("You are sober", """");
    }
  } else if (Player.leave_fire) {
      add_fire(Player.x, Player.y - 1);
  }
}
void go_left() {
  Player.x = Player.px - 1;
  Player.y = Player.py;
  if (Player.learn_4) {
      Player.x = Player.px - 2;
      Player.y = Player.py;
  } else if (Player.be_drunk) {
    Player.x = Player.px + 1;
    Player.y = Player.py;
    Player.steps++;
    if (Player.steps == 30) {
      Player.be_drunk = false; 
      speech("You are sober", """");
    } 
  } else if (Player.leave_fire) {
      add_fire(Player.x + 1, Player.y);
  }
}
void go_right() {
  Player.x = Player.px + 1;
  Player.y = Player.py;
  if (Player.learn_4) {
      Player.x = Player.px + 2;
      Player.y = Player.py;
  } else if (Player.be_drunk) {
    Player.x = Player.px - 1;
    Player.y = Player.py; 
    Player.steps++;
    if (Player.steps == 30) {
        Player.be_drunk = false;
        speech("You are sober", """");
    }
  } else if (Player.leave_fire) {
      add_fire(Player.x -1, Player.y);
  }
}


void draw_border()
{
    if (Player.learn_1 && Player.sacred_power) {
        status_color = RED;
    } else if (Player.learn_2 && Player.walkwall) {
        status_color = YELLOW;
    } else if (Player.learn_3) {
        status_color = GREEN;
    } else if (Player.learn_4) {
        status_color = BLUE;
    } else if (Player.learn_5) {
        status_color = 0xF05E16;
    } else {
        status_color = WHITE;
    }
  
    uLCD.filled_rectangle(0,     9, 127,  14, status_color); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, status_color); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, status_color); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, status_color); // Right
}
void hit_trap() {
    if (get_here(Player.x, Player.y)->type == TRAP) {
        Player.health -= 10;
        if (Player.health <= 0) {
            if (Player.life == 1) {
                Player.lose_game = true;
            } else {
                Player.life--;
                Player.health = 50 + Player.health;
            }
        }
        char* healthBar;
        char* lives;
        asprintf(&healthBar, "%d health left", Player.health);
        asprintf(&lives, "%d lives left", Player.life);
        const char* lines[] = {"Health: ", healthBar, "Lives:", lives};
        long_speech(lines, 4);
    }
}


/////////////////////////
// Update Game
/////////////////////////

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
int update_game(int action) {
  // Save player previous location before updating
  Player.px = Player.x;
  Player.py = Player.y;

  MapItem *item = NULL;

  //******************
  // TODO: Implement
  //******************

  // Do different things based on the each action.
  // You can define functions like "go_up()" that get called for each case.

  switch (action) {
  case GO_UP:
    // TODO: Implement
    // 1. Check the item north of the player
    item = get_north(Player.x, Player.y);
    if (item->walkable) {
      go_up();
    }
    if (item->type == WALL && Player.walkwall) {
        go_up();
    }
    hit_trap();

    // 2. Make sure to not walk through walls
    // 3. If it is not a wall, the walk up by updating player's coordinates
    break;

  case GO_LEFT:
    item = get_west(Player.x, Player.y);
    if (item->walkable) {
      go_left();
    }
    if (item->type == WALL && Player.walkwall) {
        go_left();
    }
    hit_trap();
    break;

  case GO_DOWN:
    item = get_south(Player.x, Player.y);
    if (item->walkable) {
      go_down();
    }
    if (item->type == WALL && Player.walkwall) {
        go_down();
    }
    hit_trap();
    break;

  case GO_RIGHT:
    item = get_east(Player.x, Player.y);
    if (item->walkable) {
      go_right(); 
    }
    if (item->type == WALL && Player.walkwall) {
        go_right();
    }
    hit_trap();
    break;

  case ACTION_BUTTON:

    //******************
    // TODO: Implement
    //******************

    // 1. Check if near NPC,
    //     - if so, mark the player has talked and give instructions on what to
    //     do
    //     - if the game is solved (defeated Buzz), give the player the key
    //     - return FULL_DRAW to redraw the scene
    if ((get_north(Player.x, Player.y)->type == NPC) ||
        (get_south(Player.x, Player.y)->type == NPC) ||
        (get_east(Player.x, Player.y)->type == NPC) ||
        (get_west(Player.x, Player.y)->type == NPC)) {
      if (!Player.talked_to_npc) {
        add_npc(10, 5, 2);  
        Player.talked_to_npc = true;
        const char *NPC_Speech[18] = {
            "U're finally here", "Our Lil Demon.",    "Demon Lord has",
            "been ruling Hell",  "Only you can",      "defeat him.",
            "First, defeat his", "minion, Archer",    "and give me",
            "The WALLET.",       "Then, fight Devam", "his 2nd minion",
            "and give me",       "The DEGREE.",       "Only then",
            "I'll teach u",      "The Sacred Spell",  "to defeat Boss."};
        long_speech(NPC_Speech, 18);
      } else if (!Player.game_solved && !Player.drop_Item &&
                 !Player.drop_Item2) {
        // Player.talked_to_npc = true;
        const char *NPC_Speech[7] = {"Oh u are back?",   "Defeat Archer",
                                     "and Devam",        "to bring me",
                                     "their items.",     "Then u'll learn",
                                     "the Sacred Spell."};
        long_speech(NPC_Speech, 7);
      } else if (!Player.game_solved && Player.drop_Item &&
                 !Player.drop_Item2) {
        const char *Special_Skill[3] = {"Thx for wallet!", "Now go defeat",
                                        "the evil Devam!"};
        long_speech(Special_Skill, 3);
      } else if (!Player.game_solved && !Player.drop_Item &&
                 Player.drop_Item2) {
        const char *Special_Skill[3] = {"Thx for degree!", "Now go defeat",
                                        "the vile Archer!"};
        long_speech(Special_Skill, 3);
      } else if (!Player.game_solved && Player.drop_Item && Player.drop_Item2) {
        add_npc(10, 5, 3); 
        const char *Special_Skill[6] = {"You have obtained", "The Sacred Power",
                                        "Now go defeat ",    "The Demon Lord",
                                        "And escape Hell",   "Good luck"};
        long_speech(Special_Skill, 6);
        Player.special_skill = true;
        Player.sacred_power = true;
      } else if (Player.game_solved) {
        add_npc(10, 5, 4);
        const char *NPC_thanks[5] = {"PEACE IS BACK!", "U r now free",
                                     "Here is the key", "to the door.",
                                     "Reward awaits u..."};
        long_speech(NPC_thanks, 5);
        Player.has_key = true;
        return FULL_DRAW;
      }
    }

    if ((get_north(Player.px, Player.py)->type == NPC4) ||
        (get_south(Player.px, Player.py)->type == NPC4) ||
        (get_east(Player.px, Player.py)->type == NPC4) ||
        (get_west(Player.px, Player.py)->type == NPC4)) {
        Player.talked_to_npc4 = true;
        const char *final_lines[] = {"Hey, I need help...", 
                                     "Archangel Hannah",
                                     "has been trapped",
                                     "behind the walls.",
                                     "Please rescue her!!!",
                                     "I'm injured, but..",
                                     "use my power,",
                                     "Blessing of Angel!",
                                     };
        long_speech(final_lines, 8);
        speech("You can now", "walk thru anything.");
        // end_speech();
        Player.walkwall = true;
        return FULL_DRAW;
    }

        if ((get_north(Player.px, Player.py)->type == NPC5) ||
        (get_south(Player.px, Player.py)->type == NPC5) ||
        (get_east(Player.px, Player.py)->type == NPC5) ||
        (get_west(Player.px, Player.py)->type == NPC5)) {
        const char *final_lines[] = {"Hey, it's RD yo.", 
                                     "U know who I am.",
                                     "Answer my quiz,",
                                     "it's very easy.",
                                     "If correct, u'll",
                                     "get a superpower!",
                                     "If wrong....",
                                     "well I would be sad",
                                     };
        long_speech(final_lines, 8);
        quiz_page();
        int input = NO_ACTION;
        while (input == NO_ACTION) {
            GameInputs quizChoose = read_inputs();
            input = get_action(quizChoose);
            switch(input) {
                case GO_LEFT:
                speech("U have obtained", "INSTA KILL");
                Player.solve_quiz = true;
                return FULL_DRAW;
                case GO_RIGHT:
                speech("Oh no, RD is sad.","You are dead.");
                Player.lose_game = true;
            }
        }

        // end_speech();

        return FULL_DRAW;
    }

        if ((get_north(Player.px, Player.py)->type == NPC2) ||
        (get_south(Player.px, Player.py)->type == NPC2) ||
        (get_east(Player.px, Player.py)->type == NPC2) ||
        (get_west(Player.px, Player.py)->type == NPC2)) {
        const char *final_lines[] = {"Wahh!?", 
                                     "Tina sent u here?",
                                     "Thanks Lil Demon",
                                     "for saving me.",
                                     "By the power",
                                     "of this Archangel,",
                                     "Here's the key",
                                     "to the gate!",
                                     "You don't have to",
                                     "kill anyone.",
                                     };
        long_speech(final_lines, 10);
        speech("You have obtained", "the key to the gate.");
        add_mud(42, 42, HORIZONTAL, 7);
        add_mud(42, 42, VERTICAL, 7);

        // end_speech();
        Player.has_key = true;
        return FULL_DRAW;
    }

    if ((get_north(Player.px, Player.py)->type == BOX) ||
        (get_south(Player.px, Player.py)->type == BOX) ||
        (get_east(Player.px, Player.py)->type == BOX) ||
        (get_west(Player.px, Player.py)->type == BOX)) {
        speech("Wow, a mystery box!", "What's inside?");
        speech2("ALCOHOL DROPPED", """");
        add_drink(4, 5);
    }
    
    if ((get_north(Player.px, Player.py)->type == DRINK) ||
        (get_south(Player.px, Player.py)->type == DRINK) ||
        (get_east(Player.px, Player.py)->type == DRINK) ||
        (get_west(Player.px, Player.py)->type == DRINK)) {
        speech("U drank booze,", "and get very drunk.");
        speech2("Ur movement is inverted", "for 30 steps");
        Player.be_drunk = true;
        Player.steps = 0;
    }
    

    // 2. Check if near a door
    //    - if the player has the key, you win the game
    //    - if not, show speech bubbles that the play needs to get the key
    //     -return FULL_DRAW to redraw the scene
    if ((get_north(Player.px, Player.py)->type == DOOR) ||
        (get_south(Player.px, Player.py)->type == DOOR) ||
        (get_east(Player.px, Player.py)->type == DOOR) ||
        (get_west(Player.px, Player.py)->type == DOOR)) {
      if (Player.has_key) {
        const char *lines[] = {"The reward's here!"};
        long_speech(lines, 1);
        // end_speech();
        // Player.win_game = true;
        add_door_open(33, 10, HORIZONTAL, 4);
        return FULL_DRAW;
      } else if (Player.talked_to_npc) {
        const char *lines[] = {"I need a key..."};
        long_speech(lines, 1);
        return FULL_DRAW;
      } else if (!Player.talked_to_npc) {
        speech("What is behind", "this door?");
        return FULL_DRAW;
      }
    }

    if ((get_north(Player.px, Player.py)->type == REWARD) ||
        (get_south(Player.px, Player.py)->type == REWARD) ||
        (get_east(Player.px, Player.py)->type == REWARD) ||
        (get_west(Player.px, Player.py)->type == REWARD)) {
      if (Player.has_key) {
        const char *final_lines[] = {"U've dropped out!", "Follow ur dream,",
                                     "And be a DJ!"};
        long_speech(final_lines, 3);
        // end_speech();
        Player.win_game = true;
        return FULL_DRAW;
      }
    }

    // 3. Check if on Pete's cave
    //    - if the player has talked to the npc, then start the speech bubble to
    //    fight buzz
    //    - set the players coordinates for the small map
    //    - and set the map to the small map
    //     -return FULL_DRAW to redraw the scene

    item = get_here(Player.x, Player.y);
    if (item->type == CAVE && Player.talked_to_npc) {
      speech("Entering cave,", "Be careful");
      StairsData *data = (StairsData *)item->data;
      Player.x = data->tx;
      Player.y = data->ty;
      set_active_map(1);
      return FULL_DRAW;
    }

    // 4. Check if on a stairs
    //    - if so, move the player the correct coordinates
    //    - and set the map back to the main big map
    //     -return FULL_DRAW to redraw the scene
    if (item->type == STAIRS) {
      speech("Climbing stairs,", "Retreat!");
      StairsData *data = (StairsData *)item->data;
      Player.x = data->tx;
      Player.y = data->ty;
      set_active_map(0);
      return FULL_DRAW;
    }

    if (item->type == STAIRS2) {
      speech("That's a cute cat,", "Wait wth?");
      StairsData *data = (StairsData *)item->data;
      set_active_map(0);
      Player.x = 12;
      Player.y = 44;
      return FULL_DRAW;
    }

    item = get_south(Player.x, Player.y);
    if (item->type == CAT1) {
      speech("Meow, wrong cat!,", "You are dead~");
      Player.lose_game = true;
      return FULL_DRAW;
    } else if (item->type == CAT3) {
        speech("Nyan, u found me~", "Back to reality~");
        StairsData *data = (StairsData *)item->data;
        Player.x = 8;
        Player.y = 5;
        set_active_map(0);
        return FULL_DRAW;
    }

    // Feel free to add more functions as you make the game!
    break;

  case ATTACK_BUTTON:

    //******************
    // TODO: Implement
    //******************

    // 1. Check if near enemy,
    //     - if so, mark enemy as defeated
    //      use speech bubbled to show what item was dropped
    //      update the player struct as needed
    //      if enemy is Pete, make sure that the right attack is being used
    //      If pete is defeated, update game as nescessary

    if (Player.regular_skill) {
      if (get_north(Player.px, Player.py) != NULL &&
          get_north(Player.px, Player.py)->type == ENEMY1) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("ARCHER SLAIN", "Wallet dropped");
        const char *Boss_Speech[4] = {"HAHAHAHAHA", "U will never",
                                      "beat DEMON LORD!", "GIVE UP U FOOL!"};
        long_speech(Boss_Speech, 4);
        add_slain_enemy(Player.x, Player.y - 1);
        Player.drop_Item = true;

      } else if (get_east(Player.px, Player.py) != NULL &&
                 get_east(Player.px, Player.py)->type == ENEMY1) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("ARCHER SLAIN", "Wallet dropped");
        const char *Boss_Speech[4] = {"HAHAHAHAHA", "U will never",
                                      "beat DEMON LORD!", "GIVE UP U FOOL!"};
        long_speech(Boss_Speech, 4);
        add_slain_enemy(Player.x + 1, Player.y);
        Player.drop_Item = true;

      } else if (get_south(Player.px, Player.py) != NULL &&
                 get_south(Player.px, Player.py)->type == ENEMY1) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("ARCHER SLAIN", "Wallet dropped");
        const char *Boss_Speech[4] = {"HAHAHAHAHA", "U will never",
                                      "beat DEMON LORD!", "GIVE UP U FOOL!"};
        long_speech(Boss_Speech, 4);
        add_slain_enemy(Player.x, Player.y + 1);
        Player.drop_Item = true;

      } else if (get_west(Player.px, Player.py) != NULL &&
                 get_west(Player.px, Player.py)->type == ENEMY1) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("ARCHER SLAIN", "Wallet dropped");
        const char *Boss_Speech[4] = {"HAHAHAHAHA", "U will never",
                                      "beat DEMON LORD!", "GIVE UP U FOOL!"};
        long_speech(Boss_Speech, 4);
        add_slain_enemy(Player.x - 1, Player.y);
        Player.drop_Item = true;
      }

      if (get_north(Player.px, Player.py) != NULL &&
          get_north(Player.px, Player.py)->type == ENEMY2) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("DEVAM SLAIN", "Degree dropped");
        const char *Boss_Speech[4] = {"Forgive me", "my Lord...",
                                      "I cant avenge u.", "This is my end"};
        long_speech(Boss_Speech, 4);
        add_slain_enemy(Player.x, Player.y - 1);
        Player.drop_Item2 = true;

      } else if (get_east(Player.px, Player.py) != NULL &&
                 get_east(Player.px, Player.py)->type == ENEMY2) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("DEVAM SLAIN", "Degree dropped");
        const char *Boss_Speech[4] = {"Forgive me", "my Lord...",
                                      "I cant avenge u.", "This is my end"};
        long_speech(Boss_Speech, 4);
        add_slain_enemy(Player.x + 1, Player.y);
        Player.drop_Item2 = true;

      } else if (get_south(Player.px, Player.py) != NULL &&
                 get_south(Player.px, Player.py)->type == ENEMY2) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("DEVAM SLAIN", "Degree dropped");
        const char *Boss_Speech[4] = {"Forgive me", "my Lord...",
                                      "I cant avenge u.", "This is my end"};
        long_speech(Boss_Speech, 4);
        add_slain_enemy(Player.x, Player.y + 1);
        Player.drop_Item2 = true;

      } else if (get_west(Player.px, Player.py) != NULL &&
                 get_west(Player.px, Player.py)->type == ENEMY2) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("DEVAM SLAIN", "Degree dropped");
        const char *Boss_Speech[4] = {"Forgive me", "my Lord...",
                                      "I cant avenge u.", "This is my end"};
        long_speech(Boss_Speech, 4);
        add_slain_enemy(Player.x - 1, Player.y);
        Player.drop_Item2 = true;
      }

      if (get_north(Player.px, Player.py) != NULL &&
          get_north(Player.px, Player.py)->type == BUZZ) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("ATTACK", "INEFFECTIVE");
        const char *Boss_Speech[5] = {"HAHAHAHAHA!", "Ur attack",
                                      "wont work on me.", "BECAUSE I AM",
                                      "THE DEMON LORD!!!"};
        long_speech(Boss_Speech, 5);

      } else if (get_east(Player.px, Player.py) != NULL &&
                 get_east(Player.px, Player.py)->type == BUZZ) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("ATTACK", "INEFFECTIVE");
        const char *Boss_Speech[5] = {"HAHAHAHAHA!", "Ur attack",
                                      "wont work on me.", "BECAUSE I AM",
                                      "THE DEMON LORD!!!"};
        long_speech(Boss_Speech, 5);

      } else if (get_south(Player.px, Player.py) != NULL &&
                 get_south(Player.px, Player.py)->type == BUZZ) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("ATTACK", "INEFFECTIVE");
        const char *Boss_Speech[5] = {"HAHAHAHAHA!", "Ur attack",
                                      "wont work on me.", "BECAUSE I AM",
                                      "THE DEMON LORD!!!"};
        long_speech(Boss_Speech, 5);

      } else if (get_west(Player.px, Player.py) != NULL &&
                 get_west(Player.px, Player.py)->type == BUZZ) {
        speech("Take this!", "REGULAR ATTACK");
        speech2("ATTACK", "INEFFECTIVE");
        const char *Boss_Speech[5] = {"HAHAHAHAHA!", "Ur attack",
                                      "wont work on me.", "BECAUSE I AM",
                                      "THE DEMON LORD!!!"};
        long_speech(Boss_Speech, 5);
      }
    }

    if (Player.learn_1) {
        if (Player.special_skill) {
            if (get_north(Player.px, Player.py) != NULL &&
                get_north(Player.px, Player.py)->type == BUZZ) {
                if (Player.sacred_power) {
                speech("Sacred Power", "UNLEASHED!");
                speech2("BOSS SLAIN,", "Tuition refunded!");
                add_slain_enemy(Player.x, Player.y - 1);
                Player.game_solved = true;
                add_fire2(Player.x + 1, Player.y);
                add_fire2(Player.x - 1, Player.y);
                add_fire2(Player.x, Player.y + 1);
                add_fire2(Player.x, Player.y - 1);
                }

            } else if (get_east(Player.px, Player.py) != NULL &&
                get_east(Player.px, Player.py)->type == BUZZ) {
                if (Player.sacred_power) {
                speech("Sacred Power", "UNLEASHED!");
                speech2("BOSS SLAIN,", "Tuition refunded!");
                add_slain_enemy(Player.x, Player.y - 1);
                Player.game_solved = true;
                add_fire2(Player.x + 1, Player.y);
                add_fire2(Player.x - 1, Player.y);
                add_fire2(Player.x, Player.y + 1);
                add_fire2(Player.x, Player.y - 1);
                }

            } else if (get_south(Player.px, Player.py) != NULL &&
                get_south(Player.px, Player.py)->type == BUZZ) {
                if (Player.sacred_power) {
                speech("Sacred Power", "UNLEASHED!");
                speech2("BOSS SLAIN,", "Tuition refunded!");
                add_slain_enemy(Player.x, Player.y - 1);
                Player.game_solved = true;
                add_fire2(Player.x + 1, Player.y);
                add_fire2(Player.x - 1, Player.y);
                add_fire2(Player.x, Player.y + 1);
                add_fire2(Player.x, Player.y - 1);
                }

            } else if (get_west(Player.px, Player.py) != NULL &&
                 get_west(Player.px, Player.py)->type == BUZZ) {
                if (Player.sacred_power) {
                speech("Sacred Power", "UNLEASHED!");
                speech2("BOSS SLAIN,", "Tuition refunded!");
                add_slain_enemy(Player.x, Player.y - 1);
                Player.game_solved = true;
                add_fire2(Player.x + 1, Player.y);
                add_fire2(Player.x - 1, Player.y);
                add_fire2(Player.x, Player.y + 1);
                add_fire2(Player.x, Player.y - 1);
                }
            }
            if (get_north(Player.px, Player.py) != NULL &&
                get_north(Player.px, Player.py)->type == ENEMY2) {
                speech("Take this!", "SPECIAL ATTACK");
                speech2("DEVAM SLAIN", "BEGONE");
                const char *Boss_Speech[4] = {"Forgive me", "my Lord...",
                                            "I cant avenge u.", "This is my end"};
                long_speech(Boss_Speech, 4);
                add_slain_enemy(Player.x, Player.y - 1);

            } else if (get_east(Player.px, Player.py) != NULL &&
                       get_east(Player.px, Player.py)->type == ENEMY2) {
                speech("Take this!", "SPECIAL ATTACK");
                speech2("DEVAM SLAIN", "BEGONE");
                const char *Boss_Speech[4] = {"Forgive me", "my Lord...",
                                            "I cant avenge u.", "This is my end"};
                long_speech(Boss_Speech, 4);
                add_slain_enemy(Player.x + 1, Player.y);

            } else if (get_south(Player.px, Player.py) != NULL &&
                       get_south(Player.px, Player.py)->type == ENEMY2) {
                speech("Take this!", "SPECIAL ATTACK");
                speech2("DEVAM SLAIN", "BEGONE");
                const char *Boss_Speech[4] = {"Forgive me", "my Lord...",
                                            "I cant avenge u.", "This is my end"};
                long_speech(Boss_Speech, 4);
                add_slain_enemy(Player.x, Player.y + 1);

            } else if (get_west(Player.px, Player.py) != NULL &&
                       get_west(Player.px, Player.py)->type == ENEMY2) {
                speech("Take this!", "SPECIAL ATTACK");
                speech2("DEVAM SLAIN", "BEGONE!!!");
                const char *Boss_Speech[4] = {"Forgive me", "my Lord...",
                                            "I cant avenge u.", "This is my end"};
                long_speech(Boss_Speech, 4);
                add_slain_enemy(Player.x - 1, Player.y);
            }
        }
        Player.learn_1 = false;
        Player.regular_skill = true;
    }

    if (Player.learn_2) {
        if (action == ATTACK_BUTTON && !Player.walkwall && Player.talked_to_npc4) {
            Player.walkwall = true;
            speech("I now can pass","through anything!");
            return FULL_DRAW;
        } else if (action == ATTACK_BUTTON && Player.walkwall && Player.talked_to_npc4) {
            Player.walkwall = false;
            speech("Deactivate Power", """");
            return FULL_DRAW;
        } else {
            speech("Power not unlocked", """");
            return FULL_DRAW;
        }
        Player.learn_2 = false;
        Player.regular_skill = true;
    }

    if (Player.learn_3) {
         if (action == ATTACK_BUTTON && Player.solve_quiz == true) {
             Player.insta_kill = true;
             speech("Insta Kill", "all small mobs");
             add_mud(11, 8, HORIZONTAL, 7);
             add_mud(11, 7, HORIZONTAL, 7);
             Player.insta_kill = false;
         } else {
            speech("Power not unlocked", """");
            return FULL_DRAW;
         }
    }

    if (Player.learn_4) {
        if (action == ATTACK_BUTTON && !Player.speedMode) {
            Player.speedMode = true;
            speech("I am now faster", """");
            return FULL_DRAW;
        } else if (action == ATTACK_BUTTON && Player.speedMode) {
            Player.speedMode = false;
            Player.learn_4 = false;
            speech("Deactivate speed", """");
            return FULL_DRAW;
        }
    }

    if (Player.learn_5) {
        if (action == ATTACK_BUTTON) {
        Player.leave_fire = true;
            // add_firewall(Player.x+1, Player.y, HORIZONTAL, 4);
            // speech("Roar","""");
        // } else if (action == ATTACK_BUTTON) {
        //     add_firewall(Player.x, Player.y + 1, VERTICAL, 4);
        //     speech("Roar","""");
        // } else if (action == GO_RIGHT ) {
        //     add_firewall(Player.x -1 , Player.y, HORIZONTAL, 4);
        //     speech("Roar","""");
        // } else if (action == GO_LEFT) {
        //     add_firewall(Player.x + 1, Player.y, HORIZONTAL, 4);
        //     speech("Roar","""");
        return FULL_DRAW;
        } else if (action == ATTACK_BUTTON && Player.leave_fire) {
            Player.leave_fire = false;
            Player.learn_5 = false;
            speech("Deactivate", "fire path");
            return FULL_DRAW;
        }
    }
    // break;

    return FULL_DRAW;
    break;

  //***********
  // Add more cases as needed
  //***********
  case MENU_BUTTON:
    int save_pos = get_active_map_index();
    menu_page();
    int input = NO_ACTION;
    while (input == NO_ACTION) {
        GameInputs decision = read_inputs();
        input = get_action(decision);
        switch(input) {
            case GO_UP:
                set_active_map(save_pos);
                Player.x = Player.px;
                Player.y = Player.py;
                return FULL_DRAW;
                break;
            case GO_DOWN:
                 __NVIC_SystemReset();
                 break;
            case GO_LEFT:
                skill_page();
                int input2 = NO_ACTION;
                while (input2 == NO_ACTION) {
                    GameInputs skillChoose = read_inputs();
                    input2 = get_action(skillChoose);
                    switch(input2) {
                        case GO_UP:
                            Player.learn_1 = true;
                            Player.learn_2 = false;
                            Player.learn_3 = false;
                            Player.learn_4 = false;
                            Player.regular_skill = false;
                            speech("Sacred Power ON", """");
                            return FULL_DRAW;
                        case GO_RIGHT:
                            Player.learn_2 = true;
                            Player.learn_1 = false;
                            Player.learn_3 = false;
                            Player.learn_4 = false;
                            Player.learn_5 = false;
                            Player.regular_skill = false;
                            speech("Angel Blessing ON", """");
                            return FULL_DRAW;
                        case GO_LEFT:
                            Player.learn_3 = true;
                            Player.learn_1 = false;
                            Player.learn_2 = false;
                            Player.learn_4 = false;
                            Player.learn_5 = false;
                            Player.regular_skill = false;
                            speech("Insta Kill ON", """");
                            return FULL_DRAW;
                        case GO_DOWN:
                            Player.learn_4 = true;
                            Player.learn_2 = false;
                            Player.learn_3 = false;
                            Player.learn_1 = false;
                            Player.learn_5 = false;
                            speech("Speeder ON", """");
                            return FULL_DRAW;
                        case GO_CENTER:
                            Player.learn_5 = true;
                            Player.learn_4 = false;
                            Player.learn_2 = false;
                            Player.learn_3 = false;
                            Player.learn_1 = false;
                            speech("Hellfire ON", """");
                            return FULL_DRAW;
                        case ATTACK_BUTTON:
                            Player.learn_5 = false;
                            Player.leave_fire = false;
                            Player.learn_4 = false;
                            Player.speedMode = false;
                            Player.learn_2 = false;
                            Player.walkwall = false;
                            Player.learn_3 = false;
                            Player.learn_1 = false;
                            speech("DEACTIVATE SKILLS", """");
                            return FULL_DRAW;
                    }
                }
        }
    }
    return FULL_DRAW;
    break;
  }

  return NO_RESULT;
}

/////////////////////////
// Draw Game
/////////////////////////

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init) {
  // Draw game border first
  if (init)
    draw_border();

  // Iterate over all visible map tiles
  for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
  {
    for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
    {
      // Here, we have a given (i,j)

      // Compute the current map (x,y) of this tile
      int x = i + Player.x;
      int y = j + Player.y;

      // Compute the previous map (px, py) of this tile
      int px = i + Player.px;
      int py = j + Player.py;

      // Compute u,v coordinates for drawing
      int u = (i + 5) * 11 + 3;
      int v = (j + 4) * 11 + 15;

      // Figure out what to draw
      DrawFunc draw = NULL;
      //            if (init && i == 0 && j == 0) // Only draw the player on
      //            init
      if (i == 0 && j == 0) // always draw the player
      {
        MapItem* coveredThing = get_here(x, y);
        if (coveredThing->type == PLANT) {
            draw = coveredThing->draw;
            if (draw) draw(u,v);
        } else {
        draw_player(u, v, Player.has_key);
        }
        continue;
      } else if (x >= 0 && y >= 0 && x < map_width() &&
                 y < map_height()) // Current (i,j) in the map
      {
        MapItem *curr_item = get_here(x, y);
        MapItem *prev_item = get_here(px, py);
        if (init || curr_item != prev_item) // Only draw if they're different
        {
          if (curr_item) // There's something here! Draw it
          {
            draw = curr_item->draw;
          } else // There used to be something, but now there isn't
          {
            draw = draw_nothing;
          }
        } else if (curr_item && curr_item->type == CLEAR) {
          // This is a special case for erasing things like doors.
          draw = curr_item->draw; // i.e. draw_nothing
        }
      } else if (init) // If doing a full draw, but we're out of bounds, draw
                       // the walls.
      {
        draw = draw_wall;
      }

      // Actually draw the tile
      if (draw)
        draw(u, v);
    }
  }

  // Draw status bars
  draw_upper_status(Player.x, Player.y);
  draw_lower_status(Player.health, Player.steps);
  //draw_border();

}

/////////////////////////
// Map Intialization
/////////////////////////

// Important locations for all maps
int cb_loc[2] = {5, 20}; // Location of the center of the cave

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map() {
  // Initialize and sets current map to the first map
  Map *map = set_active_map(0);

  /////////////////////////
  // Initial Environmnet
  /////////////////////////

  // Adding random plants
  pc.printf("Adding Plants!\r\n");
  for (int i = map_width() + 3; i < map_area(); i += 39) {
    add_plant(i % map_width(), i / map_width());
  }
  for (int i = map_width() + 3; i < map_area(); i += 31) {
    add_fire(i / map_width(), i % map_width());
  }

  // Adding wall borders
  pc.printf("Adding walls!\r\n");
  add_wall(0, 0, HORIZONTAL, map_width());
  add_wall(0, map_height() - 1, HORIZONTAL, map_width());
  add_wall(0, 0, VERTICAL, map_height());
  add_wall(map_width() - 1, 0, VERTICAL, map_height());

  // Adding extra chamber borders
  pc.printf("Add extra chamber\r\n");
  add_wall(30, 0, VERTICAL, 10);
  add_wall(30, 10, HORIZONTAL, 10);
  add_wall(39, 0, VERTICAL, 10);
  add_door(33, 10, HORIZONTAL, 4);

  // Adding wall that locks Angel
  add_catwall(7, 40, HORIZONTAL, 9);
  add_catwall(7, 40, VERTICAL, 9);
  add_catwall(16, 40, VERTICAL, 9);
  add_wall(42, 42, HORIZONTAL, 7);
  add_wall(42, 42, VERTICAL, 7);

  // Adding extra cave to Buzz's evil lair
  pc.printf("Add cave\r\n");
  add_cave(cb_loc[0], cb_loc[1], 1, 1, 5,
           5); // Cave is set as a 4x4 block to be bigger
  add_cave(cb_loc[0] + 1, cb_loc[1], 2, 1, 5, 5);
  add_cave(cb_loc[0], cb_loc[1] + 1, 3, 1, 5, 5);
  add_cave(cb_loc[0] + 1, cb_loc[1] + 1, 4, 1, 5, 5);

  pc.printf("Initial environment completed\r\n");

  /////////////////////////////////
  // Characters and Items for the map
  /////////////////////////////////

  // Add NPC
  add_npc(10, 5, 1); // NPC is initialized to (x,y) = 10, 5. Feel free to move him around
  add_cave(10, 10, 2, 1, 5, 5);
  add_enemy1(13, 6);
  add_enemy2(14, 7);
  add_npc2(45, 45);
  add_npc4(10,7);
  add_npc5(6,7);
  add_reward(35, 5);
  add_box(4, 4);
  //take to 3rd map
  add_stairs2(20, 10, 2, 14, 6);
  
  add_npc3(13, 44);
  add_cat1(13, 48);
  add_cat2(11, 48);
  add_cat3(9, 48);
  //teleport behind boss
  add_cave(10, 12, 2, 1, 15, 5);
  //***********************************
  // TODO: Implement As Needed
  //***********************************

  // Add any extra characters/items here for your project

  // Prints out map
  print_map();
}

void init_small_map() {
  // Sets current map to the second map
  set_active_map(1);

  //***********************************
  // TODO: Implement
  //***********************************

  // 1. Add walls to the smaller map.
  //    Set the dimensions to be 16x16  <-- you may change to your liking, but
  //    must be smaller than the main map Hint: See how walls are initialized in
  //    the main map
  //
  pc.printf("Adding walls!\r\n");
  // add_wall(0,              0,              HORIZONTAL, map_width());
  // add_wall(0,              map_height()-1, HORIZONTAL, map_width());
  // add_wall(0,              0,              VERTICAL,   map_height());
  // add_wall(map_width()-1,  0,              VERTICAL,   map_height());

  add_firewall(0, 0, HORIZONTAL, map_width());
  add_firewall(0, map_height() - 1, HORIZONTAL, map_width());
  add_firewall(0, 0, VERTICAL, map_height());
  add_firewall(map_width() - 1, 0, VERTICAL, map_height());
  // 2. Add a way to access your specail attacks either here or in update_game()
  // or anywhere you feel would be the best
  //
  // 3. Add Boss in the map
  add_firewall(10, 0, VERTICAL, 10);

  for (int i = map_width() + 3; i < map_area(); i += 39) {
    add_trap(i % map_width(), i / map_width());
  }
  add_mud(4, 12, HORIZONTAL, 6);
  add_mud(8, 12, HORIZONTAL, 6);
  add_mud(12, 12, HORIZONTAL, 6);
  
  add_enemy2(11,8);
  add_enemy2(12, 8);
  add_enemy2(13,8);
  add_enemy2(14, 8);
  add_enemy2(15,8);
  add_enemy2(16,8);
  add_enemy2(17,8);

  add_enemy2(11,7);
  add_enemy2(12, 7);
  add_enemy2(13,7);
  add_enemy2(14, 7);
  add_enemy2(15,7);
  add_enemy2(16,7);
  add_enemy2(17,7);

  add_buzz(14, 5);
  
  print_map();

  // You may add any extra characters/items here for your project

  // Add stairs back to main (map 0)
  add_stairs(4, 6, 0, cb_loc[0], cb_loc[1]);
}

void init_third_map() {
  // Sets current map to the second map
  set_active_map(2);

  //***********************************
  // TODO: Implement
  //***********************************

  // 1. Add walls to the smaller map.
  //    Set the dimensions to be 16x16  <-- you may change to your liking, but
  //    must be smaller than the main map Hint: See how walls are initialized in
  //    the main map
  //
  pc.printf("Adding walls!\r\n");
  // add_wall(0,              0,              HORIZONTAL, map_width());
  // add_wall(0,              map_height()-1, HORIZONTAL, map_width());
  // add_wall(0,              0,              VERTICAL,   map_height());
  // add_wall(map_width()-1,  0,              VERTICAL,   map_height());

  add_firewall(0, 0, HORIZONTAL, map_width());
  add_firewall(0, map_height() - 1, HORIZONTAL, map_width());
  add_firewall(0, 0, VERTICAL, map_height());
  add_firewall(map_width() - 1, 0, VERTICAL, map_height());
  // 2. Add a way to access your specail attacks either here or in update_game()
  // or anywhere you feel would be the best
  //
  // 3. Add Boss in the map
 
  add_npc3(14, 5);
  print_map();

  // You may add any extra characters/items here for your project

  // Add stairs back to main (map 0)
  add_stairs(4, 6, 0, cb_loc[0], cb_loc[1]);
}

//map3 

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main() {
  // First things first: initialize hardware
  ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

  // uLCD.filled_rectangle(64, 64, 74, 74, BLACK); //DELETE OR COMMENT THIS LINE
  // <-- It is a temporary indicator that your LCD is working before you
  // implement your code uLCD.locate(0, 1);
  //  uLCD.cls();
  //  uLCD.locate(3, 0);
  //  uLCD.text_width(2);
  //  uLCD.text_height(2);
  //  uLCD.color(RED);
  //  uLCD.printf("WELCOME");
  //  uLCD.color(GREEN);
  //  uLCD.locate(3, 8);
  //  uLCD.printf("TO HELL");

  uLCD.text_width(2);
  uLCD.text_height(2);
  uLCD.text_string("WELCOME", 1, 0, FONT_7X8, WHITE);

  uLCD.text_width(1);
  uLCD.text_height(2);
  uLCD.text_string("  TO HELL OF", 2, 2, FONT_7X8, RED);
  uLCD.text_string(" GEORGIA TECH ", 2, 4, FONT_7X8, 0xffd700);

  uLCD.text_width(1);
  uLCD.text_height(1);
  uLCD.text_string("   Press Button ", 0, 11, FONT_7X8, GREEN);
  uLCD.text_string("  To Begin ", 3, 14, FONT_7X8, GREEN);

  // Initialize the maps
  while (1) {
    GameInputs exit = read_inputs();
    if (get_action(exit)) {
      break;
    }
  }
  start_page();
  int input = NO_ACTION;
    while (input == NO_ACTION) {
        GameInputs decision = read_inputs();
        input = get_action(decision);
        switch(input) {
            case GO_UP:
                ctrl_page();
                while (1) {
                GameInputs exit = read_inputs();
                    if (get_action(exit)) {
                    break;
                    }
                }
            case GO_RIGHT:
                story_page();
                while (1) {
                GameInputs exit = read_inputs();
                    if (get_action(exit)) {
                    break;
                    }
                }
            case GO_DOWN:
                break;
        }
    }
  uLCD.filled_rectangle(0, 0, 127, 127, BLACK);
  maps_init();
  init_main_map();
  init_small_map();

  // Initialize game state
  set_active_map(0);
  Tree.x = Tree.y = 10;
  Player.x = Player.y = 5;
  Player.has_key = false;
  Player.game_solved = false;
  Player.talked_to_npc = false;
  Player.talked_to_npc4 = false;
  Player.leave_fire = false;
  Player.be_drunk = false;
  Player.drop_Item = false;
  Player.drop_Item2 = false;
  Player.speedMode = false;
  Player.special_skill = false;
  Player.regular_skill = true;
  Player.sacred_power = false;
  Player.walkwall = false;
  Player.solve_quiz = false;
  Player.health = 50;
  Player.life = 2;

  Player.steps = 0;

  Player.learn_1 = false;
  Player.learn_2 = false;
  Player.learn_3 = false;
  Player.learn_4 = false;

  // Initial drawing
  draw_game(true);

  // Main game loop
  while (1) {
    ////////////////////////////////
    // TODO: Implement
    ////////////////////////////////

    // Timer to measure game update speed
    Timer t;
    t.start();

    // Actually do the game update:
    // 1. Read inputs
    GameInputs input = read_inputs();
    int main_action = get_action(input);
    int result = update_game(main_action);

    // 2. Determine action (get_action)

    // 3. Update game (update_game)
    // Set this variable "result" for the resulting state after update game

    // 3b. Check for game over based on update game result
    if (result == GAME_OVER) {
    }
    draw_game(result);
    // 4. Draw screen to uLCD
    bool full_draw = false;
    if (result == FULL_DRAW)
      full_draw = true;
    draw_game(full_draw);

    // 5. Frame delay
    t.stop();
    int dt = t.read_ms();
    if (dt < 100) {
      wait_ms(100 - dt);
    }
    if (Player.win_game) {
      uLCD.filled_rectangle(0, 0, 127, 127, BLACK);
      // uLCD.locate(0, 1);
      uLCD.text_width(1);
      uLCD.text_height(2);
      uLCD.text_string("____________", 3, 1, FONT_7X8, 0xffd700);
      uLCD.text_string("U DROP OUT!!!", 3, 3, FONT_7X8, RED);
      uLCD.text_width(1);
      uLCD.text_string("GAME OVER", 5, 5, FONT_7X8, GREEN);
      uLCD.text_string("____________", 3, 6, FONT_7X8, 0xffd700);
      while (1) {
        GameInputs exit2 = read_inputs();
        if (get_action(exit2)) {
          __NVIC_SystemReset();
        }
      }
    } else if (Player.lose_game) {
      uLCD.filled_rectangle(0, 0, 127, 127, BLACK);
      // uLCD.locate(0, 1);
      uLCD.text_width(1);
      uLCD.text_height(2);
      uLCD.text_string("____________", 3, 1, FONT_7X8, 0xffd700);
      uLCD.text_string("U ARE DEAD!!!", 3, 3, FONT_7X8, RED);
      uLCD.text_width(1);
      uLCD.text_string("GAME OVER", 5, 5, FONT_7X8, GREEN);
      uLCD.text_string("____________", 3, 6, FONT_7X8, 0xffd700);
      while (1) {
        GameInputs exit3 = read_inputs();
        if (get_action(exit3)) {
          __NVIC_SystemReset();
        }
      }
    }
  }
}

/////////////////////////////
// Advanced Features
/////////////////////////////

// Plays a wavfile
void playSound(char *wav) {
  // open wav file
  FILE *wave_file;
  wave_file = fopen(wav, "r");

  if (wave_file != NULL) {
    printf("File opened successfully\n");

    // play wav file
    printf("Sound playing...\n");
    waver.play(wave_file);

    // close wav file
    printf("Sound stopped...\n");
    fclose(wave_file);
    return;
  }

  printf("Could not open file for reading - %s\n", wav);
  return;
}
