SRC_FILES= $(wildcard *.c)
OBJ_FILES= $(SRC_FILES:.c=.o)

EXAMPLES = trafic-model-ex inoutdemo debits rg-its rg-draw example-1 example-2 example-3

.PHONY: clean 

all : $(EXAMPLES)

trafic-model-ex : trafic-model-ex.o  ../$(SRC_DIR)/libndes.a
	$(CC)  trafic-model-ex.o -o trafic-model-ex $(LDFLAGS)

inoutdemo : inoutdemo.o ../$(SRC_DIR)/libndes.a
	$(CC) inoutdemo.o -o inoutdemo $(LDFLAGS)

debits : debits.o ../$(SRC_DIR)/libndes.a
	$(CC) debits.o -o debits $(LDFLAGS)

rg-its : rg-its.o ../$(SRC_DIR)/libndes.a
	$(CC) rg-its.o -o rg-its $(LDFLAGS)

rg-draw : rg-draw.o ../$(SRC_DIR)/libndes.a
	$(CC) rg-draw.o -o rg-draw $(LDFLAGS)

example-1 : example-1.o ../$(SRC_DIR)/libndes.a
	$(CC) example-1.o -o example-1 $(LDFLAGS)

example-2 : example-2.o ../$(SRC_DIR)/libndes.a
	$(CC) example-2.o -o example-2 $(LDFLAGS)

example-3 : example-3.o ../$(SRC_DIR)/libndes.a
	$(CC) example-3.o -o example-3 $(LDFLAGS)

clean :
	\rm -f $(OBJ_FILES) $(EXAMPLES) *~ 
	(cd tuto-prog-1 ; make clean)
	(cd tutorial-1 ; make clean)
	(cd tutorial-2 ; make clean)

.c.o :
	$(CC) $(CFLAGS) -I../$(INCL_DIR) $< -c

