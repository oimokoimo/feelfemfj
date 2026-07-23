/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : AssignmentStatement.hpp
 *  Date     : 2026/07/23
 *
 *  Purpose  : Assignment statement inside a scheme section
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Stores an assignment target and a right-hand-side expression.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_ASSIGNMENTSTATEMENT_HPP
#define FEELFEM2_ASSIGNMENTSTATEMENT_HPP

#include <iostream>
#include <memory>
#include <utility>

#include "Expression.hpp"
#include "LValue.hpp"
#include "SchemeStatement.hpp"

namespace feelfem2 {

class AssignmentStatement : public SchemeStatement {
public:
    AssignmentStatement() = default;

    AssignmentStatement(
        std::unique_ptr<LValue> target,
        std::unique_ptr<Expression> value,
        const SourceLocation& loc)
        : SchemeStatement(loc),
          target_(std::move(target)),
          value_(std::move(value))
    {
    }

    ~AssignmentStatement() override = default;

    AssignmentStatement(
        const AssignmentStatement&) = delete;

    AssignmentStatement& operator=(
        const AssignmentStatement&) = delete;

    AssignmentStatement(
        AssignmentStatement&&) noexcept = default;

    AssignmentStatement& operator=(
        AssignmentStatement&&) noexcept = default;

    const LValue* GetTarget() const
    {
        return target_.get();
    }

    LValue* GetTarget()
    {
        return target_.get();
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
            << "AssignmentStatement"
            << " line=" << GetLocation().line
            << "\n";

        if (target_) {
            std::cout << "  target:\n";
            target_->printout();
        }

        if (value_) {
            std::cout << "  value:\n";
            value_->printout();
        }
    }

private:
    std::unique_ptr<LValue> target_;
    std::unique_ptr<Expression> value_;
};

using AssignmentStatementPtr =
    std::unique_ptr<AssignmentStatement>;

} // namespace feelfem2

#endif // FEELFEM2_ASSIGNMENTSTATEMENT_HPP
