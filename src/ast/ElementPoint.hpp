/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : ElementPoint.hpp
 *  Date     : 2026/07/20
 *
 *  Purpose  : Interpolation point and basis function inside an
 *             element definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a reference-element point and its basis function.
 *      - Coordinates are represented by Expression AST nodes.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_ELEMENTPOINT_HPP
#define FEELFEM2_ELEMENTPOINT_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "ElementItem.hpp"
#include "Expression.hpp"

namespace feelfem2 {

class ElementPoint : public ElementItem {
public:
    using CoordinatePtr =
        std::unique_ptr<Expression>;

    using CoordinateList =
        std::vector<CoordinatePtr>;

    ElementPoint() = default;

    ElementPoint(
        CoordinateList coordinates,
        std::unique_ptr<Expression> value,
        const SourceLocation& loc)
        : ElementItem(loc),
          coordinates_(std::move(coordinates)),
          value_(std::move(value))
    {
    }

    ~ElementPoint() override = default;

    ElementPoint(
        const ElementPoint&) = delete;

    ElementPoint& operator=(
        const ElementPoint&) = delete;

    ElementPoint(
        ElementPoint&&) noexcept = default;

    ElementPoint& operator=(
        ElementPoint&&) noexcept = default;

    const CoordinateList& GetCoordinates() const
    {
        return coordinates_;
    }

    CoordinateList& GetCoordinates()
    {
        return coordinates_;
    }

    std::size_t GetCoordinateCount() const
    {
        return coordinates_.size();
    }

    const Expression* GetCoordinate(std::size_t index) const
    {
        if (index >= coordinates_.size()) {
            return nullptr;
        }

        return coordinates_[index].get();
    }

    Expression* GetCoordinate(std::size_t index)
    {
        if (index >= coordinates_.size()) {
            return nullptr;
        }

        return coordinates_[index].get();
    }

    const Expression* GetValue() const
    {
        return value_.get();
    }

    Expression* GetValue()
    {
        return value_.get();
    }

    void printout() const override
    {
        std::cout
            << "ElementPoint"
            << " coordinates=" << coordinates_.size()
            << " line=" << GetLocation().line
            << "\n";

        for (std::size_t i = 0;
             i < coordinates_.size();
             ++i) {
            std::cout
                << "  coordinate[" << i << "]:\n";

            if (coordinates_[i]) {
                coordinates_[i]->printout();
            }
        }

        if (value_) {
            std::cout << "  value:\n";
            value_->printout();
        }
    }

private:
    CoordinateList coordinates_;
    std::unique_ptr<Expression> value_;
};

using ElementPointPtr =
    std::unique_ptr<ElementPoint>;

} // namespace feelfem2

#endif // FEELFEM2_ELEMENTPOINT_HPP
