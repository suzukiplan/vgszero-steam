all: src/gamepkg.c src/err_joypad.c
	make -f Makefile.`uname`

src/gamepkg.c: game.pkg
	cd pkg2src && make
	cd src && ../pkg2src/pkg2src ../game.pkg

src/err_joypad.c: src/err_joypad.bmp
	cd bmp2img && make
	./bmp2img/bmp2img $< > $@
