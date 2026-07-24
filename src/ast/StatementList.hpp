/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : StatementList.hpp
 *  Date     : 2026/07/24
 *
 *  Purpose  : List of executable statements
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a sequence of SchemeStatement objects.
 *      - Used by SchemeSection, IfStatement, loops, etc.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_STATEMENTLIST_HPP
#define FEELFEM2_STATEMENTLIST_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "AstNode.hpp"
#include "SchemeStatement.hpp"

namespace feelfem2 {

class StatementList : public AstNode {
public:

    using StatementPtr =
        std::unique_ptr<SchemeStatement>;

    using StatementContainer =
        std::vector<StatementPtr>;

    StatementList() = default;

    explicit StatementList(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    StatementList(
        StatementContainer statements,
        const SourceLocation& loc)
        : AstNode(loc),
          statements_(std::move(statements))
    {
    }

    ~StatementList() override = default;

    StatementList(const StatementList&) = delete;
    StatementList& operator=(const StatementList&) = delete;

    StatementList(StatementList&&) noexcept = default;
    StatementList& operator=(StatementList&&) noexcept = default;

    void AddStatement(SchemeStatement* statement)
    {
        statements_.emplace_back(statement);
    }

    void AddStatement(StatementPtr statement)
    {
        statements_.push_back(std::move(statement));
    }

    std::size_t GetStatementCount() const
    {
        return statements_.size();
    }

    bool IsEmpty() const
    {
        return statements_.empty();
    }

    const StatementContainer& GetStatements() const
    {
        return statements_;
    }

    StatementContainer& GetStatements()
    {
        return statements_;
    }

    void printout() const override
    {
        std::cout
            << "StatementList"
            << " count=" << statements_.size()
            << " line=" << GetLocation().line
            << "\n";

        for (const auto& statement : statements_) {
            if (statement) {
                statement->printout();
            }
        }
    }

private:

    StatementContainer statements_;

};

using StatementListPtr =
    std::unique_ptr<StatementList>;

} // namespace feelfem2

#endif
