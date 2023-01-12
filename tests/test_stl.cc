#include "chx/skiplist.h"
#include <functional>
#include <cstring>
#include <list>
#include <unordered_set>
#include <set>
#include <unordered_map>

void test_string() {
    chx::SkipList<std::string, std::string> skipList(6);
	skipList.insertNode("name", "jack");
    skipList.insertNode("age", "18");
    skipList.dumpFile("../store/string.kv");
    skipList.displayList();

    chx::SkipList<std::string, std::string> skipList1(6);
    skipList1.loadFile("../store/string.kv");
    skipList1.displayList();
}

void test_list() {
    chx::SkipList<std::string, std::list<std::string>> skipList(6);
	skipList.insertNode("name", std::list<std::string>{"jack", "rose"});
    skipList.dumpFile("../store/list.kv");
    skipList.displayList();

    chx::SkipList<std::string, std::list<std::string>> skipList1(6);
    skipList1.loadFile("../store/list.kv");
    skipList1.displayList();
}

void test_uset() {
    chx::SkipList<std::string, std::unordered_set<std::string>> skipList(6);
	skipList.insertNode("name", std::unordered_set<std::string>{"jack","rose"});
    skipList.dumpFile("../store/uset.kv");
    skipList.displayList();

    chx::SkipList<std::string, std::unordered_set<std::string>> skipList1(6);
    skipList1.loadFile("../store/uset.kv");
    skipList1.displayList();
}

void test_set() {
    chx::SkipList<std::string, std::set<std::string>> skipList(6);
	skipList.insertNode("name", std::set<std::string>{"jack","rose"});
    skipList.dumpFile("../store/set.kv");
    skipList.displayList();

    chx::SkipList<std::string, std::set<std::string>> skipList1(6);
    skipList1.loadFile("../store/set.kv");
    skipList1.displayList();
}

void test_umap() {
    chx::SkipList<std::string, std::unordered_map<std::string,std::string>> skipList(6);
	skipList.insertNode("name", std::unordered_map<std::string,std::string>{{"jack","12"},{"rose","13"}});
    skipList.dumpFile("../store/umap.kv");
    skipList.displayList();

    chx::SkipList<std::string, std::unordered_map<std::string,std::string>> skipList1(6);
    skipList1.loadFile("../store/umap.kv");
    skipList1.displayList();
}

int main() {
    test_string();
    test_list();
    test_uset();
    test_set();
    test_umap();
    return 0;
}