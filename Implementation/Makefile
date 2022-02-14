CC=gcc
CFLAGS= -Os -ffunction-sections -fdata-sections -fno-exceptions
DEPS = sha2_opt.h stratum.h cJSON.h parser.h memzero.h util.h
OBJ = miner.o sha2_opt.o stratum.o cJSON.o parser.o memzero.o util.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

miner: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o miner