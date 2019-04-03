#include <stdexcept>
#include <cstdio>
#include <iostream>

class Matrix
{
    size_t cols;
    size_t rows;
    int* Data;

    class Row
    {
        size_t cols;
        int* data;
    public:
        Row(size_t cols_num, int* row_data)
            : cols(cols_num), data(row_data)
            {}
        int& operator[](size_t index1)
        {
            if ((index1 < cols) && (index1 >= 0))
                return data[index1];
            throw std::out_of_range("");
        }

        const int& operator[](size_t index1) const
        {
            if(index1 < cols)
                return data[index1];
            throw std::out_of_range("");
        }

    };

public:
    Matrix(size_t rows_num, size_t cols_num)
        : rows(rows_num), cols(cols_num)
        {
            Data = new int[rows * cols];
        }

    size_t getRows()
    {
        return rows;
    }

    size_t getColumns()
    {
        return cols;
    }

    Row operator [](size_t index2)
    {
        if ((index2 >= 0) && (index2 < rows))
            return Row(cols, Data + (index2) * cols);
        throw std::out_of_range("");
    }

    const Row operator [](size_t index2) const
    {
        if ((index2 >= 0) && (index2 < rows))
            return Row(cols, Data + (index2) * cols);
        throw std::out_of_range("");
    }

    Matrix& operator *=(int coeff) {
        for(size_t i = 0; i < cols * rows; i++)
            Data[i] *= coeff;
        return *this;
    }

    bool operator ==(const Matrix& Compared) const
    {
        if((rows != Compared.rows) || (cols != Compared.cols))
            return false;
        for (int i = 0; i < rows * cols; i++)
            if (Data[i] != Compared.Data[i])
                return false;
        return true;
    }

    bool operator !=(const Matrix& Compared) const
    {
        return !(*this == Compared);
    }

};
