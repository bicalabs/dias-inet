
CODE	=./../Code
LIB	=./../Libs
KSSOCK	=$(CODE)/ks.shared/kssock.h $(CODE)/ks.shared/kssock.c $(CODE)/ks.shared/ks.h

libs: $(KSSOCK)
    g++ -O2 -x c -c -o $(LIB)/ks.mod_sock.linux.x86.o kssock.c
