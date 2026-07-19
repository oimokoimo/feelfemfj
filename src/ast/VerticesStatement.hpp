/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : VerticesStatement.hpp
 *  Date     : 2026/07/18
 *
 *  Purpose  : Vertices statement AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a vertices statement such as
 *
 *            vertices (1000);
 *            vertices (nnode);
 *
 *      - Stores the requested number of mesh vertices as an expression.
 *      - The expression is validated and evaluated by Semantic Analyzer.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_VERTICESSTATEMENT_HPP
#define FEELFEM2_VERTICESSTATEMENT_HPP

#include <iostream>
#include <memory>

#include "Mesh.hpp"
#include "Expression.hpp"

namespace feelfem2
{

class VerticesStatement : public MeshStatement
{
public:
    VerticesStatement() = default;

    VerticesStatement(Expression* vertexCount,
                      const SourceLocation& loc)
        : MeshStatement(loc),
          vertexCount(vertexCount)
    {
    }

    VerticesStatement(std::unique_ptr<Expression> vertexCount,
                      const SourceLocation& loc)
        : MeshStatement(loc),
          vertexCount(std::move(vertexCount))
    {
    }

    ~VerticesStatement() override = default;

    bool HasVertexCount() const
    {
        return vertexCount != nullptr;
    }

    const Expression* GetVertexCount() const
    {
        return vertexCount.get();
    }

    Expression* GetVertexCount()
    {
        return vertexCount.get();
    }

    void SetVertexCount(Expression* expression)
    {
        vertexCount.reset(expression);
    }

    void SetVertexCount(std::unique_ptr<Expression> expression)
    {
        vertexCount = std::move(expression);
    }

    void printout() const override
    {
        std::cout
            << "VerticesStatement"
            << " line=" << GetLocation().line
            << '\n';

        if (vertexCount)
        {
            std::cout << "  vertexCount:\n";
            vertexCount->printout();
        }
        else
        {
            std::cout << "  vertexCount: <null>\n";
        }
    }

private:
    std::unique_ptr<Expression> vertexCount;
};

} // namespace feelfem2

#endif // FEELFEM2_VERTICESSTATEMENT_HPP
