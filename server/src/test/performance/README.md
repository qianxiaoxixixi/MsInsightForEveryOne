# 文件

## 1 落盘文件

执行性能测试，会在执行目录（正常情况下会在server/output/{arch}/bin/）下生成performance_test.csv，承载用例的执行情况和耗时信息。

## 2 落盘格式

| 序号 | 模块 | 用例名 | 执行结果 | 基线时间 | 实际耗时 |
| ---- |---- |---- |---- |---- |---- |
| 1 | Summary | testSummaryParser1P100MTime | pass | xx | xx |
| 2 | Timeline | testTraceParser1P100MTime | pass | xx | xx |
