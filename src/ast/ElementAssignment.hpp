/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : ElementAssignment.hpp
 *  Date     : 2026/07/20
 *
 *  Purpose  : Assignment inside an element definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents an assignment to a local element variable.
 *      - The right-hand side is represented by an Expression AST.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_ELEMENTASSIGNMENT_HPP
#define FEELFEM2_ELEMENTASSIGNMENT_HPP

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "ElementItem.hpp"
#include "Expression.hpp"

namespace feelfem2 {

class ElementAssignment : public ElementItem {
public:
    ElementAssignment() = default;

    ElementAssignment(
        std::string target,
        std::unique_ptr<Expression> value,
        const SourceLocation& loc)
        : ElementItem(loc),
          target_(std::move(target)),
          value_(std::move(value))
    {
    }

    ~ElementAssignment() override = default;

    ElementAssignment(
        const ElementAssignment&) = delete;

    ElementAssignment& operator=(
        const ElementAssignment&) = delete;

    ElementAssignment(
        ElementAssignment&&) noexcept = default;

    ElementAssignment& operator=(
        ElementAssignment&&) noexcept = default;

    const std::string& GetTarget() const
    {
        return target_;
    }

    const Expression* GetValue() const
    {
        return value_.get();
    }

    Expression* GetValue()
    {
        return value_.get();
    }

    void printout() const override
    {
        std::cout
            << "ElementAssignment"
            << " target=" << target_
            << " line=" << GetLocation().line
            << "\n";

        if (value_) {
            std::cout << "  value:\n";
            value_->printout();
        }
    }

private:
    std::string target_;
    std::unique_ptr<Expression> value_;
};

using ElementAssignmentPtr =
    std::unique_ptr<ElementAssignment>;

} // namespace feelfem2

#endif // FEELFEM2_ELEMENTASSIGNMENT_HPP
