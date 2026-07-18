                                                                                                /*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : RegionDecl.hpp
 *  Date     : 2026/07/18
 *
 *  Purpose  : Region declarator AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents one region declaration such as
 *
 *            reg1[tri](e1,e2,e3)
 *            reg2[tri](e1,e2,e3:[h1,h2,h3])
 *
 *      - Stores one outer boundary and zero or more hole boundaries.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_REGIONDECL_HPP
#define FEELFEM2_REGIONDECL_HPP

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Mesh.hpp"

namespace feelfem2
{

class RegionDecl : public MeshStatement
{
public:
    using Boundary = std::vector<std::string>;
    using HoleList = std::vector<Boundary>;

    RegionDecl() = default;

    RegionDecl(std::string regionName,
               std::string meshType,
               Boundary outerBoundary,
               HoleList holes,
               const SourceLocation& loc)
        : MeshStatement(loc),
          regionName(std::move(regionName)),
          meshType(std::move(meshType)),
          outerBoundary(std::move(outerBoundary)),
          holes(std::move(holes))
    {
    }

    ~RegionDecl() override = default;

    const std::string& GetRegionName() const
    {
        return regionName;
    }

    const std::string& GetMeshType() const
    {
        return meshType;
    }

    const Boundary& GetOuterBoundary() const
    {
        return outerBoundary;
    }

    const HoleList& GetHoles() const
    {
        return holes;
    }

    std::size_t GetHoleCount() const
    {
        return holes.size();
    }

    void printout() const override
    {
        std::cout
            << "RegionDecl"
            << " name=" << regionName
            << " meshType=" << meshType
            << " outerCount=" << outerBoundary.size()
            << " holes=" << holes.size()
            << " line=" << GetLocation().line
            << '\n';

        std::cout << "  outerBoundary:\n";

        for (std::size_t i = 0; i < outerBoundary.size(); ++i)
        {
            std::cout
                << "    item[" << i << "]="
                << outerBoundary[i]
                << '\n';
        }

        for (std::size_t holeIndex = 0;
             holeIndex < holes.size();
             ++holeIndex)
        {
            std::cout
                << "  hole[" << holeIndex << "]:\n";

            const auto& hole = holes[holeIndex];

            for (std::size_t i = 0; i < hole.size(); ++i)
            {
                std::cout
                    << "    item[" << i << "]="
                    << hole[i]
                    << '\n';
            }
        }
    }

private:
    std::string regionName;
    std::string meshType;

    Boundary outerBoundary;
    HoleList holes;
};

} // namespace feelfem2

#endif // FEELFEM2_REGIONDECL_HPP
