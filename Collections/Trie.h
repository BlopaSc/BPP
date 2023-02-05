#ifndef BPP_COLLECTIONS_MAP_TRIE_H
#define BPP_COLLECTIONS_MAP_TRIE_H

#include <algorithm>	// std::reverse
#include <map>			// std::map
#include <memory>		// std::allocator<>, std::allocator_traits<>, std::construct_at, std::destroy_at
#include <stdexcept>	// std::out_of_range
#include <utility>		// std::move, std::pair
#include <vector>		// std::vector

namespace bpp{
	namespace collections{
		namespace map{

//! Trie is a sorted associative container that contains key-value pairs with unique keys. Any indexed collection can be used as key for the Trie, but the base type should be specified. The Container support class can be specified but by default std::map is used. Keys are sorted by using the comparison function within Container. Search, removal, and insertion operations have loglinear complexity. This is an implementation of a trie/prefix tree structure.
template <class Key, class T, class Container = std::map<Key, void*>, class Allocator = std::allocator<std::pair<const Key, T>>> class Trie{
	private:
		struct NodeTrie;
		struct iterator_actions;
		// Rebind Allocator
		using AllocatorNodes = typename std::allocator_traits<Allocator>::rebind_alloc<NodeTrie>;
	public:
		// Declares member types
		//! Type of the keys for the key-value pairs.
		using key_type = const std::vector<Key>;
		//! Type of the mapped values for the key-value pairs.
		using mapped_type = T;
		//! Type of the values stored in the map.
		using value_type = std::pair<const std::vector<Key>, T>;
		//! Type used to calculate sizes.
		using size_type = std::size_t;
		//! Type used to calculate differences (unused).
		using difference_type = std::ptrdiff_t;
		//! Type of allocator used.
		using allocator_type = Allocator;
		//! Type of references to values.
		using reference = value_type&;
		//! Type of constant references to values;
		using const_reference = const value_type&;
		//! Type of container node.
		using node_type = NodeTrie;
		
		//! Constructs an empty container.
		Trie();
		//! Constructs an empty container. Receives the allocator to use for all memory allocations.
		explicit Trie(const Allocator& alloc);
		//! Constructs the container with the contents of the range [first, last). If multiple elements in the range have keys that compare equivalent, it is unspecified which element is inserted. Receives the allocator to use for all memory allocations.
		template <class InputIt> Trie(InputIt first, InputIt last, const Allocator& alloc = Allocator());
		//! Copy constructor. Constructs the container with the copy of the contents of other. Can receive the allocator to use for all memory allocations.
		Trie(const Trie& other, const Allocator& alloc = Allocator());
		//! Move constructor. Constructs the container with the contents of other using move semantics. After the move, other is guaranteed to be empty().
		Trie(Trie&& other);
		//! Move constructor. Constructs the container with the contents of other using move semantics. After the move, other is guaranteed to be empty(). Receives the allocator to use for all memory allocations.
		Trie(Trie&& other, const Allocator& alloc);
		//! Constructs the container with the contents of the initializer list init. If multiple elements in the range have keys that compare equivalent, it is unspecified which element is inserted. Can receive the allocator to use for all memory allocations.
		template <class Vector> Trie(std::initializer_list<std::pair<const Vector, T>> ilist, const Allocator& alloc = Allocator());
		
		//! Destructs the map. The destructors of the elements are called and the used storage is deallocated. Note, that if the elements are pointers, the pointed-to objects are not destroyed.
		~Trie();
		
		// Assignment
		//! Copy assignment operator. Replaces the contents with a copy of the contents of other.
		Trie& operator=(const Trie& other);
		//! Move assignment operator. Replaces the contents with those of other using move semantics (i.e. the data in other is moved from other into this container). After the move, other is in a valid but unspecified state.
		Trie& operator=(Trie&& other);
		//! Replaces the contents with those identified by initializer list ilist.
		template<class Vector> Trie& operator=(std::initializer_list<std::pair<const Vector, T>> ilist);
		
		// Get allocator
		//! Returns the allocator associated with the container.
		Allocator get_allocator() const noexcept;
		
		// Element access
		//! Returns a reference to the mapped value of the element with key equivalent to key. If no such element exists, an exception of type std::out_of_range is thrown.
		template <class Vector> const T& at(const Vector& key) const;
		//! Returns a reference to the value that is mapped to a key equivalent to key, performing an insertion if such key does not already exist.
		template <class Vector> T& operator[](const Vector& key);
		//! Returns a reference to the value that is mapped to a key equivalent to key, performing an insertion if such key does not already exist.
		template <class Vector> T& operator[](Vector&& key);
		
		// Iterators
		//! iterator is an object that allows iteration of the container in the order of the sorted keys. Beware that any changes in the trie may invalidate the iterators and produce undefined behaviour.
		struct iterator : public iterator_actions{
			using iterator_actions::iterator_actions;
			//! Category of the iterator.
			using iterator_category = std::bidirectional_iterator_tag;
			//! Type of values accessed by the iterator.
			using value_type = std::pair<const std::vector<Key>&, T&>;
			//! Type of differences between iterators.
			using difference_type = std::size_t;
			//! Type of objects pointed to by the iterator.
			using pointer = NodeTrie*;
			//! Type of references to values.
			using reference = std::pair<const std::vector<Key>&, T&>&;
			//! References the key-value pair pointed at by the iterator.
			std::pair<const std::vector<Key>&, T&>& operator*();
			//! Dereferences the key-value pair pointed at by the iterator.
			std::pair<const std::vector<Key>&, T&>* operator->();
			//! Advances the iterator to the next key-value in the map.
			iterator& operator++();
			//! Advances the iterator to the next key-value in the map.
			iterator operator++(int);
			//! Regresses the iterator to the previous key-value in the map. If out of bounds, becomes equal to end().
			iterator& operator--();
			//! Regresses the iterator to the previous key-value in the map. If out of bounds, becomes equal to end().
			iterator operator--(int);
		};
		//! const_iterator is an object that allows iteration of the container in the order of the sorted keys for access only. Beware that any changes in the tree may invalidate the iterators and produce undefined behaviour.
		struct const_iterator : public iterator_actions{
			using iterator_actions::iterator_actions;
			//! Category of the iterator.
			using iterator_category = std::bidirectional_iterator_tag;
			//! Type of values accessed by the iterator.
			using value_type = const std::pair<const std::vector<Key>&, const T&>;
			//! Type of differences between iterators.
			using difference_type = std::size_t;
			//! Type of objects pointed to by the iterator.
			using pointer = NodeTrie*;
			//! Type of references to values.
			using reference = const std::pair<const std::vector<Key>&, const T&>&;
			//! References the key-value pair pointed at by the iterator.
			const std::pair<const std::vector<Key>&, const T&>& operator*() const;
			//! Dereferences the key-value pair pointed at by the iterator.
			const std::pair<const std::vector<Key>&, const T&>* operator->() const;
			//! Advances the iterator to the next key-value in the map.
			const_iterator& operator++();
			//! Advances the iterator to the next key-value in the map.
			const_iterator operator++(int);
			//! Regresses the iterator to the previous key-value in the map. If out of bounds, becomes equal to end().
			const_iterator& operator--();
			//! Regresses the iterator to the previous key-value in the map. If out of bounds, becomes equal to end().
			const_iterator operator--(int);
		};
		//! reverse_iterator is an object that allows iteration of the container in the inverse order of the sorted keys. Beware that any changes in the trie may invalidate the iterators and produce undefined behaviour.
		struct reverse_iterator : public iterator_actions{
			using iterator_actions::iterator_actions;
			//! Category of the iterator.
			using iterator_category = std::bidirectional_iterator_tag;
			//! Type of values accessed by the iterator.
			using value_type = std::pair<const std::vector<Key>&, T&>;
			//! Type of differences between iterators.
			using difference_type = std::size_t;
			//! Type of objects pointed to by the iterator.
			using pointer = NodeTrie*;
			//! Type of references to values.
			using reference = std::pair<const std::vector<Key>&, T&>&;
			//! Returns the underlying base iterator.
			inline iterator base() const;
			//! References the key-value pair pointed at by the iterator.
			std::pair<const std::vector<Key>&, T&>& operator*();
			//! Dereferences the key-value pair pointed at by the iterator.
			std::pair<const std::vector<Key>&, T&>* operator->();
			//! Advances the iterator to the previous key-value in the map.
			reverse_iterator& operator++();
			//! Advances the iterator to the previous key-value in the map.
			reverse_iterator operator++(int);
			//! Regresses the iterator to the next key-value in the map. If out of bounds, becomes equal to end().
			reverse_iterator& operator--();
			//! Regresses the iterator to the next key-value in the map. If out of bounds, becomes equal to end().
			reverse_iterator operator--(int);
		};
		//! const_reverse_iterator is an object that allows iteration of the container in the inverse order of the sorted keys for access only. Beware that any changes in the tree may invalidate the iterators and produce undefined behaviour.
		struct const_reverse_iterator : public iterator_actions{
			using iterator_actions::iterator_actions;
			//! Category of the iterator.
			using iterator_category = std::bidirectional_iterator_tag;
			//! Type of values accessed by the iterator.
			using value_type = const std::pair<const std::vector<Key>&, const T&>;
			//! Type of differences between iterators.
			using difference_type = std::size_t;
			//! Type of objects pointed to by the iterator.
			using pointer = NodeTrie*;
			//! Type of references to values.
			using reference = const std::pair<const std::vector<Key>&, const T&>&;
			//! Returns the underlying base iterator.
			inline const_iterator base() const;
			//! References the key-value pair pointed at by the iterator.
			const std::pair<const std::vector<Key>&, const T&>& operator*() const;
			//! Dereferences the key-value pair pointed at by the iterator.
			const std::pair<const std::vector<Key>&, const T&>* operator->() const;
			//! Advances the iterator to the previous key-value in the map.
			const_reverse_iterator& operator++();
			//! Advances the iterator to the previous key-value in the map.
			const_reverse_iterator operator++(int);
			//! Regresses the iterator to the next key-value in the map. If out of bounds, becomes equal to end().
			const_reverse_iterator& operator--();
			//! Regresses the iterator to the next key-value in the map. If out of bounds, becomes equal to end().
			const_reverse_iterator operator--(int);
		};
		//! Returns an iterator to the first element of the map. If the map is empty, the returned iterator will be equal to end().
		iterator begin() const;
		//! Returns a constant iterator to the first element of the map. If the map is empty, the returned iterator will be equal to end().
		const_iterator cbegin() const;
		//! Returns an iterator to the element following the last element of the map. This element acts as a placeholder; attempting to access it results in undefined behavior.
		iterator end() const noexcept;
		//! Returns a constant iterator to the element following the last element of the map. This element acts as a placeholder; attempting to access it results in undefined behavior.
		const_iterator cend() const noexcept;
		//! Returns a reverse iterator to the first element of the reversed map. It corresponds to the last element of the non-reversed map. If the map is empty, the returned iterator is equal to rend().
		reverse_iterator rbegin() const;
		//! Returns a constant reverse iterator to the first element of the reversed map. It corresponds to the last element of the non-reversed map. If the map is empty, the returned iterator is equal to rend().
		const_reverse_iterator crbegin() const;
		//! Returns a reverse iterator to the element following the last element of the reversed map. It corresponds to the element preceding the first element of the non-reversed map. This element acts as a placeholder, attempting to access it results in undefined behavior.
		reverse_iterator rend() const noexcept;
		//! Returns a constant reverse iterator to the element following the last element of the reversed map. It corresponds to the element preceding the first element of the non-reversed map. This element acts as a placeholder, attempting to access it results in undefined behavior.
		const_reverse_iterator crend() const noexcept;
		
		// Capacity
		//! Checks if the container has no elements, i.e. whether begin() == end().
		bool empty() const noexcept;
		//! Returns the number of elements in the container.
		std::size_t size() const noexcept;
		//! Returns the total space in memory.
		std::size_t memory() const noexcept;
		
		// Modifiers
		// Clear
		//! Erases all elements from the container. After this call, size() returns zero. Invalidates any references, pointers, or iterators referring to contained elements. Any past-the-end iterator remains valid.
		void clear() noexcept;
		// Insert
		//! Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent key. Returns a pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and a bool denoting whether the insertion took place.
		template <class Vector> std::pair<iterator, bool> insert(const std::pair<Vector, T>& value);
		// template<class P> std::pair<iterator, bool> insert(P&& value);
		//! Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent key. Returns a pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and a bool denoting whether the insertion took place.
		template <class Vector> std::pair<iterator, bool> insert(std::pair<Vector, T>&& value);
		//! Inserts value in the position as close as possible to hint. Returns an iterator to the inserted element, or to the element that prevented the insertion.
		template <class Vector> iterator insert(iterator hint, const std::pair<Vector, T>& value);
		// template<class P> iterator insert(const_iterator hint, P&& value);
		//! Inserts value in the position as close as possible to hint. Returns an iterator to the inserted element, or to the element that prevented the insertion.
		template <class Vector> iterator insert(iterator hint, std::pair<Vector, T>&& value);
		//! Inserts elements from range [first, last). If multiple elements in the range have keys that compare equivalent, it is unspecified which element is inserted.
		template <class InputIt> void insert(InputIt first, InputIt last);
		//! Inserts elements from initializer list ilist. If multiple elements in the range have keys that compare equivalent, it is unspecified which element is inserted.
		template <class Vector> void insert(std::initializer_list<std::pair<Vector, T>> ilist);
		// Insert or Assign
		//! If a key equivalent to k already exists in the container, assigns obj to the mapped_type corresponding to the key k. If the key does not exist, inserts the new value as if by insert.
		template <class Vector, class M> std::pair<iterator, bool> insert_or_assign(const Vector& k, M&& obj);
		//! If a key equivalent to k already exists in the container, assigns obj to the mapped_type corresponding to the key k. If the key does not exist, inserts the new value as if by insert.
		template <class Vector, class M> std::pair<iterator, bool> insert_or_assign(Vector&& k, M&& obj);
		//! If a key equivalent to k already exists in the container, assigns obj to the mapped_type corresponding to the key k. If the key does not exist, inserts the new value as if by insert.
		template <class Vector, class M> std::pair<iterator, bool> insert_or_assign(iterator hint, const Vector& k, M&& obj);
		//! If a key equivalent to k already exists in the container, assigns obj to the mapped_type corresponding to the key k. If the key does not exist, inserts the new value as if by insert.
		template <class Vector, class M> std::pair<iterator, bool> insert_or_assign(iterator hint, Vector&& k, M&& obj);
		// Erase
		//! Removes element at pos from the container. Returns iterator following the last removed element.
		iterator erase(iterator pos);
		//! Removes the elements in the range [first; last), which must be a valid range in *this.
		iterator erase(iterator first, iterator last);
		//! Removes the element (if one exists) with the key equivalent to key. Returns the number of elements removed (0 or 1).
		template <class Vector> std::size_t erase(const Vector& key);
		//! Removes the element (if one exists) with the key equivalent to key. Returns the number of elements removed (0 or 1).
		template <class Vector> std::size_t erase(Vector&& key);
		
		// Lookup
		// Count
		//! Returns the number of elements with key that compares equivalent to the specified key, which is either 1 or 0 since this container does not allow duplicates.
		template <class K> std::size_t count(const K& key) const;
		// Find
		//! Finds an element with key equivalent to key and returns an iterator to the element. If no such element is found, past-the-end (see end()) iterator is returned.
		template <class K> iterator find(const K& key);
		// template<class K> const_iterator find(const K& key) const;
		// Contains
		//! Checks if there is an element with key that compares equivalent to the value x.
		template <class K> bool contains(const K& key) const;
		// Bounds
		//! Returns an iterator pointing to the first element that compares not less (i.e. greater or equal) to the value x. If no such element is found, past-the-end (see end()) iterator is returned.
		template <class K> iterator lower_bound(const K& key);
		// template<class K> const_iterator lower_bound(const K& key);
		//! Returns an iterator pointing to the first element that compares greater to the value x. If no such element is found, past-the-end (see end()) iterator is returned.
		template <class K> iterator upper_bound(const K& key);
		// template<class K> const_iterator upper_bound(const K& key);
		//! Returns a range containing all elements with the given key in the container. The range is defined by two iterators, one pointing to the first element that is not less than key and another pointing to the first element greater than key. Alternatively, the first iterator may be obtained with lower_bound(), and the second with upper_bound().
		template <class K> std::pair<iterator,iterator> equal_range(const K& key);
		// template<class K> std::pair<const_iterator,const_iterator> equal_range(const K& key);
		// Non-member
		// Operators
		//! Checks if the contents of lhs and rhs are equal, that is, they have the same number of elements and each element in lhs compares equal with the element in rhs at the same position.
		template<class A,class B,class C,class D> friend bool operator==(const Trie<A,B,C,D>& lhs, const Trie<A,B,C,D>& rhs);
		//! Compares the contents of lhs and rhs lexicographically.
		template<class A,class B,class C,class D> friend std::strong_ordering operator<=>(const Trie<A,B,C,D>& lhs, const Trie<A,B,C,D>& rhs);
		// Other
		//! Erases all elements that satisfy the predicate pred from the container.
		template<class A,class B,class C,class D, class Pred> friend std::size_t std::erase_if(Trie<A,B,C,D>& trie, Pred pred);
		//! Specialized swapping function.
		template<class A,class B,class C,class D> friend void std::swap(Trie<A,B,C,D> &lhs, Trie<A,B,C,D>& rhs);
		
	private:
		// Nested class NodeTrie
		struct NodeTrie{
			union{
				NodeTrie *parent;
				NodeTrie *next;
			};
			Container children;
			std::pair<Key,T> data;
			bool valid;
			NodeTrie(NodeTrie* parent=0);
			NodeTrie(const Key& key, NodeTrie* parent=0);
			NodeTrie(Key&& key, NodeTrie* parent=0);
			~NodeTrie();
			static void copy(AllocatorNodes& alloc, NodeTrie* src, NodeTrie* dst);
			static std::size_t destroy(AllocatorNodes& alloc, NodeTrie* ptr);
		};
		AllocatorNodes alloc;
		NodeTrie data;
		NodeTrie *root;
		std::size_t counter, nCounter;
		template<class Vector> inline NodeTrie* get_forward(const Vector& key, std::size_t size);
		template<class Vector> inline NodeTrie* get_forward(Vector&& key, std::size_t size);
		template<class Vector> inline NodeTrie* get_forward(const Vector& key, std::size_t size, NodeTrie* hint, std::size_t idx);
		template<class Vector> inline NodeTrie* get_forward(Vector&& key, std::size_t size, NodeTrie* hint, std::size_t idx);
		template<class Vector> inline NodeTrie* find_node(Vector key, std::size_t size) const;
		template<class Vector> inline std::size_t len_array(const Vector& vec) const;
		inline std::size_t len_array(const Key* array) const;
		inline void remove_node(NodeTrie* ptr);
		// Iterator base structure
		struct iterator_actions{
			friend class Trie<Key,T,Container,Allocator>;
			iterator_actions();
			iterator_actions(const iterator_actions& other);
			iterator_actions(iterator_actions&& other);
			iterator_actions(NodeTrie* ptr, std::size_t size);
			iterator_actions& operator=(const iterator_actions& other);
			iterator_actions& operator=(iterator_actions&& other);
			bool operator==(const iterator_actions& other) const;
			bool operator!=(const iterator_actions& other) const;
			protected:
				NodeTrie* current;
				std::vector<Key> key;
				std::pair<const std::vector<Key>&, T&> data;
				static std::pair<const std::vector<Key>&, T&> nullvalue;
				static std::vector<Key> nullvalue_key;
				static T nullvalue_value;
				inline void update(NodeTrie* ptr);
				inline void next_element();
				inline void previous_element();
		};
		// Specialized copy/move calls
		inline void sp_copy(const Trie& other, std::true_type);
		inline void sp_copy(const Trie& other, std::false_type);
		inline void sp_move(Trie&& other, std::true_type) noexcept;
		inline void sp_move(Trie&& other, std::false_type);
};

		}
	}
}

#include "Trie.cpp"

#endif
