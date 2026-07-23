/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : LValue.hpp
 *  Date     : 2026/07/23
 *
 *  Purpose  : Assignment target inside a scheme statement
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents both a simple identifier and an indexed target.
 *      - Examples: dt, u, u(i), a(i,j).
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_LVALUE_HPP
#define FEELFEM2_LVALUE_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "AstNode.hpp"
#include "Expression.hpp"

namespace feelfem2 {

class LValue : public AstNode {
public:
    using ArgumentPtr =
        std::unique_ptr<Expression>;

    using ArgumentList =
        std::vector<ArgumentPtr>;

    LValue() = default;

    LValue(
        std::string name,
        ArgumentList arguments,
        const SourceLocation& loc)
        : AstNode(loc),
          name_(std::move(name)),
          arguments_(std::move(arguments))
    {
    }

    explicit LValue(
        std::string name,
        const SourceLocation& loc)
        : AstNode(loc),
          name_(std::move(name))
    {
    }

    ~LValue() override = default;

    LValue(const LValue&) = delete;
    LValue& operator=(const LValue&) = delete;

    LValue(LValue&&) noexcept = default;
    LValue& operator=(LValue&&) noexcept = default;

    const std::string& GetName() const
    {
        return name_;
    }

    const ArgumentList& GetArguments() const
    {
        return arguments_;
    }

    ArgumentList& GetArguments()
    {
        return arguments_;
    }

    std::size_t GetArgumentCount() const
    {
        return arguments_.size();
    }

    bool HasArguments() const
    {
        return !arguments_.empty();
    }

    const Expression* GetArgument(std::size_t index) const
    {
        if (index >= arguments_.size()) {
            return nullptr;
        }

        return arguments_[index].get();
    }

    Expression* GetArgument(std::size_t index)
    {
        if (index >= arguments_.size()) {
            return nullptr;
        }

        return arguments_[index].get();
    }

    void printout() const override
    {
        std::cout
            << "LValue"
            << " name=" << name_
            << " arguments=" << arguments_.size()
            << " line=" << GetLocation().line
            << "\n";

        for (std::size_t i = 0;
             i < arguments_.size();
             ++i) {
            std::cout
                << "  argument[" << i << "]:\n";

            if (arguments_[i]) {
                arguments_[i]->printout();
            }
        }
    }

private:
    std::string name_;
    ArgumentList arguments_;
};

using LValuePtr = std::unique_ptr<LValue>;

} // namespace feelfem2

#endif // FEELFEM2_LVALUE_HPP
