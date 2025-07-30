/*
Description : This machine project is an airline seater
Author : Sean Russell B. Villeza
Section : 1-5
Last Modified : 05/04/2025
Acknowledgments : 
	
*/

#include <stdio.h>
#include <stdlib.h>

struct row{
    char seats[4];
};

//Declaration of my Functions!

/*
Program modules;
1. Menu
    1.1 Assign Seats
        Print seats -> Input Seat/Choice    -> Seat Assigned             -> Print Updated Seating -> Return Start ()
                                            -> Seat is already filled    -> Return previous step )
                                            -> Return to Menu )
        
    1.2 View Seats
        Print seats -> Input row/all        -> Print all seats          -> Return Previous step ()
                                            -> Print row                -> Return previous step ()
                                            -> Return to Menu )
*/

int assign(struct row *seatDB,int row, char collumn, int *Occupied) {
    if (*Occupied >= 20) return -2;

    int intcollumn;
    row--;
    
    if (collumn > 'D') {
        intcollumn = collumn - 'a';
    } else {
        intcollumn = collumn - 'A';
    }

    if (seatDB[row].seats[intcollumn] == 'X') {
        return -1;
    } else {
        seatDB[row].seats[intcollumn] = 'X';
        *Occupied += 1;
        return 1;
    }
}

int intInput(char *address, int *flag) { 
    *address = -1;
    scanf(" %d", address);
    clear();
    if (*address != 1 && 
        *address != 2 &&
        *address != 3 && 
        *address != 4 &&
        *address != 5 &&
        *address != 0){
    if (!(*address >= 0 && *address <= 5)){
        printf("  > Invalid data type: integers 1 -> 4 are required\n");
        return 1;
        } else if (*address == 0) {
            *flag = 0;
            return 0;
        }
    return 0; 
}
}

int charInput(char *address, int *flag) { 
    *address = -1; //input reset; from previously entered character
    scanf(" %c", address);
    clear();
    if (*address != 'A' && *address != 'a' && 
        *address != 'B' && *address != 'b' && 
        *address != 'C' && *address != 'c' && 
        *address != 'D' && *address != 'd' &&
        *address != '0'){
    if (!(*address >= 'A' && *address <= 'D') && !(*address >= 'a' && *address <= 'd') && *address != '0'){
        printf("  > Invalid data type: Characters A -> B or a -> b is required\n");
        return 1;
        } else if (*address == '0') {
            *flag = 0;
            return 0;
        }
    return 0; 
}
}

void initializeSeats(struct row* seatDB,int *occupancy) {
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 4; j++) {
            seatDB[i].seats[j] = 'A' + j;
        }
    }
    *occupancy = 0;
}

void printSeats(struct row* seatDB) {
    printf("\n");
    printf("  Row\t\tCollumns\n");
    printf("  -------------------------------");
    for(int i = 0; i < 5; i++) {
        printf("\n  %i\t", i + 1);
        for(int j = 0; j < 4; j++) {
            printf("%c\t", seatDB[i].seats[j]);
        }
    }
    printf("\n\n");
}

int seatAssign(struct row* seatDB,int *Occupied) {
    int row;
    char collumn;
    int nreturn = 1;
    printSeats(seatDB);

    do {
        printf("  > Seat Selection: Enter 0 to return\n");
        printf(" -----------------\n");

        printf("  > Row: ");
            while(intInput(&row, &nreturn) != 0){
                printf("  > Row: ");
            }
            if(nreturn == 0) return 0;

        printf("  > Collumn: ");
            while(charInput(&collumn, &nreturn) != 0){
                printf("  > Collumn: ");
            }
            if(nreturn == 0) return 0;

        int result = assign(seatDB, row, collumn, Occupied);
        if (result == -1) {
            printf("  > Seat [%d%c] is occupied; Please PICK ANOTHER \n\n", row, collumn);
        } else {
            printSeats(seatDB);
        }

    }
    while (*Occupied < 20);
    
    printf("  > NOTICE: All seats are now filled [%d/20]; Please exit the program or reset seating\n", *Occupied);
    printf("  > Press any key to return to main menu...");
    getch();
    return 0;
}

int seatView() {

}

void displayMenu() {
    printf("\n\n");
    printf("         -=\\`\\\n");
    printf("     |\\ ____\\_\\__\n");
    printf("   -=\\c`\"\"\"\"\" \"  `)\n");
    printf("      `~~~~~/ /~~`\n");
    printf("        -==/ /\n");
    printf("          '-'\n\n");
    
    printf("  Airplane Seat Operator\n");
    printf("  ----------------------\n");
    printf("  :[Menu]");
    printf("   \n   [1] ASSIGN A SEAT\n   [2] VIEW SEATS\n   [3] RESET SEATS\n   [4] ABOUT\n   [5] QUIT\n");
}

void clear() {
    while (getchar() != '\n'); 
}

int main () {
    int choice;
    int Occupied = 0;
    struct row seatDB [5];
    initializeSeats(seatDB, &Occupied);
    do {
        displayMenu();
        printf("\n   > Enter your choice: ");
        scanf("%d", &choice);
        clear();
        
        switch (choice) {
            case 1:
                seatAssign(seatDB, &Occupied);
                break;
            case 2:
                printf("\n");
                printSeats(seatDB);
                printf("  > Occupancy: [%d/20] are occupied; press any key to continue", Occupied);
                getch();
                break;
            case 3:
                printf("\n   > Resetting seats...\n");
                initializeSeats(seatDB, &Occupied);
                printSeats(seatDB);
                printf("\n  > Seats occupancy replenished [%d/20]; press any key to return to menu", Occupied);
                getch();
                break;
            case 4: 
                printf(" --------------------------------------------------------------\n");
                printf(" | Description : This machine project is an airline seater     |\n");
                printf(" |  Author : Sean Russell B. Villeza                           |\n");
                printf(" |  Section : 1-5                                              |\n");
                printf(" |  Last Modified : 05/04/2025                                 |\n");
                printf(" |  Acknowledgments: Submitted to sir Arnie Fabregas           |\n");
                printf(" ---------------------------------------------------------------\n");
                printf("  > Press any key to continue");
                getch();
                break;
            case 5:
                printf("   > Are you sure you want to quit? (Enter Y to confirm): ");
                char confirm;
                scanf(" %c", &confirm);
                if (confirm == 'y' || confirm == 'Y') {
                    printf("Program exited\n");
                    return 0;
                }
                break;
            default:
            printf("   > Invalid input; try again");
        }

        
    } while (1);

    return 0;
}