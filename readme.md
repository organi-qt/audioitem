	compile and test on pc:
		1. install dependence:
			sudo apt-get install libmpg123-dev libasound2-dev
		2. compile audioitem lib:
			cp source dir
			qmake && make distclean && qmake && make
		3. compile example
			cd example dir
			qmake && make distclean && qmake && make
	test on board:
		just cross-compile and cpoy to test
