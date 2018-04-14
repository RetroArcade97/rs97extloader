VERSION = 0.91

all:
	@echo "Available targets"
	@echo "    zip: Create zip archive"
	@echo "For building \"plash\" executable"
	make -C plash all

zip: rs97extloader-$(VERSION).zip

rs97extloader-$(VERSION).zip:	\
		plash/plash plash/*.png plash/font.ttf \
		extloader/extloader.sh extloader/*.dge
	( \
		set -euf -o pipefail ; \
		mkdir -p rs97extloader-$(VERSION) ; \
		mkdir -p rs97extloader-$(VERSION)/{data,files} ; \
		cp -a extloader/extloader.sh rs97extloader-$(VERSION)/files ; \
		sh -c 'cp -a plash/plash plash/*.png plash/font.ttf rs97extloader-$(VERSION)/data' ; \
		sh -c 'cp -a extloader/*.dge rs97extloader-$(VERSION)' ; \
	) || (rm -rf rs97extloader-$(VERSION) ; false)
	rm -f rs97extloader-$(VERSION).zip
	zip -r rs97extloader-$(VERSION).zip rs97extloader-$(VERSION)
	rm -rf rs97extloader-$(VERSION)

plash/plash: plash/main.cpp plash/Makefile
	make -C plash plash

clean:
	make -C plash clean
	rm -rf \
		rs97extloader-*.zip
