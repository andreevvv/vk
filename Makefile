CC = gcc
CFLAGS = -O3

TASK1 = bsearch
TASK2 = strsearch

all: $(TASK1) $(TASK2)

$(TASK1): $(TASK1).c
	$(CC) $(CFLAGS) -o $(TASK1) $(TASK1).c

$(TASK2): $(TASK2).c
	$(CC) $(CFLAGS) -o $(TASK2) $(TASK2).c


clean:
	$(RM) $(TASK1)
	$(RM) $(TASK2)
