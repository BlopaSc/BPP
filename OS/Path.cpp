#ifndef BPP_OS_PATH_CPP
#define BPP_OS_PATH_CPP
#include "Path.h"

namespace bpp{
	namespace os{
		namespace path{

// Static global methods (for internal use only)
#ifdef __unix__

#elif defined(_WIN32) || defined(WIN32)
template <class T> static void winGetFileAttributes(const T* path, PathAttributes& attr){
	DWORD tmp;
	switch(sizeof(T)){
		case 1:
            tmp = GetFileAttributesA((const char*) path);
			break;
		case 2:
            tmp = GetFileAttributesW((const wchar_t*) path);
			break;
	}
	if(attr.exists = (tmp != INVALID_FILE_ATTRIBUTES)){
		attr.isHidden = tmp & FILE_ATTRIBUTE_HIDDEN;
		attr.isSystem = tmp & FILE_ATTRIBUTE_SYSTEM;
		attr.isDirectory = tmp & FILE_ATTRIBUTE_DIRECTORY;
		attr.isTemporary = tmp & FILE_ATTRIBUTE_TEMPORARY;
	}
}
template <class T> static void winGetFileData(const T* path, PathAttributes& attr){
    HANDLE handle;
    BY_HANDLE_FILE_INFORMATION ffd;
	DWORD& tmp = ffd.dwFileAttributes;
	switch(sizeof(T)){
		case 1:
            handle = CreateFileA((const char*)path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
			break;
		case 2:
            handle = CreateFileW((const wchar_t*)path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
			break;
	}
	if(attr.exists = (handle != INVALID_HANDLE_VALUE)){
		attr.exists = GetFileInformationByHandle(handle, &ffd);
		CloseHandle(handle);
		if(attr.exists){
            attr.isHidden = tmp & FILE_ATTRIBUTE_HIDDEN;
			attr.isSystem = tmp & FILE_ATTRIBUTE_SYSTEM;
			attr.isDirectory = tmp & FILE_ATTRIBUTE_DIRECTORY;
			attr.isTemporary = tmp & FILE_ATTRIBUTE_TEMPORARY;
			attr.creationTime = (uint64_t)(ffd.ftCreationTime.dwLowDateTime) | ((uint64_t)(ffd.ftCreationTime.dwHighDateTime) << 32ULL);
			attr.lastAccessTime = (uint64_t)(ffd.ftLastAccessTime.dwLowDateTime) | ((uint64_t)(ffd.ftLastAccessTime.dwHighDateTime) << 32ULL);
			attr.lastModifyTime = (uint64_t)(ffd.ftLastWriteTime.dwLowDateTime) | ((uint64_t)(ffd.ftLastWriteTime.dwHighDateTime) << 32ULL);
			attr.fileSize = (uint64_t)(ffd.nFileSizeLow) | ((uint64_t)(ffd.nFileSizeHigh) << 32ULL);
		}
	}
}
#endif

template <class T> bool mkDir(const T* path){
    bool result = false;
	#ifdef __unix__
		result = !mkdir(path,DEFAULT_MODE);
	#elif defined(_WIN32) || defined(WIN32)
	    switch(sizeof(T)){
			case 1:
				if(!(result = CreateDirectoryA((const char*) path,NULL))){
					result = GetLastError() != ERROR_PATH_NOT_FOUND;
				}
				break;
			case 2:
                if(!(result = CreateDirectoryW((const wchar_t*) path,NULL))){
					result = GetLastError() != ERROR_PATH_NOT_FOUND;
				}
				break;
		}
	#endif
	return result;
}
template <class T> bool exists(const T* path){
    PathAttributes attr;
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
	    winGetFileAttributes(path, attr);
	#endif
	return attr.exists;
}
template <class T> bool isDir(const T* path){
    PathAttributes attr;
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
	    winGetFileAttributes(path, attr);
	#endif
	return attr.exists && attr.isDirectory;
}
template <class T> bool isFile(const T* path){
    PathAttributes attr;
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
	    winGetFileAttributes(path, attr);
	#endif
	return attr.exists && !attr.isDirectory;
}
template <class T> bool isHidden(const T* path){
    PathAttributes attr;
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
	    winGetFileAttributes(path, attr);
	#endif
	return attr.exists && attr.isHidden;
}
template <class T> bool isSystem(const T* path){
    PathAttributes attr;
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
	    winGetFileAttributes(path, attr);
	#endif
	return attr.exists && attr.isSystem;
}
template <class T> bool isTemporary(const T* path){
    PathAttributes attr;
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
	    winGetFileAttributes(path, attr);
	#endif
	return attr.exists && attr.isTemporary;
}
template <class T> uint64_t getSize(const T* path){
    PathAttributes attr;
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
	    winGetFileData(path, attr);
	#endif
	return attr.exists ? attr.fileSize : 0;
}
template <class T> uint64_t getCreationTime(const T* path){
    PathAttributes attr;
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
	    winGetFileData(path, attr);
	#endif
	return attr.exists ? attr.creationTime : 0;
}
template <class T> uint64_t getLastAccessTime(const T* path){
    PathAttributes attr;
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
	    winGetFileData(path, attr);
	#endif
	return attr.exists ? attr.lastAccessTime : 0;
}
template <class T> uint64_t getLastModifiedTime(const T* path){
    PathAttributes attr;
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
	    winGetFileData(path, attr);
	#endif
	return attr.exists ? attr.lastModifyTime : 0;
}
template <class T> Path getAbsolutePath(const T* path){
	return Path(path).toAbsolutePath();
}
template <class T> Path getParent(const T* path){
	return Path(path).toParent();
}
template <class T> std::vector<Path> getListDirectory(const T* path){
	std::vector<Path> result;
	T newPath[MAX_PATH];
	std::size_t i, length;
	for(i=0; path[i]; ++i){ newPath[i] = path[i]; }
	length = i;
	newPath[i] = 0;
	if(isDir(path)){
		#ifdef __unix__
	        // TODO
	    #elif defined(_WIN32) || defined(WIN32)
            HANDLE search;
            union WinData{
				WIN32_FIND_DATAA a;
				WIN32_FIND_DATAW w;
			} ffd;
			bool success, ansii = sizeof(T)==1;
	        newPath[length] = '/';
	        newPath[length+1] = '*';
	        newPath[length+2] = 0;
	        switch(sizeof(T)){
				case 1:
                    search = FindFirstFileA((char*)newPath, &ffd.a);
					break;
				case 2:
                    search = FindFirstFileW((wchar_t*)newPath, &ffd.w);
					break;
			}
			if(search != INVALID_HANDLE_VALUE){
				do{
					for(i=0; (ansii ? ffd.a.cFileName[i] : ffd.w.cFileName[i]); ++i){
						newPath[length+1+i] = (ansii ? ffd.a.cFileName[i] : ffd.w.cFileName[i]);
					}
					newPath[length+1+i] = 0;
					result.push_back(newPath);
					if(ansii){
						success = FindNextFileA(search, &ffd.a);
					}else{
                        success = FindNextFileW(search, &ffd.w);
					}
				}while(success);
				FindClose(search);
			}
		#endif
	}
	return result;
}

// Constructors
Path::Path() : path(0), length(0){ this->attr.exists = false; };
Path::Path(const Path& other) : path(0), length(other.length), name(other.name), ext(other.ext){
	this->path = new T_PATH[this->MAX_LENGTH];
	memcpy((void*) this->path, (void*) other.path, (other.length+1)*sizeof(T_PATH));
	memcpy((void*) &this->attr, (void*) &other.attr, sizeof(PathAttributes));
}
Path::Path(Path&& other) : path(other.path), length(other.length), name(other.name), ext(other.ext){
    memcpy((void*) &this->attr, (void*) &other.attr, sizeof(PathAttributes));
	other.path = 0;
	other.length = 0;
	other.attr.exists = false;
}
template <class T> Path::Path(const T* path) : path(0), length(0){
    std::size_t i;
    this->path = new T_PATH[this->MAX_LENGTH];
	for(i=0; path[i] && (i<(this->MAX_LENGTH-1)); ++i){ this->path[i] = path[i]; }
	this->path[i] = 0;
	this->length = i;
	this->parsePath();
	this->getPathAttributes();
}
// Destructor
Path::~Path(){
	if(this->path){ delete[] path; }
}

// Assignement
Path& Path::operator=(const Path& other){
	this->length = other.length;
	this->name = other.name;
	this->ext = other.ext;
	memcpy((void*) this->path, (void*) other.path, (other.length+1)*sizeof(T_PATH));
	memcpy((void*) &this->attr, (void*) &other.attr, sizeof(PathAttributes));
	return *this;
}
Path& Path::operator=(Path&& other){
	if(this->path){ delete[] path; }
	this->path = other.path;
	this->length = other.length;
	this->name = other.name;
	this->ext = other.ext;
	memcpy((void*) &this->attr, (void*) &other.attr, sizeof(PathAttributes));
	other.path = 0;
	other.length = 0;
	other.attr.exists = false;
	return *this;
}
template <class T> Path& Path::operator=(const T* path){
    std::size_t i;
    for(i=0; path[i] && (i<(this->MAX_LENGTH-1)); ++i){ this->path[i] = path[i]; }
	this->path[i] = 0;
	this->length = i;
	this->parsePath();
	this->getPathAttributes();
	return *this;
}
// Length
std::size_t Path::getLength() const{ return this->length; }
// Methods
bool Path::exists() const{ return this->attr.exists; }
bool Path::isDir() const{ return this->attr.exists && this->attr.isDirectory; }
bool Path::isFile() const{ return this->attr.exists && !this->attr.isDirectory; }
bool Path::isHidden() const{ return this->attr.exists && this->attr.isHidden; }
bool Path::isSystem() const{ return this->attr.exists && this->attr.isSystem; }
bool Path::isTemporary() const{ return this->attr.exists && this->attr.isTemporary; }
uint64_t Path::getSize() const{ return this->attr.exists ? this->attr.fileSize : 0; }
uint64_t Path::getCreationTime() const{ return this->attr.exists ? this->attr.creationTime : 0; }
uint64_t Path::getLastAccessTime() const{ return this->attr.exists ? this->attr.lastAccessTime : 0; }
uint64_t Path::getLastModifiedTime() const{ return this->attr.exists ? this->attr.lastModifyTime : 0; }
Path Path::getAbsolutePath() const{
	Path result(*this);
	return result.toAbsolutePath();
}
Path& Path::toAbsolutePath(){
    if(!this->exists()){ return *this; }
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
		T_PATH path[MAX_LENGTH];
		this->length = GetFullPathNameW(this->path,MAX_LENGTH,path,NULL);
		memcpy((void*) this->path, (void*) path, (this->length+1)*sizeof(T_PATH));
	#endif
	this->parsePath();
	return *this;
}
Path Path::getParent(bool* success) const{
	Path result(*this);
	return result.toParent(success);
}
Path& Path::toParent(bool* success){
	if(!this->exists()){ return *this; }
	#ifdef __unix__
        // TODO
    #elif defined(_WIN32) || defined(WIN32)
    	if(!this->name){
            if(this->length && this->path[this->length-1]!=':'){
				this->toAbsolutePath();
			}else{
				if(success){ *success = false; }
				return *this;
			}
		}
		if(this->name){
			this->length = this->name-1;
			this->path[this->length] = 0;
		}
	#endif
	this->parsePath();
	this->getPathAttributes();
	if(success){ *success = true; }
	return *this;
}
std::vector<Path> Path::getListDirectory() const{
	std::vector<Path> result;
	T_PATH newPath[MAX_PATH];
	std::size_t i;
	for(i=0; i<=this->length; ++i){ newPath[i] = this->path[i]; }
	if(this->isDir()){
		#ifdef __unix__
	        // TODO
	    #elif defined(_WIN32) || defined(WIN32)
            HANDLE search;
			WIN32_FIND_DATAW ffd;
			bool success;
	        newPath[this->length] = '/';
	        newPath[this->length+1] = '*';
	        newPath[this->length+2] = 0;
			search = FindFirstFileW(newPath, &ffd);
			if(search != INVALID_HANDLE_VALUE){
				do{
					for(i=0; ffd.cFileName[i]; ++i){
						newPath[this->length+1+i] = ffd.cFileName[i];
					}
					newPath[this->length+1+i] = 0;
					result.push_back(newPath);
					success = FindNextFileW(search, &ffd);
				}while(success);
				FindClose(search);
			}
		#endif
	}
	return result;
}
const Path::T_Path* Path::getPath() const{ return this->path; }
const Path::T_Path* Path::getName() const{ return this->path ? &(this->path[this->name]) : 0; }
const Path::T_Path* Path::getExtension() const{ return this->path ? &(this->path[this->ext]) : 0; }

// Private methods
void Path::parsePath(){
    std::size_t i;
	if(!this->length){ return; }
    #ifdef __unix__
        if(this->path[this->length - 1]=='/' && this->length>1){ this->path[--this->length] = 0; }
    #elif defined(_WIN32) || defined(WIN32)
		for(i=0; i<this->length; ++i){
			this->path[i] = ((this->path[i]=='\\') ? '/' : this->path[i]);
		}
		if(this->path[this->length - 1]=='/'){ this->path[--this->length] = 0; }
	#endif
	this->ext = 0;
	for(i = (this->length ? this->length-1 : 0); i && (this->path[i]!='/'); --i){
		if((this->path[i]=='.') && !this->ext){ this->ext = i+1; }
	}
	this->ext = (this->ext ? this->ext : this->length);
	this->name = (i ? i+1 : 0);
}
void Path::getPathAttributes(){
	#ifdef __unix__
		// TODO
	#elif defined(_WIN32) || defined(WIN32)
		winGetFileData(this->path, this->attr);
	#endif
}


#ifdef __unix__
	const std::size_t Path::MAX_LENGTH = 4096;
#elif defined(_WIN32) || defined(WIN32)
    const std::size_t Path::MAX_LENGTH = 256;
#endif

		}
	}
}

#endif
