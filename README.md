# 俄罗斯方块 (Tetris)

一个基于 Windows 终端的经典俄罗斯方块游戏，使用 C++20 编写。

## 游戏截图

```
+--------------------+  +---------+
|                    |  | NEXT:   |
|   []               |  |  [][]   |
|   []               |  |    []   |
|   [][]             |  |         |
|                    |  |         |
|                    |  |         |
|                    |  | SCORE:  |
|                    |  |      0  |
|                    |  |         |
|                    |  | LINES:  |
|                    |  |      0  |
|                    |  |         |
+--------------------+  +---------+
```

## 功能特性

- 7 种标准俄罗斯方块（I, O, T, L, J, S, Z）
- 10×20 游戏面板
- 方块旋转与墙踢支持
- 行消除闪光动画
- 分数系统
- 下一个方块预览
- 开始菜单与游戏结束界面

## 操作说明

| 按键 | 功能 |
|------|------|
| ← → | 左右移动 |
| ↑ | 旋转 |
| ↓ | 软降（加速下落） |
| 空格 | 硬降（直接落底） |
| Enter | 开始 / 重新开始 |
| Esc | 退出 |

## 计分规则

| 消除行数 | 得分 |
|----------|------|
| 1 行 | 100 |
| 2 行 | 300 |
| 3 行 | 500 |
| 4 行 | 800 |

## 构建方法

### 环境要求

- Windows 10/11
- g++ (MSYS2) 或 Visual Studio

### 使用 g++ 编译（MSYS2）

```bash
g++ -std=c++20 -I include src/main.cpp src/Tetromino.cpp src/Board.cpp src/Game.cpp src/Console/ConsoleRenderer.cpp src/Console/ConsoleInput.cpp -o tetris.exe -luser32
```

### 使用 CMake 编译

```bash
cmake -B build -S .
cmake --build build --config Release
```

## 运行

```bash
./tetris.exe
```

## 项目结构

```
tetris/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── include/
│   └── tetris/
│       ├── Constants.h        # 常量定义
│       ├── Types.h            # 类型定义
│       ├── Tetromino.h        # 方块类
│       ├── Board.h            # 游戏面板
│       ├── Game.h             # 游戏主逻辑
│       ├── Renderer.h         # 渲染器接口
│       ├── InputHandler.h     # 输入处理接口
│       └── Console/
│           ├── ConsoleRenderer.h
│           └── ConsoleInput.h
└── src/
    ├── main.cpp
    ├── Tetromino.cpp
    ├── Board.cpp
    ├── Game.cpp
    └── Console/
        ├── ConsoleRenderer.cpp
        └── ConsoleInput.cpp
```

## 技术栈

- **语言**: C++20
- **构建**: CMake / g++
- **平台**: Windows Console API
- **设计模式**: RAII、智能指针、策略模式

## 许可证

MIT License
