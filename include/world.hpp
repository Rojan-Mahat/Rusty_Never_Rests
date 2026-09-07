#pragma once
namespace World{
    inline constexpr float SCREEN_WIDTH = 1200.f;
    inline constexpr float SCREEN_HEIGHT = 800.f;

    inline constexpr float GROUND_Y = 500.f;
    inline constexpr float GROUND_HEIGHT = 10.f;

    inline constexpr float WATER_Y = GROUND_Y + GROUND_HEIGHT;

    inline constexpr float ENEMY_Y = GROUND_Y - 100.f;
    inline constexpr float POWER_Y = GROUND_Y - 25.f;
    inline constexpr float FIRE_MAX_Y = GROUND_Y - 25.f;

    inline constexpr float PLAYER_WIDTH = 46.f;
    inline constexpr float PLAYER_HEIGHT = 68.f;
    inline constexpr float PLAYER_Y = GROUND_Y - PLAYER_HEIGHT;
    inline constexpr float PLAYER_SWIM_Y = 620.f;
}