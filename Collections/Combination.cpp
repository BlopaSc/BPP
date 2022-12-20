#ifndef BPP_COLLECTIONS_SEQUENCE_COMBINATION_CPP
#define BPP_COLLECTIONS_SEQUENCE_COMBINATION_CPP

namespace bpp{
	namespace collections{
		namespace sequence{

// Constructors
constexpr Combination::Combination() noexcept{
	this->fromN = 0;
	this->chooseK = 0;
	this->positions = 0;
	this->crank = 0;
	this->cflag = 0;
	this->count = 1;
	this->combination = &this->combinations;
}
Combination::Combination(std::size_t n, std::size_t k, bool precalculate){
	this->positions = 0;
	this->positions = k ? new std::size_t[k+1] : 0;
	this->fromN = n;
	this->chooseK = k;
	this->count = this->combinations(n,k);
	this->first();
	this->combination = precalculate ? &this->static_combinations : &this->combinations;
	if(precalculate){ this->load_combinations(n,k); }
}
template <class It> Combination::Combination(const It& first, const It& last, std::size_t k, bool precalculate){
	std::size_t n = 0;
	this->positions = 0;
	this->positions = k ? new std::size_t[k+1] : 0;
	for(It tmp = first; tmp != last; ++tmp){ ++n; }
	this->fromN = n;
	this->chooseK = k;
	this->count = this->combinations(n,k);
	this->first();
	this->combination = precalculate ? &this->static_combinations : &this->combinations;
	if(precalculate){ this->load_combinations(n,k); }
}
Combination::Combination(const Combination& other){
	this->positions = 0;
	this->positions = new std::size_t[other.chooseK+1];
	this->fromN = other.fromN;
	this->chooseK = other.chooseK;
	this->count = other.count;
	this->crank = other.crank;
	this->cflag = other.cflag;
	for(std::size_t i=0; i<other.chooseK; i++){
		this->positions[i] = other.positions[i];
	}
	this->combination = other.combination;
}
Combination::Combination(const Combination& other, bool precalculate){
	this->positions = 0;
	this->positions = new std::size_t[other.chooseK+1];
	this->fromN = other.fromN;
	this->chooseK = other.chooseK;
	this->count = other.count;
	this->crank = other.crank;
	this->cflag = other.cflag;
	for(std::size_t i=0; i<other.chooseK; i++){
		this->positions[i] = other.positions[i];
	}
	this->combination = precalculate ? &this->static_combinations : &this->combinations;
	if(precalculate){ this->load_combinations(this->fromN, this->fromN); }
}
constexpr Combination::Combination(Combination&& other) noexcept{
	this->fromN = other.fromN;
	this->chooseK = other.chooseK;
	this->crank = other.crank;
	this->cflag = other.cflag;
	this->count = other.count;
	this->positions = other.positions;
	other.positions = 0;
	other.fromN = 0;
	other.chooseK = 0;
	other.count = 1;
	other.crank = 0;
	other.cflag = 0;
	this->combination = other.combination;
}
Combination::Combination(Combination&& other, bool precalculate){
	this->fromN = other.fromN;
	this->chooseK = other.chooseK;
	this->crank = other.crank;
	this->cflag = other.cflag;
	this->count = other.count;
	this->positions = other.positions;
	other.positions = 0;
	other.fromN = 0;
	other.chooseK = 0;
	other.count = 1;
	other.crank = 0;
	other.cflag = 0;
	this->combination = precalculate ? &this->static_combinations : &this->combinations;
	if(precalculate){ this->load_combinations(this->fromN, this->fromN); }
}

// Destructor
Combination::~Combination(){
	if(this->positions){ delete[] this->positions; }
}

// Assignment
typename Combination::Combination& Combination::operator=(const Combination& other){
	std::size_t *srcPtr = other.positions, *tmpPtr = this->positions;
	this->positions = new std::size_t[other.chooseK+1];
	this->fromN = other.fromN;
	this->chooseK = other.chooseK;
	this->count = other.count;
	this->crank = other.crank;
	this->cflag = other.cflag;
	this->combination = other.combination;
	for(std::size_t i=0; i<other.chooseK; i++){
		this->positions[i] = srcPtr[i];
	}
	if(tmpPtr){ delete[] tmpPtr; }
	return *this;
}
typename Combination::Combination& Combination::operator=(Combination&& other) noexcept{
	if(this->positions && this->positions!=other.positions){ delete[] this->positions; }
	this->fromN = other.fromN;
	this->chooseK = other.chooseK;
	this->crank = other.crank;
	this->cflag = other.cflag;
	this->count = other.count;
	this->positions = other.positions;
	this->combination = other.combination;
	other.positions = 0;
	other.fromN = 0;
	other.chooseK = 0;
	other.count = 1;
	other.crank = 0;
	other.cflag = 0;
	return *this;
}

// Capacity
constexpr std::size_t Combination::size() const noexcept{
	return this->count;
}
constexpr std::size_t Combination::n() const noexcept{
	return this->fromN;
}
constexpr std::size_t Combination::k() const noexcept{
	return this->chooseK;
}
constexpr std::size_t Combination::rank() const noexcept{
	return this->crank;
}
constexpr std::size_t Combination::flag() const noexcept{
	return this->cflag;
}
template <std::size_t N> constexpr void Combination::flag(std::bitset<N>& flag) const{
	flag.reset();
	for(std::size_t i=0; i<this->chooseK; i++){
		flag.set(this->positions[i]);
	}
}

// Element access
constexpr std::size_t Combination::at(std::size_t k) const{
	if(this->chooseK <= k){ throw std::out_of_range("bpp::collections::sequence::Combination::at"); }
	return this->positions[k];
}
constexpr std::size_t Combination::operator[](std::size_t k) const noexcept{
	return this->positions[k];
}

// Modifiers
void Combination::seek_rank(std::size_t rank) noexcept{
	std::size_t pos = this->fromN-1, k = this->chooseK, nf = 0;
	this->crank = rank;
	do{
		std::size_t tmp = this->combination(pos,k);
		if(tmp <= rank){
			this->positions[--k] = pos;
			rank -= tmp;
			nf |= (std::size_t)(1) << pos;
		}
	}while(pos-- && k);
	this->cflag = nf;
}
void Combination::seek_flag(std::size_t flag) noexcept{
	std::size_t count = 0, k = 0;
	for(std::size_t pos = 0; pos < this->fromN && k < this->chooseK; ++pos){
		if(flag & ((std::size_t)(1)<<pos)){
			this->positions[k] = pos;
			count += this->combination(pos, ++k);
		}
	}
	this->cflag = flag;
	this->crank = count;
}
template <std::size_t N> void Combination::seek_flag(std::bitset<N>& flag) noexcept{
	std::size_t count = 0, k=0, nf = 0;
	for(std::size_t pos = 0; pos < this->fromN && k < this->chooseK; ++pos){
		if(flag[pos]){
			this->positions[k] = pos;
			count += (this->combination(pos, ++k));
			nf |= ((std::size_t)(1)<<pos);
		}
	}
	this->cflag = nf;
	this->crank = count;
}

// Iteration
void Combination::first(){
	this->crank = 0;
	this->cflag = ((std::size_t)(-1)) >> ((sizeof(std::size_t)*8) - this->chooseK);
	for(std::size_t i=0; i<this->chooseK; i++){
		this->positions[i] = i;
	}
	this->positions[this->chooseK] = this->fromN;
}
void Combination::last(){
	this->crank = this->count - 1;
	this->cflag = (((std::size_t)(-1)) >> ((sizeof(std::size_t)*8) - this->chooseK)) << (this->fromN - this->chooseK);
	for(std::size_t i=0; i<this->chooseK; i++){
		this->positions[i] = this->fromN - this->chooseK + i;
	}
	this->positions[this->chooseK] = this->fromN;
}
constexpr std::size_t Combination::end() const noexcept{
	return this->count;
}
constexpr std::size_t Combination::rend() const noexcept{
	return (std::size_t)(-1);
}
Combination& Combination::operator++(){
	if(++this->crank < this->count){
		const std::size_t one = 1;
		std::size_t pos = 0;
		do{
			std::size_t tmp = this->positions[pos];
			if((tmp+1)!=this->positions[pos+1]){
				this->cflag = (this->cflag & (~(one<<tmp))) | (one<<(++this->positions[pos]));
				break;
			}
			this->positions[pos] = pos;
			this->cflag = (this->cflag & (~(one<<tmp))) | (one<<pos);
			++pos;
		}while(true);
	}else{
		this->crank = this->count;
	}
	return *this;
}
Combination Combination::operator++(int){
	Combination result(*this);
	++(*this);
	return result;
}
Combination& Combination::operator--(){
	if(--this->crank < this->count){
		const std::size_t one = 1;
		std::size_t pos = 0;
		std::size_t tmp, tmp2;
		while((tmp = this->positions[pos]) == pos){ ++pos; }
		this->cflag = (this->cflag & (~(one<<tmp))) | (one<<(tmp2 = --this->positions[pos]));
		while(pos--){
			tmp = this->positions[pos];
			this->cflag = (this->cflag & (~(one<<tmp))) | (one<<(this->positions[pos] = --tmp2));
		}
	}else{
		this->crank = -1;
	}
	return *this;
}
Combination Combination::operator--(int){
	Combination result(*this);
	--(*this);
	return result;
}
Combination& Combination::operator+=(std::size_t n){
	std::size_t tmp = this->crank + n;
	if(tmp < this->count){
		this->seek_rank(tmp);
	}else{
		this->last();
		++this->crank;
	}
	return *this;
}
Combination Combination::operator+(std::size_t n) const{
	Combination result(*this);
	return result += n;
}
Combination& Combination::operator-=(std::size_t n){
	std::size_t tmp = this->crank - n;
	if(tmp < this->count){
		this->seek_rank(tmp);
	}else{
		this->first();
		--this->crank;
	}
	return *this;
}
Combination Combination::operator-(std::size_t n) const{
	Combination result(*this);
	return result -= n;
}
std::ptrdiff_t Combination::operator-(const Combination& other) const noexcept{
	return this->crank - other.crank;
}

// Comparison
constexpr bool Combination::comparable(const Combination& other) const noexcept{
	return (this->fromN == other.fromN) && (this->chooseK == other.chooseK);
}

// Static methods
constexpr std::size_t Combination::max_size() noexcept{
	return sizeof(std::size_t)*8;
}
constexpr std::size_t Combination::combinations(std::size_t fromN, std::size_t chooseK) noexcept{
	std::size_t count = chooseK <= fromN ? 1 : 0;
	for(std::size_t i=0; i<chooseK;){
		count *= (fromN-i);
		count /= ++i;
	}
	return count;
}

// Private static methods
void Combination::load_combinations(std::size_t fromN, std::size_t chooseK){
	if(preCombinations.size() < fromN){
		preCombinations.reserve(fromN);
		do{
			preCombinations.push_back(std::vector<std::size_t>());
		}while(preCombinations.size() < fromN);
	}
	for(std::size_t i=0; i<fromN; i++){
		if(preCombinations[i].size() <= chooseK){
			preCombinations[i].reserve(chooseK+1);
			do{
				preCombinations[i].push_back(combinations(i, preCombinations[i].size()));
			}while(preCombinations[i].size() <= chooseK);
		}
	}
}
std::size_t Combination::static_combinations(std::size_t fromN, std::size_t chooseK){
	return preCombinations[fromN][chooseK];
}

// Non-member functions
// Operators
bool operator==(const Combination& lhs, const Combination& rhs){
	return lhs.crank == rhs.crank;
}
std::strong_ordering operator<=>(const Combination& lhs, const Combination& rhs){
	if(lhs.crank < rhs.crank){
		return std::strong_ordering::less;
	}else if(lhs.crank > rhs.crank){
		std::strong_ordering::greater;
	}
	return std::strong_ordering::equal;
}
bool operator==(const Combination& lhs, std::size_t rhs){
	return lhs.crank == rhs;
}
std::strong_ordering operator<=>(const Combination& lhs, std::size_t rhs){
	if(lhs.crank < rhs){
		return std::strong_ordering::less;
	}else if(lhs.crank > rhs){
		std::strong_ordering::greater;
	}
	return std::strong_ordering::equal;
}

std::vector<std::vector<std::size_t>> Combination::preCombinations;

		}
	}
}

#endif
