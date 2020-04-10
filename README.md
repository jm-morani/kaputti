# Kaputti

Kaputti is a small C++ test framework inspired from Mocha and Chai.


## Example

Here is the Bag class to be tested:

```c++
class Bag {
  private:
    forward_list<string> items;
  public:
    void append(string item) {
      items.push_front(item);
    }
    bool contains(string searched) {
      for(auto item : items)
        if (item == searched)
          return true;
      return false;
    }
};
```

You could describe its behavior as following:

```c++
describe("Bag", {
  it("should contain the appended item", {
    Bag bag;
    bag.append("foo");
    expect(bag.contains("foo"))
      ->to.equal(true);
  });

  it("should not contain a never appended item", {
    Bag bag;
    bag.append("foo");
    expect(bag.contains("bar"))
      ->to.equal(false)
      ->to.never.equal(true);  // chaining
  });

  it("should be case insensitive", {
    Bag bag;
    bag.append("foo");
    expect(bag.contains("FOO"))
      ->to.equal(true);  // will fail
  });
});
```

With Kaputti already installed, compile and run this example like that:

```
$ $(CC) -o example -std=c++17 example.cc -lkaputti -lfmt
$ ./example
Bag
  should contain the appended item             [ PASS ]
  should not contain a never appended item     [ PASS ]
  should be case insensitive                   [ FAIL ]
    Expression `bag.contains("FOO")`
    expected to be equal to true but was false
    in file example.cc at line 43
```


To get a junit formatted report for CI toolchain:
```
$ ./example --report-junit build-12345.xml
```


## Installation

### Prerequisite

Library [fmt](https://github.com/fmtlib/fmt) installation is a prerequisite.

With a Debian derivatives, `apt-get install libfmt-dev` may do the job.

### Manual installation

There is no configure script. Default path *prefix*  is `/usr/local`.

Download, build, then install as root:

```
$ git clone https://github.com/jm-morani/kaputti
$ cd kaputti
$ make
$ sudo make install
```

Compile and run the example to check the installation:

```
$ make demo-post-install
$ make clean
```

### Uninstall

An *uninstall* target does a clean removal to prevent conflict with any future packaged release.

```
$ sudo make uninstall
```

### Packaging

Here is how to run the installation for packaging purpose.

```
FAKETREE=$(mktemp -d)
fakeroot make DESTDIR=${FAKETREE} PREFIX=/usr LDCONFIG= install
```

## Notes / Advice / Known bug

* Write tests in a distinct file but with a similar name to the one you test.
* Include "*.specs*" in file names to keep in mind they do specify behaviors.
* You can compile separately the piece of software to be tested and its tests.
* There is already a *main()* function as a part of Kaputti. Don't link yours.
* With GCC prior to version 9, the line number in failure message is wrong.


## MIT License

Copyright (c) Jean-Michel Morani, January 2020

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
