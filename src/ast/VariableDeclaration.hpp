/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : VariableDeclaration.hpp
 *  Date     : 2026/07/13
 *
 *  Purpose  : Scalar variable declaration AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a complete scalar variable declaration such as
 *
 *            double a, b, c;
 *            int ia, ib = 10;
 *
 *      - Each declared variable is represented by VariableDeclarator.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_VARIABLEDECLARATION_HPP
#define FEELFEM2_VARIABLEDECLARATION_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "Declaration.hpp"
#include "VariableDeclarator.hpp"

namespace feelfem2
{

enum class ScalarType
{
    Double,
    Int
};

class VariableDeclaration : public Declaration
{
public:
    VariableDeclaration() = default;

    explicit VariableDeclaration(
        ScalarType type,
        const SourceLocation& loc = SourceLocation{})
        : Declaration(loc),
          type(type)
    {
    }

    ~VariableDeclaration() override = default;

    ScalarType GetType() const
    {
        return type;
    }

    void SetType(ScalarType newType)
    {
        type = newType;
    }

    void AddDeclarator(VariableDeclarator* declarator)
    {
        declarators.emplace_back(declarator);
    }

    void AddDeclarator(std::unique_ptr<VariableDeclarator> declarator)
    {
        declarators.push_back(std::move(declarator));
    }

    std::size_t GetDeclaratorCount() const
    {
        return declarators.size();
    }

    const VariableDeclarator* GetDeclarator(std::size_t index) const
    {
        return declarators.at(index).get();
    }

    VariableDeclarator* GetDeclarator(std::size_t index)
    {
        return declarators.at(index).get();
    }

    const std::vector<std::unique_ptr<VariableDeclarator>>&
    GetDeclarators() const
    {
        return declarators;
    }

    void printout() const override
    {
        std::cout
            << "VariableDeclaration"
            << " type=" << TypeName(type)
            << " count=" << declarators.size()
            << " line=" << GetLocation().line
            << '\n';

        for (const auto& declarator : declarators)
        {
            if (declarator)
            {
                declarator->printout();
            }
        }
    }

private:
    static const char* TypeName(ScalarType scalarType)
    {
        switch (scalarType)
        {
        case ScalarType::Double:
            return "double";

        case ScalarType::Int:
            return "int";
        }

        return "unknown";
    }

private:
    ScalarType type = ScalarType::Double;

    std::vector<std::unique_ptr<VariableDeclarator>> declarators;
};

} // namespace feelfem2

#endif // FEELFEM2_VARIABLEDECLARATION_HPP
