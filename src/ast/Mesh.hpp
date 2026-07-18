/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : Mesh.hpp
 *  Date     : 2026/07/18
 *
 *  Purpose  : Base class definition for mesh-related AST nodes
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      - Defines the common base class for statements appearing
 *        inside a mesh section.
 *      - PointDecl, PointStatement, EdgeDecl, RegionDecl,
 *        DomainDecl, and VerticesStatement derive from this class.
 *      - Concrete mesh AST nodes are defined in separate headers.
 *      - Modern C++20 implementation.
 *
 */

#ifndef FEELFEM2_MESH_HPP
#define FEELFEM2_MESH_HPP

#include "AstNode.hpp"

namespace feelfem2
{

class MeshStatement : public AstNode
{
public:
    MeshStatement() = default;

    explicit MeshStatement(const SourceLocation& loc)
        : AstNode(loc)
    {
    }

    ~MeshStatement() override = default;
};

} // namespace feelfem2

#endif // FEELFEM2_MESH_HPP
