bin_PROGRAMS = griddaamfs

AM_CFLAGS = \
            -g -W -Wall -ansi -pedantic -Wbad-function-cast -Wcast-align \
            -Wcast-qual -Wchar-subscripts -Winline \
            -Wmissing-prototypes -Wnested-externs -Wpointer-arith \
            -Wredundant-decls -Wshadow -Wstrict-prototypes \
            -Wpointer-arith -Wno-long-long \
            -Wcomment -Wmissing-include-dirs \
            -O2 -Wuninitialized \
            -D__FreeBSD__=10 -D__DARWIN_64_BIT_INO_T=1 -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=27 \
            $(CURL_CFLAGS) $(FUSE_CFLAGS) $(JANSSON_CFLAGS)


griddaamfs_SOURCES = \
		fuse-driver/helloworld.c
griddaamfs_LDFLAGS = 
griddaamfs_LDADD = $(CURL_LIBS) $(FUSE_LIBS) $(JANSSON_LIBS)

