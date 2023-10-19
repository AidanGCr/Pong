#include <raylib.h>
#include <raymath.h>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdateHitbox(Vector4 *hitbox, Vector2 position, Vector2 size);
Vector2 GetRandomVector(float scale);
void ResetBall(Vector2 *position, Vector2 *vector, int height, int width);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    srand(time(NULL));
    bool endScreen = false;
    const int screenWidth = 800;
    const int screenHeight = 450;
    const Vector2 rectSize = { (float)20, (float)100 };
    const int ballSize = 10;

    InitWindow(screenWidth, screenHeight, "Pong!");

    //Player locations
    Vector2 rectPosition1 = { (float)10, (float)screenHeight/2 - rectSize.y/2 };  
    Vector2 rectPosition2 = { (float)screenWidth - 10 - rectSize.x, (float)screenHeight/2 - rectSize.y/2 };   
    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };
    Vector2 ballVector = GetRandomVector(6.0); 
    //Hitboxes for collisions
    Vector4 rectHitbox1 = { 
		(float)rectPosition1.x - rectSize.x/2, 
		(float)rectPosition1.x + rectSize.x/2,  
		(float)rectPosition1.y - rectSize.y/2, 
		(float)rectPosition1.y + rectSize.y/2,  
	}; 
    Vector4 rectHitbox2 = { 
		(float)rectPosition2.x - rectSize.x/2, 
		(float)rectPosition2.x + rectSize.x/2,  
		(float)rectPosition2.y - rectSize.y/2, 
		(float)rectPosition2.y + rectSize.y/2,  
	};  
    //Scores
    int leftScore = 0;
    int rightScore = 0;
    char leftScoreStr[3];
    char rightScoreStr[3];

    SetTargetFPS(60);               // Set game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    { 
	if (IsKeyDown(KEY_SPACE) && endScreen) exit(0);
        // Update
        //---------------------------------------------------------------------------------- 
	// Move player
        if (IsKeyDown(KEY_W) && rectPosition1.y > 0) rectPosition1.y -= 4.0f;
        if (IsKeyDown(KEY_S) && rectPosition1.y < screenHeight - rectSize.y) rectPosition1.y += 4.0f;
        if (IsKeyDown(KEY_UP) && rectPosition2.y > 0) rectPosition2.y -= 4.0f;
        if (IsKeyDown(KEY_DOWN) && rectPosition2.y < screenHeight - rectSize.y) rectPosition2.y += 4.0f;
	if (IsKeyDown(KEY_R)) ResetBall(&ballPosition, &ballVector, screenHeight, screenWidth);
	// Update hitboxes
	UpdateHitbox(&rectHitbox1, rectPosition1, rectSize); 
	UpdateHitbox(&rectHitbox2, rectPosition2, rectSize);  
	// Move ball
	ballPosition.x += ballVector.x; 
	ballPosition.y += ballVector.y; 
	// Check for collisions with walls
	if (ballPosition.y > screenHeight - ballSize) {
	    Vector2 normal = { (float)0, (float)1 }; 
	    ballVector = Vector2Reflect(ballVector, normal); 
	} else if (ballPosition.y < ballSize) { 
	    Vector2 normal = { (float)0, (float)1 }; 
	    ballVector = Vector2Reflect(ballVector, normal); 
	}
	// Check for collision with left player
	if (ballPosition.x - ballSize <= rectHitbox1.y && ballPosition.y <= rectHitbox1.z && ballPosition.y >= rectHitbox1.w) {
	    Vector2 normal = { (float)1, (float)0 }; 
	    ballVector = Vector2Reflect(ballVector, normal);
	// TODO implement side collision for left player
	} else if (false) { 
	} else if (false) { 
	} 
	// Check for collision with right player 
	if (ballPosition.x + ballSize >= rectHitbox2.x && ballPosition.y <= rectHitbox2.z && ballPosition.y >= rectHitbox2.w) {
	    Vector2 normal = { (float)1, (float)0 }; 
	    ballVector = Vector2Reflect(ballVector, normal); 
	// TODO implement side collision for right player
	} else if (false) { 
	} else if (false) { 
	} 
	// Scoring and resetting the ball
	if (ballPosition.x <= 0) {
	    rightScore++;
	    ResetBall(&ballPosition, &ballVector, screenHeight, screenWidth);
	} else if (ballPosition.x >= screenWidth) {
	    leftScore++;  
	    ResetBall(&ballPosition, &ballVector, screenHeight, screenWidth);
	} 
	sprintf(leftScoreStr, "%d", leftScore);
	sprintf(rightScoreStr, "%d", rightScore);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
	if (leftScore == 10 || rightScore == 10) {
	    EnableEventWaiting(); 
	    endScreen = true; 

            ClearBackground(RAYWHITE);

	    const char* text = "GAME OVER";
	    int textSize = MeasureText(text, 50);
	    Vector2 textPosition = {(screenWidth - textSize)/2, screenHeight/2};
	    const char* text2 = "Press <SPACE> to exit"; 
	    textSize = MeasureText(text2, 30);
	    Vector2 textPosition2 = {(screenWidth - textSize)/2, screenHeight/2 + 50};

	    DrawText("GAME OVER", textPosition.x, textPosition.y, 50, RED);  
	    DrawText("Press <SPACE> to exit", textPosition2.x, textPosition2.y, 30, DARKGRAY);  
	    DrawText("Left: ", 10, 10, 20, DARKGRAY);  
	    DrawText(leftScoreStr, 70, 10, 20, DARKGRAY);  
	    DrawText("Right: ", screenWidth - 100, 10, 20, DARKGRAY);  
	    DrawText(rightScoreStr, screenWidth - 30, 10, 20, DARKGRAY);    

	} else { 
            ClearBackground(RAYWHITE);

            DrawRectangleV(rectPosition1, rectSize, BLACK);
            DrawRectangleV(rectPosition2, rectSize, BLACK); 
	    DrawCircleV(ballPosition, ballSize, BLACK); 

	    DrawText("Left: ", 10, 10, 20, DARKGRAY);  
	    DrawText(leftScoreStr, 70, 10, 20, DARKGRAY);  
	    DrawText("Right: ", screenWidth - 100, 10, 20, DARKGRAY);  
	    DrawText(rightScoreStr, screenWidth - 30, 10, 20, DARKGRAY);   
	} 
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Helper functions ------------------------------------------------------------------------

void UpdateHitbox(Vector4 *hitbox, Vector2 position, Vector2 size) {  
    hitbox->x = position.x; 
    hitbox->y = position.x + size.x;
    hitbox->z = position.y + size.y; 
    hitbox->w = position.y; 
} 

Vector2 GetRandomVector(float scale) {
    int min = -10;
    int max = 10; 
    int xDirection = rand() % (max - min + 1) + min;
    int yDirection = rand() % (max - min + 1) + min;
    Vector2 vector = { (float)xDirection, (float)yDirection }; 
    vector = Vector2Normalize(vector); 
    vector = Vector2Scale(vector, scale);
    return vector;
}

void ResetBall(Vector2 *position, Vector2 *vector, int height, int width) {  
    position->x = (float)width/2; 
    position->y = (float)height/2;
    Vector2 tempVector = GetRandomVector(6.0);
    vector->x = tempVector.x; 
    vector->y = tempVector.y;
} 
