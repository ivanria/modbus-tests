#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <modbus/modbus.h>
#include <stdbool.h>

void connect_to_server(modbus_t *ctx) {
	if (modbus_connect(ctx) == -1) {
		fprintf(stderr, "Connection failed: %s\n",
				modbus_strerror(errno));
		modbus_free(ctx);
		exit(EXIT_FAILURE);
	}
}

void print_get(void *a, size_t memb_s, size_t s, char *func) {
	size_t i;
	fprintf(stdout, "function %s ", func);
	fprintf(stdout, "gets %zu bytes\n", s);
	s = s * memb_s;
	fprintf(stdout, "0X");
	for (i = 0; i < s; i++) {
		if ((i % memb_s == 0) && (i > 0))
			fprintf(stdout, " 0X");
		fprintf(stdout, "%X", *((uint8_t *)a + i));
	}
	fprintf(stdout, "\n");
}

void print_err(int err, char *func, modbus_t *ctx)
{
	fprintf(stderr, "%s: %s\n", func, modbus_strerror(err));
	if (ctx) {
		modbus_free(ctx);
		exit(EXIT_FAILURE);
	}
}
