/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : RegionStatement.hpp
 *  Date     : 2026/07/18
 *
 *  Purpose  : Region statement AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a complete region statement.
 *      - Each region is represented by RegionDecl.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_REGIONSTATEMENT_HPP
#define FEELFEM2_REGIONSTATEMENT_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "Mesh.hpp"
#include "RegionDecl.hpp"

namespace feelfem2
{

class RegionStatement : public MeshStatement
{
public:
    RegionStatement() = default;

    explicit RegionStatement(const SourceLocation& loc)
        : MeshStatement(loc)
    {
    }

    ~RegionStatement() override = default;

    void AddRegion(RegionDecl* region)
    {
        regions.emplace_back(region);
    }

    void AddRegion(std::unique_ptr<RegionDecl> region)
    {
        regions.push_back(std::move(region));
    }

    std::size_t GetRegionCount() const
    {
        return regions.size();
    }

    const RegionDecl* GetRegion(std::size_t index) const
    {
        return regions.at(index).get();
    }

    RegionDecl* GetRegion(std::size_t index)
    {
        return regions.at(index).get();
    }

    void printout() const override
    {
        std::cout
            << "RegionStatement"
            << " count=" << regions.size()
            << " line=" << GetLocation().line
            << '\n';

        for (const auto& region : regions)
        {
            if (region)
            {
                region->printout();
            }
        }
    }

private:
    std::vector<std::unique_ptr<RegionDecl>> regions;
};

} // namespace feelfem2

#endif // FEELFEM2_REGIONSTATEMENT_HPP
