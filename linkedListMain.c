#include "linkedList.h"
#include <stdio.h>

int main(){
	struct LinkedList* l = linkedListCreate(); 
	linkedListAddFront(l, (TYPE)1); //1
	linkedListAddBack(l, (TYPE)2); //1,2
	linkedListAddBack(l, (TYPE)3); //1,2,3
	linkedListAddFront(l, (TYPE)4); //4,1,2,3
	linkedListAddFront(l, (TYPE)5); //5,4,1,2,3
	linkedListAddBack(l, (TYPE)6); //5,4,1,2,3,6
	linkedListPrint(l);
	printf("%i\n", linkedListFront(l)); //5
	printf("%i\n", linkedListBack(l)); //6
	linkedListRemoveFront(l); //4,1,2,3,6
	linkedListRemoveBack(l); //4,1,2,3
	linkedListPrint(l);
        linkedListDestroy(l);
/* BAG */
	
      struct LinkedList* k = linkedListCreate(); 
       linkedListAdd (k, (TYPE)10); //10
       linkedListAdd (k, (TYPE)11); //11,10
        linkedListAdd (k, (TYPE)13); //13,11,10
       linkedListAdd(k, (TYPE)14); //14,13,11,10
    linkedListAdd(k, (TYPE)11); //11,14,13,11,10
       linkedListRemove(k, (TYPE)11); //14,13,11,10
    printf("%d\n", linkedListContains(k, (TYPE)10)); //1
    linkedListPrint(k);
        linkedListDestroy(k);
	return 0;
}

