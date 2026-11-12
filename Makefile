CC = cc
CFLAGS = -ggdb -O3 -Iinclude

.PHONY: clean

bin/libminilog.a: bin-int/minilog.o
	mkdir -p $(@D)
	ar -rcs $@ $<

bin-int/minilog.o: src/minilog.c include/minilog.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf bin-int
	rm -rf bin
