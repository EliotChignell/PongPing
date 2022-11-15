/* Raylib game by Eliot Chignell */

#include "raylib/src/raylib.h" // path to raylib.h
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "string.h"

Color colour1 = {23, 24, 41, 255};
Color colour1b = {51 * 2, 55 * 2, 69 * 2, 255};
Color colour2 = {230, 52, 98, 255};
Color colour3 = {254, 95, 85, 255};
Color colour4 = {68, 157, 209, 255};
Color colour5 = {128, 255, 232, 255};

int main(void) {
    // Core variables
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    // Constants
    float initialVelocity = 10;
    float deceleration = 0.1;
    float velocity = 0;
    bool direction = 1;
    bool moving = 0;

    // Paddle variables
    float paddleRoundness = 0.5f;
    int paddleSegments = 0;
    int paddleThickness = 5;
    Vector2 playerDimensions = { (int) 300, (int) 50 };
    Vector2 player = { (float) (screenWidth/2 - playerDimensions.x/2), (float) (screenHeight - playerDimensions.y - (paddleThickness) * 2) };

    // Ball variables
    Vector2 ballDimensions = { (int) 40, (int) 40 };
    Vector2 ball = { (float) (screenWidth/2 - ballDimensions.x/2), (float) (screenHeight/2 - ballDimensions.y/2)};
    const int initialBallVelocity = 10;
    Vector2 ballVelocity = { (float) initialBallVelocity, (float) -initialBallVelocity};

    ChangeDirectory(GetApplicationDirectory()); 

    int gameState = 0;
    int score = 0;
    /* 0 - menu, 1 - playing, 2 - game over */
    int number = 0;

    InitWindow(screenWidth, screenHeight, "PongPing");
    SetExitKey(0);
    bool exitWindow = false;
    
    // Fonts
    Font Righteous = LoadFontEx("Righteous-Regular.ttf", 96, 0, 0);

    SetTargetFPS(60);

    while (!exitWindow) {
        BeginDrawing();

        if (WindowShouldClose()) exitWindow = true;
        
        switch (gameState) {
            case 0: // Menu
                ClearBackground(colour1);

                const char title[] = "PongPing";
                const char sub[] = "Pong with a twist";
                const char text1[] = "Press P to play";
                const char text2[] = "Press ESC to quit";
                Vector2 titlePosition, subPosition, t1pos, t2pos;
                titlePosition.x = screenWidth/2 - MeasureTextEx(Righteous, title, 100.0f, 3.0f).x/2;
                titlePosition.y = 150.0f;
                subPosition.x = screenWidth/2 - MeasureTextEx(Righteous, sub, 50.0f, 3.0f).x/2;
                subPosition.y = 250.0f;
                t1pos.x = screenWidth/2 - MeasureTextEx(Righteous, text1, 40.0f, 3.0f).x/2;
                t1pos.y = 400.0f;
                t2pos.x = screenWidth/2 - MeasureTextEx(Righteous, text2, 40.0f, 3.0f).x/2;
                t2pos.y = 450.0f;
                DrawTextEx(Righteous, title, titlePosition, 100.0f, 3.0f, colour2);
                DrawRectangle(titlePosition.x - 20, titlePosition.y + 10, 10, 80, colour4);
                DrawRectangle(screenWidth/2 + (screenWidth/2 - titlePosition.x) + 10, titlePosition.y + 10, 10, 80, colour4);
                DrawTextEx(Righteous, sub, subPosition, 50.0f, 3.0f, colour4);
                DrawTextEx(Righteous, text1, t1pos, 40.0f, 3.0f, colour5);
                DrawTextEx(Righteous, text2, t2pos, 40.0f, 3.0f, colour5);

                if (IsKeyDown(KEY_P)) {
                    gameState = 1;
                }

                if (IsKeyDown(KEY_ESCAPE)) {
                    exitWindow = true;
                }
                break;
                
            case 1: // Playing
                /* Gradient background */
                ClearBackground(colour1);

                if (moving) { // Changing colour to fade to background
                    Color currentColour = {colour4.r, colour4.g, colour4.b, (abs((int) velocity) / initialVelocity) * 255};
                    if (direction) DrawRectangleGradientH(player.x, player.y, playerDimensions.x, playerDimensions.y, currentColour, colour1);
                    if (!direction) DrawRectangleGradientH(player.x, player.y, playerDimensions.x, playerDimensions.y, colour1, currentColour);
                }

                Rectangle paddle = {player.x, player.y, playerDimensions.x, playerDimensions.y};
                /* DrawRectangleLines(player.x, player.y, playerDimensions.x, playerDimensions.y, WHITE); */
                DrawRectangleRoundedLines(paddle, paddleRoundness, paddleSegments, paddleThickness, colour4);
                DrawCircle(ball.x, ball.y, ballDimensions.x/2, colour5);

                const char* scoreMsg = "Score: ";
                char *num = malloc(100 * sizeof(char));
                char buffer[100];

                if (sprintf(num, "%d", score) == -1) {
                    perror("asprintf");
                } else {
                    strcat(strcpy(buffer, scoreMsg), num);
                }
                
                free(num);

                Vector2 scoreMsgPosition = { 10, 10 };
                Vector2 tipMsgPosition = { 1020, 10 };
                DrawTextEx(Righteous, buffer, scoreMsgPosition, 45, 3, colour3);
                DrawTextEx(Righteous, "Press SPACE", tipMsgPosition, 45, 3, colour3);

                player.x += velocity;

                ball.x += ballVelocity.x;
                ball.y += ballVelocity.y;

                if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W)) {
                    if (direction) {
                        velocity = initialVelocity;
                    }
                    if (!direction) {
                        velocity = -initialVelocity;
                    }
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
                    // srand(time(NULL));
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
                }

                /* Colliding with paddle on sides */
                if (((ball.x + ballDimensions.x/2) > player.x || (ball.x - ballDimensions.x/2) < (player.x + playerDimensions.x + paddleThickness)) &&
                    (ball.y + ballDimensions.y/2) > (player.y + playerDimensions.y + paddleThickness)) {
                    ballVelocity.x = -ballVelocity.x;
                }

                /* User loses */
                if ((ball.y + ballDimensions.y/2) >= (player.y + playerDimensions.y)) {
                    gameState = 2;
                    
                    /* reset game - initialVelocity = 10;
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
                    ballVelocity.y = -initialBallVelocity; */
                }
                break;
                
            case 2: // Game over
                ClearBackground(colour1);

                const char gameOverMsg[] = "Game Over!";
                // const char msg0[] = "Score: 99";
                const char msg1[] = "Press P to play again";
                const char msg2[] = "Press ESC to quit";

                const char* scoreMsg1 = "Score: ";
                char *num1 = malloc(100 * sizeof(char));
                char msg0[100];

                if (sprintf(num1, "%d", score) == -1) {
                    perror("asprintf");
                } else {
                    strcat(strcpy(msg0, scoreMsg1), num1);
                }

                free(num1);

                Vector2 gopos, m0pos, m1pos, m2pos;
                gopos.x = screenWidth/2 - MeasureTextEx(Righteous, gameOverMsg, 96.0f, 3.0f).x/2;
                gopos.y = 100.0f;
                m0pos.x = screenWidth/2 - MeasureTextEx(Righteous, msg0, 50.0f, 3.0f).x/2;
                m0pos.y = 200.0f;
                m1pos.x = screenWidth/2 - MeasureTextEx(Righteous, msg1, 40.0f, 3.0f).x/2;
                m1pos.y = 400.0f;
                m2pos.x = screenWidth/2 - MeasureTextEx(Righteous, msg2, 40.0f, 3.0f).x/2;
                m2pos.y = 450.0f;

                DrawTextEx(Righteous, gameOverMsg, gopos, 96, 3, colour2);
                DrawTextEx(Righteous, msg0, m0pos, 50, 3, colour4);
                DrawTextEx(Righteous, msg1, m1pos, 40, 3, colour5);
                DrawTextEx(Righteous, msg2, m2pos, 40, 3, colour5);

                if (IsKeyDown(KEY_P)) {
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

                    gameState = 1;
                }

                if (IsKeyDown(KEY_ESCAPE)) {
                    exitWindow = true;
                }
                
                break;
        }

        EndDrawing();
    }
    
    UnloadFont(Righteous);

    CloseWindow();
}
