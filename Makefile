
LIBS = lib/jpeg-turbo/libturbojpeg.a

OBJS += log.o \
	take_screen.o  \
	grabbers/x11_grabber.o \
	shot/o_bmp.o \
	shot/o_jpeg.o \
	net/udp.o  \
	clock.o \
	cmdutil.o 

include common.mk

LDFLAGS = -lX11

a11: take-screen

take-screen: $(OBJS)
	gcc -o $@   $^  $(LIBS)  $(LDFLAGS)


.PHONY : clean distclean 

clean:
	-rm *.[od]
	-rm shot/*.[od]
	-rm net/*.[od]
	-rm grabbers/*.[od]

distclean:
	find .  -name "*.[oadP]" | xargs -I{} rm {}

