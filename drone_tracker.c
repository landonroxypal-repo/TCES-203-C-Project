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
#define MAX_MODEL_NAME_LENGTH 7 // Note: due to the terminate character this value must be the name length you want + 1. (i.e if you want a six character name limit set this value to 7)
#define TEST_MODEL_NAME_SIZE 10 // This size is used for data validation. since the maximum amount of characters is 6, 10 sould be overkill enough to see if the string is too large. 

// This function clears the input stream so that scanf won't read input that is left during previous calls. 
void clear_input() {
    int flushchar; 
    while ((flushchar = getchar()) != '\n' && flushchar != EOF); // copied from week 4 assignment
}

// This function is used to check if the id added in the get_id function is unique.
bool is_id_dupe(int id, int ids[MAX_FLEET_SIZE]) { 
    for (int index = 0; index < MAX_FLEET_SIZE; index++) {
        if (ids[index] == id) {
            return true;
        }
    }

    return false;
}

// This function prompts for an id and returns the resulting numerical value.
int get_id(int ids[MAX_FLEET_SIZE], bool restrict_to_unique) {
    int id;
    printf("Enter an ID: ");

    bool is_dupe = false;
    while (scanf("%u", &id) != 1 || id == 0 || restrict_to_unique && (is_dupe = is_id_dupe(id, ids))){ // ID checked to see if it's a duplicate here.
        printf("Invalid input. %s", is_dupe ? "Entered ID is a duplicate, please enter a different ID: " : "Please enter a nonzero positive integer:");
        clear_input(); // without this clear, an infnite loop is possible!
    }

    clear_input();
    return id;
}

// THis function creates adds characters to a string array and then returns its size.
int get_char_array(char string_array[]){
    int character; // needs to be an int so that EOF works properly
    int index = 0;
    
    while ((character = getchar()) != '\n' && character != EOF && index < TEST_MODEL_NAME_SIZE) { // While getchar is reading and the string is not longer than the test array size..
        string_array[index] = (char) character; // We need to typecast the character variable (int to char) so that the compiler doesn't throw any errors
        index++;
    }

    if (index == TEST_MODEL_NAME_SIZE) { // Uh oh! The user inputted extra characters, so we need to clear the input!
        clear_input();
    }

    return index;
}

// This function is used to create a model name for the drones. It is then returned in add_drone. 
void get_name(char name[MAX_MODEL_NAME_LENGTH]) {
    printf("Enter model name: ");
    char test_array[TEST_MODEL_NAME_SIZE];
    int name_size; // stores size and index that will have the next character inserted/
    // REMBER FOR NOW: MAX_MODEL_NAME_LENGTH - 1

    // Asks for a name until it is short enough. 
    while ((name_size = get_char_array(test_array)) >= MAX_MODEL_NAME_LENGTH){
        printf("Invalid name input. Model name must be no longer than %d characters long. Please try again: ", MAX_MODEL_NAME_LENGTH - 1);
    }

    for (int i = 0; i < name_size; i++){
        name[i] = test_array[i];
    }
    name[name_size + 1] = '\0'; // end of string
}

// This function pulls the battery level inputted by the user.
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

// This function calculates the average battery levels of the entire drone fleet. 
void calculate_average_battery(float batteries[MAX_FLEET_SIZE], int fleet_size) {
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

// This function is a helper function that just fetches a float with a coordinate label attached to it
float get_coordinate(char coord_char) { 
    float coord;

    printf("Enter %c coordinate: ", coord_char);
    
    while (scanf("%f", &coord) != 1){
        printf("Invalid input. Please enter a number: ");
        clear_input(); // without this clear, an infnite loop is possible!
    }

    clear_input();
    return coord;
}

// This function assigns the position to a drone. It is assumed that you are only inputting the part of the positions array that carries the coordinates
void get_position(float position[2]) {
    position[0] = get_coordinate('X');
    position[1] = get_coordinate('Y');
}

// This function adds a drone to the program by adding information to a series of arrays defined in the parameteers. It uses a fleet_size variable across these arrays to associate the 
// information that belongs to each drone with a certain index. 
int add_drone(int ids[MAX_FLEET_SIZE], int fleet_size, char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH], float batteries[MAX_FLEET_SIZE], float positions[MAX_FLEET_SIZE][2]) {
    printf("Add a drone:\n"); // Feedback to the user is important!

    ids[fleet_size] = get_id(ids, true);

    get_name(models[fleet_size]); // Pass array we want edited

    batteries[fleet_size] = get_battery_level(); // Gets a float from 0.0 to 100.0

    get_position(positions[fleet_size]); // Gets two values

    printf("Drone %s [ID=%u] added!\n", models[fleet_size], ids[fleet_size]); // Feedback to the user is important!

    // input data into all arrays now that we fetched it

    return fleet_size + 1; // Increment fleet size by 1
}

// This function displays a table header for the table that is printed in the display_drone and display_drones function. 
void display_table_header() {
    printf("%-6s | %-6s | %-4s | %-4s | %-4s |\n", "ID", "Model", "Battery", "X", "Y"); // Using %s because using manual spaces really sucks 
    printf("-------------------------------------------\n"); // Cosmetic
}

// This function displays a part of the table for a single drone. Additionally, if the single variable is true, the table header is in display_table_header is als printed. 
void display_drone(int index, int ids[MAX_FLEET_SIZE], char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH], float batteries[MAX_FLEET_SIZE], float positions[MAX_FLEET_SIZE][2], bool single) {
    if (ids[index] != 0) {
        if (single) {
            display_table_header();
        } 

        printf("%-6u | %-6s | %-6.2f%% | %-4.2f | %-4.2f |\n", ids[index], models[index], batteries[index], positions[index][0], positions[index][1]);
    } else {
        printf("No drone to display at index %d!", index);
    }
}

// This function creates a table that displayes all of the drones that are stored in the arrays. 
void display_drones(int ids[MAX_FLEET_SIZE], int fleet_size, char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH], float batteries[MAX_FLEET_SIZE], float positions[MAX_FLEET_SIZE][2]) {
    if (fleet_size > 0) {
        printf("Drones:\n");
        display_table_header();

        for (int index = 0; index < fleet_size; index++) {
            display_drone(index, ids, models, batteries, positions, false);
        }        
    } else {
        printf("No drones to display!\n");
    }
}

// The search_drone_by_id function searched for a drone in the ids array. After that, it 
bool search_drone_by_id(int ids[MAX_FLEET_SIZE], int fleet_size, char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH], float batteries[MAX_FLEET_SIZE], float positions[MAX_FLEET_SIZE][2]) {
    printf("Search for drone by ID:\n");
    int id = get_id(ids, false);

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
int add_fleet(int ids[MAX_FLEET_SIZE], int fleet_size, char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH], float batteries[MAX_FLEET_SIZE], float positions[MAX_FLEET_SIZE][2]) {
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

double calculate_distance(float x0, float y0, float x1, float y1) {
    double distance = sqrt((x1-x0) * (x1 - x0) + (y1 - y0) *(y1-y0));
    return distance;
}

// This is the funcition 2b that satifsies Variaton 2b-- Find nearest drone. 
void find_nearest_drone(int ids[MAX_FLEET_SIZE], int fleet_size, char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH], float batteries[MAX_FLEET_SIZE], float positions[MAX_FLEET_SIZE][2]){
    
    
    printf("Finding nearest drone: \n");
    float x = get_coordinate('X');
    float y = get_coordinate('Y');


    float current_x = positions[0][0], current_y = positions[0][1];
    double distance = calculate_distance(x, y, current_x, current_y);
    int index;
    double test_distance;

    // this for loop iterates through the position array
    for (int i = 1; i < fleet_size; i++){
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
    int ids[MAX_FLEET_SIZE];
    int fleet_size = 0;
    char models[MAX_FLEET_SIZE][MAX_MODEL_NAME_LENGTH];
    float batteries[MAX_FLEET_SIZE];
    float positions[MAX_FLEET_SIZE][2];
    
    int choice;

    do{
        printf("Options: \n\t[1] add a drone \n\t[2] display drones \n\t[3] ID search \n\t[4] average battery \n\t[5] add fleet \n\t[6] find nearest drone \n\t[0] exit\n\nEnter command: ");

        while (scanf("%d", &choice) != 1 && choice >= 0 && choice <= 7){
            printf("Invalid input. Please enter an integer between 0 and 6: ");
            clear_input();
        }

        printf("\n");

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
                break;
            }
            case 0: {
                printf("Goodbye!\n");
                break;
            }
            default: {
                printf("Invalid Command. Please try again.\n");
                break;
            }
        }

        printf("\n");
        
    } while (choice != 0);
    
    return 0;
}
