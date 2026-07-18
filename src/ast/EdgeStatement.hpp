/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : EdgeStatement.hpp
 *  Date     : 2026/07/18
 *
 *  Purpose  : Edge statement AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a complete edge statement such as
 *
 *            edge e1(a,b,c,d,a), e2(p,q,r);
 *
 *      - Each edge is represented by EdgeDecl.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_EDGESTATEMENT_HPP
#define FEELFEM2_EDGESTATEMENT_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "Mesh.hpp"
#include "EdgeDecl.hpp"

namespace feelfem2
{

class EdgeStatement : public MeshStatement
{
public:
    EdgeStatement() = default;

    explicit EdgeStatement(const SourceLocation& loc)
        : MeshStatement(loc)
    {
    }

    ~EdgeStatement() override = default;

    void AddEdge(EdgeDecl* edge)
    {
        edges.emplace_back(edge);
    }

    void AddEdge(std::unique_ptr<EdgeDecl> edge)
    {
        edges.push_back(std::move(edge));
    }

    std::size_t GetEdgeCount() const
    {
        return edges.size();
    }

    const EdgeDecl* GetEdge(std::size_t index) const
    {
        return edges.at(index).get();
    }

    EdgeDecl* GetEdge(std::size_t index)
    {
        return edges.at(index).get();
    }

    const std::vector<std::unique_ptr<EdgeDecl>>&
    GetEdges() const
    {
        return edges;
    }

    void printout() const override
    {
        std::cout
            << "EdgeStatement"
            << " count=" << edges.size()
            << " line=" << GetLocation().line
            << '\n';

        for (const auto& edge : edges)
        {
            if (edge)
            {
                edge->printout();
            }
        }
    }

private:
    std::vector<std::unique_ptr<EdgeDecl>> edges;
};

} // namespace feelfem2

#endif // FEELFEM2_EDGESTATEMENT_HPP
