#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#include <modbus/modbus.h>

#include "test.h"

int main(void)
{
	modbus_t *ctx;
	uint8_t get_byte[MODBUS_TCP_MAX_ADU_LENGTH];
#ifdef TCP
	int socket;
#endif
	int rc, i;
	modbus_mapping_t *mapping;

	fprintf(stdout, "MODBUS_TCP_MAX_ADU_LENGTH = %d\n",
			MODBUS_TCP_MAX_ADU_LENGTH);

#ifdef TCP
	if ((ctx = modbus_new_tcp(NULL, TCP_PORT)) == NULL)
		print_err(errno, "modbus_new_tcp", ctx);
#elif defined(RTU)
	if ((ctx = modbus_new_rtu(TTY_DEV, BAUD, PARITY, DATA_BIT,
					STOP_BIT)) == NULL)
		print_err(errno, "modbus_new_rtu", ctx);
#endif

	if (modbus_set_debug(ctx, TRUE) == -1)
		print_err(errno, "modbus_set_debug", NULL);

	/*
	if (modbus_set_slave(ctx, YOUR_DEVICE_ID) == -1) {
		fprintf(stderr, "modbus_set_slave: %s\n",
				modbus_strerror(errno));
	}
	*/

	/* BITS_NB is bits for write and read from client */
	if (!(mapping = modbus_mapping_new(BITS_NB, INPUT_BITS_NB,
			REGISTERS_NR, INPUT_REGISTERS_NR)))
		print_err(errno, "modbus_mapping_new", ctx);

	for (i = 0; i < INPUT_REGISTERS_NR ; i++)
		if (i % 4 == 0)
			*(mapping->tab_input_registers + i) = ON;

	for (i = 0; i < INPUT_BITS_NB ; i++)
		if (i % 2 == 0)
			*(mapping->tab_input_bits + i) = ON;

#ifdef TCP
	if ((socket = modbus_tcp_listen(ctx, 100)) == -1)
		print_err(errno, "modbus_tcp_listen", ctx);
#elif defined(RTU)
	if (modbus_connect(ctx) == -1)
		print_err(errno, "modbus_connect", ctx);
#endif

	for (;;) {
	
#ifdef TCP
		if (modbus_tcp_accept(ctx, &socket) == -1)
			print_err(errno, "modbus_tcp_accept", ctx);
#endif

		if ((rc = modbus_receive(ctx, get_byte)) == -1)
			print_err(errno, "modbus_recieve", ctx);
	
		PRINT_GET_WRAP(get_byte, rc, "modbus_receive");

		if ((rc = modbus_reply(ctx, get_byte, rc, mapping)) == -1)
			print_err(errno, "modbus_reply", ctx);
	}

	modbus_free(ctx);
	return 0;
}
