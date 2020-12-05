#include "bmatrix.h"

BM::BM(int rows, int cols, bool value)
{
	//if (rows_ < 0 || cols < 0 || rows >= rows_ || cols >= cols_)
	if (rows < 1 || cols < 1)
	{
		std::cout << "Incorrect indexes. Creating default Boolean Matrix\n";
		rows = 1;
		cols = 1;
	}

	rows_ = rows;
	cols_ = cols;

	bm_ = new BV[rows_];

	for (int row = 0; row < rows; ++row)
	{
		bm_[row] = BV(cols_, value);
	}
}


BM::BM(const BM& bm)
{
	bm_ = nullptr;
	*this = bm;
}


BM::BM(int rows, int cols, const uint8_t** matrix)
{
	if (rows <= 0 || cols <= 0)
	{
		std::cout << "Failed to initialize BM by 'uint8_t** matrix'\n";
		*this = BM();
		return;
	}

	rows_ = rows;
	cols_ = cols;
	bm_ = new BV[rows_];

	for (int i = 0; i < rows_; ++i)
	{
		bm_[i] = BV(cols, matrix[i]);
	}
}



BM::~BM()
{
	delete[] bm_;
	bm_ = nullptr;
}


int BM::getWeight() const
{
	int weight = 0;

	for (int i = 0; i < rows_; ++i)
		weight += bm_[i].getWeight();


	return weight;
}


int BM::getWeight(int row) const
{
	if (row < 0 || row >= rows_)
	{
		std::cout << "Incorrect index\n";
		row = 0;
	}

	return bm_[row].getWeight();
}


void BM::set()
{
	for (int i = 0; i < rows_; ++i)
		bm_[i].set();
}


void BM::set(int row)
{
	if (row < 0 || row >= rows_)
	{
		std::cout << "Incorrect index of row\n";
		return;
	}

	bm_[row].set();
}


void BM::set(int row, int col)
{
	if (row < 0 || row >= rows_)
	{
		std::cout << "Incorrect index of row\n";
		return;
	}

	bm_[row].set(col);
}


void BM::reset()
{
	for (int i = 0; i < rows_; ++i)
		bm_[i].reset();
}


void BM::reset(int row)
{
	if (row < 0 || row >= rows_)
	{
		std::cout << "Incorrect index of row\n";
		return;
	}

	bm_[row].reset();
}


void BM::reset(int row, int col)
{
	if (row < 0 || row >= rows_)
	{
		std::cout << "Incorrect index of row\n";
		return;
	}

	bm_[row].reset(col);
}


void BM::update(int row, int col, bool value, int  amount)
{
	if (row < 0 || row >= rows_)
	{
		std::cout << "Incorrect index of row\n";
		return;
	}

	bm_[row].update(value, col, amount);
}


void BM::inverse()
{
	for (int i = 0; i < rows_; ++i)
		bm_[i].inverse();
}


void BM::inverse(int row)
{
	if (row < 0 || row >= rows_)
	{
		std::cout << "Incorrect index of row\n";
		return;
	}

	bm_[row].inverse();
}


void BM::inverse(int row, int col, int amount)
{
	if (row < 0 || row >= rows_)
	{
		std::cout << "Incorrect index of row\n";
		return;
	}

	bm_[row].inverse(col, amount);
}


BV BM::conjunctionAll()
{
	BV result(bm_[0]);

	for (int i = 1; i < rows_; ++i)
		result &= bm_[i];

	return result;
}


BV BM::disjunctionAll()
{
	BV result(bm_[0]);

	for (int i = 1; i < rows_; ++i)
		result |= bm_[i];

	return result;
}



BV& BM::operator[] (int row)
{
	if (row < 0 || row >= rows_)
	{
		std::cout << "Incorrect index\n";
		row = 0;
	}

	return bm_[row];
}


BM& BM::operator= (const BM& bm)
{
	if (this == &bm)
		return *this;

	if (bm_)
		delete bm_;

	rows_ = bm.rows_;
	cols_ = bm.cols_;
	bm_ = new BV[rows_];

	for (int i = 0; i < rows_; ++i)
		bm_[i] = bm.bm_[i];

	return *this;
}


BM BM::operator~()
{
	BM result(*this);
	for (int i = 0; i < result.rows_; ++i)
	{
		result.bm_[i] = ~result.bm_[i];
	}

	return result;
}


BM BM::operator& (const BM& bm)
{
	BM result(*this);
	result &= bm;
	return result;
}


BM BM::operator| (const BM& bm)
{
	BM result(*this);
	result |= bm;
	return result;
}


BM BM::operator^ (const BM& bm)
{
	BM result(*this);
	result ^= bm;
	return result;
}


BM& BM::operator&= (const BM& bm)
{
	int minRows = std::min(rows_, bm.rows_);

	for (int i = 0; i < minRows; ++i)
		bm_[i] &= bm.bm_[i];


	// if rows_ < rows
	// value & 0 == 0 -> ����� �������� �������
	for (int i = rows_; i < rows_; ++i)
		bm_[i] = BV(cols_, false);

	return *this;
}


BM& BM::operator|= (const BM& bm)
{
	for (int i = 0; i < std::min(rows_, bm.rows_); ++i)
		bm_[i] |= bm.bm_[i];

	// if rows_ < bm.rows_
	// undefined | value -> ? (� ���������� ��������� ������ �������� � ������, �� ��������)
	return *this;
}


BM& BM::operator^= (const BM& bm)
{
	for (int i = 0; i < std::min(rows_, bm.rows_); ++i)
		bm_[i] ^= bm.bm_[i];

	return *this;
}



std::ostream& operator<<(std::ostream& out, const BM& bm)
{
	//for (const auto& vec : bm.bm_)
		//out << vec << '\n';

	for (int row = 0; row < bm.rows_ - 1; ++row)
		out << bm.bm_[row] << '\n';

	// I wanted to exclude last '\n'
	out << bm.bm_[bm.rows_ - 1];
	return out;
}


std::istream& operator>>(std::istream& in, BM& bm)
{
	try
	{
		std::cout << "Enter an amount of 'rows': ";
		in >> bm.rows_;
		std::cout << "Enter an amount of 'cols': ";
		in >> bm.cols_;

		bm = BM(bm.rows_, bm.cols_, false);

		for (int i = 0; i < bm.rows_; ++i)
		{
			std::cout << "Enter Boolean Vector:";
			in >> bm.bm_[i];
		}
		return in;

	}
	catch (...)
	{
		std::cout << "Incorrecti initialization. Creating default Boolean Matrix\n";
		bm = BM();
		return in;
	}

}
