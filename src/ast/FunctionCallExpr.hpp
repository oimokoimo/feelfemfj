/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : FunctionCallExpr.hpp
 *  Date     : 2026/07/19
 *
 *  Purpose  : Function-call expression AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a function call such as
 *
 *            sqrt(5.0)
 *            sin(x)
 *            max(a, b)
 *
 *      - Stores arguments in source order.
 *      - Parser-created raw Expression pointers are owned by this node
 *        through std::unique_ptr.
 *      - Function-name resolution and argument checking are performed
 *        by Semantic Analyzer.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_FUNCTIONCALLEXPR_HPP
#define FEELFEM2_FUNCTIONCALLEXPR_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Expression.hpp"

namespace feelfem2
{

class FunctionCallExpr : public Expression
{
public:
    FunctionCallExpr() = default;

    FunctionCallExpr(std::string functionName,
                     const SourceLocation& loc)
        : Expression(loc),
          functionName(std::move(functionName))
    {
    }

    FunctionCallExpr(std::string functionName,
                     std::vector<Expression*>* argumentList,
                     const SourceLocation& loc)
        : Expression(loc),
          functionName(std::move(functionName))
    {
        if (argumentList != nullptr)
        {
            for (Expression* argument : *argumentList)
            {
                arguments.emplace_back(argument);
            }

            delete argumentList;
        }
    }

    ~FunctionCallExpr() override = default;

    const std::string& GetFunctionName() const
    {
        return functionName;
    }

    void SetFunctionName(std::string name)
    {
        functionName = std::move(name);
    }

    void AddArgument(Expression* argument)
    {
        arguments.emplace_back(argument);
    }

    void AddArgument(std::unique_ptr<Expression> argument)
    {
        arguments.push_back(std::move(argument));
    }

    std::size_t GetArgumentCount() const
    {
        return arguments.size();
    }

    const Expression* GetArgument(std::size_t index) const
    {
        return arguments.at(index).get();
    }

    Expression* GetArgument(std::size_t index)
    {
        return arguments.at(index).get();
    }

    const std::vector<std::unique_ptr<Expression>>&
    GetArguments() const
    {
        return arguments;
    }

    void printout() const override
    {
        std::cout
            << "FunctionCallExpr"
            << " name=" << functionName
            << " arguments=" << arguments.size()
            << " line=" << GetLocation().line
            << '\n';

        for (std::size_t i = 0; i < arguments.size(); ++i)
        {
            std::cout << "  argument[" << i << "]:\n";

            if (arguments[i])
            {
                arguments[i]->printout();
            }
            else
            {
                std::cout << "    <null>\n";
            }
        }
    }

private:
    std::string functionName;

    std::vector<std::unique_ptr<Expression>> arguments;
};

} // namespace feelfem2

#endif // FEELFEM2_FUNCTIONCALLEXPR_HPP
