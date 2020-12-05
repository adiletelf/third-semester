#include "set.h"

// private method
int BV::bit2byte(int bits)
{
	return (bits + BYTE_LEN - 1) / BYTE_LEN;
}

// private method
int BV::byte2bit(int bytes)
{
	return bytes * BYTE_LEN;
}

// private method
int BV::getBitIndex(int bit)
{
	return (bit + BYTE_LEN) % BYTE_LEN;
}

// private method
int BV::getByteIndex(int bit)
{
	return bit / BYTE_LEN;
}


BV::BV(int len, bool value)
{
	if (len < 0)
		len = 1;

	len_ = len;
	size_ = bit2byte(len);
	bv_ = new uint8_t[size_];

	uint8_t fill = value ? 0xFF : 0x00;
	for (int i = 0; i < size_; ++i)
		bv_[i] = fill;
}


BV::BV(int len, const uint8_t* vector)
{
	if (len < 0)
	{
		std::cout << "Incorrect len of vector\n";
		*this = BV();
		return;
	}

	len_ = len;
	size_ = bit2byte(len);
	bv_ = new uint8_t[size_];
	for (int i = 0; i < size_; ++i)
	{
		bv_[i] = vector[i];
	}
}


BV::BV(const BV& bv)
{
	delete[] bv_;

	len_ = bv.len_;
	size_ = bv.size_;
	bv_ = new uint8_t[size_];
	for (int i = 0; i < size_; ++i)
		bv_[i] = bv.bv_[i];


}


BV::~BV()
{
	if (bv_)
	{
		delete[] bv_;
		bv_ = nullptr;
	}

}


int BV::getWeight() const
{
	int weight = 0;

	int byteIndex, bitIndex;
	for (int bit = 0; bit < len_; ++bit)
	{
		byteIndex = bit / BYTE_LEN;
		bitIndex = (bit + BYTE_LEN) % BYTE_LEN;

		if ((bv_[byteIndex] & (ONE << bitIndex)) > 0)
			++weight;

	}
	return weight;
}


void BV::set()
{
	uint8_t full = 0xFF;

	for (int i = 0; i < size_; ++i)
	{
		bv_[i] = full;
	}
}


void BV::set(int index, int amount)
{
	if (index < 0 || index >= len_ || amount < 0)
	{
		//std::cout << "Incorrect try.\n";
		assert( (index >= 0 || index < len_ || amount >= 0) && "Incorrect try.");

		return;
	}

	for (int bit = index, count = 0; bit < len_ && count != amount; ++bit)
	{
		int byteIndex = bit / BYTE_LEN;
		int bitIndex = (bit + BYTE_LEN) % BYTE_LEN;

		bv_[byteIndex] |= (ONE << bitIndex);
		count++;
	}
}


void BV::reset()
{
	uint8_t full = 0x00;

	for (int i = 0; i < size_; ++i)
	{
		bv_[i] = full;
	}
}


void BV::reset(int index, int amount)
{
	if (index < 0 || index >= len_)
	{
		std::cout << "Incorrect index.\n";
		return;
	}

	for (int bit = index, count = 0; bit < len_ && count != amount; ++bit)
	{
		int byteIndex = bit / BYTE_LEN;
		int bitIndex = (bit + BYTE_LEN) % BYTE_LEN;

		bv_[byteIndex] &= ~(ONE << bitIndex);
		count++;
	}
}


void BV::update(bool value, int index, int amount)
{
	if (index < 0 || index >= len_ || amount < 0)
	{
		std::cout << "Incorrect try to update." << '\n';
		return;
	}

	if (value)
		set(index, amount);
	else
		reset(index, amount);
}


void BV::inverse()
{
	for (int bit = 0; bit < len_; ++bit)
	{
		int byteIndex = bit / BYTE_LEN;
		int bitIndex = (bit + BYTE_LEN) % BYTE_LEN;

		bv_[byteIndex] ^= (ONE << bitIndex);
	}
}


void BV::inverse(int index, int amount)
{
	if (index < 0 || index >= len_ || amount < 0)
	{
		std::cout << "Incorrect index.\n";
		return;
	}

	int byteIndex = index / BYTE_LEN;
	int bitIndex = (index + BYTE_LEN) % BYTE_LEN;
	for (int bit = index, count = 0; bit < len_ && count != amount; ++bit)
	{
		byteIndex = bit / BYTE_LEN;
		bitIndex = (bit + BYTE_LEN) % BYTE_LEN;

		bv_[byteIndex] ^= (ONE << bitIndex);
		count++;
	}

}


int BV::getWeight()
{
	int weight = 0;

	bool value;
	for (int bit = 0; bit < len_; ++bit)
	{
		int byteIndex = bit / BYTE_LEN;
		int bitIndex = bit % BYTE_LEN;
		value = ((bv_[byteIndex] & (ONE << bitIndex)) > 0);

		if (value)
			++weight;

	}

	return weight;
}


bool BV::empty()
{
	for (int bit = len_; bit < size_ * BYTE_LEN; ++bit)
	{
		reset(bit);
	}

	int weight = 0;
	for (int byte = 0; byte < size_; ++byte)
	{
		weight += (int)bv_[byte];
	}

	if (weight)
		return false;
	else
		return true;
}


bool BV::operator== (const BV& bv)
{
	if (len_ != bv.len_)
		return false;

	for (int byte = 0; byte < size_ - 1; ++byte)
	{
		if (bv_[byte] != bv.bv_[byte])
			return false;
	}

	for (int bit = (size_ - 1) * BYTE_LEN; bit < len_; ++bit)
	{
		int byteIndex = bit / BYTE_LEN;
		int bitIndex = bit % BYTE_LEN;

		bool value1 = ((bv_[byteIndex] & (ONE << bitIndex)) > 0);
		bool value2 = ((bv.bv_[byteIndex] & (ONE << bitIndex)) > 0);

		if (value1 != value2)
			return false;
	}

	return true;
}


bool BV::operator!= (const BV& bv)
{
	return !(*this == bv);
}


BoolRank& BV::operator[] (int index)
{
	if (index < 0 || index >= size_)
	{
		std::cout << "Incorrect size." << '\n';
		index = 0;
	}

	bool value;
	int byteIndex = index / BYTE_LEN;
	int bitIndex = (index + BYTE_LEN) % BYTE_LEN;
	value = ((bv_[byteIndex] & (ONE << bitIndex)) > 0);

	BoolRank br(*this, index, value);
	// BoolRank* br = new BoolRank(*this, index, value);

	return br;
}



BV& BV::operator= (const BV& bv)
{
	if (this == &bv)
		return *this;

	delete[] bv_;
	len_ = bv.len_;
	size_ = bv.size_;
	bv_ = new uint8_t[size_];

	for (int byte = 0; byte < size_; ++byte)
	{
		bv_[byte] = bv.bv_[byte];
	}

	return *this;
}


BV BV::operator~()
{
	BV result(*this);
	for (int i = 0; i < size_; ++i)
		result.bv_[i] = ~result.bv_[i];

	return result;
}




BV BV::operator& (const BV& bv)
{
	BV result(*this);
	result &= bv;
	return result;
}


BV BV::operator| (const BV& bv)
{
	BV result(*this);
	result |= bv;
	return result;
}


BV BV::operator^ (const BV& bv)
{
	BV result(*this);
	result ^= bv;
	return result;
}


BV BV::operator<< (int shift)
{
	BV result(*this);
	result <<= shift;
	return result;
}


BV BV::operator>> (int shift)
{
	BV result(*this);
	result >>= shift;
	return result;
}


BV& BV::operator&= (const BV& bv)
{
	for (int byte = 0; byte < std::min(size_, bv.size_); ++byte)
	{
		bv_[byte] &= bv.bv_[byte];
	}

	// �������� �����, �.�. value & 0 == 0
	if (size_ > bv.size_)
	{
		for (int byte = bv.size_; byte < size_; ++byte)
			bv_[byte] = 0;
	}

	return *this;
}



BV& BV::operator|= (const BV& bv)
{
	for (int byte = 0; byte < std::min(size_, bv.size_); ++byte)
	{
		bv_[byte] |= bv.bv_[byte];
	}

	// if realSize_ > second.realSize_, �� �������� ����� this �� ���������, �.�. value | 0 == value
	// ������ ����� realSize_ < second.realSize_ ��� �� ����������, ���� ����� �������� �� �������� ������ this
	return *this;
}


BV& BV::operator^= (const BV& bv)
{
	for (int byte = 0; byte < std::min(size_, bv.size_); ++byte)
	{
		bv_[byte] ^= bv.bv_[byte];
	}

	// if (realSize_ > second.realSize_), �� �������� ^ ������ �� �������
	// ��� ��� value ^ 0 == value

	return *this;
}


BV& BV::operator<<= (int shift)
{
	if (shift < 0)
	{
		std::cout << "Incorrect value of 'shift.'\n";
		return *this;
	}

	if (shift == 0)
	{
		return *this;
	}

	if (shift >= len_)
	{
		reset();
		return *this;
	}

	bool value;
	int bitIndex, byteIndex, shiftBitIndex, shiftByteIndex;
	for (int bit = shift; bit < len_ - shift; ++bit)
		//for (int bit = len_ - 1 - shift; bit >= 0; --bit)
	{
		byteIndex = bit / BYTE_LEN;
		bitIndex = (bit + BYTE_LEN) % BYTE_LEN;
		shiftByteIndex = (bit - shift) / BYTE_LEN;
		shiftBitIndex = (bit - shift + BYTE_LEN) % BYTE_LEN;

		value = ((bv_[byteIndex] & (ONE << bitIndex)) > 0);

		if (value)
		{
			bv_[shiftByteIndex] |= (ONE << shiftBitIndex);
		}
		else
		{
			bv_[shiftByteIndex] &= ~(ONE << shiftBitIndex);
		}

	}

	// �������� ��������� ����
	int bit;
	for (bit = len_ - shift; bit < len_ && bit % BYTE_LEN; ++bit)
	{
		byteIndex = bit / BYTE_LEN;
		bitIndex = (bit + BYTE_LEN) % BYTE_LEN;

		bv_[byteIndex] &= ~(ONE << bitIndex);
	}

	for (byteIndex = bit / BYTE_LEN; byteIndex < size_; ++byteIndex)
	{
		bv_[byteIndex] = 0;
	}

	return *this;
}


BV& BV::operator>>= (int shift)
{
	if (shift < 0)
	{
		std::cout << "Incorrect value of 'shift.'\n";
		return *this;
	}

	if (shift == 0)
	{
		return *this;
	}

	if (shift >= len_)
	{
		reset();
		return *this;
	}

	bool value;
	int bitIndex, byteIndex, shiftBitIndex, shiftByteIndex;
	for (int bit = shift; bit < len_ - 1; ++bit)
	{
		byteIndex = bit / BYTE_LEN;
		bitIndex = (bit + BYTE_LEN) % BYTE_LEN;
		shiftByteIndex = (bit - shift) / BYTE_LEN;
		shiftBitIndex = (bit - shift + BYTE_LEN) % BYTE_LEN;

		value = ((bv_[byteIndex] & (ONE << bitIndex)) > 0);

		if (value)
		{
			bv_[shiftByteIndex] |= (ONE << shiftBitIndex);
		}
		else
		{
			bv_[shiftByteIndex] &= ~(ONE << shiftBitIndex);
		}
	}

	// �������� ������ ����
	for (int bit = 0; bit < shift; ++bit)
	{
		byteIndex = bit / BYTE_LEN;
		bitIndex = (bit + BYTE_LEN) % BYTE_LEN;

		bv_[byteIndex] &= ~(ONE << bitIndex);
	}

	return *this;
}


std::ostream& operator<<(std::ostream& out, const BV& bv)
{
	bool value;
	//for (int i = bv.len_ - 1; i >= 0; --i)
	for (int i = bv.len_; i >= 0; --i)
	{
		int byte = i / BYTE_LEN;
		int bit = (i + BYTE_LEN) % BYTE_LEN;
		//int bit = (i + BYTE_LEN - 1) / BYTE_LEN;

		value = (bv.bv_[byte] & (ONE << bit)) > 0;
		out << value;
	}

	return out;
}


std::istream& operator>>(std::istream& in, BV& bv)
{
	delete bv.bv_;

	std::string str;
	in >> str;

	int length = str.size();
	int size = (length + BYTE_LEN - 1) / BYTE_LEN;

	uint8_t* vec = new uint8_t[size];
	uint8_t zero = 0x00;
	for (int i = 0; i < size; ++i)
		vec[i] = zero;

	for (int bit = 0, index = length - 1; bit < length && index >= 0; ++bit, --index)
	{
		int byteIndex = bit / BYTE_LEN;
		int bitIndex = (bit + BYTE_LEN) % BYTE_LEN;

		if (str[index] == '0')
			continue;

		vec[byteIndex] |= (ONE << bitIndex);
	}

	bv.len_ = length;
	bv.size_ = size;
	bv.bv_ = vec;
	return in;
}

// ------------------------------------------------------------------------------------------------
// class BoolRank START

BoolRank& BoolRank::operator= (const BoolRank& br)
{
	value_ = br.value_;
	bv_.update(value_, index_);
	return *this;
}

BoolRank BoolRank::operator& (const BoolRank& br)
{
	BoolRank result(*this);
	result &= br;
	return result;
}


BoolRank BoolRank::operator| (const BoolRank& br)
{
	BoolRank result(*this);
	result |= br;
	return result;
}


BoolRank BoolRank::operator^ (const BoolRank& br)
{
	BoolRank result(*this);
	result ^= br;
	return result;
}


BoolRank& BoolRank::operator&= (const BoolRank& br)
{
	value_ &= br.value_;
	bv_.update(value_, index_);
	return *this;
}


BoolRank& BoolRank::operator|= (const BoolRank& br)
{
	value_ |= br.value_;
	bv_.update(value_, index_);
	return *this;
}


BoolRank& BoolRank::operator^= (const BoolRank& br)
{
	value_ ^= br.value_;
	bv_.update(value_, index_);
	return *this;
}


std::ostream& operator<< (std::ostream& out, const BoolRank& br)
{
	out << br.value_;
	return out;
}


std::istream& operator>> (std::istream& in, BoolRank& br)
{
	bool value;
	in >> value;
	br.value_ = value;
	br.bv_.update(br.value_, br.index_);

	return in;
}



bool Set::has(const unsigned char ch) const
{
	int index = (int)ch;

	int byte = index / BYTE_LEN;
	int bit = index % BYTE_LEN;

	return ((bv_[byte] & (ONE << bit)) > 0);
}


char Set::min()
{
	if (empty())
	{
		std::cout << "Set is empty\n";
		return (unsigned char)0;
	}


	uint8_t min = ~0;

	for (int i = 0; i < SET_SIZE; ++i)
	{
		if (has(i))
		{
			return min;
		}
	}

	// doesn't needed
	return min;
}


char Set::max()
{
	if (empty())
	{
		std::cout << "Set is empty\n";
		return (unsigned char)255;
	}


	uint8_t max = 0;

	for (int i = 255; i >= 0; --i)
	{
		if (has(i))
		{
			return max;
		}
	}

	// doesn't needed
	return max;
}


Set Set::operator~()
{
	Set result(*this);
	for (int i = 0; i < size_; ++i)
		result.bv_[i] = ~result.bv_[i];

	return result;
}



int Set::capacity()
{
	if (empty())
		return 0;

	return getWeight();
}


Set Set::operator/ (const Set& set)
{
	Set result(*this);

	for (int byte = 0; byte < size_; ++byte)
	{
		uint8_t mask = ~set.bv_[byte];
		result.bv_[byte] &= mask;
	}

	return result;
}


Set& Set::operator/= (const Set& set)
{
	for (int byte = 0; byte < size_; ++byte)
	{
		uint8_t mask = ~set.bv_[byte];
		bv_[byte] &= mask;
	}

	return *this;
}


Set Set::operator+ (char ch)
{
	Set result(*this);
	result.set((int)ch);

	return result;
}


Set& Set::operator+= (char ch)
{
	set((int)ch);
	return *this;
}


Set Set::operator- (char ch)
{
	Set result(*this);
	result.reset((int)ch);

	return result;
}


Set& Set::operator-= (char ch)
{
	reset((int)ch);
	return *this;
}




std::ostream& operator<< (std::ostream& out, const Set& set)
{
	bool value;
	for (int i = 0, count = 0; i < set.len_; ++i, ++count)
	{
		int byte = i / BYTE_LEN;
		int bit = (i + BYTE_LEN) % BYTE_LEN;

		value = (set.bv_[byte] & (ONE << bit)) > 0;

		if (value)
		{
			out << (unsigned char)i << " = " << i << '\n';
			//out << (unsigned char)i << " = " << i << '\t';
		}


	}

	out << "\n";
	return out;
}

std::istream& operator>> (std::istream& in, Set& set)
{
	set.reset();
	std::string str;
	std::cout << "Enter a string:" << std::endl;
	std::getline(in, str);

	for (size_t i = 0; i < str.size(); ++i)
	{
		int index = (int)str[i];
		set.set(index);
	}

	return in;

}
