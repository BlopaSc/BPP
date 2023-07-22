#ifndef BPP_UTILITY_BYTEORDERING_H
#define BPP_UTILITY_BYTEORDERING_H
#include <cstdint>

namespace bpp{
	namespace utility{
		namespace byte_ordering{

//! Transform an unsigned 16-bit integer from the host machine byte-ordering to big-endian byte-ordering.
template <typename T> uint16_t htobe_16(T value);
//! Transform an unsigned 32-bit integer from the host machine byte-ordering to big-endian byte-ordering.
template <typename T> uint32_t htobe_32(T value);
//! Transform an unsigned 64-bit integer from the host machine byte-ordering to big-endian byte-ordering.
template <typename T> uint64_t htobe_64(T value);

//! Transform an unsigned 16-bit integer from the host machine byte-ordering to little-endian byte-ordering.
template <typename T> uint16_t htole_16(T value);
//! Transform an unsigned 32-bit integer from the host machine byte-ordering to little-endian byte-ordering.
template <typename T> uint32_t htole_32(T value);
//! Transform an unsigned 64-bit integer from the host machine byte-ordering to little-endian byte-ordering.
template <typename T> uint64_t htole_64(T value);

//! Transform an unsigned 16-bit integer from the big-endian byte-ordering to the host machine byte-ordering.
template <typename T> uint16_t betoh_16(T value);
//! Transform an unsigned 32-bit integer from the big-endian byte-ordering to the host machine byte-ordering.
template <typename T> uint32_t betoh_32(T value);
//! Transform an unsigned 64-bit integer from the big-endian byte-ordering to the host machine byte-ordering.
template <typename T> uint64_t betoh_64(T value);

//! Transform an unsigned 16-bit integer from the little-endian byte-ordering to the host machine byte-ordering.
template <typename T> uint16_t letoh_16(T value);
//! Transform an unsigned 32-bit integer from the little-endian byte-ordering to the host machine byte-ordering.
template <typename T> uint32_t letoh_32(T value);
//! Transform an unsigned 64-bit integer from the little-endian byte-ordering to the host machine byte-ordering.
template <typename T> uint64_t letoh_64(T value);

		};
	};
};

#include "ByteOrdering.cpp"

#endif
