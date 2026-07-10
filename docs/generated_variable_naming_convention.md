# Naming Convention for Generated Finite Element Variables  
# FEEL / feelfemfj における有限要素生成変数の命名規則

## 1. Overview / 概要

FEEL and feelfemfj automatically generate many variables representing finite element quantities.  
Their names are constructed systematically so that the mathematical role of each variable can be inferred directly from its identifier.

FEEL および feelfemfj では、有限要素法のコード生成時に多数の変数が自動生成されます。  
これらの変数名は任意に付けられるのではなく、各変数の数学的役割を識別子から直接読み取れるよう、一定の規則に従って構成されます。

The naming convention records information such as:

- finite element name,
- local node number on the reference element,
- derivative direction,
- quadrature-rule identifier,
- and whether the value belongs to the reference element or the current quadrature point.

この命名規則には、次の情報が埋め込まれます。

- 有限要素名
- 参照要素内の局所節点番号
- 微分方向
- 数値積分公式の識別番号
- 参照要素上の値か、現在の数値積分点で使用する値か

---

## 2. Reference-Element Variables / 参照要素上の変数

Variables beginning with `r` represent quantities defined or precomputed on the **reference element**.

`r` で始まる変数は、**参照要素**上で定義または事前計算された量を表します。

Example:

```text
rP1_1_2
```

The components have the following meanings:

| Component | English | 日本語 |
|---|---|---|
| `r` | Reference element | 参照要素 |
| `P1` | Finite element name | 有限要素名 |
| first `1` | Local node number | 参照要素内の局所節点番号 |
| final `2` | Quadrature-rule identifier | 数値積分公式の識別番号 |

Therefore,

```fortran
rP1_1_2(itp)
```

means:

> The value of the first shape function of the `P1` element at quadrature point `itp`, using quadrature rule No. 2.

すなわち、

> 数値積分公式 2 の第 `itp` 積分点における、`P1` 要素の第 1 形状関数の値

を表します。

---

## 3. Shape-Function Derivatives / 形状関数の微分

Examples:

```text
rP1_1_x_2
rP1_1_y_2
```

Their components are interpreted as follows:

| Component | English | 日本語 |
|---|---|---|
| `r` | Reference element | 参照要素 |
| `P1` | Finite element name | 有限要素名 |
| `1` | Local node number | 局所節点番号 |
| `x` or `y` | Derivative direction on the reference element | 参照要素座標に関する微分方向 |
| `2` | Quadrature-rule identifier | 数値積分公式の識別番号 |

For example,

```fortran
rP1_1_x_2(itp)
```

represents the derivative of the first `P1` shape function with respect to the first reference coordinate, evaluated at quadrature point `itp`.

例えば、

```fortran
rP1_1_x_2(itp)
```

は、数値積分公式 2 の第 `itp` 積分点における、第 1 `P1` 形状関数の第 1 参照座標方向微分を表します。

Similarly,

```fortran
rP1_1_y_2(itp)
```

represents the derivative with respect to the second reference coordinate.

同様に、

```fortran
rP1_1_y_2(itp)
```

は、第 2 参照座標方向の微分を表します。

> Note: The suffixes `x` and `y` are historical FEEL names. In mathematical descriptions they correspond to derivatives with respect to the reference coordinates, often written as `xi` and `eta`.

> 注記: 接尾辞 `x` と `y` は FEEL の歴史的な命名です。数学的には、通常 `xi`、`eta` などで表される参照座標に関する微分に対応します。

---

## 4. Current-Quadrature-Point Variables / 現在の数値積分点での変数

Variables beginning with `q` represent values used at the **current quadrature point**.

`q` で始まる変数は、**現在処理している数値積分点**で使用する値を表します。

Examples:

```text
qP1_1
qP1_2
qP1_3
```

Typical generated code is:

```fortran
qP1_1 = rP1_1_2(itp)
qP1_2 = rP1_2_2(itp)
qP1_3 = rP1_3_2(itp)
```

Here,

```text
qP1_1
```

means:

> The value of the first `P1` shape function at the current quadrature point.

ここで、

```text
qP1_1
```

は、

> 現在の数値積分点における `P1` 要素の第 1 形状関数の値

を表します。

The `q` variables act as local aliases for reference-element tables indexed by the current quadrature-point number.

`q` 変数は、現在の数値積分点番号で参照された参照要素上の表データを、局所的に利用するための別名として機能します。

---

## 5. General Naming Patterns / 一般的な命名形式

### 5.1 Shape-function value on the reference element

```text
r<ElementName>_<LocalNode>_<QuadratureRule>
```

Example:

```text
rP1_2_4
```

Meaning:

- element: `P1`
- local node: `2`
- quadrature rule: `4`

### 5.2 Shape-function derivative on the reference element

```text
r<ElementName>_<LocalNode>_<Direction>_<QuadratureRule>
```

Example:

```text
rP1_3_x_4
```

Meaning:

- element: `P1`
- local node: `3`
- derivative direction: `x`
- quadrature rule: `4`

### 5.3 Value at the current quadrature point

```text
q<ElementName>_<LocalNode>
```

Example:

```text
qP1_3
```

Meaning:

- element: `P1`
- local node: `3`
- value evaluated at the current quadrature point

---

## 6. Example in Generated Fortran / 生成 Fortran コードの例

```fortran
do itp = 1, NPG2

  rj(1,1) = x1*rP1_1_x_2(itp) &
           +x2*rP1_2_x_2(itp) &
           +x3*rP1_3_x_2(itp)

  rj(1,2) = y1*rP1_1_x_2(itp) &
           +y2*rP1_2_x_2(itp) &
           +y3*rP1_3_x_2(itp)

  rj(2,1) = x1*rP1_1_y_2(itp) &
           +x2*rP1_2_y_2(itp) &
           +x3*rP1_3_y_2(itp)

  rj(2,2) = y1*rP1_1_y_2(itp) &
           +y2*rP1_2_y_2(itp) &
           +y3*rP1_3_y_2(itp)

  det = rj(1,1)*rj(2,2) - rj(1,2)*rj(2,1)
  wt  = det*w_2(itp)

  qP1_1 = rP1_1_2(itp)
  qP1_2 = rP1_2_2(itp)
  qP1_3 = rP1_3_2(itp)

  ea(1,1) = ea(1,1) + (-sc_k*qP1_1**2)*wt
  ea(1,2) = ea(1,2) + (-sc_k*qP1_1*qP1_2)*wt

end do
```

In this example:

- `_2` identifies quadrature rule No. 2.
- `itp` is the current quadrature-point index.
- `rP1_1_2(itp)` is a precomputed reference-element value.
- `qP1_1` is the corresponding local value at the current quadrature point.
- `ea` is the local element matrix.

この例では、

- `_2` は数値積分公式 2 を表します。
- `itp` は現在の数値積分点番号です。
- `rP1_1_2(itp)` は参照要素上で事前計算された値です。
- `qP1_1` は現在の積分点で使用する対応する局所変数です。
- `ea` は要素行列です。

---

## 7. Design Rationale / 設計上の意図

The naming convention was designed for generated numerical code rather than for manually written application code.

この命名規則は、人間が手書きするアプリケーションコードではなく、自動生成される数値計算コードのために設計されています。

Its principal goals are:

1. **Deterministic generation**  
   The code generator can construct names mechanically from finite-element metadata.

2. **Uniqueness**  
   Element names, local node numbers, derivative directions, and quadrature identifiers prevent name collisions.

3. **Traceability**  
   A generated variable can be traced back to the corresponding finite-element quantity.

4. **Readability of generated code**  
   Although compact, the identifier retains its mathematical meaning.

5. **Efficient generated code**  
   Frequently used table entries can be copied into local variables before element-matrix evaluation.

主な目的は次の通りです。

1. **決定的なコード生成**  
   コード生成器が有限要素メタデータから機械的に名前を構成できます。

2. **一意性**  
   要素名、局所節点番号、微分方向、数値積分識別番号によって名前の衝突を防ぎます。

3. **追跡可能性**  
   生成された変数から、対応する有限要素量を逆に特定できます。

4. **生成コードの可読性**  
   短い識別子でありながら、数学的意味を保持します。

5. **生成コードの効率性**  
   繰り返し参照される表データを局所変数へ移し、要素行列計算で利用できます。

---

## 8. Policy for feelfemfj / feelfemfj における方針

feelfemfj preserves the essential idea of this naming convention.

feelfemfj では、この命名規則の基本思想を継承します。

The exact spelling may be refined when necessary, but generated names should continue to expose:

- the finite element,
- local basis-function number,
- derivative direction,
- and quadrature rule.

必要に応じて表記を整理することはありますが、生成変数名から少なくとも次の情報を読み取れる設計を維持します。

- 有限要素
- 局所基底関数番号
- 微分方向
- 数値積分公式

Any changes to the naming convention should be documented and covered by code-generation tests.

命名規則を変更する場合は、この文書を更新し、コード生成テストによって変更内容を確認するものとします。
