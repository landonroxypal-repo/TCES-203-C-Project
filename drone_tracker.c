/*  
    Filename:   drone_tracker.c
    Purpose:    to create a drone management system.
    Authors:    Landon Wardle and Robert Cromer
*/

/***********************************VARIATIONS***************************************/
// Variation 1:     1C - Continuous entry mode
// Variation 2:     2B - Nearest Drone Finder

#include <stdio.h>

#define MAX_FLEET_SIZE 100
#define MAX_MODEL_NAME_LENGTH 10


int main(void){
    unsigned int ids[MAX_FLEET_SIZE];
    int fleet_size = 0;
    char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH];
    float batteries[MAX_FLEET_SIZE];
    float positions[MAX_FLEET_SIZE][2];
    


    while (0);
    
    return 0;
}

// adds a drone the id value cannot
void add_drone(unsigned int ids[], int fleet_size, char name[][], float battery_levels[], float positions[]){
    
    unsigned int id = get_id();
    // Gets integer


    printf("to be completed");
}


void clear_input(){
    printf("to be finished later");
}

// Variation 1C - Continuous entry mode
void add_fleet(){
    printf("to be completed");
}

// Variation 2B - Nearest Drone finder
void display_drone(void){
    printf("to be completed");
}

unsigned int get_id(void){
    unsigned int id;
    printf("Enter an ID: ");
    while (scanf("%u", &id) != 1 || id == 0){
        printf("Invalid input. Please enter a nonzero positive integer.\n");
    }
    return id;
}

void make_name(char name[MAX_MODEL_NAME_LENGTH]){
    char c; 

    printf("Enter model name: ");
}