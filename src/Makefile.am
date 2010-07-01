bin_PROGRAMS = griddaamfs

# -ansi


AM_CFLAGS = \
            -g -W -Wall -pedantic -Wbad-function-cast -Wcast-align \
            -Wcast-qual -Wchar-subscripts -Winline \
            -Wmissing-prototypes -Wnested-externs -Wpointer-arith \
            -Wredundant-decls -Wshadow -Wstrict-prototypes \
            -Wpointer-arith -Wno-long-long \
            -Wcomment -Wmissing-include-dirs \
            -O2 -Wuninitialized \
            -D__FreeBSD__=10 -D__DARWIN_64_BIT_INO_T=1 -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=27 \
            $(FUSE_CFLAGS) \
            -I../ \
            -Icommon \
            -Iproto



griddaamfs_SOURCES = \
        common/common.c \
		proto/curl-getinmemory.c \
        fuse-driver/griddaam_readdir.c \
		fuse-driver/griddaam_getattr.c \
		fuse-driver/griddaam_main.c
griddaamfs_LDFLAGS = 
griddaamfs_LDADD = -lcurl $(FUSE_LIBS) $(JANSSON_LIBS)

