# game_engine
Learn game engine

## submodules

```bash
git submodule add https://github.com/gabime/spdlog 3rdparty/spdlog
git submodule add -b 3.3-stable https://github.com/glfw/glfw 3rdparty/glfw
git submodule add -b docking https://github.com/ocornut/imgui 3rdparty/imgui
git submodule add https://github.com/g-truc/glm 3rdparty/glm
```

## Application

### 构造函数

- 获取自身实例
- 创建窗口实例
- 设置事件回调

### window（窗口）

- GetWindow

### OnEvent（事件回调）

- 调度事件
- 窗口关闭事件
- 遍历层事件

### layer（层）

- PushLayer
- PushOverlay


