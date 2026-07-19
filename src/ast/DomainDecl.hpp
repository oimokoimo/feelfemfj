/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : DomainDecl.hpp
 *  Date     : 2026/07/18
 *
 *  Purpose  : Domain declarator AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents one domain declarator such as
 *
 *            dom1(reg1, reg2)
 *
 *      - In two-dimensional problems, the identifiers normally
 *        refer to regions.
 *      - In three-dimensional problems, the identifiers normally
 *        refer to volumes.
 *      - Dimensional consistency is checked by Semantic Analyzer.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_DOMAINDECL_HPP
#define FEELFEM2_DOMAINDECL_HPP

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Mesh.hpp"

namespace feelfem2
{

class DomainDecl : public MeshStatement
{
public:
    DomainDecl() = default;

    DomainDecl(std::string domainName,
               std::vector<std::string>* componentNames,
               const SourceLocation& loc)
        : MeshStatement(loc),
          domainName(std::move(domainName))
    {
        if (componentNames != nullptr)
        {
            components = std::move(*componentNames);
            delete componentNames;
        }
    }

    ~DomainDecl() override = default;

    const std::string& GetDomainName() const
    {
        return domainName;
    }

    void SetDomainName(std::string name)
    {
        domainName = std::move(name);
    }

    void AddComponentName(std::string name)
    {
        components.push_back(std::move(name));
    }

    std::size_t GetComponentCount() const
    {
        return components.size();
    }

    const std::string& GetComponentName(std::size_t index) const
    {
        return components.at(index);
    }

    const std::vector<std::string>& GetComponentNames() const
    {
        return components;
    }

    void printout() const override
    {
        std::cout
            << "DomainDecl"
            << " name=" << domainName
            << " components=" << components.size()
            << " line=" << GetLocation().line
            << '\n';

        for (std::size_t i = 0; i < components.size(); ++i)
        {
            std::cout
                << "  component[" << i << "]="
                << components[i]
                << '\n';
        }
    }

private:
    std::string domainName;
    std::vector<std::string> components;
};

} // namespace feelfem2

#endif // FEELFEM2_DOMAINDECL_HPP
