SUBDIRS = src
dist_doc_DATA = README

MAIN_FS_URI="http://asen.nikhef.nl:8000"
MOUNT_POINT=./test/
BIN="./src/griddaamfs"

mount: all
	@mkdir -p ${MOUNT_POINT}
	${BIN} ${MAIN_FS_URI} ${MOUNT_POINT}

umount:
	umount ${MOUNT_POINT}
	@rmdir ${MOUNT_POINT}

run: all

