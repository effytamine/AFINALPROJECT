#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum {
    //string and buffer consts
    MAX_INPUT_STRING = 26,
    MAX_COLS = 4,
    MAX_ROWS = 5,
    MAX_PASSENGERS = MAX_ROWS*MAX_COLS,
};

// program state flags (macros)
#define STATE_boot_menu         (1u << 0) // Bit 0: Main menu active
#define STATE_session_menu      (1u << 1) // Bit 1: Session menu active
#define STATE_passenger_menu    (1u << 2) // Bit 2: Passenger menu active
#define STATE_passenger_edit    (1u << 3) // Bit 3: Passenger edit menu active
#define STATE_passenger_change  (1u << 4) // Bit 5: Passenger Change
#define STATE_exit_requested    (1u << 5) // Bit 6: Exit requested


// Dstruc
struct _passenger {
    int __seatNumber; // integer TYPEs
    int __age;
    int __passengerCode;
    char __name[MAX_INPUT_STRING];
    char __street[MAX_INPUT_STRING];
    char __city[MAX_INPUT_STRING];
};

struct _seat {
    int __seatNumber; // 1D_array 
    int __passengerCode; // (NULL or TRUE)
};

struct _flightDetails {
    char __code[MAX_INPUT_STRING]; // 
    char __date[MAX_INPUT_STRING];
    char __time[MAX_INPUT_STRING];
    char __from[MAX_INPUT_STRING];
    char __to[MAX_INPUT_STRING];
    char __gate[MAX_INPUT_STRING];
    char __boardingTime[MAX_INPUT_STRING];
};

struct _node {
    struct _passenger* __ptrPassenger;
    struct _node* __link; 
};

// globals
const char GLOBAL_col_labels[MAX_COLS] = {'A', 'B', 'C', 'D'};
int GLOBAL_passenger_pop = 0;
uint32_t GLOBAL_curr_program_state = STATE_boot_menu; // Start in main menu
uint32_t GLOBAL_program_dep_state = STATE_boot_menu;

struct _seat GLOBAL_seat_matrix[MAX_PASSENGERS]; // main matrix
struct _node* GLOBAL_session_list = NULL; // head of list ptr
struct _flightDetails GLOBAL_curr_flight;


// prototypes

// CLI constants
const char* GLOBAL_cli_divider = "\n  %s-------------------------------\n";
const char* GLOBAL_cli_input = "\n  > enter choice: ";
const char* GLOBAL_cli_getchar = "\n  > %s - press enter to %s;\n";


// Init Functions
void init_seat_matrix() {
    for (int i = 0; i < 20; i++) {
        GLOBAL_seat_matrix[i].__seatNumber = i;
        GLOBAL_seat_matrix[i].__passengerCode = 0;
    }
}

void init_flight_details() {
    strcpy(GLOBAL_curr_flight.__code, "SK180");
    strcpy(GLOBAL_curr_flight.__date, "2025-05-27");
    strcpy(GLOBAL_curr_flight.__time, "08:45 AM");
    strcpy(GLOBAL_curr_flight.__from, "MNL");
    strcpy(GLOBAL_curr_flight.__to, "CEB");
    strcpy(GLOBAL_curr_flight.__gate, "5");
    strcpy(GLOBAL_curr_flight.__boardingTime, "08:15 AM");
}

// Node functions
struct _node* create_passenger (int _a_seatNumber, char* _a_name, int _a_age, char* _a_street, char* _a_city) {
    if (_a_seatNumber < 0 || _a_name == NULL || _a_age < 0 || _a_city == NULL || _a_street == NULL || GLOBAL_passenger_pop < 0) return NULL;
    
    struct _node* new_passenger_node = malloc(sizeof(struct _node));
    new_passenger_node->__ptrPassenger = malloc(sizeof(struct _passenger));
    new_passenger_node->__link = NULL;

    new_passenger_node->__ptrPassenger->__seatNumber = _a_seatNumber; // seat no
    strcpy(new_passenger_node->__ptrPassenger->__name, _a_name); // name
    new_passenger_node->__ptrPassenger->__age = _a_age; // age
    strcpy(new_passenger_node->__ptrPassenger->__street, _a_street); // street
    strcpy(new_passenger_node->__ptrPassenger->__city, _a_city); // city
    
    if (GLOBAL_passenger_pop >= 0) GLOBAL_passenger_pop++;
    new_passenger_node->__ptrPassenger->__passengerCode = GLOBAL_passenger_pop;
    GLOBAL_seat_matrix[_a_seatNumber].__passengerCode = GLOBAL_passenger_pop;
    
    return new_passenger_node;
}

struct _node* traverse_list(struct _node* _a_currNode) { // objective: return the address of the next node
    if (_a_currNode->__ptrPassenger == NULL) return NULL;
        return _a_currNode -> __link;
}

void append_node(struct _node** _a_headNode, struct _node* _a_newPassenger) {
    // edge case: empty list, allocate and append
    if (*_a_headNode == NULL) {
        (*_a_headNode) = malloc(sizeof(struct _node));
        (*_a_headNode) = _a_newPassenger;
        (*_a_headNode)->__link = NULL;
        return;
    }

    struct _node* last_node = *_a_headNode;
    struct _node* temp;

    do { 
        temp = traverse_list(last_node);
        if (temp != NULL) last_node = temp;
    } while (temp != NULL);

    // last_node is now true
    // connect new_node to last_node -> link
    last_node -> __link = _a_newPassenger;
}

struct _node* find_node(struct _node** _a_headNode, const int _a_code) { // objective: traverse_list until NULL or current_node -> link  == find_node
    if (*_a_headNode == NULL) return NULL;
    struct _node* current_node = (*_a_headNode);

    while(current_node != NULL) {
        if (current_node -> __ptrPassenger -> __passengerCode == _a_code) {
            return current_node;
        }
        current_node = traverse_list(current_node);
    } 
        printf("Object not found");
        return NULL;
}

void remove_node(struct _node** _a_head, struct _node* _a_objAddress) { // objective: traverse_list until NULL or current_node = obj_address
    if ((*_a_head) == NULL) return;

    struct _node* previous_node = (*_a_head);
    struct _node* current_node = (*_a_head);

    while(current_node != NULL) {
        if (current_node == _a_objAddress) {
            if (current_node == *_a_head) {
                *_a_head = current_node -> __link;
            } else {
                previous_node -> __link = current_node -> __link;
            }
            free(current_node->__ptrPassenger);
            free(current_node);
            if (GLOBAL_passenger_pop > 0) GLOBAL_passenger_pop--;
            return;
        } 
        previous_node = current_node;
        current_node = traverse_list(current_node);
    }  

    printf("Object not found");
    return;
}

// Helper Functions
void clearin() {
    int stream;
    while((stream = getchar()) != EOF && stream != '\n');
}

void print_strings_array(const char* _a_stringsArr[], int _a_len, int _a_mode) {
    for (int i = 0; i < _a_len; i++) {
        if (_a_mode == 0) printf("  [%d] ", i + 1); 
        printf("%s", _a_stringsArr[i]);
        if (_a_mode == 0) printf("\n");
    }
}

void seat_to_id(int* _a_seatNumber, char* _a_seatID) {
    int row = *_a_seatNumber / 4;     
    int col = *_a_seatNumber % 4;       
    _a_seatID[0] = '1' + row;          
    _a_seatID[1] = 'A' + col;      
    _a_seatID[2] = '\0';               
}

int id_to_seatno(char* _a_seatID) {
    int row = _a_seatID[0] - '1';
    int col;
    
    if (_a_seatID[1] >= 'a' && _a_seatID[1] <= 'd') _a_seatID[1] -= 32;
    col = (_a_seatID[1]) - 'A';    
    return row * 4 + col;       // 0 to 3
}

int is_seat_valid(const int _a_seatNumber) { 
    if (_a_seatNumber >= 0 && _a_seatNumber <= 19) return 1;
    return -1;
    // 1 if valid, -1 if not valid;
}

int is_seat_free(const int _a_seatNumber) {
    if (GLOBAL_seat_matrix[_a_seatNumber].__passengerCode != 0) return -1;
    return 1; 
    // 1 if free, -1 if not free
}

int verify_by_name(const char *_a_originalString, const char *_a_StringtoCompare) {
    while (*_a_originalString && *_a_StringtoCompare) {
        char x = (*_a_originalString >= 'a' && *_a_originalString <= 'z') ? *_a_originalString - 32 : *_a_originalString;
        char y = (*_a_StringtoCompare >= 'a' && *_a_StringtoCompare <= 'z') ? *_a_StringtoCompare - 32 : *_a_StringtoCompare;
        if (x != y) return 0;
        _a_originalString++, _a_StringtoCompare++;
    }
    return (*_a_originalString == *_a_StringtoCompare) ? 1 : 0;
}

int input_seat_details(char* _a_seatID, int* _a_seatNumber, char* _a_name, int* _a_age, char* _a_street, char* _a_city) {
    printf("\n    Instructions: Choose a seat above, to be validated for the operation\n");
    printf("    Select a seat (ex. 1A, 0 to abort): ");
        scanf(" %2[^\n]", _a_seatID);
        clearin( );
        
    if (_a_seatID[0] == '0') return -4;

    *_a_seatNumber = id_to_seatno(_a_seatID); // convert

    // Validation 1
    if ((GLOBAL_curr_program_state & (STATE_passenger_menu)) || (GLOBAL_curr_program_state & (STATE_passenger_change))){
        int is_valid = is_seat_valid(*_a_seatNumber); // is seat valid?
        printf("    ooo verifying if seat %s is valid\n", _a_seatID);
        if (is_valid != 1) return -2; 

        int is_free = is_seat_free(*_a_seatNumber);
        printf("    ooo verifying if seat %s is free\n", _a_seatID);
        if (is_free != 1) return -3;
    } else if (GLOBAL_curr_program_state & STATE_passenger_edit) {
        int is_valid = is_seat_valid(*_a_seatNumber); // is seat valid?
        printf("    ooo verifying if seat %s is valid\n", _a_seatID);
        if (is_valid != 1) return -2; 

        int is_free = is_seat_free(*_a_seatNumber);
        printf("    ooo verifying if seat %s is taken\n", _a_seatID);
        if (is_free == 1) return -5;
    } else {
        return -6;
    }

    // Input Details
    if (GLOBAL_curr_program_state & (STATE_passenger_menu)){
        printf(GLOBAL_cli_divider, "  ");
        printf("    Seat verified: You picked %s", _a_seatID);
        printf(GLOBAL_cli_divider, "  ");

        printf("    > Passenger Fields\n");
        printf("    %-10s:  ", "Name");
            scanf(" %25[^\n]", _a_name);
            clearin();
        printf("    %-10s:  ", "Age");
            scanf(" %d", _a_age);
            clearin();
        printf("    %-10s:  ", "Street");
            scanf(" %25[^\n]", _a_street);
            clearin();
        printf("    %-10s:  ", "City");
            scanf(" %25[^\n]", _a_city);
            clearin();
    } else if (GLOBAL_curr_program_state & (STATE_passenger_edit)){
        printf("    > Verify Seat Holder by Name\n");
        printf("    %-10s:  ", "Name");
        struct _node* node_to_verify = find_node(&GLOBAL_session_list,GLOBAL_seat_matrix[*_a_seatNumber].__passengerCode);
        scanf(" %25[^\n]", _a_name);
        int is_seat_verified_by_name = verify_by_name(_a_name, node_to_verify->__ptrPassenger->__name);
        clearin();
        if (!is_seat_verified_by_name) return -7;
        
        printf(GLOBAL_cli_divider, "  ");
        printf("    Seat verified: You picked %s to edit", _a_seatID);
        printf("\n    Seat edit menu (cancel or change) will now load.");
        printf(GLOBAL_cli_divider, "  ");
        getchar();
        return node_to_verify->__ptrPassenger->__passengerCode;
    } else if (GLOBAL_curr_program_state & (STATE_passenger_change)) {
        return -8;
    } else {
        return -6; 
    }
    
    return -1;
    // -1 if success, -2 if seat not valid, -3 if seat is taken, -4 if exit triggered, -5 if seat is not taken, -6 if no validation occured , -7 if seat could not be verified with name, -8 seat is free to swap
}

// GUI Print Functions
void print_base_case(int _a_error, int _a_reason) {
    const char* error [] = {"Cannot select/reserve any more Seat", "Cannot edit any more seat", "Cannot reset seats"};
    const char* reason [] = {"Seat capacity full", "Seat capacity empty"}; 
    const char* solution [] = {"Select/reserve seats", "Cancel or reset seats"};
    printf(GLOBAL_cli_divider, "");
    printf("  [ ! ] %s\n", error[_a_error]);
    printf("        There are currently [%d/%d] Passengers\n", GLOBAL_passenger_pop, MAX_PASSENGERS);
    printf("        Reason: %s\n", reason[_a_reason]);
    printf("        Solution: %s", solution[_a_reason]);
    printf(GLOBAL_cli_divider, "");
    printf(GLOBAL_cli_getchar, "", "continue");
    getchar();
}

void print_program_author () {
    printf("  -------------------------------------------------------------------\n");
    printf("  | Description : This machine project is an Airline Seat Operator  |\n");
    printf("  |  Author : Sean Russell B. Villeza                               |\n");
    printf("  |  Section : 1-5                                                  |\n");
    printf("  |  Last Modified : 06/21/2025                                     |\n");
    printf("  |  Acknowledgments: Submitted to Sir Arnie Fabregas               |\n");
    printf("  -------------------------------------------------------------------\n");
    printf("  > Press enter to continue");
}

void print_ascii_menu () {
    printf("\n");
    printf("\n  ---------------------------------------------------------------------------\n");
    printf("      _    ___ ____  _     ___ _   _ _____                                   \n");
    printf("     / \\  |_ _|  _ \\| |   |_ _| \\ | | ____|                                  \n");
    printf("    / _ \\  | || |_) | |    | ||  \\| |  _|                                    \n");
    printf("   / ___ \\ | ||  _ <| |___ | || |\\  | |___                                   \n");
    printf("  /_/__ \\_\\___|_| \\_\\_____|___|_| \\_|_____|___ ____      _  _____ ___  ____  \n");
    printf("  / ___|| ____|  / \\|_   _|  / _ \\|  _ \\| ____|  _ \\    / \\|_   _/ _ \\|  _ \\ \n");
    printf("  \\___ \\|  _|   / _ \\ | |   | | | | |_) |  _| | |_) |  / _ \\ | || | | | |_) |\n");
    printf("   ___) | |___ / ___ \\| |   | |_| |  __/| |___|  _ <  / ___ \\| || |_| |  _ < \n");
    printf("  |____/|_____/_/   \\_\\_|    \\___/|_|   |_____|_| \\_\\/_/   \\_\\_| \\___/|_| \\_\\\n");
}

void print_seats() {
    int seatNumber;

    printf(GLOBAL_cli_divider, " ");
    printf("\n        Left   Hall  Right\n");
    for(int i = 0; i < MAX_ROWS; i++) {
        printf("    %d > ", i + 1);
        for(int j = 0; j < MAX_COLS; j++) {
            seatNumber = i * 4 + j;
            (GLOBAL_seat_matrix[seatNumber].__passengerCode == 0) ? printf("[%c] ", GLOBAL_col_labels[j]) : printf("[%c] ", 'X');
            if (j == 1) printf("   ");
        }
        if (i == 1) printf("  Seat Map:  1A, 1B, 2C, 3D....");
        if (i == 2) printf("  Current Passengers: [%d/%d]", GLOBAL_passenger_pop, MAX_PASSENGERS);
        printf("\n");
    }
    printf(GLOBAL_cli_divider," ");
}

void print_seats_file (FILE* _a_fstream) {
    int seatNumber;
    fprintf(_a_fstream, GLOBAL_cli_divider, " ");
    fprintf(_a_fstream,"\n        Left   Hall  Right\n");
    for(int i = 0; i < MAX_ROWS; i++) {
        fprintf(_a_fstream,"    %d > ", i + 1);
        for(int j = 0; j < MAX_COLS; j++) {
            seatNumber = i * 4 + j;
            (GLOBAL_seat_matrix[seatNumber].__passengerCode == 0) ? fprintf(_a_fstream, "[%c] ", GLOBAL_col_labels[j]) : fprintf(_a_fstream,"[%c] ", 'X');
            if (j == 1) fprintf(_a_fstream,"   ");
        }
        fprintf(_a_fstream,"\n");
    }
    fprintf(_a_fstream, GLOBAL_cli_divider," ");
}

void print_tix(struct _node* _a_passenger){
    char seatID[3] = {0};
    seat_to_id(&(_a_passenger->__ptrPassenger->__seatNumber), seatID);  
    printf(GLOBAL_cli_divider, "","");
    printf("  %-15s: %-25s\n", "Flight", GLOBAL_curr_flight.__code);
    printf("  %-15s: %-25s\n", "Seat Number", seatID);
    printf("  %-15s: %-25s\n", "Passenger", _a_passenger->__ptrPassenger->__name);
    printf("  %-15s: %-25s\n", "Date", GLOBAL_curr_flight.__date);
    printf("  %-15s: %-25s", "Time", GLOBAL_curr_flight.__time);
    printf(GLOBAL_cli_divider, "","");
    printf("  %-15s: %-25s\n","From", GLOBAL_curr_flight.__from);
    printf("  %-15s: %-25s\n","To", GLOBAL_curr_flight.__to);
    printf("  %-15s: %-25s\n","Gate", GLOBAL_curr_flight.__gate);
    printf("  %-15s: %-25s", "Boarding", GLOBAL_curr_flight.__boardingTime);
    printf(GLOBAL_cli_divider, "","");
    printf(GLOBAL_cli_getchar, "", "continue");
    getchar();
}

void print_program_trivias () {
    printf(GLOBAL_cli_divider, "");
    printf("    > This program took 1 and a half weeks to plan, and 3 weeks/ 2 hours per day of work to finish\n");
    printf("    > It was originally supposed to have an admin feature to reset seats, change flight information, and display all passengers in JSON format\n");
    printf("    > This version of the program IS NOT the best way to solve the solution, but includes a lot of lessons from CPROG I and II\n");
    printf("    > Passenger data is handled by implementation of linked lists and its functions, Seat plan is managed through a 1D array\n");
    printf("    > I write this message to promise i will post a full version on my Github - Villeza, Sean Russell B., June 21, 2025 !");
    printf(GLOBAL_cli_divider, "");
    printf(GLOBAL_cli_getchar,"", "continue");
    getchar();
}

void print_howto () {
    printf(GLOBAL_cli_divider, "");
    printf("    SELECTING A SEAT:\n"); 
    printf("    1) Enter current / new session\n    2) Enter passenger menu\n    3) Enter select / reserve a seat\n    4) Enter the seat ID (ex. 1D) you'd like to occupy (it should have no X mark = Unoccupied seat)\n    5) Resolve all validation requests (if you encounter any)\n    6) Fill passenger data field with the right data\n    7) You have selected a seat! The ticket will print for your information and will reflect on the file and seat map");
    printf(GLOBAL_cli_divider, "");
    printf(GLOBAL_cli_divider, "");
    printf("    CANCELLING A SEAT:\n"); 
    printf("    1) Enter current / new session\n    2) Enter passenger menu\n    3) Enter edit seats\n    4) Enter the seat ID (ex. 1A) of the seat you'd like to edit, it is logical that this seat must have been occupied to be eligible\n    5) Resolve all seat validation requests (if any)\n    6) Enter that seat's passenger's name to confirm you own that seat\n    7) Resolve all passenger validation requests(if any)\n    8) Confirm the cancellation of your seat; enter Y or y\n    9) You have cancelled your seat! It will reflect on the file and the seat map");
    printf(GLOBAL_cli_divider, "");
    printf(GLOBAL_cli_divider, "");
    printf("    CANCELLING A SEAT:\n"); 
    printf("    1) Enter current / new session\n    2) Enter passenger menu\n    3) Enter edit seats\n    4) Enter the seat ID (ex. 1A) of the seat you'd like to edit, it is logical that this seat must have been occupied to be eligible\n    5) Resolve all seat validation requests (if any)\n    6) Enter that seat's passenger's name to confirm you own that seat\n    7) Resolve all passenger validation requests(if any)\n    8) Enter the new seat ID you want to occupy\n    9) Resolve all new seat validation requests(if any)\n    10) Confirm to change seats; enter y or Y\n    11) You have changed / swapped seats! It will reflect via a new ticket, the seat map and the file");
    printf(GLOBAL_cli_divider, "");
    printf(GLOBAL_cli_getchar,"", "continue back to menu");
    getchar();
}

void show_main_menu() { 
    // strings
    const char* print_program_states[] = { "BOOT", "CURRENT-SESSION", "PASSENGER", "EDIT", "ADMIN"};
    const char* print_terminators[] = {"Back", "Exit"};
    const char* print_menu_boot[] = {"New Session / Current Session", "Program Trivias", "How to Use", "About"};
    const char* print_menu_session[] = {"Passenger"};
    const char* print_menu_passenger[] = {"Select / Reserve Seat", "Edit Seats"};
    const char* print_menu_passenger_edit[] = {"Cancel Seat", "Change Seat"};
    const char* print_menu_admin[] = {"Reset Seats", "Passenger Details", "Change Flight Details", "Export Flight Database(CSV)"};
    int last = 2;
    print_ascii_menu();

    printf(GLOBAL_cli_divider, ""); // top divider
    printf("  | "); // design

    for(int i = 0; i < 4; i++) {
        if (GLOBAL_program_dep_state & (1u << i)) {
            printf("%s", print_program_states[i]);
            printf(" > ");
        } 
    }
    printf(GLOBAL_cli_divider, ""); // bottom divider
    
    int bit_index = 0;
    while (bit_index < 4) {
        if (GLOBAL_curr_program_state & (1u << bit_index)) break;
        bit_index++;
    }

    switch (bit_index) {
        case 0: {
            print_strings_array(print_menu_boot, 4, 0); 
            last += 2;
            break;
        }
        case 1: {
            print_strings_array(print_menu_session, 1, 0); 
            last--;
            break;
        }
        case 2: print_strings_array(print_menu_passenger, 2, 0); break;
        case 3: print_strings_array(print_menu_passenger_edit, 2, 0); break;
        case 4: {
            print_strings_array(print_menu_admin, 4, 0); 
            last += 2;
            break;
        }
    }
    (bit_index == 0) ? printf("  [%d] %s\n", last + 1, print_terminators[1]): printf("  [%d] %s\n", last + 1, print_terminators[0]);
}

void print_header_file(FILE* _a_fstream) {
        fprintf(_a_fstream, GLOBAL_cli_divider,"");
        fprintf(_a_fstream, "> AIRLINE FLIGHT REPORT");
        fprintf(_a_fstream, GLOBAL_cli_divider,"");
        fprintf(_a_fstream,"  %-20s  :\t%-20s", "Carrier", "Small Airplane");
        fprintf(_a_fstream,"\n  %-20s  :\t%-20s", "Flight No.", GLOBAL_curr_flight.__code);
        fprintf(_a_fstream,"\n  %-20s  :\t%-20s", "Flight Date", GLOBAL_curr_flight.__date);
        fprintf(_a_fstream,"\n  %-20s  :\t%d", "Seats Total", MAX_PASSENGERS);
        fprintf(_a_fstream,"\n  %-20s  :\t%d\n", "Seats Occupied", GLOBAL_passenger_pop);
        fprintf(_a_fstream,"  %-20s  :\t%-d", "Seats Available", MAX_PASSENGERS - GLOBAL_passenger_pop);
        fprintf(_a_fstream, GLOBAL_cli_divider, "");
        fprintf(_a_fstream,"\t PASSENGER MASTERLIST");
        fprintf(_a_fstream, GLOBAL_cli_divider, "");
    
}

void print_passenger_file (FILE* _a_fstream, struct _passenger* _a_passenger) {
    char seatID[3] = {0};
    seat_to_id(&(_a_passenger->__seatNumber), seatID);

    char conc_address[51] = {0};
    snprintf(conc_address, sizeof(conc_address), "%s, %s", _a_passenger->__street, _a_passenger->__city);
    fprintf(_a_fstream,"  | %-5d | %-8s | %-25s | %-5d | %-50s  |\n", _a_passenger->__passengerCode, seatID, _a_passenger->__name, _a_passenger->__age, conc_address);
}

// Main functions
int menu_input () {
    int choice_1_;
    show_main_menu();
    printf(GLOBAL_cli_input, "");
    choice_1_ = -1; 
    scanf(" %d", &choice_1_); //user_input
    clearin();

    return choice_1_;
}

int record_pass(struct _node *_a_head) {
    struct _node* curr_node = _a_head;
    char seatID[3] = {0};

    FILE* fstream;
    fstream = fopen("plist.txt", "w");
    if (fstream == NULL) {
        printf("Err: record_pass");
        return 0;
    }
     
    print_header_file(fstream);
    fprintf(fstream,"\n  | %-5s | %-8s | %-25s | %-5s | %-50s  |\n", "No.", "Seat" ,"Passenger Name", "Age", "Home Address");
    fprintf(fstream,"  | %-5s | %-8s | %-25s | %-5s | %-50s  |\n", "-----", "--------" ,"-------------------------", "-----", "--------------------------------------------------");
    if (curr_node == NULL) fprintf(fstream,"  | %-5s | %-8s | %-25s | %-5s | %-51s |\n", "NULL", "NULL", "No Passengers yet", "NULL", "NULL" );
    while (curr_node != NULL) {
        print_passenger_file(fstream, curr_node->__ptrPassenger);
        curr_node = traverse_list(curr_node);
    }
    fprintf(fstream,"\n\t> SEAT MAP");
    print_seats_file(fstream);
    fprintf(fstream,"\n\n  --------------------------------------------------------------------\n");
    fprintf(fstream,"  |  Description : This machine project is an Airline Seat Operator  |\n");
    fprintf(fstream,"  |  Author : Sean Russell B. Villeza                                |\n");
    fprintf(fstream,"  |  Section : 1-5                                                   |\n");
    fprintf(fstream,"  |  Last Modified : 06/21/2025                                      |\n");
    fprintf(fstream,"  |  Acknowledgments: Submitted to sir Arnie Fabregas                |\n");
    fprintf(fstream,"  --------------------------------------------------------------------\n");
    fclose(fstream);
}

int seat_cancel(int _a_passengerCode) {
    struct _node* rm_passenger = find_node(&GLOBAL_session_list, _a_passengerCode); 
    char seatID[3] = {0};
    char cancel_1_ = 0;
    printf("\n  > OPERATION: [ SEAT CANCEL ]");

    printf(GLOBAL_cli_divider, "");
    seat_to_id(&(rm_passenger->__ptrPassenger->__seatNumber), seatID);
    printf("    [ ! ] Are you sure you want to cancel / free your seat %s?\n", seatID);
    printf("    {   [%c] -> [%s]   }", 'X', seatID);
    printf(GLOBAL_cli_divider, "");

    printf("\n  : [Y] Yes [N] No");
    printf("\n\n  > enter choice : ");
    scanf(" %c", &cancel_1_);
    clearin();
    if (cancel_1_ == 'Y' || cancel_1_ == 'y') {
        GLOBAL_seat_matrix[rm_passenger->__ptrPassenger->__seatNumber].__passengerCode = 0;
        remove_node(&GLOBAL_session_list, rm_passenger);
        record_pass(GLOBAL_session_list);
        printf(GLOBAL_cli_divider, "");
        printf("\n    Seat cancelled: You picked %s -> now free", seatID);
        printf(GLOBAL_cli_getchar, "", "continue back to find seat for seat edit");
        printf(GLOBAL_cli_divider, "");
        getchar();
    }
    return 1;
}

int seat_change(int _a_passengerCode) {
    struct _node* from_passenger = find_node(&GLOBAL_session_list, _a_passengerCode); 
    int seatNumber = 0, is_valid_seatforchange = 0, age = 0;
    char name [MAX_INPUT_STRING] = {0};
    char street[MAX_INPUT_STRING] = {0};
    char city[MAX_INPUT_STRING] = {0};
    char toseatID[3] = {0};
    char fromseatID[3] = {0};
    do { // verify seat
        printf("\n  > OPERATION: [ SEAT CHANGE ]"); 
        print_seats();
        char seatID[3] = {0};
        is_valid_seatforchange = input_seat_details(toseatID, &seatNumber, name, &age, street, city);

        if (is_valid_seatforchange != -8) {
            seatNumber = 0;
            
            if (is_valid_seatforchange == -4) return 0;

            printf(GLOBAL_cli_divider, "    ");
            printf("\n    For seat %s:  ", seatID);
            if (is_valid_seatforchange == -2) printf(GLOBAL_cli_getchar, "Invalid seat", "try again");
            if (is_valid_seatforchange == -3) printf(GLOBAL_cli_getchar, "Seat is taken", "try again");
            printf(GLOBAL_cli_divider, "    ");
            getchar();
        }

    } while (is_valid_seatforchange != -8); 
    
    printf(GLOBAL_cli_divider, "");
    seat_to_id(&(from_passenger->__ptrPassenger->__seatNumber), fromseatID);
    printf("    [ ! ] Are you sure you want to change seat %s?\n", fromseatID);
    printf("    {   [%s] -> [%s]   }", fromseatID , toseatID);
    printf(GLOBAL_cli_divider, "");
    
    char change_1_;
    printf("\n  : [Y] Yes [N] No");
    printf("\n\n  > enter choice : ");
    scanf(" %c", &change_1_);
    clearin();
    if (change_1_ == 'Y' || change_1_ == 'y') {
        GLOBAL_seat_matrix[from_passenger->__ptrPassenger->__seatNumber].__passengerCode = 0; // uninit past seat
        GLOBAL_seat_matrix[seatNumber].__passengerCode = from_passenger->__ptrPassenger->__passengerCode; // init new seat
        from_passenger->__ptrPassenger->__seatNumber = seatNumber; // change seat number of passenger
        record_pass(GLOBAL_session_list); // record

        printf(GLOBAL_cli_divider, "");
        printf("\n    Seat changed: You picked to change from %s -> %s", fromseatID, toseatID);
        printf(GLOBAL_cli_getchar, "", "continue back to find seat for seat edit");
        printf(GLOBAL_cli_divider, "");
        getchar();
    }
    return 1;
}

int find_seat() {
    // init_passenger details
    int seatNumber = 0, is_valid_passenger = -10, age = 0;
    char name [MAX_INPUT_STRING] = {0};
    char street[MAX_INPUT_STRING] = {0};
    char city[MAX_INPUT_STRING] = {0};

    do { // verify seat
        printf("\n  > OPERATION: [ FIND SEAT ];");
        print_seats();
        char seatID[3] = {0};
        is_valid_passenger = input_seat_details(seatID, &seatNumber, name, &age, street, city);

        if (is_valid_passenger < 0) {
            seatNumber = 0;

            if (is_valid_passenger == -4) return 0;
            printf(GLOBAL_cli_divider, "    ");
            printf("\n    For seat %s:  ", seatID);
            if (is_valid_passenger == -2) printf(GLOBAL_cli_getchar, "Invalid seat", "try again");
            if (is_valid_passenger == -3) printf(GLOBAL_cli_getchar, "Seat is taken", "try again");
            if (is_valid_passenger == -5) printf(GLOBAL_cli_getchar, "No passenger occupies that seat", "try again");
            if (is_valid_passenger == -6) printf(GLOBAL_cli_getchar, "No validation occurred", "try again");
            if (is_valid_passenger == -7) printf(GLOBAL_cli_getchar, "Could not be verified by name", "try again");
            printf(GLOBAL_cli_divider, "    ");
            getchar();
        }

    } while (is_valid_passenger < 0);
    return is_valid_passenger;   
} 

int select_seat() {
    int choice_1_;

    do {
        if (GLOBAL_passenger_pop == 20) {
            print_base_case(0, 0);
            choice_1_ = 3;
        return 0;
        } 
        // init_passenger details
        int seatNumber = 0, is_valid_passenger = 0, age = 0;
        char name [MAX_INPUT_STRING] = {0};
        char street[MAX_INPUT_STRING] = {0};
        char city[MAX_INPUT_STRING] = {0};

        do { // verify seat
            printf("\n  > OPERATION: [ SEAT SELECT ];");
            print_seats();
            char seatID[3] = {0};
            is_valid_passenger = input_seat_details(seatID, &seatNumber, name, &age, street, city);

            if (is_valid_passenger != -1) {
                seatNumber = 0;
                
                if (is_valid_passenger == -4) return 0;

                printf(GLOBAL_cli_divider, "    ");
                printf("\n    For seat %s:  ", seatID);
                if (is_valid_passenger == -2) printf(GLOBAL_cli_getchar, "Invalid seat", "try again");
                if (is_valid_passenger == -3) printf(GLOBAL_cli_getchar, "Seat is taken", "try again");
                printf(GLOBAL_cli_divider, "    ");
                getchar();
            }

        } while (is_valid_passenger != -1); 

        struct _node* new_passenger = create_passenger(seatNumber, name, age, street, city);
        append_node(&GLOBAL_session_list, new_passenger);
        struct _node* appended_passenger = find_node(&GLOBAL_session_list, new_passenger->__ptrPassenger->__passengerCode);
        record_pass(GLOBAL_session_list);
        print_tix(appended_passenger);
        print_seats();

        printf("\n  [1] Pick another seat\n  [2] Back");
        printf(GLOBAL_cli_input);
        scanf(" %d", &choice_1_);
            clearin();

        switch (choice_1_) {
            case 1: break;
            case 2: {
                return 1;
                break;
            }
            default: {
                printf(GLOBAL_cli_getchar, "invalid input", "try again");
                getchar();
            }
        }
    } while (1);

    return 1;
}

// Menu Functions
void passenger_edit_state () {
    int choice_1_;
    int seat_found = 0;

    if (GLOBAL_passenger_pop == 0) {
        print_base_case(1, 1);
        choice_1_ = 3;
    } else {
        seat_found = find_seat();
        if (seat_found){
            choice_1_ = menu_input();
        } else {
            choice_1_ = 3;
        }
    }

    switch (choice_1_) {
        case 1: seat_cancel(seat_found); break; // call seat cancel
        
        case 2: {
                GLOBAL_curr_program_state = (GLOBAL_curr_program_state & ~STATE_passenger_edit) | STATE_passenger_change; // clr and make session_menu
                GLOBAL_program_dep_state |= STATE_passenger_change; // take off passenger state
                seat_change(seat_found); 
                GLOBAL_curr_program_state = (GLOBAL_curr_program_state & ~STATE_passenger_change) | STATE_passenger_edit; // clr and make session_menu
                GLOBAL_program_dep_state ^= STATE_passenger_change; // take off passenger state
                break;
        } 
        case 3: {
            GLOBAL_curr_program_state = (GLOBAL_curr_program_state & ~STATE_passenger_edit) | STATE_passenger_menu; // clr and make session_menu
            GLOBAL_program_dep_state ^= STATE_passenger_edit; // take off passenger state
            break;
        }
        default: {
            printf(GLOBAL_cli_getchar, "invalid input", "try again");
            getchar();
        }
    }
}

void passenger_state () {
    int choice_1_ = menu_input();

    switch(choice_1_) {
        case 1: select_seat(); break; // call select /  reserve seat
        case 2: {
            GLOBAL_curr_program_state = (GLOBAL_curr_program_state & ~STATE_passenger_menu) | STATE_passenger_edit; // clr and make passenger edit
            GLOBAL_program_dep_state |= STATE_passenger_edit; // append passenger edit state
            break;
        }
        case 3: {
            GLOBAL_curr_program_state = (GLOBAL_curr_program_state & ~STATE_passenger_menu) | STATE_session_menu; // clr and make session_menu
            GLOBAL_program_dep_state ^= STATE_passenger_menu; // take off passenger state
            break;
        }
        default: {
            printf(GLOBAL_cli_getchar, "invalid input", "try again");
            getchar();
        }
    }
}

void session_menu_state () {
    int choice_1_ = menu_input();
    
    switch(choice_1_) {
        case 1:
            GLOBAL_curr_program_state = (GLOBAL_curr_program_state & ~STATE_session_menu) | STATE_passenger_menu; // clr and make passenger
            GLOBAL_program_dep_state |= STATE_passenger_menu; // append passenger state
            break;
        case 2: 
            GLOBAL_curr_program_state = (GLOBAL_curr_program_state & ~STATE_session_menu) | STATE_boot_menu; // clr and make boot_menu
            GLOBAL_program_dep_state ^= STATE_session_menu; // take off session state
            break;
        default: {
            printf(GLOBAL_cli_getchar, "invalid input", "try again");
            getchar();
        }    
    }
}

void boot_state () {
    char quit_;
    int choice_1_ = menu_input();
            
    switch(choice_1_) {
        case 1:  {
            GLOBAL_curr_program_state = (GLOBAL_curr_program_state & ~STATE_boot_menu) | STATE_session_menu; // clear program_state
            GLOBAL_program_dep_state |= STATE_session_menu;  //append session_menu
            break;
        }
        case 2: {
            print_program_trivias();
            break; 
        }
        case 3: {
            print_howto();
            break;
        }
        case 4: {
            print_program_author();
            getchar();
            break;
        }
        case 5: {
            printf("\n  : Would you like to exit Airline Seat Operator?");
            printf("\n  : [Y] Yes [N] No");
            printf("\n\n  > enter choice : ");
            scanf(" %c", &quit_);
            clearin();
            if (quit_ == 'Y' || quit_ == 'y') {
                GLOBAL_curr_program_state = (GLOBAL_curr_program_state & ~STATE_boot_menu) | STATE_exit_requested; // clean program state
                print_program_author();
                // free(passenger_list);
            }
            break;
        }
        default: {
            printf(GLOBAL_cli_getchar, "invalid input", "try again");
            getchar();
        }
    }
}

int main() {
    init_seat_matrix(); // initialize seat matrix
    init_flight_details(); // initialize flight_details

    while (!(GLOBAL_curr_program_state & STATE_exit_requested)){

        if (GLOBAL_curr_program_state & (STATE_boot_menu)){ // 1: Boot State
            boot_state();
        } else if (GLOBAL_curr_program_state & (STATE_session_menu)) {
            session_menu_state();
        } else if (GLOBAL_curr_program_state & (STATE_passenger_menu)) {
            passenger_state();
        } else if (GLOBAL_curr_program_state & (STATE_passenger_edit)) {
            passenger_edit_state();
        }
    }

    return 1;
}
