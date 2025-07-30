#ifndef NODES_H
#define NODES_H
#include "globals.h"

struct node { // node definition
    struct n_passenger* passenger;
    struct node* link; 
};

struct node* create_passenger(int seatNumber, char* name, int age, char* street, char* city, int p_code);
struct node* create_node();
struct node* traverse_list(struct node* tempnode);
void append_node(struct node* head, struct node* new_passenger);
struct node* find_node(struct node* head, int find_data);
void remove_node(struct node** head, struct node* obj_address);

#endif