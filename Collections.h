#ifndef BPP_COLLECTIONS_H
#define BPP_COLLECTIONS_H

namespace bpp{
	//! @defgroup group_collections Collections
	//! @brief Namespaces of the Collections module of B++.
	//!
	//! The Collections module holds all data structures classes for storing and managing information.
	//! The header file <Collections.h> will include all of the Collections module namespaces into the project.
	//! @{
	
	//! Generic ordered container that contains index-accessible values.
	namespace list{
		
	}
	
	//! Generic sorted associative containers that contains key-value pairs with unique keys. Keys are sorted by using the comparison function Compare.
	namespace map{
		
	}
	
	//! @}
}

#include "Collections/ListArray.h"
#include "Collections/TreeAVL.h"

#endif
