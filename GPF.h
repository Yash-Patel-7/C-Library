/*
General Purpose Functions
Strictly Not Customizable: Do Not Change GPF Functionalities.
Do Not Create any Unused or Unnecessary functionalities that hinder the Software Design Principles.
*/

/*
Documentation is given below for all GPF functionalities.
*/

/*
Memory Management Setup:
Place this declaration at the top of your program's file: 
	memory_cdll *memory;
Within your program's main function, place this at the beginning: 
	memory = memory_cdll_constructor();
After each call to malloc(), place this: 
	memory_cdll_push(memory, pointer);
When you want to free the memory: 
	In the middle of your program: 
		memory_cdll_free(memory, true);
	At the end of your program: 
		memory_cdll_free(memory, false);
If you want to store temporary memory for a target function, consider creating a new memory_cdll object:
	Within your target function, place this at the beginning:
		memory_cdll *tmp_memory = memory_cdll_constructor();
	Continue to use the push function: 
		memory_cdll_push(tmp_memory, pointer);
	Free the tmp_memory as needed in the middle or at the end of your target function.
	You should create a new memory_cdll object for each target process or algorithm.
If you need to free a pointer manually in your program:
	pointer = Free(pointer);
	You can re-use the same pointer again, if you want, because it is set to NULL after it is freed.
*/

/*
readLine_char_firstD* readLine(FILE *fp, readLine_char_firstD *line, unsigned int count_chars_in_delimiter, char *delimiter):
	Reads line by line from a file.
	Returns a line, split into parts using the delimiter.
	(*line).i[index1].j = Gives a char *j string at index1.
	(*line).i[index1].j[index2] = Gives a char at (index1, index2).
	(*line).size = Gives the size of the i-array, which is an array of structs that each contain a char *j string.
	(*line).i[index1].size = Gives the size of the char *j string contained in the struct at index1.
	Visual:
	The Left side is an array of structs i[{index1}, {index2}, {index3},] going downwards, and its size (First Dimension).
	The Right side is an array of characters, also known as a string, and its size (Second Dimension).
	i[
	{index1}, -> j"String 1\0", size = 9
	{index2}, -> j"String 2\0", size = 9
	{index3}, -> j"String 3\0", size = 9
	], size = 3
	Use:
	readLine_char_firstD *line = NULL;
	while ((line = readLine(fp, line, 3, ",- ")) != NULL) {
		if ((*line).size == 0) {
			// What to do if it is a blank line. By default, it is skipped.
			continue;
		}
		// Process the line here
	}
	line = readLine_free(line);
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void* Free(void *pointer) {
	free(pointer);
	pointer = NULL;
	return NULL;
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct memory_cdllnode;
struct memory_cdll;

typedef struct memory_cdllnode {
	struct memory_cdllnode *next;
	struct memory_cdllnode *prev;
	void *pointer;
} memory_cdllnode;

typedef struct memory_cdll {
	struct memory_cdllnode *tail;
	unsigned int num_of_nodes;
} memory_cdll;

memory_cdllnode* memory_cdllnode_constructor(void);
memory_cdll* memory_cdll_constructor(void);
memory_cdllnode* memory_cdllnode_getNext(memory_cdllnode *self);
memory_cdllnode* memory_cdllnode_getPrev(memory_cdllnode *self);
void memory_cdllnode_setNext(memory_cdllnode *self, memory_cdllnode *next);
void memory_cdllnode_setPrev(memory_cdllnode *self, memory_cdllnode *prev);
bool memory_cdll_isEmpty(memory_cdll *self);
memory_cdllnode* memory_cdllnode_free(memory_cdllnode *self);
memory_cdll* memory_cdll_free(memory_cdll *self, bool onlyNodes);
void memory_cdll_toString(memory_cdll *self);
void memory_cdll_addToHead(memory_cdll *self, void *pointer);
void memory_cdll_addToTail(memory_cdll *self, void *pointer);
memory_cdllnode* memory_cdll_removeHead(memory_cdll *self, bool returnHead);
memory_cdllnode* memory_cdll_removeTail(memory_cdll *self, bool returnTail);
void (*memory_cdll_enqueue)(memory_cdll *, void*) = memory_cdll_addToTail;
memory_cdllnode* (*memory_cdll_dequeue)(memory_cdll *, bool) = memory_cdll_removeHead;
void (*memory_cdll_push)(memory_cdll *, void*) = memory_cdll_addToHead;
memory_cdllnode* (*memory_cdll_pop)(memory_cdll *, bool) = memory_cdll_removeHead;

memory_cdllnode* memory_cdllnode_constructor(void) {
	memory_cdllnode *self = malloc(sizeof(memory_cdllnode));
	(*self).next = NULL;
	(*self).prev = NULL;
	(*self).pointer = NULL;
	return self;
}

memory_cdll* memory_cdll_constructor(void) {
	memory_cdll *self = malloc(sizeof(memory_cdll));
	(*self).tail = NULL;
	(*self).num_of_nodes = 0;
	return self;
}

memory_cdllnode* memory_cdllnode_getNext(memory_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	return (*self).next;
}

memory_cdllnode* memory_cdllnode_getPrev(memory_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	return (*self).prev;
}

void memory_cdllnode_setNext(memory_cdllnode *self, memory_cdllnode *next) {
	if (self == NULL) {
		return;
	}
	(*self).next = next;
	return;
}

void memory_cdllnode_setPrev(memory_cdllnode *self, memory_cdllnode *prev) {
	if (self == NULL) {
		return;
	}
	(*self).prev = prev;
	return;
}

bool memory_cdll_isEmpty(memory_cdll *self) {
	if (self == NULL) {
		return true;
	}
	return ((*self).num_of_nodes == 0);
}

memory_cdllnode* memory_cdllnode_free(memory_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	(*self).pointer = Free((*self).pointer);
	self = Free(self);
	return NULL;
}

memory_cdll* memory_cdll_free(memory_cdll *self, bool onlyNodes) {
	if (self == NULL) {
		return NULL;
	}
	unsigned int num_of_nodes = (*self).num_of_nodes;
	for (unsigned int i = 0; i < num_of_nodes; i++) {
		memory_cdll_removeHead(self, false);
	}
	if (onlyNodes == false) {
		self = Free(self);
		return NULL;
	}
	return self;
}

void memory_cdll_toString(memory_cdll *self) {
	if (self == NULL || memory_cdll_isEmpty(self)) {
		printf("EMPTY\n");
		return;
	}
	unsigned int num_of_nodes = (*self).num_of_nodes;
	memory_cdllnode *head = memory_cdllnode_getNext((*self).tail);
	if (num_of_nodes == 1) {
		printf("%p\n", (*head).pointer);
		return;
	}
	for (unsigned int i = 0; i < num_of_nodes - 1; i++) {
		printf("%p ", (*head).pointer);
		head = memory_cdllnode_getNext(head);
	}
	printf("%p\n", (*head).pointer);
	return;
}

void memory_cdll_addToHead(memory_cdll *self, void *pointer) {
	if (self == NULL) {
		return;
	}
	memory_cdllnode *newnode = memory_cdllnode_constructor();
	(*newnode).pointer = pointer;
	if (memory_cdll_isEmpty(self)) {
		(*self).tail = newnode;
		(*self).tail->next = newnode;
		(*self).tail->prev = newnode;
		(*self).num_of_nodes++;
		return;
	}
	memory_cdllnode_setNext(newnode, memory_cdllnode_getNext((*self).tail));
	(*self).tail->next = newnode;
	(*newnode).next->prev = newnode;
	(*newnode).prev = (*self).tail;
	(*self).num_of_nodes++;
	return;
}

void memory_cdll_addToTail(memory_cdll *self, void *pointer) {
	if (self == NULL) {
		return;
	}
	memory_cdllnode *newnode = memory_cdllnode_constructor();
	(*newnode).pointer = pointer;
	if (memory_cdll_isEmpty(self)) {
		(*self).tail = newnode;
		(*self).tail->next = newnode;
		(*self).tail->prev = newnode;
		(*self).num_of_nodes++;
		return;
	}
	memory_cdllnode_setNext(newnode, memory_cdllnode_getNext((*self).tail));
	memory_cdllnode_setNext((*self).tail, newnode);
	(*newnode).prev = (*self).tail;
	(*self).tail = newnode;
	(*self).tail->next->prev = (*self).tail;
	(*self).num_of_nodes++;
	return;
}

memory_cdllnode* memory_cdll_removeHead(memory_cdll *self, bool returnHead) {
	if (self == NULL || memory_cdll_isEmpty(self)) {
		return NULL;
	}
	memory_cdllnode *head = memory_cdllnode_getNext((*self).tail);
	(*self).tail->next = (*head).next;
	(*self).tail->next->prev = (*self).tail;
	(*head).next = NULL;
	(*head).prev = NULL;
	(*self).num_of_nodes--;
	if (memory_cdll_isEmpty(self)) {
		(*self).tail = NULL;
	}
	if (returnHead == true) {
		return head;
	}
	head = memory_cdllnode_free(head);
	return NULL;
}

memory_cdllnode* memory_cdll_removeTail(memory_cdll *self, bool returnTail) {
	if (self == NULL || memory_cdll_isEmpty(self)) {
		return NULL;
	}
	memory_cdllnode *tail = (*self).tail;
	memory_cdllnode *prev = (*self).tail->prev;
	(*prev).next = (*tail).next;
	(*tail).next->prev = prev;
	(*self).tail = prev;
	(*tail).next = NULL;
	(*tail).prev = NULL;
	(*self).num_of_nodes--;
	if (memory_cdll_isEmpty(self)) {
		(*self).tail = NULL;
	}
	if (returnTail == true) {
		return tail;
	}
	tail = memory_cdllnode_free(tail);
	return NULL;
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct readLine_cdllnode;
struct readLine_cdll;

typedef struct readLine_cdllnode {
	struct readLine_cdllnode *next;
	struct readLine_cdllnode *prev;
	char c;
} readLine_cdllnode;

typedef struct readLine_cdll {
	struct readLine_cdllnode *tail;
	unsigned int num_of_nodes;
} readLine_cdll;

readLine_cdllnode* readLine_cdllnode_constructor(void);
readLine_cdll* readLine_cdll_constructor(void);
readLine_cdllnode* readLine_cdllnode_getNext(readLine_cdllnode *self);
readLine_cdllnode* readLine_cdllnode_getPrev(readLine_cdllnode *self);
void readLine_cdllnode_setNext(readLine_cdllnode *self, readLine_cdllnode *next);
void readLine_cdllnode_setPrev(readLine_cdllnode *self, readLine_cdllnode *prev);
bool readLine_cdll_isEmpty(readLine_cdll *self);
readLine_cdllnode* readLine_cdllnode_free(readLine_cdllnode *self);
readLine_cdll* readLine_cdll_free(readLine_cdll *self, bool onlyNodes);
void readLine_cdll_toString(readLine_cdll *self);
void readLine_cdll_addToHead(readLine_cdll *self, char c);
void readLine_cdll_addToTail(readLine_cdll *self, char c);
readLine_cdllnode* readLine_cdll_removeHead(readLine_cdll *self, bool returnHead);
readLine_cdllnode* readLine_cdll_removeTail(readLine_cdll *self, bool returnTail);
void (*readLine_cdll_enqueue)(readLine_cdll *, char) = readLine_cdll_addToTail;
readLine_cdllnode* (*readLine_cdll_dequeue)(readLine_cdll *, bool) = readLine_cdll_removeHead;
void (*readLine_cdll_push)(readLine_cdll *, char) = readLine_cdll_addToHead;
readLine_cdllnode* (*readLine_cdll_pop)(readLine_cdll *, bool) = readLine_cdll_removeHead;

readLine_cdllnode* readLine_cdllnode_constructor(void) {
	readLine_cdllnode *self = malloc(sizeof(readLine_cdllnode));
	(*self).next = NULL;
	(*self).prev = NULL;
	(*self).c = '\0';
	return self;
}

readLine_cdll* readLine_cdll_constructor(void) {
	readLine_cdll *self = malloc(sizeof(readLine_cdll));
	(*self).tail = NULL;
	(*self).num_of_nodes = 0;
	return self;
}

readLine_cdllnode* readLine_cdllnode_getNext(readLine_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	return (*self).next;
}

readLine_cdllnode* readLine_cdllnode_getPrev(readLine_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	return (*self).prev;
}

void readLine_cdllnode_setNext(readLine_cdllnode *self, readLine_cdllnode *next) {
	if (self == NULL) {
		return;
	}
	(*self).next = next;
	return;
}

void readLine_cdllnode_setPrev(readLine_cdllnode *self, readLine_cdllnode *prev) {
	if (self == NULL) {
		return;
	}
	(*self).prev = prev;
	return;
}

bool readLine_cdll_isEmpty(readLine_cdll *self) {
	if (self == NULL) {
		return true;
	}
	return ((*self).num_of_nodes == 0);
}

readLine_cdllnode* readLine_cdllnode_free(readLine_cdllnode *self) {
	if (self == NULL) {
		return NULL;
	}
	self = Free(self);
	return NULL;
}

readLine_cdll* readLine_cdll_free(readLine_cdll *self, bool onlyNodes) {
	if (self == NULL) {
		return NULL;
	}
	unsigned int num_of_nodes = (*self).num_of_nodes;
	for (unsigned int i = 0; i < num_of_nodes; i++) {
		readLine_cdll_removeHead(self, false);
	}
	if (onlyNodes == false) {
		self = Free(self);
		return NULL;
	}
	return self;
}

void readLine_cdll_toString(readLine_cdll *self) {
	if (self == NULL || readLine_cdll_isEmpty(self)) {
		printf("EMPTY\n");
		return;
	}
	unsigned int num_of_nodes = (*self).num_of_nodes;
	readLine_cdllnode *head = readLine_cdllnode_getNext((*self).tail);
	if (num_of_nodes == 1) {
		printf("%c\n", (*head).c);
		return;
	}
	for (unsigned int i = 0; i < num_of_nodes - 1; i++) {
		printf("%c ", (*head).c);
		head = readLine_cdllnode_getNext(head);
	}
	printf("%c\n", (*head).c);
	return;
}

void readLine_cdll_addToHead(readLine_cdll *self, char c) {
	if (self == NULL) {
		return;
	}
	readLine_cdllnode *newnode = readLine_cdllnode_constructor();
	(*newnode).c = c;
	if (readLine_cdll_isEmpty(self)) {
		(*self).tail = newnode;
		(*self).tail->next = newnode;
		(*self).tail->prev = newnode;
		(*self).num_of_nodes++;
		return;
	}
	readLine_cdllnode_setNext(newnode, readLine_cdllnode_getNext((*self).tail));
	(*self).tail->next = newnode;
	(*newnode).next->prev = newnode;
	(*newnode).prev = (*self).tail;
	(*self).num_of_nodes++;
	return;
}

void readLine_cdll_addToTail(readLine_cdll *self, char c) {
	if (self == NULL) {
		return;
	}
	readLine_cdllnode *newnode = readLine_cdllnode_constructor();
	(*newnode).c = c;
	if (readLine_cdll_isEmpty(self)) {
		(*self).tail = newnode;
		(*self).tail->next = newnode;
		(*self).tail->prev = newnode;
		(*self).num_of_nodes++;
		return;
	}
	readLine_cdllnode_setNext(newnode, readLine_cdllnode_getNext((*self).tail));
	readLine_cdllnode_setNext((*self).tail, newnode);
	(*newnode).prev = (*self).tail;
	(*self).tail = newnode;
	(*self).tail->next->prev = (*self).tail;
	(*self).num_of_nodes++;
	return;
}

readLine_cdllnode* readLine_cdll_removeHead(readLine_cdll *self, bool returnHead) {
	if (self == NULL || readLine_cdll_isEmpty(self)) {
		return NULL;
	}
	readLine_cdllnode *head = readLine_cdllnode_getNext((*self).tail);
	(*self).tail->next = (*head).next;
	(*self).tail->next->prev = (*self).tail;
	(*head).next = NULL;
	(*head).prev = NULL;
	(*self).num_of_nodes--;
	if (readLine_cdll_isEmpty(self)) {
		(*self).tail = NULL;
	}
	if (returnHead == true) {
		return head;
	}
	head = readLine_cdllnode_free(head);
	return NULL;
}

readLine_cdllnode* readLine_cdll_removeTail(readLine_cdll *self, bool returnTail) {
	if (self == NULL || readLine_cdll_isEmpty(self)) {
		return NULL;
	}
	readLine_cdllnode *tail = (*self).tail;
	readLine_cdllnode *prev = (*self).tail->prev;
	(*prev).next = (*tail).next;
	(*tail).next->prev = prev;
	(*self).tail = prev;
	(*tail).next = NULL;
	(*tail).prev = NULL;
	(*self).num_of_nodes--;
	if (readLine_cdll_isEmpty(self)) {
		(*self).tail = NULL;
	}
	if (returnTail == true) {
		return tail;
	}
	tail = readLine_cdllnode_free(tail);
	return NULL;
}

typedef struct readLine_char_secondD {
	char *j;
	unsigned int size;
} readLine_char_secondD;

typedef struct readLine_char_firstD {
	struct readLine_char_secondD *i;
	unsigned int size;
} readLine_char_firstD;

bool readLine_isDelimiter(char c, unsigned int count_chars_in_delimiter, char *delimiter) {
	for (int i = 0; i < count_chars_in_delimiter; i++) {
		if (delimiter[i] == c) {
			return true;
		}
	}
	return false;
}

readLine_char_firstD* readLine_free(readLine_char_firstD *line) {
	if (line != NULL) {
		unsigned int firstDSize = (*line).size;
		for (unsigned int i = 0; i < firstDSize; i++) {
			(*line).i[i].j = Free((*line).i[i].j);
		}
		(*line).i = Free((*line).i);
    	line = Free(line);
	}
	return line;
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

readLine_char_firstD* readLine(FILE *fp, readLine_char_firstD *line, unsigned int count_chars_in_delimiter, char *delimiter) {
	line = readLine_free(line);
    int c;
    readLine_cdll *cdll = readLine_cdll_constructor();
    while (true) {
        c = fgetc(fp);
		if (feof(fp) && readLine_cdll_isEmpty(cdll)) {
			cdll = readLine_cdll_free(cdll, false);
			return line;
		}
        if (c == '\n' && readLine_cdll_isEmpty(cdll)) {
            line = malloc(sizeof(readLine_char_firstD));
            (*line).i = NULL;
			(*line).size = 0;
			cdll = readLine_cdll_free(cdll, false);
            return line;
        }
        if (c == '\n' || feof(fp)) {
            unsigned int num_of_nodes = (*cdll).num_of_nodes;
			readLine_cdllnode *head = (*cdll).tail->next;
			unsigned int num_of_compartments = 1;
            for (unsigned int i = 0; i < num_of_nodes; i++) {
				if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) && i == 0) {
					for (i = 1; i < num_of_nodes; i++) {
						head = readLine_cdllnode_getNext(head);
						if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false) {
							break;
						}
					}
					if (i == num_of_nodes) {
						line = malloc(sizeof(readLine_char_firstD));
            			(*line).i = NULL;
						(*line).size = 0;
						cdll = readLine_cdll_free(cdll, false);
            			return line;
					}
				}
				if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter)) {
					if (readLine_isDelimiter((*head).prev->c, count_chars_in_delimiter, delimiter) == false) {
						for (i = i + 1; i < num_of_nodes; i++) {
							head = readLine_cdllnode_getNext(head);
							if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false) {
								num_of_compartments++;
								break;
							}
						}
					}
				}
				head = readLine_cdllnode_getNext(head);
			}
			head = (*cdll).tail->next;
			line = malloc(sizeof(readLine_char_firstD));
			(*line).i = malloc(sizeof(readLine_char_secondD) * num_of_compartments);
			(*line).size = num_of_compartments;
			readLine_cdll *comp = readLine_cdll_constructor();
			unsigned int compartment = 0;
			for (unsigned int i = 0; i < num_of_nodes; i++) {
				if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) && i == 0) {
					for (i = 1; i < num_of_nodes; i++) {
						head = readLine_cdllnode_getNext(head);
						if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false) {
							break;
						}
					}
				}
				for (i = i; i < num_of_nodes; i++) {
					if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false && i == num_of_nodes - 1) {
						readLine_cdll_addToTail(comp, (*head).c);
						unsigned int compSize = (*comp).num_of_nodes;
						(*line).i[compartment].j = malloc(sizeof(char) * (compSize + 1));
						(*line).i[compartment].size = compSize + 1;
						(*line).i[compartment].j[compSize] = '\0';
						readLine_cdllnode *cmphead = (*comp).tail->next;
						for (int i = 0; i < compSize; i++) {
							(*line).i[compartment].j[i] = (*cmphead).c;
							cmphead = readLine_cdllnode_getNext(cmphead);
						}
						comp = readLine_cdll_free(comp, false);
						cdll = readLine_cdll_free(cdll, false);
						return line;
					}
					if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false) {
						readLine_cdll_addToTail(comp, (*head).c);
						head = readLine_cdllnode_getNext(head);
						continue;
					}
					unsigned int compSize = (*comp).num_of_nodes;
					(*line).i[compartment].j = malloc(sizeof(char) * (compSize + 1));
					(*line).i[compartment].size = compSize + 1;
					(*line).i[compartment].j[compSize] = '\0';
					readLine_cdllnode *cmphead = (*comp).tail->next;
					for (int i = 0; i < compSize; i++) {
						(*line).i[compartment].j[i] = (*cmphead).c;
						cmphead = readLine_cdllnode_getNext(cmphead);
					}
					compartment++;
					comp = readLine_cdll_free(comp, true);
					break;
				}
				if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter)) {
					if (readLine_isDelimiter((*head).prev->c, count_chars_in_delimiter, delimiter) == false) {
						for (i = i + 1; i < num_of_nodes; i++) {
							head = readLine_cdllnode_getNext(head);
							if (readLine_isDelimiter((*head).c, count_chars_in_delimiter, delimiter) == false) {
								i--;
								head = readLine_cdllnode_getPrev(head);
								break;
							}
						}
						if (i == num_of_nodes) {
							comp = readLine_cdll_free(comp, false);
							cdll = readLine_cdll_free(cdll, false);
							return line;
						}
					}
				}
				head = readLine_cdllnode_getNext(head);
			}
			comp = readLine_cdll_free(comp, false);
            cdll = readLine_cdll_free(cdll, false);
            return line;
        }
        readLine_cdll_addToTail(cdll, c);
    }
}


