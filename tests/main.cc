#include "chx/skiplist.h""
#include <functional>
int main() {
    chx::SkipList<int, std::string> skipList(6);
	// skipList.insertNode(1, "学"); 
	// skipList.insertNode(3, "算法"); 
	// skipList.insertNode(7, "认准"); 
	// skipList.insertNode(8, "微信公众号：代码随想录"); 
	// skipList.insertNode(9, "学习"); 
	// skipList.insertNode(19, "算法不迷路"); 
	// skipList.insertNode(19, "赶快关注吧你会发现详见很晚！"); 
    // skipList.dumpFile("kv");
    skipList.loadFile("kv");

    std::cout << "skipList size:" << skipList.size() << std::endl;


    // skipList.load_file();

    skipList.searchNode(9);
    skipList.searchNode(18);

    //std::cout << skipList[18];
    std::cout << skipList[9];

    // skipList.displayList();

    // skipList.deleteNode(3);
    // skipList.deleteNode(7);

    // std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.displayList();
    return 0;
}