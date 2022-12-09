# game_engine
Learn game engine

## submodules

```bash
git submodule add https://github.com/gabime/spdlog 3rdparty/spdlog
git submodule add -b 3.3-stable https://github.com/glfw/glfw 3rdparty/glfw
git submodule add -b docking https://github.com/ocornut/imgui 3rdparty/imgui
git submodule add https://github.com/g-truc/glm 3rdparty/glm
git submodule add https://github.com/nothings/stb 3rdparty/stb
```
## 日志


## Application

- 事件（OnEvent）
- 窗口（Windows）
    - 窗口事件，单独调度窗口事件
- 层栈（LayerStack）
- 主循环（Run）
    - 事件调度
    - 更新层

## Event（事件）

- 事件分类
    -Type
        -WindowClose，WindowResize，WindowFocus，WindowLostFocus，WindowMoved
        -AppTick，AppUpdate，AppRender
        -KeyPressed，KeyReleased，KeyTyped
        -MouseButtonPressed，MouseButtonReleased，MouseMoved，MouseScrolled
    -Category
        -Application
        -Input
        -Keyboard
        -Mouse
        -MouseButton

- 事件类（Event）
- 事件调度类（EventDispatcher）

## window（窗口）

- 窗口属性结构体（WindowsProp）
    - Title,Width,Height
- 窗口类（Window）
    - OnUpdate
    - SetEventCallback
    - IsVSync
    - GetNativeWindow（多平台支持）

## layerStack（层栈）

- PushLayer
- PushOverlay

## Layer(层)

- OnAttach（初始化层-分配资源）
- OnDetach（释放层-资源释放）
- OnUpdate（层更新，主要是事件分发）

## Input（输入）

- 单例

## Renderer 头文件层次关系

renderer->RenderCommand->renderAPI->vertexArray->buffer
renderer->shader
renderer->OrthographicCamera
texture

## Texture

- 纹理绑定（Bind）

### Texture2D

- 从文件创建纹理

### OpenGLTexture2D

- 创建纹理（构造函数）
    -加载图片文件使用`stb_image`
    -glCreateTextures
    -glTextureStorage2D
    -glTextureParameteri
    -glTextureSubImage2D
    -释放图片
- 绑定纹理（Bind）
    -glBindTextureUnit
- 释放纹理（析构函数）
    - glDeleteTextures



## OpenGL

### buffer 类型

- GL_ARRAY_BUFFER（顶点数组缓存，配合VertexArray使用）
- GL_ELEMENT_ARRAY_BUFFER（索引缓存，配合glDrawElements使用）
- GL_TEXTURE_BUFFER（纹理缓存，配合glTexBuffer使用）
- GL_UNIFORM_BUFFER（uniform缓存，传递复杂uniform）
- GL_TRANSFORM_FEEDBACK_BUFFER
- GL_PIXEL_PACK_BUFFER and GL_PIXEL_UNPACK_BUFFER（像素缓存）
- GL_COPY_READ_BUFFER and GL_COPY_WRITE_BUFFER（复制缓存）
- GL_DRAW_INDIRECT_BUFFER
