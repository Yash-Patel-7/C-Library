/*
Circular Doubly Linked List Implementation
Do Not Create any Unused or Unnecessary functionalities that hinder the Software Design Principles.
Press Command F to open the word finder on the page in VSCode.
Replace all "name" with the topic of your data.
Replace all "data" with your data types.
Fill in the "cdllnode_constructor" function to initialize your data.
Fill in the "cdllnode_free" function.
Replace all "cdll *, int" with your data types.
All implementations use only the memory management functionality from GPF.h file.
Remove #include "GPF.h" below.
Copy + paste this entire customization at the bottom of GPF.h file.
*/

#include "GPF.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct name_cdllnode;
struct name_cdll;

typedef struct name_cdllnode {
	struct name_cdllnode *next;
	struct name_cdllnode *prev;
	int data;
} name_cdllnode;

typedef struct name_cdll {
	struct name_cdllnode *tail;
	unsigned int num_of_nodes;
} name_cdll;

name_cdllnode* name_cdllnode_constructor(void);
name_cdll* name_cdll_constructor(void);
name_cdllnode* name_cdllnode_getNext(name_cdllnode *self);
name_cdllnode* name_cdllnode_getPrev(name_cdllnode *self);
void name_cdllnode_setNext(name_cdllnode *self, name_cdllnode *next);
void name_cdllnode_setPrev(name_cdllnode *self, name_cdllnode *prev);
bool name_cdll_isEmpty(name_cdll *self);
name_cdllnode* name_cdllnode_free(name_cdllnode *self);
name_cdll* name_cdll_free(name_cdll *self, bool onlyNodes);
void name_cdll_toString(name_cdll *self);
void name_cdll_addToHead(name_cdll *self, int data);
void name_cdll_addToTail(name_cdll *self, int data);
name_cdllnode* name_cdll_removeHead(name_cdll *self, bool returnHead);
name_cdllnode* name_cdll_removeTail(name_cdll *self, bool returnTail);
void (*name_cdll_enqueue)(name_cdll *, int) = name_cdll_addToTail;
name_cdllnode* (*name_cdll_dequeue)(name_cdll *, bool) = name_cdll_removeHead;
void (*name_cdll_push)(name_cdll *, int) = name_cdll_addToHead;
name_cdllnode* (*name_cdll_pop)(name_cdll *, bool) = name_cdll_removeHead;

name_cdllnode* name_cdllnode_constructor(void) {
	name_cdllnode *self = malloc(sizeof(name_cdllnode));
	(*self).next = NULL;
	(*self).prev = NULL;
	(*self).data = 0;
	return self;
}

name_cdll* name_cdll_constructor(void) {
	name_cdll *self = malloc(sizeof(name_cdll));
	(*self).tail = NULL;
	(*self).num_of_nodes = 0;
	return self;
}

name_cdllnode* name_cdllnode_getNext(name_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	return (*self).next;
}

name_cdllnode* name_cdllnode_getPrev(name_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	return (*self).prev;
}

void name_cdllnode_setNext(name_cdllnode *self, name_cdllnode *next) {
	if (self == NULL) {
		return;
	}
	(*self).next = next;
	return;
}

void name_cdllnode_setPrev(name_cdllnode *self, name_cdllnode *prev) {
	if (self == NULL) {
		return;
	}
	(*self).prev = prev;
	return;
}

bool name_cdll_isEmpty(name_cdll *self) {
	if (self == NULL) {
		return true;
	}
	return ((*self).num_of_nodes == 0);
}

name_cdllnode* name_cdllnode_free(name_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	/*
	COPY + PASTE THIS FOR EVERY DATA POINTER WITH DYNAMICALLY ALLOCATED MEMORY
	(*self).pointer = Free((*self).pointer);
	*/
	self = Free(self);
	return NULL;
}

name_cdll* name_cdll_free(name_cdll *self, bool onlyNodes) {
	if (self == NULL) {
		return NULL;
	}
	unsigned int num_of_nodes = (*self).num_of_nodes;
	for (unsigned int i = 0; i < num_of_nodes; i++) {
		name_cdll_removeHead(self, false);
	}
	if (onlyNodes == false) {
		self = Free(self);
		return NULL;
	}
	return self;
}

void name_cdll_toString(name_cdll *self) {
	if (self == NULL || name_cdll_isEmpty(self)) {
		printf("EMPTY\n");
		return;
	}
	unsigned int num_of_nodes = (*self).num_of_nodes;
	name_cdllnode *head = name_cdllnode_getNext((*self).tail);
	if (num_of_nodes == 1) {
		printf("%d\n", (*head).data);
		return;
	}
	for (unsigned int i = 0; i < num_of_nodes - 1; i++) {
		printf("%d ", (*head).data);
		head = name_cdllnode_getNext(head);
	}
	printf("%d\n", (*head).data);
	return;
}

void name_cdll_addToHead(name_cdll *self, int data) {
	if (self == NULL) {
		return;
	}
	name_cdllnode *newnode = name_cdllnode_constructor();
	(*newnode).data = data;
	if (name_cdll_isEmpty(self)) {
		(*self).tail = newnode;
		(*self).tail->next = newnode;
		(*self).tail->prev = newnode;
		(*self).num_of_nodes++;
		return;
	}
	name_cdllnode_setNext(newnode, name_cdllnode_getNext((*self).tail));
	(*self).tail->next = newnode;
	(*newnode).next->prev = newnode;
	(*newnode).prev = (*self).tail;
	(*self).num_of_nodes++;
	return;
}

void name_cdll_addToTail(name_cdll *self, int data) {
	if (self == NULL) {
		return;
	}
	name_cdllnode *newnode = name_cdllnode_constructor();
	(*newnode).data = data;
	if (name_cdll_isEmpty(self)) {
		(*self).tail = newnode;
		(*self).tail->next = newnode;
		(*self).tail->prev = newnode;
		(*self).num_of_nodes++;
		return;
	}
	name_cdllnode_setNext(newnode, name_cdllnode_getNext((*self).tail));
	name_cdllnode_setNext((*self).tail, newnode);
	(*newnode).prev = (*self).tail;
	(*self).tail = newnode;
	(*self).tail->next->prev = (*self).tail;
	(*self).num_of_nodes++;
	return;
}

name_cdllnode* name_cdll_removeHead(name_cdll *self, bool returnHead) {
	if (self == NULL || name_cdll_isEmpty(self)) {
		return NULL;
	}
	name_cdllnode *head = name_cdllnode_getNext((*self).tail);
	(*self).tail->next = (*head).next;
	(*self).tail->next->prev = (*self).tail;
	(*head).next = NULL;
	(*head).prev = NULL;
	(*self).num_of_nodes--;
	if (name_cdll_isEmpty(self)) {
		(*self).tail = NULL;
	}
	if (returnHead == true) {
		return head;
	}
	head = name_cdllnode_free(head);
	return NULL;
}

name_cdllnode* name_cdll_removeTail(name_cdll *self, bool returnTail) {
	if (self == NULL || name_cdll_isEmpty(self)) {
		return NULL;
	}
	name_cdllnode *tail = (*self).tail;
	name_cdllnode *prev = (*self).tail->prev;
	(*prev).next = (*tail).next;
	(*tail).next->prev = prev;
	(*self).tail = prev;
	(*tail).next = NULL;
	(*tail).prev = NULL;
	(*self).num_of_nodes--;
	if (name_cdll_isEmpty(self)) {
		(*self).tail = NULL;
	}
	if (returnTail == true) {
		return tail;
	}
	tail = name_cdllnode_free(tail);
	return NULL;
}


