/*  
    Filename:   drone_tracker.c
    Purpose:    to create a drone management system.
    Authors:    Landon Wardle and Robert Cromer
*/

/***********************************VARIATIONS***************************************/
// Variation 1:     1C - Continuous entry mode
// Variation 2:     2B - Nearest Drone Finder

#include <stdio.h>  
#include <stdbool.h>

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
        printf("Invalid input. Please enter a nonzero positive integer: ");
        clear_input(); // without this clear, an infnite loop is possible!
    }

    clear_input();
    ids[fleet_size] = id;
}

void get_name(char models[][MAX_MODEL_NAME_LENGTH], int fleet_size) { // TODO: More advanced input verification
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
        printf("Invalid input. Please enter a floating point value from 0.0 to 100.0: ");
        clear_input(); // without this clear, an infnite loop is possible!
    }

    clear_input();
    batteries[fleet_Size] = level;
}

void calculate_average_battery(float batteries[], int fleet_size) {
    if (fleet_size > 0) {
        float average = 0.0f;

        for (int index = 0; index < fleet_size; index++) {
            average += batteries[index]; // add values
        }

        average /= fleet_size; // calculate average

        printf("Average battery of all drones: %.2f\n", average);
    } else {
        printf("No drones available.\n");
    }
}

float get_coordinate(char coord_char) { // Helper function that just fetches an int with a coordinate label attached to it
    float coord;

    printf("Enter %c coordinate: ", coord_char);
    
    while (scanf("%f", &coord) != 1){
        printf("Invalid input. Please enter a number: ");
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

void display_table_header() {
    printf("%-4s | %-5s | %-4s | %-4s | %-4s |\n", "ID", "Model", "Battery", "X", "Y"); // Using %s because using manual spaces really sucks 
    printf("-------------------------------------------\n"); // Cosmetic
}

// Function that satisfies Variation 2B - Nearest Drone finder
void display_drone(int index, unsigned int ids[], char models[][MAX_MODEL_NAME_LENGTH], float batteries[], float positions[][2], bool single) {
    if (single) {
        display_table_header();
    }

    printf("%-4u | %-5s | %-7.2f | %-4.2f | %-4.2f |\n", ids[index], models[index], batteries[index], positions[index][0], positions[index][1]);
}

// Variation 2B - Nearest Drone finder
void display_drones(unsigned int ids[], int fleet_size, char models[][MAX_MODEL_NAME_LENGTH], float batteries[], float positions[][2]) {
    printf("Drones:\n");
    display_table_header();

    for (int index = 0; index < fleet_size; index++) {
        display_drone(index, ids, models, batteries, positions, false);
    }
}

bool search_drone_by_id(unsigned int id, unsigned int ids[], int fleet_size, char models[][MAX_MODEL_NAME_LENGTH], float batteries[], float positions[][2]) {
    for (int index = 0; index < fleet_size; index++) {
        printf("%u\n", ids[index]);
        if (ids[index] == id) {
            printf("Drone [ID=%u] found!\n", id);
            printf("Drone [ID=%u] Data:\n", id);
            display_drone(index, ids, models, batteries, positions, true);
            return true; // No point in iterating more when we found the target drone
        }
    }
    
    printf("No drone of [ID=%u] found!\n", id);
    return false;
}

// Function that satisfies Variation 1C - Continuous entry mode
int add_fleet(unsigned int ids[], int fleet_size, char models[][MAX_MODEL_NAME_LENGTH], float batteries[], float positions[][2]) {
    char choice;
    printf("Adding Fleet:\n");
    
    do {
        fleet_size = add_drone(ids, fleet_size, models, batteries, positions);
        

        printf("Do you want to enter another drone? (y/n): ");
        while (scanf("%c", &choice) != 1 || (choice != 'y' && choice != 'n' && choice != 'Y' && choice != 'N')){
            printf("Invalid input. Please enter either 'y' or 'n': ");
            clear_input();
        }
    } while (choice == 'y' || choice == 'Y');

    display_drones(ids, fleet_size, models, batteries, positions);
    
    return fleet_size;
}

int main(void) {
    unsigned int ids[MAX_FLEET_SIZE];
    int fleet_size = 0;
    char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH];
    float batteries[MAX_FLEET_SIZE];
    float positions[MAX_FLEET_SIZE][2];
    
    int choice;

    do{
        printf("Options: \n\t[1] add a drone \n\t[2] display drones \n\t[3] ID search \n\t[4] average battery \n\t[5] add fleet \n\t[6] find nearest drone \n\t[0] exit\n\n Enter command: ");

        while (scanf("%d", &choice) != 1 && choice >= 0 && choice <= 7){
            printf("Invalid input. Please enter an integer between 1 and 7: ");
            clear_input();
        }

        switch (choice) {
            case 1: {
                fleet_size = add_drone(ids, fleet_size, models, batteries, positions);
                break;
            }
            case 2: {
                display_drones(ids, fleet_size, models, batteries, positions);
                break;
            }
            case 3: {
                printf("Very absent at this moment(10/22/2025)");
                break;
            } 
            case 4: {
                printf("Average battery function doot doot doot doot");
                break;
            }
            case 5: {
                fleet_size = add_fleet(ids, fleet_size, models, batteries, positions);
                break;
            }
            case 6: {
                printf("Done later");
                break;
            }
            case 0: {
                printf("goodbye!");
                break;
            }
            default: {
                printf("invalid command");
                break;
            }
        }
        
    } while (choice != 0);
    
    return 0;
}
