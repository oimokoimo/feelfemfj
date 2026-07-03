/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : AstNode.hpp
 *  Date     : 2026/07/03
 *
 *  Purpose  : Abstract Syntax Tree (AST) base class definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Base class of all AST nodes.
 *      - Stores source location information.
 *      - Modern C++20 implementation.
 *
 */
#ifndef FEELFEM2_ASTNODE_HPP
#define FEELFEM2_ASTNODE_HPP
#include <memory>

namespace feelfem2 {

struct SourceLocation {
    int line   = 0;
    int column = 0;
};

class AstNode {
public:
    AstNode() = default;
    explicit AstNode(const SourceLocation& loc)
        : location(loc) {}

    virtual ~AstNode() = default;

    const SourceLocation& GetLocation() const {
        return location;
    }

    void SetLocation(const SourceLocation& loc) {
        location = loc;
    }

protected:
    SourceLocation location;
};

using AstNodePtr = std::shared_ptr<AstNode>;

} // namespace feelfem2

#endif