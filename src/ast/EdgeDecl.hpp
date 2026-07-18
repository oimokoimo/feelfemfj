/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : EdgeDecl.hpp
 *  Date     : 2026/07/18
 *
 *  Purpose  : Edge declarator AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents one edge declarator such as
 *
 *            e1(a,b,c,d,a)
 *
 *      - Stores the edge name and an ordered list of point names.
 *      - Parametric edges are represented separately.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_EDGEDECL_HPP
#define FEELFEM2_EDGEDECL_HPP

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Mesh.hpp"

namespace feelfem2
{

class EdgeDecl : public MeshStatement
{
public:
    EdgeDecl() = default;

    EdgeDecl(std::string edgeName,
             std::vector<std::string>* pointNames,
             const SourceLocation& loc)
        : MeshStatement(loc),
          edgeName(std::move(edgeName))
    {
        if (pointNames != nullptr)
        {
            points = std::move(*pointNames);
            delete pointNames;
        }
    }

    ~EdgeDecl() override = default;

    const std::string& GetEdgeName() const
    {
        return edgeName;
    }

    void SetEdgeName(std::string name)
    {
        edgeName = std::move(name);
    }

    void AddPointName(std::string name)
    {
        points.push_back(std::move(name));
    }

    std::size_t GetPointCount() const
    {
        return points.size();
    }

    const std::string& GetPointName(std::size_t index) const
    {
        return points.at(index);
    }

    const std::vector<std::string>& GetPointNames() const
    {
        return points;
    }

    void printout() const override
    {
        std::cout
            << "EdgeDecl"
            << " name=" << edgeName
            << " points=" << points.size()
            << " line=" << GetLocation().line
            << '\n';

        for (std::size_t i = 0; i < points.size(); ++i)
        {
            std::cout
                << "  point[" << i << "]="
                << points[i]
                << '\n';
        }
    }

private:
    std::string edgeName;
    std::vector<std::string> points;
};

} // namespace feelfem2

#endif // FEELFEM2_EDGEDECL_HPP
