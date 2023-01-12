#include "chx/skiplist.h"
#include <functional>
#include <thread>

chx::SkipList<std::string, std::string> skipList(18);

std::string rand_str(const int len)  /*参数为字符串的长度*/
{
    /*初始化*/
    std::string str;                 /*声明用来保存随机字符串的str*/
    char c;                     /*声明字符c，用来保存随机生成的字符*/
    int idx;                    /*用来循环的变量*/
    /*循环向字符串中添加随机生成的字符*/
    for(idx = 0;idx < len;idx ++)
    {
        /*rand()%26是取余，余数为0~25加上'a',就是字母a~z,详见asc码表*/
        c = 'a' + rand()%26;
        str.push_back(c);       /*push_back()是string类尾插函数。这里插入随机字符c*/
    }
    return str;                 /*返回生成的随机字符串*/
}
int cnt = 0;
void fun() {
    uint64_t old = time(0);
    while(time(0)-old <= 1) {
        int op = 0;
        switch (op)
        {
        case 0:
            skipList.insertNode(rand_str(4), rand_str(30));
            break;
        case 1:
            skipList.deleteNode(rand_str(4));
            break;
        case 2:
            skipList.updateNode(rand_str(4), rand_str(30));
            break;
        case 3:
            skipList.searchNode(rand_str(4));
            break;
        default:
            break;
        }
        cnt++;
    }
}

int main() {
    std::vector<std::thread> Threads;
    for(int i = 0; i<1; i++)
    {
        Threads.push_back(std::thread(fun));
    }
    for(auto& i : Threads)
    {
        i.join();
    }
    skipList.dumpFile("../store/test.kv");
    std::cout<<skipList.size()<<std::endl;
    std::cout<<"cnt : " <<cnt<<std::endl;
    return 0;
}