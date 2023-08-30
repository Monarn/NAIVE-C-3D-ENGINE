CFLAGS="-Wall -Werror"
LIBRARY="-lm -lraylib -ldl -lpthread -lglfw -lX11"
NAME="demo2"

gcc $CFLAGS $NAME.c renderer.c object.c  -o $NAME $LIBRARY
