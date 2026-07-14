/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : VarSection.hpp
 *  Date     : 2026/07/14
 *
 *  Purpose  : Variable section AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents the complete var { ... } section.
 *      - Stores scalar and field declarations.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_VARSECTION_HPP
#define FEELFEM2_VARSECTION_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "AstNode.hpp"
#include "Declaration.hpp"

namespace feelfem2
{

class VarSection : public AstNode
{
public:
    VarSection() = default;

    explicit VarSection(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ~VarSection() override = default;

    void AddDeclaration(Declaration* declaration)
    {
        declarations.emplace_back(declaration);
    }

    void AddDeclaration(std::unique_ptr<Declaration> declaration)
    {
        declarations.push_back(std::move(declaration));
    }

    std::size_t GetDeclarationCount() const
    {
        return declarations.size();
    }

    const Declaration* GetDeclaration(std::size_t index) const
    {
        return declarations.at(index).get();
    }

    Declaration* GetDeclaration(std::size_t index)
    {
        return declarations.at(index).get();
    }

    const std::vector<std::unique_ptr<Declaration>>&
    GetDeclarations() const
    {
        return declarations;
    }

    void printout() const override
    {
        std::cout
            << "VarSection"
            << " count=" << declarations.size()
            << " line=" << GetLocation().line
            << '\n';

        for (const auto& declaration : declarations)
        {
            if (declaration)
            {
                declaration->printout();
            }
        }
    }

private:
    std::vector<std::unique_ptr<Declaration>> declarations;
};

} // namespace feelfem2

#endif // FEELFEM2_VARSECTION_HPP
