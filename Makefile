CC=g++

libkaputti.so: include/*.h include/kaputti/*.h include/kaputti/reporter/*.h src/*.cc src/reporter/*.cc
	$(CC) -o libkaputti.so -shared -fPIC -std=c++17 -Iinclude src/*.cc src/reporter/*.cc

clean:
	rm -rf libkaputti.so example

install: libkaputti.so include/kaputti.h include/kaputti
	@bash -c '[[ $$UID -eq 0 ]] || ( echo "\"make install\" has to be run as root user"; exit 1 )'
	cp libkaputti.so /usr/local/lib/
	cp include/kaputti.h /usr/local/include/
	cp -r include/kaputti /usr/local/include/
	chmod 755 /usr/local/lib/libkaputti.so
	chmod a+r-w /usr/local/include/kaputti.h
	chmod a+r-w -R /usr/local/include/kaputti
	ldconfig

uninstall:
	@bash -c '[[ $$UID -eq 0 ]] || ( echo "\"make uninstall\" has to be run as root user"; exit 1 )'
	rm -f /usr/local/lib/libkaputti.so
	rm -f /usr/local/include/kaputti.h
	rm -rf /usr/local/include/kaputti
	ldconfig

demo: example.cc libkaputti.so
	$(CC) -o example -std=c++17 -Iinclude -L. example.cc -lkaputti -lfmt
	LD_LIBRARY_PATH=/home/username/foo:. ./example

demo-post-install: example.cc
	@bash -c '[[ -r /usr/local/lib/libkaputti.so ]] || ( echo "You need to install Kaputti prior to run this target"; exit 1 )'
	$(CC) -o example -std=c++17 example.cc -lkaputti -lfmt
	./example

.PHONY: clean install uninstall demo demo-post-install
