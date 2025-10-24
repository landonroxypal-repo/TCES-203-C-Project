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
#include <math.h>

#define MAX_FLEET_SIZE 100
#define MAX_MODEL_NAME_LENGTH 5

void clear_input() {
    int flushchar; 
    while ((flushchar = getchar()) != '\n' && flushchar != EOF); // copied from week 4 assignment
}

unsigned int get_id() {
    unsigned int id;
    printf("Enter an ID: ");

    while (scanf("%u", &id) != 1 || id == 0){
        printf("Invalid input. Please enter a nonzero positive integer: ");
        clear_input(); // without this clear, an infnite loop is possible!
    }

    clear_input();
    return id;
}

void get_name(char name[MAX_MODEL_NAME_LENGTH]) { // TODO: More advanced input verification
    printf("Enter model name: ");

    int character; // needs to be an int so that EOF works properly
    int name_index = 0;
    
    while ((character = getchar()) != '\n' && character != EOF && name_index < MAX_MODEL_NAME_LENGTH) { // While getchar is reading and the string is not longer than the name limit..
        name[name_index] = (char) character; //Typecast or things will not work properly
        name_index++;
    }

    name[name_index] = '\0'; // end of string

    if (name_index >= MAX_MODEL_NAME_LENGTH) { // Uh oh! The user inputted extra characters, so we need to clear the input!
        clear_input();
    }
}

float get_battery_level() {
    float level;
    printf("Enter battery level: ");

    while (scanf("%f", &level) != 1 || level < 0.0f || level > 100.0f){
        printf("Invalid input. Please enter a floating point value from 0.0 to 100.0: ");
        clear_input(); // without this clear, an infnite loop is possible!
    }

    clear_input();
    return level;
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

void get_position(float position[2]) {
    position[0] = get_coordinate('X');
    position[1] = get_coordinate('Y');
}

// adds a drone the id value cannot
int add_drone(unsigned int ids[], int fleet_size, char models[][MAX_MODEL_NAME_LENGTH], float batteries[], float positions[][2]) {
    printf("Add a drone:\n"); // Feedback to the user is important!

    ids[fleet_size] = get_id();

    get_name(models[fleet_size]); // Pass array we want edited because we cannot 

    batteries[fleet_size] = get_battery_level(); // Gets a float from 0.0 to 100.0

    get_position(positions[fleet_size]); // Gets two values

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

bool search_drone_by_id(unsigned int ids[], int fleet_size, char models[][MAX_MODEL_NAME_LENGTH], float batteries[], float positions[][2]) {
    printf("Search for drone by id:\n");
    unsigned int id = get_id();

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

float calculate_distance(float x0, float y0, float x1, float y1) {
    return sqrt(pow(x1-x0, 2) + pow(y1 - y0, 2));
} // possible bug in the distance calculation.

void find_nearest_drone(unsigned int ids[], int fleet_size, char models[][MAX_MODEL_NAME_LENGTH], float batteries[], float positions[MAX_FLEET_SIZE][2]){
    
    
    printf("Finding nearest drone: \n");
    float x = get_coordinate('X');
    float y = get_coordinate('Y');


    float current_x = positions[0][0], current_y = positions[0][1];
    double distance = calculate_distance(x, y, current_x, current_y);
    int index;
    double test_distance;



    // this for loop iterates through the position array and 
    for (int i = 1; i < MAX_FLEET_SIZE; i++){
        if (test_distance = calculate_distance(x, y, positions[i][0], positions[i][1]) < distance){
            index = i;
            distance = test_distance;
        }
    }

    // prints resulting drone
    printf("Nearest drone (%.2f units away): \n", distance);
    display_drone(index, ids, models, batteries, positions, true);
    printf("\n");
}

int main(void) {
    unsigned int ids[MAX_FLEET_SIZE];
    int fleet_size = 0;
    char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH];
    float batteries[MAX_FLEET_SIZE];
    float positions[MAX_FLEET_SIZE][2];
    
    int choice;

    do{
        printf("Options: \n\t[1] add a drone \n\t[2] display drones \n\t[3] ID search \n\t[4] average battery \n\t[5] add fleet \n\t[6] find nearest drone \n\t[0] exit\n\nEnter command: ");

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
                search_drone_by_id(ids, fleet_size, models, batteries, positions);
                break;
            } 
            case 4: {
                calculate_average_battery(batteries, fleet_size);
                break;
            }
            case 5: {
                fleet_size = add_fleet(ids, fleet_size, models, batteries, positions);
                break;
            }
            case 6: {
                find_nearest_drone(ids, fleet_size, models, batteries, positions);
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
