
#include "request.c"

void test1(){
    buffer buf;
    buffer_p b = &buf;
    uint8_t array[50];
    buffer_init(b, 50, array);
    uint8_t values[] = { /* Packet 3015 */
        0x05, 0x01, 0x00, 0x03, 0x0f, 0x66, 0x6f, 0x6f, 
        0x2e, 0x6c, 0x65, 0x61, 0x6b, 0x2e, 0x63, 0x6f, 
        0x6d, 0x2e, 0x61, 0x72, 0x00, 0x50 };

    request_t request = newRequest();
    request_p r = &request;
    int i = 0;
    do {
        buffer_write(b, values[i++]);
    }while(!processRequest(b, r));

    printRequest(r);
}

void test2(){
    buffer buf;
    buffer_p b = &buf;
    uint8_t array[50];
    buffer_init(b, 50, array);
    uint8_t values[] = { /* Packet 3779 */
        0x05, 0x01, 0x00, 0x01, 0x34, 0xda, 0xe8, 0xea, 
        0x00, 0x50 };

    request_t request = newRequest();
    request_p r = &request;
    int i = 0;
    do {
        buffer_write(b, values[i++]);
    }while(!processRequest(b, r));

    printRequest(r);
}

void test3(){
    buffer buf;
    buffer_p b = &buf;
    uint8_t array[50];
    buffer_init(b, 50, array);
    uint8_t values[] ={ /* Packet 98833 */
        0x05, 0x01, 0x00, 0x04, 0x28, 0x00, 0x03, 0xf0, 
        0x40, 0x02, 0x08, 0x0c, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x20, 0x0e, 0x00, 0x50 };

    request_t request = newRequest();
    request_p r = &request;
    int i = 0;
    do {
        buffer_write(b, values[i++]);
    }while(!processRequest(b, r));

    printRequest(r);
}

int main(int argc, char const *argv[])
{
    printf("=====TEST 1==========\n");
    test1();
    printf("=====END TEST 1======\n");
    printf("=====TEST 2==========\n");
    test2();
    printf("=====END TEST 2======\n");
    printf("=====TEST 3==========\n");
    test3();
    printf("=====END TEST 3======\n");

    return 0;
}
