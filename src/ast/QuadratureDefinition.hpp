/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : QuadratureDefinition.hpp
 *  Date     : 2026/07/19
 *
 *  Purpose  : Complete quadrature definition AST node
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 */

#ifndef FEELFEM2_QUADRATUREDEFINITION_HPP
#define FEELFEM2_QUADRATUREDEFINITION_HPP

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "AstNode.hpp"
#include "QuadratureItem.hpp"

namespace feelfem2
{

class QuadratureDefinition : public AstNode
{
public:
    QuadratureDefinition(
        std::string name,
        std::string referenceElementName,
        const SourceLocation& loc)
        : AstNode(loc),
          name(std::move(name)),
          referenceElementName(std::move(referenceElementName))
    {
    }

    ~QuadratureDefinition() override = default;

    void AddItem(QuadratureItem* item)
    {
        items.emplace_back(item);
    }

    const std::string& GetQuadratureName() const
    {
        return name;
    }

    const std::string& GetReferenceElementName() const
    {
        return referenceElementName;
    }


    void SetReferenceElementName(const std::string& name)
    {
        referenceElementName = name;
    }

    void printout() const override
    {
        std::cout
            << "QuadratureDefinition"
            << " name=" << name
            << " referenceElement=" << referenceElementName
            << " items=" << items.size()
            << " line=" << GetLocation().line
            << '\n';

        for (const auto& item : items)
        {
            if (item)
            {
                item->printout();
            }
        }
    }

private:
    std::string name;
    std::string referenceElementName;
    std::vector<std::unique_ptr<QuadratureItem>> items;
};

} // namespace feelfem2

#endif
