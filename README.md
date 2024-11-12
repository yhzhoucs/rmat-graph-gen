# RMat Graph Generator

一个用 Rust 实现的 RMat 图生成器。根目录 cpp_impl 下是它的 C++ 实现。

生成器写法参照 [sbeamer/gapbs](https://github.com/sbeamer/gapbs.git) 。

## 准备

为了运行 Rust 版本的生成器，你需要：
- Rust 编译工具，安装方法见 [官网](https://www.rust-lang.org/tools/install)

为了运行 C++ 版本的生成器，你需要：
- CMake 3.19 及以上
- C++17 支持 ( gcc11 及以上)

## 构建

### Rust 版本

只需要一行命令：

```sh
cargo build --release
```

### C++ 版本

1. 写 CMakeUserPresets.json 配置，这里提供了一个模板 `cpp_impl/CMakeUserPresets.json.example`

```sh
cd cpp_impl
cp CMakeUserPresets.json.example CMakeUserPresets.json
```

然后修改 CMakeUserPresets 让它适应你的系统。比如你可能需要**去掉** "CMAKE_BUILD_RPATH" 配置，如果你系统的 libstdc++ 版本足够高（即系统搜索目录下的 gcc 版本足够高）。或者，如果你使用的 gcc 不在系统环境变量 $PATH 中，你应该在 "base" 配置下指定 "CMAKE_CXX_COMPILER" 选项。具体可参加模板中的 $comment 项。

2. 构建 

```sh
# under cpp_impl
cmake --preset x64-release
cmake --build --preset release
```

## 用法

Rust 版本你可以用 cargo 运行或直接运行：

```sh
cargo run --release -- -h # 传入 -h 参数查看使用方法
# 或直接运行 ./target/release/rmat_graph_gen -h
```

C++ 版本直接运行：

```sh
# under cpp_impl
/out/build/x64-release/rmat_graph_gen -h
```

你可以按上面的使用 `-h` 参数查看命令使用方法。

两个版本的命令行参数基本相同：
- `a b c` 位置参数，代表 RMat 生成所需 a, b, c 的值，默认为 `0.57 0.19 0.19`
- `-s, --scale <SCALE>`: 图中点数量的指数，点数量为 `2^SCALE`，**此参数是唯一必须指定的参数**
- `-o, --outfile <OUTFILE>`: 输入文件名，默认为当前目录下的 `output.txt`
- `--par`: 是否开启多线程（ C++ 版本需要 OpenMP 支持），不指定时为单线程
- `-r, --randseed <RANDSEED>`: 生成图的种子，为一个无符号整数，给定的种子生成的图相同，不指定时采用随机种子
- `-d, --degree <DEGREE>`: 平均度数，默认为 16 ，边数量为 `DEGREE * 2^SCALE`

以下是几个生成示例：

Rust

```sh
# 生成参数为 0.48 0.11 0.21 ，点数量为 2^14 ，边数量为 2^17 的 RMat 图
# 输出到 output/r_14_17.txt 文件里（output 文件夹将自动创建），采用多线程
cargo run --release -- 0.48 0.11 0.21 -s 14 -o output/r_14_17.txt -d 8 --par

# 生成参数为 0.57 0.19 0.19 ，点数量为 2^12 ，边数量为 2^16 的 RMat 图
# 输出到 output.txt ，采用单线程
cargo run --release -s 12

# 生成参数为 0.57 0.19 0.19 ，点数量为 2^14 ，边数量为 2^18 的 RMat 图
# 输出到 output/r_14_18_s2233.txt 文件，采用固定种子 2233 ，生成使用多线程
cargo run --release -- -s 14 -o output/r_14_18_s2233.txt --randseed 2233 --par
```

C++

```sh
# 生成参数为 0.48 0.11 0.21 ，点数量为 2^14 ，边数量为 2^17 的 RMat 图
# 输出到 output/r_14_17.txt 文件里（output 文件夹将自动创建），采用多线程
./out/build/x64-release/rmat_graph_gen 0.48 0.11 0.21 -s 14 -o output/r_14_17.txt -d 8 --par

# 生成参数为 0.57 0.19 0.19 ，点数量为 2^12 ，边数量为 2^16 的 RMat 图
# 输出到 output.txt ，采用单线程
./out/build/x64-release/rmat_graph_gen -s 12

# 生成参数为 0.57 0.19 0.19 ，点数量为 2^14 ，边数量为 2^18 的 RMat 图
# 输出到 output/r_14_18_s2233.txt 文件，采用固定种子 2233 ，生成使用多线程
./out/build/x64-release/rmat_graph_gen -s 14 -o output/r_14_17_s2233.txt --randseed 2233 --par
```

## 输出格式

输出文件为边表格式：

```
<源点编号> <目的点编号>
```

编号均从 0 开始。

## License

This project is licensed under the MIT License.
