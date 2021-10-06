#ifndef TEST_H
#define TEST_H

int EF_ALIGNMENT = 0;
int EF_PROTECT_FREE = 1;

#define PRINT_GET_WRAP(a, s, str) print_get(a, sizeof(typeof(a[0])), s, str)

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
