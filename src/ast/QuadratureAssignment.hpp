/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : QuadratureAssignment.hpp
 *  Date     : 2026/07/19
 *
 *  Purpose  : Assignment inside a quadrature definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 */

#ifndef FEELFEM2_QUADRATUREASSIGNMENT_HPP
#define FEELFEM2_QUADRATUREASSIGNMENT_HPP

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "QuadratureItem.hpp"
#include "Expression.hpp"

namespace feelfem2
{

class QuadratureAssignment : public QuadratureItem
{
public:
    QuadratureAssignment(
        std::string targetName,
        Expression* value,
        const SourceLocation& loc)
        : QuadratureItem(loc),
          targetName(std::move(targetName)),
          value(value)
    {
    }

    ~QuadratureAssignment() override = default;

    const std::string& GetTargetName() const
    {
        return targetName;
    }

    const Expression* GetValue() const
    {
        return value.get();
    }

    void printout() const override
    {
        std::cout
            << "QuadratureAssignment"
            << " target=" << targetName
            << " line=" << GetLocation().line
            << '\n';

        if (value)
        {
            std::cout << "  value:\n";
            value->printout();
        }
    }

private:
    std::string targetName;
    std::unique_ptr<Expression> value;
};

} // namespace feelfem2

#endif
