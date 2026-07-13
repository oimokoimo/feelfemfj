/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : FieldDeclarator.hpp
 *  Date     : 2026/07/13
 *
 *  Purpose  : Field declarator AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a field declarator such as u[P1].
 *      - Used by fem, vfem, and ewise variable declarations.
 *      - The declaration kind is stored by FieldDeclaration.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_FIELDDECLARATOR_HPP
#define FEELFEM2_FIELDDECLARATOR_HPP

#include <iostream>
#include <string>
#include <utility>

#include "AstNode.hpp"

namespace feelfem2
{

class FieldDeclarator : public AstNode
{
public:
    FieldDeclarator() = default;

    FieldDeclarator(std::string fieldName,
                    std::string elementName)
        : fieldName(std::move(fieldName)),
          elementName(std::move(elementName))
    {
    }

    FieldDeclarator(std::string fieldName,
                    std::string elementName,
                    const SourceLocation& loc)
        : AstNode(loc),
          fieldName(std::move(fieldName)),
          elementName(std::move(elementName))
    {
    }

    ~FieldDeclarator() override = default;

    const std::string& GetFieldName() const
    {
        return fieldName;
    }

    void SetFieldName(std::string name)
    {
        fieldName = std::move(name);
    }

    const std::string& GetElementName() const
    {
        return elementName;
    }

    void SetElementName(std::string name)
    {
        elementName = std::move(name);
    }

    void printout() const override
    {
        std::cout
            << "FieldDeclarator"
            << " field=" << fieldName
            << " element=" << elementName
            << " line=" << GetLocation().line
            << '\n';
    }

private:
    std::string fieldName;
    std::string elementName;
};

} // namespace feelfem2

#endif // FEELFEM2_FIELDDECLARATOR_HPP
