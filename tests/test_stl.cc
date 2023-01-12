#include "chx/skiplist.h"
#include <functional>
#include <vector>

int main() {
    chx::SkipList<std::string, std::unordered_map<std::string,std::string>> skipList(6);
	skipList.insertNode("name", std::unordered_map<std::string,std::string>{{"jack","12"},{"rose","13"}});
    skipList.dumpFile("kv_stl");
    skipList.displayList();

    chx::SkipList<std::string, std::unordered_map<std::string,std::string>> skipList1(6);
    skipList1.loadFile("kv_stl");
    skipList1.displayList();
    return 0;
}