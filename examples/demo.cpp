#include "GrafiX/GrafiX.hpp"

int main() {
    gfx::createWindow(1000, 700, "GrafiX Rotate Demo");

    float rot = 0.f;

    gfx::run(
        [&](){
            gfx::strokeWeight(4.f);
        },
        [&](float dt){
            rot += 80.f * dt; // degrees per second

            gfx::background(gfx::color(25, 25, 35));

            gfx::fill(gfx::color(70, 150, 240));
            gfx::stroke(gfx::color(20, 20, 20));
            gfx::rect(80, 80, 200, 120);

            gfx::fill(gfx::color(240, 190, 60));
            gfx::rectRotate(80, 260, 200, 120, rot);

            gfx::noFill();
            gfx::stroke(gfx::color(30, 220, 120));
            gfx::ellipseRotate(520, 210, 120, 70, rot);

            gfx::stroke(gfx::color(220, 70, 90));
            gfx::lineRotate(520, 420, 820, 540, rot);

            gfx::fill(gfx::color(230, 230, 235));
            gfx::noStroke();
            gfx::text("rectRotate(x,y,w,h,deg) rotiert um Mittelpunkt", 20, 20, 18);

            gfx::arc(400, 300, 120, 0, 90);

            if (gfx::keyDown(gfx::Key::Space)) {
                gfx::text("SPACE down", 20, 50, 18);
            }
            if (gfx::mousePressed(gfx::MouseButton::Left)) {
                gfx::text("CLICK!", (float)gfx::mouseX(), (float)gfx::mouseY(), 20);
            }
        }
    );
}
