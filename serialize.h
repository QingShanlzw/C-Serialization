#pragma once


namespace LearnSerialize {
	namespace serialize {

		class Datastream;
		class Object {
		public:
			virtual void serialize(Datastream& ds) const = 0;
			virtual bool deserialize(Datastream& ds)=0;
		};
        #define SERIALIZE(...)                              \
            void serialize(DataStream & stream) const       \
            {                                               \
                char type = DataStream::CUSTOM;             \
                stream.write((char *)&type, sizeof(char));  \
                stream.write_args(__VA_ARGS__);             \
            }                                               \
                                                            \
            bool unserialize(DataStream & stream)           \
            {                                               \
                char type;                                  \
                stream.read(&type, sizeof(char));           \
                if (type != DataStream::CUSTOM)             \
                {                                           \
                    return false;                           \
                }                                           \
                stream.read_args(__VA_ARGS__);              \
                return true;                                \
            }
	}
}


