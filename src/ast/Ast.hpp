// ast/Ast.hpp
#ifndef FEELFEM2_AST_HPP
#define FEELFEM2_AST_HPP

#include "AstNode.hpp"
#include "ProgramModel.hpp"
#include "Expression.hpp"

// var block
#include "VarSection.hpp"
#include "FieldDeclarator.hpp"
#include "VariableDeclarator.hpp"
#include "VariableDeclaration.hpp"
#include "FieldDeclaration.hpp"
#include "Declaration.hpp"

// mesh block
#include "Mesh.hpp"
#include "MeshSection.hpp"

#include "PointDecl.hpp"
#include "PointStatement.hpp"

#include "EdgeDecl.hpp"
#include "EdgeStatement.hpp"

#include "RegionBoundary.hpp"
#include "RegionDecl.hpp"
#include "RegionStatement.hpp"

#include "DomainDecl.hpp"
#include "DomainStatement.hpp"

#include "VerticesStatement.hpp"


//quadrature
#include "QuadratureAssignment.hpp"
#include "QuadratureItem.hpp"
#include "QuadratureVariableDeclaration.hpp"
#include "QuadratureDefinition.hpp"
#include "QuadraturePoint.hpp"
#include "QuadratureVariableDeclarator.hpp"

// #include "VarBlock.hpp"
// #include "SchemeBlock.hpp"
// #include "SolveBlock.hpp"
// #include "Expr.hpp"
// #include "Stmt.hpp"

#endif
