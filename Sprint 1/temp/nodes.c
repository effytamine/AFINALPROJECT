#include <stdio.h>
#include <stdlib.h>
#include "nodes.h"

int helper_copy(const char* o_string, char* buffer, int max_len) {
    int i = 0;
    while (o_string[i] != '\0' && i < max_len - 1) {
        buffer[i] = o_string[i];
        i++;
    }
    buffer[i] = '\0';
    return i;
}

struct node* create_node() { // objective: return a new allocated node object with initialized data
    struct node* temp = (struct node *)malloc(sizeof(struct node));
    if (temp == NULL) return NULL;
    temp -> link = NULL;
    return temp;
}

struct node* create_passenger(int seatNumber, char* name, int age, char* street, char* city, int p_code) {
    //edge case: any of the fields is missing
    if (seatNumber < 0 ||
        name == NULL ||
        age < 0 ||
        city == NULL ||
        street == NULL ||
        p_code < 0) return NULL;

    struct node* new_passenger = create_node(); // create a node
    if (new_passenger == NULL) return NULL;

    //define and init new passenger
    struct n_passenger* passenger = (struct n_passenger *)malloc(sizeof(struct n_passenger));
    if (passenger == NULL) return NULL;
    
    // non-array members init
    passenger -> seatNumber = seatNumber;
    passenger -> age = age;
    passenger -> p_code = p_code;
    
    // array-type members init
    helper_copy(name, passenger -> name, MAX_NAME_LEN);
    helper_copy(city, passenger -> city, MAX_CITY_LEN);
    helper_copy(street, passenger -> street, MAX_STREET_LEN);

    // passenger init done; link node member: passenger to init passenger ptr
    new_passenger -> passenger = passenger;

    // link is already NULL from creat_node()
    // return new_passenger as a new node
    return new_passenger;
}

struct node* traverse_list(struct node* tempnode) { // objective: return the address of the next node
    if (tempnode -> link == NULL) return NULL;
        return tempnode -> link;
}

void append_node(struct node* head, struct node* new_passenger) {
    // edge case: empty list
    if (head == NULL) return;

    struct node* last_node = head;
    struct node* temp;

    do { // loop traverse_node() till link == NULL; 
        temp = traverse_list(last_node);
        if (temp != NULL) last_node = temp;
    } while (temp != NULL);

    // last_node is now true
    // connect new_node to last_node -> link

    last_node -> link = new_passenger;
}

struct node* find_node(struct node* head, const int code) { // objective: traverse_list until NULL or current_node -> link  == find_node
    if (head == NULL) return NULL;
    struct node* current_node = head;

    while(current_node != NULL) {
        if (current_node -> passenger -> p_code == code) {
            return current_node;
        }
        current_node = traverse_list(current_node);
    } 
        printf("Object not found");
        return NULL;

}

void remove_node(struct node** head, struct node* obj_address) { // objective: traverse_list until NULL or current_node = obj_address
    if (*head == NULL) return;

    struct node* previous_node = *head;
    struct node* current_node = *head;

    while(current_node != NULL) {
        if (current_node == obj_address) {
            if (current_node == *head) {
                *head = current_node -> link;
            } else {
                previous_node -> link = current_node -> link;
            }
            free(current_node);
            return;
        } 
        previous_node = current_node;
        current_node = traverse_list(current_node);
    }  

    printf("Object not found");
    return;
}
