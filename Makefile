SOURCES		:= $(wildcard *.c)
OBJECTS		:= $(patsubst %c,%o,$(SOURCES))
DEPFILES	:= $(patsubst %.c,%.d,$(SOURCES))
#ifeq ($(IF_DEBUG),true)
CFLAGS += -Wall -Wextra -Wpedantic -g
LDFLAGS += -lefence -lmodbus
#else
#export CFLAGS := -Wall -Wextra -Wpedantic -g
#export LDFLAGS := -lmodbus
#endif

.PHONY: all
all: master slave

.PHONY: clean
clean:

master: master.o func.o
	$(CC) -o $@ $^ $(LDFLAGS) 

slave: slave.o func.o
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o: %.d

%.d: %.c
	$(CC) $(CFLAGS) -MM -MT '$(patsubst %.c,%.o,$<)' $< -MF $@
	
ifeq (0, $(words $(findstring $(MAKECMDGOALS), clean print_vars cleanall)))
-include $(DEPFILES)
endif

.PHONY: cleanall clean
clean:
	rm -f $(OBJECTS) slave  master

cleanall: clean
	rm -f $(DEPFILES)

.PHONY: print_vars
print_vars:
	@echo "===print_vars==="
	@echo "PROG		= $(PROG)"
	@echo "CFLAGS		= $(CFLAGS)"
	@echo "LDFLAGS		= $(LDFLAGS)"
	@echo "NODEP		= $(NODEP)"
	@echo "SOURCES		= $(SOURCES)"
	@echo "DEPFILES		= $(DEPFILES)"


