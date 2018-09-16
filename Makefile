CFLAGS = -O0

all: bitcnt tags

bitcnt: bitcnt.c
	$(CC) $(CFLAGS) -Wall $< -o $@

tags: bitcnt.c
	ctags -R .

.PHONY:
clean:
	rm -vf bitcnt
