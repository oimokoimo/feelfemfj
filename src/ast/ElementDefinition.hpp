/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : ElementDefinition.hpp
 *  Date     : 2026/07/20
 *
 *  Purpose  : AST node for a finite element definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Stores the finite element name and reference-element name.
 *      - Stores declarations, assignments, and interpolation points.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_ELEMENTDEFINITION_HPP
#define FEELFEM2_ELEMENTDEFINITION_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "AstNode.hpp"
#include "ElementItem.hpp"

namespace feelfem2 {

class ElementDefinition : public AstNode {
public:
    using ItemPtr =
        std::unique_ptr<ElementItem>;

    using ItemList =
        std::vector<ItemPtr>;

    ElementDefinition() = default;

    ElementDefinition(
        std::string name,
        std::string referenceElement,
        ItemList items,
        const SourceLocation& loc)
        : AstNode(loc),
          name_(std::move(name)),
          referenceElement_(std::move(referenceElement)),
          items_(std::move(items))
    {
    }

    ~ElementDefinition() override = default;

    ElementDefinition(
        const ElementDefinition&) = delete;

    ElementDefinition& operator=(
        const ElementDefinition&) = delete;

    ElementDefinition(
        ElementDefinition&&) noexcept = default;

    ElementDefinition& operator=(
        ElementDefinition&&) noexcept = default;

    const std::string& GetName() const
    {
        return name_;
    }

    const std::string& GetReferenceElement() const
    {
        return referenceElement_;
    }

    const ItemList& GetItems() const
    {
        return items_;
    }

    ItemList& GetItems()
    {
        return items_;
    }

    std::size_t GetItemCount() const
    {
        return items_.size();
    }

    bool IsEmpty() const
    {
        return items_.empty();
    }

    void printout() const override
    {
        std::cout
            << "ElementDefinition"
            << " name=" << name_
            << " referenceElement=" << referenceElement_
            << " count=" << items_.size()
            << " line=" << GetLocation().line
            << "\n";

        for (const auto& item : items_) {
            if (item) {
                item->printout();
            }
        }
    }

private:
    std::string name_;
    std::string referenceElement_;
    ItemList items_;
};

using ElementDefinitionPtr =
    std::unique_ptr<ElementDefinition>;

} // namespace feelfem2

#endif // FEELFEM2_ELEMENTDEFINITION_HPP
