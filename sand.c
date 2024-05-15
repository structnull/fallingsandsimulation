#include <raylib.h>
#include <stdlib.h>

#define cellSize 5

int *grid;
int *nextgrid;
int rows, cols;

void initializeGrid(int screenWidth, int screenHeight)
{
	rows = screenHeight / cellSize;
	cols = screenWidth / cellSize;

	// Allocate memory for grids
	grid = (int *)malloc(rows * cols * sizeof(int));
	nextgrid = (int *)malloc(rows * cols * sizeof(int));
	for (int i = 0; i < rows * cols; i++)
	{
		grid[i] = 0;
		nextgrid[i] = 0;
	}
}

void resizeGrid(int newCols, int newRows)
{
	int *newGrid = (int *)malloc(newRows * newCols * sizeof(int));
	int *newNextgrid = (int *)malloc(newRows * newCols * sizeof(int));

	for (int i = 0; i < newRows; i++)
	{
		for (int j = 0; j < newCols; j++)
		{
			if (i < rows && j < cols)
			{
				newGrid[i * newCols + j] = grid[i * cols + j];
			}
			else
			{
				newGrid[i * newCols + j] = 0;
			}
			newNextgrid[i * newCols + j] = 0;
		}
	}

	free(grid);
	free(nextgrid);

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
			if (grid[i * cols + j] == 1)
			{
				int belowA = 0, belowB = 0, below = 0;
				if (i + 1 < rows)
				{
					if (j - 1 >= 0)
						belowA = grid[(i + 1) * cols + (j - 1)];
					if (j + 1 < cols)
						belowB = grid[(i + 1) * cols + (j + 1)];
					below = grid[(i + 1) * cols + j];
				}

				if (i + 1 < rows && below == 0)
				{
					nextgrid[i * cols + j] = 0;
					nextgrid[(i + 1) * cols + j] = 1;
				}
				else if (i + 1 < rows && belowA == 0 && j - 1 >= 0)
				{
					nextgrid[i * cols + j] = 0;
					nextgrid[(i + 1) * cols + (j - 1)] = 1;
				}
				else if (i + 1 < rows && belowB == 0 && j + 1 < cols)
				{
					nextgrid[i * cols + j] = 0;
					nextgrid[(i + 1) * cols + (j + 1)] = 1;
				}
				else
				{
					nextgrid[i * cols + j] = 1;
				}
			}
		}
	}

	for (int i = 0; i < rows * cols; i++)
	{
		grid[i] = nextgrid[i];
		nextgrid[i] = 0;
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Rectangle cell = {j * cellSize, i * cellSize, cellSize, cellSize};
			if (grid[i * cols + j] == 0)
				DrawRectangle(cell.x, cell.y, cell.width, cell.height, BLACK);
			else if (grid[i * cols + j] == 1)
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
					grid[ny * cols + nx] = 1;
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

	free(grid);
	free(nextgrid);
	CloseWindow();
	return 0;
}
