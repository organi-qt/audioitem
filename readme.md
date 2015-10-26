	compile and test on pc:
		1. install dependence:
			sudo apt-get install libmpg123-dev libasound2-dev
		2. compile audioitem lib:
			cp source dir
			qmake && make distclean && qmake && make
			make install
		3. compile example
			cd example dir
			qmake && make distclean && qmake && make

	compile and test for board:
		1. install dependence:
			opkg -o $OECORE_TARGET_SYSROOT -f $OECORE_TARGET_SYSROOT/etc/opkg.conf install mpg123-dev
			opkg -o $OECORE_TARGET_SYSROOT -f $OECORE_TARGET_SYSROOT/etc/opkg.conf install alsa-lib-dev
		2. compile audioitem lib:
			cp source dir
			qmake && make distclean && qmake && make
		3. just cross-compile and cpoy to test
