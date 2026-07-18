/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : PointDecl.hpp
 *  Date     : 2026/07/18
 *
 *  Purpose  : Point declarator AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents one point declarator such as
 *
 *            a(0, 0)
 *            p(x0, y0)
 *            q(x0, y0, z0)
 *
 *      - Coordinate values are stored as expressions.
 *      - The number of coordinates is not fixed to two.
 *      - A complete point statement is represented by PointStatement.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_POINTDECL_HPP
#define FEELFEM2_POINTDECL_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "AstNode.hpp"
#include "Mesh.hpp"
#include "Expression.hpp"

namespace feelfem2
{

class PointDecl : public MeshStatement
{
public:
    PointDecl() = default;

    explicit PointDecl(std::string pointName,
                       const SourceLocation& loc = SourceLocation{})
        : MeshStatement(loc),
          pointName(std::move(pointName))
    {
    }

    PointDecl(std::string pointName,
              std::vector<Expression*>* coordinateList,
              const SourceLocation& loc)
        : MeshStatement(loc),
          pointName(std::move(pointName))
    {
        if (coordinateList != nullptr)
        {
            for (Expression* coordinate : *coordinateList)
            {
                coordinates.emplace_back(coordinate);
            }

            delete coordinateList;
        }
    }

    ~PointDecl() override = default;

    const std::string& GetPointName() const
    {
        return pointName;
    }

    void SetPointName(std::string name)
    {
        pointName = std::move(name);
    }

    void AddCoordinate(Expression* coordinate)
    {
        coordinates.emplace_back(coordinate);
    }

    void AddCoordinate(std::unique_ptr<Expression> coordinate)
    {
        coordinates.push_back(std::move(coordinate));
    }

    std::size_t GetCoordinateCount() const
    {
        return coordinates.size();
    }

    const Expression* GetCoordinate(std::size_t index) const
    {
        return coordinates.at(index).get();
    }

    Expression* GetCoordinate(std::size_t index)
    {
        return coordinates.at(index).get();
    }

    const std::vector<std::unique_ptr<Expression>>&
    GetCoordinates() const
    {
        return coordinates;
    }

    void printout() const override
    {
        std::cout
            << "PointDecl"
            << " name=" << pointName
            << " dimension=" << coordinates.size()
            << " line=" << GetLocation().line
            << '\n';

        for (std::size_t i = 0; i < coordinates.size(); ++i)
        {
            std::cout << "  coordinate[" << i << "]:\n";

            if (coordinates[i])
            {
                coordinates[i]->printout();
            }
            else
            {
                std::cout << "    <null>\n";
            }
        }
    }

private:
    std::string pointName;

    std::vector<std::unique_ptr<Expression>> coordinates;
};

} // namespace feelfem2

#endif // FEELFEM2_POINTDECL_HPP
