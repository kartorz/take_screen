CC = gcc
AS = as
LD = ld

CFG_DEBUG = -g

TOPDIR = /home/joni/Projects/take_screen

INCLUDE = -I$(TOPDIR) \
		  -I$(TOPDIR)/lib/jpeg-turbo 

CFLAGS += $(INCLUDE)  -Wall $(CFG_DEBUG)

DEFINES = -DHAS_PROJECTION

%o : %S
	$(CC) $(DEFINES) $(CFLAGS) -c $< -o $@

%o : %c
	$(CC) $(DEFINES) $(CFLAGS) -c $< -o $@

$(lib_a): $(objs)
	$(AR) -r $@  $(objs)
