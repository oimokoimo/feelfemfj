/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : DomainStatement.hpp
 *  Date     : 2026/07/18
 *
 *  Purpose  : Domain statement AST node definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Represents a complete domain statement such as
 *
 *            domain dom1(reg1), dom2(reg2, reg3);
 *
 *      - Each domain is represented by DomainDecl.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_DOMAINSTATEMENT_HPP
#define FEELFEM2_DOMAINSTATEMENT_HPP

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "Mesh.hpp"
#include "DomainDecl.hpp"

namespace feelfem2
{

class DomainStatement : public MeshStatement
{
public:
    DomainStatement() = default;

    explicit DomainStatement(const SourceLocation& loc)
        : MeshStatement(loc)
    {
    }

    ~DomainStatement() override = default;

    void AddDomain(DomainDecl* domain)
    {
        domains.emplace_back(domain);
    }

    void AddDomain(std::unique_ptr<DomainDecl> domain)
    {
        domains.push_back(std::move(domain));
    }

    std::size_t GetDomainCount() const
    {
        return domains.size();
    }

    const DomainDecl* GetDomain(std::size_t index) const
    {
        return domains.at(index).get();
    }

    DomainDecl* GetDomain(std::size_t index)
    {
        return domains.at(index).get();
    }

    const std::vector<std::unique_ptr<DomainDecl>>&
    GetDomains() const
    {
        return domains;
    }

    void printout() const override
    {
        std::cout
            << "DomainStatement"
            << " count=" << domains.size()
            << " line=" << GetLocation().line
            << '\n';

        for (const auto& domain : domains)
        {
            if (domain)
            {
                domain->printout();
            }
        }
    }

private:
    std::vector<std::unique_ptr<DomainDecl>> domains;
};

} // namespace feelfem2

#endif // FEELFEM2_DOMAINSTATEMENT_HPP
