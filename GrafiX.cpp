#include "GrafiX/GrafiX.hpp"

#include <array>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <cstdint>

#include <SFML/Window/WindowBase.hpp>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace gfx {
    static std::optional<sf::RenderWindow> gWindow;

    static sf::Color gFillColor = sf::Color::White;
    static bool gUseFill = true;
    static sf::Color gStrokeColor = sf::Color::Black;
    static bool gUseStroke = true;
    static float gStrokeW = 1.f;

    static sf::Font gFont;
    static bool gFontLoaded = false;

    // Input state
    static std::array<bool, sf::Keyboard::KeyCount> gKeyDown{};
    static std::array<bool, sf::Keyboard::KeyCount> gKeyPressed{};
    static std::array<bool, sf::Keyboard::KeyCount> gKeyReleased{};

    static std::array<bool, sf::Mouse::ButtonCount> gMouseDown{};
    static std::array<bool, sf::Mouse::ButtonCount> gMousePressed{};
    static std::array<bool, sf::Mouse::ButtonCount> gMouseReleased{};

    static int gMouseX = 0, gMouseY = 0, gPMouseX = 0, gPMouseY = 0;
    static int gWheelDelta = 0;

    sf::RenderWindow& window() {
        if (!gWindow) throw std::runtime_error("gfx::window() called before createWindow()");
        return *gWindow;
    }

    int width() {
        return (int)window().getSize().x;
    }

    int height() {
        return (int)window().getSize().y;
    }

    void createWindowMaximized(const std::string& title) {
        // Startgröße egal, wird direkt maximiert
        createWindow(1280, 720, title);
        maximizeWindow();
    }

    void maximizeWindow() {
#ifdef _WIN32
        if (!gWindow) throw std::runtime_error("gfx::maximizeWindow() called before createWindow()");

        // SFML 3: Native handle über WindowBase
        auto handle = gWindow->getNativeHandle(); // HWND auf Windows
        HWND hwnd = (HWND)handle;

        ShowWindow(hwnd, SW_MAXIMIZE);
        UpdateWindow(hwnd);
#endif
    }

    static void resetPerFrameInput() {
        gKeyPressed.fill(false);
        gKeyReleased.fill(false);
        gMousePressed.fill(false);
        gMouseReleased.fill(false);
        gWheelDelta = 0;

        gPMouseX = gMouseX;
        gPMouseY = gMouseY;
    }

    static bool keyIndexOk(size_t idx) { return idx < gKeyDown.size(); }
    static bool mouseIndexOk(size_t idx) { return idx < gMouseDown.size(); }

    static void ensureValidViewSize() {
        if (!gWindow) return;
        const sf::Vector2u s = gWindow->getSize();
        const float w = (float)std::max(1u, s.x);
        const float h = (float)std::max(1u, s.y);
        gWindow->setView(sf::View(sf::FloatRect({0.f, 0.f}, {w, h})));
    }

    void createWindow(int w, int h, const std::string& title) {
        w = std::max(1, w);
        h = std::max(1, h);

        gWindow.emplace(sf::VideoMode({(unsigned)w, (unsigned)h}), title, sf::Style::Default);
        gWindow->setFramerateLimit(60);

        ensureValidViewSize();

        auto sz = gWindow->getSize();
        gMouseX = (int)sz.x / 2;
        gMouseY = (int)sz.y / 2;
        gPMouseX = gMouseX;
        gPMouseY = gMouseY;

        if (!gFontLoaded) {
            gFontLoaded = gFont.openFromFile("assets/front.ttf"); // optional
        }
    }

    void run(std::function<void()> setup, std::function<void(float dt)> draw) {
        if (!gWindow) throw std::runtime_error("gfx::run() called before createWindow()");
        setup();

        sf::Clock clock;
        while (gWindow->isOpen()) {
            resetPerFrameInput();
            ensureValidViewSize();

            while (auto ev = gWindow->pollEvent()) {
                const sf::Event& e = *ev;

                if (e.is<sf::Event::Closed>()) gWindow->close();
                if (e.is<sf::Event::Resized>()) ensureValidViewSize();

                if (e.is<sf::Event::KeyPressed>()) {
                    auto* kp = e.getIf<sf::Event::KeyPressed>();
                    if (kp) {
                        size_t idx = (size_t)kp->code;
                        if (keyIndexOk(idx)) {
                            if (!gKeyDown[idx]) gKeyPressed[idx] = true;
                            gKeyDown[idx] = true;
                        }
                    }
                }

                if (e.is<sf::Event::KeyReleased>()) {
                    auto* kr = e.getIf<sf::Event::KeyReleased>();
                    if (kr) {
                        size_t idx = (size_t)kr->code;
                        if (keyIndexOk(idx)) {
                            gKeyDown[idx] = false;
                            gKeyReleased[idx] = true;
                        }
                    }
                }

                if (e.is<sf::Event::MouseButtonPressed>()) {
                    auto* mb = e.getIf<sf::Event::MouseButtonPressed>();
                    if (mb) {
                        size_t idx = (size_t)mb->button;
                        if (mouseIndexOk(idx)) {
                            if (!gMouseDown[idx]) gMousePressed[idx] = true;
                            gMouseDown[idx] = true;
                        }
                    }
                }

                if (e.is<sf::Event::MouseButtonReleased>()) {
                    auto* mb = e.getIf<sf::Event::MouseButtonReleased>();
                    if (mb) {
                        size_t idx = (size_t)mb->button;
                        if (mouseIndexOk(idx)) {
                            gMouseDown[idx] = false;
                            gMouseReleased[idx] = true;
                        }
                    }
                }

                if (e.is<sf::Event::MouseMoved>()) {
                    auto* mm = e.getIf<sf::Event::MouseMoved>();
                    if (mm) {
                        gMouseX = mm->position.x;
                        gMouseY = mm->position.y;
                    }
                }

                if (e.is<sf::Event::MouseWheelScrolled>()) {
                    auto* mw = e.getIf<sf::Event::MouseWheelScrolled>();
                    if (mw) gWheelDelta += (int)mw->delta;
                }
            }

            const sf::Vector2u ws = gWindow->getSize();
            if (ws.x == 0 || ws.y == 0) {
                clock.restart();
                continue;
            }

            float dt = clock.restart().asSeconds();
            if (dt > 0.1f) dt = 0.1f;

            draw(dt);

            ensureValidViewSize();
            gWindow->display();
        }
    }

    void clear(sf::Color c) { window().clear(c); }
    void background(sf::Color c) { window().clear(c); }

    void fill(sf::Color c) {
        gUseFill = true;
        gFillColor = c;
    }

    void noFill() { gUseFill = false; }

    void stroke(sf::Color c) {
        gUseStroke = true;
        gStrokeColor = c;
    }

    void noStroke() { gUseStroke = false; }

    void strokeWeight(float px) { gStrokeW = std::max(0.f, px); }

    void textSize(unsigned int) {
        /* optional */
    }

    sf::Color color(int r, int g, int b, int a) {
        auto clamp255 = [](int v) -> std::uint8_t {
            v = std::clamp(v, 0, 255);
            return static_cast<std::uint8_t>(v);
        };
        return sf::Color(clamp255(r), clamp255(g), clamp255(b), clamp255(a));
    }

    float radians(float deg) { return deg * 3.1415926535f / 180.f; }
    float degrees(float rad) { return rad * 180.f / 3.1415926535f; }

    // --- arc() helpers: 0° = oben, clockwise ---
    static float normDeg(float d) {
        d = std::fmod(d, 360.f);
        if (d < 0.f) d += 360.f;
        return d;
    }

    // cw-span von a -> b (beide in Grad). Ergebnis: [0..360)
    static float cwSpanDeg(float a, float b) {
        a = normDeg(a);
        b = normDeg(b);
        float span = b - a;
        if (span < 0.f) span += 360.f;
        return span;
    }

    // 0° oben (0,-1), 90° rechts (1,0), clockwise
    static sf::Vector2f dirFromDeg(float deg) {
        const float rad = radians(deg);
        return { std::sinf(rad), -std::cosf(rad) };
    }

    static int arcSegments(float spanDeg, float r) {
        int seg = (int)std::ceil(spanDeg * std::max(1.f, r) / 45.f);
        seg = std::clamp(seg, 2, 1440);
        return seg;
    }

    static void applyFill(sf::Shape& s) {
        if (gUseFill) s.setFillColor(gFillColor);
        else s.setFillColor(sf::Color::Transparent);
    }

    // Wichtig: Outline nur für "stabile" Shapes (Circle/Rect/Ellipse). NICHT für Convex (Triangles/Polygons).
    static void applyStableStroke(sf::Shape& s) {
        if (gUseStroke && gStrokeW > 0.f) {
            const float safeW = std::min(gStrokeW, 20.0f);
            s.setOutlineThickness(safeW);
            s.setOutlineColor(gStrokeColor);
        } else {
            s.setOutlineThickness(0.f);
        }
    }

    // ----- Normal primitives -----
    void circle(float x, float y, float r) {
        sf::CircleShape s(r);
        s.setPosition({x - r, y - r});
        applyFill(s);
        applyStableStroke(s);
        window().draw(s);
    }

    void rect(float x1, float y1, float x2, float y2) {
        float w = x2 - x1;
        float h = y2 - y1;
        float x = x1;
        float y = y1;
        sf::RectangleShape s({w, h});
        s.setPosition({x, y});
        applyFill(s);
        applyStableStroke(s);
        window().draw(s);
    }

    void line(float x1, float y1, float x2, float y2) {
        if (!gUseStroke) return;

        sf::Vector2f a(x1, y1), b(x2, y2);
        sf::Vector2f d = b - a;
        float len = std::sqrt(d.x * d.x + d.y * d.y);
        if (len <= 0.001f) return;

        sf::RectangleShape s({len, std::max(1.f, gStrokeW)});
        s.setFillColor(gStrokeColor);
        s.setOrigin({0.f, s.getSize().y * 0.5f});
        s.setPosition(a);
        float angDeg = std::atan2(d.y, d.x) * 180.f / 3.1415926535f;
        s.setRotation(sf::degrees(angDeg));
        window().draw(s);
    }

    void pixel(float x, float y) {
        if (!gUseStroke) return;
        sf::RectangleShape s({1.f, 1.f});
        s.setPosition({x, y});
        s.setFillColor(gStrokeColor);
        window().draw(s);
    }

    void ellipse(float x, float y, float rx, float ry) {
        sf::CircleShape s(1.f, 60);
        s.setOrigin({1.f, 1.f});
        s.setPosition({x, y});
        s.setScale({rx, ry});
        applyFill(s);
        applyStableStroke(s);
        window().draw(s);
    }

    // *** CRASH-FIX ***
    void triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
        sf::ConvexShape s;
        s.setPointCount(3);
        s.setPoint(0, {x1, y1});
        s.setPoint(1, {x2, y2});
        s.setPoint(2, {x3, y3});

        if (gUseFill) s.setFillColor(gFillColor);
        else s.setFillColor(sf::Color::Transparent);

        s.setOutlineThickness(0.f);
        window().draw(s);
    }

    void polygon(const std::vector<sf::Vector2f>& points) {
        if (points.size() < 3) return;

        sf::ConvexShape s;
        s.setPointCount(points.size());
        for (size_t i = 0; i < points.size(); ++i) s.setPoint(i, points[i]);

        if (gUseFill) s.setFillColor(gFillColor);
        else s.setFillColor(sf::Color::Transparent);

        s.setOutlineThickness(0.f);
        window().draw(s);
    }

    void text(const std::string& str, float x, float y, int size) {
        if (!gFontLoaded) return;
        sf::Text t(gFont, str, (unsigned)std::max(1, size));
        if (gUseFill) t.setFillColor(gFillColor);
        else t.setFillColor(sf::Color::Transparent);
        t.setPosition({x, y});
        window().draw(t);
    }

    // ----- Rotate helpers -----
    static sf::RenderStates rotatedAround(float cx, float cy, float deg) {
        sf::Transform tr;
        tr.translate({cx, cy});
        tr.rotate(sf::degrees(deg));
        tr.translate({-cx, -cy});
        return sf::RenderStates(tr);
    }

    void circleRotate(float x, float y, float r, float deg) {
        sf::CircleShape s(r);
        s.setPosition({x - r, y - r});
        applyFill(s);
        applyStableStroke(s);
        window().draw(s, rotatedAround(x, y, deg));
    }

    void rectRotate(float x, float y, float w, float h, float deg) {
        float cx = x + w * 0.5f;
        float cy = y + h * 0.5f;

        sf::RectangleShape s({w, h});
        s.setPosition({x, y});
        applyFill(s);
        applyStableStroke(s);

        window().draw(s, rotatedAround(cx, cy, deg));
    }

    void ellipseRotate(float x, float y, float rx, float ry, float deg) {
        sf::CircleShape s(1.f, 60);
        s.setOrigin({1.f, 1.f});
        s.setPosition({x, y});
        s.setScale({rx, ry});
        applyFill(s);
        applyStableStroke(s);

        window().draw(s, rotatedAround(x, y, deg));
    }

    void arc(float x, float y, float r, float degree1, float degree2) {
        if (r <= 0.f) return;

        const float a0 = normDeg(degree1);
        const float span = cwSpanDeg(degree1, degree2);

        if (span <= 0.0001f) return;

        if (span >= 359.999f) {
            circle(x, y, r);
            return;
        }

        const int seg = arcSegments(span, r);
        const float step = span / (float)seg;

        if (gUseFill) {
            std::vector<sf::Vector2f> pts;
            pts.reserve((size_t)seg + 2);

            pts.push_back({x, y});

            for (int i = 0; i <= seg; i++) {
                float a = a0 + step * (float)i;
                sf::Vector2f d = dirFromDeg(a);
                pts.push_back({x + d.x * r, y + d.y * r});
            }

            polygon(pts);
        }

        if (gUseStroke) {
            sf::Vector2f prev;
            {
                sf::Vector2f d = dirFromDeg(a0);
                prev = {x + d.x * r, y + d.y * r};
            }

            for (int i = 1; i <= seg; i++) {
                float a = a0 + step * (float)i;
                sf::Vector2f d = dirFromDeg(a);
                sf::Vector2f cur = {x + d.x * r, y + d.y * r};

                line(prev.x, prev.y, cur.x, cur.y);
                prev = cur;
            }
        }
    }

    void triangleRotate(float x1, float y1, float x2, float y2, float x3, float y3, float deg) {
        float cx = (x1 + x2 + x3) / 3.f;
        float cy = (y1 + y2 + y3) / 3.f;

        sf::ConvexShape s;
        s.setPointCount(3);
        s.setPoint(0, {x1, y1});
        s.setPoint(1, {x2, y2});
        s.setPoint(2, {x3, y3});

        if (gUseFill) s.setFillColor(gFillColor);
        else s.setFillColor(sf::Color::Transparent);
        s.setOutlineThickness(0.f);

        window().draw(s, rotatedAround(cx, cy, deg));
    }

    void polygonRotate(const std::vector<sf::Vector2f>& points, float deg) {
        if (points.size() < 3) return;

        float cx = 0.f, cy = 0.f;
        for (auto p : points) {
            cx += p.x;
            cy += p.y;
        }
        cx /= (float)points.size();
        cy /= (float)points.size();

        sf::ConvexShape s;
        s.setPointCount(points.size());
        for (size_t i = 0; i < points.size(); ++i) s.setPoint(i, points[i]);

        if (gUseFill) s.setFillColor(gFillColor);
        else s.setFillColor(sf::Color::Transparent);
        s.setOutlineThickness(0.f);

        window().draw(s, rotatedAround(cx, cy, deg));
    }

    void lineRotate(float x1, float y1, float x2, float y2, float deg) {
        float cx = (x1 + x2) * 0.5f;
        float cy = (y1 + y2) * 0.5f;

        if (!gUseStroke) return;

        sf::Vector2f a(x1, y1), b(x2, y2);
        sf::Vector2f d = b - a;
        float len = std::sqrt(d.x * d.x + d.y * d.y);
        if (len <= 0.001f) return;

        sf::RectangleShape s({len, std::max(1.f, gStrokeW)});
        s.setFillColor(gStrokeColor);
        s.setOrigin({0.f, s.getSize().y * 0.5f});
        s.setPosition(a);
        float angDeg = std::atan2(d.y, d.x) * 180.f / 3.1415926535f;
        s.setRotation(sf::degrees(angDeg));

        window().draw(s, rotatedAround(cx, cy, deg));
    }

    static size_t toKeyIndex(gfx::Key k) {
        switch (k) {
        case gfx::Key::Unknown: return (size_t)sf::Keyboard::Key::Unknown;

        case gfx::Key::A: return (size_t)sf::Keyboard::Key::A;
        case gfx::Key::B: return (size_t)sf::Keyboard::Key::B;
        case gfx::Key::C: return (size_t)sf::Keyboard::Key::C;
        case gfx::Key::D: return (size_t)sf::Keyboard::Key::D;
        case gfx::Key::E: return (size_t)sf::Keyboard::Key::E;
        case gfx::Key::F: return (size_t)sf::Keyboard::Key::F;
        case gfx::Key::G: return (size_t)sf::Keyboard::Key::G;
        case gfx::Key::H: return (size_t)sf::Keyboard::Key::H;
        case gfx::Key::I: return (size_t)sf::Keyboard::Key::I;
        case gfx::Key::J: return (size_t)sf::Keyboard::Key::J;
        case gfx::Key::K: return (size_t)sf::Keyboard::Key::K;
        case gfx::Key::L: return (size_t)sf::Keyboard::Key::L;
        case gfx::Key::M: return (size_t)sf::Keyboard::Key::M;
        case gfx::Key::N: return (size_t)sf::Keyboard::Key::N;
        case gfx::Key::O: return (size_t)sf::Keyboard::Key::O;
        case gfx::Key::P: return (size_t)sf::Keyboard::Key::P;
        case gfx::Key::Q: return (size_t)sf::Keyboard::Key::Q;
        case gfx::Key::R: return (size_t)sf::Keyboard::Key::R;
        case gfx::Key::S: return (size_t)sf::Keyboard::Key::S;
        case gfx::Key::T: return (size_t)sf::Keyboard::Key::T;
        case gfx::Key::U: return (size_t)sf::Keyboard::Key::U;
        case gfx::Key::V: return (size_t)sf::Keyboard::Key::V;
        case gfx::Key::W: return (size_t)sf::Keyboard::Key::W;
        case gfx::Key::X: return (size_t)sf::Keyboard::Key::X;
        case gfx::Key::Y: return (size_t)sf::Keyboard::Key::Y;
        case gfx::Key::Z: return (size_t)sf::Keyboard::Key::Z;

        case gfx::Key::Num0: return (size_t)sf::Keyboard::Key::Num0;
        case gfx::Key::Num1: return (size_t)sf::Keyboard::Key::Num1;
        case gfx::Key::Num2: return (size_t)sf::Keyboard::Key::Num2;
        case gfx::Key::Num3: return (size_t)sf::Keyboard::Key::Num3;
        case gfx::Key::Num4: return (size_t)sf::Keyboard::Key::Num4;
        case gfx::Key::Num5: return (size_t)sf::Keyboard::Key::Num5;
        case gfx::Key::Num6: return (size_t)sf::Keyboard::Key::Num6;
        case gfx::Key::Num7: return (size_t)sf::Keyboard::Key::Num7;
        case gfx::Key::Num8: return (size_t)sf::Keyboard::Key::Num8;
        case gfx::Key::Num9: return (size_t)sf::Keyboard::Key::Num9;

        case gfx::Key::Escape: return (size_t)sf::Keyboard::Key::Escape;
        case gfx::Key::LControl: return (size_t)sf::Keyboard::Key::LControl;
        case gfx::Key::LShift: return (size_t)sf::Keyboard::Key::LShift;
        case gfx::Key::LAlt: return (size_t)sf::Keyboard::Key::LAlt;
        case gfx::Key::LSystem: return (size_t)sf::Keyboard::Key::LSystem;
        case gfx::Key::RControl: return (size_t)sf::Keyboard::Key::RControl;
        case gfx::Key::RShift: return (size_t)sf::Keyboard::Key::RShift;
        case gfx::Key::RAlt: return (size_t)sf::Keyboard::Key::RAlt;
        case gfx::Key::RSystem: return (size_t)sf::Keyboard::Key::RSystem;
        case gfx::Key::Menu: return (size_t)sf::Keyboard::Key::Menu;

        case gfx::Key::LBracket: return (size_t)sf::Keyboard::Key::LBracket;
        case gfx::Key::RBracket: return (size_t)sf::Keyboard::Key::RBracket;
        case gfx::Key::Semicolon: return (size_t)sf::Keyboard::Key::Semicolon;
        case gfx::Key::Comma: return (size_t)sf::Keyboard::Key::Comma;
        case gfx::Key::Period: return (size_t)sf::Keyboard::Key::Period;
        case gfx::Key::Slash: return (size_t)sf::Keyboard::Key::Slash;
        case gfx::Key::Backslash: return (size_t)sf::Keyboard::Key::Backslash;
        case gfx::Key::Apostrophe: return (size_t)sf::Keyboard::Key::Apostrophe;
        case gfx::Key::Grave: return (size_t)sf::Keyboard::Key::Grave;
        case gfx::Key::Equal: return (size_t)sf::Keyboard::Key::Equal;
        case gfx::Key::Hyphen: return (size_t)sf::Keyboard::Key::Hyphen;
        case gfx::Key::Space: return (size_t)sf::Keyboard::Key::Space;
        case gfx::Key::Enter: return (size_t)sf::Keyboard::Key::Enter;
        case gfx::Key::Backspace: return (size_t)sf::Keyboard::Key::Backspace;
        case gfx::Key::Tab: return (size_t)sf::Keyboard::Key::Tab;

        case gfx::Key::PageUp: return (size_t)sf::Keyboard::Key::PageUp;
        case gfx::Key::PageDown: return (size_t)sf::Keyboard::Key::PageDown;
        case gfx::Key::End: return (size_t)sf::Keyboard::Key::End;
        case gfx::Key::Home: return (size_t)sf::Keyboard::Key::Home;
        case gfx::Key::Insert: return (size_t)sf::Keyboard::Key::Insert;
        case gfx::Key::Delete: return (size_t)sf::Keyboard::Key::Delete;

        case gfx::Key::Add: return (size_t)sf::Keyboard::Key::Add;
        case gfx::Key::Subtract: return (size_t)sf::Keyboard::Key::Subtract;
        case gfx::Key::Multiply: return (size_t)sf::Keyboard::Key::Multiply;
        case gfx::Key::Divide: return (size_t)sf::Keyboard::Key::Divide;

        case gfx::Key::Left: return (size_t)sf::Keyboard::Key::Left;
        case gfx::Key::Right: return (size_t)sf::Keyboard::Key::Right;
        case gfx::Key::Up: return (size_t)sf::Keyboard::Key::Up;
        case gfx::Key::Down: return (size_t)sf::Keyboard::Key::Down;

        case gfx::Key::Numpad0: return (size_t)sf::Keyboard::Key::Numpad0;
        case gfx::Key::Numpad1: return (size_t)sf::Keyboard::Key::Numpad1;
        case gfx::Key::Numpad2: return (size_t)sf::Keyboard::Key::Numpad2;
        case gfx::Key::Numpad3: return (size_t)sf::Keyboard::Key::Numpad3;
        case gfx::Key::Numpad4: return (size_t)sf::Keyboard::Key::Numpad4;
        case gfx::Key::Numpad5: return (size_t)sf::Keyboard::Key::Numpad5;
        case gfx::Key::Numpad6: return (size_t)sf::Keyboard::Key::Numpad6;
        case gfx::Key::Numpad7: return (size_t)sf::Keyboard::Key::Numpad7;
        case gfx::Key::Numpad8: return (size_t)sf::Keyboard::Key::Numpad8;
        case gfx::Key::Numpad9: return (size_t)sf::Keyboard::Key::Numpad9;

        case gfx::Key::F1: return (size_t)sf::Keyboard::Key::F1;
        case gfx::Key::F2: return (size_t)sf::Keyboard::Key::F2;
        case gfx::Key::F3: return (size_t)sf::Keyboard::Key::F3;
        case gfx::Key::F4: return (size_t)sf::Keyboard::Key::F4;
        case gfx::Key::F5: return (size_t)sf::Keyboard::Key::F5;
        case gfx::Key::F6: return (size_t)sf::Keyboard::Key::F6;
        case gfx::Key::F7: return (size_t)sf::Keyboard::Key::F7;
        case gfx::Key::F8: return (size_t)sf::Keyboard::Key::F8;
        case gfx::Key::F9: return (size_t)sf::Keyboard::Key::F9;
        case gfx::Key::F10: return (size_t)sf::Keyboard::Key::F10;
        case gfx::Key::F11: return (size_t)sf::Keyboard::Key::F11;
        case gfx::Key::F12: return (size_t)sf::Keyboard::Key::F12;
        case gfx::Key::F13: return (size_t)sf::Keyboard::Key::F13;
        case gfx::Key::F14: return (size_t)sf::Keyboard::Key::F14;
        case gfx::Key::F15: return (size_t)sf::Keyboard::Key::F15;

        case gfx::Key::Pause: return (size_t)sf::Keyboard::Key::Pause;

        default: return (size_t)sf::Keyboard::Key::Unknown;
        }
    }

    static size_t toMouseIndex(gfx::MouseButton b) {
        switch (b) {
        case gfx::MouseButton::Left: return (size_t)sf::Mouse::Button::Left;
        case gfx::MouseButton::Right: return (size_t)sf::Mouse::Button::Right;
        case gfx::MouseButton::Middle: return (size_t)sf::Mouse::Button::Middle;
        default: return (size_t)sf::Mouse::Button::Left;
        }
    }

    bool keyDown(gfx::Key k) {
        size_t idx = toKeyIndex(k);
        return keyIndexOk(idx) ? gKeyDown[idx] : false;
    }

    bool keyPressed(gfx::Key k) {
        size_t idx = toKeyIndex(k);
        return keyIndexOk(idx) ? gKeyPressed[idx] : false;
    }

    bool keyReleased(gfx::Key k) {
        size_t idx = toKeyIndex(k);
        return keyIndexOk(idx) ? gKeyReleased[idx] : false;
    }

    bool mouseDown(gfx::MouseButton b) {
        size_t idx = toMouseIndex(b);
        return mouseIndexOk(idx) ? gMouseDown[idx] : false;
    }

    bool mousePressed(gfx::MouseButton b) {
        size_t idx = toMouseIndex(b);
        return mouseIndexOk(idx) ? gMousePressed[idx] : false;
    }

    bool mouseReleased(gfx::MouseButton b) {
        size_t idx = toMouseIndex(b);
        return mouseIndexOk(idx) ? gMouseReleased[idx] : false;
    }

    int mouseX() { return gMouseX; }
    int mouseY() { return gMouseY; }
    int pmouseX() { return gPMouseX; }
    int pmouseY() { return gPMouseY; }
    int mouseWheelDelta() { return gWheelDelta; }
} // namespace gfx
