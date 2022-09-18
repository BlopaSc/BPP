#ifndef BPP_MAP_TREEAVL_H
#define BPP_MAP_TREEAVL_H
#include <compare>			// std::strong_ordering
#include <functional>		// std::less
#include <initializer_list>	// std::initializer_list<>
#include <memory>			// std::allocator, std::allocator_traits<>
#include <stdexcept>		// std::out_of_range
#include <utility>			// std::pair, std::swap

namespace bpp{
	namespace map{

//! TreeAVL is a sorted associative container that contains key-value pairs with unique keys. Keys are sorted by using the comparison function Compare. Search, removal, and insertion operations have logarithmic complexity. This is an implementation of an Adelson-Velsky and Landis Tree.
template <class Key, class T, class Compare = std::less<Key>,class Allocator = std::allocator<std::pair<const Key, T>>> class TreeAVL{
	private:
		struct NodeAVL;
		class iterator_base;
		// Rebind Allocator
		using AllocatorNodes = typename std::allocator_traits<Allocator>::rebind_alloc<NodeAVL>;
	public:
		//! Constructs an empty container.
		TreeAVL();
		// explicit TreeAVL(const Compare& comp, const Allocator& alloc = Allocator());
		// explicit TreeAVL(const Allocator& alloc);
		
		//! Constructs the container with the contents of the range [first, last). If multiple elements in the range have keys that compare equivalent, it is unspecified which element is inserted.
		template<class InputIt> TreeAVL(InputIt first, InputIt last/*, const Compare& comp = Compare(), const Allocator& alloc = Allocator()*/);
		// template<class InputIt> TreeAVL(InputIt first, InputIt last, const Allocator& alloc);
		
		//! Copy constructor. Constructs the container with the copy of the contents of other.
		TreeAVL(const TreeAVL& other);
		// TreeAVL(const TreeAVL& other, const Allocator& alloc);
		
		//! Move constructor. Constructs the container with the contents of other using move semantics.
		TreeAVL(TreeAVL&& other);
		// TreeAVL(TreeAVL&& other, const Allocator& alloc);
		
		//! Constructs the container with the contents of the initializer list init. If multiple elements in the range have keys that compare equivalent, it is unspecified which element is inserted.
		TreeAVL(std::initializer_list<std::pair<const Key, T>> ilist/*, const Compare& comp = Compare(), const Allocator& alloc = Allocator()*/);
		//TreeAVL(std::initializer_list<value_type> ilist, const Allocator&);
		
		//! Destructs the map. The destructors of the elements are called and the used storage is deallocated. Note, that if the elements are pointers, the pointed-to objects are not destroyed.
		~TreeAVL();
		
		// Assignment
		//! Copy assignment operator. Replaces the contents with a copy of the contents of other.
		TreeAVL& operator=(const TreeAVL& other);
		//! Move assignment operator. Replaces the contents with those of other using move semantics (i.e. the data in other is moved from other into this container).
		TreeAVL& operator=(TreeAVL&& other) noexcept;
		//! Replaces the contents with those identified by initializer list ilist.
		TreeAVL& operator=(std::initializer_list<std::pair<const Key, T>> ilist);
		
		// Get allocator
		// Allocator get_allocator() const noexcept;
		
		// Element access
		//! Returns a reference to the mapped value of the element with key equivalent to key. If no such element exists, an exception of type std::out_of_range is thrown.
		const T& at(const Key& key) const;
		//! Returns a reference to the value that is mapped to a key equivalent to key, performing an insertion if such key does not already exist.
		T& operator[](const Key& key);
		//! Returns a reference to the value that is mapped to a key equivalent to key, performing an insertion if such key does not already exist.
		T& operator[](Key&& key);
		
		// Iterators
		//! iterator is an object that allows iteration of the container in the order of the sorted keys. Beware that any changes in the tree may invalidate the iterators and produce undefined behaviour.
		class iterator : public iterator_base{
			public:
				//! Should not be called, use begin() and end() instead.
				iterator(NodeAVL* init=0);
				//! References the key-value pair pointed at by the iterator.
				std::pair<Key, T>& operator*() const;
				//! Dereferences the key-value pair pointed at by the iterator.
				std::pair<Key, T>* operator->() const;
				//! Advances the iterator to the next key-value in the map.
				iterator& operator++();
		};
		class const_iterator;
		//! reverse_iterator is an object that allows iteration of the container in the inverse order of the sorted keys. Beware that any changes in the tree may invalidate the iterators and produce undefined behaviour.
		class reverse_iterator : public iterator_base{
			public:
				//! Should not be called, use rbegin() and rend() instead.
				reverse_iterator(NodeAVL* init=0);
				//! References the key-value pair pointed at by the iterator.
				std::pair<Key, T>& operator*() const;
				//! Dereferences the key-value pair pointed at by the iterator.
				std::pair<Key, T>* operator->() const;
				//! Advances the iterator to the previous key-value in the map.
				reverse_iterator& operator++();
		};
		class const_reverse_iterator;
		//! Returns an iterator to the first element of the map. If the map is empty, the returned iterator will be equal to end().
		iterator begin() const noexcept;
		// const_iterator begin() const noexcept;
		// const_iterator cbegin() const noexcept;
		//! Returns an iterator to the element following the last element of the map. This element acts as a placeholder; attempting to access it results in undefined behavior.
		iterator end() const noexcept;
		// const_iterator end() const noexcept;
		// const_iterator cend() const noexcept;
		//! Returns a reverse iterator to the first element of the reversed map. It corresponds to the last element of the non-reversed map. If the map is empty, the returned iterator is equal to rend().
		reverse_iterator rbegin() const noexcept;
		// const_reverse_iterator rbegin() const noexcept;
		// const_reverse_iterator crbegin() const noexcept;
		//! Returns a reverse iterator to the element following the last element of the reversed map. It corresponds to the element preceding the first element of the non-reversed map. This element acts as a placeholder, attempting to access it results in undefined behavior.
		reverse_iterator rend() const noexcept;
		// const_reverse_iterator rend() const noexcept;
		// const_reverse_iterator crend() const noexcept;
		
		// Capacity
		//! Checks if the container has no elements, i.e. whether begin() == end().
		bool empty() const noexcept;
		//! Returns the number of elements in the container.
		std::size_t size() const noexcept;
		
		// Modifiers
		// Clear
		//! Erases all elements from the container. After this call, size() returns zero. Invalidates any references, pointers, or iterators referring to contained elements. Any past-the-end iterator remains valid.
		void clear() noexcept;
		// Insert
		//! Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent key. Returns a pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and a bool denoting whether the insertion took place.
		std::pair<iterator, bool> insert(const std::pair<const Key, T>& value);
		// template<class P> std::pair<iterator, bool> insert(P&& value);
		//! Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent key. Returns a pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and a bool denoting whether the insertion took place.
		std::pair<iterator, bool> insert(std::pair<const Key, T>&& value);
		//! Inserts value in the position as close as possible to hint. Returns an iterator to the inserted element, or to the element that prevented the insertion.
		iterator insert(iterator_base hint, const std::pair<const Key, T>& value);
		// template<class P> iterator insert(const_iterator hint, P&& value);
		//! Inserts value in the position as close as possible to hint. Returns an iterator to the inserted element, or to the element that prevented the insertion.
		iterator insert(iterator_base hint, std::pair<const Key, T>&& value);
		//! Inserts elements from range [first, last). If multiple elements in the range have keys that compare equivalent, it is unspecified which element is inserted.
		template<class InputIt> void insert(InputIt first, InputIt last);
		//! Inserts elements from initializer list ilist. If multiple elements in the range have keys that compare equivalent, it is unspecified which element is inserted.
		void insert(std::initializer_list<std::pair<const Key, T>> ilist);
		// Insert or Assign
		//! If a key equivalent to k already exists in the container, assigns obj to the mapped_type corresponding to the key k. If the key does not exist, inserts the new value as if by insert.
		template <class M> std::pair<iterator, bool> insert_or_assign(const Key& k, M&& obj);
		//! If a key equivalent to k already exists in the container, assigns obj to the mapped_type corresponding to the key k. If the key does not exist, inserts the new value as if by insert.
		template <class M> std::pair<iterator, bool> insert_or_assign(Key&& k, M&& obj);
		//! If a key equivalent to k already exists in the container, assigns obj to the mapped_type corresponding to the key k. If the key does not exist, inserts the new value as if by insert.
		template <class M> std::pair<iterator, bool> insert_or_assign(iterator_base hint, const Key& k, M&& obj);
		//! If a key equivalent to k already exists in the container, assigns obj to the mapped_type corresponding to the key k. If the key does not exist, inserts the new value as if by insert.
		template <class M> std::pair<iterator, bool> insert_or_assign(iterator_base hint, Key&& k, M&& obj);
		// Erase
		//! Removes element at pos from the container. Returns iterator following the last removed element.
		iterator erase(iterator pos);
		// iterator erase(const_iterator);
		// iterator erase(iterator first, iterator last);
		// iterator erase(const_iterator_base first, const_iterator last);
		//! Removes the element (if one exists) with the key equivalent to key. Returns the number of elements removed (0 or 1).
		std::size_t erase(const Key& key);
		//! Removes the element (if one exists) with the key equivalent to key. Returns the number of elements removed (0 or 1).
		template<class K> std::size_t erase(K&& key);
		
		// Lookup
		// Count
		//! Returns the number of elements with key that compares equivalent to the specified argument, which is either 1 or 0 since this container does not allow duplicates.
		template<class K> std::size_t count(const K& key) const;
		// Find
		//! Finds an element with key equivalent to key and returns an iterator to the element. If no such element is found, past-the-end (see end()) iterator is returned.
		template<class K> iterator find(const K& key);
		// template<class K> const_iterator find(const K& key) const;
		// Contains
		//! Checks if there is an element with key that compares equivalent to the value x.
		template<class K> bool contains(const K& key) const;
		// Bounds
		//! Returns an iterator pointing to the first element that compares not less (i.e. greater or equal) to the value x.
		template<class K> iterator lower_bound(const K& key);
		// template<class K> const_iterator lower_bound(const K& key);
		//! Returns an iterator pointing to the first element that compares greater to the value x.
		template<class K> iterator upper_bound(const K& key);
		// template<class K> const_iterator upper_bound(const K& key);
		//! Returns a range containing all elements with the given key in the container. The range is defined by two iterators, one pointing to the first element that is not less than key and another pointing to the first element greater than key. Alternatively, the first iterator may be obtained with lower_bound(), and the second with upper_bound().
		template<class K> std::pair<iterator,iterator> equal_range(const K& key);
		// template<class K> std::pair<const_iterator,const_iterator> equal_range(const K& key);
		
		// Non-member
		// Operators
		//! Checks if the contents of lhs and rhs are equal, that is, they have the same number of elements and each element in lhs compares equal with the element in rhs at the same position.
		template<class A,class B,class C,class D> friend bool operator==(const TreeAVL<A,B,C,D>& lhs, const TreeAVL<A,B,C,D>& rhs);
		//! Compares the contents of lhs and rhs lexicographically.
		template<class A,class B,class C,class D> friend std::strong_ordering operator<=>(const TreeAVL<A,B,C,D>& lhs, const TreeAVL<A,B,C,D>& rhs);
		// Other
		//! Erases all elements that satisfy the predicate pred from the container.
		template<class A,class B,class C,class D, class Pred> friend std::size_t erase_if(TreeAVL<A,B,C,D>& tree, Pred pred);
		
	private:
		// Nested class NodeAVL
		struct NodeAVL{
			NodeAVL *parent,*leftChild,*rightChild;
			std::pair<Key,T> data;
			std::size_t height;
			void create(const Key& key, NodeAVL* parent=0);
			void create(Key&& key, NodeAVL* parent=0);
			void destroy(AllocatorNodes& alloc);
			inline void recalculate_height();
			inline int balance();
			static NodeAVL* copy(AllocatorNodes& alloc, NodeAVL* src, NodeAVL* parent=0);
		};
		AllocatorNodes alloc;
		Compare cmp;
		NodeAVL *root;
		std::size_t counter;
		inline NodeAVL* get_forward(const Key& key);
		inline NodeAVL* get_forward(Key&& key);
		inline NodeAVL* get_forward(const Key& key, NodeAVL* hint);
		inline NodeAVL* get_forward(Key&& key, NodeAVL* hint);
		inline NodeAVL* remove_node(NodeAVL* src);
		inline void rebalance(NodeAVL *source);
		inline static NodeAVL* rotation_LL(NodeAVL* source);
		inline static NodeAVL* rotation_RR(NodeAVL* source);
		inline static NodeAVL* rotation_LR(NodeAVL* source);
		inline static NodeAVL* rotation_RL(NodeAVL* source);
		// Iterator base class
		class iterator_base{
			friend class TreeAVL<Key,T,Compare,Allocator>;
			public:
				bool operator==(const iterator_base& other) const;
				bool operator!=(const iterator_base& other) const;
			protected:
				NodeAVL* current;
				static std::pair<Key, T> nullvalue;
		};
};

#include "TreeAVL.cpp"

	}
}

#endif
