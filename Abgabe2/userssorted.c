#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHARS 120 // increased size
#define DELIM ":"

typedef struct user {
	char *uname;
	int uid;
	struct user *next;
} User;

int foundUserCount = 0;
void insertElement(User **head, User *u)
{
	//Note: Head is used to know the first element of the list.
	//When inserting a new element, there are three cases:
	//1. new element is head (if list was empty)
	//2. new element is inserted between two existing elements
	//3. new element is inserted at the end of the list 

	if (*head == NULL) { //list is empty -> current element becomes head
		*head = u;
	} else { 

		User *prev = NULL; //pointer to previous element
		User *curr = *head; //pointer to current element

		while (curr != NULL) {

			// advance through the list until the next element is "bigger" than the one to insert
			// use strcmp(s1, s2) to compare. returns <0 if s1 is less, 0 if equal, >0 if bigger
			// checks whether current is already bigger, so we insert between priv and curr
			if (strcmp(curr->uname, u->uname) > 0) {
				// insert element before curr
				u->next = curr;
				
				// if there is no previous element, it will be inserted at first position 
				if (prev == NULL) {
					*head = u;
				} else { // otherwise, just add it after prev
					prev->next = u;
				}

				return;
			}

			// check next element as the right place hasn't been reached yet
			prev = curr;
			curr = curr->next;
		}

		// no "bigger" element found inside the list, so add it at the end
		prev->next = u;
	}
}

void printList(User *head) {
	User *curr = head;
	int count=0;
	while (curr != NULL) {
		count++;
		printf("\tUser %s with id %d\n", curr->uname, curr->uid);
		curr = curr->next;
	}
	printf("Found %d users.\n", count);
	printf("Actual users from passwd: %d\n", foundUserCount);
}

void freeList(User *head) {
	User *curr = head, *prev = NULL;
	while (curr != NULL) {
		free(curr->uname); //free string data
		prev = curr;
		curr = curr->next;
		free(prev); //free previous element
	}
}

int main(void)
{
	char tmp[MAXCHARS];
	User *head = NULL;

	//open users file
	FILE *fp = fopen("/etc/passwd", "r");
	while (!feof(fp) && fgets(tmp, sizeof(tmp), fp)) {

		User *u = NULL;

		u = malloc(sizeof(User));


		// as in the old exercise, only get the relevant info with strtok
		// and use sscanf to parse the int from the UID column	
		u->uname = malloc(MAXCHARS + 1); // have to malloc first, else it overwrites same string
		strcpy(u->uname, strtok(tmp, DELIM));		
		strtok(NULL, DELIM);
		sscanf(strtok(NULL, DELIM), "%d", &u->uid);

		u->next = NULL; //set next pointer to NULL (may be changed in insertElement())

		printf("Read user with name=%s and id=%d\n", u->uname, u->uid);
		foundUserCount++;
		insertElement(&head, u);
	}
	fclose(fp);

	printList(head);
	freeList(head); 

	return 0;
}


