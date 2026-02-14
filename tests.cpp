#include "card.h"
#include "card_list.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static string capturePrint(const CardList& list) {
    std::streambuf* old = std::cout.rdbuf();
    ostringstream oss;
    std::cout.rdbuf(oss.rdbuf());
    list.print();
    std::cout.rdbuf(old);
    return oss.str();
}

static string cardToString(const Card& card) {
    ostringstream oss;
    oss << card;
    return oss.str();
}

static void loadCards(CardList& list, const vector<Card>& cards) {
    for (const auto& c : cards) list.insert(c);
}

struct TestRunner {
    int passed = 0;
    int failed = 0;

    void check(bool cond, const string& name) {
        if (cond) {
            ++passed;
            cout << "passed " << name << "\n";
        } else {
            ++failed;
            cout << "failed " << name << "\n";
        }
    }
};

static void testInsert(TestRunner& t) {
    {
        CardList list;
        t.check(list.insert(Card("c", "a")), "insert #1: first insert succeeds");
    }
    {
        CardList list;
        Card x("d", "10");
        bool ok = list.insert(x) && !list.insert(x);
        t.check(ok, "insert #2: duplicate insert rejected");
    }
    {
        CardList list;
        bool ok = true;
        ok = ok && list.insert(Card("c", "2"));
        ok = ok && list.insert(Card("c", "a"));
        ok = ok && list.insert(Card("h", "k"));
        t.check(ok, "insert #3: multiple unique inserts succeed");
    }
    {
        CardList list;
        Card root("d", "7");
        Card left("c", "5");
        Card right("h", "3");
        list.insert(root);
        bool ok = list.insert(left) && list.insert(right);
        t.check(ok, "insert #4: inserts can go left and right of root");
    }
    {
        CardList list;
        Card x("s", "9");
        list.insert(x);
        list.erase(x);
        t.check(list.insert(x), "insert #5: reinsert after erase works");
    }
}

static void testContains(TestRunner& t) {
    {
        CardList list;
        t.check(!list.contains(Card("c", "a")), "contains #1: empty list returns false");
    }
    {
        CardList list;
        Card x("d", "3");
        list.insert(x);
        t.check(list.contains(x), "contains #2: single element found");
    }
    {
        CardList list;
        Card x("d", "3");
        list.insert(x);
        t.check(!list.contains(Card("d", "4")), "contains #3: missing element not found");
    }
    {
        CardList list;
        Card a("c", "2"), b("d", "10"), c("h", "k");
        loadCards(list, {a, b, c});
        t.check(list.contains(b), "contains #4: found in multi-node tree");
    }
    {
        CardList list;
        Card z("s", "q");
        list.insert(z);
        list.erase(z);
        t.check(!list.contains(z), "contains #5: erased element not found");
    }
}

static void testErase(TestRunner& t) {
    {
        CardList list;
        t.check(!list.erase(Card("c", "a")), "erase #1: erase from empty returns false");
    }
    {
        CardList list;
        list.insert(Card("d", "3"));
        t.check(!list.erase(Card("d", "4")), "erase #2: erase missing returns false");
    }
    {
        CardList list;
        Card root("d", "7");
        Card leaf("c", "a");
        list.insert(root);
        list.insert(leaf);
        bool ok = list.erase(leaf) && !list.contains(leaf) && list.contains(root);
        t.check(ok, "erase #3: remove leaf keeps root");
    }
    {
        CardList list;
        Card a("d", "7"), b("c", "5"), c("c", "4");
        loadCards(list, {a, b, c});
        bool ok = list.erase(b) && !list.contains(b) && list.contains(c) && list.contains(a);
        t.check(ok, "erase #4: remove node with one child");
    }
    {
        CardList list;
        Card n1("d", "7"), n2("c", "5"), n3("h", "8"), n4("d", "6");
        loadCards(list, {n1, n2, n3, n4});
        bool ok = list.erase(n1) && !list.contains(n1)
               && list.contains(n2) && list.contains(n3) && list.contains(n4);
        t.check(ok, "erase #5: remove node with two children");
    }
}

static void testPrint(TestRunner& t) {
    {
        CardList list;
        t.check(capturePrint(list) == "", "print #1: empty prints nothing");
    }
    {
        CardList list;
        list.insert(Card("c", "a"));
        t.check(capturePrint(list) == "c a\n", "print #2: single prints one line");
    }
    {
        CardList list;
        loadCards(list, {Card("h", "3"), Card("c", "2"), Card("d", "a")});
        t.check(capturePrint(list) == "c 2\nd a\nh 3\n", "print #3: in-order sorted output");
    }
    {
        CardList list;
        loadCards(list, {Card("c", "2"), Card("d", "a"), Card("h", "3")});
        list.erase(Card("d", "a"));
        t.check(capturePrint(list) == "c 2\nh 3\n", "print #4: after erase, order updates");
    }
    {
        CardList list;
        Card d("s", "10");
        list.insert(d);
        list.insert(d);
        t.check(capturePrint(list) == "s 10\n", "print #5: duplicates not printed twice");
    }
}

static void testIterator(TestRunner& t) {
    {
        CardList empty;
        t.check(empty.begin() == empty.end(), "iter #1: empty begin==end");
        t.check(empty.rbegin() == empty.rend(), "iter #2: empty rbegin==rend");
    }
    {
        CardList single;
        Card only("d", "5");
        single.insert(only);

        auto it = single.begin();
        bool ok = (it != single.end()) && (*it == only);
        ++it;
        ok = ok && (it == single.end());
        t.check(ok, "iter #3: forward iteration on single node");
    }
    {
        CardList single;
        Card only("d", "5");
        single.insert(only);

        auto rit = single.rbegin();
        bool ok = (rit != single.rend()) && (*rit == only);
        ++rit;
        ok = ok && (rit == single.rend());
        t.check(ok, "iter #4: reverse iteration on single node");
    }
    {
        CardList multi;
        Card a("c", "2"), b("d", "a"), c("h", "3");
        loadCards(multi, {c, a, b});

        vector<string> seq;
        for (auto it = multi.begin(); it != multi.end(); ++it) {
            seq.push_back(cardToString(*it));
        }
        t.check(seq == vector<string>({"c 2", "d a", "h 3"}), "iter #5: ++ gives in-order traversal");
    }
    {
        CardList multi;
        Card a("c", "2"), b("d", "a"), c("h", "3");
        loadCards(multi, {c, a, b});

        vector<string> rev;
        for (auto it = multi.rbegin(); it != multi.rend(); ++it) {
            rev.push_back(cardToString(*it));
        }
        t.check(rev == vector<string>({"h 3", "d a", "c 2"}), "iter #6: reverse iterator gives reverse order");
    }
    {
        CardList multi;
        loadCards(multi, {Card("c", "2"), Card("d", "a"), Card("h", "3")});

        auto it = multi.end();
        ++it;
        t.check(it == multi.end(), "iter #7: increment past end stays end");
    }
    {
        CardList multi;
        loadCards(multi, {Card("c", "2"), Card("d", "a"), Card("h", "3")});

        auto it = multi.rend();
        --it;
        t.check(it == multi.rend(), "iter #8: decrement past rend stays rend");
    }
    {
        CardList multi;
        loadCards(multi, {Card("c", "2"), Card("d", "a"), Card("h", "3")});

        auto b1 = multi.begin();
        auto b2 = multi.begin();
        auto r1 = multi.rbegin();
        t.check((b1 == b2) && (b1 != r1), "iter #9: basic == and != behavior");
    }
}

static void playGame(CardList& alice, CardList& bob, std::ostream& out) {
    bool alicePick = true;
    while (true) {
        bool found = false;

        if (alicePick) {
            for (auto it = alice.begin(); it != alice.end(); ++it) {
                if (bob.contains(*it)) {
                    out << "Alice picked matching card " << *it << "\n";
                    Card picked = *it;
                    bob.erase(picked);
                    alice.erase(picked);
                    found = true;
                    break;
                }
            }
        } else {
            for (auto it = bob.rbegin(); it != bob.rend(); ++it) {
                if (alice.contains(*it)) {
                    out << "Bob picked matching card " << *it << "\n";
                    Card picked = *it;
                    alice.erase(picked);
                    bob.erase(picked);
                    found = true;
                    break;
                }
            }
        }

        if (!found) break;
        alicePick = !alicePick;
    }
}

static void testPlayGame(TestRunner& t) {
    {
        CardList alice, bob;
        loadCards(alice, {Card("c", "3"), Card("d", "a"), Card("h", "9")});
        loadCards(bob,   {Card("c", "3"), Card("d", "a"), Card("s", "2")});
        ostringstream out;

        playGame(alice, bob, out);

        bool ok =
            !alice.contains(Card("c", "3")) && !alice.contains(Card("d", "a")) &&
            !bob.contains(Card("c", "3")) && !bob.contains(Card("d", "a")) &&
            alice.contains(Card("h", "9")) && bob.contains(Card("s", "2")) &&
            out.str() == "Alice picked matching card c 3\nBob picked matching card d a\n";

        t.check(ok, "game #1: common cards removed from both lists");
    }
    {
        CardList alice, bob;
        loadCards(bob, {Card("c", "3"), Card("d", "a")});
        ostringstream out;

        playGame(alice, bob, out);

        bool ok = out.str().empty() && bob.contains(Card("c", "3")) && bob.contains(Card("d", "a"));
        t.check(ok, "game #2: empty alice exits immediately");
    }
    {
        CardList alice, bob;
        loadCards(alice, {Card("c", "3"), Card("h", "9")});
        loadCards(bob,   {Card("d", "a"), Card("s", "2")});
        ostringstream out;

        playGame(alice, bob, out);

        bool ok =
            out.str().empty() &&
            alice.contains(Card("c", "3")) && alice.contains(Card("h", "9")) &&
            bob.contains(Card("d", "a")) && bob.contains(Card("s", "2"));

        t.check(ok, "game #3: no overlap => no moves");
    }
}

int main() {
    TestRunner t;

    testInsert(t);
    testContains(t);
    testErase(t);
    testPrint(t);
    testIterator(t);
    testPlayGame(t);

    cout << "\nSummary: " << t.passed << " passed, " << t.failed << " failed\n";
    return (t.failed == 0) ? 0 : 1;
}