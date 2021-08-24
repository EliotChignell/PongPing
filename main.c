/* Raylib game by Eliot Chignell */

#include "./raylib/src/raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

Color colour1 = {51, 55, 69, 255};
Color colour1b = {51 * 2, 55 * 2, 69 * 2, 255};
Color colour2 = {230, 52, 98, 255};
Color colour3 = {254, 95, 85, 255};
Color colour4 = {68, 157, 209, 255};
Color colour5 = {128, 255, 232, 255};

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    float initialVelocity = 10;
    float deceleration = 0.1;
    float velocity = 0;
    bool direction = 1;
    bool moving = 0;

    float paddleRoundness = 0.5f;
    int paddleSegments = 0;
    int paddleThickness = 5;
    Vector2 playerDimensions = { (int) 300, (int) 50 };
    Vector2 player = { (float) (screenWidth/2 - playerDimensions.x/2), (float) (screenHeight - playerDimensions.y - (paddleThickness) * 2) };

    Vector2 ballDimensions = { (int) 40, (int) 40 };
    Vector2 ball = { (float) (screenWidth/2 - ballDimensions.x/2), (float) (screenHeight/2 - ballDimensions.y/2)};
    const int initialBallVelocity = 10;
    Vector2 ballVelocity = { (float) initialBallVelocity, (float) -initialBallVelocity};

    int gameState = 1;
    int score = 0;
    /* 0 - menu, 1 - playing */
    int number = 0;

    InitWindow(screenWidth, screenHeight, "eliot chignell");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        /* Gradient background */
        ClearBackground(colour1);

        if (moving) { // Changing colour to fade to background
            Color currentColour = {colour3.r, colour3.g, colour3.b, (abs((int) velocity) / initialVelocity) * 255};
            if (direction) DrawRectangleGradientH(player.x, player.y, playerDimensions.x, playerDimensions.y, currentColour, colour1);
            if (!direction) DrawRectangleGradientH(player.x, player.y, playerDimensions.x, playerDimensions.y, colour1, currentColour);
        }

        Rectangle paddle = {player.x, player.y, playerDimensions.x, playerDimensions.y};
        /* DrawRectangleLines(player.x, player.y, playerDimensions.x, playerDimensions.y, WHITE); */
        DrawRectangleRoundedLines(paddle, paddleRoundness, paddleSegments, paddleThickness, colour4); 
        DrawCircle(ball.x, ball.y, ballDimensions.x/2, colour5);

        player.x += velocity;

        ball.x += ballVelocity.x;
        ball.y += ballVelocity.y;

        if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W)) {
            if (direction) velocity = initialVelocity;
            if (!direction) velocity = -initialVelocity;

            moving = 1;
        }

        if (direction && (player.x + playerDimensions.x) >= screenWidth) {
            direction = 0;
            velocity = -velocity;
        }

        if (!direction && player.x <= 0) {
            direction = 1;
            velocity = -velocity;
        }

        if (moving) {
            if (direction) velocity -= deceleration;
            if (!direction) velocity += deceleration;

            if (velocity <= 0.00001 && velocity >= -0.00001) {
                moving = 0;
                velocity = 0;
            }
        }

        /* Ball collisions with walls*/ 
        if ((ball.x - ballDimensions.x/2) < 0) ballVelocity.x = -ballVelocity.x;
        if ((ball.x + ballDimensions.x/2) > screenWidth) ballVelocity.x = -ballVelocity.x;

        if ((ball.y - ballDimensions.y/2) < 0) {
            srand(time(NULL));
            float randomVelocityX = 4 + (rand() % 8);
            float randomVelocityY = sqrt(200 - pow(randomVelocityX, 2));

            if (ballVelocity.x > 0) {
                ballVelocity.x = randomVelocityX;
            } else {
                ballVelocity.x = -randomVelocityX;
            }

            if (ballVelocity.y > 0) {
                ballVelocity.y = -randomVelocityY;
            } else {
                ballVelocity.y = randomVelocityY;
            }

            ball.y = ballDimensions.y/2;
        }

        /* Colliding with paddle on face */
        if ((ball.x - ballDimensions.x/2) > (player.x - paddleThickness) &&
            (ball.x + ballDimensions.x/2) < (player.x + playerDimensions.x + paddleThickness) &&
            (ball.y + ballDimensions.y/2) > (player.y - paddleThickness)) {
            ball.y = paddle.y - paddleThickness - ballDimensions.y/2;
            ballVelocity.y = -ballVelocity.y;
            score++;
            printf("%d\n", score);
        }

        /* Colliding with paddle on sides */
        if (((ball.x + ballDimensions.x/2) > player.x || (ball.x - ballDimensions.x/2) < (player.x + playerDimensions.x + paddleThickness)) &&
            (ball.y + ballDimensions.y/2) > (player.y + playerDimensions.y + paddleThickness)) {
            ballVelocity.x = -ballVelocity.x;
        }

        /* User loses */
        if ((ball.y + ballDimensions.y/2) >= (player.y + playerDimensions.y)) {
            initialVelocity = 10;
            deceleration = 0.1;
            velocity = 0;
            direction = 1;
            moving = 0;
            score = 0;

            player.x = (screenWidth/2 - playerDimensions.x/2);
            player.y = (screenHeight - playerDimensions.y - (paddleThickness) * 2);

            ball.x = (screenWidth/2 - ballDimensions.x/2);
            ball.y = (screenHeight/2 - ballDimensions.y/2);
            ballVelocity.x = initialBallVelocity;
            ballVelocity.y = -initialBallVelocity;
        }

        EndDrawing();
    }

    CloseWindow();
}