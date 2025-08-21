# 🎯 BitSweeper - 现代化扫雷游戏

一个使用C语言和Raylib开发的现代化扫雷游戏，具有美观的用户界面和完整的游戏功能。

![扫雷游戏](https://img.shields.io/badge/Game-Minesweeper-blue) ![语言](https://img.shields.io/badge/Language-C-green) ![构建系统](https://img.shields.io/badge/Build-CMake-red) ![图形库](https://img.shields.io/badge/Graphics-Raylib-orange)

## ✨ 功能特性

- 🎮 **经典扫雷游戏玩法**：16x16网格，50个地雷
- 🎨 **现代化UI设计**：美观的圆角界面，悬停效果
- 🚩 **完整插旗系统**：右键插旗/取消插旗，实时计数
- ⏱️ **游戏计时器**：精确的时间记录和进度条显示
- 📊 **实时统计**：已揭示单元格数、插旗数、进度百分比
- 🎯 **游戏状态管理**：游戏中、暂停、胜利、失败状态
- ⌨️ **键盘快捷键**：R键重新开始，暂停/继续功能
- 🔧 **模块化架构**：核心逻辑与UI完全分离

## 🔧 系统要求

### Windows
- **编译器**: MinGW-w64 或 Visual Studio 2019+
- **CMake**: 3.15 或更高版本
- **Git**: 用于克隆子模块

### Linux/macOS
- **编译器**: GCC 或 Clang
- **CMake**: 3.15 或更高版本
- **系统库**: OpenGL, ALSA (Linux)

## 📦 依赖库

项目使用Git子模块管理依赖：
- **[Raylib](https://github.com/raysan5/raylib)** - 2D/3D图形库
- **[Unity](https://github.com/ThrowTheSwitch/Unity)** - C语言单元测试框架

## 🚀 快速开始

### 1. 克隆项目
```bash
git clone https://github.com/your-username/bit-sweeper.git
cd bit-sweeper
```

### 2. 初始化子模块
```bash
git submodule update --init --recursive
```

### 3. 创建构建目录
```bash
mkdir build
cd build
```

### 4. 配置项目
```bash
cmake ..
```

### 5. 编译项目
```bash
cmake --build .
```

### 6. 运行游戏
```bash
# Windows
ui\bitsweeper-game.exe

# Linux/macOS
./ui/bitsweeper-game
```

## 🎮 游戏操作

| 操作 | 说明 |
|------|------|
| **左键点击** | 揭示单元格 |
| **右键点击** | 插旗/取消插旗 |
| **R 键** | 重新开始游戏 |
| **New Game 按钮** | 开始新游戏 |
| **Pause 按钮** | 暂停/继续游戏 |
| **Exit 按钮** | 退出游戏 |

## 📁 项目结构

```
bit-sweeper/
├── CMakeLists.txt           # 根CMake配置
├── README.md               # 项目说明文档
├── .gitignore             # Git忽略规则
├── .gitmodules            # Git子模块配置
├── core/                  # 核心游戏逻辑
│   ├── src/
│   │   ├── bit-sweep/     # 主游戏逻辑
│   │   ├── cell/          # 单元格管理
│   │   └── utils/         # 工具函数
│   └── test/              # 单元测试
└── ui/                    # 用户界面
    ├── src/
    │   ├── widget/        # UI组件
    │   ├── theme.c        # 主题系统
    │   └── game-state.c   # 游戏状态管理
    └── lib/raylib/        # Raylib子模块
```

## 🏗️ 构建选项

### 调试构建
```bash
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

### 发布构建
```bash
mkdir build-release
cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

### 运行测试
```bash
# 在构建目录中
cmake --build . --target test
```

## 🔧 开发指南

### 添加新功能
1. 核心逻辑放在 `core/src/` 目录
2. UI相关代码放在 `ui/src/` 目录
3. 为新功能添加单元测试
4. 更新相应的CMakeLists.txt

### 代码风格
- 使用C99标准
- 函数名使用camelCase
- 类型名使用PascalCase
- 常量使用UPPER_CASE

## 🐛 故障排除

### Windows常见问题
- **找不到编译器**: 确保MinGW-w64或Visual Studio已正确安装
- **缺少DLL**: 确保系统PATH包含必要的库路径

### Linux常见问题
- **缺少依赖**: `sudo apt-get install build-essential cmake libgl1-mesa-dev`
- **权限问题**: 使用 `chmod +x` 给可执行文件添加权限

### macOS常见问题
- **Xcode工具**: 确保已安装Xcode命令行工具
- **Homebrew依赖**: `brew install cmake`

## 📝 版本历史

- **v1.0.0** - 初始版本
  - 基础扫雷游戏功能
  - 现代化UI界面
  - 插旗系统
  - 游戏统计

## 🤝 贡献指南

欢迎贡献代码！请遵循以下步骤：

1. Fork此项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建Pull Request

## 📄 许可证

本项目采用MIT许可证 - 详见 [LICENSE](LICENSE) 文件

## 🙏 致谢

- [Raylib](https://github.com/raysan5/raylib) - 优秀的C语言图形库
- [Unity](https://github.com/ThrowTheSwitch/Unity) - 简洁的C语言测试框架
- 所有贡献者和测试用户

## 📞 联系方式

- GitHub Issues: [提交问题](https://github.com/your-username/bit-sweeper/issues)
- 邮箱: your.email@example.com

---

⭐ 如果这个项目对你有帮助，请给它一个星标！
