OBJS:=main.o mplayerudp.o csv.o vorze.o js.o
TARGET:=vorzemplayer

LIBLUA_INCDIR=/usr/include/lua5.2

all : $(TARGET) mpv_bindings.so

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

mpv_bindings.so : 
	$(CC) -Wall -shared -fPIC -o mpv_bindings.so csv.c vorze.c -I$(LIBLUA_INCDIR) -llua5.2 mpv_bindings.c

clean:
	rm -f $(OBJS)
	rm -f mpv_bindings.so
