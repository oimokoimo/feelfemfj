/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : QuadratureVariableDeclarator.hpp
 *  Date     : 2026/07/19
 *
 *  Purpose  : Quadrature-local variable declarator AST node
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 */

#ifndef FEELFEM2_QUADRATUREVARIABLEDECLARATOR_HPP
#define FEELFEM2_QUADRATUREVARIABLEDECLARATOR_HPP

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "AstNode.hpp"
#include "Expression.hpp"

namespace feelfem2
{

class QuadratureVariableDeclarator : public AstNode
{
public:
    QuadratureVariableDeclarator() = default;

    QuadratureVariableDeclarator(
        std::string name,
        Expression* initializer,
        const SourceLocation& loc)
        : AstNode(loc),
          name(std::move(name)),
          initializer(initializer)
    {
    }

    ~QuadratureVariableDeclarator() override = default;

    const std::string& GetName() const
    {
        return name;
    }

    bool HasInitializer() const
    {
        return initializer != nullptr;
    }

    const Expression* GetInitializer() const
    {
        return initializer.get();
    }

    void printout() const override
    {
        std::cout
            << "QuadratureVariableDeclarator"
            << " name=" << name
            << " line=" << GetLocation().line
            << '\n';

        if (initializer)
        {
            std::cout << "  initializer:\n";
            initializer->printout();
        }
    }

private:
    std::string name;
    std::unique_ptr<Expression> initializer;
};

} // namespace feelfem2

#endif
