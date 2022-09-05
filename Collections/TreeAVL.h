#ifndef TREEAVL_H
#define TREEAVL_H
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
		TreeAVL(std::initializer_list<std::pair<const Key, T>> init/*, const Compare& comp = Compare(), const Allocator& alloc = Allocator()*/);
		//TreeAVL(std::initializer_list<value_type> init, const Allocator&);
		
		// Destructor
		~TreeAVL();
		
		// Assignment
		TreeAVL& operator=(const TreeAVL& other);
		TreeAVL& operator=(TreeAVL&& other);
		//TreeAVL& operator=(std::initializer_list<value_type> ilist);
		
		// Get allocator
		// Allocator get_allocator() const noexcept;
		
		// Element access
		T& at(const Key& key);
		T& operator[](const Key& key);
		T& operator[](Key&& key);
		
		// Iterators
		
		// Capacity
		bool empty() const;
		std::size_t size() const;
		
	private:
		// Nested class NodeAVL
		struct NodeAVL{
			NodeAVL *parent,*leftChild,*rightChild;
			Key key;
			T value;
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
		std::size_t count;
		inline NodeAVL* get_forward(const Key& key);
		inline NodeAVL* get_forward(Key&& key);
		inline void rebalance(NodeAVL *source);
		inline static NodeAVL* rotation_LL(NodeAVL* source);
		inline static NodeAVL* rotation_RR(NodeAVL* source);
		inline static NodeAVL* rotation_LR(NodeAVL* source);
		inline static NodeAVL* rotation_RL(NodeAVL* source);
};

	}
}
#endif
