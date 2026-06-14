# Element Definitions

## Overview

An element definition describes the interpolation functions associated with a finite element.

Unlike many finite element systems, element definitions are not hard-coded into the compiler.

Elements are described explicitly within the language.

An element definition consists of:

* an element name
* a reference element shape
* optional element auxiliary variables
* a list of interpolation functions

---

## Syntax

```feel
element <name>[<shape>] {

    <element auxiliary variable declarations>

    (<reference coordinates>) : <interpolation function>;

    ...
}
```

The coordinates `r`, `s`, and `t` denote the local coordinate system of the reference element.

Typical shapes include:

```text
line
tri
rect
tetra
hexa
```

---

## Example: Linear Tetrahedral Element

```feel
element T1[tetra] {
  (0,0,0): 1.0-r-s-t;
  (1,0,0): r;
  (0,1,0): s;
  (0,0,1): t;
}
```

---

## Example: Quadratic Tetrahedral Element

```feel
element T2[tetra] {
  (0,0,0): (1.0-r-s-t)*(2.0*(1.0-r-s-t)-1.0);
  (1,0,0): r*(2.0*r-1.0);
  (0,1,0): s*(2.0*s-1.0);
  (0,0,1): t*(2.0*t-1.0);

  (0.5,0,0)   : 4.0*r*(1.0-r-s-t);
  (0.5,0.5,0) : 4.0*r*s;
  (0,0.5,0)   : 4.0*s*(1.0-r-s-t);

  (0,0,0.5)   : 4.0*t*(1.0-r-s-t);
  (0.5,0,0.5) : 4.0*r*t;
  (0,0.5,0.5) : 4.0*s*t;
}
```

---

## Example: Linear Triangle Element

```feel
element P1[tri] {
  (0,0): 1.0-r-s;
  (1,0): r;
  (0,1): s;
}
```

---

## Element Auxiliary Variables

Element definitions may contain auxiliary variables.

Auxiliary variables are evaluated during compilation and are used to simplify the definition of complex interpolation functions.

Example:

```feel
double t1;
double t2;
```

Auxiliary variables are local to the element definition.

---

## Physical Element Coordinates

feelfem also supports element definitions expressed directly in terms of the physical element geometry.

For quadrilateral elements, the following coordinates are available:

```text
x1,y1
x2,y2
x3,y3
x4,y4
```

These values denote the coordinates of the element vertices in physical space.

This mechanism allows geometry-dependent interpolation functions to be defined directly within the language.

---

## Example: Kikuchi Modified 8-Node Element

```feel
element PK2[rect] {

    double d1;
    double d2;
    double d3;
    double d4;

    ...

    d1 = (x2-x1)*(y4-y1)
       - (x4-x1)*(y2-y1);

    ...

    (-1,-1):
        (r-1)*(1-s)*(1+s+r)*0.25
      + t1*(1-r*r)*(1-s*s);

    ...

}
```

The element definition may contain arbitrary algebraic expressions involving:

* reference coordinates
* physical coordinates
* auxiliary variables

---

## Design Rationale

Finite elements are treated as mathematical data rather than compiler knowledge.

This allows users to:

* define custom finite elements
* experiment with new interpolation schemes
* implement research elements
* reproduce published element formulations

without modifying the compiler itself.

The compiler interprets the element definition and generates the corresponding element routines automatically.

