// Vanhaverbeke, Brenainn - 1DAE10
// Duterte, Tom - 1DAE11

#include "pch.h"
#include "Game.h"
#include <iostream>

//Basic game functions
#pragma region gameFunctions

void Start()
{
	InitialiseBorder();
	InitialiseTanks();
	InitBackground();
	InitTile();
	InitializeObstacle();


}

void Draw()
{

	ClearBackground();
	DrawBackground();
	DrawGrid();
	DrawTanks();

	//DrawBarrel();
}

void Update(float elapsedSec)
{

}

void End()
{
	EndBackground();
	EndTile();
	// free game resources here
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{

}

void OnKeyUpEvent(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_LEFT:
	case SDLK_a:
		MoveTank(TankOrientation::LEFT);
		break;
	case SDLK_RIGHT:
	case SDLK_d:
		MoveTank(TankOrientation::RIGHT);
		break;
	case SDLK_DOWN:
	case SDLK_s:
		MoveTank(TankOrientation::DOWN);
		break;
	case SDLK_UP:
	case SDLK_w:
		MoveTank(TankOrientation::UP);
		break;
	}
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	g_MousePosition = Point2f{ (float)e.x, g_WindowHeight - (float)e.y };
	CalculateBarrelAngle(g_MousePosition);
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{

}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{

}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here

void InitBackground()
{
	bool isCreationOk{ TextureFromFile("Resources/Grass.png", g_Background) };
	if (!isCreationOk)
		std::cout << "Error loading Grass.png" << std::endl;
}

void InitTile()
{
	bool isCreationOk{ TextureFromFile("Resources/Block.png", g_Tile) };
	if (!isCreationOk)
		std::cout << "Error loading texture Block.png" << std::endl;
}

void InitialiseBorder()
{
	for (int row{ 0 }; row < g_GridRows; row++)
	{
		for (int column{ 0 }; column < g_GridColumns; column++)
		{
			if (row == 0 || row == g_GridRows - 1 || column == 0 || column == g_GridColumns - 1)
			{
				int index{ GetLinearIndexFrom2D(row, column, g_GridColumns) };
				g_IsCellFree[index] = false;
			}
		}
	}
}

void InitialiseTanks()
{
	const int initialHealth{ 3 };
	Tank& tank = g_Tanks[0];
	tank.columnIndex = 1;
	tank.rowIndex = g_GridRows - 2;
	tank.health = initialHealth;
	int tankLocationIndex{ GetLinearIndexFrom2D(tank.rowIndex, tank.columnIndex, g_GridColumns) };
	g_IsCellFree[tankLocationIndex] = false;
	Tank& tank2 = g_Tanks[1];
	tank2.columnIndex = g_GridColumns - 2;
	tank2.rowIndex = 1;
	tank2.health = initialHealth;
	int tank2LocationIndex{ GetLinearIndexFrom2D(tank2.rowIndex, tank2.columnIndex, g_GridColumns) };
	g_IsCellFree[tank2LocationIndex] = false;
}

void MoveTank(TankOrientation direction)
{
	int xDisplacement{};
	int yDisplacement{};
	Tank& tank{ g_Tanks[g_TurnCounter] };
	switch (direction)
	{
	case TankOrientation::UP:
		yDisplacement = 1;
		break;
	case TankOrientation::DOWN:
		yDisplacement = -1;
		break;
	case TankOrientation::LEFT:
		xDisplacement = -1;
		break;
	case TankOrientation::RIGHT:
		xDisplacement = 1;
		break;
	}
	int tankLocationIndex{ GetLinearIndexFrom2D(tank.rowIndex, tank.columnIndex, g_GridColumns) };
	int tankDestinationIndex{ GetLinearIndexFrom2D(tank.rowIndex + yDisplacement, tank.columnIndex + xDisplacement, g_GridColumns) };
	if (g_IsCellFree[tankDestinationIndex])
	{
		tank.columnIndex += xDisplacement;
		tank.rowIndex += yDisplacement;
		g_IsCellFree[tankLocationIndex] = true;
		g_IsCellFree[tankDestinationIndex] = false;
	}
	g_TurnCounter = (g_TurnCounter + 1) % g_AmountOfPlayers;
}

void CalculateBarrelAngle(const Point2f& mousePosition)
{
	Tank& activeTank{ g_Tanks[g_TurnCounter] };
	Rectf tankRectangle{ g_SideLength * activeTank.columnIndex, g_SideLength * activeTank.rowIndex, g_SideLength, g_SideLength };

	Point2f tankCenter{ GetCenterOfRectangle(tankRectangle) };
	float barrelAngle{ GetBarrelAngle(tankCenter, mousePosition) };
	activeTank.barrelAngle = barrelAngle;
}

void InitializeObstacle()
{
	int nbrOfObstacles{ 20 };

	for (int i{}; i < nbrOfObstacles; i++)
	{
		int rdmRow{ rand() % (g_GridRows - 2) + 1 };
		int rdmColumn{ rand() % (g_GridColumns - 2) + 1 };

		int index{ GetLinearIndexFrom2D(rdmRow, rdmColumn, g_GridColumns) };
		if (g_IsCellFree[index] == false) --i;
		g_IsCellFree[index] = false;
	}


	int index{ GetLinearIndexFrom2D(5, 5, g_GridColumns) };
	g_IsCellFree[index] = false;
};

void DrawGrid()
{
	const Color4f toggledOn{ 1.0f, 0.4f, 0.2f, 1.0f };
	const Color4f togledOff{ 0.8f, 0.8f, 0.8f, 0.0f };
	for (int row{ 0 }; row < g_GridRows; row++)
	{
		for (int column{ 0 }; column < g_GridColumns; column++)
		{
			Point2f bottomLeft{ g_SideLength * column, g_SideLength * row };
			int index{ GetLinearIndexFrom2D(row, column, g_GridColumns) };
			SetColor(togledOff);
			if (!g_IsCellFree[index])
			{
				if (row == 0 || row == g_GridRows - 1 || column == 0 || column == g_GridColumns - 1)
				{
					Rectf size{ 0,128, g_SideLength, g_SideLength };
					DrawTexture(g_Tile, bottomLeft, size);
				}
				else SetColor(toggledOn);
			}
			FillRect(bottomLeft, g_SideLength, g_SideLength);
			SetColor(g_White);
			DrawRect(bottomLeft, g_SideLength, g_SideLength);
		}
	}
}

void DrawTanks()
{
	const Color4f tankColour{ 0.0f, 0.75f, 0.0f, 1.0f };
	const Color4f barrelColour{ 0.0f, 0.0f, 0.0f, 1.0f };
	const float barrelLength{ 60.0f };
	Rectf tankDestination{ 0, 0, g_SideLength, g_SideLength };
	for (int player = 0; player < g_AmountOfPlayers; player++)
	{
		Tank& tank{ g_Tanks[player] };
		tankDestination.left = tank.columnIndex * g_SideLength;
		tankDestination.bottom = tank.rowIndex * g_SideLength;
		SetColor(tankColour);
		FillRect(tankDestination);
		Point2f tankCenter{ GetCenterOfRectangle(tankDestination) };
		Point2f barrelEnd{ GetCoordinatesFromRadians(barrelLength, tank.barrelAngle, tankCenter) };
		SetColor(barrelColour);
		DrawLine(tankCenter, barrelEnd, 3.0f);
	}
}

int GetLinearIndexFrom2D(int rowIndex, int columnIndex, int nrOfColumns)
{
	return rowIndex * nrOfColumns + columnIndex;
}



void DrawBackground()
{
	Point2f bottom{ 0,0 };
	DrawTexture(g_Background, bottom);
}

void EndBackground()
{
	DeleteTexture(g_Background);
}



void EndTile()
{
	DeleteTexture(g_Tile);
}

#pragma endregion ownDefinitions