// http://www.kaushikbaruah.com/posts/data-structure-in-c-hashmap/
#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>

struct node {
    int key;
    int val;
    struct node *next;
} node;

struct table {
    int size;
    struct node **list;
} table;

struct table *createTable(int size);
int hashCode(struct table *t, int key);
void insert(struct table *t, int key, int val);
int lookup(struct table *t, int key);

#endif
