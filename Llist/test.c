#include <math.h>
#include <time.h>
#include "Llist.h"

#ifdef _MSC_VER
#define inline 
#endif


static const int DefaultListSize = 100;

static inline int doubleCompare(double left, double right) {
    if (left == right) {
       return 0;
    } else if (left < right) {
      return -1;
    } else {
      return 1;
    }
}

static inline double nextValue() {
	return 100*( rand() / (double)RAND_MAX);
}

int sort_test(LlistNode_t ** list) {
      
   printf("Unsorted:\n");
   Llist_print(*list);
	
   *list = Llist_sort(*list, &doubleCompare);

   printf("******************************\n\n");
	
   printf("Sorted:\n");
   Llist_print(*list);

   return 0;
}


int main(int argc, char * argv[]) {
   LlistNode_t * head;
   LlistNode_t * n;
   int i;
   int listSize = DefaultListSize;

   head = NULL;

   srand((unsigned int) time(0));
	
      if (argc >= 2) {
         listSize = atoi(argv[1]);
      }

   if  (listSize > 0) {		
	   LlistNode_t * current;
		
	   head = Llist_create(nextValue());
	   current = head;
		
	   for (i = 1; i < listSize; ++i) {
		   Llist_insert(current, nextValue()); 
		   current = current->next;
	   }
   } else {
	   head = NULL;
   }
   
   sort_test(&head);

   n = head->next;

   if (n) {
      Llist_insert(n, 1231123);
   }

   head = Llist_sort(head, &doubleCompare);

   if (Llist_contains(head, 1231123)) {
      printf("Value added successfully.\n");
   }

   Llist_print(head);

   Llist_destroy_rest(n);

   Llist_print(head);
	
   Llist_destroy(head);
    
   return 0;
}
