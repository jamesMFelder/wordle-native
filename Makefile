OBJS=main.o
LDFLAGS+=-lncurses

wordle: $(OBJS)
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) wordle $(OBJS)
