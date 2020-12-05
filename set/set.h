#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <cassert>

const int BYTE_LEN = 8;
const uint8_t ONE = 1;

class BoolRank;


class BV
{
protected:
	int len_; // stores an amount of bits
	int size_; // stores an amount of bytes
	uint8_t* bv_ = nullptr;

	int bit2byte(int bits);
	int byte2bit(int bytes);
	int getBitIndex(int bit);
	int getByteIndex(int bit);

public:
	BV(int len = 1, bool value = false);
	BV(int len, const uint8_t* vector); // len - amount of bits
	BV(const BV& bv);
	~BV();

	int getLen() const { return len_; }
	int getSize() const { return size_; }
	int getWeight() const;

	void set(); // all set
	void set(int index, int amount = 1);
	void reset(); // all reset
	void reset(int index, int amount = 1);
	void update(bool value, int index, int amount = 1); // change 'amount' elements to 'value' starting from 'index'
	void inverse();
	void inverse(int index, int amount = 1);

	int getWeight();
	bool empty();
	bool operator== (const BV& bv);
	bool operator!= (const BV& bv);

	BoolRank& operator[] (int index); // special return value
	BV& operator= (const BV& bv);
	BV operator~();

	BV operator|    (const BV& bv);
	BV operator&    (const BV& bv);
	BV operator^    (const BV& bv);
	BV operator<<   (int shift);
	BV operator>>   (int shift);

	BV& operator|=  (const BV& bv);
	BV& operator&=  (const BV& bv);
	BV& operator^=  (const BV& bv);
	BV& operator<<= (int shift);
	BV& operator>>= (int shift);



	friend std::ostream& operator<<(std::ostream& out, const BV& bv);
	friend std::istream& operator>>(std::istream& in, BV& bv);
};



class BoolRank
{
private:
	BV& bv_;
	int index_;
	bool value_;

public:
	BoolRank(BV& bv, int index, bool value) :
		bv_(bv), index_(index), value_(value)
	{
	}

	explicit operator bool() const { return value_; }
	bool operator<   (bool value) const { return value_ < value; }
	bool operator<=  (bool value) const { return value_ <= value; }
	bool operator>   (bool value) const { return value_ > value; }
	bool operator>=  (bool value) const { return value_ >= value; }
	bool operator==  (bool value) const { return value_ == value; }
	bool operator!=  (bool value) const { return value_ != value; }

	bool operator<  (const BoolRank& br) const { return value_ < br.value_; }
	bool operator<= (const BoolRank& br) const { return value_ <= br.value_; }
	bool operator>  (const BoolRank& br) const { return value_ > br.value_; }
	bool operator>= (const BoolRank& br) const { return value_ >= br.value_; }
	bool operator== (const BoolRank& br) const { return value_ == br.value_; }
	bool operator!= (const BoolRank& br) const { return value_ != br.value_; }

	BoolRank& operator= (bool value)
    {
        value_ = value;
        bv_.update(value_, index_);
        return *this;
    }
	BoolRank& operator= (int value)
    {
        value_ = bool(value);
        bv_.update(value_, index_);
        return *this;
    }
	BoolRank& operator= (char ch)
    {
        value_ = (ch != '0') ? true : false;
        bv_.update(value_, index_);
        return *this;
    }

	// See the definition in BV.cpp
	BoolRank& operator= (const BoolRank& br);

	BoolRank operator& (const BoolRank& br);
	BoolRank operator| (const BoolRank& br);
	BoolRank operator^ (const BoolRank& br);

	BoolRank& operator&= (const BoolRank& br);
	BoolRank& operator|= (const BoolRank& br);
	BoolRank& operator^= (const BoolRank& br);

	friend std::ostream& operator<< (std::ostream&, const BoolRank& br);
	friend std::istream& operator>> (std::istream&, BoolRank& br);
};


const int SET_SIZE = 256;
class Set : public BV
{
public:
	Set(bool value = false) : BV(::SET_SIZE, value)
	{
	}

	~Set()
	{
	}

	bool has(const unsigned char ch) const;
	char min();
	char max();
	int capacity();

	// operators: =, ==, !=, |, |=, &, &= done in BV.h

	Set operator~();

	Set operator/ (const Set& set);
	Set& operator/= (const Set& set);

	Set operator+ (char ch);
	Set& operator+= (char ch);

	Set operator- (char ch);
	Set& operator-= (char ch);



	friend std::ostream& operator<<(std::ostream& out, const Set& set);
	friend std::istream& operator>>(std::istream& in, Set& set);
};

