DEFAULT_FLAGS = -Werror -Wall -iquote src/ -Ofast
CFILES = $(wildcard src/*.c)
VPATH = src

all: build

make_bin_dir:
	mkdir -p bin/

build: make_bin_dir
	$(CC) $(CFLAGS) $(DEFAULT_FLAGS) $(CFILES) -o bin/tbf

# The last optimization option to be listed always takes precedence
#     meaning that -Og will take effect rather than -Ofast when this
#     rule is invoked
.PHONY: debug
debug: DEFAULT_FLAGS += -g -Og
debug: build

.PHONY: clean
clean:
	rm -r bin/

.PHONY: install
install:
	cp bin/tbf /usr/local/bin/tbf
