# feelfem2018 PETSc ProgramModel — 生成Fortranプログラム構造

作成日: 2026-07-24  
対象: `feelfem_petsc_programmodel` プロトタイプ

## 1. 概要

このプログラムは、feelfem2018 が生成する有限要素法 Fortran コードに PETSc の疎行列・反復法ソルバーを接続したプロトタイプである。

従来の skyline 版から、有限要素積分ルーチンや入力データ構造を保ったまま、主として次の部分を PETSc へ置き換えている。

- 大域剛性行列の格納
- 大域右辺ベクトルの格納
- 要素行列・要素ベクトルの assembly
- Dirichlet 境界条件の適用
- 線形方程式の解法

現在の実装は `PETSC_COMM_SELF` を使用する逐次 PETSc プログラムであり、P1 三角形要素による対称正定値の Poisson/Helmholtz 型問題を主対象としている。

## 2. 全体構造

```text
feelfem.F90                         メインプログラム
  |
  +-- fileread.f90                  solv_dat / feel_dat の入力
  |
  +-- let1.f90, let2.f90            Scheme の代入文から生成された初期値設定
  |
  +-- solve1.F90                    solve ブロック全体の制御
        |
        +-- feelP2data.f90          EDAT / NSET データの取り出し
        +-- subsolveff90p2.f90      自由度番号と接続数の構成
        +-- ipdinfo1_1.f90          FEM変数 u の局所自由度情報
        |
        +-- elem1.F90               全要素ループと PETSc assembly
        |     |
        |     +-- eset1_1_1.f90     第1数値積分則と基底関数値の初期化
        |     +-- eset1_1_2.f90     第2数値積分則と基底関数値の初期化
        |     +-- ecal1.f90         要素行列 ea と要素ベクトル eb の計算
        |     +-- element_data1.f90 要素行列・要素ベクトルの共有データ
        |     +-- quadrature_data*.f90
        |                            数値積分点・重み・基底関数値の共有データ
        |
        +-- dcond1_1.f90            第1 Dirichlet 条件
        +-- dcond1_2.f90            第2 Dirichlet 条件
        |
        +-- PETSc Mat / Vec / KSP   行列、右辺、境界条件、反復法ソルバー
```

## 3. 実行時の処理順序

```text
PETScInitialize
    |
    v
solv_dat の読み込み          ctrlinp
    |
    v
feel_dat の読み込み          datinp
    |
    v
FEM変数の確保                fem_f, fem_u
    |
    v
Scheme 初期値設定             let1, let2
    |
    v
solve1
    |
    +-- 要素・自由度情報の準備
    +-- PETSc Mat / Vec の生成
    +-- elem1 による全要素 assembly
    +-- Dirichlet 境界条件の収集と適用
    +-- KSP の生成・設定・実行
    +-- PETSc 解ベクトルから fem_u へ書き戻し
    |
    v
fem_u_petsc.dat の出力
    |
    v
PETScFinalize
```

## 4. メインプログラム: `feelfem.F90`

`feelfem.F90` は生成プログラム全体の入口である。

主な処理は次のとおり。

1. `PetscInitialize` により PETSc を初期化する。
2. `ctrlinp` で `solv_dat` を読み込む。
3. `datinp` で `feel_dat` を読み込む。
4. FEM変数 `fem_f`、`fem_u` を節点数 `npmax` に応じて確保する。
5. Scheme の代入文から生成された `let1`、`let2` を呼び出す。
6. `solve1` を呼び出して有限要素方程式を組み立て、PETScで解く。
7. 得られた `fem_u` を `fem_u_petsc.dat` に出力する。
8. `PetscFinalize` により PETSc を終了する。

現在の例では、Scheme の

```text
k = 1.0
f = 1.0
```

に相当する設定が、それぞれ `let1.f90` と `let2.f90` に分離されている。

## 5. solve ブロック: `solve1.F90`

`solve1.F90` は PETSc ProgramModel の中心である。有限要素法固有の情報を準備し、PETSc オブジェクトを管理する。

### 5.1 解析対象データの取得

```fortran
solveInfoPtr => solveLst%solveDataList(solveNo)
edatno = solveInfoPtr%edatno
call setP2edat(...)
```

`solv_dat` に記録された solve 情報から EDAT 番号を取り出し、要素接続 `ielem`、材料番号 `matno`、要素数 `nelem` などを得る。

### 5.2 自由度番号の作成

```fortran
call makeipfp2(..., ipf)
call makeipdp2(ipf, ipd, npmax, neq)
call ipdinfo1_1(..., ipd_u)
```

- `ipf`: 各節点に存在する自由度数
- `ipd`: 各節点の大域方程式番号の開始位置
- `ipd_u`: FEM変数 `u` の節点内自由度オフセット
- `neq`: 総方程式数

現在の P1 スカラー問題では、各有効節点に1自由度を割り当てる。

### 5.3 PETSc行列の事前確保

```fortran
call nc_connectp2(..., ncmax)
nz_per_row = max(1, np * ncmax)
```

各節点に接続する要素数の最大値 `ncmax` を求め、1行当たりの非零要素数を安全側に見積もる。

```fortran
MatCreateSeqAIJ(..., nz_per_row, ..., A, ...)
```

現在は一様な逐次 AIJ 行列を生成している。この見積もりは P1 三角形要素に対して意図的に余裕を持たせている。

### 5.4 PETScオブジェクト

`solve1` は次のPETScオブジェクトを生成する。

| オブジェクト | 役割 |
|---|---|
| `A` | 大域係数行列 |
| `b` | 大域右辺ベクトル |
| `sol` | 解ベクトル |
| `xbc` | Dirichlet境界値ベクトル |
| `ksp` | Krylov反復法ソルバー |
| `pc` | 前処理器 |

## 6. 要素 assembly: `elem1.F90`

`elem1` は全要素を巡回し、生成済みの要素積分コードと PETSc assembly を接続する。

### 6.1 数値積分データの初期化

要素ループに入る前に、各数値積分則を一度だけ初期化する。

```fortran
call eset1_1_1
call eset1_1_2
```

`eset1_1_1` と `eset1_1_2` は、数値積分点、重み、および各積分点における参照要素基底関数とその微分を、対応するデータモジュールへ格納する。

### 6.2 要素データの収集

各要素について、次を大域配列から局所配列へ取り出す。

- 節点座標 `ex`, `ey`
- 節点上の FEM変数 `efem_f`

### 6.3 要素行列・要素ベクトルの計算

```fortran
call ecal1_1(...)
```

`ecal1_1` は弱形式から生成された積分式を評価し、共有モジュール `mod_element_data1` の

```fortran
ea(3,3)
eb(3)
```

へ要素行列と要素ベクトルを書き込む。

### 6.4 PETScへの加算

大域自由度番号を PETSc の0始まり添字へ変換する。

```fortran
rows(j) = ipd(...) + iedp(j) - 1
```

その後、要素行列・要素ベクトルを加算する。

```fortran
MatSetValues(A, ..., peamat, ADD_VALUES, ...)
VecSetValues(b, ..., peb, ADD_VALUES, ...)
```

Fortran の2次元配列 `ea(:,:)` は列優先であるため、`solve1` 側で

```fortran
MatSetOption(A, MAT_ROW_ORIENTED, PETSC_FALSE, ...)
```

を指定している。

## 7. 要素積分コード: `ecal1.f90`

`ecal1_1` は、feelfem の弱形式記述から生成された要素積分ルーチンである。

主な処理は次のとおり。

1. 要素行列 `ea` と要素ベクトル `eb` をゼロ初期化する。
2. 第1数値積分則を使い、勾配項と右辺項を積分する。
3. Jacobian行列 `rj`、逆行列 `rij`、行列式 `det` を計算する。
4. 参照座標上の基底関数微分を物理座標へ変換する。
5. 第2数値積分則を使い、反応項などを積分する。

このルーチンは PETSc に依存しない。PETSc化によって変更されたのは、主として `elem1` より外側の大域 assembly と solver 部分である。

## 8. 数値積分・要素共有データ

### `quadrature_data1_1_1.f90`

第1積分則のデータを保持する。

- 積分点数 `NPG1 = 6`
- 積分点 `gx_1`, `gy_1`
- 重み `w_1`
- P1基底関数値と微分 `rP1_*_1`

### `quadrature_data1_1_2.f90`

第2積分則のデータを保持する。

- 積分点数 `NPG2 = 3`
- 積分点 `gx_2`, `gy_2`
- 重み `w_2`
- P1基底関数値と微分 `rP1_*_2`

### `element_data1.f90`

要素行列・要素ベクトルを保持する。

```fortran
real(kind=REAL8), save :: ea(3,3)
real(kind=REAL8), save :: eb(3)
```

`ecal1_1` が値を生成し、`elem1` がPETScへ加算する。

## 9. Dirichlet境界条件

境界条件は二段階で処理する。

### 9.1 境界自由度の収集

```fortran
call dcond1_1(..., bc_mark, bc_value)
call dcond1_2(..., bc_mark, bc_value)
```

各 `dcond` ルーチンは、対応する NSET に含まれる節点を調べ、

- `bc_mark(ieq)` に境界自由度であることを記録
- `bc_value(ieq)` に境界値を記録

する。

現在の例では、

- `dcond1_1`: 境界値 1.0
- `dcond1_2`: 境界値 0.0

を設定している。

### 9.2 PETSc行列への適用

境界自由度を PETSc の0始まり添字 `bc_rows` に変換し、境界値ベクトル `xbc` を作る。

```fortran
VecSetValues(xbc, ..., INSERT_VALUES, ...)
MatZeroRowsColumns(A, ..., 1.0, xbc, b, ...)
```

`MatZeroRowsColumns` により、Dirichlet自由度に対応する行と列を処理し、右辺 `b` も境界値に合わせて補正する。

## 10. KSPソルバー

デフォルト設定は次のとおり。

```text
KSP: CG
PC : ICC
相対許容誤差: 1.0e-12
```

```fortran
KSPSetType(ksp, KSPCG, ...)
PCSetType(pc, PCICC, ...)
KSPSetTolerances(...)
```

その後、

```fortran
KSPSetFromOptions(ksp, ...)
```

を呼ぶため、実行時オプションで上書きできる。

例:

```sh
./feelfem_petsc \
  -ksp_type cg \
  -pc_type gamg \
  -ksp_rtol 1e-12 \
  -ksp_monitor \
  -ksp_converged_reason
```

非対称問題では、たとえば次のような選択が可能である。

```sh
./feelfem_petsc -ksp_type gmres -pc_type ilu
./feelfem_petsc -ksp_type bcgs  -pc_type ilu
```

## 11. 解のFEM変数への書き戻し

`KSPSolve` 後、PETSc解ベクトルを一括して取得する。

```fortran
VecGetValues(sol, ..., sol_vals, ...)
```

その後、要素接続と `ipd` を使って生成FEM変数 `fem_u` へ書き戻す。

```fortran
fem_u(no) = sol_vals(ipd(no))
```

現在の例は1節点1自由度のP1スカラー要素を前提としている。

## 12. 入出力・共通基盤

| ファイル | 役割 |
|---|---|
| `numeric.f90` | `INT4`, `REAL8` など数値kindの定義 |
| `femDataType.f90` | EDAT、NSET、solve情報などの派生型 |
| `feelP2data.f90` | リンクリスト形式のEDAT/NSETから対象データを取得 |
| `fileread.f90` | `solv_dat`, `feel_dat` の読み込み |
| `report90.f90` | 経過時間と処理段階の表示 |
| `abortmodule.f90` | 異常終了処理 |
| `subsolveff90p2.f90` | 自由度番号、方程式数、節点接続数の計算 |

## 13. ビルド構造

`Makefile.petsc` は、通常の `.f90` と PETScプリプロセスを必要とする `.F90` を分けている。

```text
.f90
  数値型、入力、データモジュール、要素設定、境界条件など

.F90
  elem1.F90
  solve1.F90
  feelfem.F90
```

`.F90` ファイルでは、

```fortran
#include <petsc/finclude/petscksp.h>
```

および `PetscCallA` を使用する。

ビルド例:

```sh
make -f Makefile.petsc \
  PETSC_DIR=/path/to/petsc \
  PETSC_ARCH=arch-linux-c-opt
```

PETScがprefix installされ、`PETSC_ARCH` を使わない場合:

```sh
make -f Makefile.petsc PETSC_DIR=/path/to/petsc/install
```

## 14. 生成コードとPETSc固有コードの境界

このプロトタイプの重要な設計点は、有限要素法コード生成部分と線形代数バックエンドを分離していることである。

### feelfem由来で再利用される部分

- 入力データ構造
- EDAT / NSET の処理
- 自由度情報
- 数値積分点・重み
- 基底関数とその微分
- Jacobian変換
- 要素行列・要素ベクトルの生成
- Scheme の初期値代入

### PETSc ProgramModel固有部分

- `MatCreateSeqAIJ`
- `MatSetValues`
- `VecSetValues`
- `MatZeroRowsColumns`
- `KSPCreate`
- KSP / PC のデフォルト設定
- `KSPSetFromOptions`
- `KSPSolve`
- PETScオブジェクトの破棄

したがって、弱形式から要素積分コードを生成する中心アルゴリズムを維持したまま、ProgramModelによって線形代数バックエンドを交換する構造になっている。

## 15. 現在の制約

このプロトタイプには、少なくとも次の制約がある。

- `PETSC_COMM_SELF` による逐次実行
- P1三角形、3節点、1節点1自由度を前提とする箇所がある
- `solveNo = 1` に固定されている
- Dirichlet条件数が2、Neumann条件数が0に固定されている
- 非零要素数の事前確保は安全側の一様見積もり
- 解の書き戻しはP1スカラー問題に特化している
- デフォルトのCG + ICCは対称正定値問題向け

これらは、feelfemfj の ProgramModel と solve AST を整備した後、問題記述から自動生成されるべき情報である。

## 16. 今後の拡張候補

- MPI並列行列 `MatCreateAIJ` と領域分割への対応
- 要素接続からの厳密な行別非零数事前計算
- 複数FEM変数・複数自由度への一般化
- 複数 solve ブロックへの対応
- Neumann境界条件ルーチンの接続
- 非対称・不定値問題に応じたKSP/PCデフォルトの生成
- `edev1_1` を利用した一般的な解ベクトル書き戻し
- PETSc ProgramModelを feelfemfj のAST・コード生成器へ正式統合

## 17. ファイル一覧と分類

### プログラム制御

- `feelfem.F90`
- `solve1.F90`
- `elem1.F90`

### 要素・数値積分コード

- `ecal1.f90`
- `eset1_1_1.f90`
- `eset1_1_2.f90`
- `element_data1.f90`
- `quadrature_data1_1_1.f90`
- `quadrature_data1_1_2.f90`

### 境界条件・自由度

- `dcond1_1.f90`
- `dcond1_2.f90`
- `ipdinfo1_1.f90`
- `edev1_1.f90`
- `subsolveff90p2.f90`

### 入力・データ型・共通処理

- `fileread.f90`
- `femDataType.f90`
- `feelP2data.f90`
- `numeric.f90`
- `report90.f90`
- `abortmodule.f90`
- `zero.f90`

### Schemeから生成された代入

- `let1.f90`
- `let2.f90`

### ビルド・検証

- `Makefile.petsc`
- `README_PETSC.md`
- `compare_fem_u.py`

### 旧skyline関連

- `mat_skyline.f90`
- `skyline_scal.f90`
- `skyline_solver.f90`

旧skyline関連ファイルは比較・参照用にアーカイブへ含まれているが、`Makefile.petsc` のビルド対象には入っていない。
