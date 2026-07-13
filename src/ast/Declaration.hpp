/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : Declaration.hpp
 *  Date     : 2026/07/13
 *
 *  Purpose  : Base class for declaration AST nodes
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Base class of declaration nodes.
 *      - VariableDeclaration and FieldDeclaration derive from this class.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_DECLARATION_HPP
#define FEELFEM2_DECLARATION_HPP

#include "AstNode.hpp"

namespace feelfem2
{

class Declaration : public AstNode
{
public:
    Declaration() = default;

    explicit Declaration(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ~Declaration() override = default;
};

} // namespace feelfem2

#endif // FEELFEM2_DECLARATION_HPP
