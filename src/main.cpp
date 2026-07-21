#include "raylib.h"
#include "raymath.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wdeprecated-enum-compare-conditional"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#pragma clang diagnostic pop

#include "Cell.h"
#include "Entity.h"
#include "WorldSettings.h"
#include "World.h"
#include <iostream>

namespace {

struct GameSettings {
    float cameraZoom = 0.05f;
    float simSpeed = 1.0f;
    bool skipRender = false;
    float plantCount = 5.0f;
};


void clampToWorld(Entity& entity, float radius) {
    entity.pos.x = Clamp(entity.pos.x, radius, WORLD_WIDTH - radius);
    entity.pos.y = Clamp(entity.pos.y, radius, WORLD_HEIGHT - radius);
}

// void updateEntity(Entity& entity, float dt, const GameSettings& settings) {
//     Vector2 move{};

//     if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
//         move.y -= 1.0f;
//     if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
//         move.y += 1.0f;
//     if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
//         move.x -= 1.0f;
//     if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
//         move.x += 1.0f;

//     if (move.x != 0.0f || move.y != 0.0f) {
//         move = Vector2Normalize(move);
//         entity.pos.x += move.x * settings.moveSpeed * dt;
//         entity.pos.y += move.y * settings.moveSpeed * dt;
//     }

//     clampToWorld(entity, settings.playerRadius);
// }


bool isMouseInViewport(int viewWidth) {
    return GetMousePosition().x < static_cast<float>(viewWidth);
}

void drawSettingsPanel(GameSettings& settings, int viewWidth, int screenHeight) {
    const int x = viewWidth;
    const int pad = 12;
    const int rowH = 28;
    const int sliderW = PANEL_WIDTH - pad * 2;
    int y = pad;

    DrawRectangle(x, 0, PANEL_WIDTH, screenHeight, {32, 34, 42, 255});
    DrawLine(x, 0, x, screenHeight, {55, 58, 68, 255});

    GuiGroupBox({static_cast<float>(x + pad - 4), static_cast<float>(y - 4),
                 static_cast<float>(sliderW + 8), static_cast<float>(screenHeight - pad * 2)},
                "Settings");
    y += 24;

    GuiSliderBar({static_cast<float>(x + pad), static_cast<float>(y), static_cast<float>(sliderW), 20},
                 "Sim x", TextFormat("%.0f", settings.simSpeed), &settings.simSpeed, 1.0f, 100.0f);
    y += rowH;

    GuiSliderBar({static_cast<float>(x + pad), static_cast<float>(y), static_cast<float>(sliderW), 20},
                 "Plant Count", TextFormat("%.0f", settings.plantCount), &settings.plantCount, 0.0f, 200.0f);
    y += rowH;

    GuiCheckBox({static_cast<float>(x + pad), static_cast<float>(y), 20, 20}, "Skip render", &settings.skipRender);
    y += rowH;

    DrawTextEx(GetFontDefault(), "RMB — pan (left area)\nWheel — zoom (left area)",
               {static_cast<float>(x + pad), static_cast<float>(y)}, 16, 1.0f, LIGHTGRAY);
}

} // namespace

int main() {
    constexpr int screenW = 1000 + PANEL_WIDTH;
    constexpr int screenH = 720;
    constexpr int viewW = screenW - PANEL_WIDTH;

    InitWindow(screenW, screenH, "Evolution");
    SetTargetFPS(60);

    GameSettings settings{};

    Camera2D camera{};
    camera.zoom = settings.cameraZoom;
    camera.offset = {viewW * 0.5f, screenH * 0.5f};
    camera.target.x = WORLD_WIDTH * 0.5f;
    camera.target.y = WORLD_HEIGHT * 0.5f;

    World world;

    for (int i = 0; i < 1000; i++) {
        world.spawnCell(Vector2{static_cast<float>(Random::getInt(0, 50000)), static_cast<float>(Random::getInt(0, 50000))});
    }

    // world.spawnCell(Vector2{static_cast<float>(Random::getInt(0, 50000)), static_cast<float>(Random::getInt(0, 50000))});


    for (int i = 0; i < 3000; i++) {
        world.spawnPlant(Vector2{static_cast<float>(Random::getInt(0, 50000)), static_cast<float>(Random::getInt(0, 50000))});
    }


    while (!WindowShouldClose()) {
        SetTargetFPS(settings.skipRender ? 0 : 60);

        camera.zoom = settings.cameraZoom;

        if (isMouseInViewport(viewW) && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouseDelta = GetMouseDelta();
            camera.target.x -= mouseDelta.x / camera.zoom;
            camera.target.y -= mouseDelta.y / camera.zoom;
        }
        if (isMouseInViewport(viewW)) {
            float wheelMove = GetMouseWheelMove();
            if (wheelMove != 0.0f) {
                float zoomIncrement = 0.1f * camera.zoom;
                settings.cameraZoom += wheelMove * zoomIncrement;
                settings.cameraZoom = Clamp(settings.cameraZoom, 0.001f, 3.0f);
            }
        }

        const int simSteps = static_cast<int>(Clamp(settings.simSpeed, 1.0f, 100.0f));
        for (int step = 0; step < simSteps; ++step) {
            world.updateEntities();

            for (int i = 0; i < settings.plantCount; i++) {
                if (!Random::getInt(0, 100))
                    world.spawnPlant(Vector2{
                        static_cast<float>(Random::getInt(0, static_cast<int>(WORLD_WIDTH))),
                        static_cast<float>(Random::getInt(0, static_cast<int>(WORLD_HEIGHT)))
                    });
            }
        }

        BeginDrawing();
        ClearBackground({24, 26, 32, 255});

        if (!settings.skipRender) {
            BeginScissorMode(0, 0, viewW, screenH);
            BeginMode2D(camera);
            world.drawEntities();
            EndMode2D();
            EndScissorMode();
        } else {
            DrawText("Simulation running (render skipped)", 12, 12, 20, LIGHTGRAY);
        }

        drawSettingsPanel(settings, viewW, screenH);

        DrawText(TextFormat("Cells: %d", world.countCell), 12, 36, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
