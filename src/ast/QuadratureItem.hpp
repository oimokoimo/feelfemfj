/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : QuadratureItem.hpp
 *  Date     : 2026/07/19
 *
 *  Purpose  : Base class for quadrature-definition items
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 */

#ifndef FEELFEM2_QUADRATUREITEM_HPP
#define FEELFEM2_QUADRATUREITEM_HPP

#include "AstNode.hpp"

namespace feelfem2
{

class QuadratureItem : public AstNode
{
public:
    QuadratureItem() = default;

    explicit QuadratureItem(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ~QuadratureItem() override = default;
};

} // namespace feelfem2

#endif // FEELFEM2_QUADRATUREITEM_HPP
