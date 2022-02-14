TARGET = Test
OBJS = main.o sha2.o bitcoin_miner.o bitcoin_miner_opt.o sha2_opt.o
CFLAGS = -O0 -G0 -Wall -g 
CXXFLAGS = $(CFLAGS) -std=c++14 -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBS = -lpsputility -lpsphttp -lpspssl -lpspgum -lpspgu -lm -lpsprtc

#PSP 
BUILD_PRX = 1
PSP_FW_VERSION = 500
PSP_LARGE_MEMORY = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = MINING_TEST

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak