#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace gfx {
    // -------------------- Setup / main loop --------------------
    void createWindow(int w, int h, const std::string& title = "GrafiX");
    void run(std::function<void()> setup, std::function<void(float dt)> draw);
    sf::RenderWindow& window();

    // -------------------- Window info / convenience --------------------
    void createWindowMaximized(const std::string& title = "GrafiX");
    void maximizeWindow();
    int width();
    int height();

    // -------------------- Processing-like state --------------------
    void background(sf::Color c);
    void clear(sf::Color c);

    void fill(sf::Color c);
    void noFill();

    void stroke(sf::Color c);
    void noStroke();

    void strokeWeight(float px);
    void textSize(unsigned int px);

    // Helpers (Processing-ish)
    sf::Color color(int r, int g, int b, int a = 255);
    float radians(float deg);
    float degrees(float rad);

    // -------------------- Primitives (normal) --------------------
    void circle(float x, float y, float r);
    void arc(float x, float y, float r, float degree1, float degree2);
    void rect(float x1, float y1, float x2, float y2);
    void line(float x1, float y1, float x2, float y2);
    void pixel(float x, float y);
    void ellipse(float x, float y, float rx, float ry);
    void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
    void polygon(const std::vector<sf::Vector2f>& points);
    void text(const std::string& str, float x, float y, int size = 20);

    // -------------------- Primitives (rotate around center; degree input) --------------------
    // Exactly like normal versions, but rotated around their center/pivot.
    void circleRotate(float x, float y, float r, float deg);
    void rectRotate(float x, float y, float w, float h, float deg);
    void ellipseRotate(float x, float y, float rx, float ry, float deg);
    void triangleRotate(float x1, float y1, float x2, float y2, float x3, float y3, float deg);
    void polygonRotate(const std::vector<sf::Vector2f>& points, float deg);

    // Line rotate variant (rotates the segment around its midpoint)
    void lineRotate(float x1, float y1, float x2, float y2, float deg);

    // -------------------- Input enums (no SFML needed in user code) --------------------
    enum class Key {
        Unknown,

        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

        Escape,
        LControl, LShift, LAlt, LSystem,
        RControl, RShift, RAlt, RSystem,
        Menu,

        LBracket, RBracket,
        Semicolon, Comma, Period, Apostrophe, Slash, Backslash, Grave, Equal, Hyphen,
        Space,
        Enter,
        Backspace,
        Tab,
        PageUp, PageDown,
        End, Home,
        Insert, Delete,

        Add, Subtract, Multiply, Divide,

        Left, Right, Up, Down,

        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4,
        Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,

        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,

        Pause
    };

    enum class MouseButton {
        Left,
        Right,
        Middle,
    };

    // -------------------- Input --------------------
    bool keyDown(Key k);
    bool keyPressed(Key k);
    bool keyReleased(Key k);

    bool mouseDown(MouseButton b);
    bool mousePressed(MouseButton b);
    bool mouseReleased(MouseButton b);

    int mouseX();
    int mouseY();
    int pmouseX();
    int pmouseY();
    int mouseWheelDelta(); // per frame
} // namespace gfx
