#include "myRingBuffer.h"


/** Initialize the ring buffer. 
 */
struct RingBuffer* init_RingBuffer()
{
    struct RingBuffer* rb = (struct RingBuffer *)malloc(sizeof(struct RingBuffer));
    rb -> capacity = BUFFER_SIZE;
    rb -> zero = 0;
    rb -> size = 0;
    rb -> bufdata[0] = 0;
    rb -> bufdata[BUFFER_SIZE] = 0;
    return rb;
}


/** Write to buffer. 
 */
int write_RingBuffer(struct RingBuffer *rb, const char *newdata) 
{
    int datalen = strlen(newdata),last;
    // if ring buffer too small, don't copy at all
    if(datalen > rb->capacity - rb->size - 1)
        return 0;
    // if after copying doesn't surpass highest index
    if(rb->zero + rb->size + datalen < rb->capacity){
        strncpy(rb->bufdata+rb->zero + rb->size,newdata,datalen);
        last = rb->zero + rb->size + datalen;
        rb->bufdata[last] = 0;
    // if existing data already surpassed highest index
    }else if(rb->zero + rb->size > rb->capacity){
        int firstavailable = rb->zero + rb->size % rb->capacity;
        strncpy(rb->bufdata + firstavailable,newdata,datalen);
        last = firstavailable + datalen;
        rb->bufdata[last] = 0;
    // if newdata is split(including final 0 attached to index 0)
    }else{
        int l1 = rb->capacity - rb->zero - rb->size;
        int l2 = datalen - l1;
        strncpy(rb->bufdata+rb->zero + rb->size,newdata, l1);
        strncpy(rb->bufdata,newdata+l1, l2);
        rb->bufdata[l2] = 0;
    }
    rb->size += datalen;
    return 1;
}


/** Pop from buffer.
 */
int pop_RingBuffer(struct RingBuffer *rb, char *result, int poplen)
{
	const char * pointer ;
    if(poplen <= rb->size){
        // split into two parts
        if(rb->zero + poplen > rb->capacity){
            int len1 = rb->capacity - rb->zero;
            int len2 = poplen - len1;
            strncpy(result,rb->bufdata +rb->zero,len1);
            strncpy(result+len1,rb->bufdata,len2);
            result[poplen] = 0;
        // in one piece
        }else{
            strncpy(result,rb->bufdata + rb->zero,poplen);
            result[poplen] = 0;
        }
        pointer = rb->bufdata + rb->zero;
        rb->zero = (rb->zero+poplen)%rb->capacity;
        rb->size -= poplen;
        return 1;
    }else{
        return 0;
    }
}

/** Find char in normal string.
 *  @return: distance from "zero" or -1 for not found.
 */
int find_NormalString(const char*s,char c)
{
    char *found;
    found = strchr(s, c);
    if (found)
        return found - s;
    else
        return -1; 
}


/** Find char in Ring Buffer.
 *  @return: distance from "zero" or -1 for not found.
 */
int find_RingBuffer(struct RingBuffer *rb, char c)
{
    if(rb->zero+rb->size > rb->capacity){
        int findinl1 = find_NormalString(rb->bufdata+rb->zero,c);
        if( findinl1 != -1){
            return findinl1;
        }else{
            return find_NormalString(rb->bufdata,c) + rb->capacity-rb->zero;
        }
    }else{
        return find_NormalString(rb->bufdata+rb->zero,c);
    }
}


/** Print info about Ring Buffer.
 */
void print_RingBuffer(struct RingBuffer *rb){
    printf("ZeroIndex:%d,Size:%d,Capacity:%d\n",rb->zero,
        rb->size,rb->capacity);
    if(rb->zero+rb->size > rb->capacity){
        printf("BUFFER:\"%s", rb->bufdata+rb->zero);
        printf("%s\"\n",rb->bufdata );
    }else{
        printf("BUFFER:\"%s\"\n", rb->bufdata+rb->zero);
    }
}


/** Free rb at the end.
 */
void finish_RingBuffer(struct RingBuffer *rb){
    free(rb);
}