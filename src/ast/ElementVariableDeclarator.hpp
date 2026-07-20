/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : ElementVariableDeclarator.hpp
 *  Date     : 2026/07/20
 *
 *  Purpose  : Variable declarator inside an element definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents one variable in a double declaration.
 *      - An initializer expression is optional.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_ELEMENTVARIABLEDECLARATOR_HPP
#define FEELFEM2_ELEMENTVARIABLEDECLARATOR_HPP

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "AstNode.hpp"
#include "Expression.hpp"

namespace feelfem2 {

class ElementVariableDeclarator : public AstNode {
public:
    ElementVariableDeclarator() = default;

    ElementVariableDeclarator(
        std::string name,
        const SourceLocation& loc)
        : AstNode(loc),
          name_(std::move(name)),
          initializer_(nullptr)
    {
    }

    ElementVariableDeclarator(
        std::string name,
        std::unique_ptr<Expression> initializer,
        const SourceLocation& loc)
        : AstNode(loc),
          name_(std::move(name)),
          initializer_(std::move(initializer))
    {
    }

    ~ElementVariableDeclarator() override = default;

    ElementVariableDeclarator(
        const ElementVariableDeclarator&) = delete;

    ElementVariableDeclarator& operator=(
        const ElementVariableDeclarator&) = delete;

    ElementVariableDeclarator(
        ElementVariableDeclarator&&) noexcept = default;

    ElementVariableDeclarator& operator=(
        ElementVariableDeclarator&&) noexcept = default;

    const std::string& GetName() const
    {
        return name_;
    }

    bool HasInitializer() const
    {
        return initializer_ != nullptr;
    }

    const Expression* GetInitializer() const
    {
        return initializer_.get();
    }

    Expression* GetInitializer()
    {
        return initializer_.get();
    }

    void printout() const override
    {
        std::cout
            << "ElementVariableDeclarator"
            << " name=" << name_
            << " line=" << GetLocation().line
            << "\n";

        if (initializer_) {
            std::cout << "  initializer:\n";
            initializer_->printout();
        }
    }

private:
    std::string name_;
    std::unique_ptr<Expression> initializer_;
};

using ElementVariableDeclaratorPtr =
    std::unique_ptr<ElementVariableDeclarator>;

} // namespace feelfem2

#endif // FEELFEM2_ELEMENTVARIABLEDECLARATOR_HPP
