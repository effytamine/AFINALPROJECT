#include <stdio.h>
#include <stdlib.h>
#include "nodes.h"
#include "globals.h"

void show_main_menu(int mode) {
    printf("\n");
    printf("      _    ___ ____  _     ___ _   _ _____                                   \n");
    printf("     / \\  |_ _|  _ \\| |   |_ _| \\ | | ____|                                  \n");
    printf("    / _ \\  | || |_) | |    | ||  \\| |  _|                                    \n");
    printf("   / ___ \\ | ||  _ <| |___ | || |\\  | |___                                   \n");
    printf("  /_/__ \\_\\___|_| \\_\\_____|___|_| \\_|_____|___ ____      _  _____ ___  ____  \n");
    printf("  / ___|| ____|  / \\|_   _|  / _ \\|  _ \\| ____|  _ \\    / \\|_   _/ _ \\|  _ \\ \n");
    printf("  \\___ \\|  _|   / _ \\ | |   | | | | |_) |  _| | |_) |  / _ \\ | || | | | |_) |\n");
    printf("   ___) | |___ / ___ \\| |   | |_| |  __/| |___|  _ <  / ___ \\| || |_| |  _ < \n");
    printf("  |____/|_____/_/   \\_\\_|    \\___/|_|   |_____|_| \\_\\/_/   \\_\\_| \\___/|_| \\_\\\n");
    printf("\n");
    printf("  [ AIRLINE SEAT OPERATOR ]: ONLINE\n\n");

    switch(mode){
        case 1: 
            pre_menu();
            break;
        case 2:
            break;
    }
}

void pre_menu() {
    printf("  [1] New Flight\n  [2] Load Flight\n  [3] How to Use\n  [5] About\n  [4] Exit\n");
}

int main() {

    int choice;
    char nQuit;

    do {
        show_main_menu(1);
        printf("\n  > enter choice : ");
        choice = -1;
        scanf(" %d", &choice);

        seatMatrix.seatRow[0].seat[0].seatNumber = 1;
        seatMatrix.seatRow[0].seat[0].passenger = 42;

        switch(choice) {
            case 1:
                // new flight
                break;
            case 2:
                // load flight
                break;
            case 3:
                break;
            case 4: 
                break;
            case 5:
                printf("\n  : Would you like to exit Airline Seat Operator?");
                printf("\n  : [Y] Yes [N] No");
                printf("\n\n  > enter choice : ");
                scanf(" %c", &nQuit);
                    if (nQuit == 'Y' || nQuit == 'y') return 0;
                break;
        }
    } while (1);
}