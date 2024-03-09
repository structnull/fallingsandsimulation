#include <raylib.h>

#define sWidth 800
#define sHeight 800
#define cellSize 10

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
			nextgrid[i][j] = grid[i][j];
		}
	}

	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++)
		{
			int state = grid[i][j];
			if (state == 1)
			{
				// Check if we are not at the last row(fixed out of bounds )
				if (i < rows - 1)
				{
					// Check below
					if (grid[i + 1][j] == 0)
					{
						nextgrid[i][j] = 0;
						nextgrid[i + 1][j] = 1;
					}
					// Check below right(huh?)
					else if (j < cols - 1 && grid[i + 1][j + 1] == 0)
					{
						nextgrid[i][j + 1] = 1;
					}
					// Check below left
					else if (j > 0 && grid[i + 1][j - 1] == 0)
					{
						nextgrid[i][j - 1] = 1;
					}
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
		ClearBackground(RAYWHITE);
		handleMouseInput();
		drawGrid();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
