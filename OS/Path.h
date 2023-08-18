#ifndef BPP_OS_PATH_H
#define BPP_OS_PATH_H
#include <cstring>      // memcpy
#include <cstdint>      // uint64_t
#include <vector>       // std::vector<>
#ifdef __unix__
	// TODO: Specialized includes
#elif defined(_WIN32) || defined(WIN32)
    #include <windows.h>
#endif

namespace bpp{
	namespace os{
		namespace path{

#ifdef __unix__
	typedef char T_PATH;
#elif defined(_WIN32) || defined(WIN32)
    typedef wchar_t T_PATH;
#endif

struct Path;
struct PathAttributes;

// Global methods
//! Creates a directory in the specified path. Returns whether the operation was successful or failed.
template <class T> bool mkDir(const T* path);
//! Returns whether or not the specified path exists (represents a valid file or directory in the filesystem).
template <class T> bool exists(const T* path);
//! Returns whether or not the specified path corresponds to a directory in the filesystem.
template <class T> bool isDir(const T* path);
//! Returns whether or not the specified path corresponds to a file in the filesystem.
template <class T> bool isFile(const T* path);
//! Returns whether or not the specified path corresponds to a hidden file or directory in the filesystem.
template <class T> bool isHidden(const T* path);
//! Returns whether or not the specified path corresponds to a system file or directory in the filesystem.
template <class T> bool isSystem(const T* path);
//! Returns whether or not the specified path corresponds to a temporary file or directory in the filesystem.
template <class T> bool isTemporary(const T* path);
//! Returns the size of the file specified by path. If the path corresponds to a directory or does not exists then the returned size is 0.
template <class T> uint64_t getSize(const T* path);
//! Returns an unsigned 64-bit integer representing the creation time of the file speficied by path. What this number means may vary from system to system.
template <class T> uint64_t getCreationTime(const T* path);
//! Returns an unsigned 64-bit integer representing the last access time of the file speficied by path. What this number means may vary from system to system.
template <class T> uint64_t getLastAccessTime(const T* path);
//! Returns an unsigned 64-bit integer representing the last modification time of the file speficied by path. What this number means may vary from system to system.
template <class T> uint64_t getLastModifiedTime(const T* path);
//! Returns a Path object with the absolute path of the specified path.
template <class T> Path getAbsolutePath(const T* path);
//! Returns a Path object with the path to the parent of the specified path.
template <class T> Path getParent(const T* path);
//! Returns a vector with the list of all files contained in the directory specified by path. If the path does not exists or is not a directory the returned vector is empty.
template <class T> std::vector<Path> getListDirectory(const T* path);

//! PathAttributes is a data structure that stores the attributes of a given file or directory in the filesystem.
struct PathAttributes{
    uint64_t fileSize, creationTime, lastAccessTime, lastModifyTime;
    bool exists, isDirectory, isHidden, isSystem, isTemporary;
};

//! Path is an specialized string-like container to handle filesystem paths and get information on the directories and files stored in them.
struct Path{
	//! Type of the characteres required to store the path. Depending on the system may be char or wchar_t.
	using T_Path = T_PATH;
	
	// Constructors
	//! Constructs an empty container.
	Path();
	//! Copy constructor. Constructs the container with the copy of the contents of other.
	Path(const Path& other);
	//! Move constructor. Constructs the container with the contents of other using move semantics. After the move, other is empty.
	Path(Path&& other);
	//! Constructs the container with the provided path given and reads the attributes of the specified file or directory.
	template <class T> Path(const T* path);
	
	//! Destructs the path. Any allocated memory is released.
	~Path();
	
	// Assignement
	//! Copy assignment operator. Replaces the contents with a copy of the contents of other.
	Path& operator=(const Path& other);
	//! Move assignment operator. Replaces the contents with those of other using move semantics. After the move, other is empty.
	Path& operator=(Path&& other);
	//! Replaces the contents in path with those of the new path. Reads the attributes of the new specified file or directory.
	template <class T> Path& operator=(const T* path);
	
	// Length
	//! Returns the total length of the path.
	std::size_t getLength() const;
	
	// Methods
	//! Returns whether the specified path exists in the filesystem.
	bool exists() const;
	//! Returns whether the specified path corresponds to a directory in the filesystem.
	bool isDir() const;
	//! Returns whether the specified path corresponds to a file in the filesystem.
	bool isFile() const;
	//! Returns whether the specified path corresponds to a hidden file or directory in the filesystem.
	bool isHidden() const;
	//! Returns whether the specified path corresponds to a system file or directory in the filesystem.
	bool isSystem() const;
	//! Returns whether the specified path corresponds to a temporary file or directory in the filesystem.
	bool isTemporary() const;
	//! Returns the size of the file for the specified path. If the path corresponds to a directory or does not exists then the returned size is 0.
	uint64_t getSize() const;
	//! Returns an unsigned 64-bit integer representing the creation time of the file. What this number means may vary from system to system.
	uint64_t getCreationTime() const;
	//! Returns an unsigned 64-bit integer representing the last access time of the file. What this number means may vary from system to system.
	uint64_t getLastAccessTime() const;
	//! Returns an unsigned 64-bit integer representing the last modification time for the file. What this number means may vary from system to system.
	uint64_t getLastModifiedTime() const;
	//! Returns a Path object with the absolute path of the current object.
	Path getAbsolutePath() const;
	//! Transforms the current path to its absolute path form. If the path was already in absolute form no change is applied.
	Path& toAbsolutePath();
	//! Returns a Path object with the path to the parent of the current object.
	Path getParent(bool* success=0) const;
	//! Transforms the current path to the path of its parent.
	Path& toParent(bool* success=0);
	//! Returns a vector with the list of all files contained in the current directory. If the path does not exists or is not a directory the returned vector is empty.
	std::vector<Path> getListDirectory() const;

	// Data access
	//! Returns a pointer to the entire path stored in the object.
	const T_Path* getPath() const;
	//! Returns a pointer to the name of the path stored in the object.
	const T_Path* getName() const;
	//! Returns a pointer to the extension of the file stored in the object. If the path doesn't contains an extension then the returned pointer will point to the end of the string.
	const T_Path* getExtension() const;
	private:
        PathAttributes attr;
		T_PATH *path;
		std::size_t length, name, ext;
		static const std::size_t MAX_LENGTH;
		void parsePath();
		void getPathAttributes();
};

		}
	}
}

#endif
