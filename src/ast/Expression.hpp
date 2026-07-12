/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : Expression.hpp
 *  Date     : 2026/07/12
 *
 *  Purpose  : Expression AST node definitions
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Defines AST nodes for numeric literals, identifiers,
 *        unary expressions, binary expressions, and function calls.
 *      - Parser semantic values may use raw pointers.
 *      - Parent AST nodes take ownership using std::unique_ptr.
 *
 */

#ifndef FEELFEM2_EXPRESSION_HPP
#define FEELFEM2_EXPRESSION_HPP

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "AstNode.hpp"

namespace feelfem2
{

// ------------------------------------------------------------
// Operators
// ------------------------------------------------------------

enum class UnaryOperator
{
    Plus,
    Minus,
    LogicalNot
};

enum class BinaryOperator
{
    Add,
    Subtract,
    Multiply,
    Divide,
    Power,

    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Equal,
    NotEqual,

    LogicalAnd,
    LogicalOr
};

// ------------------------------------------------------------
// Expression base class
// ------------------------------------------------------------

class Expression : public AstNode
{
public:
    Expression() = default;

    explicit Expression(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ~Expression() override = default;
};

using ExpressionPtr = std::unique_ptr<Expression>;

// ------------------------------------------------------------
// NumberExpr
// ------------------------------------------------------------

class NumberExpr : public Expression
{
public:
    NumberExpr(double value,
               const SourceLocation& loc)
        : Expression(loc),
          value(value)
    {
    }

    double GetValue() const
    {
        return value;
    }

    void printout() const override
    {
        std::cout << "NumberExpr"
                  << " value=" << value
                  << " line=" << GetLocation().line
                  << '\n';
    }

private:
    double value = 0.0;
};

// ------------------------------------------------------------
// IdentifierExpr
// ------------------------------------------------------------

class IdentifierExpr : public Expression
{
public:
    IdentifierExpr(std::string name,
                   const SourceLocation& loc)
        : Expression(loc),
          name(std::move(name))
    {
    }

    const std::string& GetName() const
    {
        return name;
    }

    void printout() const override
    {
        std::cout << "IdentifierExpr"
                  << " name=" << name
                  << " line=" << GetLocation().line
                  << '\n';
    }

private:
    std::string name;
};

// ------------------------------------------------------------
// UnaryExpr
// ------------------------------------------------------------

class UnaryExpr : public Expression
{
public:
    UnaryExpr(UnaryOperator op,
              Expression* operand,
              const SourceLocation& loc)
        : Expression(loc),
          op(op),
          operand(operand)
    {
    }

    UnaryOperator GetOperator() const
    {
        return op;
    }

    const Expression* GetOperand() const
    {
        return operand.get();
    }

    Expression* GetOperand()
    {
        return operand.get();
    }

    void printout() const override
    {
        std::cout << "UnaryExpr"
                  << " op=" << OperatorName(op)
                  << " line=" << GetLocation().line
                  << '\n';

        if (operand) {
            operand->printout();
        }
    }

private:
    static const char* OperatorName(UnaryOperator op)
    {
        switch (op) {
        case UnaryOperator::Plus:
            return "+";

        case UnaryOperator::Minus:
            return "-";

        case UnaryOperator::LogicalNot:
            return "not";
        }

        return "unknown";
    }

private:
    UnaryOperator op;
    ExpressionPtr operand;
};

// ------------------------------------------------------------
// BinaryExpr
// ------------------------------------------------------------

class BinaryExpr : public Expression
{
public:
    BinaryExpr(BinaryOperator op,
               Expression* left,
               Expression* right,
               const SourceLocation& loc)
        : Expression(loc),
          op(op),
          left(left),
          right(right)
    {
    }

    BinaryOperator GetOperator() const
    {
        return op;
    }

    const Expression* GetLeft() const
    {
        return left.get();
    }

    Expression* GetLeft()
    {
        return left.get();
    }

    const Expression* GetRight() const
    {
        return right.get();
    }

    Expression* GetRight()
    {
        return right.get();
    }

    void printout() const override
    {
        std::cout << "BinaryExpr"
                  << " op=" << OperatorName(op)
                  << " line=" << GetLocation().line
                  << '\n';

        if (left) {
            left->printout();
        }

        if (right) {
            right->printout();
        }
    }

private:
    static const char* OperatorName(BinaryOperator op)
    {
        switch (op) {
        case BinaryOperator::Add:
            return "+";

        case BinaryOperator::Subtract:
            return "-";

        case BinaryOperator::Multiply:
            return "*";

        case BinaryOperator::Divide:
            return "/";

        case BinaryOperator::Power:
            return "^";

        case BinaryOperator::Less:
            return "<";

        case BinaryOperator::LessEqual:
            return "<=";

        case BinaryOperator::Greater:
            return ">";

        case BinaryOperator::GreaterEqual:
            return ">=";

        case BinaryOperator::Equal:
            return "==";

        case BinaryOperator::NotEqual:
            return "!=";

        case BinaryOperator::LogicalAnd:
            return "and";

        case BinaryOperator::LogicalOr:
            return "or";
        }

        return "unknown";
    }

private:
    BinaryOperator op;
    ExpressionPtr left;
    ExpressionPtr right;
};

// ------------------------------------------------------------
// FunctionCallExpr
// ------------------------------------------------------------

class FunctionCallExpr : public Expression
{
public:
    FunctionCallExpr(std::string functionName,
                     const SourceLocation& loc)
        : Expression(loc),
          functionName(std::move(functionName))
    {
    }

    const std::string& GetFunctionName() const
    {
        return functionName;
    }

    void AddArgument(Expression* argument)
    {
        arguments.emplace_back(argument);
    }

    std::size_t GetArgumentCount() const
    {
        return arguments.size();
    }

    const Expression* GetArgument(std::size_t index) const
    {
        return arguments.at(index).get();
    }

    Expression* GetArgument(std::size_t index)
    {
        return arguments.at(index).get();
    }

    void printout() const override
    {
        std::cout << "FunctionCallExpr"
                  << " name=" << functionName
                  << " arguments=" << arguments.size()
                  << " line=" << GetLocation().line
                  << '\n';

        for (const auto& argument : arguments) {
            if (argument) {
                argument->printout();
            }
        }
    }

private:
    std::string functionName;
    std::vector<ExpressionPtr> arguments;
};

} // namespace feelfem2

#endif // FEELFEM2_EXPRESSION_HPP
