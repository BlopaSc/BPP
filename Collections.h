#ifndef BPP_COLLECTIONS_H
#define BPP_COLLECTIONS_H

namespace bpp{
	
	//! Data structures for data management.
	namespace collections{
	//! @defgroup group_collections Collections
	//! @brief Namespaces of the Collections module of B++.
	//!
	//! The Collections module holds all data structures classes for storing and managing information.
	//! The header file <Collections.h> will include all of the Collections module namespaces into the project.
	//! @{
	
	//! Generic containers that allow quick lookup times for the smallest element.
	namespace priority_queue{
		
	}
	
	//! Generic sorted associative containers that contains key-value pairs with unique keys. Keys are sorted by using the comparison function Compare.
	namespace map{
		
	}
	
	//! Iterators for special sequences of elements of a set, like permutations and combinations.
	namespace iteration{
		
	}
	
	//! @}
	}
}

#include "Collections/TreeAVL.h"
#include "Collections/Combination.h"

#endif
