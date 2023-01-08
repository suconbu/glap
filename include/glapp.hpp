#pragma once

#include "GLFW/glfw3.h"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace glapp {

namespace internal {

    class noncopyable {
    protected:
        noncopyable() = default;

    private:
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    };

    // Wrap raw pointer and custom destructor
    template <typename T>
    class handle_holder : noncopyable {
    private:
        T* handle_ = nullptr;
        using destruct_callback_t = std::function<void(T*)>;
        destruct_callback_t destruct_callback_ = nullptr;

    public:
        handle_holder() = default;
        handle_holder(T* handle, destruct_callback_t callback)
            : handle_(handle)
            , destruct_callback_(callback)
        {
        }
        ~handle_holder()
        {
            if (destruct_callback_ != nullptr) {
                destruct_callback_(handle_);
            }
        }
        operator bool() const { return handle_ != nullptr; }
        constexpr T* get() const noexcept { return handle_; }
    };

    constexpr const char* key_to_name(int key)
    {
        // clang-format off
        return
            (key == GLFW_KEY_SPACE) ? "space" :
            // (key == GLFW_KEY_APOSTROPHE) ? "'" :
            // (key == GLFW_KEY_COMMA) ? "," :
            // (key == GLFW_KEY_MINUS) ? "-" :
            // (key == GLFW_KEY_PERIOD) ? "." :
            // (key == GLFW_KEY_SLASH) ? "/" :
            (key == GLFW_KEY_0) ? "0" :
            (key == GLFW_KEY_1) ? "1" :
            (key == GLFW_KEY_2) ? "2" :
            (key == GLFW_KEY_3) ? "3" :
            (key == GLFW_KEY_4) ? "4" :
            (key == GLFW_KEY_5) ? "5" :
            (key == GLFW_KEY_6) ? "6" :
            (key == GLFW_KEY_7) ? "7" :
            (key == GLFW_KEY_8) ? "8" :
            (key == GLFW_KEY_9) ? "9" :
            // (key == GLFW_KEY_SEMICOLON) ? ";" :
            // (key == GLFW_KEY_EQUAL) ? "=" :
            (key == GLFW_KEY_A) ? "a" :
            (key == GLFW_KEY_B) ? "b" :
            (key == GLFW_KEY_C) ? "c" :
            (key == GLFW_KEY_D) ? "d" :
            (key == GLFW_KEY_E) ? "e" :
            (key == GLFW_KEY_F) ? "f" :
            (key == GLFW_KEY_G) ? "g" :
            (key == GLFW_KEY_H) ? "h" :
            (key == GLFW_KEY_I) ? "i" :
            (key == GLFW_KEY_J) ? "j" :
            (key == GLFW_KEY_K) ? "k" :
            (key == GLFW_KEY_L) ? "l" :
            (key == GLFW_KEY_M) ? "m" :
            (key == GLFW_KEY_N) ? "n" :
            (key == GLFW_KEY_O) ? "o" :
            (key == GLFW_KEY_P) ? "p" :
            (key == GLFW_KEY_Q) ? "q" :
            (key == GLFW_KEY_R) ? "r" :
            (key == GLFW_KEY_S) ? "s" :
            (key == GLFW_KEY_T) ? "t" :
            (key == GLFW_KEY_U) ? "u" :
            (key == GLFW_KEY_V) ? "v" :
            (key == GLFW_KEY_W) ? "w" :
            (key == GLFW_KEY_X) ? "x" :
            (key == GLFW_KEY_Y) ? "y" :
            (key == GLFW_KEY_Z) ? "z" :
            // (key == GLFW_KEY_LEFT_BRACKET) ? "(" :
            // (key == GLFW_KEY_BACKSLASH) ? "\\" :
            // (key == GLFW_KEY_RIGHT_BRACKET) ? ")" :
            // (key == GLFW_KEY_GRAVE_ACCENT) ? "`" :
            // (key == GLFW_KEY_WORLD_1) ? "" :
            // (key == GLFW_KEY_WORLD_2) ? "" :
            (key == GLFW_KEY_ESCAPE) ? "escape" :
            (key == GLFW_KEY_ENTER) ? "enter" :
            (key == GLFW_KEY_TAB) ? "tab" :
            (key == GLFW_KEY_BACKSPACE) ? "backspace" :
            (key == GLFW_KEY_INSERT) ? "insert" :
            (key == GLFW_KEY_DELETE) ? "delete" :
            (key == GLFW_KEY_RIGHT) ? "right" :
            (key == GLFW_KEY_LEFT) ? "left" :
            (key == GLFW_KEY_DOWN) ? "down" :
            (key == GLFW_KEY_UP) ? "up" :
            (key == GLFW_KEY_PAGE_UP) ? "pageup" :
            (key == GLFW_KEY_PAGE_DOWN) ? "pagedown" :
            (key == GLFW_KEY_HOME) ? "home" :
            (key == GLFW_KEY_END) ? "end" :
            (key == GLFW_KEY_CAPS_LOCK) ? "capslock" :
            (key == GLFW_KEY_SCROLL_LOCK) ? "scrolllock" :
            (key == GLFW_KEY_NUM_LOCK) ? "numlock" :
            (key == GLFW_KEY_PRINT_SCREEN) ? "printscreen" :
            (key == GLFW_KEY_PAUSE) ? "pause" :
            (key == GLFW_KEY_F1) ? "f1" :
            (key == GLFW_KEY_F2) ? "f2" :
            (key == GLFW_KEY_F3) ? "f3" :
            (key == GLFW_KEY_F4) ? "f4" :
            (key == GLFW_KEY_F5) ? "f5" :
            (key == GLFW_KEY_F6) ? "f6" :
            (key == GLFW_KEY_F7) ? "f7" :
            (key == GLFW_KEY_F8) ? "f8" :
            (key == GLFW_KEY_F9) ? "f9" :
            (key == GLFW_KEY_F10) ? "f10" :
            (key == GLFW_KEY_F11) ? "f11" :
            (key == GLFW_KEY_F12) ? "f12" :
            (key == GLFW_KEY_F13) ? "f13" :
            (key == GLFW_KEY_F14) ? "f14" :
            (key == GLFW_KEY_F15) ? "f15" :
            (key == GLFW_KEY_F16) ? "f16" :
            (key == GLFW_KEY_F17) ? "f17" :
            (key == GLFW_KEY_F18) ? "f18" :
            (key == GLFW_KEY_F19) ? "f19" :
            (key == GLFW_KEY_F20) ? "f20" :
            (key == GLFW_KEY_F21) ? "f21" :
            (key == GLFW_KEY_F22) ? "f22" :
            (key == GLFW_KEY_F23) ? "f23" :
            (key == GLFW_KEY_F24) ? "f24" :
            (key == GLFW_KEY_F25) ? "f25" :
            (key == GLFW_KEY_KP_0) ? "num0" :
            (key == GLFW_KEY_KP_1) ? "num1" :
            (key == GLFW_KEY_KP_2) ? "num2" :
            (key == GLFW_KEY_KP_3) ? "num3" :
            (key == GLFW_KEY_KP_4) ? "num4" :
            (key == GLFW_KEY_KP_5) ? "num5" :
            (key == GLFW_KEY_KP_6) ? "num6" :
            (key == GLFW_KEY_KP_7) ? "num7" :
            (key == GLFW_KEY_KP_8) ? "num8" :
            (key == GLFW_KEY_KP_9) ? "num9" :
            // (key == GLFW_KEY_KP_DECIMAL) ? "decimal" :
            // (key == GLFW_KEY_KP_DIVIDE) ? "/" :
            // (key == GLFW_KEY_KP_MULTIPLY) ? "*" :
            // (key == GLFW_KEY_KP_SUBTRACT) ? "-" :
            // (key == GLFW_KEY_KP_ADD) ? "+" :
            (key == GLFW_KEY_KP_ENTER) ? "enter" :
            // (key == GLFW_KEY_KP_EQUAL) ? "=" :
            (key == GLFW_KEY_LEFT_SHIFT) ? "lshift" :
            (key == GLFW_KEY_LEFT_CONTROL) ? "lcontrol" :
            (key == GLFW_KEY_LEFT_ALT) ? "lalt" :
            (key == GLFW_KEY_LEFT_SUPER) ? "lsuper" :
            (key == GLFW_KEY_RIGHT_SHIFT) ? "rshift" :
            (key == GLFW_KEY_RIGHT_CONTROL) ? "rcontrol" :
            (key == GLFW_KEY_RIGHT_ALT) ? "ralt" :
            (key == GLFW_KEY_RIGHT_SUPER) ? "rsuper" :
            (key == GLFW_KEY_MENU) ? "menu" :
            "";
        // clang-format on
    }

    constexpr const char* mouse_button_to_name(int button)
    {
        // clang-format off
        return
            (button == GLFW_MOUSE_BUTTON_LEFT) ? "left" :
            (button == GLFW_MOUSE_BUTTON_RIGHT) ? "right" :
            (button == GLFW_MOUSE_BUTTON_MIDDLE) ? "middle" :
            "";
        // clang-format on
    }

    constexpr const char* or_empty(const char* str)
    {
        return (str != nullptr) ? str : "";
    }

    using glfw_window_handle = internal::handle_holder<GLFWwindow>;
    using glfw_monitor_handle = internal::handle_holder<GLFWmonitor>;
} // namespace internal

enum class cursor_mode : int32_t {
    // Cursor motion is not limited
    normal = GLFW_CURSOR_NORMAL,

    // Cursor to become hidden when it is over a window but still want it to behave normally
    hidden = GLFW_CURSOR_HIDDEN,

    // Hide the cursor and lock it to the specified window
    disabled = GLFW_CURSOR_DISABLED
};

template <typename T>
class point {
private:
    bool valid_ {};
    T x_ {};
    T y_ {};

public:
    point() = default;
    point(T width, T height)
        : valid_(true)
        , x_(width)
        , y_(height)
    {
    }
    operator bool() const { return valid_; }
    T x() const { return x_; }
    T y() const { return y_; }
};

template <typename T>
class size {
private:
    bool valid_ {};
    T width_ {};
    T height_ {};

public:
    size() = default;
    size(T width, T height)
        : valid_(true)
        , width_(width)
        , height_(height)
    {
    }
    operator bool() const { return valid_; }
    T width() const { return width_; }
    T height() const { return height_; }
};

template <typename T>
class rect {
private:
    bool valid_ {};
    T x_ {};
    T y_ {};
    T width_ {};
    T height_ {};

public:
    rect() = default;
    rect(T x, T y, T width, T height)
        : valid_(true)
        , x_(x)
        , y_(y)
        , width_(width)
        , height_(height)
    {
    }
    rect(const glapp::point<T>& pos, const glapp::size<T>& size)
        : valid_(true)
        , x_(pos.x())
        , y_(pos.y())
        , width_(size.width())
        , height_(size.height())
    {
    }
    operator bool() const { return valid_; }
    T x() const { return x_; }
    T y() const { return y_; }
    T width() const { return width_; }
    T height() const { return height_; }
    T left() const { return x_; }
    T right() const { return x_ + width_; }
    T top() const { return y_; }
    T bottom() const { return y_ + height_; }
    glapp::point<T> pos() const { return { x_, y_ }; }
    glapp::size<T> size() const { return { width_, height_ }; }
};

class key_state {
    friend class window;

private:
    bool pressed_ = false;
    bool repeated_ = false;

public:
    bool pressed() const { return pressed_; }
    bool repeated() const { return repeated_; }

private:
    key_state(int state)
        : pressed_(state == GLFW_PRESS || state == GLFW_REPEAT)
        , repeated_(state == GLFW_REPEAT)
    {
    }
};

class button_state {
    friend class window;

private:
    bool pressed_ = false;

public:
    bool pressed() const { return pressed_; }

private:
    button_state(int state)
        : pressed_(state == GLFW_PRESS)
    {
    }
};

class modifier {
    friend class window;

private:
    bool shift_ = false;
    bool control_ = false;
    bool alt_ = false;
    bool super_ = false;
    bool caps_lock_ = false;
    bool num_lock_ = false;

public:
    bool shift() const { return shift_; }
    bool control() const { return control_; }
    bool alt() const { return alt_; }
    bool super() const { return super_; }
    bool caps_lock() const { return caps_lock_; }
    bool num_lock() const { return num_lock_; }

private:
    modifier(int mods)
        : shift_((mods & GLFW_MOD_SHIFT) != 0)
        , control_((mods & GLFW_MOD_CONTROL) != 0)
        , alt_((mods & GLFW_MOD_ALT) != 0)
        , super_((mods & GLFW_MOD_SUPER) != 0)
        , caps_lock_((mods & GLFW_MOD_CAPS_LOCK) != 0)
        , num_lock_((mods & GLFW_MOD_NUM_LOCK) != 0)
    {
    }
};

enum class opengl_api : int32_t {
    opengl = GLFW_OPENGL_API,
    opengl_es = GLFW_OPENGL_ES_API,
    no = GLFW_NO_API
};

enum class opengl_profile : int32_t {
    any = GLFW_OPENGL_ANY_PROFILE,
    compat = GLFW_OPENGL_COMPAT_PROFILE,
    core = GLFW_OPENGL_CORE_PROFILE
};

class window_options {
    friend class window;

private:
    int32_t opengl_version_major_ = 1;
    int32_t opengl_version_minor_ = 0;
    glapp::opengl_api opengl_api_ = glapp::opengl_api::opengl;
    glapp::opengl_profile opengl_profile_ = glapp::opengl_profile::any;
    int32_t framebuffer_red_bits_ = 8;
    int32_t framebuffer_green_bits_ = 8;
    int32_t framebuffer_blue_bits_ = 8;
    int32_t framebuffer_alpha_bits_ = 8;
    int32_t framebuffer_depth_bits_ = 24;
    int32_t framebuffer_stencil_bits_ = 8;
    int32_t msaa_samples_ = 0;
    int32_t refresh_rate_ = 60;
    bool doublebuffer_ = true;
    bool resizable_ = true;
    bool visible_on_created_ = true;
    bool maximize_on_created_ = false;
    bool topmost_on_created_ = false;
    bool auto_minimize = true;
    bool content_scale_to_monitor_ = false;

public:
    glapp::window_options& set_opengl_version(int32_t major, int32_t minor)
    {
        opengl_version_major_ = major;
        opengl_version_minor_ = minor;
        return *this;
    }
    glapp::window_options& set_opengl_api(glapp::opengl_api api)
    {
        opengl_api_ = api;
        return *this;
    }
    glapp::window_options& set_opengl_profile(glapp::opengl_profile profile)
    {
        opengl_profile_ = profile;
        return *this;
    }
    glapp::window_options& set_framebuffer_red_bits(int32_t bits)
    {
        framebuffer_red_bits_ = bits;
        return *this;
    }
    glapp::window_options& set_framebuffer_green_bits(int32_t bits)
    {
        framebuffer_green_bits_ = bits;
        return *this;
    }
    glapp::window_options& set_framebuffer_blue_bits(int32_t bits)
    {
        framebuffer_blue_bits_ = bits;
        return *this;
    }
    glapp::window_options& set_framebuffer_alpha_bits(int32_t bits)
    {
        framebuffer_alpha_bits_ = bits;
        return *this;
    }
    glapp::window_options& set_framebuffer_depth_bits(int32_t bits)
    {
        framebuffer_depth_bits_ = bits;
        return *this;
    }
    glapp::window_options& set_framebuffer_stencil_bits(int32_t bits)
    {
        framebuffer_stencil_bits_ = bits;
        return *this;
    }
    glapp::window_options& set_msaa_samples(int32_t samples)
    {
        msaa_samples_ = samples;
        return *this;
    }
    glapp::window_options& set_refresh_rate(int32_t refresh_rate)
    {
        refresh_rate_ = refresh_rate;
        return *this;
    }
    glapp::window_options& set_doublebuffer(bool double_buffer)
    {
        doublebuffer_ = double_buffer;
        return *this;
    }
    glapp::window_options& set_resizable(bool resizable)
    {
        resizable_ = resizable;
        return *this;
    }
    glapp::window_options& set_visible_on_created(bool visible)
    {
        visible_on_created_ = visible;
        return *this;
    }
    glapp::window_options& set_maximize_on_created(bool enable)
    {
        maximize_on_created_ = enable;
        return *this;
    }
    glapp::window_options& set_topmost_on_created(bool enable)
    {
        topmost_on_created_ = enable;
        return *this;
    }
    // Specifies whether to minimize when losing focus in full screen
    glapp::window_options& set_auto_minimize(bool enable)
    {
        auto_minimize = enable;
        return *this;
    }
    glapp::window_options& set_content_scale_to_monitor(bool enable)
    {
        content_scale_to_monitor_ = enable;
        return *this;
    }

private:
    void apply() const
    {
        glfwWindowHint(GLFW_CLIENT_API, static_cast<int32_t>(opengl_api_));
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int32_t>(opengl_version_major_));
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int32_t>(opengl_version_minor_));
        glfwWindowHint(GLFW_RED_BITS, framebuffer_red_bits_);
        glfwWindowHint(GLFW_GREEN_BITS, framebuffer_green_bits_);
        glfwWindowHint(GLFW_BLUE_BITS, framebuffer_blue_bits_);
        glfwWindowHint(GLFW_ALPHA_BITS, framebuffer_alpha_bits_);
        glfwWindowHint(GLFW_DEPTH_BITS, framebuffer_depth_bits_);
        glfwWindowHint(GLFW_STENCIL_BITS, framebuffer_stencil_bits_);
        glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate_);
        glfwWindowHint(GLFW_SAMPLES, msaa_samples_);
        glfwWindowHint(GLFW_DOUBLEBUFFER, doublebuffer_);
        glfwWindowHint(GLFW_RESIZABLE, resizable_ ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, visible_on_created_ ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_MAXIMIZED, maximize_on_created_ ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, topmost_on_created_ ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_AUTO_ICONIFY, auto_minimize ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, content_scale_to_monitor_ ? GLFW_TRUE : GLFW_FALSE);
    }
};

class monitor_mode {
    friend class monitor;

private:
    bool valid_ {};
    int32_t width_ {};
    int32_t height_ {};
    int32_t red_bits_ {};
    int32_t green_bits_ {};
    int32_t blue_bits_ {};
    int32_t refresh_rate_ {};

public:
    monitor_mode() = default;

    operator bool() const { return valid_; }

    int32_t width() const { return width_; }
    int32_t height() const { return height_; }
    int32_t red_bits() const { return red_bits_; }
    int32_t green_bits() const { return green_bits_; }
    int32_t blue_bits() const { return blue_bits_; }
    int32_t refresh_rate() const { return refresh_rate_; }

private:
    monitor_mode(const GLFWvidmode& mode)
        : valid_(true)
        , width_(mode.width)
        , height_(mode.height)
        , red_bits_(mode.redBits)
        , green_bits_(mode.greenBits)
        , blue_bits_(mode.blueBits)
        , refresh_rate_(mode.refreshRate)
    {
    }
};

class monitor {
    friend class window;
    friend class app;

private:
    std::shared_ptr<internal::glfw_monitor_handle> handle_;

public:
    monitor() = default;

    static const monitor& empty_instance()
    {
        static const monitor monitor {};
        return monitor;
    }

    operator bool() const { return handle_ && *handle_; }

    GLFWmonitor* glfw_handle() const { return handle_ ? handle_->get() : nullptr; }

    std::string to_string() const
    {
        // {name} - position:{x},{y} size:{w}x{h}px(workarea:{w}x{h}px) {pw}x{ph}mm {rate}Hz
        std::stringstream ss;
        ss << name()
           << " - position:" << rect().x() << "," << rect().y()
           << " size:" << rect().width() << "x" << rect().height() << "px"
#if (3 <= GLFW_VERSION_MINOR)
           << "(workarea:" << workarea_rect().width() << "x" << workarea_rect().height() << "px)"
#endif
           << " " << pysical_size().width() << "x" << pysical_size().height() << "mm"
           << " " << refresh_rate() << "Hz";
        return ss.str();
    }

    std::string name() const
    {
        std::string name {};
        if (handle_) {
            name = glfwGetMonitorName(handle_->get());
        }
        return name;
    }

    // Returns the current screen size in screen coordinates
    glapp::rect<int32_t> rect() const
    {
        glapp::rect<int32_t> rect {};
        if (handle_) {
            int xpos {};
            int ypos {};
            glfwGetMonitorPos(handle_->get(), &xpos, &ypos);
            const auto mode = current_mode();
            rect = { xpos, ypos, mode.width(), mode.height() };
        }
        return rect;
    }

#if (3 <= GLFW_VERSION_MINOR)
    // Returns the current screen rect (not including the taskbar) in screen coordinates
    glapp::rect<int32_t> workarea_rect() const
    {
        glapp::rect<int32_t> rect {};
        if (handle_) {
            int xpos {};
            int ypos {};
            int width {};
            int height {};
            glfwGetMonitorWorkarea(handle_->get(), &xpos, &ypos, &width, &height);
            rect = { xpos, ypos, width, height };
        }
        return rect;
    }
#endif

    // Returns the current refresh rate in Hz
    int32_t refresh_rate() const
    {
        int32_t refresh_rate {};
        const auto mode = current_mode();
        if (mode) {
            refresh_rate = mode.refresh_rate();
        }
        return refresh_rate;
    }

    // Returns the monitor size in millimetres (Aspect ratio is reflected)
    glapp::size<int32_t> pysical_size() const
    {
        glapp::size<int32_t> size {};
        if (handle_) {
            int width {};
            int height {};
            glfwGetMonitorPhysicalSize(handle_->get(), &width, &height);
            size = { width, height };
        }
        return size;
    }

    glapp::size<float> content_scale() const
    {
        glapp::size<float> scale {};
        if (handle_) {
            float xscale {};
            float yscale {};
            glfwGetMonitorContentScale(handle_->get(), &xscale, &yscale);
            scale = { xscale, yscale };
        }
        return scale;
    }

    glapp::monitor_mode current_mode() const
    {
        glapp::monitor_mode mode {};
        if (handle_) {
            auto glfw_mode = glfwGetVideoMode(handle_->get());
            if (glfw_mode != nullptr) {
                mode = glapp::monitor_mode(*glfw_mode);
            }
        }
        return mode;
    }

    std::vector<glapp::monitor_mode> supported_modes() const
    {
        std::vector<glapp::monitor_mode> modes {};
        if (handle_) {
            int count {};
            auto glfw_modes = glfwGetVideoModes(handle_->get(), &count);
            if (glfw_modes != nullptr) {
                modes.reserve(count);
                for (int i = 0; i < count; ++i) {
                    modes.push_back(glapp::monitor_mode(glfw_modes[i]));
                }
            }
        }
        return modes;
    }

    void set_user_pointer(void* pointer)
    {
        if (handle_) {
            glfwSetMonitorUserPointer(handle_->get(), pointer);
        }
    }
    void* user_pointer() const
    {
        void* pointer {};
        if (handle_) {
            pointer = glfwGetMonitorUserPointer(handle_->get());
        }
        return pointer;
    }

private:
    monitor(GLFWmonitor* glfw_monitor)
        : handle_(std::make_shared<internal::glfw_monitor_handle>(glfw_monitor, nullptr))
    {
    }

    void destroy()
    {
        if (handle_) {
            handle_.reset();
        }
    }
};

enum class window_state {
    normal,
    minimized,
    maximized,
    fullscreen
};

class window : internal::noncopyable {
    friend class app;

private:
    std::shared_ptr<internal::glfw_window_handle> handle_;
    std::string title_;
    std::string tag_;
    int32_t swap_interval_ = 0;
    int32_t last_swap_interval_ = INT32_MAX;
    void* user_pointer_ {};
    int64_t frame_count_ = 0;
    glapp::rect<int32_t> normal_window_rect_;
    glapp::rect<int32_t> fullscreen_backup_window_rect_;
    glapp::size<int32_t> size_limit_min_;
    glapp::size<int32_t> size_limit_max_;
    glapp::size<int32_t> aspect_ratio_;

    template <typename... Args>
    class event {
    private:
        std::function<void(Args...)> callback_;

    public:
        template <typename T>
        void set_callback(T&& callback)
        {
            callback_ = std::forward<T>(callback);
        }

        void operator()(Args... args)
        {
            if (callback_) {
                callback_(args...);
            }
        }
    };

    event<window&> frame_event;
    event<window&, const std::string&, const glapp::key_state&, const glapp::modifier&> key_event;
    event<window&, const std::string&, const glapp::button_state&, const glapp::modifier&> mouse_button_event;
    event<window&, double, double> cursor_pos_event;
    event<window&, bool> cursor_enter_event;
    event<window&, double, double> scroll_event;
    event<window&, int32_t, int32_t> window_pos_event;
    event<window&, int32_t, int32_t> window_size_event;
    event<window&> window_close_event;
    event<window&> window_refresh_event;
    event<window&, bool> window_focus_event;
    event<window&, glapp::window_state> window_state_event;
    event<window&, float, float> window_contentscale_event;
    event<window&, int32_t, int32_t> framebuffer_size_event;
    event<window&, const std::vector<std::string>&> drop_event;

    // Folloing member is valid only for reference instance
    std::weak_ptr<window> weakref_;

public:
    window() = delete;

    ~window()
    {
        if (handle_) {
            handle_.reset();
        }
    }

    operator bool() const { return handle_ && *handle_; }

    GLFWwindow* glfw_handle() const { return handle_ ? handle_->get() : nullptr; }

    int64_t frame_count() const { return frame_count_; }

    void set_resizable(bool resizable)
    {
        if (handle_) {
            glfwSetWindowAttrib(handle_->get(), GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
        }
    }

    bool resizable() const
    {
        bool resizable = false;
        if (handle_) {
            resizable = glfwGetWindowAttrib(handle_->get(), GLFW_RESIZABLE) == GLFW_TRUE;
        }
        return resizable;
    }

    void set_topmost(bool topmost)
    {
        if (handle_) {
            glfwSetWindowAttrib(handle_->get(), GLFW_FLOATING, topmost ? GLFW_TRUE : GLFW_FALSE);
        }
    }

    bool topmost() const
    {
        bool topmost = false;
        if (handle_) {
            topmost = glfwGetWindowAttrib(handle_->get(), GLFW_FLOATING) == GLFW_TRUE;
        }
        return topmost;
    }

    void set_pos(const glapp::point<int32_t>& pos)
    {
        if (handle_) {
            glfwSetWindowPos(handle_->get(), pos.x(), pos.y());
        }
    }

    void set_pos(int32_t xpos, int32_t ypos)
    {
        if (handle_) {
            glfwSetWindowPos(handle_->get(), xpos, ypos);
        }
    }

    glapp::point<int32_t> pos() const
    {
        int32_t xpos = 0;
        int32_t ypos = 0;
        if (handle_) {
            glfwGetWindowPos(handle_->get(), &xpos, &ypos);
        }
        return { xpos, ypos };
    }

    void set_size(const glapp::size<int32_t>& size)
    {
        set_size_internal(size.width(), size.height());
    }

    void set_size(int32_t width, int32_t height)
    {
        set_size_internal(width, height);
    }

    glapp::size<int32_t> size() const
    {
        int32_t width = 0;
        int32_t height = 0;
        if (handle_) {
            glfwGetWindowSize(handle_->get(), &width, &height);
        }
        return { width, height };
    }

    void set_size_limit_min(int32_t min_width, int32_t min_height)
    {
        if (handle_) {
            size_limit_min_ = glapp::size<int32_t>(min_width, min_height);
            set_size_limit_internal();
        }
    }

    // Reset minimum size limit
    void set_size_limit_min()
    {
        if (handle_) {
            size_limit_min_ = {};
            set_size_limit_internal();
        }
    }

    void set_size_limit_max(int32_t max_width, int32_t max_height)
    {
        if (handle_) {
            size_limit_max_ = glapp::size<int32_t>(max_width, max_height);
            set_size_limit_internal();
        }
    }

    // Reset maximum size limit
    void set_size_limit_max()
    {
        if (handle_) {
            size_limit_max_ = {};
            set_size_limit_internal();
        }
    }

    void set_aspect_ratio(int32_t numerator, int32_t denominator)
    {
        if (handle_) {
            aspect_ratio_ = glapp::size<int32_t>(numerator, denominator);
            glfwSetWindowAspectRatio(handle_->get(), numerator, denominator);
        }
    }

    // Reset aspect ratio constraints
    void set_aspect_ratio()
    {
        if (handle_) {
            aspect_ratio_ = {};
            glfwSetWindowAspectRatio(handle_->get(), GLFW_DONT_CARE, GLFW_DONT_CARE);
        }
    }

    glapp::size<int32_t> framebuffer_size() const
    {
        int32_t framebuffer_width = 0;
        int32_t framebuffer_height = 0;
        if (handle_) {
            glfwGetFramebufferSize(handle_->get(), &framebuffer_width, &framebuffer_height);
        }
        return { framebuffer_width, framebuffer_height };
    }

    glapp::point<float> contentscale() const
    {
        float xscale = 0.0f;
        float yscale = 0.0f;
        if (handle_) {
            glfwGetWindowContentScale(handle_->get(), &xscale, &yscale);
        }
        return { xscale, yscale };
    }

    void set_opacity(float opacity) const
    {
        if (handle_) {
            glfwSetWindowOpacity(handle_->get(), opacity);
        }
    }

    float opacity() const
    {
        float opacity = 0.0f;
        if (handle_) {
            opacity = glfwGetWindowOpacity(handle_->get());
        }
        return opacity;
    }

    // Minimize the window
    // If the window is already minimized, this function does nothing
    void minimize()
    {
        if (handle_) {
            glfwIconifyWindow(handle_->get());
        }
    }

    // Maximize the window
    // If the window is already Maximize this function does nothing
    void maximize()
    {
        if (handle_) {
            if (glfwGetWindowMonitor(handle_->get()) != nullptr) {
                if (state_internal() == glapp::window_state::minimized) {
                    // Allows back to normal size when restoring from maximized
                    glfwRestoreWindow(handle_->get());
                }
                glfwSetWindowMonitor(handle_->get(), NULL, fullscreen_backup_window_rect_.x(), fullscreen_backup_window_rect_.y(), fullscreen_backup_window_rect_.width(), fullscreen_backup_window_rect_.height(), 0);
            }
            glfwMaximizeWindow(handle_->get());
        }
    }

    // Make the window full screen on the monitor where the window is currently located
    // It is equivalent to `fullscreen(winodw.placed_window())`
    void fullscreen()
    {
        fullscreen_internal(placed_monitor_internal());
    }

    // Make the window full screen on the specified monitor
    void fullscreen(const std::shared_ptr<glapp::monitor> monitor)
    {
        fullscreen_internal(monitor);
    }

    // Restore the window
    void restore()
    {
        if (handle_) {
            if (glfwGetWindowMonitor(handle_->get()) != nullptr) {
                if (state_internal() == glapp::window_state::minimized) {
                    // Return to fullscreen once by this call,
                    // to avoid restoring to maximum size
                    glfwRestoreWindow(handle_->get());
                }
                glfwSetWindowMonitor(handle_->get(), NULL, normal_window_rect_.x(), normal_window_rect_.y(), normal_window_rect_.width(), normal_window_rect_.height(), 0);
            }
            glfwRestoreWindow(handle_->get());
        }
    }

    glapp::window_state state() const
    {
        return state_internal();
    }

    // Returns the monitor on which the window is currently placed
    std::shared_ptr<glapp::monitor> placed_monitor() const
    {
        return placed_monitor_internal();
    }

    void set_visible(bool visible) const
    {
        if (handle_) {
            if (visible) {
                glfwShowWindow(handle_->get());
            } else {
                glfwHideWindow(handle_->get());
            }
        }
    }

    bool visible() const
    {
        bool visible = false;
        if (handle_) {
            visible = glfwGetWindowAttrib(handle_->get(), GLFW_VISIBLE) == GLFW_TRUE;
        }
        return visible;
    }

    void focus() const
    {
        if (handle_) {
            glfwFocusWindow(handle_->get());
        }
    }

    bool is_focused() const
    {
        bool focused = false;
        if (handle_) {
            focused = glfwGetWindowAttrib(handle_->get(), GLFW_FOCUSED) == GLFW_TRUE;
        }
        return focused;
    }

    void flash() const
    {
        if (handle_) {
            glfwRequestWindowAttention(handle_->get());
        }
    }

    void set_title(const char* title)
    {
        if (handle_) {
            title_ = title;
            glfwSetWindowTitle(handle_->get(), title);
        }
    }
    const std::string& title() const { return title_; }

    void set_swap_interval(int32_t interval)
    {
        swap_interval_ = interval;
    }
    int32_t swap_interval() { return swap_interval_; }

    void set_user_pointer(void* pointer) { user_pointer_ = pointer; }
    void* user_pointer() const { return user_pointer_; }

    void set_tag(const char* tag) { tag_ = tag; }
    const std::string& tag() const { return tag_; }

    void set_cursor_mode(glapp::cursor_mode mode) const
    {
        if (handle_) {
            int glfw_mode = (mode == glapp::cursor_mode::normal) ? GLFW_CURSOR_NORMAL :
                (mode == glapp::cursor_mode::hidden)             ? GLFW_CURSOR_HIDDEN :
                (mode == glapp::cursor_mode::disabled)           ? GLFW_CURSOR_DISABLED :
                                                                   GLFW_CURSOR_NORMAL;
            glfwSetInputMode(handle_->get(), GLFW_CURSOR, glfw_mode);
        }
    }

    glapp::cursor_mode cursor_mode() const
    {
        glapp::cursor_mode mode = glapp::cursor_mode::normal;
        if (handle_) {
            const int glfw_mode = glfwGetInputMode(handle_->get(), GLFW_CURSOR);
            mode = (glfw_mode == GLFW_CURSOR_NORMAL) ? glapp::cursor_mode::normal :
                (glfw_mode == GLFW_CURSOR_HIDDEN)    ? glapp::cursor_mode::hidden :
                (glfw_mode == GLFW_CURSOR_DISABLED)  ? glapp::cursor_mode::disabled :
                                                       glapp::cursor_mode::normal;
        }
        return mode;
    }

    void set_cursor_pos(double xpos, double ypos)
    {
        if (handle_) {
            glfwSetCursorPos(handle_->get(), xpos, ypos);
        }
    }

    double cursor_pos_x() const
    {
        double xpos = 0.0;
        if (handle_) {
            glfwGetCursorPos(handle_->get(), &xpos, NULL);
        }
        return xpos;
    }

    double cursor_pos_y() const
    {
        double ypos = 0.0;
        if (handle_) {
            glfwGetCursorPos(handle_->get(), NULL, &ypos);
        }
        return ypos;
    }

    void set_clipboard_string(const char* str)
    {
        if (handle_) {
            glfwSetClipboardString(handle_->get(), str);
        }
    }

    const std::string& clipboard_string()
    {
        static thread_local std::string str;
        str.clear();
        if (handle_) {
            str = glfwGetClipboardString(handle_->get());
        }
        return str;
    }

    void close() const
    {
        if (handle_) {
            glfwSetWindowShouldClose(handle_->get(), GLFW_TRUE);
        }
    }

    bool should_close() const
    {
        bool result = true;
        if (handle_) {
            result = glfwWindowShouldClose(handle_->get()) == GLFW_TRUE;
        }
        return result;
    }

    // clang-format off

    // (glapp::window& window)
    template <typename... Args> void on_frame(Args... args) { frame_event.set_callback(args...); }

    // (glapp::window& window, const std::string& key_name, const glapp::key_state& state, const glapp::modifier& modifier)
    // key_name - see the 'key_to_name' function
    template <typename... Args> void on_key(Args... args) { key_event.set_callback(args...); }

    // (glapp::window& window, const std::string& button_name, const glapp::button_state& state, const glapp::modifier& modifier)
    // button_name - 'left', 'right', 'middle'
    template <typename... Args> void on_mouse_button(Args... args) { mouse_button_event.set_callback(args...); }

    // (glapp::window& window, double x, double y)
    template <typename... Args> void on_mouse_pos(Args... args) { cursor_pos_event.set_callback(args...); }

    // (glapp::window& window, bool entered)
    template <typename... Args> void on_mouse_enter(Args... args) { cursor_enter_event.set_callback(args...); }

    // (glapp::window& window, double xoffset, double yoffset)
    template <typename... Args> void on_mouse_wheel(Args... args) { scroll_event.set_callback(args...); }

    // (glapp::window& window, int32_t x, int32_t y)
    template <typename... Args> void on_window_pos(Args... args) { window_pos_event.set_callback(args...); }

    // (glapp::window& window, int32_t width, int32_t height)
    template <typename... Args> void on_window_size(Args... args) { window_size_event.set_callback(args...); }

    // (glapp::window& window)
    template <typename... Args> void on_window_close(Args... args) { window_close_event.set_callback(args...); }

    // (glapp::window& window)
    template <typename... Args> void on_window_redraw(Args... args) { window_refresh_event.set_callback(args...); }

    // (glapp::window& window, bool focused)
    template <typename... Args> void on_window_focus(Args... args) { window_focus_event.set_callback(args...); }

    // (glapp::window& window, glapp::window_state state)
    template <typename... Args> void on_window_state(Args... args) { window_state_event.set_callback(args...); }

    // (glapp::window& window, float xscale, float yscale)
    template <typename... Args> void on_window_contentscale(Args... args) { window_contentscale_event.set_callback(args...); }

    // (glapp::window& window, int32_t width, int32_t height)
    template <typename... Args> void on_framebuffer_size(Args... args) { framebuffer_size_event.set_callback(args...); }

    // (glapp::window& window, int32_t count, const std::string& paths[])
    template <typename... Args> void on_drop(Args... args) { drop_event.set_callback(args...); }

    // clang-format on

private:
    window(int32_t width, int32_t height, const char* title, const std::shared_ptr<glapp::monitor> monitor, const glapp::window_options& options)
        : title_(internal::or_empty(title))
    {
        glapp::window_options actual_options = options;
        GLFWwindow* glfw_window = nullptr;
        if (monitor) {
            const auto& mode = monitor->current_mode();
            actual_options.set_framebuffer_red_bits(mode.red_bits());
            actual_options.set_framebuffer_green_bits(mode.green_bits());
            actual_options.set_framebuffer_blue_bits(mode.blue_bits());
            actual_options.set_refresh_rate(mode.refresh_rate());
            actual_options.apply();
            (void)width;
            (void)height;
            glfw_window = glfwCreateWindow(mode.width(), mode.height(), title_.c_str(), monitor->glfw_handle(), NULL);
        } else {
            options.apply();
            glfw_window = glfwCreateWindow(width, height, title_.c_str(), NULL, NULL);
        }

        if (glfw_window == nullptr) {
            return;
        }

        auto destroy_callback = [](GLFWwindow* glfw_window) {
            glfwDestroyWindow(glfw_window);
        };
        handle_ = std::make_shared<internal::glfw_window_handle>(glfw_window, destroy_callback);
        if (!handle_) {
            glfwDestroyWindow(glfw_window);
            return;
        }

        glfwSetWindowUserPointer(glfw_window, this);
        setup_callbacks(glfw_window);

        if (state_internal() == glapp::window_state::normal) {
            normal_window_rect_ = current_window_rect();
        } else {
            const auto monitor = placed_monitor_internal();
            assert(monitor);
            const auto rect = monitor->rect();
            normal_window_rect_ = glapp::rect<int32_t>(
                rect.left() + rect.width() / 4,
                rect.top() + rect.height() / 4,
                rect.width() / 2,
                rect.height() / 2);
        }
    }

    void destroy()
    {
        if (handle_) {
            handle_.reset();
        }
    }

    glapp::window_state state_internal() const
    {
        glapp::window_state state = glapp::window_state::normal;
        if (handle_) {
            if (glfwGetWindowAttrib(handle_->get(), GLFW_ICONIFIED) == GLFW_TRUE) {
                state = glapp::window_state::minimized;
            } else if (glfwGetWindowMonitor(handle_->get()) != nullptr) {
                state = glapp::window_state::fullscreen;
            } else if (glfwGetWindowAttrib(handle_->get(), GLFW_MAXIMIZED) == GLFW_TRUE) {
                state = glapp::window_state::maximized;
            } else {
                // glapp::window_state::normal
            }
        }
        return state;
    }

    void set_size_internal(int32_t width, int32_t height)
    {
        if (handle_) {
            int32_t w = width;
            int32_t h = height;
            const float r1 = static_cast<float>(width) / static_cast<float>(height);
            const float r2 = static_cast<float>(aspect_ratio_.width()) / static_cast<float>(aspect_ratio_.height());
            if (r1 < r2) {
                h = static_cast<int32_t>(std::round(width / r2));
            } else if (r2 < r1) {
                w = static_cast<int32_t>(std::round(height * r2));
            } else {
            }
            glfwSetWindowSize(handle_->get(), w, h);
        }
    }

    void fullscreen_internal(const std::shared_ptr<glapp::monitor> monitor)
    {
        if (handle_ && monitor) {
            auto current_state = state_internal();
            if (current_state == glapp::window_state::minimized && glfwGetWindowMonitor(handle_->get()) != nullptr) {
                glfwRestoreWindow(handle_->get());
            } else if (current_state != glapp::window_state::fullscreen) {
                if (current_state == glapp::window_state::maximized) {
                    fullscreen_backup_window_rect_ = current_window_rect();
                } else {
                    fullscreen_backup_window_rect_ = normal_window_rect_;
                }
                glfwSetWindowMonitor(handle_->get(), monitor->glfw_handle(), 0, 0, monitor->rect().width(), monitor->rect().height(), monitor->refresh_rate());
                window_state_event(*this, glapp::window_state::fullscreen);
            } else {
            }
        }
    }

    // Returns the monitor on which the window is currently placed
    std::shared_ptr<glapp::monitor> placed_monitor_internal() const
    {
        int32_t i = 0;
        int32_t max_area = 0;
        int32_t max_index = -1;
        const auto monitors = all_monitors();
        const auto wrect = (state_internal() == glapp::window_state::minimized) ? normal_window_rect_ : glapp::rect<int32_t>(pos(), size());
        for (auto&& monitor : monitors) {
            const auto mrect = monitor->rect();
            const int32_t dx = (std::min)(wrect.right(), mrect.right()) - (std::max)(wrect.left(), mrect.left());
            const int32_t dy = (std::min)(wrect.bottom(), mrect.bottom()) - (std::max)(wrect.top(), mrect.top());
            if (0 < dx && 0 < dy) {
                const int32_t area = dx * dy;
                if (max_area < area) {
                    max_area = area;
                    max_index = i;
                }
            }
            ++i;
        }
        return (0 <= max_index) ? monitors[max_index] : nullptr;
    }

    static void setup_callbacks(GLFWwindow* glfw_window)
    {
        glfwSetKeyCallback(glfw_window, glfw_key_callback);
        glfwSetMouseButtonCallback(glfw_window, glfw_mouse_button_callback);
        glfwSetCursorPosCallback(glfw_window, glfw_cursor_pos_callback);
        glfwSetCursorEnterCallback(glfw_window, glfw_cursor_enter_callback);
        glfwSetScrollCallback(glfw_window, glfw_scroll_callback);
        glfwSetWindowPosCallback(glfw_window, glfw_window_pos_callback);
        glfwSetWindowSizeCallback(glfw_window, glfw_window_size_callback);
        glfwSetWindowCloseCallback(glfw_window, glfw_window_close_callback);
        glfwSetWindowRefreshCallback(glfw_window, glfw_window_refresh_callback);
        glfwSetWindowFocusCallback(glfw_window, glfw_window_focus_callback);
        glfwSetWindowIconifyCallback(glfw_window, glfw_window_iconify_callback);
        glfwSetWindowMaximizeCallback(glfw_window, glfw_window_maximize_callback);
        glfwSetFramebufferSizeCallback(glfw_window, glfw_framebuffer_size_callback);
        glfwSetWindowContentScaleCallback(glfw_window, glfw_window_contentscale_callback);
        glfwSetDropCallback(glfw_window, glfw_drop_callback);
    }

    glapp::rect<int32_t> current_window_rect()
    {
        glapp::rect<int32_t> rect {};
        if (handle_) {
            int xpos = 0;
            int ypos = 0;
            int width = 0;
            int height = 0;
            glfwGetWindowPos(handle_->get(), &xpos, &ypos);
            glfwGetWindowSize(handle_->get(), &width, &height);
            rect = glapp::rect<int32_t>(xpos, ypos, width, height);
        }
        return rect;
    }

    void set_size_limit_internal()
    {
        if (handle_) {
            int min_width = GLFW_DONT_CARE;
            int min_height = GLFW_DONT_CARE;
            int max_width = GLFW_DONT_CARE;
            int max_height = GLFW_DONT_CARE;
            if (size_limit_min_) {
                min_width = size_limit_min_.width();
                min_height = size_limit_min_.height();
            }
            if (size_limit_max_) {
                max_width = size_limit_max_.width();
                max_height = size_limit_max_.height();
            }
            glfwSetWindowSizeLimits(handle_->get(), min_width, min_height, max_width, max_height);
        }
    }

    static void glfw_key_callback(GLFWwindow* glfw_window, int key, int scancode, int state, int mods)
    {
        (void)scancode;
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        std::string key_name = internal::key_to_name(key);
        window->key_event(*window, key_name, glapp::key_state(state), glapp::modifier(mods));
    }

    static void glfw_mouse_button_callback(GLFWwindow* glfw_window, int button, int action, int mods)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        std::string button_name = internal::mouse_button_to_name(button);
        window->mouse_button_event(*window, button_name, glapp::button_state(action), glapp::modifier(mods));
    }

    static void glfw_cursor_pos_callback(GLFWwindow* glfw_window, double x, double y)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        window->cursor_pos_event(*window, x, y);
    }

    static void glfw_cursor_enter_callback(GLFWwindow* glfw_window, int entered)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        window->cursor_enter_event(*window, entered);
    }

    static void glfw_scroll_callback(GLFWwindow* glfw_window, double xoffset, double yoffset)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        window->scroll_event(*window, xoffset, yoffset);
    }

    static void glfw_window_pos_callback(GLFWwindow* glfw_window, int xpos, int ypos)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        if (window->state_internal() == glapp::window_state::normal) {
            window->normal_window_rect_ = window->current_window_rect();
        }
        window->window_pos_event(*window, xpos, ypos);
    }

    static void glfw_window_size_callback(GLFWwindow* glfw_window, int width, int height)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        if (window->state_internal() == glapp::window_state::normal) {
            window->normal_window_rect_ = window->current_window_rect();
            // window->normal_window_size_ = glapp::size<int32_t>(width, height);
        }
        window->window_size_event(*window, width, height);
    }

    static void glfw_window_close_callback(GLFWwindow* glfw_window)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        window->window_close_event(*window);
    }

    static void glfw_window_refresh_callback(GLFWwindow* glfw_window)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        window->window_refresh_event(*window);
    }

    static void glfw_window_focus_callback(GLFWwindow* glfw_window, int focused)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        window->window_focus_event(*window, focused == GLFW_TRUE);
    }

    static void glfw_window_iconify_callback(GLFWwindow* glfw_window, int iconified)
    {
        (void)iconified;
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        auto state = window->state_internal();
        window->window_state_event(*window, state);
    }

    static void glfw_window_maximize_callback(GLFWwindow* glfw_window, int maximized)
    {
        (void)maximized;
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        auto state = window->state_internal();
        window->window_state_event(*window, state);
    }

    static void glfw_framebuffer_size_callback(GLFWwindow* glfw_window, int width, int height)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        window->framebuffer_size_event(*window, width, height);
    }

    static void glfw_window_contentscale_callback(GLFWwindow* glfw_window, float xscale, float yscale)
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        window->window_contentscale_event(*window, xscale, yscale);
    }

    static void glfw_drop_callback(GLFWwindow* glfw_window, int path_count, const char* paths[])
    {
        auto window = static_cast<glapp::window*>(glfwGetWindowUserPointer(glfw_window));
        assert(window != nullptr);
        std::vector<std::string> drop_paths(path_count);
        for (int32_t i = 0; i < path_count; ++i) {
            drop_paths[i] = paths[i];
        }
        window->drop_event(*window, drop_paths);
    }

    void draw()
    {
        // Require the std::weak_ptr::lock to be atomic operation (It is supported since C++14)
        if (handle_) {
            glfwMakeContextCurrent(handle_->get());
            frame_event(*this);
            if (last_swap_interval_ != swap_interval_) {
                glfwSwapInterval(swap_interval_);
                last_swap_interval_ = swap_interval_;
            }
            glfwSwapBuffers(handle_->get());
            // Avoid crash when multi window
            glfwMakeContextCurrent(NULL);
            ++frame_count_;
        }
    }

    std::vector<std::shared_ptr<glapp::monitor>> all_monitors() const;
};

class error {
    friend class app;

private:
    int32_t code_ = GLFW_NO_ERROR;
    std::string description_;

public:
    int32_t code() const { return code_; }
    const std::string& description() const { return description_; }

private:
    error() = default;

    error(int32_t code, const char* description)
        : code_(code)
        , description_(internal::or_empty(description))
    {
    }

    using error_callback_t = std::function<void(const glapp::error&)>;

    static void set_callback(error_callback_t&& callback)
    {
        tls_callback() = callback;
        glfwSetErrorCallback(glfw_error_callback);
    }

    static error_callback_t& tls_callback()
    {
        thread_local error_callback_t callback;
        return callback;
    }

    static error& tls_error()
    {
        thread_local glapp::error error;
        return error;
    }

    static void glfw_error_callback(int error_code, const char* description)
    {
        auto& callback = tls_callback();
        if (callback) {
            glapp::error error(error_code, description);
            callback(error);
        }
    }

    static const glapp::error& last()
    {
        auto& error = tls_error();
        const char* description {};
        error.code_ = glfwGetError(&description);
        error.description_ = internal::or_empty(description);
        return error;
    }
};

class app : internal::noncopyable {
    friend window;

private:
    std::mutex mtx_;
    std::vector<std::shared_ptr<glapp::window>> windows_;
    std::unordered_map<GLFWmonitor*, std::shared_ptr<glapp::monitor>> monitors_;
    std::atomic<bool> drawing_;

public:
    ~app()
    {
        // All remaining windows must be destroyed before glfwTerminate
        for (auto&& window : windows_) {
            window->destroy();
        }
        glfwTerminate();
    }

    static std::shared_ptr<glapp::app> instance()
    {
        static std::mutex mtx;
        static std::weak_ptr<glapp::app> existing_app;

        std::lock_guard<std::mutex> lock(mtx);
        std::shared_ptr<glapp::app> app;
        if (!existing_app.expired()) {
            app = existing_app.lock();
        } else {
            app = std::shared_ptr<glapp::app>(new glapp::app());
            existing_app = std::weak_ptr<glapp::app>(app);
        }
        return app;
    }

    // Returns the last error recorded
    static const error& last_error()
    {
        return error::last();
    }

    // Set callback function on error
    static void on_error(std::function<void(const error&)>&& callback)
    {
        error::set_callback(std::forward<decltype(callback)>(callback));
    }

    std::shared_ptr<glapp::window> add_window(int32_t width, int32_t height, const char* title, const glapp::window_options& options = {})
    {
        return add_window_internal(width, height, title, nullptr, options);
    }

    std::shared_ptr<glapp::window> add_window(int32_t width, int32_t height, const char* title, const std::shared_ptr<glapp::monitor> monitor, const glapp::window_options& options = {})
    {
        return add_window_internal(width, height, title, monitor, options);
    }

    int32_t run(bool use_individual_drawing_thread = false)
    {
        drawing_ = true;
        std::future<void> drawloop_future;
        if (use_individual_drawing_thread) {
            drawloop_future = std::async(std::launch::async, &app::drawloop, this);
        }
        while (!windows_.empty()) {
            if (use_individual_drawing_thread) {
                glfwWaitEvents();
            } else {
                draw_windows();
                glfwPollEvents();
            }

            // Destroy and remove closed window
            std::lock_guard<std::mutex> lock(mtx_);
            for (auto&& window : windows_) {
                if (window->should_close()) {
                    window->destroy();
                }
            }
            // Erase remove idiom
            windows_.erase(
                std::remove_if(
                    windows_.begin(),
                    windows_.end(),
                    [](std::shared_ptr<glapp::window> window) { return !(window && *window); }),
                windows_.end());
        }
        drawing_ = false;
        return 0;
        // Blocked until drawloop is finished by destructor of 'drawloop_future'
    }

    void exit()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        for (auto&& window : windows_) {
            window->close();
        }
    }

    // Returns whether the specified extension is available
    // e.g. GL_ARB_gl_spirv
    bool has_extension(const char* extension)
    {
        bool supported = glfwExtensionSupported(extension) == GLFW_TRUE;
        return supported;
    }

    // Returns the functor of specified function
    // e.g. glSpecializeShaderARB
    std::function<void(void)> get_proc(const char* procname)
    {
        auto proc = glfwGetProcAddress(procname);
        return proc;
    }

    // Resets the time returned by `get_time` to zero
    void set_time(double time)
    {
        return glfwSetTime(time);
    }

    // Returns the elapsed time (seconds) since the program started
    double get_time()
    {
        double time = glfwGetTime();
        return time;
    }

    // Returns primary monitor from the connected monitors
    std::shared_ptr<glapp::monitor> primary_monitor()
    {
        auto glfw_monitor = glfwGetPrimaryMonitor();
        auto iter = monitors_.find(glfw_monitor);
        if (iter != monitors_.end()) {
            // Create a reference instances
            return iter->second;
        } else {
            return {};
        }
    }

    // Returns connected monitors
    std::vector<std::shared_ptr<glapp::monitor>> all_monitors()
    {
        std::vector<std::shared_ptr<glapp::monitor>> monitors;
        for (auto&& monitor : monitors_) {
            monitors.push_back(monitor.second);
        }
        return monitors;
    }

private:
    app()
    {
        auto status = glfwInit();
        assert(status == GLFW_TRUE);

        init_monitors();
    }

    static void glfw_monitor_callback(GLFWmonitor* glfw_monitor, int event)
    {
        auto app = instance();
        assert(app);
        auto iter = app->monitors_.find(glfw_monitor);
        if (iter != app->monitors_.end()) {
            iter->second->destroy();
            app->monitors_.erase(iter);
        }

        if (event == GLFW_CONNECTED) {
            app->monitors_[glfw_monitor] = std::shared_ptr<glapp::monitor>(new glapp::monitor(glfw_monitor)); // std::make_shared<glfw_monitor_handle>(glfw_monitor, nullptr);
        } else if (event == GLFW_DISCONNECTED) {
            // Nothing to do
        } else {
        }
    }

    void init_monitors()
    {
        glfwSetMonitorCallback(glfw_monitor_callback);
        int count = 0;
        auto glfw_monitors = glfwGetMonitors(&count);
        assert(0 < count);
        for (int i = 0; i < count; ++i) {
            monitors_[glfw_monitors[i]] = std::shared_ptr<glapp::monitor>(new glapp::monitor(glfw_monitors[i]));
        }
    }

    std::shared_ptr<glapp::window> add_window_internal(int32_t width, int32_t height, const char* title, const std::shared_ptr<glapp::monitor> monitor, const glapp::window_options& options)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        auto window = std::shared_ptr<glapp::window>(new glapp::window(width, height, title, monitor, options));
        if (window && *window) {
            windows_.emplace_back(window);
        } else {
            window = nullptr;
        }
        return window;
    }

    void drawloop()
    {
        while (drawing_) {
            draw_windows();
            std::this_thread::yield();
        }
    }

    void draw_windows()
    {
        std::vector<std::shared_ptr<glapp::window>> windows;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            for (auto&& window : windows_) {
                windows.push_back(window);
            }
        }
        for (auto&& window : windows) {
            window->draw();
        }
    }
};

std::shared_ptr<glapp::app> get()
{
    return glapp::app::instance();
}

inline std::vector<std::shared_ptr<glapp::monitor>> glapp::window::all_monitors() const
{
    std::vector<std::shared_ptr<glapp::monitor>> monitors;
    auto app = glapp::app::instance();
    if (app) {
        monitors = app->all_monitors();
    }
    return monitors;
}

} // namespace glapp
