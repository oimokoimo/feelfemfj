/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : QuadraturePoint.hpp
 *  Date     : 2026/07/19
 *
 *  Purpose  : Quadrature point AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 */

#ifndef FEELFEM2_QUADRATUREPOINT_HPP
#define FEELFEM2_QUADRATUREPOINT_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "QuadratureItem.hpp"
#include "Expression.hpp"

namespace feelfem2
{

class QuadraturePoint : public QuadratureItem
{
public:
    QuadraturePoint(
        std::vector<Expression*>* coordinates,
        Expression* weight,
        const SourceLocation& loc)
        : QuadratureItem(loc),
          weight(weight)
    {
        if (coordinates)
        {
            for (auto* coordinate : *coordinates)
            {
                this->coordinates.emplace_back(coordinate);
            }

            delete coordinates;
        }
    }

    ~QuadraturePoint() override = default;

    std::size_t GetCoordinateCount() const
    {
        return coordinates.size();
    }

    const Expression* GetCoordinate(std::size_t index) const
    {
        return coordinates.at(index).get();
    }

    const Expression* GetWeight() const
    {
        return weight.get();
    }

    void printout() const override
    {
        std::cout
            << "QuadraturePoint"
            << " dimension=" << coordinates.size()
            << " line=" << GetLocation().line
            << '\n';

        for (std::size_t i = 0; i < coordinates.size(); ++i)
        {
            std::cout << "  coordinate[" << i << "]:\n";

            if (coordinates[i])
            {
                coordinates[i]->printout();
            }
        }

        std::cout << "  weight:\n";

        if (weight)
        {
            weight->printout();
        }
    }

private:
    std::vector<std::unique_ptr<Expression>> coordinates;
    std::unique_ptr<Expression> weight;
};

} // namespace feelfem2

#endif
