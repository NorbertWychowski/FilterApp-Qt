#include "matrix.h"
#include <vector>

const Matrix Matrix::laplaceKernel  = {
    { -4, -1, 0, -1, -4 },
    { -1,  2, 3,  2, -1 },
    {  0,  3, 4,  3,  0 },
    { -1,  2, 3,  2, -1 },
    { -4, -1, 0, -1, -4 }
};

const Matrix Matrix::highPassKernel = {
    {  0, -1, -1, -1,  0 },
    { -1,  2, -4,  2, -1 },
    { -1, -4, 13, -4, -1 },
    { -1,  2, -4,  2, -1 },
    {  0, -1, -1, -1,  0 }
};

Matrix::Matrix() : N(0), M(0) {
}

Matrix::Matrix(int N) : N(N), M(N) {
    matrix = new double*[N];
    for(int i=0; i<N; i++)
        matrix[i] = new double[N];
}

Matrix::Matrix(int N, int M) : N(N), M(M) {
    matrix = new double*[N];
    for(int i=0; i<N; i++)
        matrix[i] = new double[M];
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double> > values) :
    N(int(values.size())), M(int(values.begin()->size())) {

    matrix = new double*[N];

    int i = 0;
    int j = 0;
    for(std::initializer_list<double> x : values) {
        matrix[i] = new double[M];
        for(double val : x)
            matrix[i][j++] = val;

        i++;
        j = 0;
    }
}

Matrix::Matrix(const Matrix& matrix) {
    this->N = matrix.N;
    this->M = matrix.M;
    this->matrix = matrix.matrix;
}

void Matrix::deleteMatrix() {
    for(int i=0; i<N; i++)
        delete[] matrix[i];
    delete[] matrix;
}

int Matrix::getNDimension() {
    return N;
}

int Matrix::getMDimension() {
    return M;
}

double Matrix::getNorm(Matrix m) {
    int N = m.getNDimension();
    int M = m.getMDimension();
    double norm = 0;
    for(int i=0; i<N; ++i)
        for(int j=0; j<M; ++j)
            norm += m[i][j];
    return norm;
}

Matrix Matrix::getLaplaceKernel() {
    return laplaceKernel;
}

Matrix Matrix::getHighPassKernel() {
    return highPassKernel;
}

Matrix::Row Matrix::operator[](int row) {
    return Row(*this, row);
}

double &Matrix::Row::operator [](int col) {
    return parent.matrix[row][col];
}

Matrix::Row::Row(Matrix &parent, int row) : parent(parent), row(row) { }
