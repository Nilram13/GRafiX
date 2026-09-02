#include <GrafiX/GrafiX.hpp>

int main()
{
    // 1) Create a window
    gfx::createWindow(800, 600, "GrafiX - simple demo");

    // 2) Run the main loop: setup() is called once, draw(dt) every frame
    gfx::run(
        []() {
            // setup (runs once)
            gfx::textSize(24);
        },
        [](float /*dt*/) {
            // draw (runs every frame)

            // Clear the screen
            gfx::background(gfx::color(20, 20, 30));

            // Draw a big blue circle in the center
            gfx::fill(gfx::color(80, 200, 255));
            gfx::stroke(gfx::color(255, 255, 255));
            gfx::strokeWeight(3);
            gfx::circle(400, 300, 80);

            // Draw a green circle at the mouse position
            gfx::noStroke();
            gfx::fill(gfx::color(120, 255, 140));
            gfx::circle((float)gfx::mouseX(), (float)gfx::mouseY(), 12);

            // Draw a label
            gfx::fill(gfx::color(255, 255, 255));
            gfx::text("Move the mouse (green dot).", 20, 20, 24);
        }
    );

    return 0;
}