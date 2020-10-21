
#include "hello.c"

int main(int argc, char const *argv[])
{
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

    hello_message_t m = newMessage();
    while(!readHello(b, &m));

    printf("Version: %x. Method count: %x\n", m.version, m.methodsCount);
    for (size_t i = 0; i < 3; i++){
        printf("Method %ld: %x\n", i, m.methods[i]);
    }
    
    return 0;
}