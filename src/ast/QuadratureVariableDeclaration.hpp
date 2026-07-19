/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : QuadratureVariableDeclaration.hpp
 *  Date     : 2026/07/19
 *
 *  Purpose  : Quadrature-local variable declaration AST node
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 */

#ifndef FEELFEM2_QUADRATUREVARIABLEDECLARATION_HPP
#define FEELFEM2_QUADRATUREVARIABLEDECLARATION_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "QuadratureItem.hpp"
#include "QuadratureVariableDeclarator.hpp"

namespace feelfem2
{

class QuadratureVariableDeclaration : public QuadratureItem
{
public:
    using QuadratureItem::QuadratureItem;

    void AddDeclarator(QuadratureVariableDeclarator* declarator)
    {
        declarators.emplace_back(declarator);
    }

    std::size_t GetDeclaratorCount() const
    {
        return declarators.size();
    }

    void printout() const override
    {
        std::cout
            << "QuadratureVariableDeclaration"
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
    std::vector<std::unique_ptr<QuadratureVariableDeclarator>> declarators;
};

} // namespace feelfem2

#endif
