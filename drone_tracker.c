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
#define MAX_MODEL_NAME_LENGTH 5

void clear_input() {
    int flushchar; 
    while ((flushchar = getchar()) != '\n' && flushchar != EOF); // copied from week 4 assignment
}

unsigned int get_id(unsigned int ids[], int fleet_size) {
    unsigned int id;
    printf("Enter an ID: ");

    while (scanf("%u", &id) != 1 || id == 0){
        printf("Invalid input. Please enter a nonzero positive integer.\n");
        clear_input(); // without this clear, an infnite loop is possible!
    }

    clear_input();
    ids[fleet_size] = id;
}

void get_name(char models[][MAX_MODEL_NAME_LENGTH], int fleet_size) {
    printf("Enter model name: ");

    int character; // needs to be an int so that EOF works properly
    int name_index = 0;
    
    while ((character = getchar()) != '\n' && character != EOF && name_index < MAX_MODEL_NAME_LENGTH) { // While getchar is reading and the string is not longer than the name limit..
        models[fleet_size][name_index] = (char) character; //Typecast or things will not work properly
        name_index++;
    }

    models[fleet_size][name_index] = '\0'; // end of string

    if (name_index >= MAX_MODEL_NAME_LENGTH) { // Uh oh! The user inputted extra characters, so we need to clear the input!
        clear_input();
    }
}

float get_battery_level(float batteries[], int fleet_Size) {
    float level;
    printf("Enter battery level: ");

    while (scanf("%f", &level) != 1 || level < 0.0f || level > 100.0f){
        printf("Invalid input. Please enter a floating point value from 0.0 to 100.0.\n");
        clear_input(); // without this clear, an infnite loop is possible!
    }

    clear_input();
    batteries[fleet_Size] = level;
}

float get_coordinate(char coord_char) { // Helper function that just fetches an int with a coordinate label attached to it
    float coord;

    printf("Enter %c coordinate: ", coord_char);
    
    while (scanf("%f", &coord) != 1){
        printf("Invalid input. Please enter a number.\n");
        clear_input(); // without this clear, an infnite loop is possible!
    }

    clear_input();
    return coord;
}

void get_position(float positions[][2], int fleet_size) {
    positions[fleet_size][0] = get_coordinate('X');
    positions[fleet_size][1] = get_coordinate('Y');
}

// adds a drone the id value cannot
int add_drone(unsigned int ids[], int fleet_size, char models[][MAX_MODEL_NAME_LENGTH], float batteries[], float positions[][2]) {
    printf("Add a drone:\n"); // Feedback to the user is important!

    get_id(ids, fleet_size); // Gets integer

    get_name(models, fleet_size); // Pass array we want edited because we cannot 

    get_battery_level(batteries, fleet_size); // Gets a float from 0.0 to 100.0

    get_position(positions, fleet_size); // Gets two values

    printf("Drone %s [ID=%u] added!\n", models[fleet_size], ids[fleet_size]); // Feedback to the user is important!

    // input data into all arrays now that we fetched it

    return fleet_size + 1; // Increment fleet size by 1
}

// Variation 1C - Continuous entry mode
void add_fleet() {
    printf("to be completed\n");
}

// Variation 2B - Nearest Drone finder
void display_drones(unsigned int ids[], int fleet_size, char models[][MAX_MODEL_NAME_LENGTH], float batteries[], float positions[][2]) {
    printf("Drones:\n");
    printf("%-4s | %-5s | %-4s | %-4s | %-4s |\n", "ID", "Model", "Battery", "X", "Y"); // Using %s because using manual spaces really sucks 
    printf("-------------------------------------------\n"); // Cosmetic

    for (int index = 0; index < fleet_size; index++) {
        printf("%-4u | %-5s | %-7.2f | %-4.2f | %-4.2f |\n", ids[index], models[index], batteries[index], positions[index][0], positions[index][1]);
    }
}

int main(void) {
    unsigned int ids[MAX_FLEET_SIZE];
    int fleet_size = 0;
    char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH];
    float batteries[MAX_FLEET_SIZE];
    float positions[MAX_FLEET_SIZE][2];

    for(;;) {
        fleet_size = add_drone(ids, fleet_size, models, batteries, positions);
        display_drones(ids, fleet_size, models, batteries, positions);
    }
    
    return 0;
}