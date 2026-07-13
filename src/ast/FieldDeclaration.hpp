/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : FieldDeclaration.hpp
 *  Date     : 2026/07/13
 *
 *  Purpose  : Field variable declaration AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a complete field declaration such as
 *
 *            fem   u[P1], v[P1];
 *            vfem  w[P2];
 *            ewise q[gaux];
 *
 *      - Each declared field is represented by FieldDeclarator.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_FIELDDECLARATION_HPP
#define FEELFEM2_FIELDDECLARATION_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "Declaration.hpp"
#include "FieldDeclarator.hpp"

namespace feelfem2
{

enum class FieldType
{
    Fem,
    Vfem,
    Ewise
};

class FieldDeclaration : public Declaration
{
public:
    FieldDeclaration() = default;

    explicit FieldDeclaration(
        FieldType type,
        const SourceLocation& loc = SourceLocation{})
        : Declaration(loc),
          type(type)
    {
    }

    ~FieldDeclaration() override = default;

    FieldType GetType() const
    {
        return type;
    }

    void SetType(FieldType newType)
    {
        type = newType;
    }

    void AddDeclarator(FieldDeclarator* declarator)
    {
        declarators.emplace_back(declarator);
    }

    void AddDeclarator(std::unique_ptr<FieldDeclarator> declarator)
    {
        declarators.push_back(std::move(declarator));
    }

    std::size_t GetDeclaratorCount() const
    {
        return declarators.size();
    }

    const FieldDeclarator* GetDeclarator(std::size_t index) const
    {
        return declarators.at(index).get();
    }

    FieldDeclarator* GetDeclarator(std::size_t index)
    {
        return declarators.at(index).get();
    }

    const std::vector<std::unique_ptr<FieldDeclarator>>&
    GetDeclarators() const
    {
        return declarators;
    }

    void printout() const override
    {
        std::cout
            << "FieldDeclaration"
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
    static const char* TypeName(FieldType fieldType)
    {
        switch (fieldType)
        {
        case FieldType::Fem:
            return "fem";

        case FieldType::Vfem:
            return "vfem";

        case FieldType::Ewise:
            return "ewise";
        }

        return "unknown";
    }

private:
    FieldType type = FieldType::Fem;

    std::vector<std::unique_ptr<FieldDeclarator>> declarators;
};

} // namespace feelfem2

#endif // FEELFEM2_FIELDDECLARATION_HPP
