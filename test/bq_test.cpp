#define FMT_HEADER_ONLY
#include <fmt/printf.h>

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace fmt;

#include "src/BlockingQueue.h"

class Obj {
public:
    Obj() { print("default\n"); }
    Obj(const Obj& rhs) { print("copy constructor\n"); }
    Obj(const Obj&& rhs) { print("move constructor\n"); }
    Obj& operator=(const Obj& rhs) {
        print("copy operator=\n");
        return *this;
    }
    Obj& operator=(const Obj&& rhs) {
        print("move operator=\n");
        return *this;
    }
};

Obj getObj() {
    Obj x;
    return x;
}

Obj getMoveObj() {
    Obj x;
    return move(x);
}

Obj&& getMoveObj2() {
    Obj x;
    return move(x);
}

void test() {
    print("=== {}\n", 1);
    Obj a;
    print("=== {}\n", 2);
    Obj b(a);
    print("=== {}\n", 3);
    Obj c(move(b));
    print("=== {}\n", 4);
    Obj d;
    print("=== {}\n", 5);
    d = c;
    print("=== {}\n", 6);
    d = move(c);
    print("=== {}\n", 7);
    Obj e = getObj();
    print("=== {}\n", 8);
    Obj f = getMoveObj();
    print("=== {}\n", 9);
    Obj g = getMoveObj2();
    print("=== {}\n", 10);
    g = getObj();
    print("=== {}\n", 11);
    g = getMoveObj();
    print("=== {}\n", 12);
    g = getMoveObj2();
    print("=== {}\n", 13);
}

int main() {
    // test();
    BlockingQueue<Obj> bq(2);
    bq.enqueue(Obj());
    bq.enqueue(Obj());

    Obj x = bq.dequeue();

    return 0;
}