/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : RegionBoundary.hpp
 *  Date     : 2026/07/18
 *
 *  Purpose  : Temporary region boundary representation used by parser
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 */

#ifndef FEELFEM2_REGIONBOUNDARY_HPP
#define FEELFEM2_REGIONBOUNDARY_HPP

#include <string>
#include <vector>

namespace feelfem2
{

struct RegionBoundary
{
    using IdentifierList = std::vector<std::string>;
    using HoleList       = std::vector<IdentifierList>;

    IdentifierList outerBoundary;
    HoleList holes;
};

} // namespace feelfem2

#endif // FEELFEM2_REGIONBOUNDARY_HPP
