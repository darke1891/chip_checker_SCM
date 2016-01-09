#ifndef __MY_RING_BUFFER_H__
#define __MY_RING_BUFFER_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** Program specific. If class is available would change to OOP
 *  malloc on constructor and free on destructor. Ring buffer is
 *  used to buffer communications.
 *  @params zero: current 0 index.
 *  @params capacity: total buffer size.(should not be able to change
 *  after constructor)
 *  @params size:  current size of data written into buffer.  
 *  @params data: could hold capacity-1 characters. 
 *  data[BUFFER_SIZE] and last character stored in data are both 
 *  set to 0 to comply with C standard string functions.                         
 */
#define BUFFER_SIZE 100
struct RingBuffer 
{
    int zero;
    int size;
    int capacity;
    char bufdata[BUFFER_SIZE+10]; 
};

/** Initialize the ring buffer. */
struct RingBuffer* init_RingBuffer();
/** Write to buffer.  */
int write_RingBuffer(struct RingBuffer *rb, const char *newdata);
/** Pop from buffer. */
int pop_RingBuffer(struct RingBuffer *rb, char *result, int poplen);
/** Find char in normal string.
 *  @return: distance from "zero" or -1 for not found.
 */
int find_NormalString(const char*s,char c);
/** Find char in Ring Buffer.
 *  @return: distance from "zero" or -1 for not found.
 */
int find_RingBuffer(struct RingBuffer *rb, char c);
/** Print info about Ring Buffer.
 */
void print_RingBuffer(struct RingBuffer *rb);
/** Free rb at the end.
 */
void finish_RingBuffer(struct RingBuffer *rb);


#endif