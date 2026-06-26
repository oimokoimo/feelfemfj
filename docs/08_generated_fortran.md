# feelfem90 複数 Quadrature 仕様

## 1. 概要

feelfem90 では、同一の弱形式方程式内に複数の要素積分項が存在する場合、項ごとに異なる数値積分公式（Quadrature Rule）を指定できる。

例えば、

```feel
solve[u;tu] {

    quadrature tri4;

    weq:
        integral(52*dx(u)*dx(tu)+52*dy(u)*dy(tu))
      + integral[tri2](dx(u)*tu)
      = -bintegral(g*tu);

}
```

では、

| Weak Form | Quadrature |
|------------|------------|
| `integral(...)` | `tri4` |
| `integral[tri2](...)` | `tri2` |

となる。

---

# 2. FEEL 文法

## 2.1 デフォルト Quadrature

solve ブロック内で

```feel
quadrature tri4;
```

を指定すると、その solve ブロック内の通常の

```feel
integral(...)
```

はすべて tri4 を用いて積分される。

---

## 2.2 項別 Quadrature

各積分項は

```feel
integral[quadrature_name](...)
```

の形式で個別に quadrature を指定できる。

例

```feel
integral[tri2](...)
integral[tri7](...)
integral[gauss5](...)
```

---

## 2.3 優先順位

Quadrature の決定規則は

```
integral[...] の指定
        ↓
solve 内 quadrature 指定
```

である。

つまり

```feel
quadrature tri4;

integral(...)
integral[tri2](...)
```

では

```
integral(...)       → tri4
integral[tri2](...) → tri2
```

となる。

---

# 3. Fortran コード生成

## 3.1 Quadrature の分類

コンパイラは弱形式中で使用される Quadrature を抽出し、

```
Quadrature No.1
Quadrature No.2
...
```

のように番号を付与する。

例

```fortran
integer,parameter :: NPG1 = 6
integer,parameter :: NPG2 = 3
```

```
NPG1 → tri4
NPG2 → tri2
```

---

## 3.2 初期化モジュール

各 Quadrature ごとに独立した初期化ルーチンを生成する。

例

```fortran
use mod_eset1_1_1
use mod_eset1_1_2
```

それぞれ

```
eset1_1_1
    tri4

eset1_1_2
    tri2
```

に対応する。

---

## 3.3 Common Block

各 Quadrature は独立したデータ領域を持つ。

例

```fortran
common /em1_1_1/
     gx_1,
     gy_1,
     w_1,
     ...

common /em1_1_2/
     gx_2,
     gy_2,
     w_2,
     ...
```

---

# 4. Element Matrix 生成

各 Quadrature ごとに独立した積分ループを生成する。

## Quadrature No.1

```fortran
do itp=1,NPG1

    ...

    ea(i,j)=ea(i,j)
           + value * weight

enddo
```

対応する弱形式

```feel
integral(...)
```

---

## Quadrature No.2

```fortran
do itp=1,NPG2

    ...

    ea(i,j)=ea(i,j)
           + value * weight

enddo
```

対応する弱形式

```feel
integral[tri2](...)
```

---

# 5. 要素行列への加算

各積分ループは同一の要素行列・右辺へ寄与を加算する。

```
ea =

  Diffusion contribution
+ Convection contribution
+ Reaction contribution
+ ...
```

各積分項は独立に評価されるが、

```
ea(i,j)

eb(i)
```

へ逐次加算される。

---

# 6. 境界積分

境界積分

```feel
bintegral(...)
```

は要素積分とは独立した境界 Quadrature を使用する。

したがって

```
integral()

integral[tri2]()

bintegral()
```

は互いに独立な積分規則を持つ。

---

# 7. 実装上の利点

この仕様により、

- 拡散項だけ高次積分
- 移流項だけ低次積分
- 非線形項のみ高精度積分
- SUPG・GLS等の安定化項だけ専用積分

などを自然に記述できる。

---

# 8. 設計方針

feelfem90 は

> 「Weak Form の各積分項が、自身の Quadrature 属性を持つ」

という設計思想を採用している。

コンパイラは

1. Weak Form を解析する。
2. Quadrature ごとに積分項を分類する。
3. Quadrature ごとの積分ループを生成する。
4. すべて同一要素行列へ加算する。

という手順で Fortran コードを生成する。

---

# 9. 例

```feel
quadrature tri4;

weq:

    integral(grad(u)'*grad(v))
  + integral[tri2](dx(u)*v)
  + integral[tri7](u*v);
```

生成されるコードは概念的には

```
Loop(tri4)
    grad(u)・grad(v)

Loop(tri2)
    dx(u)*v

Loop(tri7)
    u*v
```

となり、各ループの結果は同一要素行列へ加算される。
