#include <string>
#include <forward_list>

#include <kaputti.h>

using namespace std;


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
      ->to.never.equal(true);  // synonym
  });

  it("should be case insensitive", {
    Bag bag;
    bag.append("foo");
    expect(bag.contains("FOO"))
      ->to.equal(true);  // will fail
  });
});

describe("With raw pointers", {
  describe("not being char*", {
    it("should compare addresses", {
      int a;
      int *b = &a;
      expect(b)
        ->to.equal(&a);
    });
  });
});

