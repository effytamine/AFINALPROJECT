#ifndef GLOBALS_H
#define GLOBALS_H
#define MAX_NAME_LEN 50
#define MAX_STREET_LEN 25
#define MAX_CITY_LEN 25

// Struct definitions
struct n_passenger {
    int seatNumber; // 1D array
    char name[MAX_NAME_LEN];
    int age;
    char street[MAX_STREET_LEN];
    char city[MAX_CITY_LEN];    
    int p_code;
};

struct n_seat {
    int seatNumber; // 1D array
    int passenger;  // if (not taken) then (null or 0)    
};

struct n_seatRow {
    struct n_seat seat[4]; // 4 contiguous instances of seat
};

struct n_seatMatrix {
    struct n_seatRow seatRow[5]; // matrix [43]
};

// Declare global variables to be used in multiple .c files
extern struct n_seatMatrix seatMatrix;

#endif