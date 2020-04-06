/***********************************************************
* Author: Darci Martin
* Email: martdarc@oregonstate.edu
* Date Created: 10/23/19
* Filename: linkedList.c
*
* Overview:
*   This program is a linked list implementation of the deque
*	(as worked on in Worksheet 19) and bag (as worked on in
*	Worksheet 22) ADTs.
*	The deque ADT allows for the following behavior:
*		- adding a new link to the front/back
*		- getting the value of the front/back links
*		- removing the front/back link
*	The bag ADT allows for the following behavior:
*		- adding a new link
*		- checking if a link exists with a given value
*		- removing a link  with a given value if it exists
*	Both allow for:
*		- checking if empty
*		- printing the values of all of the links
*
*	Note that both implementations utilize a linked list with
*	both a front and back sentinel and double links (links with
*	next and prev pointers).
************************************************************/
#include "linkedList.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef FORMAT_SPECIFIER
#define FORMAT_SPECIFIER "%d"
#endif

// Double link
struct Link
{
	TYPE value;
	struct Link* next;
	struct Link* prev;
};

// Double linked list with front and back sentinels
struct LinkedList
{
	struct Link* frontSentinel;
	struct Link* backSentinel;
	int size;
};

/**
  	Allocates the list's sentinel and sets the size to 0.
  	The sentinels' next and prev should point to eachother or NULL
  	as appropriate.
	param: 	list 	struct LinkedList ptr
	pre: 	         list is not null
	post: 	list front and back sentinel not null
			front sentinel next points to back
			front sentinel prev points to null
			back sentinel prev points to front
			back sentinel next points to null
			list size is 0
 */
static void init(struct LinkedList* list) {
    //list is not null
    assert(list != 0);
    //allocate front and back sentinel links
    list->frontSentinel = malloc(sizeof(struct Link));
    list->backSentinel = malloc(sizeof(struct Link));
    //front and back sentinel not null
    assert(list->frontSentinel != 0 && list->backSentinel != 0);
    //point frontSentinel next to back
    list->frontSentinel->next = list->backSentinel;
    //point frontSentinel prev to null
    list->frontSentinel->prev = 0;
    //point backSentinel prev to front
    list->backSentinel->prev = list->frontSentinel;
    //point backSentinel next to null
    list->backSentinel->next = 0;
    //set list size to zero
    list->size = 0;
}

/**
 	Adds a new link with the given value before the given link and
	increments the list's size.
 	param: 	list 	struct LinkedList ptr
 	param:	link 	struct Link ptr
 	param: 	TYPE
	pre: 	         list and link are not null
	post: 	newLink is not null
			newLink w/ given value is added before param link
			list size is incremented by 1
 */
static void addLinkBefore(struct LinkedList* list, struct Link* link, TYPE value)
{
    //list and link are not null
    assert(list != 0 && link != 0);
    //create new link to add to list
    struct Link *new = malloc(sizeof(struct Link));
    //add value to new link
    new->value = value;
    //add new link before link passed as param
    //point new link to the passed param
    new->next = link;
    //point the prev of the new link to the passed param link prev
    new->prev = link->prev;
    //break and update the passed param prev link to the new link
    link->prev = new;
    //break and update the link for the new link's prev to point as next to new link
    new->prev->next = new;
    //increment list size
    list->size++;
}

/**
	Removes the given link from the list and
	decrements the list's size.
	param: 	list 	struct LinkedList ptr
 	param:	link 	struct Link ptr
	pre: 	         list and link are not null
	post: 	param link is removed from param list
			memory allocated to link is freed
			list size is decremented by 1
 */
static void removeLink(struct LinkedList* list, struct Link* link)
{
    //list and link are not null
    assert(list != 0 && link != 0);
    //param link is removed from param list
    //point link prior to param to point to link after param
    link->prev->next = link->next;
    //point link after param to point back to link prior to param
    link->next->prev = link->prev;
    //memory allocated to link is freed
    free(link);
    link = 0;
    //list size is decremented by 1
    list->size--;
}

/**
	Allocates and initializes a list.
	pre: 	         none
	post: 	memory allocated for new struct LinkedList ptr
			list init (call to init func)
	return:       list
 */
struct LinkedList* linkedListCreate()
{
	struct LinkedList* list = malloc(sizeof(struct LinkedList));
	init(list);
	return list;
}

/**
	Deallocates every link in the list including the sentinels,
	and frees the list itself.
	param:	list 	struct LinkedList ptr
	pre: 	         list is not null
	post: 	memory allocated to each link is freed
			" " front and back sentinel " "
			" " list " "
 */
void linkedListDestroy(struct LinkedList* list)
{
	assert(list != NULL);
	while (!linkedListIsEmpty(list)) {
		linkedListRemoveFront(list);
	}
	free(list->frontSentinel);
	free(list->backSentinel);
	free(list);
	list = NULL;
}

/**Deque implementation */

/**
	Adds a new link with the given value to the front of the deque.
	param: 	deque 	struct LinkedList ptr
	param: 	value 	TYPE
	pre: 	        deque is not null
	post: 	link is created w/ param value stored before current first link
			(call to addLinkBefore)
 */
void linkedListAddFront(struct LinkedList* deque, TYPE value)
{
    //deque is not null
    assert(deque != 0);
    //call addLinkBefore passing link that frontSentinel is pointing to (front link)
    addLinkBefore(deque, deque->frontSentinel->next, value);
}

/**
	Adds a new link with the given value to the back of the deque.
	param: 	deque 	struct LinkedList ptr
	param: 	value 	TYPE
	pre: 	         deque is not null
	post: 	link is created with given value before last link (sentinel)
			(call to addLinkBefore)
 */
void linkedListAddBack(struct LinkedList* deque, TYPE value)
{
	//deque is not null
    assert(deque != 0);
    //call addLinkBefore passing backSentinel
    addLinkBefore(deque, deque->backSentinel, value);
}

/**
	Returns the value of the link at the front of the deque.
	param: 	deque 	struct LinkedList ptr
	pre:	         deque is not null
	pre:	         deque is not empty
	post:	         none
	ret:	         first link's value
 */
TYPE linkedListFront(struct LinkedList* deque)
{
	//deque is not null and not empty
    assert(deque != 0 && !linkedListIsEmpty(deque));
    //get value from first link and return
    return deque->frontSentinel->next->value;
}

/**
	Returns the value of the link at the back of the deque.
	param: 	deque 	struct LinkedList ptr
	pre:	         deque is not null
	pre:	         deque is not empty
	post:	         none
	ret:	         last link's value
 */
TYPE linkedListBack(struct LinkedList* deque)
{
    //deque is not null and not empty
    assert(deque != 0 && !linkedListIsEmpty(deque));
    //get value from last link and return
    return deque->backSentinel->prev->value;
}

/**
	Removes the link at the front of the deque.
	param: 	deque 	struct LinkedList ptr
	pre:	         deque is not null
	pre:	         deque is not empty
	post:	         first link is removed and freed (call to removeLink)
 */
void linkedListRemoveFront(struct LinkedList* deque)
{
    //deque is not null and not empty
    assert(deque != 0 && !linkedListIsEmpty(deque));
    //remove first link using removeLink passing link we need to remove
    removeLink(deque, deque->frontSentinel->next);
}

/**
	Removes the link at the back of the deque.
	param: 	deque 	struct LinkedList ptr
	pre:	         deque is not null
	pre:	         deque is not empty
	post:	         last link is removed and freed (call to removeLink)
 */
void linkedListRemoveBack(struct LinkedList* deque)
{
    //deque is not null and not empty
    assert(deque != 0 && !linkedListIsEmpty(deque));
    //remove first link using removeLink passing link we need to remove
    removeLink(deque, deque->backSentinel->prev);
}

/**
	Returns 1 if the deque is empty and 0 otherwise.
	param:	deque	struct LinkedList ptr
	pre:	         deque is not null
	post:	         none
	ret:	         1 if its size is 0 (empty), otherwise 0 (not empty)
 */
int linkedListIsEmpty(struct LinkedList* deque)
{
	//deque is not null
    assert(deque != 0);
    //check size and return
    if (deque->size == 0)
        return 1;
    else
        return 0;
}

/**
	Prints the values of the links in the deque from front to back.
	param:	deque	struct LinkedList ptr
	pre:	         deque is not null
	post:   	none
	ret:	         outputs to the console the values of the links from front
			to back; if empty, prints msg that is empty
 */
void linkedListPrint(struct LinkedList* deque)
{
	//deque is not null
    assert(deque != 0);
    //if empty print msg empty
    if (linkedListIsEmpty(deque))
        printf("Deque is Empty\n");
    //traverse and print the entire list
    else {
        //get the starting point
        struct Link *placeHolder = deque->frontSentinel->next;
        //loop until you hit the end
        while (placeHolder != deque->backSentinel) {
            printf("%d\n", placeHolder->value);
            placeHolder = placeHolder->next;
        }
    }
}

/** Bag Interface */
/**
	Adds a link with the given value to the bag.
	param:	bag		struct LinkedList ptr
	param: 	value 	TYPE
	pre: 	         bag is not null
	post: 	link is created w/ given value before current first link
			(call to addLinkBefore)
			Note that bag doesn't specify where new link should be added;
			can be anywhere in bag according to its ADT.
 */
void linkedListAdd(struct LinkedList* bag, TYPE value)
{
	//bag is not null
    assert(bag != 0);
    //add Link with value given, adding to front
    addLinkBefore(bag, bag->frontSentinel->next, value);
}

/**
	Returns 1 if a link with the value is in the bag and 0 otherwise.
	param:	bag		struct LinkedList ptr
	param: 	value 	TYPE
	pre: 	         bag is not null
	post:	         none
	ret:	         1 if link with given value found; otherwise, 0
 */
int linkedListContains(struct LinkedList* bag, TYPE value)
{
    //bag is not null
    assert(bag != 0);
    //traverse bag and return if found
    //start at the beginning
    struct Link *placeHolder = bag->frontSentinel->next;
    while (placeHolder != bag->backSentinel) {
        //return if found
        if (placeHolder->value == value)
            return 1;
        //otherwise continue traversal
        else
            placeHolder = placeHolder->next;
    }
    //was not found in the bag, return 0
    return 0;
}

/**
	Removes the first occurrence of a link with the given value.
	param:	bag		struct LinkedList ptr
	param: 	value 	TYPE
	pre: 	         bag is not null
	post:	         if link with given value found, link is removed
			(call to removeLink)
 */
void linkedListRemove(struct LinkedList* bag, TYPE value)
{
	//bag is not null
    assert(bag != 0);
    //traverse bag and remove if found
    //start at the beginning
    struct Link *placeHolder = bag->frontSentinel->next;
    while (placeHolder != bag->backSentinel) {
        //remove the link if the value is found and exit
        if (placeHolder->value == value) {
            removeLink(bag, placeHolder);
            return;
        }
        //otherwise continue traversal
        else
            placeHolder = placeHolder->next;
    }
}
