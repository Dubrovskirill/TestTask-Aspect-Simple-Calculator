#ifndef ARITHMETICSTRATEGIES_H
#define ARITHMETICSTRATEGIES_H

#include "IArithmeticStrategy.h"
#include <stdexcept>


class AddStrategy : public IArithmeticStrategy {
public:
    double execute(double left, double right) override {
        return left + right;
    }
};


class SubStrategy : public IArithmeticStrategy {
public:
    double execute(double left, double right) override {
        return left - right;
    }
};


class MulStrategy : public IArithmeticStrategy {
public:
    double execute(double left, double right) override {
        return left * right;
    }
};


class DivStrategy : public IArithmeticStrategy {
public:
    double execute(double left, double right) override {
        if (right == 0.0) {
            throw std::runtime_error("Error");
        }
        return left / right;
    }
};


class PercentStrategy : public IArithmeticStrategy {
public:
    double execute(double left, double right) override {
        return left * (right / 100.0);
    }
};

#endif // ARITHMETICSTRATEGIES_H
