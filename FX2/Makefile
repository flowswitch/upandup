PROJECT=gpio256
INCLUDE=-I/home/devel/electro/USB/FX2/fx2lib/include

BUILD_DIR = ./build

SOURCES = main.c
SOURCES += init.c
SOURCES += gpif.c
SOURCES += cpld.c
SOURCES += fpga.c

OBJS=$(addprefix $(BUILD_DIR)/, $(addsuffix .rel, $(notdir $(basename $(SOURCES)))))


all: | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

all: $(BUILD_DIR)/$(PROJECT).ihx

$(BUILD_DIR)/$(PROJECT).ihx: $(OBJS)
	sdcc -mmcs51 -o $@ $^ --code-size 0x3200 --xram-size 0x0E00 --xram-loc 0x3200

%.rel: ../%.c
	sdcc -mmcs51 $(INCLUDE) -c -o $@ $<

clean:
	rm build/*
