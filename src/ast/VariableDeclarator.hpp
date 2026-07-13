/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : VariableDeclarator.hpp
 *  Date     : 2026/07/13
 *
 *  Purpose  : Scalar variable declarator AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a scalar variable declarator such as
 *            a
 *            ia
 *            ib = 10
 *      - The declaration type (double, int, ...) is stored
 *        by VariableDeclaration.
 *      - Initializer is optional.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_VARIABLEDECLARATOR_HPP
#define FEELFEM2_VARIABLEDECLARATOR_HPP

#include <iostream>
#include <memory>
#include <string>

#include "AstNode.hpp"
#include "Expression.hpp"

namespace feelfem2
{

class VariableDeclarator : public AstNode
{
public:

    VariableDeclarator() = default;

    VariableDeclarator(const std::string& variableName,
                       const SourceLocation& loc)
        : AstNode(loc),
          variableName(variableName)
    {
    }

    VariableDeclarator(const std::string& variableName,
                       Expression* initializer,
                       const SourceLocation& loc)
        : AstNode(loc),
          variableName(variableName),
          initializer(initializer)
    {
    }

    virtual ~VariableDeclarator() = default;

    const std::string& GetVariableName() const
    {
        return variableName;
    }

    void SetVariableName(const std::string& name)
    {
        variableName = name;
    }

    bool HasInitializer() const
    {
        return initializer != nullptr;
    }

    const Expression* GetInitializer() const
    {
        return initializer.get();
    }

    Expression* GetInitializer()
    {
        return initializer.get();
    }

    void SetInitializer(Expression* expr)
    {
        initializer.reset(expr);
    }

    void printout() const override
    {
        std::cout
            << "VariableDeclarator "
            << variableName;

        if (initializer) {
            std::cout << " = " ;  initializer->printout(); 
	}

        std::cout
            << " line " << GetLocation().line
            << std::endl;
    }

private:

    std::string variableName;

    std::unique_ptr<Expression> initializer;

};

} // namespace feelfem2

#endif // FEELFEM2_VARIABLEDECLARATOR_HPP
