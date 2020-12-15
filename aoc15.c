#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// I use a modified version of https://www.codesdope.com/blog/article/binary-search-tree-in-c/
// A hashmap would be much nicer and may be faster, but I wanted the whole code except some basic
// libs here.
// Unfortunally it takes a lot of time to calculate, but it is handabel don´t forget to add -O3 for
// compiling. 
struct node {
    int data; //node will store an integer
    int last;
    struct node *right_child; // right child
    struct node *left_child; // left child
};

struct node* search(struct node *root, int x) {
    if (root == NULL || root->data == x) { //if root->data is x then the element is found
        return root;
    } else if (x > root->data) { // x is greater, so we will search the right subtree
        return search(root->right_child, x);
    } else { //x is smaller than the data, so we will search the left subtree
        return search(root->left_child, x);
    }
}

// function to create a node
struct node* new_node(int x, int last) {
    struct node *p;
    p = malloc(sizeof(struct node));
    p->data = x;
    p->last = last;
    p->left_child = NULL;
    p->right_child = NULL;

    return p;
}

struct node* insert(struct node *root, int x, int last) {
    // searching for the place to insert
    if (root == NULL) {
        return new_node(x, last);
    } else if (x > root->data) { // x is greater. Should be inserted to right
        root->right_child = insert(root->right_child, x, last);
    } else { // x is smaller should be inserted to left
        root->left_child = insert(root->left_child, x, last);
    }
    return root;
}

int game(char *input, int end) {
    char *s = input;
    int pos;

    // parse input
    pos = 0;
    struct node *root = NULL;
    int value;
    while (*s) {
        value = strtol(s, &s, 0);
        if (pos == 0) {
            root = new_node(value, 0);
        } else {
            insert(root, value, pos);
        }
        pos++;
        if (*s == ',' || *s == 10) {
            s++;
        }
    }
    // play game
    struct node *found = NULL;
    int nextValue = 0; // new value
    while (pos < end) {
        value = nextValue;
        found = search(root, value);
        if (found == NULL) {
            nextValue = 0;
            insert(root, value, pos);
        } else {
            nextValue = pos - found->last;
            found->last = pos;
        }
        pos++;
    }
    return value;
}

int main(void) {
    //printf("Test p1 1 436 = %d\n", game("0,3,6", 2020));
    //printf("Test p1 2 10 = %d\n", game("2,1,3"), 2020);
    printf("result part1 = %d\n", game("0,13,1,8,6,15", 2020));
    //printf("Test p2 1  175594 = %d\n", game("0,3,6", 30000000));
    printf("result part2 = %d\n", game("0,13,1,8,6,15", 30000000));
    return 0;
}
