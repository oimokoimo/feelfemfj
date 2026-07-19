/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : MeshSection.hpp
 *  Date     : 2026/07/19
 *
 *  Purpose  : Mesh section AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents the complete mesh { ... } section.
 *      - Stores mesh-related statements such as point, edge,
 *        region, domain, and vertices statements.
 *      - Concrete mesh statements derive from MeshStatement.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_MESHSECTION_HPP
#define FEELFEM2_MESHSECTION_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "AstNode.hpp"
#include "Mesh.hpp"

namespace feelfem2
{

class MeshSection : public AstNode
{
public:
    MeshSection() = default;

    explicit MeshSection(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ~MeshSection() override = default;

    void AddStatement(MeshStatement* statement)
    {
        statements.emplace_back(statement);
    }

    void AddStatement(std::unique_ptr<MeshStatement> statement)
    {
        statements.push_back(std::move(statement));
    }

    std::size_t GetStatementCount() const
    {
        return statements.size();
    }

    const MeshStatement* GetStatement(std::size_t index) const
    {
        return statements.at(index).get();
    }

    MeshStatement* GetStatement(std::size_t index)
    {
        return statements.at(index).get();
    }

    const std::vector<std::unique_ptr<MeshStatement>>&
    GetStatements() const
    {
        return statements;
    }

    void printout() const override
    {
        std::cout
            << "MeshSection"
            << " count=" << statements.size()
            << " line=" << GetLocation().line
            << '\n';

        for (const auto& statement : statements)
        {
            if (statement)
            {
                statement->printout();
            }
        }
    }

private:
    std::vector<std::unique_ptr<MeshStatement>> statements;
};

} // namespace feelfem2

#endif // FEELFEM2_MESHSECTION_HPP
