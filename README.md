# agile_jsmn

## 1、介绍

- jsmn是一个超轻巧，携带方便，单文件，适用于单片机中存储空间有限的环境，简单的可以作为ANSI-C标准的JSON解析器。
- agile_jsmn在jsmn的基础上参照cJSON增加了一些API以实现简单的对象获取操作。
- 为了区分，将其命名为agile_jsmn。

### 1.1 目录结构

| 名称 | 说明 |
| ---- | ---- |
| examples | 例程目录 |
| inc  | jsmn头文件目录 |
| src  | jsmn源代码目录 |
| util | API文件目录   |

### 1.2 许可证

**agile_jsmn** 遵循 MIT 许可，详见 `LICENSE` 文件。

### 1.3 依赖

无依赖，使用标准C库开发，可直接在电脑上编译。

## 2、使用 agile_jsmn

- agile_jsmn例子代码，该代码位于：[/examples/example_jsmn.c](examples/example_jsmn.c)。

- 可直接使用`make`编译，在电脑上运行。

## 3、联系方式 & 感谢

- 维护：[loogg](https://github.com/loogg)
- jsmn原作者主页：[https://github.com/zserge/jsmn](https://github.com/zserge/jsmn)
