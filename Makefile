
LIBS = lib/jpeg-turbo/libturbojpeg.a

OBJS += log.o \
	take_screen.o  \
	grabbers/x11_grabber.o \
	shot/o_bmp.o \
	shot/o_jpeg.o 

#subdirs = kernel
include common.mk

LDFLAGS = -lX11

a11: take-screen

take-screen: $(OBJS)
	gcc -o $@   $^  $(LIBS)  $(LDFLAGS)

subdir: force
	$(foreach d, $(subdirs), $(MAKE) -C $(d))

.PHONY : clean

clean:
	@find . -name "*.[oadP]" | xargs -I{} rm {}

