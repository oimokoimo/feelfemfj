/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : QuadratureSection.hpp
 *  Date     : 2026/07/19
 *
 *  Purpose  : Quadrature section AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a collection of quadrature definitions.
 *      - Stores QuadratureDefinition nodes in source order.
 *      - A feelfem input file may contain multiple quadrature
 *        definitions for different reference elements or
 *        integration orders.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_QUADRATURESECTION_HPP
#define FEELFEM2_QUADRATURESECTION_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "AstNode.hpp"
#include "QuadratureDefinition.hpp"

namespace feelfem2
{

class QuadratureSection : public AstNode
{
public:
    QuadratureSection() = default;

    explicit QuadratureSection(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ~QuadratureSection() override = default;

    void AddDefinition(QuadratureDefinition* definition)
    {
        definitions.emplace_back(definition);
    }

    void AddDefinition(
        std::unique_ptr<QuadratureDefinition> definition)
    {
        definitions.push_back(std::move(definition));
    }

    std::size_t GetDefinitionCount() const
    {
        return definitions.size();
    }

    bool Empty() const
    {
        return definitions.empty();
    }

    const QuadratureDefinition*
    GetDefinition(std::size_t index) const
    {
        return definitions.at(index).get();
    }

    QuadratureDefinition*
    GetDefinition(std::size_t index)
    {
        return definitions.at(index).get();
    }

    const std::vector<
        std::unique_ptr<QuadratureDefinition>>&
    GetDefinitions() const
    {
        return definitions;
    }

    const QuadratureDefinition*
    FindDefinition(const std::string& quadratureName) const
    {
        for (const auto& definition : definitions)
        {
            if (definition != nullptr &&
                definition->GetQuadratureName() == quadratureName)
            {
                return definition.get();
            }
        }

        return nullptr;
    }

    QuadratureDefinition*
    FindDefinition(const std::string& quadratureName)
    {
        for (auto& definition : definitions)
        {
            if (definition != nullptr &&
                definition->GetQuadratureName() == quadratureName)
            {
                return definition.get();
            }
        }

        return nullptr;
    }

    void printout() const override
    {
        std::cout
            << "QuadratureSection"
            << " count=" << definitions.size()
            << " line=" << GetLocation().line
            << '\n';

        for (const auto& definition : definitions)
        {
            if (definition != nullptr)
            {
                definition->printout();
            }
        }
    }

private:
    std::vector<
        std::unique_ptr<QuadratureDefinition>> definitions;
};

} // namespace feelfem2

#endif // FEELFEM2_QUADRATURESECTION_HPP
