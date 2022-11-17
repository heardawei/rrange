# 一个基于数字范围的生成器

Range: 基于范围`[a, b)`生成连续的迭代器, 每个迭代器指向一个数字

Range2D: 基于行`[a, b)`, 列`[c, d)`形成的二维数组生成连续的迭代器, 每个迭代器指向一对数字

RRange: 基于范围`[a, b)`生成连续的迭代器, 每个迭代器指向一个范围`[m, n)`, 其中`m, n∈[a, b)`

此外

* `unit`: 设置所有迭代器`[m, n)`的距离, 使之满足 `m + unit = n`, 当最后一个范围不足`unit`时，自适应。
* `overlap`: 设置相邻迭代器之间重叠的范围, 假设`[a, b)`与`[c, d)`是任意相邻的迭代器，重叠范围满足`b = c + overlap`

RRange2D: 基于行`[a, b)`, 列`[c, d)`形成的二维数组生成连续的迭代器, 每个迭代器指向一个Range2D范围, 若任一范围的行为`[m, n)`, 列为`[p, q)`, 则满足`m, n∈[a, b)`且`p, q∈[c, d)`

此外

* `unit_row`: 设置所有迭代器`[m, n)`的距离, 使之满足 `m + unit_row = n`, 当最后一行的迭代器的行数不足`unit_row`时，自适应。

* `unit_col`: 设置所有迭代器`[p, q)`的距离, 使之满足 `p + unit_col = q`, 当最后一列迭代器的列数不足`unit_col`时，自适应。

* `overlap_col`: 设置相邻迭代器之间重叠的范围, 使之满足: 在相同的`unit_row`这几行中, 任意两个左右相邻的迭代器, 它们重叠了`overlap_col`列。

* `overlap_row`: 设置相邻迭代器之间重叠的范围, 使之满足: 在相同的`unit_col`这几列中, 任意两个上下相邻的迭代器, 它们重叠了`overlap_row`行。
