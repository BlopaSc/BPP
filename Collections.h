#ifndef BPP_COLLECTIONS_H
#define BPP_COLLECTIONS_H
#define BPP_COLLECTIONS_STANDALONE
#ifndef BPP_STANDALONE
#include <compare>
#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>
#endif

//! B++ namespace
namespace bpp{
	//! Generic sorted associative containers that contains key-value pairs with unique keys. Keys are sorted by using the comparison function Compare.
	namespace map{
		#include "Collections/TreeAVL.h"
	}
};

#endif
