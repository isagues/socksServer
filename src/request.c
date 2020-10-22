#include <stdint.h>
#include <arpa/inet.h>
#include <stdbool.h> 
#include <stdio.h> 
#include "buffer.h"

#define MAX_DOMAIN_NAME_SIZE 255
#define MAX_ADDR_SIZE 255 + 1

typedef enum {VERSION, COMMAND, RESERVED, ADDR_TYPE, ADDR, PORT, DONE} state_t; 
typedef enum {CONNECT = 1, BIND = 2, UDP = 3} command_t; 
typedef enum {IPV4 = 1, DOMAIN = 3, IPV6 = 4} addr_type; 

typedef struct {
    char addr[MAX_ADDR_SIZE];
    uint16_t size;
    uint16_t index;
} addr_t;

typedef struct {
    uint8_t version;
    command_t command;
    uint8_t reserved;
    addr_type addrType;
    addr_t addr;
    uint16_t port;

    uint8_t currentReadCount;
    state_t state;
}request_t;

typedef request_t *request_p;

int8_t processRequest(buffer_p b, request_p r){
    
    if(!buffer_can_read(b))
        return 0;
    
    size_t nbytes, i = 0;
    uint8_t *read_p = buffer_read_ptr(b, &nbytes);
    
    while(i < nbytes && r->state != DONE ) {
        switch (r->state) {
            
            case VERSION:
                r->version = read_p[i++];
                r->state = COMMAND;
                break;

            case COMMAND:
                uint8_t command = read_p[i++];
                if(command > 3)
                    return -1;
                r->command = command;
                r->state = RESERVED;
                break;

            case RESERVED:
                if(read_p[i++] != 0)
                    return -1;
                r->state = ADDR_TYPE;
                break;

            case ADDR_TYPE:
                uint8_t addrType = read_p[i++];
                if(addrType == 2 || addrType > 4)
                    return -1;
                r->addrType = addrType;
                r->state = ADDR;
                break;

                
            case DONE:                
                return 1;
        }
    }
    if(i != 0){
        buffer_read_adv(b, i);
    }
    if(m->state == DONE)
        return 1;
    return 0;
}