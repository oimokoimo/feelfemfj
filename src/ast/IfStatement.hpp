/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : IfStatement.hpp
 *  Date     : 2026/07/24
 *
 *  Purpose  : If statement AST node
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents IF-THEN or IF-THEN-ELSE statement.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_IFSTATEMENT_HPP
#define FEELFEM2_IFSTATEMENT_HPP

#include <iostream>
#include <memory>

#include "Expression.hpp"
#include "SchemeStatement.hpp"
#include "StatementList.hpp"

namespace feelfem2 {

class IfStatement : public SchemeStatement {
public:

    IfStatement() = default;

    IfStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<StatementList> thenPart,
        std::unique_ptr<StatementList> elsePart,
        const SourceLocation& loc)
        : SchemeStatement(loc),
          condition_(std::move(condition)),
          thenPart_(std::move(thenPart)),
          elsePart_(std::move(elsePart))
    {
    }

    ~IfStatement() override = default;

    IfStatement(const IfStatement&) = delete;
    IfStatement& operator=(const IfStatement&) = delete;

    IfStatement(IfStatement&&) noexcept = default;
    IfStatement& operator=(IfStatement&&) noexcept = default;

    const Expression* GetCondition() const
    {
        return condition_.get();
    }

    const StatementList* GetThenPart() const
    {
        return thenPart_.get();
    }

    const StatementList* GetElsePart() const
    {
        return elsePart_.get();
    }

    bool HasElsePart() const
    {
        return elsePart_ != nullptr;
    }

    void printout() const override
    {
        std::cout
            << "IfStatement"
            << " line=" << GetLocation().line
            << "\n";

        std::cout << "  condition:\n";

        if (condition_) {
            condition_->printout();
        }

        std::cout << "  then:\n";

        if (thenPart_) {
            thenPart_->printout();
        }

        if (elsePart_) {
            std::cout << "  else:\n";
            elsePart_->printout();
        }
    }

private:

    std::unique_ptr<Expression> condition_;

    std::unique_ptr<StatementList> thenPart_;

    std::unique_ptr<StatementList> elsePart_;

};

using IfStatementPtr =
    std::unique_ptr<IfStatement>;

} // namespace feelfem2

#endif
