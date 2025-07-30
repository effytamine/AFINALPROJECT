#include <stdio.h>
#include <stdlib.h>

//data
struct student{
    int id;
    char name[30];
    char course[30];
    int year;
};
//functions
void displayMenu() {
    printf("       _________\n");
    printf("      / ======= \\\n");
    printf("     / __________\\\n");
    printf("    | ___________ |\n");
    printf("    | | -       | |\n");
    printf("    | |         | |\n");
    printf("    | |_________| |\n");
    printf("    \\=____________/\n");
    printf("    /'''''''''''''\\ \n");
    printf("   / ::::::::::::: \\ \n");
    printf("  (_________________)\n");
    printf("\n");
    printf("  STUDENT INFORMATION SYSTEM INPUT\n");
    printf("  [input]:\n");
    printf("  [1] Input student information\n");
    printf("  [2] About program\n");
    printf("  [3] Exit\n");

}

void printProgramInfo() {
    printf("  --------------------------------------------------------------\n");
    printf("  |  Description : This machine project inputs student info     |\n");
    printf("  |  Author : Sean Russell B. Villeza                           |\n");
    printf("  |  Section : 1-5                                              |\n");
    printf("  |  Last Modified : 26/04/2025                                 |\n");
    printf("  |  Acknowledgments: Submitted to sir Arnie Fabregas           |\n");
    printf("  ---------------------------------------------------------------\n");
}

void printStudents(struct student *stds) {
    printf("  %-9s\t%-30s\t%-30s\t%-10s\n", "Student no.", "Name", "Course", "Year");

    for(int i= 0; i < 5; i++) {
            printf("  %-9d\t%-30s\t%-30s\t", stds[i].id, stds[i].name, stds[i].course);

            switch (stds[i].year) {
                case 1:
                    printf("1st Year\n");
                    break;
                case 2:
                    printf("2nd Year\n");
                    break;
                case 3:
                    printf("3rd Year\n");
                    break;
                case 4:
                    printf("4th Year\n");
                    break;
        }
    }

}
void verify(char *array){
    for(int i = 0; i < 30; i++) {
        if(array[i] == '\n') {
            array[i] = '\0';
        }
    }
}

int inputstuds(struct student *stds){
    while (getchar() != '\n'); 
        for(int i = 0 ; i < 5; i++) {
            printf("  [STUDENT NO %i]\n", i+1);

            printf("  > Student YEAR (indicate a number (ex. 1, 2, 3, 4)): ");
            scanf(" %1d", &stds[i].year);
            
            printf("  > Student ID (8-digits): ");
            scanf(" %8d", &stds[i].id);
            
            printf("  > Student NAME (30 Characters max): ");
            while (getchar() != '\n');
            fgets(stds[i].name, 30 * sizeof(char), stdin);
            verify(stds[i].name);

            printf("  > Student COURSE (30 Characters max): ");
            fgets(stds[i].course, 30 * sizeof(char), stdin);
            verify(stds[i].course);

            printf("\n");
    }
    printStudents(stds);
    printf("\n");
    return 1;
}

//main menu
int main() {
    int choice;
    struct student stds[10];
    do {
        displayMenu();
        printf("  > Enter your choice: ");
        int choice = -1;
        scanf(" %d", &choice);

        switch(choice) {
            case 1:
                inputstuds(stds);
                printf("  > Above is the list of inputted students, press enter to continue...");
                getchar();
                break;
            case 2:
                printProgramInfo();
                printf("  Press enter to continue;\n");
                while (getchar() != '\n'); 
                getchar();
                break;
            case 3:
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
            printf("  > Invalid input; Press enter to try again\n");
            printf("  Press enter to continue;\n");
            while (getchar() != '\n'); 
            getchar();
        }
    } while (1);
    
    return 0;

}