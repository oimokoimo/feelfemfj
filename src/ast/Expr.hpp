/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : Expr.hpp
 *  Date     : 2026/07/07
 *
 *  Purpose  : Expression class definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Expressions are stored as source strings.
 *      - Symbolic manipulation is delegated to GiNaC.
 *
 */

#ifndef FEELFEM2_EXPR_HPP
#define FEELFEM2_EXPR_HPP

#include <string>

#include "AstNode.hpp"

namespace feelfem2
{

class Expr : public AstNode
{
public:

    Expr() = default;

    explicit Expr(const std::string& source)
        : source(source)
    {
    }

    Expr(const std::string& source, const SourceLocation& loc)
        : AstNode(loc), source(source)
    {
    }

    virtual ~Expr() = default;

    const std::string& GetSource() const
    {
        return source;
    }

    void SetSource(const std::string& s)
    {
        source = s;
    }

private:

    std::string source;

};

} // namespace feelfem2

#endif // FEELFEM2_EXPR_HPP