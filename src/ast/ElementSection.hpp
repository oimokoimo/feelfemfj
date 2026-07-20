/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : ElementSection.hpp
 *  Date     : 2026/07/20
 *
 *  Purpose  : AST node for a collection of finite element definitions
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Owns all ElementDefinition nodes.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_ELEMENTSECTION_HPP
#define FEELFEM2_ELEMENTSECTION_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "AstNode.hpp"
#include "ElementDefinition.hpp"

namespace feelfem2 {

class ElementSection : public AstNode {
public:
    using DefinitionPtr =
        std::unique_ptr<ElementDefinition>;

    using DefinitionList =
        std::vector<DefinitionPtr>;

    ElementSection() = default;

    explicit ElementSection(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ElementSection(
        DefinitionList definitions,
        const SourceLocation& loc)
        : AstNode(loc),
          definitions_(std::move(definitions))
    {
    }

    ~ElementSection() override = default;

    ElementSection(const ElementSection&) = delete;
    ElementSection& operator=(const ElementSection&) = delete;

    ElementSection(ElementSection&&) noexcept = default;
    ElementSection& operator=(ElementSection&&) noexcept = default;

    void AddDefinition(ElementDefinition* definition)
    {
        definitions_.emplace_back(definition);
    }

    void AddDefinition(DefinitionPtr definition)
    {
        definitions_.push_back(std::move(definition));
    }

    const DefinitionList& GetDefinitions() const
    {
        return definitions_;
    }

    DefinitionList& GetDefinitions()
    {
        return definitions_;
    }

    std::size_t GetDefinitionCount() const
    {
        return definitions_.size();
    }

    bool IsEmpty() const
    {
        return definitions_.empty();
    }

    void printout() const override
    {
        std::cout
            << "ElementSection"
            << " count=" << definitions_.size()
            << " line=" << GetLocation().line
            << "\n";

        for (const auto& definition : definitions_) {
            if (definition) {
                definition->printout();
            }
        }
    }

private:
    DefinitionList definitions_;
};

using ElementSectionPtr =
    std::unique_ptr<ElementSection>;

} // namespace feelfem2

#endif // FEELFEM2_ELEMENTSECTION_HPP
