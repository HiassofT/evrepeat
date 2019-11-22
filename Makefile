PROGRAM = evrepeat
all: $(PROGRAM)

DESTDIR ?=
prefix ?= /usr
bindir ?= $(prefix)/bin

INSTALL := install

CFLAGS ?=
CFLAGS += -Wall -Wextra -std=c99 -pedantic
LDFLAGS ?=

$(PROGRAM): $(PROGRAM).c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(PROGRAM)

.PHONY: install
install:
	$(INSTALL) -m 755 -d $(DESTDIR)$(bindir)
	$(INSTALL) -m 755 $(PROGRAM) $(DESTDIR)$(bindir)
