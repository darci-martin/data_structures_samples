/***********************************************************
* Author: Darci Martin
* Email: martdarc@oregonstate.edu
* Date Created: 2019-10-25
* Filename: circularList.c
*
* Overview:
*   This program is a circular doubly linked list implementation
*	of a deque with a front sentinel.
*	It allows for the following behavior:
*		- adding a new link to the front/back
*		- getting the value of the front/back links
*		- removing the front/back link
*		- checking if the deque is empty
*		- printing the values of all the links
*		- reversing the order of the links
*
*	Note that this implementation uses double links (links with
*	next and prev pointers) and that given that it is a circular
*	linked deque the last link points to the Sentinel and the first
*	link points to the Sentinel -- instead of null.
************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "circularList.h"

#ifndef FORMAT_SPECIFIER
#define FORMAT_SPECIFIER "%g"
#endif

// Double link
struct Link
{
	TYPE value;
	struct Link * next;
	struct Link * prev;
};

struct CircularList
{
	int size;
	struct Link* sentinel;
};

/**
  	Allocates the deque's sentinel and sets the size to 0.
  	The sentinel's next and prev should point to the sentinel itself.
 	param: 	deque 	struct CircularList ptr
	pre: 	         deque is not null
	post: 	deque sentinel not null
			sentinel next points to sentinel
			sentinel prev points to sentinel
			deque size is 0
 */
static void init(struct CircularList* deque)
{
    //deque is not null
    assert(deque != 0);
    //allocate space for the sentinel
    deque->sentinel = malloc(sizeof(struct Link));
    //confirm sentinel is not null
    assert(deque->sentinel != 0);
    //sentinel next points to itself
    deque->sentinel->next = deque->sentinel;
    //sentinel prev points to itself
    deque->sentinel->prev = deque->sentinel;
    //set deque size to zero
    deque->size = 0;
}

/**
	Creates a link with the given value and NULL next and prev pointers.
	param: 	value 	TYPE
	pre: 	         none
	post: 	newLink is not null
			newLink value init to value
			newLink next and prev init to NULL
 */
static struct Link* createLink(TYPE value)
{
    //create new Link
    struct Link *newLink = malloc(sizeof(struct Link));
    //newLink is not null
    assert(newLink != 0);
    //value in newLink is set to param value
    newLink->value = value;
    //newLink next and prev init to Null
    newLink->next = newLink->prev = NULL;
    //return the Link created
    return newLink;
}

/**
	Adds a new link with the given value after the given link and
	increments the deque's size.
	param: 	deque 	struct CircularList ptr
 	param:	link 	struct Link ptr
 	param: 	TYPE
	pre: 	deque and link are not null
	post: 	newLink is not null
			newLink w/ given value is added after param link
			deque size is incremented by 1
 */
static void addLinkAfter(struct CircularList* deque, struct Link* link, TYPE value)
{
    //deque and link are not null
    assert(deque != 0 && link != 0);
    //create new link with given value
    struct Link *new = createLink(value);
    //new link is not null
    assert(new != 0);
    //insert new link after given link
    //point new link next to the same as param link
    new->next = link->next;
    //point the link after param prev to the new link
    link->next->prev = new;
    //point new link prev to param link
    new->prev = link;
    //point param link next to new link
    link->next = new;
    //increment deque size by 1
    deque->size++;
}

/**
	Removes the given link from the deque and decrements the deque's size.
	param: 	deque 	struct CircularList ptr
 	param:	link 	struct Link ptr
	pre: 	deque and link are not null
	post: 	param link is removed from param deque
			memory allocated to link is freed
			deque size is decremented by 1
 */
static void removeLink(struct CircularList* deque, struct Link* link)
{
    //deque and link are not null
    assert(deque != 0 && link != 0);
    //point link prior to param to next of param
    link->prev->next = link->next;
    //point link after param to link prior to param
    link->next->prev = link->prev;
    //free param link memory
    free(link);
    link = 0;
    //decrement deque size by 1
    deque->size--;
}

/**
	Allocates and initializes a deque.
	pre: 	none
	post: 	memory allocated for new struct CircularList ptr
			deque init (call to init func)
	return: deque
 */
struct CircularList* circularListCreate()
{
	struct CircularList* deque = malloc(sizeof(struct CircularList));
	init(deque);
	return deque;
}

/**
	Deallocates every link in the deque and frees the deque pointer.
	pre: 	deque is not null
	post: 	memory allocated to each link is freed
			" " sentinel " "
			" " deque " "
 */

void circularListDestroy(struct CircularList* deque)
{
    //deque is not null
    assert(deque != 0);
    //remove links until deque has size zero
    //move forward one while still maintaining the prior to remove
    struct Link* remove = 0;
    struct Link* position = deque->sentinel->next;
    while (deque->size != 0) {
        //set the one to remove
        remove = position;
        //move to the next one
        position = position->next;
        //delete the prior one
        removeLink(deque, remove);
    }
    //free memory allocated for sentinel
    free(deque->sentinel);
    //free memory allocated for deque
    free(deque);
    deque = 0;
}

/**
	Adds a new link with the given value to the front of the deque.
	param:	deque 	struct CircularList ptr
	param: 	value 	TYPE
	pre: 	deque is not null
	post: 	link is created w/ given value before current first link
			(call to addLinkAfter)
 */
void circularListAddFront(struct CircularList* deque, TYPE value)
{
    //deque is not null
    assert(deque != 0);
    //call addLinkAfter and pass current first link and value param
    addLinkAfter(deque, deque->sentinel, value);
}

/**
	Adds a new link with the given value to the back of the deque.
	param: 	deque 	struct CircularList ptr
	param: 	value 	TYPE
	pre: 	deque is not null
	post: 	link is created w/ given value after the current last link
			(call to addLinkAfter)
 */
void circularListAddBack(struct CircularList* deque, TYPE value)
{
    //deque is not null
    assert(deque != 0);
    //add new link to the back of the deque
    addLinkAfter(deque, deque->sentinel->prev, value);
}

/**
	Returns the value of the link at the front of the deque.
	param: 	deque 	struct CircularList ptr
	pre:	deque is not null
	pre:	deque is not empty
	post:	none
	ret:	first link's value
 */
TYPE circularListFront(struct CircularList* deque)
{
    //deque is not null and deque is not empty
    assert(deque != 0 && deque->size > 0);
	//return value from first link
    return deque->sentinel->next->value;
}

/**
  	Returns the value of the link at the back of the deque.
	param: 	deque 	struct CircularList ptr
	pre:	deque is not null
	pre:	deque is not empty
	post:	none
	ret:	last link's value
 */
TYPE circularListBack(struct CircularList* deque)
{
    //deque is not null and deque is not empty
    assert(deque != 0 && deque->size > 0);
    //return value from first link
    return deque->sentinel->prev->value;
}

/**
	Removes the link at the front of the deque.
	param: 	deque 	struct CircularList ptr
	pre:	deque is not null
	pre:	deque is not empty
	post:	first link is removed and freed (call to removeLink)
 */
void circularListRemoveFront(struct CircularList* deque)
{
    //deque is not null and deque is not empty
    assert(deque != 0 && deque->size > 0);
    //remove first link in the deque
    removeLink(deque, deque->sentinel->next);
}

/**
	Removes the link at the back of the deque.
	param: 	deque 	struct CircularList ptr
	pre:	deque is not null
	pre:	deque is not empty
	post:	last link is removed and freed (call to removeLink)
 */
void circularListRemoveBack(struct CircularList* deque)
{
    //deque is not null and deque is not empty
    assert(deque != 0 && deque->size > 0);
    //remove last link in the deque
    removeLink(deque, deque->sentinel->prev);
}

/**
	Returns 1 if the deque is empty and 0 otherwise.
	param:	deque	struct CircularList ptr
	pre:	deque is not null
	post:	none
	ret:	1 if its size is 0 (empty), otherwise 0 (not empty)
 */
int circularListIsEmpty(struct CircularList* deque)
{
	//deque is not null
    assert(deque != 0);
    //check size of deque
    if (deque->size == 0)
        return 1;
    else
        return 0;
}

/**
	Prints the values of the links in the deque from front to back.
	param:	deque	struct CircularList ptr
	pre:	deque is not null
	post:	none
	ret:	outputs to the console the values of the links from front
			to back; if empty, prints msg that is empty
 */
void circularListPrint(struct CircularList* deque)
{
	//deque is not null
    assert(deque != 0);
    //if empty print message that it is empty
    if (circularListIsEmpty(deque))
        printf("Deque is empty\n");
    //if not empty then traverse and print the value in each
    else {
        struct Link *holder = deque->sentinel->next;
        while(holder != deque->sentinel) {
            printf("%g\n", holder->value);
            holder = holder->next;
        }
    }
}

/**
	Reverses the deque in place without allocating any new memory.
	The process works as follows: current starts pointing to sentinel;
	tmp points to current's next, current's next points to current's prev,
	current's prev is assigned to tmp and current points to current's next
	(which points to current's prev), so you proceed stepping back through
	the deque, assigning current's next to current's prev, until current
	points to the sentinel then you know the each link has been looked at
	and the link order reversed.
	param: 	deque 	struct CircularList ptr
	pre:	deque is not null
	pre:	deque is not empty
	post:	order of deque links is reversed
 */
void circularListReverse(struct CircularList* deque)
{
    //deque is not null and deque is not empty
    assert(deque != 0 && !circularListIsEmpty(deque));
	//current starts pointing to sentinel
    struct Link* current = deque->sentinel;
    //temp points to current's next
    struct Link* temp = current->next;
    do {
        //current's next points to current's prev
        current->next = current->prev;
        //current's prev is assigned to temp
        current->prev = temp;
        //current points to current's next
        current = current->next;
        //move temp forward
        temp = current->next;
    } while (current != deque->sentinel);
}
