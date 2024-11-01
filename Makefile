BIN := ./bin
SRC := ./src

TARGET := iscrub

CC := gcc
fCC := -Wall -Wextra -fsanitize=leak -fstack-protector-all -std=c99 -iquote$(SRC)/include
fDEBUG := -g2

CSRC := $(wildcard $(SRC)/*/*.c $(SRC)/*/*/*.c)
COBJ := $(CSRC:%.c=%.o)

debug: $(fCC) += $(fDEBUG)
debug: $(TARGET)

$(TARGET): $(COBJ)
	$(CC) $(fCC) -o $@ $^
	@mv $(COBJ) $(TARGET) $(BIN)

%.o: %.c
	$(CC) $(fCC) -o $@ -c $^

clean:
	@if [ -f "$(BIN)/$(TARGET)" ]; then \
		rm $(BIN)/*; \
	elif [ -f "$(COBJ)" ]; then \
		rm $(COBJ); \
	fi