VERSION = 0.92

help:
	@echo "Available targets"
	@echo "    zip: Create zip archive"
	@echo "For building \"plash\" executable"
	make -C plash help

zip: rs97extloader-$(VERSION).zip

ask.png: confirm.txt plash/kickstart.png
	convert -background '#0008' -fill white -gravity center -size 320x90 \
          caption:"$$(cat confirm.txt)" \
          plash/kickstart.png +swap -gravity north \
          -composite ask.png


rs97extloader-$(VERSION).zip:	\
		plash/plash plash/sdlmenu \
		plash/*.png plash/font.ttf \
		extloader/extloader.sh extloader/intboot.sh extloader/autopatch.sh \
		extloader/*.dge ask.png
	( \
		set -euf -o pipefail ; \
		mkdir -p rs97extloader-$(VERSION) ; \
		mkdir -p rs97extloader-$(VERSION)/{data,files} ; \
		cp -a extloader/extloader.sh rs97extloader-$(VERSION)/files ; \
		cp -a ask.png rs97extloader-$(VERSION)/data ; \
		sh -c 'cp -a plash/plash plash/sdlmenu plash/*.png plash/font.ttf rs97extloader-$(VERSION)/data' ; \
		sh -c 'cp -a extloader/{intboot,autopatch}.sh rs97extloader-$(VERSION)/data' ; \
		sh -c 'cp -a extloader/*.dge rs97extloader-$(VERSION)' ; \
	) || (rm -rf rs97extloader-$(VERSION) ; false)
	rm -f rs97extloader-$(VERSION).zip
	zip -r rs97extloader-$(VERSION).zip rs97extloader-$(VERSION)
	rm -rf rs97extloader-$(VERSION)

plash/sdlmenu: plash/sdlmenu.c plash/Makefile
	make -C plash sdlmenu

plash/plash: plash/main.cpp plash/Makefile
	make -C plash plash

clean:
	make -C plash clean
	rm -rf \
		rs97extloader-*.zip ask.png

