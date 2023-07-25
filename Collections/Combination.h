#ifndef BPP_COLLECTIONS_ITERATION_COMBINATION_H
#define BPP_COLLECTIONS_ITERATION_COMBINATION_H
#include <bitset>			// std::bitset<>
#include <compare>			// std::strong_ordering
#include <stdexcept>		// std::out_of_range
#include <utility>			// std::move
#include <vector>			// std::vector
#include "Serialize.h"

namespace bpp{
	namespace collections{
		namespace iteration{

//! Combination is an iterator structure that allows iteration over the sequence of combinations from N choose K.
class Combination{
	public:
		//! Category of the iterator.
		using iterator_category = std::random_access_iterator_tag;
		//! Type of values accessed by the iterator.
		using value_type = std::size_t;
		//! Type of differences between iterators.
		using difference_type = std::ptrdiff_t;
		//! Type of objects pointed to by the iterator.
		using pointer = std::size_t*;
		//! Type of references to values.
		using reference = std::size_t&;
		
		// Constructors
		//! Constructs an empty iterator.
		constexpr Combination() noexcept;
		//! Constructors an iterator for the combination from N choose K. Specifies whether to precalculate values for the seek functions.
		Combination(std::size_t n, std::size_t k, bool precalculate = false);
		//! Constructs the iterator in the range [first, last). Specifies the number of elements to choose from the given range. Specifies whether to precalculate values for the seek functions.
		template <class It> Combination(const It& first, const It& last, std::size_t k, bool precalculate);
		//! Copy constructor. Constructs the iterator with the copy of the contents of other.
		Combination(const Combination& other);
		//! Copy constructor. Constructs the iterator with the copy of the contents of other. Specifies whether to precalculate values for the seek functions.
		Combination(const Combination& other, bool precalculate);
		//! Move constructor. Constructs the iterator with with the contents of other using move semantics. After the move, other is in a valid but unspecified state.
		constexpr Combination(Combination&& other) noexcept;
		//! Move constructor. Constructs the iterator with with the contents of other using move semantics. After the move, other is in a valid but unspecified state. Specifies whether to precalculate values for the seek functions.
		Combination(Combination&& other, bool precalculate);
		
		// Destructor
		//! Destructs the combination. The memory used as part of the combination is released.
		~Combination();
		
		// Assignment
		//! Copy assignment operator. Replaces the iterator with a copy of the contents of other.
		Combination& operator=(const Combination& other);
		//! Move assignment operator. Replaces the iterator with those of other using move semantics (i.e. the data in other is moved from other into this structure). After the move, other is in a valid but unspecified state.
		Combination& operator=(Combination&& other) noexcept;
		
		// Capacity
		//! Returns the total number of different combinations (from N choose K) belonging to the iterator.
		constexpr std::size_t size() const noexcept;
		//! Returns the number of elements to choose from in the combination.
		constexpr std::size_t n() const noexcept;
		//! Returns the number of elements chosen from the available ones.
		constexpr std::size_t k() const noexcept;
		//! Returns the rank of the current combination. Each different combination is assigned a rank in the range [0, size()).
		constexpr std::size_t rank() const noexcept;
		//! Returns the bitflag that represents the current combination as a std::size_t value. Note that if n exceeds the bits of an object std::size_t then the result may be invalid.
		constexpr std::size_t flag() const noexcept;
		//! Stores the bitflag that represents the current combination in a bitset object. The number of bits in the bitset should be equal or greater to n.
		template <std::size_t N> constexpr void flag(std::bitset<N>& flag) const;
		
		// Element access
		//! Returns a reference to the iterator value for the k-th chosen element of the combination. If no such element exists, an exception of type std::out_of_range is thrown.
		constexpr std::size_t at(std::size_t k) const;
		//! Returns a reference to the iterator value for the k-th chosen element of the combination. No bounds checking is performed.
		constexpr std::size_t operator[](std::size_t k) const noexcept;
		
		//! Sets the iterator to the combination identified by rank. If an invalid rank is provided undefined behaviour may occur.
		void seek_rank(std::size_t rank) noexcept;
		//! Sets the iterator to the combination with bitflag flag. If an invalid bitflag is provided undefined behaviour may occur.
		void seek_flag(std::size_t flag) noexcept;
		//! Sets the iterator to the combination with bitflag flag. If an invalid bitflag is provided undefined behaviour may occur.
		template <std::size_t N> void seek_flag(std::bitset<N>& flag) noexcept;
		
		// Iteration
		//! Sets the iterator to the first combination.
		void first();
		//! Sets the iterator to the last combination.
		void last();
		//! Returns the rank value following that of the last element of the combination.
		constexpr std::size_t end() const noexcept;
		//! Returns the rank value preceding that of the first element of the combination.
		constexpr std::size_t rend() const noexcept;
		//! Advances the iterator to the next combination in the sequence.
		Combination& operator++();
		//! Advances the iterator to the next combination in the sequence. Returns a copy of the original iterator.
		Combination operator++(int);
		//! Regresses the iterator to the previous combination in the sequence.
		Combination& operator--();
		//! Regresses the iterator to the previous combination in the sequence. Returns a copy of the original iterator.
		Combination operator--(int);
		//! Advances the iterator to the combination n steps ahead in the sequence. Applies a seek_rank call with constant duration for a given nCk.
		Combination& operator+=(std::size_t n);
		//! Returns a new iterator to the combination n steps ahead in the sequence. Applies a seek_rank call with constant duration for a given nCk.
		Combination operator+(std::size_t n) const;
		//! Regresses the iterator to the combination n steps before in the sequence. Applies a seek_rank call with constant duration for a given nCk.
		Combination& operator-=(std::size_t n);
		//! Returns a new iterator to the combination n steps before in the sequence. Applies a seek_rank call with constant duration for a given nCk.
		Combination operator-(std::size_t n) const;
		//! Returns the distance between the ranks of the two combinations.
		std::ptrdiff_t operator-(const Combination& other) const noexcept;
		
		// Comparison
		//! Checks whether both objects iterate over the same set of elements and represent the same nCk combination. The base iterator must be comparable.
		constexpr bool comparable(const Combination& other) const noexcept;
		
		// Static methods
		//! Returns the maximum n size for a bitflag which can be encoded in a std::size_t value.
		static constexpr std::size_t max_size() noexcept;
		//! Returns the number of possible combinations produced by choosing K elements from a set of size N. This is the cardinality of the sequence of combinations given by nCk.
		static constexpr std::size_t combinations(std::size_t fromN, std::size_t chooseK) noexcept;
		
		// Non-member
		// Operators
		//! Checks if the iterators share the same current rank. Assumes the combinations point to the same set of elements and represent the same nCk combination.
		inline friend bool operator==(const Combination& lhs, const Combination& rhs);
		//! Compares the ranks of the left iterator against the rank of the right iterator. Assumes the combinations point to the same set of elements and represent the same nCk combination.
		inline friend std::strong_ordering operator<=>(const Combination& lhs, const Combination& rhs);
		//! Checks if the current rank of the iterator is equal to an integer.
		inline friend bool operator==(const Combination& lhs, std::size_t rhs);
		//! Compares the current rank of the iterator against an integer.
		inline friend std::strong_ordering operator<=>(const Combination& lhs, std::size_t rhs);
		// Friendship
		//! Specialization of the Serialize template to support Combination objects.
		template<typename Buff> friend struct bpp::collections::serialize::Serialize;
		//! Specialization of the Deserialize template to support Combination objects.
		template<typename Buff> friend struct bpp::collections::serialize::Deserialize;
		
	private:
		std::size_t* positions;
		std::size_t fromN, chooseK, count;
		std::size_t crank;
		std::size_t cflag;
		std::size_t (*combination)(std::size_t, std::size_t);
		// Static private methods and shared structures
		static std::vector<std::vector<std::size_t>> preCombinations;
		static void load_combinations(std::size_t fromN, std::size_t chooseK);
		static std::size_t static_combinations(std::size_t fromN, std::size_t chooseK);
};

		}
		namespace serialize{

//! Specialization of the Serialize template to support Combination objects.
template<typename Buff> struct Serialize<Buff,bpp::collections::iteration::Combination>{
	std::size_t operator()(Buff& buffer, const bpp::collections::iteration::Combination& obj) const;
};
//! Specialization of the Deserialize template to support Combination objects.
template<typename Buff> struct Deserialize<Buff,bpp::collections::iteration::Combination>{
	std::size_t operator()(Buff& buffer, bpp::collections::iteration::Combination& obj) const;
};

		}
	}
}

#endif
