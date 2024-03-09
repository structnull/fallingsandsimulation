#include <raylib.h>

#define sWidth 800
#define sHeight 800
#define cellSize 10

#define rows (sHeight / cellSize)
#define cols (sWidth / cellSize)

int grid[rows][cols];
int nextgrid[rows][cols];

void initializeGrid() {
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
				if (i < rows - 1 && grid[i + 1][j] == 0) 
				{
					nextgrid[i][j] = 0;
					nextgrid[i + 1][j] = 1; 
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

int main()
{
	InitWindow(sWidth, sHeight, "Falling sand simulator");
	SetTargetFPS(60);

	initializeGrid();

	grid[10][20] = 1;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		drawGrid();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
