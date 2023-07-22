#ifndef BPP_COLLECTIONS_SERIALIZE_H
#define BPP_COLLECTIONS_SERIALIZE_H
#include "../Utility/ByteOrdering.h"
#include <cstdint>
#include <string>
#include <vector>

namespace bpp{
	namespace collections{
		namespace serialize{

//! serialize is a function to allow serialization of a single object. A stream-like buffer which supports the write(const char*, std::streamsize) method and a single object are required. A specialization of the Serialize template must exist for the specified object type. Returns the number of bytes written to the buffer.
template <typename Buff, typename T> std::size_t serialize(Buff& buffer, const T& obj);
//! serialize is a function to allow continuous serialization of one or more objects. A stream-like buffer which supports the write(const char*, std::streamsize) method and at least a single object are required as initial parameters. Specializations of the Serialize template must exist for all the specified object types. Returns the number of bytes written to the buffer.
template <typename Buff, typename T, typename... Args> std::size_t serialize(Buff& buffer, const T& obj, const Args&... args);

//! Each specialization of the Serialize template defines a function object that implements the serialization of a specific data type or structure. The serialized object is written to a stream-like buffer which supports the write(const char*, std::streamsize) method and returns the number of bytes written to the buffer.
template <typename Buff, typename T, typename = void> struct Serialize{
	//! Callable function to be implemented in all template specializations of Serialize. The serialized object is written to a stream-like buffer which supports the write(const char*, std::streamsize) method and returns the number of bytes written to the buffer.
	std::size_t operator()(Buff& buffer, const T& obj) const=0;
};

//! Specialization of the Serialize template to support all arithmetic types and their aliases. Stores basic types using little-endian byte ordering.
template <typename Buff, typename T> struct Serialize<Buff,T,std::enable_if_t<std::is_arithmetic<T>::value>>{
	std::size_t operator()(Buff& buffer, const T& obj) const;
};

//! Specialization of the Serialize template to support std::basic_string objects.
template <typename Buff, typename... Types> struct Serialize<Buff,std::basic_string<Types...>>{
	std::size_t operator()(Buff& buffer, const std::basic_string<Types...>& obj) const;
};

//! Specialization of the Serialize template to support std::vector objects.
template <typename Buff, typename... Types> struct Serialize<Buff,std::vector<Types...>>{
	std::size_t operator()(Buff& buffer, const std::vector<Types...>& obj) const;
};

//! deserialize is a function to allow deserialization of a single object. A stream-like buffer which supports the read(char*, std::streamsize) method and a single object are required. A specialization of the Deserialize template must exist for the specified object type. Returns the number of bytes read from the buffer.
template<typename Buff, typename T> std::size_t deserialize(Buff& buffer, T& obj);
//! deserialize is a function to allow continuous deserialization of one or more objects. A stream-like buffer which supports the read(char*, std::streamsize) method and at least a single object are required as initial parameters. Specializations of the Deserialize template must exist for all the specified object types. Returns the number of bytes read from the buffer.
template<typename Buff, typename T, typename... Args> std::size_t deserialize(Buff& buffer, T& obj, Args&... args);

//! Each specialization of the Deserialize template defines a function object that implements the deserialization of a specific data type or structure. The deserialized object is read from a stream-like buffer which supports the read(char*, std::streamsize) method and returns the number of bytes read from the buffer.
template <typename Buff, typename T, typename = void> struct Deserialize{
	//! Callable function to be implemented in all template specializations of Deserialize. The deserialized object is read to a stream-like buffer which supports the read(char*, std::streamsize) method and returns the number of bytes read from the buffer.
	std::size_t operator()(Buff& buffer, T& obj) const=0;
};

//! Specialization of the Desearialize template to support all arithmetic types and their aliases. Loads basic types using little-endian byte ordering.
template <typename Buff, typename T> struct Deserialize<Buff,T,std::enable_if_t<std::is_arithmetic<T>::value>>{
	std::size_t operator()(Buff& buffer, T& obj) const;
};

//! Specialization of the Desearialize template to support std::basic_string objects.
template <typename Buff, typename... Types> struct Deserialize<Buff,std::basic_string<Types...>>{
	std::size_t operator()(Buff& buffer, std::basic_string<Types...>& obj) const;
};

//! Specialization of the Desearialize template to support std::vector objects.
template <typename Buff, typename... Types> struct Deserialize<Buff,std::vector<Types...>>{
	std::size_t operator()(Buff& buffer, std::vector<Types...>& obj) const;
};

		};
	};
};

#include "Serialize.cpp"

#endif
