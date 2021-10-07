#ifndef TEST_H
#define TEST_H

#ifndef TCP
#ifndef RTU
#define RTU
#endif
#endif

int EF_ALIGNMENT = 0;
int EF_PROTECT_FREE = 1;

#define PRINT_GET_WRAP(a, s, str) print_get(a, sizeof(typeof(a[0])), s, str)

#define TTY_DEV "/dev/ttyUSB0"
#define BAUD 115200
#define PARITY 'N'
#define DATA_BIT 8
#define STOP_BIT 0
#define TCP_PORT 33333
#define YOUR_DEVICE_ID 123
#define SOURCE_SIZE 10
#define BITS_NB 99
#define INPUT_BITS_NB 98
#define REGISTERS_NR 97
#define INPUT_REGISTERS_NR 96

void connect_to_server(modbus_t *ctx);
void print_get(void *a, size_t memb_s, size_t s, char *func);
void print_err(int err, char *func, modbus_t *ctx);

#endif
