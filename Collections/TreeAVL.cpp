#ifndef BPP_COLLECTIONS_MAP_TREEAVL_CPP
#define BPP_COLLECTIONS_MAP_TREEAVL_CPP

namespace bpp{
	namespace collections{
		namespace map{

// Empty initialization
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL() :
	root(0),
	counter(0){ }
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(const Compare& comp, const Allocator& alloc) :
	alloc(alloc),
	cmp(comp),
	root(0),
	counter(0){ }
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(const Allocator& alloc) :
	alloc(alloc),
	root(0),
	counter(0){ }

// Iterator initilization
template <class Key, class T, class Compare, class Allocator> template<class InputIt> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(InputIt first, InputIt last, const Compare& comp, const Allocator& alloc) :
	alloc(alloc),
	cmp(comp),
	root(0),
	counter(0){
	for(auto it = first; it != last; ++it){
		this->get_forward(it->first)->data.second = it->second;
	}
}
template <class Key, class T, class Compare, class Allocator> template<class InputIt> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(InputIt first, InputIt last, const Allocator& alloc) :
	alloc(alloc),
	root(0),
	counter(0){
	for(auto it = first; it != last; ++it){
		this->get_forward(it->first)->data.second = it->second;
	}
}

// Copy constructor
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(const TreeAVL& other) :
	alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator())),
	root(other.root ? NodeAVL::copy(this->alloc, other.root) : 0),
	counter(other.counter){ }
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(const TreeAVL& other, const Allocator& alloc) :
	alloc(alloc),
	root(other.root ? NodeAVL::copy(this->alloc, other.root) : 0),
	counter(other.counter){ }

// Move constructor
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(TreeAVL&& other) :
	alloc(std::move(other.alloc)),
	cmp(std::move(other.cmp)),
	root(other.root),
	counter(other.counter){
	other.root = 0;
	other.counter = 0;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(TreeAVL&& other, const Allocator& alloc) :
	alloc(alloc),
	cmp(std::move(other.cmp)),
	counter(other.counter){
	if(this->alloc == other.alloc){
		this->root = other.root;
		other.root = 0;
		other.counter = 0;
	}else{
		this->root = other.root ? NodeAVL::copy(this->alloc, other.root) : 0;
		other.clear();
	}
}

// Initializer list initialization
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(std::initializer_list<std::pair<const Key, T>> ilist, const Compare& comp, const Allocator& alloc) :
	alloc(alloc),
	cmp(comp),
	root(0),
	counter(0){
	for(auto it : ilist){
		this->get_forward(it.first)->data.second = it.second;
	}
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::TreeAVL(std::initializer_list<std::pair<const Key, T>> ilist, const Allocator& alloc) :
	alloc(alloc),
	root(0),
	counter(0){
	for(auto it : ilist){
		this->get_forward(it.first)->data.second = it.second;
	}
}

// Destructor
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::~TreeAVL(){
	if(this->root){ NodeAVL::destroy(this->alloc,this->root); }
}

// Assignment
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>& TreeAVL<Key,T,Compare,Allocator>::operator=(const TreeAVL<Key,T,Compare,Allocator>& other){
	this->sp_copy(other, typename std::allocator_traits<Allocator>::propagate_on_container_copy_assignment());
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>& TreeAVL<Key,T,Compare,Allocator>::operator=(TreeAVL<Key,T,Compare,Allocator>&& other){
	this->sp_move(std::move(other), typename std::allocator_traits<Allocator>::propagate_on_container_move_assignment());
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
template <class Key, class T, class Compare, class Allocator> Allocator TreeAVL<Key,T,Compare,Allocator>::get_allocator() const noexcept{
	return this->alloc;
}

// Element access
template <class Key, class T, class Compare, class Allocator> const T& TreeAVL<Key,T,Compare,Allocator>::at(const Key& key) const{
	NodeAVL* tmp = this->root;
	while(tmp && tmp->data.first!=key){
		tmp = this->cmp(key, tmp->data.first) ? tmp->leftChild : tmp->rightChild;
	}
	if(!tmp){ throw std::out_of_range("bpp::collections::map::TreeAVL::at"); }
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
template <class Key, class T, class Compare, class Allocator> std::pair<const Key, T>& TreeAVL<Key,T,Compare,Allocator>::iterator::operator*() const{
	return this->current ? this->current->data : this->nullvalue;
}
template <class Key, class T, class Compare, class Allocator> std::pair<const Key, T>* TreeAVL<Key,T,Compare,Allocator>::iterator::operator->() const{
	return this->current ? &this->current->data : 0;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator& TreeAVL<Key,T,Compare,Allocator>::iterator::operator++(){
	this->next_element();
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::iterator::operator++(int){
	iterator pit(this->current);
	this->next_element();
	return pit;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator& TreeAVL<Key,T,Compare,Allocator>::iterator::operator--(){
	this->previous_element();
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::iterator::operator--(int){
	iterator pit(this->current);
	this->previous_element();
	return pit;
}
// Constant Iterator
template <class Key, class T, class Compare, class Allocator> const std::pair<const Key, T>& TreeAVL<Key,T,Compare,Allocator>::const_iterator::operator*() const{
	return this->current ? this->current->data : this->nullvalue;
}
template <class Key, class T, class Compare, class Allocator> const std::pair<const Key, T>* TreeAVL<Key,T,Compare,Allocator>::const_iterator::operator->() const{
	return this->current ? &this->current->data : 0;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_iterator& TreeAVL<Key,T,Compare,Allocator>::const_iterator::operator++(){
	this->next_element();
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_iterator TreeAVL<Key,T,Compare,Allocator>::const_iterator::operator++(int){
	const_iterator pit(this->current);
	this->next_element();
	return pit;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_iterator& TreeAVL<Key,T,Compare,Allocator>::const_iterator::operator--(){
	this->previous_element();
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_iterator TreeAVL<Key,T,Compare,Allocator>::const_iterator::operator--(int){
	const_iterator pit(this->current);
	this->previous_element();
	return pit;
}
// Reverse Iterator
template <class Key, class T, class Compare, class Allocator> constexpr TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::base() const{ return iterator(this->current); }
template <class Key, class T, class Compare, class Allocator> std::pair<const Key, T>& TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::operator*() const{
	return this->current ? this->current->data : this->nullvalue;
}
template <class Key, class T, class Compare, class Allocator> std::pair<const Key, T>* TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::operator->() const{
	return this->current ? &this->current->data : 0;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::reverse_iterator& TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::operator++(){
	this->previous_element();
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::reverse_iterator TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::operator++(int){
	reverse_iterator pit(this->current);
	this->previous_element();
	return pit;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::reverse_iterator& TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::operator--(){
	this->next_element();
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::reverse_iterator TreeAVL<Key,T,Compare,Allocator>::reverse_iterator::operator--(int){
	reverse_iterator pit(this->current);
	this->next_element();
	return pit;
}
// Constant Reverse Iterator
template <class Key, class T, class Compare, class Allocator> constexpr TreeAVL<Key,T,Compare,Allocator>::const_iterator TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator::base() const{ return const_iterator(this->current); }
template <class Key, class T, class Compare, class Allocator> const std::pair<const Key, T>& TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator::operator*() const{
	return this->current ? this->current->data : this->nullvalue;
}
template <class Key, class T, class Compare, class Allocator> const std::pair<const Key, T>* TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator::operator->() const{
	return this->current ? &this->current->data : 0;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator& TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator::operator++(){
	this->previous_element();
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator::operator++(int){
	const_reverse_iterator pit(this->current);
	this->previous_element();
	return pit;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator& TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator::operator--(){
	this->next_element();
	return *this;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator::operator--(int){
	const_reverse_iterator pit(this->current);
	this->next_element();
	return pit;
}

// Iterator generation methods
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::begin() const noexcept{
	NodeAVL* tmp = this->root;
	while(tmp && tmp->leftChild){ tmp = tmp->leftChild; }
	return iterator(tmp);
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_iterator TreeAVL<Key,T,Compare,Allocator>::cbegin() const noexcept{
	NodeAVL* tmp = this->root;
	while(tmp && tmp->leftChild){ tmp = tmp->leftChild; }
	return const_iterator(tmp);
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::end() const noexcept{
	return iterator();
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_iterator TreeAVL<Key,T,Compare,Allocator>::cend() const noexcept{
	return const_iterator();
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::reverse_iterator TreeAVL<Key,T,Compare,Allocator>::rbegin() const noexcept{
	NodeAVL* tmp = this->root;
	while(tmp && tmp->rightChild){ tmp = tmp->rightChild; }
	return reverse_iterator(tmp);
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator TreeAVL<Key,T,Compare,Allocator>::crbegin() const noexcept{
	NodeAVL* tmp = this->root;
	while(tmp && tmp->rightChild){ tmp = tmp->rightChild; }
	return const_reverse_iterator(tmp);
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::reverse_iterator TreeAVL<Key,T,Compare,Allocator>::rend() const noexcept{
	return reverse_iterator();
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::const_reverse_iterator TreeAVL<Key,T,Compare,Allocator>::crend() const noexcept{
	return const_reverse_iterator();
}

// Capacity
template <class Key, class T, class Compare, class Allocator> bool TreeAVL<Key,T,Compare,Allocator>::empty() const noexcept{
	return this->counter == 0;
}
template <class Key, class T, class Compare, class Allocator> std::size_t TreeAVL<Key,T,Compare,Allocator>::size() const noexcept{
	return this->counter;
}
template <class Key, class T, class Compare, class Allocator> std::size_t TreeAVL<Key,T,Compare,Allocator>::memory() const noexcept{
	return sizeof(TreeAVL) + this->counter*sizeof(NodeAVL);
}

// Modifiers
// Clear
template <class Key, class T, class Compare, class Allocator> void TreeAVL<Key,T,Compare,Allocator>::clear() noexcept{
	if(this->root){
		NodeAVL::destroy(this->alloc,this->root);
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
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::insert(TreeAVL<Key,T,Compare,Allocator>::iterator hint, const std::pair<const Key, T>& value){
	std::size_t pcounter = this->counter;
	NodeAVL* tmp = this->get_forward(value.first, hint.current);
	if(pcounter != this->counter){ tmp->data.second = value.second; }
	return iterator(tmp);
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::insert(TreeAVL<Key,T,Compare,Allocator>::iterator hint, std::pair<const Key, T>&& value){
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
template <class Key, class T, class Compare, class Allocator> template <class M> std::pair<typename TreeAVL<Key,T,Compare,Allocator>::iterator, bool> TreeAVL<Key,T,Compare,Allocator>::insert_or_assign(TreeAVL<Key,T,Compare,Allocator>::iterator hint, const Key& k, M&& obj){
	std::size_t pcounter = this->counter;
	NodeAVL* tmp = this->get_forward(k, hint.current);
	tmp->data.second = obj;
	return std::pair<iterator, bool>(iterator(tmp), pcounter != this->counter);
}
template <class Key, class T, class Compare, class Allocator> template <class M> std::pair<typename TreeAVL<Key,T,Compare,Allocator>::iterator, bool> TreeAVL<Key,T,Compare,Allocator>::insert_or_assign(TreeAVL<Key,T,Compare,Allocator>::iterator hint, Key&& k, M&& obj){
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
	iterator it(tmp);
	if(tmp && this->cmp(tmp->data.first, k)){ ++it; }
	return it;
}
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator TreeAVL<Key,T,Compare,Allocator>::erase(iterator first, iterator last){
	while(first != last){
		first = erase(first);
	}
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

// Nested class NodeAVL
template <class Key, class T, class Compare,class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL::NodeAVL(NodeAVL* parent) :
	parent(parent),
	leftChild(0),
	rightChild(0),
	height(0){ }
template <class Key, class T, class Compare,class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL::NodeAVL(const Key& key, NodeAVL* parent) :
	parent(parent),
	leftChild(0),
	rightChild(0),
	height(0){
	*((Key*) &this->data.first) = key;
}
template <class Key, class T, class Compare,class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL::NodeAVL(Key&& key, NodeAVL* parent) :
	parent(parent),
	leftChild(0),
	rightChild(0),
	height(0){
	*((Key*) &this->data.first) = std::move(key);
}
template <class Key, class T, class Compare,class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL::~NodeAVL(){}
template <class Key, class T, class Compare,class Allocator> void TreeAVL<Key,T,Compare,Allocator>::NodeAVL::recalculate_height(){
	std::size_t lz = (this->leftChild ? this->leftChild->height : 0), rz = (this->rightChild ? this->rightChild->height : 0);
	this->height = (lz>rz ? lz : rz) + 1;
}
template <class Key, class T, class Compare,class Allocator> int TreeAVL<Key,T,Compare,Allocator>::NodeAVL::left_heavy(){
	return (this->leftChild ? this->leftChild->height : 0) > (this->rightChild ? this->rightChild->height : 0);
}
template <class Key, class T, class Compare,class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::NodeAVL::copy(AllocatorNodes& alloc, NodeAVL* src, NodeAVL* dst){
	NodeAVL* result = dst, *ptr;
	if(!result){
		result = alloc.allocate(1);
		std::construct_at(result);
	}
	ptr = result;
	*((std::pair<Key,T>*)&ptr->data) = src->data;
	ptr->height = src->height;
	goto nodeavl_copy_get_leftmost_child_loop;
	while(ptr){
		if(!src->leftChild && ptr->leftChild){ ptr->leftChild = NodeAVL::destroy(alloc, ptr->leftChild); }
		if(src->rightChild){
			if(!ptr->rightChild){
				ptr->rightChild = alloc.allocate(1);
				std::construct_at(ptr->rightChild, ptr);
			}
			src = src->rightChild;
			ptr = ptr->rightChild;
			*((std::pair<Key,T>*)&ptr->data) = src->data;
			ptr->height = src->height;
			nodeavl_copy_get_leftmost_child_loop:
			while(src->leftChild){
				if(!ptr->leftChild){
					ptr->leftChild = alloc.allocate(1);
					std::construct_at(ptr->leftChild, ptr);
				}
				src = src->leftChild;
				ptr = ptr->leftChild;
				*((std::pair<Key,T>*)&ptr->data) = src->data;
				ptr->height = src->height;
			};
		}else{
			if(ptr->rightChild){ ptr->rightChild = NodeAVL::destroy(alloc, ptr->rightChild); }
			while(src->parent && src == src->parent->rightChild){
				src = src->parent;
				ptr = ptr->parent;
			}
			src = src->parent;
			ptr = ptr->parent;
		}
	}
	return result;
}
template <class Key, class T, class Compare,class Allocator> TreeAVL<Key,T,Compare,Allocator>::NodeAVL* TreeAVL<Key,T,Compare,Allocator>::NodeAVL::destroy(AllocatorNodes& alloc, NodeAVL* node){
	NodeAVL* ptr = node;
	ptr->parent = 0;
	while(ptr->leftChild){ ptr = ptr->leftChild; }
	while(ptr){
		if(ptr->rightChild){
			ptr = ptr->rightChild;
			while(ptr->leftChild){ ptr = ptr->leftChild; }
		}else{
			while(ptr->parent && ptr == ptr->parent->rightChild){
				ptr = ptr->parent;
				std::destroy_at(ptr->rightChild);
				alloc.deallocate(ptr->rightChild,1);
			}
			ptr = ptr->parent;
			if(ptr && ptr->leftChild){
				std::destroy_at(ptr->leftChild);
				alloc.deallocate(ptr->leftChild,1);
			}
		}
	}
	std::destroy_at(node);
	alloc.deallocate(node,1);
	return 0;
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
			std::construct_at(tmp, key, parent);
			*(this->cmp(key, parent->data.first) ? &(parent->leftChild) : &(parent->rightChild)) = tmp;
			this->rebalance(parent);
			this->counter++;
		}
	}else{
		tmp = this->alloc.allocate(1);
		std::construct_at(tmp, key);
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
			std::construct_at(tmp, key, parent);
			*(this->cmp(key, parent->data.first) ? &(parent->leftChild) : &(parent->rightChild)) = tmp;
			this->rebalance(parent);
			this->counter++;
		}
	}else{
		tmp = this->alloc.allocate(1);
		std::construct_at(tmp, key);
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
			std::construct_at(tmp, key, parent);
			*(this->cmp(key, parent->data.first) ? &(parent->leftChild) : &(parent->rightChild)) = tmp;
			this->rebalance(parent);
			this->counter++;
		}
	}else{
		tmp = this->alloc.allocate(1);
		std::construct_at(tmp, key);
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
			std::construct_at(tmp, key, parent);
			*(this->cmp(key, parent->data.first) ? &(parent->leftChild) : &(parent->rightChild)) = tmp;
			this->rebalance(parent);
			this->counter++;
		}
	}else{
		tmp = this->alloc.allocate(1);
		std::construct_at(tmp, key);
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
			*((std::pair<Key,T>*)&src->data) = std::move(tmp->data);
		}else if(tmp->leftChild){
			tmp = tmp->leftChild;
			*((std::pair<Key,T>*)&src->data) = std::move(tmp->data);
			tmp->parent->leftChild = 0;
		}else{
			if(result = tmp->parent){
				*((tmp->parent->leftChild == tmp) ? &(tmp->parent->leftChild):&(tmp->parent->rightChild)) = 0;
			}else{
				this->root = 0;
			}
		}
		if(tmp->parent){ this->rebalance(tmp->parent); }
		NodeAVL::destroy(this->alloc,tmp);
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
			NodeAVL* result = (lz>rz) ? (source->leftChild->left_heavy() ? rotation_LL(source) : rotation_LR(source)) : ((source->rightChild->left_heavy()) ? rotation_RL(source) : rotation_RR(source));
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

// Iterator actions
template <class Key, class T, class Compare, class Allocator> TreeAVL<Key,T,Compare,Allocator>::iterator_actions::iterator_actions(NodeAVL* init) : 
	current(init){ }
template <class Key, class T, class Compare, class Allocator> void TreeAVL<Key,T,Compare,Allocator>::iterator_actions::next_element(){
	if(this->current){
		if(this->current->rightChild){
			this->current = this->current->rightChild;
			while(this->current->leftChild){ this->current = this->current->leftChild; }
		}else{
			while(this->current->parent && (this->current->parent->rightChild == this->current)){ this->current = this->current->parent; }
			this->current = this->current->parent;
		}
	}
}
template <class Key, class T, class Compare, class Allocator> void TreeAVL<Key,T,Compare,Allocator>::iterator_actions::previous_element(){
	if(this->current){
		if(this->current->leftChild){
			this->current = this->current->leftChild;
			while(this->current->rightChild){ this->current = this->current->rightChild; }
		}else{
			while(this->current->parent && (this->current->parent->leftChild == this->current)){ this->current = this->current->parent; }
			this->current = this->current->parent;
		}
	}
}
template <class Key, class T, class Compare, class Allocator> bool TreeAVL<Key,T,Compare,Allocator>::iterator_actions::operator==(const TreeAVL<Key,T,Compare,Allocator>::iterator_actions& other) const{
	return this->current == other.current;
}
template <class Key, class T, class Compare, class Allocator> bool TreeAVL<Key,T,Compare,Allocator>::iterator_actions::operator!=(const TreeAVL<Key,T,Compare,Allocator>::iterator_actions& other) const{
	return this->current != other.current;
}

template <class Key, class T, class Compare, class Allocator> std::pair<const Key, T> TreeAVL<Key,T,Compare,Allocator>::iterator_actions::nullvalue;

// Specialized copy/move calls
template <class Key, class T, class Compare, class Allocator> void TreeAVL<Key,T,Compare,Allocator>::sp_copy(const TreeAVL& other, std::true_type){
	NodeAVL* nroot;
	if(this->alloc != other.alloc){
		nroot = other.root ? NodeAVL::copy(other.alloc, other.root) : 0;
		if(this->root){ NodeAVL::destroy(this->alloc, this->root); }
		this->alloc = other.alloc;
	}else{
		this->alloc = other.alloc;
		nroot = other.root ? NodeAVL::copy(this->alloc, other.root, this->root) : 0;
		if(!nroot && this->root){ NodeAVL::destroy(this->alloc, this->root); }
	}
	this->cmp = other.cmp;
	this->counter = other.counter;
	this->root = nroot;
}
template <class Key, class T, class Compare, class Allocator> void TreeAVL<Key,T,Compare,Allocator>::sp_copy(const TreeAVL& other, std::false_type){
	NodeAVL* nroot = other.root ? NodeAVL::copy(this->alloc, other.root, this->root) : 0;
	if(!nroot && this->root){ NodeAVL::destroy(this->alloc, this->root); }
	this->cmp = other.cmp;
	this->counter = other.counter;
	this->root = nroot;
}
template <class Key, class T, class Compare, class Allocator> void TreeAVL<Key,T,Compare,Allocator>::sp_move(TreeAVL&& other, std::true_type) noexcept{
	if(this->root && this->root!=other.root){ NodeAVL::destroy(this->alloc, this->root); }
	this->alloc = std::move(other.alloc);
	this->cmp = std::move(other.cmp);
	this->root = other.root;
	this->counter = other.counter;
	other.root = 0;
	other.counter = 0;
}
template <class Key, class T, class Compare, class Allocator> void TreeAVL<Key,T,Compare,Allocator>::sp_move(TreeAVL&& other, std::false_type){
	if(this->alloc == other.alloc){
		if(this->root && this->root != other.root){ NodeAVL::destroy(this->alloc, this->root); }
		this->cmp = std::move(other.cmp);
		this->root = other.root;
		this->counter = other.counter;
		other.root = 0;
		other.counter = 0;
	}else{
		NodeAVL* nroot = other.root ? NodeAVL::copy(this->alloc, other.root, this->root) : 0;
		if(!nroot && this->root){ NodeAVL::destroy(this->alloc, this->root); }
		this->cmp = other.cmp;
		this->counter = other.counter;
		this->root = nroot;
	}
}

		}
		namespace serialize{

//! Specialization of the Serialize template to support TreeAVL objects.
template<typename Buff, typename... Types> std::size_t Serialize<Buff,bpp::collections::map::TreeAVL<Types...>>::operator()(Buff& buffer, const bpp::collections::map::TreeAVL<Types...>& obj) const{
	std::queue<typename bpp::collections::map::TreeAVL<Types...>::node_type*> queue;
	std::size_t objs = obj.size();
	std::size_t res = serialize(buffer, objs);
	if(objs){
		queue.push(obj.root);
		do{
			typename bpp::collections::map::TreeAVL<Types...>::node_type* ptr = queue.front();
			queue.pop();
			res += serialize(buffer, ptr->data);
			if(ptr->leftChild){ queue.push(ptr->leftChild); }
			if(ptr->rightChild){ queue.push(ptr->rightChild); }
		}while(queue.size());
	}
	return res;
}
template<typename Buff, typename... Types> std::size_t Deserialize<Buff,bpp::collections::map::TreeAVL<Types...>>::operator()(Buff& buffer, bpp::collections::map::TreeAVL<Types...>& obj) const{
	std::size_t objs;
	std::size_t res = deserialize(buffer, objs);
	obj.clear();
	for(std::size_t i=0; i<objs; ++i){
		std::pair<typename bpp::collections::map::TreeAVL<Types...>::key_type,typename bpp::collections::map::TreeAVL<Types...>::mapped_type> value;
		res += deserialize(buffer, value);
		obj.insert(value);
	}
	return res;
}

		}
	}
}

// Non-member
// Other
template<class A,class B,class C,class D, class Pred> std::size_t std::erase_if(bpp::collections::map::TreeAVL<A,B,C,D>& tree, Pred pred){
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
template<class A,class B,class C,class D> void std::swap(bpp::collections::map::TreeAVL<A,B,C,D>& lhs, bpp::collections::map::TreeAVL<A,B,C,D>& rhs){
	std::swap(lhs.alloc, rhs.alloc);
	std::swap(lhs.cmp, rhs.cmp);
	std::swap(lhs.root, rhs.root);
	std::swap(lhs.counter, rhs.counter);
}

#endif
