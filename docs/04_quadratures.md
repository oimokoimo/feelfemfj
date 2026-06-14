# Quadrature Definitions

## Overview

A quadrature definition describes a numerical integration rule used by weak-form integrals.

Unlike many finite element systems, quadrature rules are not hard-coded into the compiler. They are explicitly defined within the language.

A quadrature definition consists of:

* a quadrature name
* a reference element shape
* optional quadrature auxiliary variables
* a list of quadrature points and weights

Quadrature auxiliary variables are provided to simplify the description of numerical integration rules.

All auxiliary variables are evaluated at compile time.

Consequently, quadrature points and weights are precomputed and embedded as constants into the generated Fortran or C++ source code.

---

## Syntax

```feel
quadrature <name>[<shape>] {

    <quadrature auxiliary variable declarations>

    (<integration point>) : <weight>;

    ...
}
```

---

## Quadrature Auxiliary Variables

Auxiliary variables may be declared inside a quadrature block.

Example:

```feelfem
double a = 0.445948490915965;
double b = 0.091576213509771;
```

These variables are visible only within the quadrature definition.

They are evaluated during compilation and do not appear as runtime variables.

---

## Example: Fourth-Order Triangle Quadrature

```feelfem
quadrature tri4[tri] {

    double a = 0.445948490915965;
    double b = 0.091576213509771;

    (a, a)       : 0.111690794839005;
    (1-2*a, a)   : 0.111690794839005;
    (a, 1-2*a)   : 0.111690794839005;

    (b, b)       : 0.054975871827661;
    (1-2*b, b)   : 0.054975871827661;
    (b, 1-2*b)   : 0.054975871827661;
}
```

This definition generates six quadrature points and corresponding weights on the reference triangle.

The generated code contains only numerical constants, for example:

```fortran
qp(1,1) = 0.445948490915965d0
qp(1,2) = 0.445948490915965d0
w(1)    = 0.111690794839005d0
```

No runtime evaluation of the expressions is required.

---

## Design Rationale

Quadrature rules are treated as mathematical data rather than compiler knowledge.

This allows users to:

* define custom quadrature rules
* experiment with new integration schemes
* control integration accuracy independently for each weak-form term
* extend the language without modifying the compiler

For example,

```feelfem
integral[tri4](k*grad(u)*grad(tu))
+ integral[tri7](beta*u*tu)
```

uses different quadrature rules for different contributions to the weak formulation.

