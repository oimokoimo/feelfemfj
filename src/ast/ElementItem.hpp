/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : ElementItem.hpp
 *  Date     : 2026/07/20
 *
 *  Purpose  : Base class for items inside an element definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Base class for declarations, assignments, and element points.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_ELEMENTITEM_HPP
#define FEELFEM2_ELEMENTITEM_HPP

#include "AstNode.hpp"

namespace feelfem2 {

class ElementItem : public AstNode {
public:
    ElementItem() = default;

    explicit ElementItem(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ~ElementItem() override = default;

    ElementItem(const ElementItem&) = delete;
    ElementItem& operator=(const ElementItem&) = delete;

    ElementItem(ElementItem&&) noexcept = default;
    ElementItem& operator=(ElementItem&&) noexcept = default;

    virtual void printout() const override = 0;
};

} // namespace feelfem2

#endif // FEELFEM2_ELEMENTITEM_HPP
