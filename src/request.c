#include <stdint.h>
#include <arpa/inet.h>
#include <stdbool.h> 
#include <string.h> 
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

static int8_t parsePort(buffer_p b, request_p r);
static int8_t parseAddr(buffer_p b, request_p r);
static void nextState(request_p r);

int8_t processRequest(buffer_p b, request_p r){
    
    // if(!buffer_can_read(b))
    //     return 0;
    
    // size_t nbytes, i = 0;
    // uint8_t *read_p = buffer_read_ptr(b, &nbytes);
    uint8_t value;
    int8_t ans;
    
    while(buffer_can_read(b) && r->state != DONE ) {
        switch (r->state) {
            
            case VERSION:
                r->version = buffer_read(b);
                nextState(r);
                break;

            case COMMAND:
                value = buffer_read(b);
                if(value > 3)
                    return -1;
                r->command = value;
                nextState(r);
                break;

            case RESERVED:
                if(buffer_read(b) != 0)
                    return -1;
                nextState(r);
                break;

            case ADDR_TYPE:
                value = buffer_read(b);
                if(value == 2 || value > 4)
                    return -1;
                r->addrType = value;
                nextState(r);
                break;

            case ADDR:
                ans = parseAddr(b, r);
                if(ans == -1)
                    return -1;
                if(ans == 1)
                    nextState(r);
                break;

            case PORT:
                ans = parsePort(b, r);
                if(ans == -1)
                    return -1;
                if(ans == 1)
                    nextState(r);
                break;

            case DONE:
                return 1;
        }
    }
    // if(i != 0){
    //     buffer_read_adv(b, i);
    // }
    if(r->state == DONE)
        return 1;
    return 0;
}

static void nextState(request_p r){

    switch (r->state) {
    case VERSION:
        r->state = COMMAND;
        break;
    
    case COMMAND:
        r->state = RESERVED;
        break;
    
    case RESERVED:
        r->state = ADDR_TYPE;
        break;
    
    case ADDR_TYPE:
        r->state = ADDR;
        break;
    
    case ADDR:
        r->state = PORT;
        break;
    
    case PORT:
        r->state = DONE;
        break;

    default:
        break;
    }

    r->currentReadCount = 0;
}

static int8_t parseAddr(buffer_p b, request_p r) {

    if(r->addr.size == 0){
        switch (r->addrType) {
            case IPV4:
                r->addr.size = 4 * 3 + 3;
                break;
            
            case IPV6:
                r->addr.size = 16 * 2 + 7;
                break;

            case DOMAIN:
                if(buffer_can_read(b))
                    r->addr.size = buffer_read(b);
                
                break;

            default:
                break;
        }
    }
    char buffer[10];
    while (buffer_can_read(b) && r->addr.index < r->addr.size) {
        uint8_t value = buffer_read(b);
        r->currentReadCount++;

        if(r->addrType == IPV4){
            sprintf(buffer, "%03d", value);
            memcpy(r->addr.addr + r->addr.index, buffer, 3);
            r->addr.index += 3;
            
        } else if(r->addrType == IPV6){
            sprintf(buffer, "%02x", value);
            memcpy(r->addr.addr + r->addr.index, buffer, 2);
            r->addr.index += 2;
        }
        else
        {
            r->addr.addr[r->addr.index++] = value;
        }
        if(r->addrType == IPV4 && r->addr.index < r->addr.size){
            r->addr.addr[r->addr.index++] = '.';
        }
        if(r->addrType == IPV6 && r->currentReadCount % 2 == 0 && r->addr.index < r->addr.size){
            r->addr.addr[r->addr.index++] = ':';
        }
    }

    if(r->addr.index == r->addr.size){
        r->addr.addr[r->addr.index++] = '\0';
        return 1;
    }

    return 0;
}

static int8_t parsePort(buffer_p b, request_p r) {

    while (buffer_can_read(b) && r->currentReadCount < 2) {
        r->port = (r->port << 8) + buffer_read(b);
        r->currentReadCount++;
    }
    
    if(r->currentReadCount == 2)
        return 1;
    
    return 0;
}

request_t newRequest(){

    request_t ans;
    addr_t addr;
    addr.index = addr.size = 0;
    ans.state = VERSION;
    ans.addr = addr;
    return ans;    
}

void printRequest(request_p r) {

    printf("Version: %x. Command: %x. Addrtype: %d. Addr size: %d.\n", r->version, r->command, r->addrType, r->addr.size);
    printf("Addr: %s. Port: %d\n", r->addr.addr, r->port);
}

