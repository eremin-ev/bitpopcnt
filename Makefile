CFLAGS = -O0

all: bitpopcnt tags

bitpopcnt: bitpopcnt.c
	$(CC) $(CFLAGS) -Wall $< -o $@

tags: bitpopcnt.c
	ctags -R .

.PHONY:
clean:
	rm -vf bitpopcnt
