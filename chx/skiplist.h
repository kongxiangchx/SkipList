#ifndef __SKIPLIST_H__
#define __SKIPLIST_H__

#include <iostream>
#include <cstring>
#include <stdint.h>
#include <fstream>
#include <functional>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "LexicalCast.h"
#include "lock.h"

#define delimiter ":"

namespace chx {

template<class K, class V>
class SkipList;

template<class K, class V>
class Node {
friend class SkipList<K, V>;
public:
    Node() {}
    Node(K key, V val, int level);
    ~Node();
    K getKey() const;
    V getVal() const;
    void setVal(V val);

    std::string getKeyStr();
    std::string getValStr();
private:
    K m_key;
    V m_val;
    int m_level;
    Node<K, V> **forward;
};

template<class K, class V>
Node<K, V>::Node(K key, V val, int level)
    : m_key(key), m_val(val), m_level(level) {

    this->forward = new Node<K, V>*[level+1];

    memset(this->forward, 0, sizeof(Node<K, V>*)*(level+1));
}

template<class K, class V>
Node<K, V>::~Node() {
    delete []forward;
}

template<class K, class V>
K Node<K, V>::getKey() const {
    return m_key;
}

template<class K, class V>
V Node<K, V>::getVal() const {
    return m_val;
}

template<class K, class V>
void Node<K, V>::setVal(V val) {
    m_val = val;
}

template<class K, class V>
std::string Node<K, V>::getKeyStr() {
    return LexicalCast<K, std::string>()(m_key);
}

template<class K, class V>
std::string Node<K, V>::getValStr() {
    return LexicalCast<V, std::string>()(m_val);
}

template<class K, class V>
class SkipList {
public:
    typedef std::function<bool (const K& key1, const K& key2)> cmp_function;

    SkipList(int maxLevel);
    SkipList(int maxLevel, cmp_function cmp);
    ~SkipList();
    
    int getRandomLevel();

    Node<K, V>* createNode(K key, V val, int level);
    
    bool insertNode(K key, V val);
    void deleteNode(K key);
    bool updateNode(K key, V val);
    bool searchNode(K key);
    Node<K, V>* getNode(K key);

    V& operator[](const K key) {
        Node<K, V>* node = getNode(key);
        if(node != nullptr) {
            return node->m_val;
        }
        else {
            throw std::logic_error("The key is not found");
        }
    }

    bool compare(K key1, K key2) {
        if(m_cmp == nullptr) {
            return key1 < key2;
        }
        else {
            return m_cmp(key1, key2);
        }
    }

    int size() const;

    void dumpFile(const char* filename);
    void loadFile(const char* filename);

    void displayList(); 
    
private:
    bool fromString(const std::string& str, std::string* key, std::string* val);

private:
    int m_maxLevel;
    int m_level;
    Node<K, V> *m_header;

    int m_length;
    cmp_function m_cmp;

    std::ofstream m_fileWriter;
    std::ifstream m_fileReader;

    SpinLock m_spinlock;
};

template<class K, class V>
SkipList<K, V>::SkipList(int maxLevel)
    : m_maxLevel(maxLevel), m_level(0), m_length(0) {
    K key;
    V val;
    m_header = new Node<K, V>(key, val, m_maxLevel);
}

template<class K, class V>
SkipList<K, V>::SkipList(int maxLevel, cmp_function cmp)
    : m_maxLevel(maxLevel), m_level(0), m_length(0), m_cmp(cmp) {
    K key;
    V val;
    m_header = new Node<K, V>(key, val, m_maxLevel);
}
    
template<class K, class V>
SkipList<K, V>::~SkipList() {
    if (m_fileWriter.is_open()) {
        m_fileWriter.close();
    }
    if (m_fileReader.is_open()) {
        m_fileReader.close();
    }
    delete m_header;
}
    
template<class K, class V>
int SkipList<K, V>::getRandomLevel() {
    int k = 1;
    while(rand() % 2) {
        k++;
    }
    k = (k < m_maxLevel) ? k : m_maxLevel;
    return k;
}

template<class K, class V>
Node<K, V>* SkipList<K, V>::createNode(K key, V val, int level) {
    Node<K, V> *node = new Node<K, V>(key, val, level);
    return node;
}

template<class K, class V>   
bool SkipList<K, V>::insertNode(K key, V val) {
    SpinLock::SpinLockGuard guard(m_spinlock);

    Node<K, V> *current = m_header;

    Node<K, V> *update[m_maxLevel+1];
    memset(update, 0, sizeof(Node<K, V>*)*(m_maxLevel+1));

    for(int i = m_level; i >= 0; i--) {
        while(current->forward[i] != nullptr && compare(current->forward[i]->getKey(), key)) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];

    if(current != nullptr && current->getKey() == key) {
        //std::cout << "key : " << key << " exists" << std::endl;
        return false;
    }

    if(current == nullptr || current->getKey() != key) {
        int randomLevel = getRandomLevel();

        if(randomLevel > m_level) {
            for(int i = m_level+1; i <= randomLevel; i++) {
                update[i] = m_header;
            }
            m_level = randomLevel;
        }
        Node<K, V>* node = createNode(key, val, randomLevel);
        for(int i = 0; i <= randomLevel; i++) {
            node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = node;
        }
        //std::cout << "Successfully inserted key : " << node->getKeyStr() << ", value : " << node->getValStr() << std::endl;
        m_length++;
        return true;
    }
    return false;
}

template<class K, class V>
void SkipList<K, V>::deleteNode(K key) {
    SpinLock::SpinLockGuard guard(m_spinlock);

    Node<K, V> *current = m_header;
    Node<K, V> *update[m_maxLevel+1];
    memset(update, 0, sizeof(Node<K, V>*)*(m_maxLevel+1));

    for(int i = m_level; i >= 0; i--) {
        while(current->forward[i] != NULL && compare(current->forward[i]->getKey(), key)) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if(current != nullptr && current->getKey() == key) {
        for(int i = 0; i <= m_level; i++) {
            if(update[i]->forward[i] != current) {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }

        current->~Node();

        while(m_level > 0 && m_header->forward[m_level] == nullptr) {
            m_level--;
        }

        //std::cout << "Successfully deleted key : " << key << std::endl;
        m_length--;
    }
    return ;
}

template<class K, class V>   
bool SkipList<K, V>::updateNode(K key, V val) {
    SpinLock::SpinLockGuard guard(m_spinlock);

    Node<K, V> *current = m_header;

    Node<K, V> *update[m_maxLevel+1];
    memset(update, 0, sizeof(Node<K, V>*)*(m_maxLevel+1));

    for(int i = m_level; i >= 0; i--) {
        while(current->forward[i] != nullptr && compare(current->forward[i]->getKey(), key)) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];

    if(current != nullptr && current->getKey() == key) {
        //std::cout << "update key : " << current->getKeyStr() << " oldValue : " << current->getValStr();
        current->setVal(val);
        //std::cout << " newValue : " << current->getValStr() << std::endl;
        return true;
    }
    return false;
}

template<class K, class V>
bool SkipList<K, V>::searchNode(K key) {
    //std::cout << "search_element-----------------" << std::endl;
    Node<K, V> *current = m_header;

    for(int i = m_level; i >= 0; i--) {
        while(current->forward[i] && compare(current->forward[i]->getKey(), key)) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if(current != nullptr && current->getKey() == key) {
        std::cout << "Found key : " << current->getKeyStr() << ", value : " << current->getValStr() << std::endl;
        return true;
    }

    std::cout << "Not found key : " << key << std::endl;
    return false;
}

template<class K, class V>
Node<K, V>* SkipList<K, V>::getNode(K key) {
    Node<K, V> *current = m_header;

    for(int i = m_level; i >= 0; i--) {
        while(current->forward[i] && compare(current->forward[i]->getKey(), key)) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if(current != nullptr && current->getKey() == key) {
        return current;
    }
    return nullptr;
}

template<class K, class V>
int SkipList<K, V>::size() const{
    return m_length;
}

template<class K, class V>
void SkipList<K, V>::dumpFile(const char* filename) {
    std::cout << "\ndump file---------------------" << std::endl;
    if(m_fileWriter) {
        m_fileWriter.close();
    }
    m_fileWriter.open(filename);
    Node<K, V> *node = m_header->forward[0];
    while(node != nullptr) {
        //std::cout<<node->getKeyStr() << delimiter << node->getValStr() << "\n";
        m_fileWriter << node->getKeyStr() << delimiter << node->getValStr() << "\n";
        node = node->forward[0];
    }
    m_fileWriter.flush();
    m_fileWriter.close();
    return ;
}

template<class K, class V>
void SkipList<K, V>::loadFile(const char* filename) {
    std::cout << "\nload file---------------------" << std::endl;
    if(m_fileReader) {
        m_fileReader.close();
    }
    m_fileReader.open(filename);
    std::string line;
    std::string* key = new std::string();
    std::string* val = new std::string();
    while(getline(m_fileReader, line)) {
        if(fromString(line, key, val)) {
            insertNode(LexicalCast<std::string, K>()(*key), LexicalCast<std::string, V>()(*val));
        }
    }
    delete key;
    delete val;
    m_fileReader.close();
}

template<class K, class V>
void SkipList<K, V>::displayList() {
    std::cout << "\n*****SkipList*****\n";
    for(int i = 0; i <= m_level; i++) {
        Node<K, V> *node = m_header->forward[i];
        std::cout << "Level " << i << " : ";
        while(node != nullptr) {
            std::cout << "{" << node->getKeyStr() << " : " << node->getValStr() << "} ";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
}

template<class K, class V>
bool SkipList<K, V>::fromString(const std::string& str, std::string* key, std::string* val) {
    if(str.empty()) {
        return false;
    }
    size_t pos = str.find(delimiter);
    if(pos == std::string::npos) {
        return false;
    }
    *key = str.substr(0, pos);
    *val = str.substr(pos+1, str.length());
    if(key->empty() || val->empty()) {
        return false;
    }
    return true;
}

}


#endif