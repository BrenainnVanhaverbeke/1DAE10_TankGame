// Vanhaverbeke, Brenainn - 1DAE10
// Duterte, Tom - 1DAE11

#include "pch.h"
#include "Game.h"
#include <iostream>

//Basic game functions
#pragma region gameFunctions

void Start()
{
	InitialiseGrid();
	InitialiseBorder();
	InitialiseTanks();
	InitialiseBackground();
	InitialiseTiles();
	//InitialiseObstacles(g_Tanks);
}

void Draw()
{
	ClearBackground();
	DrawBackground();
	DrawGrid();
	DrawTanks();
	DrawInstructions();
}

void Update(float elapsedSec)
{

}

void End()
{
	EndBackground();
	EndTile();
	delete[] g_Tanks;
	delete[] g_Grid;
	g_Tanks = nullptr;
	g_Grid = nullptr;
}

#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key) {}

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

#pragma region Initialisers

void InitialiseBackground()
{
	bool isCreationOk{ TextureFromFile("Resources/Grass.png", g_Background) };
	if (!isCreationOk)
		std::cout << "Error loading Grass.png" << std::endl;
}

void InitialiseTiles()
{
	bool isCreationOk{ TextureFromFile("Resources/Block.png", g_Tile) };
	if (!isCreationOk)
		std::cout << "Error loading texture Block.png" << std::endl;
}

void InitialiseGrid()
{
	for (int row = 0; row < g_GridRows; row++)
	{
		for (int column = 0; column < g_GridColumns; column++)
		{
			Rectf cell{ g_SideLength * column, g_SideLength * row, g_SideLength, g_SideLength };
			Index2D index{ column, row };
			bool isFree{ true };
			int linearIndex{ GetLinearIndexFrom2D(index, g_GridColumns) };
			g_Grid[linearIndex] = GridCell{ isFree, index, cell };
		}
	}
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
				g_Grid[index].isFree = false;
			}
		}
	}
}

void InitialiseTanks()
{
	const int initialHealth{ 3 };
	for (int i = 0; i < g_AmountOfPlayers; i++)
	{
		Tank& tank = g_Tanks[i];
		tank.tankIndex = g_StartingPositions[i];
		tank.health = initialHealth;
		int tankLocationIndex{ GetLinearIndexFrom2D(tank.tankIndex, g_GridColumns) };
		g_Grid[tankLocationIndex].isFree = false;
	}
}

void InitialiseObstacles(Tank* pTanks)
{
	int nbrOfObstacles{ 20 };
	for (int i{}; i < nbrOfObstacles; ++i)
	{
		int rdmRow{ rand() % (g_GridRows - 2) + 1 };
		int rdmColumn{ rand() % (g_GridColumns - 2) + 1 };
		int rdmIndex{ GetLinearIndexFrom2D(rdmRow, rdmColumn, 16) };
		int tankIndex = GetLinearIndexFrom2D(1, 14, 16);
		if ((tankIndex + 1 == rdmIndex) || tankIndex - 1 == rdmIndex || tankIndex + g_GridColumns - 1 == rdmIndex || tankIndex + g_GridColumns + 1 == rdmIndex)
		{
			--i;
		}
		else
		{
			int index{ GetLinearIndexFrom2D(rdmRow, rdmColumn, g_GridColumns) };
			if (g_Grid[index].isFree == false) --i;
			g_Grid[index].isFree = false;
		}
	}
}

#pragma endregion Initialisers

#pragma region Logic

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
	int tankLocationIndex{ GetLinearIndexFrom2D(tank.tankIndex, g_GridColumns) };
	int tankDestinationIndex{ GetLinearIndexFrom2D(tank.tankIndex.row + yDisplacement, tank.tankIndex.column + xDisplacement, g_GridColumns) };
	if (g_Grid[tankDestinationIndex].isFree)
	{
		tank.tankIndex.column += xDisplacement;
		tank.tankIndex.row += yDisplacement;
		g_Grid[tankLocationIndex].isFree = true;
		g_Grid[tankDestinationIndex].isFree = false;
	}
	g_TurnCounter = (g_TurnCounter + 1) % g_AmountOfPlayers;
}

void CalculateBarrelAngle(const Point2f& mousePosition)
{
	Tank& activeTank{ g_Tanks[g_TurnCounter] };
	Rectf tankRectangle{ g_SideLength * activeTank.tankIndex.column, g_SideLength * activeTank.tankIndex.row, g_SideLength, g_SideLength };
	Point2f tankCenter{ GetCenterOfRectangle(tankRectangle) };
	float barrelAngle{ GetBarrelAngle(tankCenter, mousePosition) };
	activeTank.barrelAngle = barrelAngle;
}

#pragma endregion Logic

#pragma region DrawFunctions

void DrawBackground()
{
	Point2f bottom{ 0,0 };
	DrawTexture(g_Background, bottom);
}

void DrawGrid()
{
	for (int i = 0; i < g_AmountOfGridCells; i++)
	{
		const GridCell& gridCell{ g_Grid[i] };
		SetColor(g_White);
		DrawRect(gridCell.cell);
		if (gridCell.index.column == 0 || gridCell.index.column == g_GridColumns - 1
			|| gridCell.index.row == 0 || gridCell.index.row == g_GridRows - 1)
		{
			DrawTexture(g_Tile, gridCell.cell);
		}
	}
}

void DrawTanks()
{
	const Color4f tankColour{ 0.0f, 0.75f, 0.0f, 1.0f };
	SetColor(tankColour);
	for (int player = 0; player < g_AmountOfPlayers; player++)
	{
		Tank& tank{ g_Tanks[player] };
		Rectf& tankRectangle{ g_Grid[GetLinearIndexFrom2D(tank.tankIndex, g_GridColumns)].cell };
		FillRect(tankRectangle);
	}
	DrawBarrels();
}

void DrawBarrels()
{
	const Color4f barrelColour{ 0.0f, 0.0f, 0.0f, 1.0f };
	const float barrelLength{ 60.0f };
	SetColor(barrelColour);
	for (int player = 0; player < g_AmountOfPlayers; player++)
	{
		Tank& tank{ g_Tanks[player] };
		Rectf& tankRectangle{ g_Grid[GetLinearIndexFrom2D(tank.tankIndex, g_GridColumns)].cell };
		Point2f tankCenter{ GetCenterOfRectangle(tankRectangle) };
		Point2f barrelEnd{ GetCoordinatesFromRadians(barrelLength, tank.barrelAngle, tankCenter) };
		DrawLine(tankCenter, barrelEnd, 3.0f);
	}
}

void DrawInstructions()
{
	const int fontPoints{ 24 };
	const int instructionLines{ 4 };
	std::string pGameInstructions[instructionLines]
	{
	  "W or Up key to move tank up",
	  "A or Left key to move tank left",
	  "S or Down key to move tank down",
	  "D or Right key to move tank right"
	};
	for (int i{ 0 }; i < instructionLines; ++i)
	{
		Texture textTexture{};
		bool successful{ TextureFromString(pGameInstructions[i], "Resources/DIN-Light.otf", fontPoints, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f }, textTexture) };
		if (successful)
			DrawTexture(textTexture, Point2f{ 0.0f, g_WindowHeight - (textTexture.height * (i + 1)) });
		DeleteTexture(textTexture);
	}
}

#pragma endregion DrawFunctions

#pragma region CleanupFunctions

void EndBackground()
{
	DeleteTexture(g_Background);
}

void EndTile()
{
	DeleteTexture(g_Tile);
}

#pragma endregion CleanupFunctions

#pragma endregion ownDefinitions