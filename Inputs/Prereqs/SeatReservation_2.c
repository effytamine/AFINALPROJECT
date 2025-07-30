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

void clear() {
    while (getchar() != '\n'); 
}

int seatOperate(struct row *seatDB, int *row, char *collumn, int mode, int *Occupied) {
    int intcollumn[2];
    int repeat = 1;
    if (mode == 3) repeat = 2;

    //clean rows and collumns for operation
    for (int i = 0; i < repeat; i++) {
        row[i] -= 1; // minus

        if (collumn[i] > 'D') { 
            collumn[i] -= 32;
        } 
        intcollumn[i] = collumn[i] - 'A';
    }

    switch (mode) {
        case 1:
            if (seatDB[row[0]].seats[intcollumn[0]] == 'X') {
                return -1;
            } else {
                seatDB[row[0]].seats[intcollumn[0]] = 'X';
                *Occupied += 1;
                return 1;
            }
        break;
        case 2:
            if (seatDB[row[0]].seats[intcollumn[0]] != 'X') {
                return -2;
            } else {
                seatDB[row[0]].seats[intcollumn[0]] = collumn[0];
                *Occupied -= 1;
                return 2;
            }
        break;
        case 3:
            //check if initial seat is taken and target seat is free;
            if ((seatDB[row[0]].seats[intcollumn[0]] == 'X') && (seatDB[row[1]].seats[intcollumn[1]] != 'X')) {
                seatDB[row[0]].seats[intcollumn[0]] = collumn[0];
                seatDB[row[1]].seats[intcollumn[1]] = 'X';
                return 3;
            }
            else {
                return -3;
            } 
        break;
    }
}

int intInput(int *address, int *flag) { 
    *address = -1; //input reset; from previously entered character
    scanf(" %d", address);
    clear();
    if (!(*address >= 0 && *address <= 5)){
        printf("  > Invalid data type: integers 1 -> 4 are required\n");
        return 1;
        } else if (*address == 0) {
            *flag = 0;
            return 0;
        }
    return 0; 
}

int charInput(char *address,int *flag) { 
    *address = -1; //input reset; from previously entered character
    scanf(" %c", address);
    clear();
    if (!(*address >= 'A' && *address <= 'D') && !(*address >= 'a' && *address <= 'd') && *address != '0'){
        printf("  > Invalid data type: Characters A -> B or a -> b is required\n");
        return 1;
        } else if (*address == '0') {
            *flag = 0;
            return 0;
        }
    return 0; 
}

void initializeSeats(struct row* seatDB,int *occupancy) {
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 4; j++) {
            seatDB[i].seats[j] = 'A' + j;
            // seatDB[i].seats[j] = 'X'; fot testing
        }
    }
    *occupancy = 0;
    // *occupancy = 20; // for testing
}

void printSeats(struct row* seatDB) {
    printf("\n\n");
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

int seatInput(struct row* seatDB, int *Occupied, int mode) {
    int row[2];
    char collumn[2];
    int nreturn = 1;
    int repeat = 1;
    int flag = 1;

    if (mode == 3) repeat = 2;

    printSeats(seatDB);

    do {
        printf("  MODE : [");
        switch (mode) {
            case 1:
                printf("ASSIGN]\n");
                printf("  > Seat Input: Enter 0 to return\n");
                printf(" -----------------\n");
            break;
            case 2:
                printf("CANCEL]\n");
                printf("  > Seat Input: Enter 0 to return\n");
                printf(" -----------------\n");
            break;
            case 3:
                printf("CHANGE]\n");
                printf("  > Seat Input (Current Seat): Enter 0 to return\n");
                printf(" -----------------\n");
            break;
        }

        for (int i = 0; i < repeat; i++) {
            printf("  > Row: ");
                while(intInput(&row[i], &nreturn) != 0){
                    printf("  > Row: ");
                }
            if(nreturn == 0) return 0;

            printf("  > Collumn: ");
                while(charInput(&collumn[i], &nreturn) != 0){
                    printf("  > Collumn: ");
                }
            if(nreturn == 0) return 0;

            if(mode == 3 && i < 1) {
                printf("\n  > Seat Input (Target Seat): Enter 0 to return\n");
                printf(" -----------------\n");
            }
        }

        int result = seatOperate(seatDB, row, collumn, mode, Occupied);
            row[0] += 1; // transform back to original
            if (mode == 3)row[1] += 1;

            switch (result) {
                case -1:
                    printf("  > Seat [%d%c] is occupied; Please PICK ANOTHER \n\n", row[0], collumn[0]);
                break;
                case 1:
                    printf("  > SYSTEM: Seat [%d%c] Assigned", row[0], collumn[0]);
                    printSeats(seatDB);
                break;
                case -2:
                printf("  > Seat [%d%c] is not occupied; cannot cancel; Please PICK ANOTHER \n\n", row[0], collumn[0]);
                break;
                case 2:
                    printf("  > SYSTEM: Seat [%d%c] Cancelled", row[0], collumn[0]);
                    printSeats(seatDB);
                break;
                case -3:
                    printf("  > One of the seats [%d%c] or [%d%c] is ineligeble; Current seat must be occupied and Target seat must\n     be unoccupied to successfully CHANGE SEATS, Press enter to try again..\n\n", row[0], collumn[0], row[1], collumn[1]);
                    getchar();
                    printSeats(seatDB);
                break;
                case 3:
                    printf("  > SYSTEM: Seat changed from [%d%c] to [%d%c]", row[0], collumn[0], row[1], collumn[1]);
                    printSeats(seatDB);
                break;
            }

            switch (mode) { // check if repeat
                case 1:
                    if (*Occupied >= 20) {
                        printf("  > NOTICE: All seats are now occupied [%d/20]; Please exit the program, CANCEL A SEAT, or RESET SEATS\n", *Occupied);
                        printf("  > Press enter to return to main menu...");
                        flag = 0;
                        getchar();
                    }
                break;
                case 2:
                    if (*Occupied == 0) {
                        printf("  > NOTICE: All seats are now unoccupied [%d/20]; Please exit the program, or ASSIGN A SEAT\n", *Occupied);
                        printf("  > Press enter to return to main menu...");
                        flag = 0;
                        getchar();
                    }
                break;
                case 3:
                if (*Occupied >= 20 && *Occupied == 0) {
                        printf("  > NOTICE: There are no seats eligible for changing [%d/20]; Please exit the program, ASSIGN A SEAT or CANCEL A SEAT\n", *Occupied);
                        printf("  > Press enter to return to main menu...");
                        flag = 0;
                        getchar();
                }
                break;
            }
        } while (flag != 0);
        
        return 0;

    }

int seatView(struct row *seatDB, int *Occupied) {
    printSeats(seatDB);
    printf("  > Occupancy: [%d/20] are occupied; press enter to continue", *Occupied);
    getchar();
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
    printf("   \n   [1] ASSIGN A SEAT\n   [2] CANCEL A SEAT\n   [3] CHANGE SEATS\n   [4] VIEW SEATS\n   [5] RESET SEATS\n   [6] ABOUT\n   [7] QUIT\n");
}

void printProgramInfo() {
    printf("  --------------------------------------------------------------\n");
    printf("  | Description : This machine project is an airline seater     |\n");
    printf("  |  Author : Sean Russell B. Villeza                           |\n");
    printf("  |  Section : 1-5                                              |\n");
    printf("  |  Last Modified : 12/04/2025                                 |\n");
    printf("  |  Acknowledgments: Submitted to sir Arnie Fabregas           |\n");
    printf("  ---------------------------------------------------------------\n");
}

int main () {
    int choice;
    int Occupied = 0;
    struct row seatDB [5];
    initializeSeats(seatDB, &Occupied);
    do {
        displayMenu();
        printf("\n   > Enter your choice: ");
        choice = -1;
        scanf("%d", &choice);
        clear();
        
        switch (choice) {
            case 1:
                seatInput(seatDB, &Occupied, 1);
                break;
            case 2: 
                seatInput(seatDB, &Occupied, 2);
                break;
            case 3:
                seatInput(seatDB, &Occupied, 3);
                break;
            case 4:
                seatView(seatDB, &Occupied);
                break;
            case 5:
                printf("\n   > Resetting seats...\n");
                initializeSeats(seatDB, &Occupied);
                printSeats(seatDB);
                printf("\n  > Seats occupancy replenished [%d/20]; press enter to return to menu", Occupied);
                getchar();
                break;
            case 6: 
                printProgramInfo();
                printf("  > Press enter to continue");
                getchar();
                break;
            case 7:
                printf("   > Are you sure you want to quit? (Enter Y to confirm): ");
                char confirm;
                scanf(" %c", &confirm);
                if (confirm == 'y' || confirm == 'Y') {
                    printProgramInfo();
                    printf("  -------------------------------------\n");
                    printf("  | Thank you for using the program!! |\n");
                    printf("  -------------------------------------\n");
                    printf("  > Program exited\n");
                    return 0;
                }
                break;
            default:
            printf("   > Invalid input; Press enter to try again\n");
            getchar();
        }

    } while (1);

    return 0;
}