#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus/modbus.h>

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

