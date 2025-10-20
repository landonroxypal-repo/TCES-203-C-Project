/*  
    Filename:   drone_tracker.c
    Purpose:    to create a drone management system.
    Authors:    Landon Wardle and Robert Cromer
*/

/***********************************VARIATIONS***************************************/
// Variation 1:     1C - Continuous entry mode
// Variation 2:     2B - Nearest Drone Finder
//

#include <stdio.h>

#define MAX_FLEET_SIZE 100
#define MAX_MODEL_NAME_LENGTH 10


int main(void){
    unsigned int ids[MAX_FLEET_SIZE];
    int size = 0;
    char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH];
    float batteries[MAX_FLEET_SIZE];
    float positions[MAX_FLEET_SIZE][2];


    while (0);
    
    return 0;
}

// adds a drone
void add_drone(unsigned int ids[], int size, char name[], float battery_level, float x, float y){
    unsigned int id;

    printf("to be completed");
}

// Variation 1C - Continuous entry mode
void add_fleet(){
    printf("to be completed");
}
void display_drone(void){
    printf("to be completed");
}

// we chose to do 
