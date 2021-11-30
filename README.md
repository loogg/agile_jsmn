# Agile Jsmn

## 1、介绍

- `jsmn` 是一个超轻巧，携带方便，单文件，适用于单片机中存储空间有限的环境，简单的可以作为 ANSI-C 标准的 JSON 解析器。
- Agile Jsmn 在 jsmn 的基础上参照 cJSON 增加了一些 API 以实现简单的对象获取操作。

### 1.1、目录结构

| 名称 | 说明 |
| ---- | ---- |
| doc | 文档目录 |
| examples | 例程目录 |
| jsmn-1.0.0 | jsmn-1.0.0 源码目录 |
| util | Agile Jsmn 源码目录 |

### 1.2、许可证

Agile Jsmn 遵循 MIT 许可，详见 `LICENSE` 文件。

### 1.3、依赖

无依赖，使用标准C库开发，可直接在电脑上编译。

## 2、使用 Agile Jsmn

- Agile Jsmn 例子代码，该代码位于：[/examples/example_jsmn.c](examples/example_jsmn.c)。

- 可直接使用 `make` 编译，在电脑上运行。

## 3、Doxygen 文档生成

- 使用 `Doxywizard` 打开 [Doxyfile](./doc/doxygen/Doxyfile) 运行，生成的文件在 [doxygen/output](./doc/doxygen/output) 下。
- 需要更改 `Graphviz` 路径。
- `HTML` 生成未使用 `chm` 格式的，如果使能需要更改 `hhc.exe` 路径。

## 4、联系方式 & 感谢

- 维护：马龙伟
- 主页：<https://github.com/loogg/agile_jsmn>
- 邮箱：<2544047213@qq.com>
- jsmn 原作者主页：[https://github.com/zserge/jsmn](https://github.com/zserge/jsmn)
