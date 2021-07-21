#################
# Demo Makefile #
#################

# Definitions

LIBS = -lm
OBJECTS = korflib.o
CFLAGS = -O2 -Wall

# Applications

APP1 = tester
OBJ1 = tester.o

APP2 = dust
OBJ2 = dust.o

# Targets

default:
	make $(APP1)
	make $(APP2)

$(APP1): $(OBJ1) $(OBJECTS)
	$(CC) -o $(APP1) $(CFLAGS) $(OBJ1) $(OBJECTS) $(LIBS)

$(APP2): $(OBJ2) $(OBJECTS)
	$(CC) -o $(APP2) $(CFLAGS) $(OBJ2) $(OBJECTS) $(LIBS)

clean:
	rm -f *.o
	rm -f $(APP1) $(APP2) $(ARC)

test: $(APP1)
	./$(APP1)

# Inference
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

