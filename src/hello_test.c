
#include "hello.c"

void test1(){
    buffer buf;
    buffer_p b = &buf;
    uint8_t array[50];
    buffer_init(b, 50, array);
    size_t nbytes;
    uint8_t *write_p = buffer_write_ptr(b, &nbytes);
    write_p[0] = 0x05;
    write_p[1] = 0x03;
    write_p[2] = 0x01;
    write_p[3] = 0x02;
    write_p[4] = 0x03;
    buffer_write_adv(b, 5);

    hello_t hello = newHello();
    hello_p h = &hello;
    while(!establishConnection(b, h));

    printMessage(&(h->message));
}

void test2(){
    buffer buf;
    buffer_p b = &buf;
    uint8_t array[50];
    buffer_init(b, 50, array);
    uint8_t values[5];
    values[0] = 0x05;
    values[1] = 0x02;
    values[2] = 0x01;
    values[3] = 0x02;
    values[4] = 0x03;

    hello_t hello = newHello();
    hello_p h = &hello;
    int i = 0;
    do {
        buffer_write(b, values[i++]);
    }while(!establishConnection(b, h));

    printMessage(&(h->message));
}

int main(int argc, char const *argv[])
{
    printf("=====TEST 1==========\n");
    test1();
    printf("=====END TEST 1======\n");

    printf("=====TEST 2==========\n");
    test2();
    printf("=====END TEST 2======\n");

    return 0;
}
