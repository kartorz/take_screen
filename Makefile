
LIBS = lib/jpeg-turbo/libturbojpeg.a

OBJS += log.o \
	take_screen.o  \
	grabbers/x11_grabber.o \
	shot/o_bmp.o \
	shot/o_jpeg.o \
	net/socket.o \
	net/udp.o 

#subdirs = kernel
include common.mk

LDFLAGS = -lX11

a11: take-screen

take-screen: $(OBJS)
	gcc -o $@   $^  $(LIBS)  $(LDFLAGS)

subdir: force
	$(foreach d, $(subdirs), $(MAKE) -C $(d))

.PHONY : clean distclean 

clean:
	-rm *.[od]
	-rm shot/*.[od]
	-rm net/*.[od]
	-rm grabbers/*.[od]

distclean:
	find .  -name "*.[oadP]" | xargs -I{} rm {}

