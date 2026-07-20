/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : ElementVariableDeclaration.hpp
 *  Date     : 2026/07/20
 *
 *  Purpose  : Variable declaration inside an element definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a double declaration in an element block.
 *      - May contain one or more variable declarators.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_ELEMENTVARIABLEDECLARATION_HPP
#define FEELFEM2_ELEMENTVARIABLEDECLARATION_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "ElementItem.hpp"
#include "ElementVariableDeclarator.hpp"

namespace feelfem2 {

class ElementVariableDeclaration : public ElementItem {
public:
    using DeclaratorPtr =
        std::unique_ptr<ElementVariableDeclarator>;

    using DeclaratorList =
        std::vector<DeclaratorPtr>;

    ElementVariableDeclaration() = default;

    ElementVariableDeclaration(
        DeclaratorList declarators,
        const SourceLocation& loc)
        : ElementItem(loc),
          declarators_(std::move(declarators))
    {
    }

    ~ElementVariableDeclaration() override = default;

    ElementVariableDeclaration(
        const ElementVariableDeclaration&) = delete;

    ElementVariableDeclaration& operator=(
        const ElementVariableDeclaration&) = delete;

    ElementVariableDeclaration(
        ElementVariableDeclaration&&) noexcept = default;

    ElementVariableDeclaration& operator=(
        ElementVariableDeclaration&&) noexcept = default;

    const DeclaratorList& GetDeclarators() const
    {
        return declarators_;
    }

    DeclaratorList& GetDeclarators()
    {
        return declarators_;
    }

    std::size_t GetDeclaratorCount() const
    {
        return declarators_.size();
    }

    bool IsEmpty() const
    {
        return declarators_.empty();
    }

    void printout() const override
    {
        std::cout
            << "ElementVariableDeclaration"
            << " count=" << declarators_.size()
            << " line=" << GetLocation().line
            << "\n";

        for (const auto& declarator : declarators_) {
            if (declarator) {
                declarator->printout();
            }
        }
    }

private:
    DeclaratorList declarators_;
};

using ElementVariableDeclarationPtr =
    std::unique_ptr<ElementVariableDeclaration>;

} // namespace feelfem2

#endif // FEELFEM2_ELEMENTVARIABLEDECLARATION_HPP
