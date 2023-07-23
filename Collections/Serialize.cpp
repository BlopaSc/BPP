#ifndef BPP_COLLECTIONS_SERIALIZE_CPP
#define BPP_COLLECTIONS_SERIALIZE_CPP

namespace bpp{
	namespace collections{
		namespace serialize{

// Global callable functions
template<typename Buff, typename T> std::size_t serialize(Buff& buffer, const T& obj){
	return Serialize<Buff,T>()(buffer, obj);
}

template<typename Buff, typename T, typename... Args> std::size_t serialize(Buff& buffer, const T& obj, const Args&... args){
	std::size_t res = Serialize<Buff,T>()(buffer,obj);
	return res + serialize(buffer, args...);
}

// Specializations of Serialize template
template<typename Buff, typename T> std::size_t Serialize<Buff,T,std::enable_if_t<std::is_arithmetic<T>::value>>::operator()(Buff& buffer, const T& obj) const{
	union{
		uint64_t val64;
		uint32_t val32;
		uint16_t val16;
	};
	switch(sizeof(T)){
		case 1:
			buffer.write((const char*)&obj, 1);
			break;
		case 2:
			val16 = bpp::utility::byte_ordering::htole_16(obj);
			buffer.write((const char*)&val16, 2);
			break;
		case 4:
			val32 = bpp::utility::byte_ordering::htole_32(obj);
			buffer.write((const char*)&val32, 4);
			break;
		case 8:
			val64 = bpp::utility::byte_ordering::htole_64(obj);
			buffer.write((const char*)&val64, 8);
			break;
	}
	return sizeof(T);
}
template<typename Buff, typename... Types> std::size_t Serialize<Buff,std::basic_string<Types...>>::operator()(Buff& buffer, const std::basic_string<Types...>& obj) const{
	std::size_t chars = obj.size();
	std::size_t res = serialize(buffer, chars);
	for(std::size_t i=0; i<chars; ++i){
		res += serialize(buffer, obj[i]);
	}
	return res;
}
template<typename Buff, typename... Types> std::size_t Serialize<Buff,std::vector<Types...>>::operator()(Buff& buffer, const std::vector<Types...>& obj) const{
	std::size_t objs = obj.size();
	std::size_t res = serialize(buffer, objs);
	for(std::size_t i=0; i<objs; ++i){
		res += serialize(buffer, obj[i]);
	}
	return res;
}
template <typename Buff, typename... Types> std::size_t Serialize<Buff,std::pair<Types...>>::operator()(Buff& buffer, const std::pair<Types...>& obj) const{
	return serialize(buffer, obj.first, obj.second);
}

// Global callable functions
template<typename Buff, typename T> std::size_t deserialize(Buff& buffer, T& obj){
	return Deserialize<Buff,T>()(buffer, obj);
}

template<typename Buff, typename T, typename... Args> std::size_t deserialize(Buff& buffer, T& obj, Args&... args){
	std::size_t res = Deserialize<Buff,T>()(buffer,obj);
	return res + deserialize(buffer, args...);
}

// Specializations of Deserialize template
template<typename Buff, typename T> std::size_t Deserialize<Buff,T,std::enable_if_t<std::is_arithmetic<T>::value>>::operator()(Buff& buffer, T& obj) const{
	union{
		uint64_t val64;
		uint32_t val32;
		uint16_t val16;
	};
	switch(sizeof(T)){
		case 1:
			buffer.read((char*)&obj, 1);
			break;
		case 2:
			buffer.read((char*)&val16, 2);
			obj = bpp::utility::byte_ordering::letoh_16(val16);
			break;
		case 4:
			buffer.read((char*)&val32, 4);
			obj = bpp::utility::byte_ordering::letoh_32(val32);
			break;
		case 8:
			buffer.read((char*)&val64, 8);
			obj = bpp::utility::byte_ordering::letoh_64(val64);
			break;
	}
	return sizeof(T);
}
template<typename Buff, typename... Types> std::size_t Deserialize<Buff,std::basic_string<Types...>>::operator()(Buff& buffer, std::basic_string<Types...>& obj) const{
	std::size_t chars;
	std::size_t res = deserialize(buffer, chars);
	obj.clear();
	obj.reserve(chars);
	for(std::size_t i=0; i<chars; ++i){
		typename std::basic_string<Types...>::value_type val;
		res += deserialize(buffer, val);
		obj += val;
	}
	return res;
}
template<typename Buff, typename... Types> std::size_t Deserialize<Buff,std::vector<Types...>>::operator()(Buff& buffer, std::vector<Types...>& obj) const{
	std::size_t objs;
	std::size_t res = deserialize(buffer, objs);
	obj.clear();
	obj.reserve(objs);
	for(std::size_t i=0; i<objs; ++i){
		typename std::vector<Types...>::value_type val;
		res += deserialize(buffer, val);
		obj.push_back(std::move(val));
	}
	return res;
}
template<typename Buff, typename... Types> std::size_t Deserialize<Buff,std::pair<Types...>>::operator()(Buff& buffer, std::pair<Types...>& obj) const{
	return deserialize(buffer, obj.first, obj.second);
}

		};
	};
};

#endif
