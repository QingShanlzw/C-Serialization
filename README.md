# C++-Serialization
Use the original c++ to implement a serialization (support basic types, composite types and custom types)
采用二进制的存储方式进行存储。
1、基础类型：
对于char，int32，string（这个实质是一个类），bool等等这些基本类型，我们统一的方式 按照 type（1byte）+ sizeof(类型)的方式进存储
2、复合类型：
类似于 vector<T> , map<K,V>这样的容器，我们借助模板按照 type（1byte）+ size（T）遍历的方式来存储
3、自定义类型：
自定义类型的实现的方式主要方式有两点：
  1）：借助C中的宏定义的方式，以及c++11中提出的Variable Tamplate的方式，实现类的属性注册。
  2）：使用访问者模式，将所有需要序列化的类都继承自一个母类（Object），从这个母类身上继承下来的序列化和反序列的方法。
how to use:
把serizlize.h 和 Datastream.h 两个文件引入项目中，需要序列化的类继承自Object即可。
最后：
这个项目的大体框架已经搭好，只不过一些数据据类型还未包含（思路基本一致）
需要注意存在不同pc大小端的问题，需要额外处理大小端读取的问题。
  
  
  
