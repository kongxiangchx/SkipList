# SkipList
基于SkipList的轻量级KV数据库，使用C++语言编写。

## 简述
- 实现了SkipList，通过它来完成对KV数据的增删改查。（重载了[]，可以通过skiplist[key]访问并修改KV数据）
- 使用自旋锁保障数据修改时的线程安全，自旋锁的最大尝试次数默认是10。
- 通过模板编程实现了对字符串、列表、集合、有序集合和哈希表的存储，在C++中分别对应STL中的string、vector、unordered_set、set、unordered_map。
- SkipList可写入文件，也可从文件中加载，因此需要实现SkipList的序列化和反序列化。本项目使用了仿函数的形式实现了五种基础类型与string之间的相互转换。
- key默认支持string，value默认支持上述五种类型，但是key和value也可以使用int、double、char等数据类型，它们的序列化和反序列化使用了boost中的lexical_cast函数。


## 后续有待优化的部分
- SkipList在定义时需要指定key和value的类型，也就是说，同一个SkipList只能接收相同类型的KV数据。后续准备使用union或variant来保存value数据，就可以实现不同类型value的存储。
- 当前版本是对增删改的函数部分进行加锁，锁的粒度太大，后续可以尝试减小锁的粒度，即只对需要进行修改的Node节点进行加锁，例如增删需要对它前后的节点进行加锁，而改只需要对当前节点进行加锁。
- 说是KV数据库，但只是和KV数据库沾了点边，后续可以仿照其它KV数据库（Redis、Leveldb）进行优化。