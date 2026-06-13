# feelfemfj

**feelfemfj (Finite Element Equation Language - from Japan)**

A modern reimplementation of the FEEL concept originally developed by Hidehiro Fujio in the 1990s.

## Overview

FEEL (Finite Element Environment Language) was a domain-specific language (DSL) for describing weak forms of partial differential equations and generating finite element analysis programs.

The original FEEL P2 system was developed in 1993-1994 and later evolved into feelfem.

This repository contains a new implementation inspired by the original concepts and mathematical ideas, designed for modern computing environments.

## Goals

* Weak-form based PDE description language
* Automatic finite element code generation
* PETSc backend support
* Fortran 2018 code generation
* Modern mesh and visualization support
* LLM-assisted PDE programming

## Project Status

This project is currently in the design and prototyping phase.

Planned components:

* FEEL DSL parser
* Symbolic processing engine
* Fortran 2018 backend
* PETSc linear solver backend
* VTK output support
* Real-time visualization support

## Historical Background

FEEL P2 was developed at NEC Corporation in the 1990s.

This repository does not contain source code owned by NEC Corporation.

The implementation in this repository is intended to be an independent reimplementation based on publicly known mathematical concepts, published papers, manuals, and newly developed software components.

## Author

Hidehiro Fujio

Japan Agency for Marine-Earth Science and Technology (JAMSTEC)

## License

TBD
