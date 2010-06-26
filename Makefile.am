SUBDIRS = src
dist_doc_DATA = README

MAIN_FS_URI="http://asen.nikhef.nl:8000"
MOUNT_POINT=/tmp/grid

mount: all
	@mkdir -p ${MOUNT_POINT}
	-fusermount -q -u ${MOUNT_POINT}
	./bin/test -s ${MAIN_FS_URI} ${MOUNT_POINT}

umount:
	fusermount -u --lazy ${MOUNT_POINT}
	@rmdir ${MOUNT_POINT}

run: all
	@mkdir -p ${MOUNT_POINT}
	-fusermount -q -u ${MOUNT_POINT}
	./bin/test -s ${MAIN_FS_URI} ${MOUNT_POINT}
	@rmdir ${MOUNT_POINT}

