//=================================================================
// The map header file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#ifndef MAP_H
#define MAP_H

#include "hash_table.h"

/**
 * A structure to represent the map. The implementation is private.
 */
struct Map;

// A function pointer type for drawing MapItems.
// All tiles are 11x11 blocks.
// u,v is the top left corner pixel of the block
typedef void (*DrawFunc)(int u, int v);

/**
 * The data for elements in the map. Each item in the map HashTable is a
 * MapItem.
 */
typedef struct {
    /**
     * Indicates the "type" of the MapItem: WALL, DOOR, PLANT, etc. This is
     * useful for determining how to interact with the object when updating the
     * game state.
     */
    int type;
    
    /**
     * A function pointer to the drawing function for this item. Used by draw_game.
     */
    DrawFunc draw;
    
    /**
     * If zero, this item should block character motion.
     */
    int walkable;

    
    int droppedItem;    // defines if this MapItem drops an item when defeated
    
    /**
     * Arbitrary extra data for the MapItem. Could be useful for keeping up with
     * special information, like where a set of stairs should take the player.
     * 
     * Iterpretation of this can depend on the type of the MapItem. For example,
     * a WALL probably doesn't need to use this (it can be NULL), where an NPC
     * might use it to store game state (have I given the player the key yet?).
     */
    void* data;
} MapItem;

typedef struct {
    int tm;
    int tx, ty;
} StairsData;

typedef struct {
    
} CaveData;

// MapItem types
// Define more of these!
#define WALL    0
#define DOOR    1
#define DOOR_OPEN 16
#define REWARD 17
#define FIREWALL 18
#define CATWALL 21
#define PLANT   2
#define BIGCAT 30
#define TRAP   3
#define KEY     4
#define CHEST   5
#define NPC     6
#define NPC2   19
#define NPC3   20
#define NPC4   23
#define NPC5   29
#define CLEAR   7
#define STAIRS  8
#define STAIRS2 22
#define CAVE 9
#define MUD 10
#define FIRE 11
#define FIRE2 12
#define BUZZ 13
#define ENEMY1 14
#define ENEMY2 15

#define DRINK 27
#define BOX 28

#define CAT1 24
#define CAT2 25
#define CAT3 26

/**
 * Initializes the internal structures for all maps. This does not populate
 * the map with items, but allocates space for them, initializes the hash tables, 
 * and sets the width and height.
 */
void maps_init();

/**
 * Returns a pointer to the active map.
 */
Map* get_active_map();

/**
 * Sets the active map to map m, where m is the index of the map to activate.
 * Returns a pointer to the new active map.
 */
Map* set_active_map(int m);

/**
 * Returns the map m, regardless of whether it is the active map. This function
 * does not change the active map.
 */
Map* get_map(int m);

/**
 * Print the active map to the serial console.
 */
void print_map();

// Access
/**
 * Returns the width of the active map.
 */
int map_width();

/**
 * Returns the heigh of the active map.
 */
int map_height();

/**
 * Returns the total number of cells in the active map.
 */
int map_area();

/**
 * Returns the MapItem immediately above the given location.
 */
MapItem* get_north(int x, int y);

/**
 * Returns the MapItem immediately below the given location.
 */
MapItem* get_south(int x, int y);

/**
 * Returns the MapItem immediately to the right of the given location.
 */
MapItem* get_east(int x, int y);

/**
 * Returns the MapItem immediately to the left of  the given location.
 */
MapItem* get_west(int x, int y);

/**
 * Returns the MapItem at the given location.
 */
MapItem* get_here(int x, int y);

// Directions, for using the modification functions
#define HORIZONTAL  0
#define VERTICAL    1

/**
 * If there is a MapItem at (x,y), remove it from the map.
 */
void map_erase(int x, int y);

/**
 * Add WALL items in a line of length len beginning at (x,y).
 * If dir == HORIZONTAL, the line is in the direction of increasing x.
 * If dir == VERTICAL, the line is in the direction of increasing y.
 *
 * If there are already items in the map that collide with this line, they are
 * erased.
 */
void add_wall(int x, int y, int dir, int len);

void add_firewall(int x, int y, int dir, int len);
void add_catwall(int x, int y, int dir, int len);

/**
 * Add a PLANT item at (x,y). If there is already a MapItem at (x,y), erase it
 * before adding the plant.
 */
void add_plant(int x, int y);
void add_bigcat(int x, int y);
void add_drink(int x, int y);
void add_box(int x, int y);

void add_door(int x, int y, int dir, int len);
void add_door_open(int x, int y, int dir, int len);

void add_npc(int x, int y, int n);

void add_npc2(int x, int y);
void add_npc3(int x, int y);
void add_npc4(int x, int y);
void add_npc5(int x, int y);

void add_cat1(int x, int y);
void add_cat2(int x, int y);
void add_cat3(int x, int y);

void add_reward(int x, int y);

void add_stairs(int x, int y, int tm, int tx, int ty);
void add_stairs2(int x, int y, int tm, int tx, int ty);


void add_cave(int x, int y, int n,int tm, int tx, int ty);
void add_mud(int x, int y, int dir, int len);
void add_trap(int x, int y);
void add_fire(int x, int y);
void add_fire2(int x, int y);
void add_buzz(int x, int y);
void add_slain_enemy(int x, int y);

void add_enemy1(int x, int y);
void add_enemy2(int x, int y);

int get_active_map_index();


#endif //MAP_H