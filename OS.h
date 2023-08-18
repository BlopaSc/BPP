#ifndef BPP_OS_H
#define BPP_OS_H

namespace bpp{

	//! OS functions and structures used to interact with the operative system of the machine.
	namespace os{
	//! @defgroup group_os OS
	//! @brief Namespaces of the OS module of B++.
	//!
	//! The OS module holds all functions and structures used to interact with the operative system of the machine.
	//! The header file <OS.h> will include all of the OS module namespaces into the project.
	//! @{

	//! Path provides functions to interact with the file system of the machine, as well as a data structure to represent the paths of the system. Currently only provides Windows support.
	namespace path{

	}

	//! @}
	}
}

#include "OS/Path.h"

#endif
