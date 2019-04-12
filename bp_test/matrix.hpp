#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <spdlog/spdlog.h>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <vector>

class Matrix {
  std::string name;
  std::vector<std::vector<float> > values;
  friend std::ostream& operator<<(std::ostream&, const Matrix&);

public:
  const int nR, nC;

  Matrix(int r, int c, std::string name = "<unnamed-matrix>"): nR(r), nC(c), name(name) {
    spdlog::debug("Matrix {}: constructor called", name.c_str());
    values.resize(nR);
    for (auto& row: values) {
      row.resize(nC);
    }
  }

  Matrix(const Matrix& m) : nR(m.nR), nC(m.nC), name("(" + m.name + ")_copy") {
    spdlog::warn("Matrix {}: copy constructor called", name.c_str());
    values.resize(nR);
    for (auto& row: values) {
      row.resize(nC);
    }
    // deep copy the values variable
    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        values[i][j] = m.values[i][j];
      }
    }
  }

  ~Matrix() {
    spdlog::debug("Matrix {}: destructor called", name.c_str());
  }

  int getNumElements() { return nR * nC; }

  float& at(const int& i, const int& j) {
    return values[i][j];
  }

  void set(const int& i, const int& j, const float& val){
    values[i][j] = val;
  }

  Matrix* setName(const std::string& name) {
    this->name = name;
    return this;
  }

  Matrix* setZeros() {
    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        values[i][j] = 0;
      }
    }
    return this;
  }

  Matrix* setOnes() {
    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        values[i][j] = 1;
      }
    }
    return this;
  }

  Matrix* setIdentity() {
    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        values[i][j] = (i == j);
      }
    }
    return this;
  }

  Matrix* setUniform(float low, float high) {
    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        float randomNumber = (float) rand() / RAND_MAX;
        randomNumber = low + randomNumber * (high - low);
        values[i][j] = randomNumber;
      }
    }
    return this;
  }

  Matrix sigmoid()  {
    std::stringstream ss;
    ss << "(" << name << ")_SigmoidActivation";
    Matrix result(nR, nC, ss.str());

    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        result.values[i][j] = 1 / (1 + exp(-this->values[i][j]));
      }
    }

    return result;
  }
  Matrix delsigmoid()  {
    std::stringstream ss;
    ss << "(" << name << ")_SigmoidDerivative";
    Matrix result(nR, nC, ss.str());

    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        result.values[i][j] = 1 / (1 + exp(-this->values[i][j]));
        result.values[i][j] = result.values[i][j] - (result.values[i][j]*result.values[i][j]);
      }
    }

    return result;
  }

  Matrix compute_softmax(){
    std::stringstream ss;
    ss << "(" << name << ")_SoftMax";
    Matrix result(nR, nC, ss.str());
    
    if(nC == 1){
      float sum = 0;
      for (int i = 0; i < nR; ++i) {
        sum += exp(this->values[i][0]);
      }
      for (int i = 0; i < nR; ++i) {
        result.values[i][0] = exp(this->values[i][0])/sum;
      }
    }
    else{
      std::stringstream ss;
      ss <<  "Invalid operation: Attempt to compute softmax of a 2D matrix";
      throw ss.str();
    }
    
    return result;
  }

  void operator=(Matrix const &m) {
    /* = operator is completely disabled to ensure simplicity */
    if(m.nC == nC && m.nR == nR){
      name = "(" + m.name + ")_copy";
      for (int i = 0; i < nR; ++i) {
        for (int j = 0; j < nC; ++j) {
          values[i][j] = m.values[i][j];
        }
      }
    }
    else{
      std::stringstream ss;
      ss <<  "Invalid operation: Attempt to assign a matrix with different dimension" << m.name.c_str() << " to another matrix";
      throw ss.str();
    }
  }

  Matrix operator~() {
    std::stringstream ss;
    ss << "(" << name << ")_Transpose";
    Matrix result(nC, nR, ss.str());

    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        result.values[j][i] = this->values[i][j];
      }
    }

    return result;
  }

  Matrix operator+(Matrix const &m) {
    if (nR != m.nR || nC != m.nC) {
      std::stringstream ss;
      ss <<  "Invalid dimensions for matrix addition: Candidates are matrices "
        << name << "(" << nR << "," << nC << ")"
        << " and "
        << m.name << "(" << m.nR << "," << m.nC << ")";
      throw ss.str();
    }

    std::stringstream ss;
    ss << name << " + " << m.name;
    Matrix result(nR, nC, ss.str());

    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        result.values[i][j] = this->values[i][j] + m.values[i][j];
      }
    }

    return result;
  }

  Matrix operator-(Matrix const &m) {
    if (nR != m.nR || nC != m.nC) {
      std::stringstream ss;
      ss <<  "Invalid dimensions for matrix subtraction: Candidates are matrices "
        << name << "(" << nR << "," << nC << ")"
        << " and "
        << m.name << "(" << m.nR << "," << m.nC << ")";
      throw ss.str();
    }

    std::stringstream ss;
    ss << name << " - " << m.name;
    Matrix result(nR, nC, ss.str());

    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        result.values[i][j] = this->values[i][j] - m.values[i][j];
      }
    }

    return result;
  }

  Matrix operator*(Matrix const &m) {
    if (nC != m.nR) {
      std::stringstream ss;
      ss <<  "Invalid dimensions for matrix multiplication: Candidates are matrices "
        << name << "(" << nR << "," << nC << ")"
        << " and "
        << m.name << "(" << m.nR << "," << m.nC << ")";
      throw ss.str();
    }

    std::stringstream ss;
    ss << name << " * " << m.name;
    Matrix result(nR, m.nC, ss.str());

    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < m.nC; ++j) {
        float elementSum = 0;
        for (int k = 0; k < nC; ++k) {
          elementSum += this->values[i][k] * m.values[k][j];
        }
        result.values[i][j] = elementSum;
      }
    }

    return result;
  }

  Matrix operator*(float const &mul) {
    std::stringstream ss;
    ss << name << " * " << " const("<<mul<<") ";
    Matrix result(nR, nC, ss.str());

    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        result.values[i][j] = this->values[i][j]*mul;
      }
    }

    return result;
  }


  Matrix operator%(Matrix const &m) {
    if (nC != m.nC || nR != m.nR) {
      std::stringstream ss;
      ss <<  "Invalid dimensions for matrix element wise multiplication: Candidates are matrices "
        << name << "(" << nR << "," << nC << ")"
        << " and "
        << m.name << "(" << m.nR << "," << m.nC << ")";
      throw ss.str();
    }

    std::stringstream ss;
    ss << name << " % " << m.name;
    Matrix result(nR, nC, ss.str());

    for (int i = 0; i < nR; ++i) {
      for (int j = 0; j < nC; ++j) {
        result.values[i][j] = this->values[i][j] * m.values[i][j];
      }
    }

    return result;
  }

};
std::ostream& operator<<(std::ostream &out, const Matrix &m) {
  out << m.name << " of shape: (" << m.nR << "," << m.nC << ") is\n";
  for (int i = 0; i < m.nR; ++i) {
    for (int j = 0; j < m.nC; ++j) {
      out << m.values[i][j] << " ";
    }
    out << std::endl;
  }
  return out;
}


#endif
