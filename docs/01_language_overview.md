# feelfem Language Overview

## What is feelfem?

**feelfem** (*Finite Element Equation Language for FE Code Generator*) is a domain-specific language for the description of finite element applications based on weak formulations of partial differential equations.

Unlike many finite element systems, **feelfem** does not hard-code finite elements, numerical quadrature rules, or weak forms into the compiler.

Instead, these objects are explicitly described as mathematical data within the language itself.

---

## Design Philosophy

In feelfem, finite elements, numerical quadrature rules, weak forms, boundary conditions, and numerical models are represented explicitly.

### Example: Finite Element Definition

```feel
element P1[tri] {
  (0,0): 1.0-r-s;
  (1,0): r;
  (0,1): s;
}
```

### Example: Numerical Quadrature Definition

```feel
quadrature tri4[tri] {
  (a,a)     : w1;
  (1-2*a,a) : w1;
  (a,1-2*a) : w1;
}
```

The compiler interprets these definitions rather than relying on built-in knowledge of specific elements or quadrature rules.

---

## Separation of Mathematical Model and Solver

A feelfem program describes the mathematical problem.

The implementation details of sparse matrices, iterative solvers, parallel communication, and visualization are delegated to backend components.

For example,

```feel
solve(u;tu) {
    solver PETSc(CGS);
}
```

specifies a solver backend without exposing matrix storage formats or implementation details.

---

## Variable Classes

feelfem distinguishes between two fundamental classes of variables.

### FEM Variables

```feel
fem u[P1];
```

A FEM variable belongs to a finite element space and is represented by interpolation functions defined over the mesh.

Continuity between neighboring elements may be imposed by the chosen finite element.

---

### Element-wise Variables

```feel
ewise stress[tri4];
```

An element-wise variable is defined independently for each element.

Values may be stored:

* at quadrature points
* on discontinuous interpolation spaces

Element-wise variables do not require continuity between neighboring elements.
Element-wise variables are suitable for:

* constitutive models
* stresses
* material properties
* turbulence quantities
* discontinuous Galerkin formulations

---

## Weak Form Based Programming

The central object in feelfem is the weak formulation.

```feel
wea:
    integral[tri4](k*grad(u)*grad(tu))
  + bintegral[3](g*tu)
  = 0;
```

Each term may specify its own numerical quadrature rule.

This allows different contributions to:

* stiffness matrices
* mass matrices
* nonlinear terms
* boundary terms

to be integrated with different accuracies.

For example,

```feel
wea:
    integral[tri4](k*grad(u)*grad(tu))
  + integral[tri7](beta*u*tu)
  + bintegral[3](g*tu)
  = 0;
```

uses different quadrature rules for different terms of the weak form.

---

## feelfem as a PDE Compiler Framework

feelfem should be viewed as a **PDE compiler framework** rather than a finite element solver.

The language describes:

* finite element spaces
* quadrature rules
* weak forms
* boundary conditions
* numerical models

while backend systems generate and execute numerical codes.

Typical backends include:

* Fortran 2018
* PETSc
* MPI
* VTK
* Visualization tools

The primary objective of feelfem is to separate the mathematical description of a PDE problem from the implementation details of numerical solution methods.


The original FEEL P2 system was developed in 1993–1994 and served as the prototype of the feelfem language.
