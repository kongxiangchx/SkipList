#include "chx/skiplist.h"
#include <functional>
#include <vector>

int main() {
    chx::SkipList<std::string, std::unordered_map<std::string,std::string>> skipList(6);
	skipList.insertNode("name", std::unordered_map<std::string,std::string>{{"jack","12"},{"rose","13"}});
    skipList.dumpFile("../store/stl.kv");
    skipList.displayList();

    chx::SkipList<std::string, std::unordered_map<std::string,std::string>> skipList1(6);
    skipList1.loadFile("../store/stl.kv");
    skipList1.displayList();
    return 0;
}