bin_PROGRAMS = griddaamfs griddaamfsd

# -ansi


#   AM_CFLAGS = \
#               -g -W -Wall -pedantic -Wbad-function-cast -Wcast-align \
#               -Wcast-qual -Wchar-subscripts -Winline \
#               -Wmissing-prototypes -Wnested-externs -Wpointer-arith \
#               -Wredundant-decls -Wshadow -Wstrict-prototypes \
#               -Wpointer-arith -Wno-long-long \
#               -Wcomment -Wmissing-include-dirs \
#               -O3 -Wuninitialized \
#               -D__FreeBSD__=10 -D__DARWIN_64_BIT_INO_T=1 -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=27 \
#               $(FUSE_CFLAGS) \
#               -I../ \
#               -Icommon \
#               -Iproto

griddaamfs_CFLAGS = \
            -g -W -Wall \
            -Wmissing-prototypes \
            -Wredundant-decls -Wshadow \
            -O3 -Wuninitialized \
            -D__FreeBSD__=10 -D__DARWIN_64_BIT_INO_T=1 -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=27 \
            $(FUSE_CFLAGS) \
            -I../ \
            -Ilogging \
            -Icommon \
            -Iproto


griddaamfs_SOURCES = \
        common/common.c \
        logging/mylog.c \
        proto/cache.c \
        proto/vlet.c \
		proto/curl-getinmemory.c \
        fuse-driver/griddaam_mkdir.c \
        fuse-driver/griddaam_open.c \
        fuse-driver/griddaam_read.c \
        fuse-driver/griddaam_create.c \
        fuse-driver/griddaam_readdir.c \
		fuse-driver/griddaam_getattr.c \
		fuse-driver/griddaam_main.c
griddaamfs_LDFLAGS = 
griddaamfs_LDADD = -lcurl $(FUSE_LIBS) $(JANSSON_LIBS)


griddaamfsd_CFLAGS = \
            -g -W -Wall \
            -Wmissing-prototypes \
            -Wredundant-decls -Wshadow \
            -O3 -Wuninitialized \
            -I../ \
            -Ilogging \
            -Icommon \
            -Iproto \
            -Inetwork

griddaamfsd_SOURCES = \
        logging/scar_log.c \
        common/common.c \
        network/net_threader.c \
        network/net_common.c \
        network/net_server.c \
        network/net_buffer.c \
        network/unsigned_string.c \
        proto/vlet.c \
        miniFTS/fts_main.c

griddaamfsd_LDFLAGS = 
griddaamfsd_LDADD = 
