/*
序列化：
基本类型是类型自身大小加一个类型的表示。
序列化的方式就是把变量转化为char*类型（二进制）然后存入。


*/

#pragma once
#include<iostream>
#include<vector>
#include <string>
using namespace std;
#include "serialize.h"
namespace LearnSerialize {
	namespace serialize {
		class Datastream {
		public:
            enum DataType
            {
                BOOL = 0,
                CHAR,
                INT32,
                INT64,
                FLOAT,
                DOUBLE,
                STRING,
                VECTOR,
                LIST,
                MAP,
                SET,
                CUSTOM
            };
            Datastream():m_pos(0){}
            ~Datastream(){}
            //所有写的基本方式
            void write(const char* data,int len);
            //overloading all function in there 

            //32 bit int
            void write(int32_t value);
            // string in C
            void write( const char* value);
            // string in C++
            void write(const string & value);
            //复合类型的序列化
            template<typename T>
            void write(const vector<T>& value);
            //自定义类的序列化
            void write(const Object& value);
            //使用c++11的新特性模板的可变参数来实现参数不固定。
            template<typename T,typename ...Args>
            void write_args(const T & value,const Args&... args);
            void wrrte_args();

            //坑：其他的 write重载
            // 
            //操作符重载
            Datastream & operator <<(int32_t value);
            //坑：其他类型的重载操作符：
            Datastream& operator <<(Object& value);

            void reserve(int len);
            //用来展示buffer的内容
            //坑：只写了int32的。
            void show() const ;

            //反序列化：
            bool read(char* data, int len);
            bool read(int32_t& value);

            bool read(Object& value);

            template<typename T,typename ...Args>
            void read_args( T& value, Args&... args);
            void read_args();

            Datastream& operator >>(int32_t& value);
            Datastream& operator >>(Object& value);

		private:
			std::vector<char>m_buf;
            //用于反序列化，相当于一个index一直往前走。
            int m_pos;
		};
        inline Datastream& Datastream::operator<<(int32_t value)
        {
            // TODO: 在此处插入 return 语句
            write(value);
            return *this;
        }
        inline Datastream& Datastream::operator<<(Object& value)
        {
            // TODO: 在此处插入 return 语句
            write(value);
            return *this;
        }
        //手动给buf扩容
        void Datastream::reserve(int len) {
            int size = m_buf.size();
            int cap = m_buf.capacity();
            if (size + len > cap) {
                while (size + len > cap) {
                    if (cap == 0) {
                        cap = 1;
                    }
                    else {
                        cap *= 2;
                    }
                }
                m_buf.reserve(cap);
            }
        }
        inline void Datastream::show() const
        {
            int len = m_buf.size();
            int i = 0;
            cout << "buffer's size is " << len << endl;
            while (i < len) {
                switch ((DataType)m_buf[i])
                {
                case DataType::INT32:
                    cout << *(int32_t*)&m_buf[++i] << endl;
                    i += 4;
                    break;
                default:
                    break;
                }
            }
        }
        inline bool Datastream::read(char* data, int len)
        {
            std::memcpy(data, (char*)&m_buf[m_pos], len);
            m_pos += len;
            return true;
        }
        inline bool Datastream::read(int32_t& value)
        {
            if ((DataType)m_buf[m_pos] != DataType::INT32) {
                return false;
            }
            m_pos++;
            value = *(int32_t*)(&m_buf[m_pos]);
            m_pos += 4;
            return true;
        }
        inline bool Datastream::read(Object& value)
        {
            return value.deserialize(*this);
        }
        inline Datastream& Datastream::operator>>(int32_t& value)
        {
            // TODO: 在此处插入 return 语句
            read(value);
            return *this;
        }
        inline Datastream& Datastream::operator>>(Object& value)
        {
            // TODO: 在此处插入 return 语句
            read(value);
            return *this;
        }
        void Datastream::write(const char* data, int len) {
            reserve(len);
            int size = m_buf.size();
            m_buf.resize(size + len);
            std::memcpy(&m_buf[size],data,len);
        }
        void Datastream::write(int32_t value) {
            char tp = DataType::INT32;
            write((char*)&tp, sizeof(char));
            write((char*)&value, sizeof(int));
        }
         void Datastream::write(const char* value)
        {
            char tp = DataType::STRING;
            write((char*)&tp, sizeof(char));
            int len = strlen(value);
            write(len);
            write(value, len);
        }
         inline void Datastream::write(const string& value)
         {
             char tp = DataType::STRING;
             write((char*)&tp, sizeof(char));
             int len = value.length();
             write(len);
             write(value.data(), len);
         }
         inline void Datastream::write(const Object& value)
         {
             value.serialize(*this); 
         }

         template<typename T>
         inline void Datastream::write(const vector<T>& value)
         {
             char tp = DataType::VECTOR;
             write((char*)tp, sizeof(char));
             int len = value.size();
             write(len);
             for (int i = 0; i < len; i++) {
                 write(value[i]);
             }
         }
         template<typename T, typename ...Args>
         inline void Datastream::write_args(const T& value, const Args & ...args)
         {
             write(value);
             write(args...);
         }
         inline void Datastream::wrrte_args()
         {
         }
         template<typename T, typename ...Args>
         inline void Datastream::read_args(T& value, Args & ...args) {
             read(value);
             read(args...);
         }
         inline void Datastream::read_args()
         {
         }
}
}