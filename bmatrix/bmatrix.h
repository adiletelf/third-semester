#pragma once
#include "bvector.h"

class BM
{
private:
	int rows_;
	int cols_;
	//std::vector<BV*> bm_;
	BV* bm_ = nullptr;

public:
	BM(int rows = 0, int cols = 0, bool value = false);
	BM(const BM& bm);
	BM(int rows, int cols, const uint8_t** matrix);
	~BM();

	int getRows() const { return rows_; }
	int getCols() const { return cols_; }
	int getWeight() const;
	int getWeight(int row) const;

	void set();
	void set(int row);
	void set(int row, int col);

	void reset();
	void reset(int row);
	void reset(int row, int col);
	void update(int row, int col, bool value, int amount = 1);

	void inverse();
	void inverse(int row);
	void inverse(int row, int col, int amount = 1);

	BV conjunctionAll();
	BV disjunctionAll();

	BV& operator[](int row); // returns BV object
	BM& operator= (const BM& bm);

	BM operator~(void);
	BM operator&(const BM&);
	BM operator|(const BM&);
	BM operator^(const BM&);

	BM& operator&=(const BM&);
	BM& operator|=(const BM&);
	BM& operator^=(const BM&);


	friend std::ostream& operator<<(std::ostream& out, const BM& bm);
	friend std::istream& operator>>(std::istream& in, BM& bm);
};
