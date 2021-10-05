#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <modbus/modbus.h>

#include "test.h"

int main(void)
{
	modbus_t *ctx;
	uint8_t get_byte[MODBUS_TCP_MAX_ADU_LENGTH];
	int rc, socket, i;

	fprintf(stderr, "MODBUS_TCP_MAX_ADU_LENGTH = %d\n",
			MODBUS_TCP_MAX_ADU_LENGTH);

	ctx = modbus_new_tcp(NULL, 33333);
	if (ctx == NULL) {
		fprintf(stderr, "Unable to allocate libmodbus context\n");
		return -1;
	}

	if (modbus_set_debug(ctx, TRUE) == -1) {
		fprintf(stderr, "modbus_set_debug: %s\n",
				modbus_strerror(errno));
	}

	/*
	if (modbus_set_slave(ctx, YOUR_DEVICE_ID) == -1) {
		fprintf(stderr, "modbus_set_slave: %s\n",
				modbus_strerror(errno));
	}
	*/

	/* BITS_NB is bits for write and read from client */
	modbus_mapping_t *mapping = modbus_mapping_new(BITS_NB, INPUT_BITS_NB,
			REGISTERS_NR, INPUT_REGISTERS_NR);
	if (!mapping) {
		fprintf(stderr, "Failed to allocate the mapping: %s\n",
				modbus_strerror(errno));
		return -1;
	}
	for (i = 0; i < INPUT_REGISTERS_NR ; i++)
		if (i % 4 == 0)
			*(mapping->tab_input_registers + i) = ON;

	for (i = 0; i < INPUT_BITS_NB ; i++)
		if (i % 2 == 0)
			*(mapping->tab_input_bits + i) = ON;

	socket = modbus_tcp_listen(ctx, 100);
	if (socket == -1) {
		fprintf(stderr, "modbus_tcp_listen: %s\n",
				modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}

	for (;;) {
	
		if (modbus_tcp_accept(ctx, &socket) == -1) {
			fprintf(stderr, "modbus_tcp_accept: %s\n",
					modbus_strerror(errno));
		}

		if ((rc = modbus_receive(ctx, get_byte)) == -1) {
			fprintf(stderr, "modbus_recieve: %s\n",
					modbus_strerror(errno));
		}
	
		fprintf(stderr, "recive %d byte\n", rc);

		if ((rc = modbus_reply(ctx, get_byte, rc, mapping)) == -1) {
			fprintf(stderr, "modbus_reply: %s\n",
					modbus_strerror(errno));
		}
	}


	modbus_free(ctx);
	return 0;
}
