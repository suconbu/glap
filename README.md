# glapp

This library for easily creating OpenGL application.

## Example

```cpp
#include "glapp.hpp"

int main()
{
    // Create window and setup callbacks
    auto w = glapp::add_window(400, 300, "example");
    w->on_frame([](glapp::window& window){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int32_t w = window.framebuffer_size().width();
        int32_t h = window.framebuffer_size().height();
        glViewport(0, 0, w, h);
        // ...
    });

    // Start event loop
    return glapp::run();
}
```

## Dependencies

* [GLFW](https://github.com/glfw/glfw) is backend library  
If it is not installed in the environment, it will be get the source code and build automatically.

## License

MIT License.
