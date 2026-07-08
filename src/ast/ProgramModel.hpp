/*
 *  feelfem2
 *  Copyright (C) 2025-2026 Hidehiro Fujio and contributors
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Filename : ProgramModel.hpp
 *  Date     : 2026/07/07
 *
 *  Purpose  : Program model / code generation backend definition
 *
 *  Repository:
 *      https://github.com/oimokoimo/feelfemfj
 *
 *  Notes:
 *      ProgramModel specifies the target code generation backend.
 *
 */

#ifndef FEELFEM2_PROGRAMMODEL_HPP
#define FEELFEM2_PROGRAMMODEL_HPP

#include <string>

#include "AstNode.hpp"

namespace feelfem2
{

class ProgramModel : public AstNode
{
public:

    ProgramModel() = default;

    explicit ProgramModel(const std::string& modelName)
        : modelName(modelName)
    {
    }

    ProgramModel(const std::string& modelName, const SourceLocation& loc)
        : AstNode(loc), modelName(modelName)
    {
    }

    virtual ~ProgramModel() = default;

    const std::string& GetModelName() const
    {
        return modelName;
    }

    void SetModelName(const std::string& name)
    {
        modelName = name;
    }

    void printout(void) const override{
	    std::cout << "ProgramModel is " << modelName << "\n"; 
    }

private:

    std::string modelName;

};

} // namespace feelfem2

#endif // FEELFEM2_PROGRAMMODEL_HPP
