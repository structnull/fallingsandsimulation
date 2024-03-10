#include <raylib.h>
#include <stdlib.h>

#define sWidth 800
#define sHeight 800
#define cellSize 3

#define rows (sHeight / cellSize)
#define cols (sWidth / cellSize)

int grid[rows][cols];
int nextgrid[rows][cols];

void initializeGrid()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j] = 0;
			nextgrid[i][j] = 0;
		}
	}
}

void drawGrid()
{

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int state = grid[i][j];
			if (state == 1)
			{
				// Initialize belowA and belowB
				int belowA = 0, belowB = 0;
				int r = 1;
				if (rand() % 100 + 1 < 50)
				{
					r *= -1;
				}
				if (i + r >= 0 && i + r <= rows - 1 && j + 1 < cols)
				{
					belowA = grid[i + r][j + 1];
				}
				if (i + r >= 0 && i + r <= rows - 1 && j - 1 >= 0)
				{
					belowB = grid[i + r][j - 1];
				}

				if (i + 1 < rows && grid[i + 1][j] == 0)							  // check for cells below within the bounds of grid
				{
					nextgrid[i][j] = 0;
					nextgrid[i + 1][j] = 1;
				}
				else if (i + 1 < rows && belowA == 0 && j + 1 < cols)
				{
					nextgrid[i + 1][j + 1] = 1;
				}
				else if (i + 1 < rows && belowB == 0 && j - 1 >= 0)
				{
					nextgrid[i + 1][j - 1] = 1;
				}
			}
		}
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j] = nextgrid[i][j];
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
		Vector2 mousePosition = GetMousePosition();
		int gridX = mousePosition.x / cellSize;
		int gridY = mousePosition.y / cellSize;
		if (gridX >= 0 && gridX < cols && gridY >= 0 && gridY < rows)
		{
			grid[gridY][gridX] = 1;
		}
	}
}

int main()
{
	InitWindow(sWidth, sHeight, "Falling sand simulator");
	SetTargetFPS(60);

	initializeGrid();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		handleMouseInput();
		drawGrid();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
