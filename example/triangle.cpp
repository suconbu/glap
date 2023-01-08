//-----------------------------------------------
// example - triangle
// Display a rotated triangle in a window.
//
// Usage:
// F11    | Toggle fullscreen
// Escape | Quit example
//-----------------------------------------------

#include "glapp.hpp"

void frame(glapp::window& window);
void key(glapp::window& window, const std::string& key_name, const glapp::key_state& state, const glapp::modifier& modifier);

int main()
{
    auto app = glapp::get();

    const auto options = glapp::window_options()
                             .set_opengl_api(glapp::opengl_api::opengl)
                             .set_opengl_version(1, 0)
                             .set_opengl_profile(glapp::opengl_profile::core);

    // Create window and setup callbacks
    auto w = app->add_window(640, 640, "triangle", options);
    w->on_frame(frame);
    w->on_key(key);
    // w->set_swap_interval(1);

    // Start event loop
    return app->run();
}

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

void frame(glapp::window& window)
{
    // No need to call makecurrent and swapbuffer in this callback

    static auto last_time = std::chrono::high_resolution_clock::now();
    static int64_t last_frame = 0;
    static const GLfloat vtx[] = {
        -0.866f, -0.5f,
        0.0f, 1.0f,
        0.866f, -0.5f
    };
    static const GLfloat col[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    if (window.frame_count() == 0) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vtx);
        glColorPointer(3, GL_FLOAT, 0, col);
    }
    int32_t w = window.framebuffer_size().width();
    int32_t h = window.framebuffer_size().height();
    int32_t vx = (h < w) ? (w - h) / 2 : 0;
    int32_t vy = (w < h) ? (h - w) / 2 : 0;
    int32_t vw = (h < w) ? h : w;
    int32_t vh = (w < h) ? w : h;
    glViewport(vx, vy, vw, vh);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glRotatef(-0.1f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    double elapsed = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - last_time).count();
    if (0.5 <= elapsed || window.frame_count() == 0) {
        std::stringstream title;
        double fps = (window.frame_count() - last_frame) / elapsed;
        last_time = std::chrono::high_resolution_clock::now();
        last_frame = window.frame_count();

        // Show status on tile bar
        title << window.title_original() << " - ";
        title << window.framebuffer_size().width() << " x " << window.framebuffer_size().height() << " | ";
        title << std::fixed << std::setprecision(2) << fps << " fps | ";
        title << std::fixed << std::setprecision(2) << (1000.0 / fps) << " ms";
        window.set_title(title.str().c_str());
    }
}

void key(glapp::window& window, const std::string& key_name, const glapp::key_state& state, const glapp::modifier& modifier)
{
    (void)modifier;
    if (state.pressed()) {
        if (key_name == "f11") {
            // Toggle fullscreen
            if (window.state() == glapp::window_state::fullscreen) {
                window.restore();
            } else {
                window.fullscreen();
            }
        } else if (key_name == "escape") {
            window.close();
        } else {
        }
    }
}
