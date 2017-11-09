#include "matrix.h"
#include <vector>

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

Matrix::Matrix(std::initializer_list<std::initializer_list<double> > values) : N(values.size()), M(values.begin()->size()) {
    double **tmp = new double*[N];
    std::vector<std::vector<double>> tmpV;

    int i=0;
    for(std::initializer_list<double> x : values) {
        tmp[i++] = new double[M];
        tmpV.push_back(x);
    }

    for(int i=0; i<N; ++i)
        for(int j=0; j<M; ++j)
            tmp[i][j] = tmpV[i][j];

    this->matrix = tmp;
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

Matrix::Row Matrix::operator[](int row) {
    return Row(*this, row);
}

double &Matrix::Row::operator [](int col) {
    return parent.matrix[row][col];
}

Matrix::Row::Row(Matrix &parent, int row) : parent(parent), row(row) { }
