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
	InitialiseObstacles(g_Tanks);
}

void Draw()
{
	ClearBackground(0.0f, 0.0f, 0.0f);
	if (!g_IsGameOver)
	{
		DrawBackground();
		DrawGrid();
		DrawObstacles();
		DrawTanks();
		if (g_IsShooting)
			DrawProjectile();
	}
	else
		DrawEndScreen();
	if (g_IsIPressed)
		DrawInstructions();
}

void Update(float elapsedSec)
{
	UpdateBarrelEnd();
	UpdateProjectile(elapsedSec);
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
	if (!g_IsShooting)
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
	if (key == SDLK_i)
	{
		g_IsIPressed = !g_IsIPressed;
		g_IsLoggedToConsole = false;
	}
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	if (!g_IsShooting && !g_IsGameOver)
	{
		g_MousePosition = Point2f{ (float)e.x, g_WindowHeight - (float)e.y };
		g_Tanks[g_TurnCounter].barrelAngle = GetBarrelAngle(g_Tanks[g_TurnCounter].tankCenter, g_MousePosition);
	}
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{

}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	if (!g_IsShooting && !g_IsGameOver)
	{
		Tank& activeTank{ g_Tanks[g_TurnCounter] };
		g_IsShooting = true;
		g_ProjectileVector = GetVectorToDirectionWithForce(g_Tanks[g_TurnCounter].barrelAngle, g_ProjectileSpeed);
		g_Projectile.center = activeTank.barrelEnd;
	}
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
		tank.barrelAngle = ConvertDegreesToRadians(180.0f * i);
		int tankLocationIndex{ GetLinearIndexFrom2D(tank.tankIndex, g_GridColumns) };
		GridCell& gridCell{ g_Grid[tankLocationIndex] };
		tank.tankCenter = GetCenterOfRectangle(gridCell.cell);
		tank.barrelEnd = GetCoordinatesFromRadians(g_BarrelLength, tank.barrelAngle, tank.tankCenter);
		g_Grid[tankLocationIndex].isFree = false;
	}
}

void InitialiseObstacles(Tank* pTanks)
{
	int initialHealth{ 1 };
	g_pObstacles = new Obstacle[g_NrOfObstacles];
	for (int i = 0; i < g_NrOfObstacles; i++)
	{
		int randomRow{ (int)GetRandomNumber(1, g_GridRows - 2) };
		int randomColumn{ (int)GetRandomNumber(1, g_GridColumns - 2) };
		int linearIndex{ GetLinearIndexFrom2D(randomRow, randomColumn, g_GridColumns) };
		g_Grid[linearIndex].isFree = false;
		g_pObstacles[i] = Obstacle{ initialHealth, Index2D{randomColumn, randomRow} };
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
	GridCell& gridCell{ g_Grid[tankDestinationIndex] };
	if (gridCell.isFree)
	{
		tank.tankIndex.column += xDisplacement;
		tank.tankIndex.row += yDisplacement;
		g_Grid[tankLocationIndex].isFree = true;
		gridCell.isFree = false;
		tank.tankCenter = GetCenterOfRectangle(gridCell.cell);
		UpdateBarrelEnd();
	}
	g_TurnCounter = (g_TurnCounter + 1) % g_AmountOfPlayers;
}

void UpdateBarrelEnd()
{
	Tank& activeTank{ g_Tanks[g_TurnCounter] };
	int index = GetLinearIndexFrom2D(activeTank.tankIndex, g_GridColumns);
	activeTank.barrelEnd = GetCoordinatesFromRadians(g_BarrelLength, activeTank.barrelAngle, activeTank.tankCenter);
}

void UpdateProjectile(float elapsedSec)
{
	if (g_IsShooting)
	{
		g_Projectile.center.x += g_ProjectileVector.x * elapsedSec;
		g_Projectile.center.y += g_ProjectileVector.y * elapsedSec;
		int tankIndex{ CheckTankHit() };
		if (-1 < tankIndex)
		{
			ResolveTankHit(tankIndex);
			if (!g_IsGameOver)
				g_TurnCounter = (g_TurnCounter + 1) % g_AmountOfPlayers;
			return;
		}
		int obstacleIndex{ CheckObstacleHit() };
		if (-1 < obstacleIndex)
		{
			ResolveObstacleHit(obstacleIndex);
			g_TurnCounter = (g_TurnCounter + 1) % g_AmountOfPlayers;
			return;
		}
		if (g_Projectile.center.x < 0 || g_WindowWidth < g_Projectile.center.x
			|| g_Projectile.center.y < 0 || g_WindowHeight < g_Projectile.center.y)
		{
			g_IsShooting = false;
			g_TurnCounter = (g_TurnCounter + 1) % g_AmountOfPlayers;
		}
	}
}

int CheckTankHit()
{
	for (int tankIndex{ 0 }; tankIndex < g_AmountOfPlayers; ++tankIndex)
	{
		Tank& tank{ g_Tanks[tankIndex] };
		int gridCellIndex{ GetLinearIndexFrom2D(tank.tankIndex, g_GridColumns) };
		GridCell gridCell{ g_Grid[gridCellIndex] };
		if (IsIntersecting(g_Projectile, gridCell.cell) && 0 < tank.health)
			return tankIndex;
	}
	return -1;
}

int CheckObstacleHit()
{
	for (int obstacleIndex{ 0 }; obstacleIndex < g_NrOfObstacles; obstacleIndex++)
	{
		Obstacle& obstacle{ g_pObstacles[obstacleIndex] };
		int index{ GetLinearIndexFrom2D(obstacle.obstacleIndex, g_GridColumns) };
		GridCell& gridCell{ g_Grid[index] };
		if (IsIntersecting(g_Projectile, gridCell.cell) && 0 < obstacle.health)
			return obstacleIndex;
	}
	return -1;
}

void ResolveTankHit(int tankIndex)
{
	Tank& tank{ g_Tanks[tankIndex] };
	tank.health--;
	g_IsShooting = false;
	if (tank.health <= 0)
		g_IsGameOver = true;
}

void ResolveObstacleHit(int obstacleIndex)
{
	Obstacle& obstacle{ g_pObstacles[obstacleIndex] };
	obstacle.health--;
	if (obstacle.health <= 0)
	{
		int linearIndex{ GetLinearIndexFrom2D(obstacle.obstacleIndex, g_GridColumns) };
		g_Grid[linearIndex].isFree = true;
	}
	g_IsShooting = false;
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

void DrawObstacles()
{
	for (int i = 0; i < g_NrOfObstacles; i++)
	{
		Obstacle& obstacle{ g_pObstacles[i] };
		GridCell& obstacleCell{ g_Grid[GetLinearIndexFrom2D(obstacle.obstacleIndex, g_GridColumns)] };
		SetColor(0.0f, 0.0f, 0.0f);
		if (0 < obstacle.health)
			FillRect(obstacleCell.cell);
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
	SetColor(barrelColour);
	for (int player = 0; player < g_AmountOfPlayers; player++)
	{
		Tank& tank{ g_Tanks[player] };
		DrawLine(tank.tankCenter, tank.barrelEnd, 5.0f);
	}
}

void DrawProjectile()
{
	const Color4f projectileColour{ 0.7f, 0.0f, 0.0f, 1.0f };
	SetColor(projectileColour);
	FillCircle(g_Projectile);
}

void DrawInstructions()
{
	const int fontPoints{ 24 };
	const int instructionLines{ 6 };
	Point2f origin{ DrawInstructionsBackground() };
	origin.x += 20;
	origin.y += 15;
	std::string pGameInstructions[instructionLines]
	{
		"- W or Up key to move tank up",
		"- A or Left key to move tank left",
		"- S or Down key to move tank down",
		"- D or Right key to move tank right",
		"- Click to shoot in the direction of the mouse pointer.",
		"- The game is turn based, you can either move or shoot."
	};
	for (int i{ 0 }; i < instructionLines; i++)
	{
		Texture textTexture{};
		bool successful{ TextureFromString(pGameInstructions[instructionLines - 1 - i], "Resources/DIN-Light.otf", fontPoints, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f }, textTexture) };
		if (successful)
		{
			DrawTexture(textTexture, origin);
			origin.y += textTexture.height;
		}
		DeleteTexture(textTexture);
		if (!g_IsLoggedToConsole)
			std::cout << pGameInstructions[i] << std::endl;
	}
	g_IsLoggedToConsole = true;
}

Point2f DrawInstructionsBackground()
{
	const float backgroundHeight{ 200.0f };
	const float backgroundWidth{ 650.0f };
	const float left{ (g_WindowWidth - backgroundWidth) / 2 };
	const float bottom{ (g_WindowHeight - backgroundHeight) / 2 };
	SetColor(0.0f, 0.0f, 0.0f, 0.5f);
	FillRect(left, bottom, backgroundWidth, backgroundHeight);
	return Point2f{ left, bottom };
}

void DrawEndScreen()
{
	const int fontPointsHeader{ 32 };
	const int fontPointsSubtext{ 24 };
	const std::string headerText{ "You win," };
	const std::string subText{ "You sexy motherfucker!" };
	Texture textTexture{};
	TextureFromString(headerText, "Resources/DIN-Light.otf", fontPointsHeader, g_White, textTexture);
	Point2f textDestination{ (g_WindowWidth - textTexture.width) / 2, (g_WindowHeight - textTexture.height) * 0.66f };
	DrawTexture(textTexture, textDestination);
	TextureFromString(subText, "Resources/DIN-Light.otf", fontPointsSubtext, g_White, textTexture);
	textDestination.x = (g_WindowWidth - textTexture.width) / 2;
	textDestination.y -= textTexture.height;
	DrawTexture(textTexture, textDestination);
	DeleteTexture(textTexture);
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