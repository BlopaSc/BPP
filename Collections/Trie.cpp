#ifndef BPP_COLLECTIONS_MAP_TRIE_CPP
#define BPP_COLLECTIONS_MAP_TRIE_CPP

namespace bpp{
	namespace collections{
		namespace map{

// Constructors
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::Trie() :
	root(&data),
	counter(0),
	nCounter(0){ }
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::Trie(const Allocator& alloc) :
	root(&data),
	counter(0),
	nCounter(0),
	alloc(alloc){ }
template <class Key, class T, class Container, class Allocator> template<class InputIt> Trie<Key,T,Container,Allocator>::Trie(InputIt first, InputIt last, const Allocator& alloc) :
	root(&data),
	counter(0),
	nCounter(0),
	alloc(alloc){
	for(auto it = first; it != last; ++it){
		this->get_forward(it->first)->data.second = it->second;
	}
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::Trie(const Trie& other, const Allocator& alloc) :
	root(&data),
	counter(other.counter),
	nCounter(other.nCounter),
	alloc(alloc){
	NodeTrie::copy(this->alloc, other.root, this->root);
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::Trie(Trie&& other) :
	root(&data),
	counter(other.counter),
	nCounter(other.nCounter),
	alloc(std::move(other.alloc)),
	data(std::move(other.data)){
	other.counter = 0;
	other.nCounter = 0;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::Trie(Trie&& other, const Allocator& alloc) :
	root(&data),
	counter(other.counter),
	nCounter(other.nCounter),
	alloc(alloc){
	if(this->alloc == other.alloc){
		this->data = std::move(other.data);
		other.counter = 0;
		other.nCounter = 0;
	}else{
		NodeTrie::copy(this->alloc, other.root, this->root);
		other.clear();
	}
}
template <class Key, class T, class Container, class Allocator> template<class Vector> Trie<Key,T,Container,Allocator>::Trie(std::initializer_list<std::pair<const Vector, T>> ilist, const Allocator& alloc) :
	root(&data),
	counter(0),
	nCounter(0),
	alloc(alloc){
	for(auto it : ilist){
		this->get_forward(it.first)->data.second = it.second;
	}
}

// Destructor
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::~Trie(){
	for(auto it = this->data.children.begin(); it != this->data.children.end(); ++it){
		NodeTrie::destroy(this->alloc, (NodeTrie*) it->second);
	}
}

// Assignment
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>& Trie<Key,T,Container,Allocator>::operator=(const Trie& other){
	this->sp_copy(other, typename std::allocator_traits<Allocator>::propagate_on_container_copy_assignment());
	return *this;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>& Trie<Key,T,Container,Allocator>::operator=(Trie&& other){
	this->sp_move(std::move(other), typename std::allocator_traits<Allocator>::propagate_on_container_move_assignment());
	return *this;
}
template <class Key, class T, class Container, class Allocator> template <class Vector> Trie<Key,T,Container,Allocator>& Trie<Key,T,Container,Allocator>::operator=(std::initializer_list<std::pair<const Vector, T>> ilist){
	this->clear();
	for(auto it : ilist){
		this->get_forward(it.first)->data.second = it.second;
	}
	return *this;
}

// Get allocator
template <class Key, class T, class Container, class Allocator> Allocator Trie<Key,T,Container,Allocator>::get_allocator() const noexcept{ return this->alloc; }

// Element access
template <class Key, class T, class Container, class Allocator> template<class Vector> const T& Trie<Key,T,Container,Allocator>::at(const Vector& key) const{
	NodeTrie* ptr = this->find_node(key, len_array(key));
	if(!ptr || !ptr->valid){ throw std::out_of_range("bpp::collections::map::Trie::at"); }
	return ptr->data.second;
}
template <class Key, class T, class Container, class Allocator> template<class Vector> T& Trie<Key,T,Container,Allocator>::operator[](const Vector& key){
	return get_forward(key, len_array(key))->data.second;
}
template <class Key, class T, class Container, class Allocator> template<class Vector> T& Trie<Key,T,Container,Allocator>::operator[](Vector&& key){
	return get_forward(std::move(key), len_array(key))->data.second;
}

// Iterators
// Iterator
template <class Key, class T, class Container, class Allocator> std::pair<const std::vector<Key>&, T&>& Trie<Key,T,Container,Allocator>::iterator::operator*(){
	return this->data;
}
template <class Key, class T, class Container, class Allocator> std::pair<const std::vector<Key>&, T&>* Trie<Key,T,Container,Allocator>::iterator::operator->(){
	return &this->data;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator& Trie<Key,T,Container,Allocator>::iterator::operator++(){
	this->next_element();
	return *this;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::iterator::operator++(int){
	iterator pit(this->current);
	this->next_element();
	return pit;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator& Trie<Key,T,Container,Allocator>::iterator::operator--(){
	this->previous_element();
	return *this;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::iterator::operator--(int){
	iterator pit(this->current);
	this->previous_element();
	return pit;
}
// Constant Iterator
template <class Key, class T, class Container, class Allocator> const std::pair<const std::vector<Key>&, const T&>& Trie<Key,T,Container,Allocator>::const_iterator::operator*() const{
	return this->data;
}
template <class Key, class T, class Container, class Allocator> const std::pair<const std::vector<Key>&, const T&>* Trie<Key,T,Container,Allocator>::const_iterator::operator->() const{
	return (const std::pair<const std::vector<Key>&, const T&>*)&this->data;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_iterator& Trie<Key,T,Container,Allocator>::const_iterator::operator++(){
	this->next_element();
	return *this;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_iterator Trie<Key,T,Container,Allocator>::const_iterator::operator++(int){
	const_iterator pit(this->current);
	this->next_element();
	return pit;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_iterator& Trie<Key,T,Container,Allocator>::const_iterator::operator--(){
	this->previous_element();
	return *this;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_iterator Trie<Key,T,Container,Allocator>::const_iterator::operator--(int){
	const_iterator pit(this->current);
	this->previous_element();
	return pit;
}
// Reverse Iterator
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::reverse_iterator::base() const{ return iterator(*this); }
template <class Key, class T, class Container, class Allocator> std::pair<const std::vector<Key>&, T&>& Trie<Key,T,Container,Allocator>::reverse_iterator::operator*(){
	return this->data;
}
template <class Key, class T, class Container, class Allocator> std::pair<const std::vector<Key>&, T&>* Trie<Key,T,Container,Allocator>::reverse_iterator::operator->(){
	return &this->data;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::reverse_iterator& Trie<Key,T,Container,Allocator>::reverse_iterator::operator++(){
	this->previous_element();
	return *this;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::reverse_iterator Trie<Key,T,Container,Allocator>::reverse_iterator::operator++(int){
	reverse_iterator pit(this->current);
	this->previous_element();
	return pit;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::reverse_iterator& Trie<Key,T,Container,Allocator>::reverse_iterator::operator--(){
	this->next_element();
	return *this;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::reverse_iterator Trie<Key,T,Container,Allocator>::reverse_iterator::operator--(int){
	reverse_iterator pit(this->current);
	this->next_element();
	return pit;
}
// Constant Reverse Iterator
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_iterator Trie<Key,T,Container,Allocator>::const_reverse_iterator::base() const{ return const_iterator(*this); }
template <class Key, class T, class Container, class Allocator> const std::pair<const std::vector<Key>&, const T&>& Trie<Key,T,Container,Allocator>::const_reverse_iterator::operator*() const{
	return this->data;
}
template <class Key, class T, class Container, class Allocator> const std::pair<const std::vector<Key>&, const T&>* Trie<Key,T,Container,Allocator>::const_reverse_iterator::operator->() const{
	return (const std::pair<const std::vector<Key>&, const T&>*)&this->data;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_reverse_iterator& Trie<Key,T,Container,Allocator>::const_reverse_iterator::operator++(){
	this->previous_element();
	return *this;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_reverse_iterator Trie<Key,T,Container,Allocator>::const_reverse_iterator::operator++(int){
	const_reverse_iterator pit(this->current);
	this->previous_element();
	return pit;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_reverse_iterator& Trie<Key,T,Container,Allocator>::const_reverse_iterator::operator--(){
	this->next_element();
	return *this;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_reverse_iterator Trie<Key,T,Container,Allocator>::const_reverse_iterator::operator--(int){
	const_reverse_iterator pit(this->current);
	this->next_element();
	return pit;
}

// Iterator generation methods
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::begin() const{
	NodeTrie* tmp = 0;
	iterator result;
	if(this->counter){
		tmp = this->root;
		while(!tmp->valid){
			tmp = (NodeTrie*) tmp->children.begin()->second;
			result.key.push_back(tmp->data.first);
		}
	}
	result.update(tmp);
	return result;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_iterator Trie<Key,T,Container,Allocator>::cbegin() const{
	NodeTrie* tmp = 0;
	const_iterator result;
	if(this->counter){
		tmp = this->root;
		while(!tmp->valid){
			tmp = (NodeTrie*) tmp->children.begin()->second;
			result.key.push_back(tmp->data.first);
		}
	}
	result.update(tmp);
	return result;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::end() const noexcept{
	return iterator();
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_iterator Trie<Key,T,Container,Allocator>::cend() const noexcept{
	return const_iterator();
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::reverse_iterator Trie<Key,T,Container,Allocator>::rbegin() const{
	NodeTrie* tmp = 0;
	reverse_iterator result;
	if(this->counter){
		tmp = this->root;
		while(tmp->children.size()){
			tmp = (NodeTrie*) tmp->children.rbegin()->second;
			result.key.push_back(tmp->data.first);
		}
	}
	result.update(tmp);
	return result;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_reverse_iterator Trie<Key,T,Container,Allocator>::crbegin() const{
	NodeTrie* tmp = 0;
	const_reverse_iterator result;
	if(this->counter){
		tmp = this->root;
		while(tmp->children.size()){
			tmp = (NodeTrie*) tmp->children.rbegin()->second;
			result.key.push_back(tmp->data.first);
		}
	}
	result.update(tmp);
	return result;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::reverse_iterator Trie<Key,T,Container,Allocator>::rend() const noexcept{
	return reverse_iterator();
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::const_reverse_iterator Trie<Key,T,Container,Allocator>::crend() const noexcept{
	return const_reverse_iterator();
}

// Capacity
template <class Key, class T, class Container, class Allocator> bool Trie<Key,T,Container,Allocator>::empty() const noexcept{
	return this->counter != 0;
}
template <class Key, class T, class Container, class Allocator> std::size_t Trie<Key,T,Container,Allocator>::size() const noexcept{
	return this->counter;
}
template <class Key, class T, class Container, class Allocator> std::size_t Trie<Key,T,Container,Allocator>::memory() const noexcept{
	return this->nCounter * (sizeof(typename Container::node_type) + sizeof(NodeTrie)) + sizeof(Trie);
}

// Modifiers
// Clear
template <class Key, class T, class Container, class Allocator> void Trie<Key,T,Container,Allocator>::clear() noexcept{
	for(auto it = this->data.children.begin(); it != this->data.children.end(); ++it){
		NodeTrie::destroy(this->alloc, (NodeTrie*) it->second);
	}
	this->data.children.clear();
	this->counter = 0;
	this->nCounter = 0;
}
// Inserts
template <class Key, class T, class Container, class Allocator> template <class Vector> std::pair<typename Trie<Key,T,Container,Allocator>::iterator, bool> Trie<Key,T,Container,Allocator>::insert(const std::pair<Vector, T>& value){
	std::size_t pcounter = this->counter, size = len_array(value.first);
	NodeTrie* tmp = this->get_forward(value.first, size);
	if(pcounter != this->counter){ tmp->data.second = value.second; }
	return std::pair<iterator, bool>(iterator(tmp,size), pcounter != this->counter);
}
template <class Key, class T, class Container, class Allocator> template <class Vector> std::pair<typename Trie<Key,T,Container,Allocator>::iterator, bool> Trie<Key,T,Container,Allocator>::insert(std::pair<Vector, T>&& value){
	std::size_t pcounter = this->counter, size = len_array(value.first);
	NodeTrie* tmp = this->get_forward(std::move(value.first), size);
	if(pcounter != this->counter){ tmp->data.second = std::move(value.second); }
	return std::pair<iterator, bool>(iterator(tmp,size), pcounter != this->counter);
}
template <class Key, class T, class Container, class Allocator> template <class Vector> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::insert(iterator hint, const std::pair<Vector, T>& value){
	NodeTrie* tmp = hint.current;
	std::size_t pcounter = this->counter, size_input = len_array(value.first), size_hint = len_array(hint.key), equal = 0, size;
	size = size_input <= size_hint ? size_input : size_hint;
	while(equal < size && hint.key[equal]==value.first[equal]){ ++equal; }
	for(std::size_t i = equal; i < size_hint; ++i){ tmp = tmp->parent; }
	tmp = this->get_forward(value.first, size_input, tmp, equal);
	if(pcounter != this->counter){ tmp->data.second = value.second; }
	return iterator(tmp,size);
}
template <class Key, class T, class Container, class Allocator> template <class Vector> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::insert(iterator hint, std::pair<Vector, T>&& value){
	NodeTrie* tmp = hint.current;
	std::size_t pcounter = this->counter, size_input = len_array(value.first), size_hint = len_array(hint.key), equal = 0, size;
	size = size_input <= size_hint ? size_input : size_hint;
	while(equal < size && hint.key[equal]==value.first[equal]){ ++equal; }
	for(std::size_t i = equal; i < size_hint; ++i){ tmp = tmp->parent; }
	tmp = this->get_forward(std::move(value.first), size_input, tmp, equal);
	if(pcounter != this->counter){ tmp->data.second = std::move(value.second); }
	return iterator(tmp,size);
}
template <class Key, class T, class Container, class Allocator> template <class InputIt> void Trie<Key,T,Container,Allocator>::insert(InputIt first, InputIt last){
	for(auto it = first; it != last; ++it){
		std::size_t pcounter = this->counter, size = len_array(it->first);
		NodeTrie* tmp = this->get_forward(it->first, size);
		if(pcounter != this->counter){ tmp->data.second = it->second; }
	}
}
template <class Key, class T, class Container, class Allocator> template <class Vector> void Trie<Key,T,Container,Allocator>::insert(std::initializer_list<std::pair<Vector, T>> ilist){
	for(auto it : ilist){
		std::size_t pcounter = this->counter, size = len_array(it.first);
		NodeTrie* tmp = this->get_forward(it.first, size);
		if(pcounter != this->counter){ tmp->data.second = it.second; }
	}
}
// Insert or Assign
template <class Key, class T, class Container, class Allocator> template <class Vector, class M> std::pair<typename Trie<Key,T,Container,Allocator>::iterator, bool> Trie<Key,T,Container,Allocator>::insert_or_assign(const Vector& k, M&& obj){
	std::size_t pcounter = this->counter, size = len_array(k);
	NodeTrie* tmp = this->get_forward(k, size);
	tmp->data.second = std::move(obj);
	return std::pair<iterator, bool>(iterator(tmp,size), pcounter != this->counter);
}
template <class Key, class T, class Container, class Allocator> template <class Vector, class M> std::pair<typename Trie<Key,T,Container,Allocator>::iterator, bool> Trie<Key,T,Container,Allocator>::insert_or_assign(Vector&& k, M&& obj){
	std::size_t pcounter = this->counter, size = len_array(k);
	NodeTrie* tmp = this->get_forward(std::move(k), size);
	tmp->data.second = std::move(obj);
	return std::pair<iterator, bool>(iterator(tmp,size), pcounter != this->counter);
}
template <class Key, class T, class Container, class Allocator> template <class Vector, class M> std::pair<typename Trie<Key,T,Container,Allocator>::iterator, bool> Trie<Key,T,Container,Allocator>::insert_or_assign(iterator hint, const Vector& k, M&& obj){
	NodeTrie* tmp = hint.current;
	std::size_t pcounter = this->counter, size_input = len_array(k), size_hint = len_array(hint.key), equal = 0, size;
	size = size_input <= size_hint ? size_input : size_hint;
	while(equal < size && hint.key[equal]==k[equal]){ ++equal; }
	for(std::size_t i = equal; i < size_hint; ++i){ tmp = tmp->parent; }
	tmp = this->get_forward(k, size_input, tmp, equal);
	tmp->data.second = std::move(obj);
	return std::pair<iterator, bool>(iterator(tmp,size), pcounter != this->counter);
}
template <class Key, class T, class Container, class Allocator> template <class Vector, class M> std::pair<typename Trie<Key,T,Container,Allocator>::iterator, bool> Trie<Key,T,Container,Allocator>::insert_or_assign(iterator hint, Vector&& k, M&& obj){
	NodeTrie* tmp = hint.current;
	std::size_t pcounter = this->counter, size_input = len_array(k), size_hint = len_array(hint.key), equal = 0, size;
	size = size_input <= size_hint ? size_input : size_hint;
	while(equal < size && hint.key[equal]==k[equal]){ ++equal; }
	for(std::size_t i = equal; i < size_hint; ++i){ tmp = tmp->parent; }
	tmp = this->get_forward(std::move(k), size_input, tmp, equal);
	tmp->data.second = std::move(obj);
	return std::pair<iterator, bool>(iterator(tmp,size), pcounter != this->counter);
}
// Erase
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::erase(iterator pos){
	NodeTrie* ptr = pos.current;
	++pos;
	this->remove_node(ptr);
	return pos;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::erase(iterator first, iterator last){
	while(first != last){
		NodeTrie* ptr = first.current;
		++first;
		this->remove_node(ptr);
	}
	return first;
}
template <class Key, class T, class Container, class Allocator> template <class Vector> std::size_t Trie<Key,T,Container,Allocator>::erase(const Vector& key){
	NodeTrie* tmp = this->find_node(key, len_array(key));
	std::size_t result = (tmp && tmp->valid);
	this->remove_node(tmp);
	return result;
}
template <class Key, class T, class Container, class Allocator> template <class Vector> std::size_t Trie<Key,T,Container,Allocator>::erase(Vector&& key){
	NodeTrie* tmp = this->find_node(key, len_array(key));
	std::size_t result = (tmp && tmp->valid);
	this->remove_node(tmp);
	return result;
}

// Lookup
// Count
template <class Key, class T, class Container, class Allocator> template <class K> std::size_t Trie<Key,T,Container,Allocator>::count(const K& key) const{
	NodeTrie* tmp = this->find_node(key, len_array(key));
	return tmp && tmp->valid;
}
// Find
template <class Key, class T, class Container, class Allocator> template <class K> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::find(const K& key){
	std::size_t size = len_array(key);
	NodeTrie* tmp = this->find_node(key, size);
	return (tmp && tmp->valid) ? iterator(tmp, size) : this->end();
}
// Contains
template <class Key, class T, class Container, class Allocator> template <class K> bool Trie<Key,T,Container,Allocator>::contains(const K& key) const{
	NodeTrie* tmp = this->find_node(key, len_array(key));
	return tmp && tmp->valid;
}
// Bounds
template <class Key, class T, class Container, class Allocator> template <class K> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::lower_bound(const K& key){
	NodeTrie *ptr = root;
	std::size_t size = len_array(key), i=0;
	while(i < size){
		auto it = ptr->children.lower_bound(key[i]);
		if(it == ptr->children.end()){
			while(ptr->parent){
				auto it2 = ptr->parent->children.upper_bound(key[--i]);
				if(it2 == ptr->parent->children.end()){
					ptr = ptr->parent;
				}else{
					ptr = (NodeTrie*) it2->second;
					++i;
					break;
				}
			}
			if(!ptr->parent){ return this->end(); }
			break;
		}
		ptr = (NodeTrie*) it->second;
		if(it->first != key[i++]){ break; }
	}
	while(!ptr->valid){
		ptr = (NodeTrie*) ptr->children.begin()->second;
		++i;
	}
	return (ptr && ptr->valid) ? iterator(ptr, i) : this->end();
}
template <class Key, class T, class Container, class Allocator> template <class K> Trie<Key,T,Container,Allocator>::iterator Trie<Key,T,Container,Allocator>::upper_bound(const K& key){
	NodeTrie *ptr = root;
	std::size_t size = len_array(key), i=0;
	bool search = false;
	while(i < size){
		auto it = ptr->children.lower_bound(key[i]);
		if(search = (it == ptr->children.end())){ break; }
		ptr = (NodeTrie*) it->second;
		if(it->first != key[i++]){ break; }
	}
	search |= (i==size) && (ptr->children.size()==0);
	if(search){
		while(ptr->parent){
			auto it2 = ptr->parent->children.upper_bound(key[--i]);
			if(it2 == ptr->parent->children.end()){
				ptr = ptr->parent;
			}else{
				ptr = (NodeTrie*) it2->second;
				++i;
				break;
			}
		}
		if(!ptr->parent){ return this->end(); }
	}
	if(!ptr->valid || (!search && i==size)){
		do{
			ptr = (NodeTrie*) ptr->children.begin()->second;
			++i;
		}while(!ptr->valid);
	}
	return (ptr && ptr->valid) ? iterator(ptr, i) : this->end();
}
template <class Key, class T, class Container, class Allocator> template <class K> std::pair<typename Trie<Key,T,Container,Allocator>::iterator, typename Trie<Key,T,Container,Allocator>::iterator> Trie<Key,T,Container,Allocator>::equal_range(const K& key){
	return std::pair<iterator,iterator>(this->lower_bound(key), this->upper_bound(key));
}

// Non-member functions
// Operators
template<class A,class B,class C,class D> bool operator==(const Trie<A,B,C,D>& lhs, const Trie<A,B,C,D>& rhs){
	if((lhs.counter != rhs.counter) || (lhs.nCounter != rhs.nCounter) || (lhs.data.valid != rhs.data.valid) || (lhs.data.valid && (lhs.data.data.second != rhs.data.data.second))){ return false; }
	typename Trie<A,B,C,D>::NodeTrie *lPtr = lhs.root, *rPtr = rhs.root;
	typename C::iterator lit, rit;
	goto descend_to_first_leaf;
	while(lPtr->parent){
		lit = lPtr->parent->children.upper_bound(lPtr->data.first);
		if(lit == lPtr->parent->children.end()){
			lPtr = lPtr->parent;
			rPtr = rPtr->parent;
		}else{
			rit = rPtr->parent->children.upper_bound(rPtr->data.first);
			lPtr = (typename Trie<A,B,C,D>::NodeTrie*) lit->second;
			rPtr = (typename Trie<A,B,C,D>::NodeTrie*) rit->second;
			if((lPtr->data.first!=rPtr->data.first) || (lPtr->valid!=rPtr->valid) || (lPtr->valid && (lPtr->data.second != rPtr->data.second)) ||  (lPtr->children.size() != rPtr->children.size())){ return false; }
			descend_to_first_leaf:
			while(lPtr->children.size() && rPtr->children.size()){
				lPtr = (typename Trie<A,B,C,D>::NodeTrie*) lPtr->children.begin()->second;
				rPtr = (typename Trie<A,B,C,D>::NodeTrie*) rPtr->children.begin()->second;
				if((lPtr->data.first!=rPtr->data.first) || (lPtr->valid!=rPtr->valid) || (lPtr->valid && (lPtr->data.second != rPtr->data.second)) ||  (lPtr->children.size() != rPtr->children.size())){ return false; }
			}
		}
	}
	return true;
}
// TODO: DEBUG THIS TOO
template<class A,class B,class C,class D> std::strong_ordering operator<=>(const Trie<A,B,C,D>& lhs, const Trie<A,B,C,D>& rhs){
	typename Trie<A,B,C,D>::NodeTrie *lPtr = lhs.root, *rPtr = rhs.root;
	typename C::iterator lit, rit;
	bool lend, rend;
	if(lPtr->valid != rPtr->valid){
		return lPtr->valid ? std::strong_ordering::less : std::strong_ordering::greater;
	}else if(lPtr->valid && (lPtr->data.second != rPtr->data.second)){
		return (lPtr->data.second < rPtr->data.second) ? std::strong_ordering::less : std::strong_ordering::greater;
	}
	goto descend_to_first_leaf;
	while(lPtr->parent){
		lit = lPtr->parent->children.upper_bound(lPtr->data.first);
		rit = rPtr->parent->children.upper_bound(rPtr->data.first);
		lend = (lit == lPtr->parent->children.end());
		rend = (rit == rPtr->parent->children.end());
		if(lend || rend){
			if(lend != rend){
				return lend ? std::strong_ordering::greater : std::strong_ordering::less;
			}
			lPtr = lPtr->parent;
			rPtr = rPtr->parent;
		}else{
			lPtr = (typename Trie<A,B,C,D>::NodeTrie*) lit->second;
			rPtr = (typename Trie<A,B,C,D>::NodeTrie*) rit->second;
			if(lPtr->valid!=rPtr->valid){
				return lPtr->valid ? std::strong_ordering::less : std::strong_ordering::greater;
			}else if(lPtr->data.first != rPtr->data.first){
				return lPtr->data.first < rPtr->data.first ? std::strong_ordering::less : std::strong_ordering::greater;
			}else if(lPtr->valid && (lPtr->data.second != rPtr->data.second)){
				return (lPtr->data.second < rPtr->data.second) ? std::strong_ordering::less : std::strong_ordering::greater;
			}
			descend_to_first_leaf:
			while(lPtr->children.size() && rPtr->children.size()){
				lPtr = (typename Trie<A,B,C,D>::NodeTrie*) lPtr->children.begin()->second;
				rPtr = (typename Trie<A,B,C,D>::NodeTrie*) rPtr->children.begin()->second;
				if(lPtr->valid!=rPtr->valid){
					return lPtr->valid ? std::strong_ordering::less : std::strong_ordering::greater;
				}else if(lPtr->data.first != rPtr->data.first){
					return lPtr->data.first < rPtr->data.first ? std::strong_ordering::less : std::strong_ordering::greater;
				}else if(lPtr->valid && (lPtr->data.second != rPtr->data.second)){
					return (lPtr->data.second < rPtr->data.second) ? std::strong_ordering::less : std::strong_ordering::greater;
				}
			}
			if(lPtr->children.size() || rPtr->children.size()){
				return lPtr->children.size() ? std::strong_ordering::less : std::strong_ordering::greater;
			}
		}
	}
	return std::strong_ordering::equal;
}

// Nested class NodeTrie
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::NodeTrie::NodeTrie(NodeTrie* parent) :
	parent(parent),
	valid(false){ }
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::NodeTrie::NodeTrie(const Key& key, NodeTrie* parent) :
	parent(parent),
	valid(false){
	this->data.first = key;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::NodeTrie::NodeTrie(Key&& key, NodeTrie* parent) :
	parent(parent),
	valid(false){
	this->data.first = std::move(key);
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::NodeTrie::~NodeTrie(){}
template <class Key, class T, class Container, class Allocator> void Trie<Key,T,Container,Allocator>::NodeTrie::copy(AllocatorNodes& alloc, NodeTrie* src, NodeTrie* dst){
	NodeTrie* ptrSrc = src;
	NodeTrie* ptrDst = dst;
	typename Container::key_compare cmp;
	bool up = false;
	while(ptrSrc){
		typename Container::iterator mit = ptrDst->children.begin(), mite = ptrDst->children.end(), oit = ptrSrc->children.begin(), oite = ptrSrc->children.end();
		ptrDst->valid = ptrSrc->valid;
		ptrDst->data.second = ptrSrc->data.second;
		if(!up){
			while(mit != mite){
				if(oit == oite || cmp(mit->first, oit->first)){
					NodeTrie* tmp = (NodeTrie*) mit->second;
					mit = ptrDst->children.erase(mit);
					NodeTrie::destroy(alloc, tmp);
				}else if(mit->first != oit->first){
					NodeTrie** tmp = (NodeTrie**)&(ptrDst->children[oit->first]);
					*tmp = alloc.allocate(1);
					std::construct_at(*tmp, oit->first, ptrDst);
					++oit;
				}
			}
			while(oit != oite){
				NodeTrie** tmp = (NodeTrie**)&(ptrDst->children[oit->first]);
				*tmp = alloc.allocate(1);
				std::construct_at(*tmp, oit->first, ptrDst);
				++oit;
			}
		}
		up = up || !ptrSrc->children.size();
		if(up){
			while(up && ptrSrc->parent){
				typename Container::iterator tmp = ++(ptrSrc->parent->children.find(ptrSrc->data.first));
				if(tmp == ptrSrc->parent->children.end()){
					ptrDst = ptrDst->parent;
					ptrSrc = ptrSrc->parent;
				}else{
					ptrDst = (NodeTrie*) (++(ptrDst->parent->children.find(ptrSrc->data.first)))->second;
					ptrSrc = (NodeTrie*) (++(ptrSrc->parent->children.find(ptrSrc->data.first)))->second;
					up = false;
				}
			}
			ptrSrc = up ? 0 : ptrSrc;
		}else{
			ptrDst = (NodeTrie*) ptrDst->children.begin()->second;
			ptrSrc = (NodeTrie*) ptrSrc->children.begin()->second;
		}
	}
}
template <class Key, class T, class Container, class Allocator> std::size_t Trie<Key,T,Container,Allocator>::NodeTrie::destroy(AllocatorNodes& alloc, NodeTrie* ptr){
	NodeTrie *tail;
	std::size_t result = 0;
	ptr->next = 0;
	tail = ptr;
	do{
		typename Container::iterator itEnd = ptr->children.end();
		for(typename Container::iterator tmpIt = ptr->children.begin(); tmpIt != itEnd; ++tmpIt){
			tail->next = (NodeTrie*) tmpIt->second;
			tail = tail->next;
		}
		tail->next = 0;
		NodeTrie *tmp = ptr->next;
		std::destroy_at(ptr);
		alloc.deallocate(ptr,1);
		++result;
		ptr = tmp;
	}while(ptr);
	return result;
}

// Private members
template <class Key, class T, class Container, class Allocator> template<class Vector> Trie<Key,T,Container,Allocator>::NodeTrie* Trie<Key,T,Container,Allocator>::get_forward(const Vector& key, std::size_t size){
	NodeTrie *ptr = root;
	std::size_t newNodes = 0;
	for(std::size_t i=0; i<size; ++i){
		NodeTrie** tmp = (NodeTrie**)&(ptr->children[key[i]]);
		if(!(*tmp)){
			*tmp = this->alloc.allocate(1);
			std::construct_at(*tmp, key[i], ptr);
			++newNodes;
		}
		ptr = *tmp;
	}
	if(!ptr->valid){
		ptr->valid = true;
		++this->counter;
		this->nCounter += newNodes;
	}
	return ptr;
}
template <class Key, class T, class Container, class Allocator> template<class Vector> Trie<Key,T,Container,Allocator>::NodeTrie* Trie<Key,T,Container,Allocator>::get_forward(Vector&& key, std::size_t size){
	NodeTrie *ptr = root;
	std::size_t newNodes = 0;
	for(std::size_t i=0; i<size; ++i){
		NodeTrie** tmp = (NodeTrie**)&(ptr->children[key[i]]);
		if(!(*tmp)){
			*tmp = this->alloc.allocate(1);
			std::construct_at(*tmp, std::move(key[i]), ptr);
			++newNodes;
		}
		ptr = *tmp;
	}
	if(!ptr->valid){
		ptr->valid = true;
		++this->counter;
		this->nCounter += newNodes;
	}
	return ptr;
}
template <class Key, class T, class Container, class Allocator> template<class Vector> Trie<Key,T,Container,Allocator>::NodeTrie* Trie<Key,T,Container,Allocator>::get_forward(const Vector& key, std::size_t size, NodeTrie* hint, std::size_t idx){
	NodeTrie *ptr = hint;
	std::size_t newNodes = 0;
	for(std::size_t i=idx; i<size; ++i){
		NodeTrie** tmp = (NodeTrie**)&(ptr->children[key[i]]);
		if(!(*tmp)){
			*tmp = this->alloc.allocate(1);
			std::construct_at(*tmp, key[i], ptr);
			++newNodes;
		}
		ptr = *tmp;
	}
	if(!ptr->valid){
		ptr->valid = true;
		++this->counter;
		this->nCounter += newNodes;
	}
	return ptr;
}
template <class Key, class T, class Container, class Allocator> template<class Vector> Trie<Key,T,Container,Allocator>::NodeTrie* Trie<Key,T,Container,Allocator>::get_forward(Vector&& key, std::size_t size, NodeTrie* hint, std::size_t idx){
	NodeTrie *ptr = hint;
	std::size_t newNodes = 0;
	for(std::size_t i=idx; i<size; ++i){
		NodeTrie** tmp = (NodeTrie**)&(ptr->children[key[i]]);
		if(!(*tmp)){
			*tmp = this->alloc.allocate(1);
			std::construct_at(*tmp, std::move(key[i]), ptr);
			++newNodes;
		}
		ptr = *tmp;
	}
	if(!ptr->valid){
		ptr->valid = true;
		++this->counter;
		this->nCounter += newNodes;
	}
	return ptr;
}
template <class Key, class T, class Container, class Allocator> template<class Vector> inline Trie<Key,T,Container,Allocator>::NodeTrie* Trie<Key,T,Container,Allocator>::find_node(Vector key, std::size_t size) const{
	NodeTrie* ptr = root;
	for(std::size_t i=0; i<size; ++i){
		typename Container::iterator it = ptr->children.find(key[i]);
		if(it == ptr->children.end()){ return 0; }
		ptr = (NodeTrie*)it->second;
	}
	return ptr;
}
template <class Key, class T, class Container, class Allocator> template<class Vector> std::size_t Trie<Key,T,Container,Allocator>::len_array(const Vector& vec) const{ return vec.size(); }
template <class Key, class T, class Container, class Allocator> std::size_t Trie<Key,T,Container,Allocator>::len_array(const Key* array) const{
	std::size_t count = 0;
	while(array[count]){ ++count; }
	return count;
}
template <class Key, class T, class Container, class Allocator> void Trie<Key,T,Container,Allocator>::remove_node(NodeTrie* ptr){
	if(!ptr){ return; }
	if(ptr->valid){
		ptr->valid = false;
		--this->counter;
	}
	if(!ptr->children.size()){
		while(ptr->parent && ptr->parent->children.size()==1 && (!ptr->parent->valid)){
			ptr = ptr->parent;
		}
		if(ptr->parent){
			ptr->parent->children.erase(ptr->data.first);
		}
		this->nCounter -= NodeTrie::destroy(this->alloc, ptr);
	}
}

// Iterator actions
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator_actions::iterator_actions() :
	data(key, nullvalue_value){ }
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator_actions::iterator_actions(const Trie<Key,T,Container,Allocator>::iterator_actions& other) :
	data(key, other.data.second),
	key(other.key),
	current(other.current){ }
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator_actions::iterator_actions(Trie<Key,T,Container,Allocator>::iterator_actions&& other) :
	data(key, other.data.second),
	key(std::move(other.key)),
	current(other.current){ }
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator_actions::iterator_actions(NodeTrie* ptr, std::size_t size) :
	data(key, nullvalue_value){
	NodeTrie* tmp = ptr;
	this->key.reserve(size);
	while(tmp->parent){
		this->key.push_back(tmp->data.first);
		tmp = tmp->parent;
	}
	std::reverse(this->key.begin(),this->key.end());
	this->update(ptr);
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator_actions& Trie<Key,T,Container,Allocator>::iterator_actions::operator=(const Trie<Key,T,Container,Allocator>::iterator_actions& other){
	*((std::vector<Key>*)&this->data.first) = other.data.first;
	this->update(other.current);
	return *this;
}
template <class Key, class T, class Container, class Allocator> Trie<Key,T,Container,Allocator>::iterator_actions& Trie<Key,T,Container,Allocator>::iterator_actions::operator=(Trie<Key,T,Container,Allocator>::iterator_actions&& other){
	*((std::vector<Key>*)&this->data.first) = std::move(*((std::vector<Key>*)& other.data.first));
	this->update(other.current);
	return *this;
}
template <class Key, class T, class Container, class Allocator> void Trie<Key,T,Container,Allocator>::iterator_actions::update(NodeTrie* ptr){
	this->current = ptr;
	std::construct_at(&this->data, this->key, ptr ? ptr->data.second : nullvalue_value);
}
template <class Key, class T, class Container, class Allocator> void Trie<Key,T,Container,Allocator>::iterator_actions::next_element(){
	NodeTrie* ptr = this->current;
	if(ptr){
		do{
			if(ptr->children.size()){
				ptr = (NodeTrie*) ptr->children.begin()->second;
				this->key.push_back(ptr->data.first);
			}else{
				while(ptr->parent){
					typename Container::iterator it = ++(ptr->parent->children.find(ptr->data.first));
					if(it != ptr->parent->children.end()){
						ptr = (NodeTrie*) it->second;
						this->key.back() = ptr->data.first;
						break;
					}
					ptr = ptr->parent;
					this->key.pop_back();
				}
				ptr = ptr->parent ? ptr : 0;
			}
		}while(ptr && !ptr->valid);
		this->update(ptr);
	}
}
template <class Key, class T, class Container, class Allocator> void Trie<Key,T,Container,Allocator>::iterator_actions::previous_element(){
	NodeTrie* ptr = this->current;
	if(ptr){
		do{
			if(ptr->parent){
				do{
					typename Container::iterator it = ptr->parent->children.find(ptr->data.first);
					if(it != ptr->parent->children.begin()){
						ptr = (NodeTrie*) (--it)->second;
						this->key.back() = ptr->data.first;
						while(ptr->children.size()){
							ptr = (NodeTrie*) ptr->children.rbegin()->second;
							this->key.push_back(ptr->data.first);
						}
						break;
					}
					ptr = ptr->parent;
					this->key.pop_back();
					if(ptr->valid){ break; }
				}while(ptr->parent);
			}else{
				ptr = 0;
			}
		}while(ptr && !ptr->valid);
		this->update(ptr);
	}
}
template <class Key, class T, class Container, class Allocator> bool Trie<Key,T,Container,Allocator>::iterator_actions::operator==(const Trie<Key,T,Container,Allocator>::iterator_actions& other) const{
	return this->current == other.current;
}
template <class Key, class T, class Container, class Allocator> bool Trie<Key,T,Container,Allocator>::iterator_actions::operator!=(const Trie<Key,T,Container,Allocator>::iterator_actions& other) const{
	return this->current != other.current;
}

template <class Key, class T, class Container, class Allocator> T Trie<Key,T,Container,Allocator>::iterator_actions::nullvalue_value;
template <class Key, class T, class Container, class Allocator> std::vector<Key> Trie<Key,T,Container,Allocator>::iterator_actions::nullvalue_key;
template <class Key, class T, class Container, class Allocator> std::pair<const std::vector<Key>&, T&> Trie<Key,T,Container,Allocator>::iterator_actions::nullvalue = std::pair<const std::vector<Key>&, T&>(nullvalue_key, nullvalue_value);

// Specialized copy/move calls
template <class Key, class T, class Container, class Allocator> void Trie<Key,T,Container,Allocator>::sp_copy(const Trie& other, std::true_type){
	if(this->alloc != other.alloc){
		this->clear();
	}
	this->alloc = other.alloc;
	NodeTrie::copy(this->alloc, other.root, this->root);
	this->counter = other.counter;
	this->nCounter = other.nCounter;
}
template <class Key, class T, class Container, class Allocator> void Trie<Key,T,Container,Allocator>::sp_copy(const Trie& other, std::false_type){
	NodeTrie::copy(this->alloc, other.root, this->root);
	this->counter = other.counter;
	this->nCounter = other.nCounter;
}
template <class Key, class T, class Container, class Allocator> void Trie<Key,T,Container,Allocator>::sp_move(Trie&& other, std::true_type) noexcept{
	if(this->root && this->root!=other.root){ this->clear(); }
	this->alloc = std::move(other.alloc);
	this->counter = other.counter;
	this->nCounter = other.nCounter;
	this->data.children = std::move(other.data.children);
	this->data.data.second = std::move(other.data.second);
	this->data.valid = other.data.valid;
	other.data.valid = false;
	other.counter = 0;
	other.nCounter = 0;
}
template <class Key, class T, class Container, class Allocator> void Trie<Key,T,Container,Allocator>::sp_move(Trie&& other, std::false_type){
	if(this->alloc == other.alloc){
		if(this->root && this->root != other.root){ this->clear(); }
		this->counter = other.counter;
		this->nCounter = other.nCounter;
		this->data.children = std::move(other.data.children);
		this->data.data.second = std::move(other.data.second);
		this->data.valid = other.data.valid;
		other.data.valid = false;
		other.counter = 0;
		other.nCounter = 0;
	}else{
		NodeTrie::copy(this->alloc, other.root, this->root);
		this->counter = other.counter;
		this->nCounter = other.nCounter;
	}
}

		}
	}
}

// Non-member
// Other
template<class A,class B,class C,class D, class Pred> std::size_t std::erase_if(bpp::collections::map::Trie<A,B,C,D>& trie, Pred pred){
	auto original = trie.size();
	for (auto i = trie.begin(), last = trie.end(); i != last; ){
		if(pred(*i)){
			i = trie.erase(i);
		}else{
			++i;
		}
	}
	return original - trie.size();
}
template<class A,class B,class C,class D> void std::swap(bpp::collections::map::Trie<A,B,C,D>& lhs, bpp::collections::map::Trie<A,B,C,D>& rhs){
	std::swap(lhs.alloc, rhs.alloc);
	std::swap(lhs.data, rhs.data);
	std::swap(lhs.counter, rhs.counter);
	std::swap(lhs.nCounter, rhs.nCounter);
}

#endif
