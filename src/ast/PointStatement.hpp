/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : PointStatement.hpp
 *  Date     : 2026/07/14
 *
 *  Purpose  : Point declaration statement AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a complete point statement such as
 *
 *            point a(0,0), b(1,0), c(1,1);
 *
 *      - Each point is represented by PointDecl.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_POINTSTATEMENT_HPP
#define FEELFEM2_POINTSTATEMENT_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "AstNode.hpp"
#include "PointDecl.hpp"

namespace feelfem2
{

class PointStatement : public AstNode
{
public:
    PointStatement() = default;

    explicit PointStatement(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ~PointStatement() override = default;

    void AddPoint(PointDecl* point)
    {
        points.emplace_back(point);
    }

    void AddPoint(std::unique_ptr<PointDecl> point)
    {
        points.push_back(std::move(point));
    }

    std::size_t GetPointCount() const
    {
        return points.size();
    }

    const PointDecl* GetPoint(std::size_t index) const
    {
        return points.at(index).get();
    }

    PointDecl* GetPoint(std::size_t index)
    {
        return points.at(index).get();
    }

    const std::vector<std::unique_ptr<PointDecl>>&
    GetPoints() const
    {
        return points;
    }

    void printout() const override
    {
        std::cout
            << "PointStatement"
            << " count=" << points.size()
            << " line=" << GetLocation().line
            << '\n';

        for (const auto& point : points)
        {
            if (point)
            {
                point->printout();
            }
        }
    }

private:
    std::vector<std::unique_ptr<PointDecl>> points;
};

} // namespace feelfem2

#endif // FEELFEM2_POINTSTATEMENT_HPP
