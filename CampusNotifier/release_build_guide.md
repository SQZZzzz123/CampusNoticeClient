# 如何获得 Release 模式编译的可执行文件

## 方法一：使用 Qt Creator IDE（推荐）

1. **打开项目**：
   - 启动 Qt Creator
   - 选择 "文件" > "打开文件或项目"
   - 浏览并选择 `CampusNotifier.pro` 文件

2. **选择构建配置**：
   - 在 Qt Creator 左下角，找到构建配置下拉菜单
   - 从 "Debug" 切换到 "Release"

3. **构建项目**：
   - 点击工具栏上的 "构建" 按钮（或按 Ctrl+B）
   - 等待构建完成

4. **找到可执行文件**：
   - 构建成功后，可执行文件位于项目目录下的 `release` 文件夹中
   - 文件名：`CampusNotifier.exe`

## 方法二：使用命令行

### 前提条件
- 已安装 Qt 并配置好环境变量
- 已安装适合 Qt 版本的编译器（如 MinGW 或 MSVC）

### 使用 MinGW 编译器

1. **打开命令提示符**：
   - 搜索并打开 "Qt 6.x.x (MinGW x.x.x) Command Prompt"

2. **导航到项目目录**：
   ```bash
   cd b:\QtCode\CampusNotifier
   ```

3. **清理旧构建（可选）**：
   ```bash
   rmdir /s /q release
   ```

4. **生成 Makefile**：
   ```bash
   qmake -config release
   ```

5. **编译项目**：
   ```bash
   mingw32-make
   ```

6. **找到可执行文件**：
   - 可执行文件位于 `release` 文件夹中：`release\CampusNotifier.exe`

### 使用 MSVC 编译器

1. **打开命令提示符**：
   - 搜索并打开 "x64 Native Tools Command Prompt for VS 20xx"

2. **导航到项目目录**：
   ```bash
   cd b:\QtCode\CampusNotifier
   ```

3. **清理旧构建（可选）**：
   ```bash
   rmdir /s /q release
   ```

4. **生成 Makefile**：
   ```bash
   qmake -config release
   ```

5. **编译项目**：
   ```bash
   nmake
   ```

6. **找到可执行文件**：
   - 可执行文件位于 `release` 文件夹中：`release\CampusNotifier.exe`

## 注意事项

1. **缺少依赖库**：
   - 直接运行生成的 `.exe` 文件可能会提示缺少 Qt 依赖库
   - 可以使用 Qt 自带的 `windeployqt` 工具来打包所有依赖：
     ```bash
     windeployqt release\CampusNotifier.exe
     ```

2. **构建目录**：
   - 默认情况下，Release 构建会在项目根目录下创建 `release` 文件夹
   - 也可以使用影子构建（Shadow Build），在单独的目录中构建

3. **编译器选择**：
   - 确保使用的编译器与 Qt 版本兼容
   - 可以在 Qt Creator 的 "工具" > "选项" > "Kits" 中查看和配置编译器

## 常见问题

- **qmake 命令找不到**：确保 Qt 已添加到系统环境变量
- **make/nmake 命令找不到**：确保已安装并配置好相应的编译器
- **构建失败**：检查项目中是否有语法错误或缺失的依赖

按照上述步骤操作，您应该能够成功生成 Release 模式的可执行文件。