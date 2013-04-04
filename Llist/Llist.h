#pragma once

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define TDATA_t double

typedef struct LlistNode_t {
    TDATA_t data;
    struct LlistNode_t * next;
    struct LlistNode_t * prev;
} LlistNode_t;

typedef int (*cmpFunc_t)(TDATA_t left, TDATA_t right);

/*  Prints the contents of the linked list specified
    by start.
	
	TODO: This function assumes that TDATA_t is double
*/
void Llist_print(LlistNode_t const * const start) {
    LlistNode_t const * follow = start;
    LlistNode_t const * last = NULL;
    int nodeNum = 0;
    
	if (start == NULL) {
		printf("Empty Llist.\n");
		return;
	}
    
    printf("Forward:\n");
    while(follow) {
        last = follow;
        printf("Node %d: data = %g\n", nodeNum++, follow->data);
        follow = follow->next;
    }
    
    printf("\n\nBackward:\n");
    while(last) {
        printf("Node %d: data = %g\n", --nodeNum, last->data);
        last = last->prev;
    }
}

LlistNode_t * Llist_create(TDATA_t data) {
    LlistNode_t * node = (LlistNode_t *)malloc(sizeof(LlistNode_t));
    node->data = data;
	
	node->prev = NULL;
	node->next = NULL;
    
    return node;
}

/*
	Destroys all Nodes in the linked list after node, 
		including node itself.
*/
int Llist_destroy_rest(LlistNode_t * node) {
	LlistNode_t * walk;
	
	if (!node) {
		return 1;
	}
	
   if (node-> prev) {
	   node->prev->next = NULL;
   }
   walk = node;
	
	while (node != NULL) {
		walk = node->next;
		free(node);
		node = walk;
	}
	
	return 0;
}

/*
	Destroys all the Nodes in the list before node.
	Returns 0 for success, 1 for failures.
*/
int Llist_destroy_before(LlistNode_t * const node) {	
	LlistNode_t * walk;
	LlistNode_t * prev;

	if (!node) {
		return 1;
	}

   walk = node->prev;
   node->prev = NULL;

	while (walk != NULL) {
      prev = walk->prev;
		free(walk);
  		walk = prev;
	}
	
	return 0;
}

/* 
	Destroys the entire Llist pointed to by list.
	Returns:
		0 on success,
		1 if just the rest of the list was destroyed,
		2 if just the prev part of the list was destroyed,
		3 if none of the list could be destroyed.
*/
int Llist_destroy(LlistNode_t * list) {
	return Llist_destroy_before(list)
	 + Llist_destroy_rest(list)*2;
}


LlistNode_t * Llist_find(LlistNode_t * start, TDATA_t data) {
    while(start) {
        if (start->data == data) {
            return start;
        }
        
        start = start->next;
    }
    
    return NULL;
}

/* Slow, naive insertion sort */
LlistNode_t * Llist_sort(LlistNode_t * start, cmpFunc_t compare) {
   LlistNode_t * right;
   LlistNode_t * oldNext;

    if (!start)
        return NULL;
    
    right = start->next;
    start->next = NULL;

	/* Sort first using next, then patch up prev */
    while (right) {
        oldNext = right->next;
                			
        if (compare(right->data, start->data) < 0) {
            right->next = start;
			   start = right;
        } else {
            LlistNode_t * walk = start;
            
            while (walk->next && (compare(right->data, walk->next->data) > 0)) {
                walk = walk->next;
            }
                
            right->next = walk->next;
            walk->next = right;
        }
        
        right = oldNext; 
    }
    
	/* patch prev pointers */
	start->prev = NULL;
	right = start;
	while(right->next != NULL) {
		right->next->prev = right;
		right = right->next;
	}
	
    return start;
}

LlistNode_t * Llist_merge_sort(LlistNode_t * start, cmpFunc_t compare) {
	LlistNode_t *p, *q, *e, *tail;
	int insize, nmerges, psize, qsize, i;

	/*
	 * Silly special case: if `start' was passed in as NULL, return
	 * NULL immediately.
	 */
	if (!start)
		return NULL;

	insize = 1;

	while (1) {
		p = start;
		start = NULL;
		tail = NULL;

		nmerges = 0;  /* count number of merges we do in this pass */

		while (p) {
			nmerges++;  /* there exists a merge to be done */
			/* step `insize' places along from p */
			q = p;
			psize = 0;
			for (i = 0; i < insize; i++) {
				psize++;
				q = q->next;
				if (!q) break;
			}

			/* if q hasn't fallen off end, we have two starts to merge */
			qsize = insize;

			/* now we have two starts; merge them */
			while (psize > 0 || (qsize > 0 && q)) {

				/* decide whether next LlistNode_t of merge comes from p or q */
				if (psize == 0) {
					/* p is empty; e must come from q. */
					e = q; q = q->next; qsize--;
				} else if (qsize == 0 || !q) {
					/* q is empty; e must come from p. */
					e = p; p = p->next; psize--;
				} else if (compare(p->data,q->data) <= 0) {
					/* First LlistNode_t of p is lower (or same);
					 * e must come from p. */
					e = p; 
					p = p->next; 
					--psize;
				} else {
					/* First LlistNode_t of q is lower; e must come from q. */
					e = q; 
					q = q->next; 
					--qsize;
				}

				/* add the next LlistNode_t to the merged start */
				if (tail) {
					tail->next = e;
				} else {
					start = e;
				}
				
				/* Maintain reverse pointers. */
				e->prev = tail;
				tail = e;
			}

			/* now p has stepped `insize' places along, and q has too */
			p = q;
		}

		tail->next = NULL;

		/* If we have done only one merge, we're finished. */
		if (nmerges <= 1) {  /* allow for nmerges==0, the empty start case */
			return start;
		}

		/* Otherwise repeat, merging starts twice the size */
		insize *= 2;
	}
}


/* 
	Inserts toInsert into the doubly linked list defined
		by insertAfter.  
	
	Returns:
		0 for successful insertion, 
		1 for failure.
		
	If insertAfter is NULL, Llist_insert creates a new Llist
*/
int Llist_insert(LlistNode_t * insertAfter, TDATA_t toInsert) {
   LlistNode_t * newNode;

    if (!insertAfter) {
        return 1;
	}
        
    newNode = (LlistNode_t *)malloc(sizeof(LlistNode_t));
    newNode->data = toInsert; 
 
    newNode->next = insertAfter->next;
    if (insertAfter->next) {
	     insertAfter->next->prev = newNode;
    }
    
    newNode->prev = insertAfter;
    insertAfter->next = newNode;
   
    return 0;
}

/*
	Removes the element pointed to by toRemove from
		the Llist. Returns 0 for success, 1 for failure.
*/
int Llist_remove(LlistNode_t * const toRemove) {
	if (!toRemove) {
		return 1;
	}
	
	if (toRemove->next) {
		toRemove->next->prev = toRemove->prev;
	}
	
	if (toRemove->prev) {
		toRemove->prev->next = toRemove->next;
	}
	
	free(toRemove);
	return 0;
}

/* Returns 0 if the linked list defined by start
    contains data in any node.  Returns 1 otherwise,
    including if start is null
*/
int Llist_contains(LlistNode_t * start, TDATA_t data) {
    while(start) {
        if (start->data == data)
            return 1;
        else
            start = start->next;
    }
    
    return 0;
}

