#ifndef BPP_UTILITY_BYTEORDERING_CPP
#define BPP_UTILITY_BYTEORDERING_CPP
#if (__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
	#error "__BYTE_ORDER__ Flag is not set or unknown ordering is used by the system"
#endif

namespace bpp{
	namespace utility{
		namespace byte_ordering{

template <typename T> uint16_t htobe_16(T value){
	uint16_t val = *((uint16_t*)(&value));
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	val = (val>>8) | (val<<8);
	#endif
	return val;
}
template <typename T> uint32_t htobe_32(T value){
	uint32_t val = *((uint32_t*)(&value));
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	val = (val>>16) | (val<<16);
	val = ((val&0xFF00FF00U)>>8) | ((val&0x00FF00FFU)<<8);
	#endif
	return val;
}
template <typename T> uint64_t htobe_64(T value){
	uint64_t val = *((uint64_t*)(&value));
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	val = (val>>32) | (val<<32);
	val = ((val&0xFFFF0000FFFF0000ULL)>>16) | ((val&0x0000FFFF0000FFFFULL)<<16);
	val = ((val&0xFF00FF00FF00FF00ULL)>>8) | ((val&0x00FF00FF00FF00FFULL)<<8);
	#endif
	return val;
}

template <typename T> uint16_t htole_16(T value){
	uint16_t val = *((uint16_t*)(&value));
	#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	val = (val>>8) | (val<<8);
	#endif
	return val;
}
template <typename T> uint32_t htole_32(T value){
	uint32_t val = *((uint32_t*)(&value));
	#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	val = (val>>16) | (val<<16);
	val = ((val&0xFF00FF00U)>>8) | ((val&0x00FF00FFU)<<8);
	#endif
	return val;
}
template <typename T> uint64_t htole_64(T value){
	uint64_t val = *((uint64_t*)(&value));
	#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	val = (val>>32) | (val<<32);
	val = ((val&0xFFFF0000FFFF0000ULL)>>16) | ((val&0x0000FFFF0000FFFFULL)<<16);
	val = ((val&0xFF00FF00FF00FF00ULL)>>8) | ((val&0x00FF00FF00FF00FFULL)<<8);
	#endif
	return val;
}

template <typename T> uint16_t betoh_16(T value){ return htobe_16(value); }
template <typename T> uint32_t betoh_32(T value){ return htobe_32(value); }
template <typename T> uint64_t betoh_64(T value){ return htobe_64(value); }

template <typename T> uint16_t letoh_16(T value){ return htole_16(value); }
template <typename T> uint32_t letoh_32(T value){ return htole_32(value); }
template <typename T> uint64_t letoh_64(T value){ return htole_64(value); }

		};
	};
};

#endif
