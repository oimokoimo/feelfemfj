
/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : Mesh.hpp
 *  Date     : 2026/07/09
 *
 *  Purpose  : Abstract Syntax Tree (AST) mesh class definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Mesh ast.
 *      - Modern C++20 implementation.
 *
 */
#ifndef FEELFEM2_MESH_HPP
#define FEELFEM2_MESH_HPP

#include <memory>
#include <string>
#include <vector>

#include "AstNode.hpp"

namespace feelfem2
{

class MeshStatement : public AstNode
{
public:
    using AstNode::AstNode;
    virtual ~MeshStatement() = default;
};

using MeshStatementPtr = std::shared_ptr<MeshStatement>;

class PointDecl : public MeshStatement
{
public:
    PointDecl(const std::string& name,
              AstNode* coordExprList,
              const SourceLocation& loc)
        : MeshStatement(loc),
          name(name),
          coordExprList(coordExprList)
    {}

    void printout() const override
    {
        std::cout << "PointDecl " << name
                  << " at line " << GetLocation().line << "\n";
        if (coordExprList) coordExprList->printout();
    }

private:
    std::string name;
    AstNode* coordExprList = nullptr;
};

class MeshBlock : public AstNode
{
public:
    explicit MeshBlock(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    void AddStatement(MeshStatementPtr stmt)
    {
        statements.push_back(std::move(stmt));
    }

    void printout() const override
    {
        std::cout << "MeshBlock at line "
                  << GetLocation().line << "\n";

        for (const auto& stmt : statements) {
            stmt->printout();
        }
    }

private:
    std::vector<MeshStatementPtr> statements;
};

} // namespace feelfem2

#endif
