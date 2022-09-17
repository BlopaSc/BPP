#ifndef TREEAVL_CPP
#define TREEAVL_CPP
#include "TreeAVL.h"

namespace bpp{
	namespace map{

// Empty initialization
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(){
	this->root = 0;
	this->counter = 0;
}

// Iterator initilization
template <class Key, class T, class Compare, class Allocator> template<class InputIt> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(InputIt first, InputIt last/*, const Compare& comp = Compare(), const Allocator& alloc = Allocator()*/){
	this->root = 0;
	this->counter = 0;
	for(auto it = first; it != last; ++it){
		this->get_forward(it->first)->data.second = it->second;
	}
}

// Copy constructor
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(const TreeAVL& other){
	this->root = other.root ? NodeAVL::copy(this->alloc, other.root) : 0;
	this->counter = other.counter;
}

// Move constructor
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(TreeAVL&& other){
	this->alloc = other.alloc;
	this->cmp = other.cmp;
	this->root = other.root;
	this->counter = other.counter;
}

// Initializer list initialization
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(std::initializer_list<std::pair<const Key, T>> ilist/*, const Compare& comp = Compare(), const Allocator& alloc = Allocator()*/){
	this->root = 0;
	this->counter = 0;
	for(auto it : ilist){
		this->get_forward(it.first)->data.second = it.second;
	}
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
	this->counter = other.counter;
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>& TreeAVL<Key,T,Compare,Allocator>::operator=(TreeAVL<Key,T,Compare,Allocator>&& other) noexcept{
	if(this->root ^ other.root){ this->root->destroy(this->alloc); }
	this->alloc = other.alloc;
	this->cmp = other.cmp;
	this->root = other.root;
	this->counter = other.counter;
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>& TreeAVL<Key,T,Compare,Allocator>::operator=(std::initializer_list<std::pair<const Key, T>> ilist){
	this->clear();
	for(auto it : ilist){
		this->get_forward(it.first)->data.second = it.second;
	}
	return *this;
}

// Get allocator

// Element access
template <class Key, class T, class Compare, class Allocator> const T& TreeAVL<Key,T,Compare,Allocator>::at(const Key& key) const{
	NodeAVL* tmp = this->root;
	while(tmp && tmp->data.first!=key){
		tmp = this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild;
	}
	if(!tmp){ throw std::out_of_range("bpp::map::TreeAVL::at"); }
	return tmp->data.second;
}
template <class Key, class T, class Compare, class Allocator> T& TreeAVL<Key,T,Compare,Allocator>::operator[](const Key& key){
	return this->get_forward(key)->data.second;
}
template <class Key, class T, class Compare, class Allocator> T& TreeAVL<Key,T,Compare,Allocator>::operator[](Key&& key){
	return this->get_forward(key)->data.second;
}

// Iterators
// Iterator
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator::iterator(NodeAVL* init){
	this->current = init;
}
template <class Key, class T, class Compare, class Allocator> std::pair<Key, T>& TreeAVL<Key,T,Compare,Allocator>::iterator::operator*() const{
	return this->current ? this->current->data : this->nullvalue;
}
template <class Key, class T, class Compare, class Allocator> std::pair<Key, T>* TreeAVL<Key,T,Compare,Allocator>::iterator::operator->() const{
	return this->current ? &this->current->data : 0;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator& TreeAVL<Key,T,Compare,Allocator>::iterator::operator++(){
	if(this->current){
		if(this->current->rightChild){
			this->current = this->current->rightChild;
			while(this->current->leftChild){ this->current = this->current->leftChild; }
		}else{
			while(this->current->parent && (this->current->parent->rightChild == this->current)){ this->current = this->current->parent; }
			this->current = this->current->parent;
		}
	}
	return *this;
}
// Reverse Iterator
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::reverse_iterator(NodeAVL* init){
	this->current = init;
}
template <class Key, class T, class Compare, class Allocator> std::pair<Key, T>& TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::operator*() const{
	return this->current ? this->current->data : std::pair<Key, T>();
}
template <class Key, class T, class Compare, class Allocator> std::pair<Key, T>* TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::operator->() const{
	return this->current ? &this->current->data : 0;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::reverse_iterator& TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::operator++(){
	if(this->current){
		if(this->current->leftChild){
			this->current = this->current->leftChild;
			while(this->current->rightChild){ this->current = this->current->rightChild; }
		}else{
			while(this->current->parent && (this->current->parent->leftChild == this->current)){ this->current = this->current->parent; }
			this->current = this->current->parent;
		}
	}
	return *this;
}

// Iterator generation methods
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::begin() const noexcept{
	NodeAVL* tmp = this->root;
	while(tmp && tmp->leftChild){ tmp = tmp->leftChild; }
	return iterator(tmp);
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::end() const noexcept{
	return iterator();
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::reverse_iterator TreeAVL<Key,T,Compare,Allocator>::rbegin() const noexcept{
	NodeAVL* tmp = this->root;
	while(tmp && tmp->rightChild){ tmp = tmp->rightChild; }
	return reverse_iterator(tmp);
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::reverse_iterator TreeAVL<Key,T,Compare,Allocator>::rend() const noexcept{
	return reverse_iterator();
}

// Capacity
template <class Key, class T, class Compare, class Allocator> bool TreeAVL<Key,T,Compare,Allocator>::empty() const noexcept{
	return this->counter == 0;
}
template <class Key, class T, class Compare, class Allocator> std::size_t TreeAVL<Key,T,Compare,Allocator>::size() const noexcept{
	return this->counter;
}

// Modifiers
// Clear
template <class Key, class T, class Compare, class Allocator> void TreeAVL<Key,T,Compare,Allocator>::clear() noexcept{
	if(this->root){
		this->root->destroy(this->alloc);
		this->root = 0;
		this->counter = 0;
	}
}
// Insert
template <class Key, class T, class Compare, class Allocator> std::pair<typename TreeAVL<Key,T,Compare,Allocator>::iterator, bool> TreeAVL<Key,T,Compare,Allocator>::insert(const std::pair<const Key, T>& value){
	std::size_t pcounter = this->counter;
	NodeAVL* tmp = this->get_forward(value.first);
	if(pcounter != this->counter){ tmp->data.second = value.second; }
	return std::pair<iterator, bool>(iterator(tmp), pcounter != this->counter);
}
template <class Key, class T, class Compare, class Allocator> std::pair<typename TreeAVL<Key,T,Compare,Allocator>::iterator, bool> TreeAVL<Key,T,Compare,Allocator>::insert(std::pair<const Key, T>&& value){
	std::size_t pcounter = this->counter;
	NodeAVL* tmp = this->get_forward(value.first);
	if(pcounter != this->counter){ tmp->data.second = value.second; }
	return std::pair<iterator, bool>(iterator(tmp), pcounter != this->counter);
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::insert(TreeAVL<Key,T,Compare,Allocator>::iterator_base hint, const std::pair<const Key, T>& value){
	std::size_t pcounter = this->counter;
	NodeAVL* tmp = this->get_forward(value.first, hint.current);
	if(pcounter != this->counter){ tmp->data.second = value.second; }
	return iterator(tmp);
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::insert(TreeAVL<Key,T,Compare,Allocator>::iterator_base hint, std::pair<const Key, T>&& value){
	std::size_t pcounter = this->counter;
	NodeAVL* tmp = this->get_forward(value.first, hint.current);
	if(pcounter != this->counter){ tmp->data.second = value.second; }
	return iterator(tmp);
}
template <class Key, class T, class Compare, class Allocator> template<class InputIt> void TreeAVL<Key,T,Compare,Allocator>::insert(InputIt first, InputIt last){
	for(auto it = first; it != last; ++it){
		std::size_t pcounter = this->counter;
		NodeAVL* tmp = this->get_forward(it->first);
		if(pcounter != this->counter){ tmp->data.second = it->second; }
	}
}
template <class Key, class T, class Compare, class Allocator> void TreeAVL<Key,T,Compare,Allocator>::insert(std::initializer_list<std::pair<const Key, T>> ilist){
	for(auto it : ilist){
		std::size_t pcounter = this->counter;
		NodeAVL* tmp = this->get_forward(it.first);
		if(pcounter != this->counter){ tmp->data.second = it.second; }
	}
}
// Insert or Assign
template <class Key, class T, class Compare, class Allocator> template <class M> std::pair<typename TreeAVL<Key,T,Compare,Allocator>::iterator, bool> TreeAVL<Key,T,Compare,Allocator>::insert_or_assign(const Key& k, M&& obj){
	std::size_t pcounter = this->counter;
	NodeAVL* tmp = this->get_forward(k);
	tmp->data.second = obj;
	return std::pair<iterator, bool>(iterator(tmp), pcounter != this->counter);
}
template <class Key, class T, class Compare, class Allocator> template <class M> std::pair<typename TreeAVL<Key,T,Compare,Allocator>::iterator, bool> TreeAVL<Key,T,Compare,Allocator>::insert_or_assign(Key&& k, M&& obj){
	std::size_t pcounter = this->counter;
	NodeAVL* tmp = this->get_forward(k);
	tmp->data.second = obj;
	return std::pair<iterator, bool>(iterator(tmp), pcounter != this->counter);
}
template <class Key, class T, class Compare, class Allocator> template <class M> std::pair<typename TreeAVL<Key,T,Compare,Allocator>::iterator, bool> TreeAVL<Key,T,Compare,Allocator>::insert_or_assign(TreeAVL<Key,T,Compare,Allocator>::iterator_base hint, const Key& k, M&& obj){
	std::size_t pcounter = this->counter;
	NodeAVL* tmp = this->get_forward(k, hint.current);
	tmp->data.second = obj;
	return std::pair<iterator, bool>(iterator(tmp), pcounter != this->counter);
}
template <class Key, class T, class Compare, class Allocator> template <class M> std::pair<typename TreeAVL<Key,T,Compare,Allocator>::iterator, bool> TreeAVL<Key,T,Compare,Allocator>::insert_or_assign(TreeAVL<Key,T,Compare,Allocator>::iterator_base hint, Key&& k, M&& obj){
	std::size_t pcounter = this->counter;
	NodeAVL* tmp = this->get_forward(k, hint.current);
	tmp->data.second = obj;
	return std::pair<iterator, bool>(iterator(tmp), pcounter != this->counter);
}
// Erase
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::erase(iterator pos){
	NodeAVL *tmp = pos.current;
	const Key k = pos.current->data.first;
	tmp = this->remove_node(tmp);
	iterator it = iterator(tmp);
	if(tmp && this->cmp(tmp->data.first, k)){ ++it; }
	return it;
}
template <class Key, class T, class Compare, class Allocator> std::size_t TreeAVL<Key,T,Compare,Allocator>::erase(const Key& key){
	NodeAVL *tmp = this->root;
	while(tmp && tmp->data.first!=key){ tmp = this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild; }
	this->remove_node(tmp);
	return tmp ? 1 : 0;
}
template <class Key, class T, class Compare, class Allocator> template<class K> std::size_t TreeAVL<Key,T,Compare,Allocator>::erase(K&& key){
	NodeAVL *tmp = this->root;
	while(tmp && tmp->data.first!=key){ tmp = this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild; }
	this->remove_node(tmp);
	return tmp ? 1 : 0;
}

// Lookup
// Count
template <class Key, class T, class Compare, class Allocator> template<class K> std::size_t TreeAVL<Key,T,Compare,Allocator>::count(const K& key) const{
	NodeAVL* tmp = this->root;
	while(tmp && tmp->data.first!=key){ tmp = this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild; }
	return tmp ? 1 : 0;
}
// Find
template <class Key, class T, class Compare, class Allocator> template<class K> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::find(const K& key){
	NodeAVL* tmp = this->root;
	while(tmp && tmp->data.first!=key){ tmp = (this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild); }
	return iterator(tmp);
}
// Contains
template <class Key, class T, class Compare, class Allocator> template<class K> bool TreeAVL<Key,T,Compare,Allocator>::contains(const K& key) const{
	NodeAVL* tmp = this->root;
	while(tmp && tmp->data.first!=key){ tmp = this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild; }
	return tmp;
}

// Bounds
template <class Key, class T, class Compare, class Allocator> template<class K> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::lower_bound(const K& key){
	NodeAVL *tmp = this->root, *prev = 0;
	while(tmp && tmp->data.first!=key){
		prev = tmp;
		tmp = (this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild);
	}
	iterator it(tmp ? tmp : prev);
	if(it.current && this->cmp(it.current->data.first, key)){ ++it; }
	return it;
}
template <class Key, class T, class Compare, class Allocator> template<class K> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::upper_bound(const K& key){
	NodeAVL *tmp = this->root, *prev = 0;
	while(tmp && tmp->data.first!=key){
		prev = tmp;
		tmp = (this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild);
	}
	iterator it(tmp ? tmp : prev);
	if(it.current && (it.current->data.first==key || this->cmp(it.current->data.first, key))){ ++it; }
	return it;
}
template <class Key, class T, class Compare, class Allocator> template<class K> std::pair<typename TreeAVL<Key,T,Compare,Allocator>::iterator, typename TreeAVL<Key,T,Compare,Allocator>::iterator> TreeAVL<Key,T,Compare,Allocator>::equal_range(const K& key){
	return std::pair<iterator,iterator>(this->lower_bound(key), this->upper_bound(key));
}

// Non-member functions
// Operators
template<class A,class B,class C,class D> bool operator==(const TreeAVL<A,B,C,D>& lhs, const TreeAVL<A,B,C,D>& rhs){
	bool result;
	if(result = (lhs.size() == rhs.size())){
		auto it = lhs.begin(), ito = rhs.begin(), ite = lhs.end();
		while(it != ite){
			if((*it) != (*ito)){ return false; }
			++it;
			++ito;
		}
	}
	return result;
}
template<class A,class B,class C,class D> std::strong_ordering operator<=>(const TreeAVL<A,B,C,D>& lhs, const TreeAVL<A,B,C,D>& rhs){
	auto it = lhs.begin(), ito = rhs.begin(), ite = lhs.end(), itoe = rhs.end();
	while(it != ite && ito != itoe){
		if((*it) != (*ito)){ break; }
		++it;
		++ito;
	}
	if(it == ite && ito == itoe){ return std::strong_ordering::equal; }
	if(it == ite){ return std::strong_ordering::less; }
	if(ito == itoe || (*ito) < (*it)){ return std::strong_ordering::greater; }
	return std::strong_ordering::less;
}
// Other
template<class A,class B,class C,class D, class Pred> std::size_t erase_if(TreeAVL<A,B,C,D>& tree, Pred pred){
	auto original = tree.size();
	for (auto i = tree.begin(), last = tree.end(); i != last; ){
		if(pred(*i)){
			i = tree.erase(i);
		}else{
			++i;
		}
	}
	return original - tree.size();
}

// Nested class NodeAVL
template <class Key, class T, class Compare,class Allocator> void TreeAVL<Key,T,Compare,Allocator>::NodeAVL::create(const Key& key, NodeAVL* parent){
	this->parent = parent;
	this->leftChild = 0;
	this->rightChild = 0;
	this->data.first = key;
	this->height = 0;
}
template <class Key, class T, class Compare,class Allocator> void TreeAVL<Key,T,Compare,Allocator>::NodeAVL::create(Key&& key, NodeAVL* parent){
	this->parent = parent;
	this->leftChild = 0;
	this->rightChild = 0;
	this->data.first = key;
	this->height = 0;
}
template <class Key, class T, class Compare,class Allocator> void TreeAVL<Key,T,Compare,Allocator>::NodeAVL::destroy(AllocatorNodes& alloc){
	if(this->leftChild){ this->leftChild->destroy(alloc); }
	if(this->rightChild){ this->rightChild->destroy(alloc); }
	std::destroy_at<NodeAVL>(this);
	alloc.deallocate(this,1);
}
template <class Key, class T, class Compare,class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::NodeAVL::copy(AllocatorNodes& alloc, NodeAVL* src, NodeAVL* parent){
	NodeAVL* nw = alloc.allocate(1);
	nw->parent = parent;
	nw->data = src->data;
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
		while(tmp && tmp->data.first!=key){
			parent = tmp;
			tmp = this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild;
		}
		if(!tmp){
			tmp = this->alloc.allocate(1);
			tmp->create(key, parent);
			*(this->cmp(key, parent->data.first) ? &(parent->leftChild) : &(parent->rightChild)) = tmp;
			this->rebalance(parent);
			this->counter++;
		}
	}else{
		tmp = this->alloc.allocate(1);
		tmp->create(key);
		this->root = tmp;
		this->counter++;
	}
	return tmp;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::get_forward(Key&& key){
	NodeAVL *tmp = this->root, *parent;
	if(tmp){
		while(tmp && tmp->data.first!=key){
			parent = tmp;
			tmp = this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild;
		}
		if(!tmp){
			tmp = this->alloc.allocate(1);
			tmp->create(key, parent);
			*(this->cmp(key, parent->data.first) ? &(parent->leftChild) : &(parent->rightChild)) = tmp;
			this->rebalance(parent);
			this->counter++;
		}
	}else{
		tmp = this->alloc.allocate(1);
		tmp->create(key);
		this->root = tmp;
		this->counter++;
	}
	return tmp;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::get_forward(const Key& key, NodeAVL* hint){
	NodeAVL *tmp = hint ? hint : this->root, *parent;
	if(tmp){
		while(tmp->parent && ((tmp->parent->leftChild==tmp) ^ this->cmp(key, tmp->parent->data.first))){ tmp = tmp->parent; }
		while(tmp && tmp->data.first!=key){
			parent = tmp;
			tmp = this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild;
		}
		if(!tmp){
			tmp = this->alloc.allocate(1);
			tmp->create(key, parent);
			*(this->cmp(key, parent->data.first) ? &(parent->leftChild) : &(parent->rightChild)) = tmp;
			this->rebalance(parent);
			this->counter++;
		}
	}else{
		tmp = this->alloc.allocate(1);
		tmp->create(key);
		this->root = tmp;
		this->counter++;
	}
	return tmp;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::get_forward(Key&& key, NodeAVL* hint){
	NodeAVL *tmp = hint ? hint : this->root, *parent;
	if(tmp){
		while(tmp->parent && ((tmp->parent->leftChild==tmp) ^ this->cmp(key, tmp->parent->key))){ tmp = tmp->parent; }
		while(tmp && tmp->data.first!=key){
			parent = tmp;
			tmp = this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild;
		}
		if(!tmp){
			tmp = this->alloc.allocate(1);
			tmp->create(key, parent);
			*(this->cmp(key, parent->data.first) ? &(parent->leftChild) : &(parent->rightChild)) = tmp;
			this->rebalance(parent);
			this->counter++;
		}
	}else{
		tmp = this->alloc.allocate(1);
		tmp->create(key);
		this->root = tmp;
		this->counter++;
	}
	return tmp;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::remove_node(TreeAVL<Key,T,Compare,Allocator>::NodeAVL* src){
	NodeAVL *tmp = src, *result = src;
	bool leftChild;
	if(src){
		this->counter--;
		if(tmp->rightChild){
			tmp = tmp->rightChild;
			leftChild = tmp->leftChild;
			while(tmp->leftChild){ tmp = tmp->leftChild; }
			*(leftChild ? &(tmp->parent->leftChild):&(tmp->parent->rightChild)) = tmp->rightChild;
			if(tmp->rightChild){
				tmp->rightChild->parent = tmp->parent;
				tmp->rightChild = 0;
			}
			src->data = tmp->data;
		}else if(tmp->leftChild){
			tmp = tmp->leftChild;
			src->data = tmp->data;
			tmp->parent->leftChild = 0;
		}else{
			if(result = tmp->parent){
				*((tmp->parent->leftChild == tmp) ? &(tmp->parent->leftChild):&(tmp->parent->rightChild)) = 0;
			}else{
				this->root = 0;
			}
		}
		if(tmp->parent){ this->rebalance(tmp->parent); }
		tmp->destroy(this->alloc);
	}
	return result;
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

// Base iterator comparison
template <class Key, class T, class Compare, class Allocator> bool TreeAVL<Key,T,Compare,Allocator>::iterator_base::operator==(const TreeAVL<Key,T,Compare,Allocator>::iterator_base& other) const{
	return this->current == other.current;
}
template <class Key, class T, class Compare, class Allocator> bool TreeAVL<Key,T,Compare,Allocator>::iterator_base::operator!=(const TreeAVL<Key,T,Compare,Allocator>::iterator_base& other) const{
	return this->current != other.current;
}

template <class Key, class T, class Compare, class Allocator> std::pair<Key, T> TreeAVL<Key,T,Compare,Allocator>::iterator_base::nullvalue;

	}
}

#endif
