// FIFO.c
// Runs on any LM3Sxxx
// Provide functions that initialize a FIFO, put data in, get data out,
// and return the current size.  The file includes a transmit FIFO
// using index implementation and a receive FIFO using pointer
// implementation.  Other index or pointer implementation FIFOs can be
// created using the macros supplied at the end of the file.
// Daniel Valvano
// June 16, 2011

/* This example accompanies the book
     "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
     ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
            Programs 3.7, 3.8., 3.9 and 3.10 in Section 3.7

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
        You may use, edit, run or distribute this file
        as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include "fifo.h"

#define TYPE char
#define SIZE 3000
#define FAIL 0
#define SUCCESS 1

long StartCritical(void);
void EndCritical(long);

char volatile *RxPutPt;
char volatile *RxGetPt; 

char static RxFifo[SIZE];   

void RxFifo_Flush(void){
  RxPutPt = RxGetPt = &RxFifo[0];
}

void RxFifo_Init(void){
    long sr;
    sr = StartCritical();                
    RxPutPt = RxGetPt = &RxFifo[0];
    EndCritical(sr);
}

int RxFifo_Put (char data){
    char volatile *nextPutPt;
    nextPutPt = RxPutPt + 1;

    if(nextPutPt == &RxFifo[SIZE]){
        nextPutPt = &RxFifo[0];
    }

    if(nextPutPt == RxGetPt ){
        return(FAIL);
    }
    else{
        *( RxPutPt ) = data;
        RxPutPt = nextPutPt;
        return(SUCCESS);
    }
}

int RxFifo_Get (char *datapt){
    if( RxPutPt == RxGetPt ){
        return(FAIL);
    }
    *datapt = *(RxGetPt++);
    if( RxGetPt == &RxFifo[SIZE]){
        RxGetPt = &RxFifo[0];
    }
    return(SUCCESS);
}

int RxFifo_Peek (char *datapt){
    if( RxPutPt == RxGetPt ){
        return(FAIL);
    }
    *datapt = *RxGetPt;
    return(SUCCESS);
}

unsigned short RxFifo_Size (void){
    if( RxPutPt < RxGetPt ){
        return ((unsigned short)( RxPutPt - RxGetPt + (SIZE*sizeof(char)))/sizeof(char));
    }
    return ((unsigned short)( RxPutPt - RxGetPt )/sizeof(char));
}
