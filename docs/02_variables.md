# Variables

feelfem provides several classes of variables corresponding to different finite element spaces and data representations.

## Scalar Variables

Scalar variables represent ordinary numerical values.

### Double Precision Variables

```feel
double k;
double alpha, beta;
```

### Integer Variables

```feel
int n;
```

### Constant Variables

```feel
const double pi = 3.141592653589793;
const int ndim = 3;
```

Constant variables are evaluated at compile time whenever possible.

---

## FEM Variables

```feel
fem u[P1];
```

A FEM variable belongs to a finite element space.

A finite element space is defined by:

* an element definition
* interpolation functions
* a continuity condition between neighboring elements

For example,

```feel
fem u[P1];
```

defines a scalar field represented by the P1 finite element.

Mathematically,

[
u_h = \sum_i u_i \phi_i
]

where the basis functions (\phi_i) are defined by the selected finite element.

Examples:

```feel
fem temperature[P1];
fem pressure[P2];
```

---

## Vector FEM Variables

```feel
vfem A[Nedelec1];
```

A vector FEM variable belongs to a vector-valued finite element space.

Typical applications include:

* electromagnetics
* Maxwell equations
* H(curl) formulations
* H(div) formulations

Examples:

```feel
vfem E[Nedelec1];
vfem H[Nedelec1];
```

The exact continuity properties depend on the selected element definition.

---

## Element-wise Variables

```feel
ewise stress[tri4];
```

Element-wise variables are defined independently for each element.

No continuity is imposed between neighboring elements.

### Quadrature-based Variables

```feel
ewise stress[tri4];
```

Values are stored at quadrature points.

Typical applications:

* stress tensors
* strain tensors
* turbulence quantities
* nonlinear constitutive models

---

### Discontinuous Finite Element Variables

```feel
ewise p[P1];
```

Values are represented by interpolation functions but remain independent between elements.

Such variables correspond to discontinuous finite element spaces and may be used for discontinuous Galerkin formulations.

---

## Material Variables

If no quadrature rule or finite element is specified,

```feel
ewise conductivity;
ewise density;
```

the variable is treated as a material variable.

A material variable stores a single value for each element.

Example:

```feel
ewise k;
ewise rho;
```

Internally,

[
k = k_K
]

is constant within each element (K).

Material variables are commonly used for:

* material identifiers
* conductivity
* density
* permeability
* viscosity
* element-wise coefficients

---

## Summary

| Variable Type     | Representation              | Continuity               |
| ----------------- | --------------------------- | ------------------------ |
| double            | scalar value                | N/A                      |
| int               | integer value               | N/A                      |
| fem               | finite element space        | continuous or conforming |
| vfem              | vector finite element space | element dependent        |
| ewise[quadrature] | quadrature-point values     | discontinuous            |
| ewise[element]    | DG finite element space     | discontinuous            |
| ewise             | one value per element       | discontinuous            |

```
```

