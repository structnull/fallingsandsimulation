#include <raylib.h>
#include <stdlib.h>

#define cellSize 5

int** grid;
int** nextgrid;
int rows, cols;

void initializeGrid(int screenWidth, int screenHeight)
{
    rows = screenHeight / cellSize;
    cols = screenWidth / cellSize;

    // Allocating  memory for grids
    grid = (int**)malloc(rows * sizeof(int*));
    nextgrid = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
    {
        grid[i] = (int*)malloc(cols * sizeof(int));
        nextgrid[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++)
        {
            grid[i][j] = 0;
            nextgrid[i][j] = 0;
        }
    }
}

void freeGrid()
{
    for (int i = 0; i < rows; i++)
    {
        free(grid[i]);
        free(nextgrid[i]);
    }
    free(grid);
    free(nextgrid);
}

void resizeGrid(int newCols, int newRows)
{
    int** newGrid = (int**)malloc(newRows * sizeof(int*));
    int** newNextgrid = (int**)malloc(newRows * sizeof(int*));
    for (int i = 0; i < newRows; i++)
    {
        newGrid[i] = (int*)malloc(newCols * sizeof(int));
        newNextgrid[i] = (int*)malloc(newCols * sizeof(int));
        for (int j = 0; j < newCols; j++)
        {
            newGrid[i][j] = 0;
            newNextgrid[i][j] = 0;
        }
    }

    float rowScale = (float)rows / newRows;
    float colScale = (float)cols / newCols;

    for (int i = 0; i < newRows; i++)
    {
        for (int j = 0; j < newCols; j++)
        {
            int oldI = i * rowScale;
            int oldJ = j * colScale;
            if (oldI < rows && oldJ < cols)
            {
                newGrid[i][j] = grid[oldI][oldJ];
            }
        }
    }

    freeGrid();

    grid = newGrid;
    nextgrid = newNextgrid;
    cols = newCols;
    rows = newRows;
}

void drawGrid()
{
    for (int i = rows - 1; i >= 0; i--)
    {
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j] == 1)
            {
                int belowA = 0, belowB = 0, below = 0;
                if (i + 1 < rows)
                {
                    if (j - 1 >= 0)
                        belowA = grid[i + 1][j - 1];
                    if (j + 1 < cols)
                        belowB = grid[i + 1][j + 1];
                    below = grid[i + 1][j];
                }

                if (i + 1 < rows && below == 0)
                {
                    nextgrid[i][j] = 0;
                    nextgrid[i + 1][j] = 1;
                }
                else if (i + 1 < rows && belowA == 0 && j - 1 >= 0)
                {
                    nextgrid[i][j] = 0;
                    nextgrid[i + 1][j - 1] = 1;
                }
                else if (i + 1 < rows && belowB == 0 && j + 1 < cols)
                {
                    nextgrid[i][j] = 0;
                    nextgrid[i + 1][j + 1] = 1;
                }
                else
                {
                    nextgrid[i][j] = 1;
                }
            }
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            grid[i][j] = nextgrid[i][j];
            nextgrid[i][j] = 0;
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            Rectangle cell = {j * cellSize, i * cellSize, cellSize, cellSize};
            if (grid[i][j] == 0)
                DrawRectangle(cell.x, cell.y, cell.width, cell.height, BLACK);
            else if (grid[i][j] == 1)
                DrawRectangle(cell.x, cell.y, cell.width, cell.height, YELLOW);
        }
    }
}

void handleMouseInput()
{
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        Vector2 pos = GetMousePosition();
        int x = pos.x / cellSize;
        int y = pos.y / cellSize;
        int range = 2;

        for (int dy = -range; dy <= range; ++dy)
        {
            for (int dx = -range; dx <= range; ++dx)
            {
                int nx = x + dx;
                int ny = y + dy;

                int offsetX = rand() % (range * 2 + 1) - range;
                int offsetY = rand() % (range * 2 + 1) - range;

                nx += offsetX;
                ny += offsetY;

                if (nx >= 0 && nx < cols && ny >= 0 && ny < rows)
                {
                    grid[ny][nx] = 1;
                }
            }
        }
    }
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Falling sand simulator"); 
    SetTargetFPS(60);

    initializeGrid(GetScreenWidth(), GetScreenHeight());

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            resizeGrid(GetScreenWidth() / cellSize, GetScreenHeight() / cellSize);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        handleMouseInput();
        drawGrid();
        EndDrawing();
    }

    freeGrid();
    CloseWindow();
    return 0;
}
