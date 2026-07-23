/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : SchemeStatement.hpp
 *  Date     : 2026/07/23
 *
 *  Purpose  : Base class for statements inside a scheme section
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Base class of executable statements in a scheme block.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_SCHEMESTATEMENT_HPP
#define FEELFEM2_SCHEMESTATEMENT_HPP

#include "AstNode.hpp"

namespace feelfem2 {

class SchemeStatement : public AstNode {
public:
    SchemeStatement() = default;

    explicit SchemeStatement(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ~SchemeStatement() override = default;

    SchemeStatement(const SchemeStatement&) = delete;
    SchemeStatement& operator=(const SchemeStatement&) = delete;

    SchemeStatement(SchemeStatement&&) noexcept = default;
    SchemeStatement& operator=(SchemeStatement&&) noexcept = default;

    void printout() const override = 0;
};

} // namespace feelfem2

#endif // FEELFEM2_SCHEMESTATEMENT_HPP
