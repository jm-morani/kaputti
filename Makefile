CC=g++
PREFIX=/usr/local
LDCONFIG=ldconfig

USR=$(DESTDIR)$(PREFIX)

libkaputti.so: include/*.h include/kaputti/*.h include/kaputti/reporter/*.h src/*.cc src/reporter/*.cc
	$(CC) -o libkaputti.so -shared -fPIC -std=c++17 -Iinclude src/*.cc src/reporter/*.cc

clean:
	rm -rf libkaputti.so example

install: libkaputti.so include/kaputti.h include/kaputti
	@bash -c '[[ $$UID -eq 0 ]] || ( echo "\"make install\" has to be run as root user"; exit 1 )'
	mkdir -m 755 -p $(USR)/lib $(USR)/include/kaputti
	cp libkaputti.so $(USR)/lib/
	cp include/kaputti.h $(USR)/include/
	cp -r include/kaputti $(USR)/include/
	chmod 755 $(USR)/lib/libkaputti.so
	chmod a+r-w $(USR)/include/kaputti.h
	chmod a+r-w -R $(USR)/include/kaputti
	$(LDCONFIG)

uninstall:
	@bash -c '[[ $$UID -eq 0 ]] || ( echo "\"make uninstall\" has to be run as root user"; exit 1 )'
	rm -f $(USR)/lib/libkaputti.so
	rm -f $(USR)/include/kaputti.h
	rm -rf $(USR)/include/kaputti
	$(LDCONFIG)

demo: example.cc libkaputti.so
	$(CC) -o example -std=c++17 -Iinclude -L. example.cc -lkaputti -lfmt
	LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:. ./example

demo-post-install: example.cc
	@bash -c '[[ -r $(PREFIX)/lib/libkaputti.so ]] || ( echo "You need to install Kaputti prior to run this target"; exit 1 )'
	$(CC) -o example -std=c++17 example.cc -lkaputti -lfmt
	./example

.PHONY: clean install uninstall demo demo-post-install
