#include <iostream>
#include <stdexcept>
#include <memory>

template<typename T>
class Matrix {
public:
    Matrix(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), data_(std::make_unique<T[]>(rows * cols)) {}

    // Destructor (not needed explicitly, but here for clarity)
    ~Matrix() = default;

    // Move constructor
    Matrix(Matrix&& other) noexcept
        : rows_(other.rows_), cols_(other.cols_), data_(std::move(other.data_)) {
        other.rows_ = 0;
        other.cols_ = 0;
    }

    // Move assignment operator
    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = std::move(other.data_);
            other.rows_ = 0;
            other.cols_ = 0;
        }
        return *this;
    }

    // Disable copy constructor and copy assignment
    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    T& operator()(size_t row, size_t col) {
        return data_[row * cols_ + col];
    }

    const T& operator()(size_t row, size_t col) const {
        return data_[row * cols_ + col];
    }

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }

    // Matrix addition
    Matrix operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions must agree for addition.");
        }

        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] + other.data_[i];
        }
        return result;
    }

    // Matrix subtraction
    Matrix operator-(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions must agree for subtraction.");
        }

        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] - other.data_[i];
        }
        return result;
    }

    // Matrix multiplication
    Matrix operator*(const Matrix& other) const {
        if (cols_ != other.rows_) {
            throw std::invalid_argument("Matrix dimensions must agree for multiplication.");
        }

        Matrix result(rows_, other.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < other.cols_; ++j) {
                result(i, j) = 0;
                for (size_t k = 0; k < cols_; ++k) {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }
        return result;
    }

private:
    size_t rows_;
    size_t cols_;
    std::unique_ptr<T[]> data_;
};

// Function to transpose a matrix
template<typename T>
Matrix<T> transpose(const Matrix<T>& matrix) {
    Matrix<T> result(matrix.cols(), matrix.rows());
    for (size_t i = 0; i < matrix.rows(); ++i) {
        for (size_t j = 0; j < matrix.cols(); ++j) {
            result(j, i) = matrix(i, j);
        }
    }
    return result;
}

// Function to compute the trace of a square matrix
template<typename T>
T trace(const Matrix<T>& matrix) {
    if (matrix.rows() != matrix.cols()) {
        throw std::invalid_argument("Matrix must be square to compute trace.");
    }
    T sum = 0;
    for (size_t i = 0; i < matrix.rows(); ++i) {
        sum += matrix(i, i);
    }
    return sum;
}

// Example usage
int main() {
    Matrix<int> mat1(2, 2);
    mat1(0, 0) = 1; mat1(0, 1) = 2;
    mat1(1, 0) = 3; mat1(1, 1) = 4;

    Matrix<int> mat2(2, 2);
    mat2(0, 0) = 5; mat2(0, 1) = 6;
    mat2(1, 0) = 7; mat2(1, 1) = 8;

    Matrix<int> sum = mat1 + mat2;
    Matrix<int> product = mat1 * mat2;
    Matrix<int> transposed = transpose(mat1);

    int tr = trace(mat1);

    std::cout << "Sum of matrices:\n";
    for (size_t i = 0; i < sum.rows(); ++i) {
        for (size_t j = 0; j < sum.cols(); ++j) {
            std::cout << sum(i, j) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Product of matrices:\n";
    for (size_t i = 0; i < product.rows(); ++i) {
        for (size_t j = 0; j < product.cols(); ++j) {
            std::cout << product(i, j) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Transposed matrix:\n";
    for (size_t i = 0; i < transposed.rows(); ++i) {
        for (size_t j = 0; j < transposed.cols(); ++j) {
            std::cout << transposed(i, j) << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Trace of matrix: " << tr << "\n";

    return 0;
}
