	compile and test example for board:
		1. install dependence:
			opkg -o $OECORE_TARGET_SYSROOT -f $OECORE_TARGET_SYSROOT/etc/opkg.conf update
			opkg -o $OECORE_TARGET_SYSROOT -f $OECORE_TARGET_SYSROOT/etc/opkg.conf install mpg123-dev
			opkg -o $OECORE_TARGET_SYSROOT -f $OECORE_TARGET_SYSROOT/etc/opkg.conf install alsa-lib-dev
			opkg -o $OECORE_TARGET_SYSROOT -f $OECORE_TARGET_SYSROOT/etc/opkg.conf install libaudioitem-dev
		2. compile audioitem lib:
			cp source dir
			qmake && make distclean && qmake && make
		3. just cross-compile and cpoy to test
