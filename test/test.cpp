#include <gtest/gtest.h>

#include "glapp.hpp"

class GlapTest : public ::testing::Test {
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
        glapp::exit();
    }
};

TEST_F(GlapTest, Basic)
{
    auto w = glapp::add_window(640, 480, "Hello glapp");
    EXPECT_TRUE(*w);
    EXPECT_EQ(w->size().width(), 640);
    EXPECT_EQ(w->size().height(), 480);
    EXPECT_STREQ(w->title().c_str(), "Hello glapp");
    w->set_title(u8"こんにちはglap");
    EXPECT_STREQ(w->title().c_str(), u8"こんにちはglap");
    EXPECT_NE(w->glfw_handle(), nullptr);
    EXPECT_EQ(w->tag().length(), 0);
    w->set_tag("tag");
    EXPECT_STREQ(w->tag().c_str(), "tag");
    int var = 0;
    EXPECT_EQ(w->user_pointer(), nullptr);
    w->set_user_pointer(&var);
    EXPECT_EQ(w->user_pointer(), &var);
    int frame_count = 0;
    w->on_frame([&](const glapp::window&) {
        ++frame_count;
        glapp::exit();
    });
    glapp::run();
    EXPECT_EQ(frame_count, 1);
    EXPECT_EQ(w->size().width(), 0);
}

TEST_F(GlapTest, Get)
{
    auto w = glapp::add_window(640, 480, nullptr);
    w->on_frame([](glapp::window& window) {
        EXPECT_TRUE(glapp::has_extension("GL_ARB_gl_spirv"));
        EXPECT_FALSE(glapp::has_extension("GL_SPIR_V_BINARY_ARB"));
        EXPECT_FALSE(glapp::has_extension("glSpecializeShaderARB"));

        EXPECT_EQ(glapp::get_proc("GL_ARB_gl_spirv"), nullptr);
        EXPECT_EQ(glapp::get_proc("GL_SPIR_V_BINARY_ARB"), nullptr);
        EXPECT_NE(glapp::get_proc("glSpecializeShaderARB"), nullptr);

        EXPECT_LT(glapp::get_time(), 50.0);
        glapp::set_time(100.0);
        EXPECT_GE(glapp::get_time(), 100.0);
        EXPECT_LT(glapp::get_time(), 200.0);

        window.set_clipboard_string("glapp");
        EXPECT_STREQ(window.clipboard_string().c_str(), "glapp");
        window.set_clipboard_string("");
        EXPECT_STREQ(window.clipboard_string().c_str(), "");

        glapp::exit();
    });
    glapp::run();
}

TEST_F(GlapTest, Input)
{
    auto w = glapp::add_window(640, 480, nullptr);
    w->set_cursor_pos(100.0, 200.0);
    EXPECT_EQ(w->cursor_pos_x(), 100.0);
    EXPECT_EQ(w->cursor_pos_y(), 200.0);
}

TEST_F(GlapTest, Error)
{
    auto error1 = glapp::get_last_error();
    EXPECT_EQ(error1.code(), GLFW_NO_ERROR);
    EXPECT_STREQ(error1.description().c_str(), "");

    glapp::on_error([](const glapp::error& error) {
        EXPECT_EQ(error.code(), GLFW_INVALID_VALUE);
        EXPECT_STREQ(error.description().c_str(), "Invalid window size 0x0");
    });
    (void)glapp::add_window(0, 0, nullptr);
    auto error2 = glapp::get_last_error();
    EXPECT_EQ(error2.code(), GLFW_INVALID_VALUE);
    EXPECT_STREQ(error2.description().c_str(), "Invalid window size 0x0");

    glapp::on_error([](const glapp::error& error) {
        EXPECT_EQ(error.code(), GLFW_INVALID_VALUE);
        EXPECT_STREQ(error.description().c_str(), "Invalid OpenGL version 0.0");
    });
    (void)glapp::add_window(100, 100, nullptr, glapp::window_options().set_opengl_version(0, 0));
    auto error3 = glapp::get_last_error();
    EXPECT_EQ(error3.code(), GLFW_INVALID_VALUE);
    EXPECT_STREQ(error3.description().c_str(), "Invalid OpenGL version 0.0");
}

TEST_F(GlapTest, WindowCallback)
{
    auto w = glapp::add_window(640, 480, "Hello glapp");
    EXPECT_TRUE(*w);

    int draw_count = 0;
    w->on_frame([&](glapp::window& window) {
        ++draw_count;
        window.minimize();
        window.minimize();
        window.restore();
        window.maximize();
        window.maximize();
        window.restore();
        window.fullscreen();
        window.fullscreen();
        window.restore();
        window.set_pos(200, 100);
        window.set_size(320, 240);
        window.set_visible(false);
        window.set_visible(true);
        EXPECT_FLOAT_EQ(window.opacity(), 1.0f);
        window.set_opacity(0.5f);
        EXPECT_FLOAT_EQ(window.opacity(), 127.0f / 255.0f);
        window.focus();
        EXPECT_FALSE(window.should_close());
        window.close();
        EXPECT_TRUE(window.should_close());
    });
    int32_t window_pos_xpos = 0;
    int32_t window_pos_ypos = 0;
    w->on_window_pos([&](glapp::window&, int32_t xpos, int32_t ypos) {
        window_pos_xpos = xpos;
        window_pos_ypos = ypos;
    });
    int32_t window_size_width = 0;
    int32_t window_size_height = 0;
    w->on_window_size([&](glapp::window&, int32_t width, int32_t height) {
        window_size_width = width;
        window_size_height = height;
    });
    int32_t framebuffer_size_width = 0;
    int32_t framebuffer_size_height = 0;
    w->on_framebuffer_size([&](glapp::window&, int32_t width, int32_t height) {
        framebuffer_size_width = width;
        framebuffer_size_height = height;
    });
    int window_normal_count = 0;
    int window_minimize_count = 0;
    int window_maximize_count = 0;
    int window_fullscreen_count = 0;
    w->on_window_state([&](glapp::window&, glapp::window_state state) {
        if (state == glapp::window_state::minimized) {
            ++window_minimize_count;
        } else if (state == glapp::window_state::maximized) {
            ++window_maximize_count;
        } else if (state == glapp::window_state::fullscreen) {
            ++window_fullscreen_count;
        } else {
            ++window_normal_count;
        }
    });
    int window_focus_true_count = 0;
    int window_focus_false_count = 0;
    w->on_window_focus([&](glapp::window&, bool focused) {
        if (focused) {
            ++window_focus_true_count;
        } else {
            ++window_focus_false_count;
        }
    });
    int window_close_count = 0;
    w->on_window_close([&](glapp::window&) {
        ++window_close_count;
    });

    glapp::run();

    EXPECT_EQ(draw_count, 1);
    EXPECT_EQ(window_pos_xpos, 200);
    EXPECT_EQ(window_pos_ypos, 100);
    EXPECT_EQ(window_size_width, 320);
    EXPECT_EQ(window_size_height, 240);
    EXPECT_EQ(framebuffer_size_width, 320);
    EXPECT_EQ(framebuffer_size_height, 240);
    EXPECT_EQ(window_normal_count, 2);
    EXPECT_EQ(window_minimize_count, 1);
    EXPECT_EQ(window_maximize_count, 1);
    EXPECT_EQ(window_fullscreen_count, 1);
    EXPECT_EQ(window_focus_true_count, 2);
    EXPECT_EQ(window_focus_false_count, 2);
    EXPECT_EQ(window_close_count, 0);
}

TEST_F(GlapTest, WindowStateFromNormal)
{
    const glapp::size<int32_t> normal_size(640, 480);
    auto w = glapp::add_window(normal_size.width(), normal_size.height(), nullptr);
    const auto monitor = w->placed_monitor();
    ASSERT_TRUE(monitor);
    const auto monitor_rect = monitor->rect();
    w->maximize();
    const auto maximized_size = w->size();
    w->restore();

    EXPECT_EQ(w->size().width(), normal_size.width());
    EXPECT_EQ(w->size().height(), normal_size.height());
    w->fullscreen();
    EXPECT_EQ(w->size().width(), monitor_rect.width());
    EXPECT_EQ(w->size().height(), monitor_rect.height());
    w->restore();
    EXPECT_EQ(w->size().width(), normal_size.width());
    EXPECT_EQ(w->size().height(), normal_size.height());

    w->fullscreen();
    w->maximize();
    EXPECT_EQ(w->size().width(), maximized_size.width());
    EXPECT_EQ(w->size().height(), maximized_size.height());
    w->restore();
    EXPECT_EQ(w->size().width(), normal_size.width());
    EXPECT_EQ(w->size().height(), normal_size.height());

    w->fullscreen();
    w->minimize();
    w->restore();
    EXPECT_EQ(w->size().width(), normal_size.width());
    EXPECT_EQ(w->size().height(), normal_size.height());

    w->fullscreen();
    w->minimize();
    w->maximize();
    EXPECT_EQ(w->size().width(), maximized_size.width());
    EXPECT_EQ(w->size().height(), maximized_size.height());
    w->restore();
    EXPECT_EQ(w->size().width(), normal_size.width());
    EXPECT_EQ(w->size().height(), normal_size.height());

    w->fullscreen();
    w->minimize();
    w->fullscreen();
    EXPECT_EQ(w->size().width(), monitor_rect.width());
    EXPECT_EQ(w->size().height(), monitor_rect.height());
    w->restore();
    EXPECT_EQ(w->size().width(), normal_size.width());
    EXPECT_EQ(w->size().height(), normal_size.height());
}

TEST_F(GlapTest, WindowStateFromMaximized)
{
    const glapp::size<int32_t> normal_size(640, 480);
    auto w = glapp::add_window(normal_size.width(), normal_size.height(), nullptr);
    const auto monitor = w->placed_monitor();
    ASSERT_TRUE(monitor);
    const auto monitor_rect = monitor->rect();
    w->maximize();
    const auto maximized_size = w->size();

    w->fullscreen();
    EXPECT_EQ(w->size().width(), monitor_rect.width());
    EXPECT_EQ(w->size().height(), monitor_rect.height());
    w->restore();
    EXPECT_EQ(w->size().width(), normal_size.width());
    EXPECT_EQ(w->size().height(), normal_size.height());

    w->maximize();
    w->fullscreen();
    w->maximize();
    EXPECT_EQ(w->size().width(), maximized_size.width());
    EXPECT_EQ(w->size().height(), maximized_size.height());

    w->fullscreen();
    w->minimize();
    w->restore();
    EXPECT_EQ(w->size().width(), normal_size.width());
    EXPECT_EQ(w->size().height(), normal_size.height());

    w->maximize();
    w->fullscreen();
    w->minimize();
    w->maximize();
    EXPECT_EQ(w->size().width(), maximized_size.width());
    EXPECT_EQ(w->size().height(), maximized_size.height());
    w->restore();
    // EXPECT_EQ(w->size().width(), normal_size.width());
    // EXPECT_EQ(w->size().height(), normal_size.height());

    w->maximize();
    w->fullscreen();
    w->minimize();
    w->fullscreen();
    EXPECT_EQ(w->size().width(), monitor_rect.width());
    EXPECT_EQ(w->size().height(), monitor_rect.height());
    w->restore();
    // EXPECT_EQ(w->size().width(), normal_size.width());
    // EXPECT_EQ(w->size().height(), normal_size.height());
}

TEST_F(GlapTest, WindowSizeLimit)
{
    auto w = glapp::add_window(640, 480, nullptr);
    w->set_size_limit_min(300, 200);
    w->set_size(250, 250);
    EXPECT_EQ(w->size().width(), 300);
    EXPECT_EQ(w->size().height(), 250);
    w->set_size(150, 150);
    EXPECT_EQ(w->size().width(), 300);
    EXPECT_EQ(w->size().height(), 200);
    w->set_size_limit_min();
    w->set_size(150, 150);
    EXPECT_EQ(w->size().width(), 150);
    EXPECT_EQ(w->size().height(), 150);

    w->set_size_limit_max(400, 300);
    w->set_size(350, 350);
    EXPECT_EQ(w->size().width(), 350);
    EXPECT_EQ(w->size().height(), 300);
    w->set_size(450, 450);
    EXPECT_EQ(w->size().width(), 400);
    EXPECT_EQ(w->size().height(), 300);
    w->set_size_limit_max();
    w->set_size(450, 450);
    EXPECT_EQ(w->size().width(), 450);
    EXPECT_EQ(w->size().height(), 450);
}

TEST_F(GlapTest, WindowSizeAspectRatio)
{
    auto w = glapp::add_window(600, 600, nullptr);
    w->set_aspect_ratio(3, 2);
    EXPECT_EQ(w->size().width(), 600);
    EXPECT_EQ(w->size().height(), 400);
    w->set_aspect_ratio(2, 3);
    EXPECT_EQ(w->size().width(), 600);
    EXPECT_EQ(w->size().height(), 900);
    w->set_aspect_ratio();
    w->set_size(300, 300);
    EXPECT_EQ(w->size().width(), 300);
    EXPECT_EQ(w->size().height(), 300);
}
