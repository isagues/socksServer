#include <stdint.h>
#include <stdbool.h> 
#include <stdio.h> 
#include "buffer.h"

#define MAX_METHOD_COUNT 255


// typedef enum {CONTINUE=0, ERROR=-1, SUCCESS=1} return_t;
typedef enum {READING=0} state_t;
typedef enum {VERSION=0, METHOD_COUNT, METHODS, DONE} message_state_t;

typedef struct {
    uint8_t version;

    uint8_t methodsCount;

    uint8_t methods[MAX_METHOD_COUNT];

    message_state_t state;
    uint8_t methods_index;
} hello_message_t;

typedef struct
{
    state_t state;

    hello_message_t message; 
} hello_t;

typedef hello_t *hello_p;


typedef struct {

    int8_t (*methodHandler)(buffer);

} auth_method_t;

auth_method_t *availableMethods;

// int8_t stablishConnection(buffer_p b, hello_p h){ // -1 error 1 succes 0 continue

//     state_t prevState;
//     do {
//         prevState = h->state;
//         switch (h->state) {
//             case READING:
//                 int8_t readState = readHello(b, &(h->message));
//                 if(readState == 1)
//                 break;
            
//             default:
//                 break;
//         }
//     } while (prevState != h->state);

//     //Read hello

//     //Check for commmont auth method

//     //Auth process

//     //Return
// }


int8_t readHello(buffer_p b, hello_message_t *m){ // -1 error 1 succes 0 continue


    if(!buffer_can_read(b))
        return 0;
    
    size_t nbytes, i = 0;
    uint8_t *read_p = buffer_read_ptr(b, &nbytes);
    while(i < nbytes || m->state != DONE ) {
        switch (m->state) {
            
            case VERSION:
                m->version = read_p[i++];
                m->state = METHOD_COUNT;

                break;

            case METHOD_COUNT:
                m->methodsCount = read_p[i++];
                if(m->methodsCount >= MAX_METHOD_COUNT){
                    buffer_read_adv(b, i);
                    return -1;
                }
                m->state = METHODS;
                break;

            case METHODS:
                m->methods[m->methods_index++] = read_p[i++];
                if(m->methods_index == m->methodsCount){
                    m->state = DONE;
                }
                else{
                    m->state = METHODS;
                }
                break;

            case DONE:                
                return 1;
        }
    }

    buffer_read_adv(b, i);
    if(m->state == DONE)
        return 1;
    return 0;
}

hello_message_t newMessage(){
    hello_message_t ans;
    ans.state = VERSION;
    ans.methods_index = 0;
    return ans;
}