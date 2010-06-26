bin_PROGRAMS = griddaamfs

AM_CFLAGS = \
            -g -W -Wall -ansi -pedantic -Wbad-function-cast -Wcast-align \
            -Wcast-qual -Wchar-subscripts -Winline \
            -Wmissing-prototypes -Wnested-externs -Wpointer-arith \
            -Wredundant-decls -Wshadow -Wstrict-prototypes \
            -Wpointer-arith -Wno-long-long \
            -Wcomment -Wmissing-include-dirs \
            -O2 -Wuninitialized \
            $(FUSE_CFLAGS) $(JANSSON_CFLAGS)


griddaamfs_SOURCES = \
		fuse-driver/helloworld.c
griddaamfs_LDFLAGS = 
griddaamfs_LDADD = $(FUSE_LIBS) $(JANSSON_LIBS)

