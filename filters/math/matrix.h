#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>

class Matrix {
public:
    Matrix();
    Matrix(int N);
    Matrix(int N, int M);
    Matrix(std::initializer_list<std::initializer_list <double>> values);
    Matrix(const Matrix& matrix);
    void deleteMatrix();

    int getNDimension();
    int getMDimension();
    static double getNorm(Matrix m);
    static Matrix getLaplaceKernel();
    static Matrix getHighPassKernel();

    class Row {
        friend class Matrix;
    public:
        double &operator [](int col);

        Row(Matrix &parent, int row);

    private:
        Matrix &parent;
        int row;
    };

    Row operator[](int row);

private:
    int N, M;
    double **matrix = nullptr;

    static const Matrix laplaceKernel;
    static const Matrix highPassKernel;
};

#endif // MATRIX_H
