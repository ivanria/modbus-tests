#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <modbus/modbus.h>

#include "test.h"

void connect_to_server(modbus_t *ctx) {
	if (modbus_connect(ctx) == -1) {
		fprintf(stderr, "Connection failed: %s\n",
				modbus_strerror(errno));
		modbus_free(ctx);
		exit(EXIT_FAILURE);
	}
}

#define PRINT_GET_WRAP(a, s, str) print_get(a, sizeof(typeof(a[0])), s, str)

void print_get(void *a, size_t memb_s, size_t s, char *func) {
	size_t i;
	fprintf(stderr, "function %s ", func);
	fprintf(stderr, "gets %zu bytes\n", s);
	s = s * memb_s;
	fprintf(stderr, "0X");
	for (i = 0; i < s; i++) {
		if ((i % memb_s == 0) && (i > 0)) 
			fprintf(stderr, " 0X");
		fprintf(stderr, "%X", *((uint8_t *)a + i));
	}
	fprintf(stderr, "\n");
}

int main(void)
{
	modbus_t *ctx;
	int rc, i;
	uint8_t *read_bits;
	uint8_t *source;
	uint16_t *read_registers;

	if ((read_registers = calloc(REGISTERS_NR, sizeof(uint16_t))) == NULL) {
		perror("calloc");
		return -1;
	}

	if ((source = calloc(SOURCE_SIZE, sizeof(uint8_t))) == NULL) {
		perror("calloc");
		return -1;
	}

	for (i = 0; i < SOURCE_SIZE; i++)
		if (i % 2 == 1)
			source[i] = ON;

	if ((read_bits = calloc(BITS_NB, sizeof(uint8_t))) == NULL) {
		perror("calloc");
		return -1;
	}

	ctx = modbus_new_tcp("127.0.0.1", 33333);
	if (ctx == NULL) {
		fprintf(stderr, "Unable to allocate libmodbus context\n");
		return -1;
	}

	if (modbus_set_debug(ctx, TRUE) == -1) {
		perror("modbus_set_debug");
	}

	/*
	if (modbus_set_slave(ctx, YOUR_DEVICE_ID) == -1) {
		fprintf(stderr, "modbus_set_slave: %s\n",
				modbus_strerror(errno));
	}
	*/

	for (i = 0; i < 2; i++) {

		connect_to_server(ctx);

		if ((rc = modbus_read_input_registers(ctx, 0,
						INPUT_REGISTERS_NR,
						read_registers)) == -1) {
			fprintf(stderr, "modbus_read_input_registers: %s\n",
					modbus_strerror(errno));
			return -1;
		}

		PRINT_GET_WRAP(read_registers, rc,
				"modbus_read__input_registers()");

		connect_to_server(ctx);
		/* REGISTERS_NR shuld be not great than modbus_mapping_new( 3 )
		 * argument
		 */
		if ((rc = modbus_read_registers(ctx, 0, REGISTERS_NR,
					read_registers)) == -1) {
			fprintf(stderr, "modbus_read_registers: %s\n",
					modbus_strerror(errno));
			return -1;
		}

		PRINT_GET_WRAP(read_registers, rc, "modbus_read_registers()");

		connect_to_server(ctx);

		for (i = 0; i < REGISTERS_NR / 2; i++)
			if (i % 3 == 0)
				*(read_registers + i) = 0xf1;

		/*REGISTERS_NR should not greater than modbus_mapping_new( 3 )
		 * argument
		 */
		if ((rc = modbus_write_registers(ctx, 0, REGISTERS_NR / 2,
						read_registers)) == -1) {
			fprintf(stderr, "modbus_write_registers: %s\n",
					modbus_strerror(errno));
			return -1;
		}
		
		connect_to_server(ctx);

		if ((rc = modbus_write_register(ctx, 0, 0xA)) == -1) {
			fprintf(stderr, "modbus_write_register: %s\n",
					modbus_strerror(errno));
			return -1;
		}

		connect_to_server(ctx);

		if ((rc = modbus_read_registers(ctx, 0, REGISTERS_NR,
					read_registers)) == -1) {
			fprintf(stderr, "modbus_read_registers: %s\n",
					modbus_strerror(errno));
			return -1;
		}

		PRINT_GET_WRAP(read_registers, rc, "modbus_read_registers()");

		connect_to_server(ctx);

		if ((rc = modbus_read_bits(ctx, 0, BITS_NB,
						read_bits)) == -1) {
			fprintf(stderr, "modbus_read_bits: %s\n",
					modbus_strerror(errno));
			modbus_free(ctx);
			return -1;
		}

		PRINT_GET_WRAP(read_bits, rc, "modbus_read_bits()");

		connect_to_server(ctx);

		if ((rc = modbus_read_input_bits(ctx, 0, INPUT_BITS_NB,
						read_bits)) == -1) {
			fprintf(stderr, "modbus_read_input_bits: %s\n",
					modbus_strerror(errno));
			return -1;
		}

		PRINT_GET_WRAP(read_bits, rc, "modbus_read_input_bits()");

		connect_to_server(ctx);

		/* BITS_NB is size of first argument of
		 * modbus_mapping_new on server side - 1
		 */
		if (modbus_write_bit(ctx, BITS_NB - 1, ON) == -1) {
			fprintf(stderr, "modbus_write_bit, %s\n",
					modbus_strerror(errno));
			return -1;
		}

		connect_to_server(ctx);

		if ((rc = modbus_write_bits(ctx, 0, SOURCE_SIZE,
						source)) == -1) {
			fprintf(stderr, "modbus_write_bits: %s\n",
					modbus_strerror(errno));
			modbus_free(ctx);
			return -1;
		}
		PRINT_GET_WRAP(source, rc, "modbus_write_bits()");
	}

	modbus_free(ctx);
	return 0;
}
