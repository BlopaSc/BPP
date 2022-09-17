#ifndef TREEAVL_H
#define TREEAVL_H
#include <compare>			// std::strong_ordering
#include <functional>		// std::less
#include <initializer_list>	// std::initializer_list<>
#include <memory>			// std::allocator, std::allocator_traits<>
#include <stdexcept>		// std::out_of_range
#include <utility>			// std::pair, std::swap

namespace bpp{
	namespace map{

template <class Key, class T, class Compare = std::less<Key>,class Allocator = std::allocator<std::pair<const Key, T>>> class TreeAVL{
	private:
		struct NodeAVL;
		class iterator_base;
		// Rebind Allocator
		using AllocatorNodes = typename std::allocator_traits<Allocator>::rebind_alloc<NodeAVL>;
	public:
		// Empty initialization
		TreeAVL();
		// explicit TreeAVL(const Compare& comp, const Allocator& alloc = Allocator());
		// explicit TreeAVL(const Allocator& alloc);
		
		// Iterator initilization
		template<class InputIt> TreeAVL(InputIt first, InputIt last/*, const Compare& comp = Compare(), const Allocator& alloc = Allocator()*/);
		// template<class InputIt> TreeAVL(InputIt first, InputIt last, const Allocator& alloc);
		
		// Copy constructor
		TreeAVL(const TreeAVL& other);
		// TreeAVL(const TreeAVL& other, const Allocator& alloc);
		
		// Move constructor
		TreeAVL(TreeAVL&& other);
		// TreeAVL(TreeAVL&& other, const Allocator& alloc);
		
		// Initializer list initialization
		TreeAVL(std::initializer_list<std::pair<const Key, T>> ilist/*, const Compare& comp = Compare(), const Allocator& alloc = Allocator()*/);
		//TreeAVL(std::initializer_list<value_type> ilist, const Allocator&);
		
		// Destructor
		~TreeAVL();
		
		// Assignment
		TreeAVL& operator=(const TreeAVL& other);
		TreeAVL& operator=(TreeAVL&& other) noexcept;
		TreeAVL& operator=(std::initializer_list<std::pair<const Key, T>> ilist);
		
		// Get allocator
		// Allocator get_allocator() const noexcept;
		
		// Element access
		const T& at(const Key& key) const;
		T& operator[](const Key& key);
		T& operator[](Key&& key);
		
		// Iterators
		class iterator : public iterator_base{
			public:
				iterator(NodeAVL* init=0);
				std::pair<Key, T>& operator*() const;
				std::pair<Key, T>* operator->() const;
				iterator& operator++();
		};
		class const_iterator;
		class reverse_iterator : public iterator_base{
			public:
				reverse_iterator(NodeAVL* init=0);
				std::pair<Key, T>& operator*() const;
				std::pair<Key, T>* operator->() const;
				reverse_iterator& operator++();
		};
		class const_reverse_iterator;
		iterator begin() const noexcept;
		// const_iterator begin() const noexcept;
		// const_iterator cbegin() const noexcept;
		iterator end() const noexcept;
		// const_iterator end() const noexcept;
		// const_iterator cend() const noexcept;
		reverse_iterator rbegin() const noexcept;
		// const_reverse_iterator rbegin() const noexcept;
		// const_reverse_iterator crbegin() const noexcept;
		reverse_iterator rend() const noexcept;
		// const_reverse_iterator rend() const noexcept;
		// const_reverse_iterator crend() const noexcept;
		
		// Capacity
		bool empty() const noexcept;
		std::size_t size() const noexcept;
		
		// Modifiers
		// Clear
		void clear() noexcept;
		// Insert
		std::pair<iterator, bool> insert(const std::pair<const Key, T>& value);
		// template<class P> std::pair<iterator, bool> insert(P&& value);
		std::pair<iterator, bool> insert(std::pair<const Key, T>&& value);
		iterator insert(iterator_base hint, const std::pair<const Key, T>& value);
		// template<class P> iterator insert(const_iterator hint, P&& value);
		iterator insert(iterator_base hint, std::pair<const Key, T>&& value);
		template<class InputIt> void insert(InputIt first, InputIt last);
		void insert(std::initializer_list<std::pair<const Key, T>> ilist);
		// Insert or Assign
		template <class M> std::pair<iterator, bool> insert_or_assign(const Key& k, M&& obj);
		template <class M> std::pair<iterator, bool> insert_or_assign(Key&& k, M&& obj);
		template <class M> std::pair<iterator, bool> insert_or_assign(iterator_base hint, const Key& k, M&& obj);
		template <class M> std::pair<iterator, bool> insert_or_assign(iterator_base hint, Key&& k, M&& obj);
		// Erase
		iterator erase(iterator pos);
		// iterator erase(const_iterator);
		// iterator erase(iterator first, iterator last);
		// iterator erase(const_iterator_base first, const_iterator last);
		std::size_t erase(const Key& key);
		template<class K> std::size_t erase(K&& key);
		
		// Lookup
		// Count
		template<class K> std::size_t count(const K& key) const;
		// Find
		template<class K> iterator find(const K& key);
		// template<class K> const_iterator find(const K& key) const;
		// Contains
		template<class K> bool contains(const K& key) const;
		// Bounds
		template<class K> iterator lower_bound(const K& key);
		// template<class K> const_iterator lower_bound(const K& key);
		template<class K> iterator upper_bound(const K& key);
		// template<class K> const_iterator upper_bound(const K& key);
		template<class K> std::pair<iterator,iterator> equal_range(const K& key);
		// template<class K> std::pair<const_iterator,const_iterator> equal_range(const K& key);
		
		// Non-member
		// Operators
		template<class A,class B,class C,class D> friend bool operator==(const TreeAVL<A,B,C,D>& lhs, const TreeAVL<A,B,C,D>& rhs);
		template<class A,class B,class C,class D> friend std::strong_ordering operator<=>(const TreeAVL<A,B,C,D>& lhs, const TreeAVL<A,B,C,D>& rhs);
		// Other
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

	}
}
#endif
