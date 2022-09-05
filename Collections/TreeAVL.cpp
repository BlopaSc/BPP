#ifndef TREEAVL_CPP
#define TREEAVL_CPP
#include "TreeAVL.h"

namespace bpp{
	namespace map{

// Empty initialization
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(){
	this->root = 0;
	this->count = 0;
}

// Iterator initilization
template <class Key, class T, class Compare, class Allocator> template<class InputIt> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(InputIt first, InputIt last/*, const Compare& comp = Compare(), const Allocator& alloc = Allocator()*/){
	this->root = 0;
	this->count = 0;
	for(auto it = first; it != last; ++it){
		// TODO
	}
}

// Copy constructor
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(const TreeAVL& other){
	this->root = other.root ? NodeAVL::copy(this->alloc, other.root) : 0;
	this->count = other.count;
}

// Move constructor
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(TreeAVL&& other){
	this->alloc = other.alloc;
	this->cmp = other.cmp;
	this->root = other.root;
	this->count = other.count;
}

// Initializer list initialization
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(std::initializer_list<std::pair<const Key, T>> init/*, const Compare& comp = Compare(), const Allocator& alloc = Allocator()*/){
	this->root = 0;
	this->count = 0;
	
	// TODO
}

// Destructor
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::~TreeAVL(){
	if(this->root){ this->root->destroy(this->alloc); }
}

// Assignment
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>& TreeAVL<Key,T,Compare,Allocator>::operator=(const TreeAVL<Key,T,Compare,Allocator>& other){
	NodeAVL* nroot = other.root ? NodeAVL::copy(this->alloc, other.root) : 0;
	if(this->root){ this->root->destroy(this->alloc); }
	this->cmp = other.cmp;
	this->root = other.root;
	this->count = other.count;
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>& TreeAVL<Key,T,Compare,Allocator>::operator=(TreeAVL<Key,T,Compare,Allocator>&& other){
	if(this->root ^ other.root){ this->root->destroy(this->alloc); }
	this->alloc = other.alloc;
	this->cmp = other.cmp;
	this->root = other.root;
	this->count = other.count;
	return *this;
}

// Get allocator

// Element access
template <class Key, class T, class Compare, class Allocator> T& TreeAVL<Key,T,Compare,Allocator>::at(const Key& key){
	NodeAVL* tmp = this->root;
	while(tmp && tmp->key!=key){
		tmp = this->cmp(tmp->key, key) ? tmp->leftChild : tmp->rightChild;
	}
	if(!tmp){ throw std::out_of_range("bpp::map::TreeAVL::at"); }
	return tmp->value;
}
template <class Key, class T, class Compare, class Allocator> T& TreeAVL<Key,T,Compare,Allocator>::operator[](const Key& key){
	return get_forward(key)->value;
}
template <class Key, class T, class Compare, class Allocator> T& TreeAVL<Key,T,Compare,Allocator>::operator[](Key&& key){
	return get_forward(key)->value;
}

// Iterators

// Capacity
template <class Key, class T, class Compare, class Allocator> bool TreeAVL<Key,T,Compare,Allocator>::empty() const{
	return this->count == 0;
}
template <class Key, class T, class Compare, class Allocator> std::size_t TreeAVL<Key,T,Compare,Allocator>::size() const{
	return this->count;
}

// Nested class NodeAVL
template <class Key, class T, class Compare,class Allocator> void TreeAVL<Key,T,Compare,Allocator>::NodeAVL::create(const Key& key, NodeAVL* parent){
	this->parent = parent;
	this->leftChild = 0;
	this->rightChild = 0;
	this->key = key;
	this->height = 0;
}
template <class Key, class T, class Compare,class Allocator> void TreeAVL<Key,T,Compare,Allocator>::NodeAVL::create(Key&& key, NodeAVL* parent){
	this->parent = parent;
	this->leftChild = 0;
	this->rightChild = 0;
	this->key = key;
	this->height = 0;
}
template <class Key, class T, class Compare,class Allocator> void TreeAVL<Key,T,Compare,Allocator>::NodeAVL::destroy(AllocatorNodes& alloc){
	std::destroy_at<NodeAVL>(this);
	alloc.deallocate(this,1);
}
template <class Key, class T, class Compare,class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::NodeAVL::copy(AllocatorNodes& alloc, NodeAVL* src, NodeAVL* parent){
	NodeAVL* nw = alloc.allocate(1);
	nw->parent = parent;
	nw->key = src->key;
	nw->value = src->value;
	nw->height = src->height;
	nw->leftChild = src->leftChild ? copy(alloc, src->leftChild, nw) : 0;
	nw->rightChild = src->rightChild ? copy(alloc, src->rightChild, nw) : 0;
	return nw;
}
template <class Key, class T, class Compare,class Allocator> void TreeAVL<Key,T,Compare,Allocator>::NodeAVL::recalculate_height(){
	std::size_t lz = (this->leftChild ? this->leftChild->height : 0), rz = (this->rightChild ? this->rightChild->height : 0);
	this->height = (lz>rz ? lz : rz) + 1;
}
template <class Key, class T, class Compare,class Allocator> int TreeAVL<Key,T,Compare,Allocator>::NodeAVL::balance(){
	return (this->leftChild ? this->leftChild->height : 0) > (this->rightChild ? this->rightChild->height : 0);
}

// Private members
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::get_forward(const Key& key){
	NodeAVL *tmp = this->root, *parent;
	if(tmp){
		while(tmp && tmp->key!=key){
			parent = tmp;
			tmp = this->cmp(key, tmp->key) ? tmp->leftChild : tmp->rightChild;
		}
		if(!tmp){
			tmp = this->alloc.allocate(1);
			tmp->create(key, parent);
			*(this->cmp(key, parent->key) ? &(parent->leftChild) : &(parent->rightChild)) = tmp;
			rebalance(parent);
			this->count++;
		}
	}else{
		tmp = this->alloc.allocate(1);
		tmp->create(key);
		this->root = tmp;
		this->count++;
	}
	return tmp;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::get_forward(Key&& key){
	NodeAVL *tmp = this->root, *parent;
	if(tmp){
		while(tmp && tmp->key!=key){
			parent = tmp;
			tmp = this->cmp(key, tmp->key) ? tmp->leftChild : tmp->rightChild;
		}
		if(!tmp){
			tmp = this->alloc.allocate(1);
			tmp->create(key, parent);
			*(this->cmp(key, parent->key) ? &(parent->leftChild) : &(parent->rightChild)) = tmp;
			rebalance(parent);
			this->count++;
		}
	}else{
		tmp = this->alloc.allocate(1);
		tmp->create(key);
		this->root = tmp;
		this->count++;
	}
	return tmp;
}
template <class Key, class T, class Compare, class Allocator> void TreeAVL<Key,T,Compare,Allocator>::rebalance(NodeAVL *source){
	do{
		std::size_t lz = (source->leftChild ? source->leftChild->height : 0), rz = (source->rightChild ? source->rightChild->height : 0);
		std::size_t diff = lz>rz ? lz-rz : rz-lz;
		if(diff < 2){
			source->height = (lz>rz ? lz : rz) + 1;
			source = source->parent;
		}else{
			bool isLeftChild = source->parent && (source == source->parent->leftChild);
			NodeAVL* result = (lz>rz) ? (source->leftChild->balance() ? rotation_LL(source) : rotation_LR(source)) : ((source->rightChild->balance()) ? rotation_RL(source) : rotation_RR(source));
			if(result->parent){
				*(isLeftChild ? &(result->parent->leftChild) : &(result->parent->rightChild)) = result;
			}else{
				this->root = result;
			}
			source = result->parent;
		}
	}while(source);
}
// Tree rotations
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::rotation_LL(NodeAVL* source){
	NodeAVL* result = source->leftChild;
	result->parent = source->parent;
	source->leftChild = source->leftChild->rightChild;
	if(source->leftChild){ source->leftChild->parent = source; }
	source->recalculate_height();
	result->rightChild = source;
	result->rightChild->parent = result;
	result->recalculate_height();
	return result;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::rotation_RR(NodeAVL* source){
	NodeAVL* result = source->rightChild;
	result->parent = source->parent;
	source->rightChild = source->rightChild->leftChild;
	if(source->rightChild){ source->rightChild->parent = source; }
	source->recalculate_height();
	result->leftChild = source;
	result->leftChild->parent = result;
	result->recalculate_height();
	return result;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::rotation_LR(NodeAVL* source){
	source->leftChild = rotation_RR(source->leftChild);
	source->leftChild->parent = source;
	source->recalculate_height();
	return rotation_LL(source);
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::rotation_RL(NodeAVL* source){
	source->rightChild = rotation_LL(source->rightChild);
	source->rightChild->parent = source;
	source->recalculate_height();
	return rotation_RR(source);
}

	}
}

#endif
