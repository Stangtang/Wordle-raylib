#include "raylib.h"
#include "screens.h"
#include "game.h"

int main()
{
    InitWindow(100, 100, "Wordle");
    SetTargetFPS(60);

    std::stack<Screen> screen_stack;
    screen_stack.push(start);

    import_words(all_possible_random_words, all_possible_guess_words);

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        handle_screens(screen_stack);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}