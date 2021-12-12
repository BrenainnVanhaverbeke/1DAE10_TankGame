#include "pch.h"
//#define _USE_MATH_DEFINES
//#include <cmath>
#include <algorithm>
#include <iostream>
#include "utils.h"

namespace utils
{
#pragma region OpenGLDrawFunctionality
	void ClearBackground(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void ClearBackground()
	{
		ClearBackground(185.0f / 255.0f, 211.0f / 255.0f, 238.0f / 255.0f);
	}
	void SetColor(float r, float g, float b, float a)
	{
		glColor4f(r, g, b, a);
	}

	void SetColor(const Color4f& color)
	{
		glColor4f(color.r, color.g, color.b, color.a);
	}

	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINES);
		{
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
		}
		glEnd();
	}

	void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth)
	{
		DrawLine(p1.x, p1.y, p2.x, p2.y, lineWidth);
	}

	void DrawRect(float left, float bottom, float width, float height, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(left, bottom);
			glVertex2f(left + width, bottom);
			glVertex2f(left + width, bottom + height);
			glVertex2f(left, bottom + height);
		}
		glEnd();
	}

	void DrawTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
			glVertex2f(p3.x, p3.y);
		}
		glEnd();
	}

	void FillTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3)
	{
		glBegin(GL_TRIANGLES);
		{
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
			glVertex2f(p3.x, p3.y);
		}
		glEnd();
	}

	void DrawRect(const Point2f& bottomLeft, float width, float height, float lineWidth)
	{
		DrawRect(bottomLeft.x, bottomLeft.y, width, height, lineWidth);
	}

	void DrawRect(const Rectf& rect, float lineWidth)
	{
		DrawRect(rect.left, rect.bottom, rect.width, rect.height, lineWidth);
	}

	void FillRect(float left, float bottom, float width, float height)
	{
		glBegin(GL_POLYGON);
		{
			glVertex2f(left, bottom);
			glVertex2f(left + width, bottom);
			glVertex2f(left + width, bottom + height);
			glVertex2f(left, bottom + height);
		}
		glEnd();
	}

	void FillRect(const Point2f& bottomLeft, float width, float height)
	{
		FillRect(bottomLeft.x, bottomLeft.y, width, height);
	}

	void FillRect(const Rectf& rect)
	{
		FillRect(rect.left, rect.bottom, rect.width, rect.height);
	}

	void DrawCircle(const Circlef& circle, float lineWidth)
	{
		DrawEllipse(circle.center, circle.radius, circle.radius, lineWidth);
	}

	void DrawEllipse(float centerX, float centerY, float radX, float radY, float lineWidth)
	{
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			for (float angle = 0.0; angle < float(2 * g_Pi); angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
		}
		glEnd();
	}

	void DrawEllipse(const Point2f& center, float radX, float radY, float lineWidth)
	{
		DrawEllipse(center.x, center.y, radX, radY, lineWidth);
	}

	void DrawEllipse(const Ellipsef& ellipse, float lineWidth)
	{
		DrawEllipse(ellipse.center.x, ellipse.center.y, ellipse.radiusX, ellipse.radiusY, lineWidth);
	}

	void FillCircle(const Circlef& circle)
	{
		FillEllipse(circle.center, circle.radius, circle.radius);
	}

	void FillEllipse(float centerX, float centerY, float radX, float radY)
	{
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glBegin(GL_POLYGON);
		{
			for (float angle = 0.0; angle < float(2 * g_Pi); angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
		}
		glEnd();
	}

	void FillEllipse(const Ellipsef& ellipse)
	{
		FillEllipse(ellipse.center.x, ellipse.center.y, ellipse.radiusX, ellipse.radiusY);
	}

	void FillEllipse(const Point2f& center, float radX, float radY)
	{
		FillEllipse(center.x, center.y, radX, radY);
	}

	void DrawArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle, float lineWidth)
	{
		if (fromAngle > tillAngle)
		{
			return;
		}

		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glLineWidth(lineWidth);
		glBegin(GL_LINE_STRIP);
		{
			for (float angle = fromAngle; angle < tillAngle; angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
			glVertex2f(centerX + radX * float(cos(tillAngle)), centerY + radY * float(sin(tillAngle)));
		}
		glEnd();

	}

	void DrawArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth)
	{
		DrawArc(center.x, center.y, radX, radY, fromAngle, tillAngle, lineWidth);
	}

	void FillArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle)
	{
		if (fromAngle > tillAngle)
		{
			return;
		}
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glBegin(GL_POLYGON);
		{
			glVertex2f(centerX, centerY);
			for (float angle = fromAngle; angle < tillAngle; angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
			glVertex2f(centerX + radX * float(cos(tillAngle)), centerY + radY * float(sin(tillAngle)));
		}
		glEnd();
	}

	void FillArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle)
	{
		FillArc(center.x, center.y, radX, radY, fromAngle, tillAngle);
	}

	void DrawPolygon(const std::vector<Point2f>& vertices, bool closed, float lineWidth)
	{
		DrawPolygon(vertices.data(), vertices.size(), closed, lineWidth);
	}

	void DrawPolygon(const Point2f* pVertices, size_t nrVertices, bool closed, float lineWidth)
	{
		glLineWidth(lineWidth);
		closed ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP);
		{
			for (size_t idx{ 0 }; idx < nrVertices; ++idx)
			{
				glVertex2f(pVertices[idx].x, pVertices[idx].y);
			}
		}
		glEnd();
	}

	void FillPolygon(const std::vector<Point2f>& vertices)
	{
		FillPolygon(vertices.data(), vertices.size());
	}

	void FillPolygon(const Point2f* pVertices, size_t nrVertices)
	{
		glBegin(GL_POLYGON);
		{
			for (size_t idx{ 0 }; idx < nrVertices; ++idx)
			{
				glVertex2f(pVertices[idx].x, pVertices[idx].y);
			}
		}
		glEnd();
	}

	void DrawVector(Vector2f& vector, Point2f startingPoint)
	{
		const float triangleLength{ 10.0f };
		const float triangleAngle{ 30.0f + 180.0f };
		const float lineThickness{ 2.0f };

		Point2f endPoint{ startingPoint.x + vector.x, startingPoint.y + vector.y };
		float vectorLength{ Length(vector) };
		float angleInRadians{ atan2(vector.y, vector.x) };
		float angleOffsetInRadians{ ConvertDegreesToRadians(triangleAngle) };
		Point2f triangleBottomLeft{ GetCoordinatesFromRadians(triangleLength, angleInRadians + angleOffsetInRadians, endPoint) };
		Point2f triangleBottomRight{ GetCoordinatesFromRadians(triangleLength, angleInRadians - angleOffsetInRadians, endPoint) };
		DrawLine(startingPoint, endPoint, lineThickness);
		FillTriangle(endPoint, triangleBottomLeft, triangleBottomRight);
	}

#pragma endregion OpenGLDrawFunctionality


#pragma region textureImplementations

	bool TextureFromFile(const std::string& path, Texture& texture)
	{
		//Load file for use as an image in a new surface.
		SDL_Surface* pLoadedSurface = IMG_Load(path.c_str());
		if (pLoadedSurface == nullptr)
		{
			std::cerr << "TextureFromFile: SDL Error when calling IMG_Load: " << SDL_GetError() << std::endl;
			return false;
		}

		TextureFromSurface(pLoadedSurface, texture);

		//Free loaded surface
		SDL_FreeSurface(pLoadedSurface);

		return true;
	}

	bool TextureFromString(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor, Texture& texture)
	{
		// Create font
		TTF_Font* pFont{};
		pFont = TTF_OpenFont(fontPath.c_str(), ptSize);
		if (pFont == nullptr)
		{
			std::cout << "TextureFromString: Failed to load font! SDL_ttf Error: " << TTF_GetError();
			std::cin.get();
			return false;
		}

		// Create texture using this fontand close font afterwards
		bool textureOk = TextureFromString(text, pFont, textColor, texture);
		TTF_CloseFont(pFont);

		return textureOk;
	}

	bool TextureFromString(const std::string& text, TTF_Font* pFont, const Color4f& color, Texture& texture)
	{
		//Render text surface
		SDL_Color textColor{};
		textColor.r = Uint8(color.r * 255);
		textColor.g = Uint8(color.g * 255);
		textColor.b = Uint8(color.b * 255);
		textColor.a = Uint8(color.a * 255);

		SDL_Surface* pLoadedSurface = TTF_RenderText_Blended(pFont, text.c_str(), textColor);
		//SDL_Surface* pLoadedSurface = TTF_RenderText_Solid(pFont, textureText.c_str(), textColor);
		if (pLoadedSurface == nullptr)
		{
			std::cerr << "TextureFromString: Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << '\n';
			return false;
		}

		// copy to video memory
		TextureFromSurface(pLoadedSurface, texture);

		//Free loaded surface
		SDL_FreeSurface(pLoadedSurface);

		return true;
	}

	void TextureFromSurface(const SDL_Surface* pSurface, Texture& texture)
	{
		//Get image dimensions
		texture.width = float(pSurface->w);
		texture.height = float(pSurface->h);

		// Get pixel format information and translate to OpenGl format
		GLenum pixelFormat{ GL_RGB };
		switch (pSurface->format->BytesPerPixel)
		{
		case 3:
			if (pSurface->format->Rmask == 0x000000ff)
			{
				pixelFormat = GL_RGB;
			}
			else
			{
				pixelFormat = GL_BGR;
			}
			break;
		case 4:
			if (pSurface->format->Rmask == 0x000000ff)
			{
				pixelFormat = GL_RGBA;
			}
			else
			{
				pixelFormat = GL_BGRA;
			}
			break;
		default:
			std::cerr << "TextureFromSurface error: Unknow pixel format, BytesPerPixel: " << pSurface->format->BytesPerPixel << "\nUse 32 bit or 24 bit images.\n";;
			texture.width = 0;
			texture.height = 0;
			return;
		}

		//Generate an array of textures.  We only want one texture (one element array), so trick
		//it by treating "texture" as array of length one.
		glGenTextures(1, &texture.id);

		//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
		//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
		glBindTexture(GL_TEXTURE_2D, texture.id);

		// check for errors.
		GLenum e = glGetError();
		if (e != GL_NO_ERROR)
		{
			std::cerr << "TextureFromSurface, error binding textures, Error id = " << e << '\n';
			texture.width = 0;
			texture.height = 0;
			return;
		}

		//Specify the texture's data.  This function is a bit tricky, and it's hard to find helpful documentation.  A summary:
		//   GL_TEXTURE_2D:    The currently bound 2D texture (i.e. the one we just made)
		//               0:    The mipmap level.  0, since we want to update the base level mipmap image (i.e., the image itself,
		//                         not cached smaller copies)
		//         GL_RGBA:    Specifies the number of color components in the texture.
		//                     This is how OpenGL will store the texture internally (kinda)--
		//                     It's essentially the texture's type.
		//      surface->w:    The width of the texture
		//      surface->h:    The height of the texture
		//               0:    The border.  Don't worry about this if you're just starting.
		//     pixelFormat:    The format that the *data* is in--NOT the texture! 
		//GL_UNSIGNED_BYTE:    The type the data is in.  In SDL, the data is stored as an array of bytes, with each channel
		//                         getting one byte.  This is fairly typical--it means that the image can store, for each channel,
		//                         any value that fits in one byte (so 0 through 255).  These values are to be interpreted as
		//                         *unsigned* values (since 0x00 should be dark and 0xFF should be bright).
		// surface->pixels:    The actual data.  As above, SDL's array of bytes.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels);

		//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
		//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
		//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
		//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
		//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	void DeleteTexture(Texture& texture)
	{
		glDeleteTextures(1, &texture.id);
	}

	void DrawTexture(const Texture& texture, const Point2f& dstBottomLeft, const Rectf& srcRect)
	{
		Rectf dstRect{ dstBottomLeft.x, dstBottomLeft.y, srcRect.width, srcRect.height };
		DrawTexture(texture, dstRect, srcRect);
	}


	void DrawTexture(const Texture& texture, const Rectf& dstRect, const Rectf& srcRect)
	{
		// Determine texture coordinates using srcRect and default destination width and height
		float textLeft{};
		float textRight{};
		float textTop{};
		float textBottom{};

		float defaultDstWidth{};
		float defaultDstHeight{};
		if (!(srcRect.width > 0.0f && srcRect.height > 0.0f)) // No srcRect specified
		{
			// Use complete texture
			textLeft = 0.0f;
			textRight = 1.0f;
			textTop = 0.0f;
			textBottom = 1.0f;

			defaultDstHeight = texture.height;
			defaultDstWidth = texture.width;
		}
		else // srcRect specified
		{
			// Convert to the range [0.0, 1.0]
			textLeft = srcRect.left / texture.width;
			textRight = (srcRect.left + srcRect.width) / texture.width;
			textTop = (srcRect.bottom - srcRect.height) / texture.height;
			textBottom = srcRect.bottom / texture.height;

			defaultDstHeight = srcRect.height;
			defaultDstWidth = srcRect.width;
		}

		// Determine vertex coordinates
		float vertexLeft{ dstRect.left };
		float vertexBottom{ dstRect.bottom };
		float vertexRight{};
		float vertexTop{};
		if (!(dstRect.width > 0.0f && dstRect.height > 0.0f)) // If no size specified use default size
		{
			vertexRight = vertexLeft + defaultDstWidth;
			vertexTop = vertexBottom + defaultDstHeight;
		}
		else
		{
			vertexRight = vertexLeft + dstRect.width;
			vertexTop = vertexBottom + dstRect.height;
		}

		// Tell opengl which texture we will use
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		// set the texture mode https://open.gl/textures
		float color[] = { 1.0f, 0.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		// Draw
		glEnable(GL_TEXTURE_2D);
		{
			glBegin(GL_QUADS);
			{
				glTexCoord2f(textLeft, textBottom);
				glVertex2f(vertexLeft, vertexBottom);

				glTexCoord2f(textLeft, textTop);
				glVertex2f(vertexLeft, vertexTop);

				glTexCoord2f(textRight, textTop);
				glVertex2f(vertexRight, vertexTop);

				glTexCoord2f(textRight, textBottom);
				glVertex2f(vertexRight, vertexBottom);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);

	}
#pragma endregion textureImplementations


#pragma region CollisionFunctionality

	float GetDistance(Point2f pointA, Point2f pointB)
	{
		float differenceX{ abs(pointA.x - pointB.x) };
		float differenceY{ abs(pointA.y - pointB.y) };
		return sqrtf(powf(differenceX, 2) + powf(differenceY, 2));
	}

	float GetDistance(float pointAX, float pointAY, float pointBX, float pointBY)
	{
		float differenceX{ abs(pointAX - pointBX) };
		float differenceY{ abs(pointAY - pointBY) };
		return sqrtf(powf(differenceX, 2) + powf(differenceY, 2));
	}

	bool IsPointInCircle(const Circlef& circle, const Point2f& point)
	{
		if (GetDistance(circle.center, point) <= circle.radius)
			return true;
		return false;
	}

	bool IsPointInRectangle(const Rectf& rectangle, const Point2f& point)
	{
		if ((rectangle.left <= point.x && point.x <= rectangle.left + rectangle.width)
			&& (rectangle.bottom <= point.y && point.y <= rectangle.bottom + rectangle.height))
			return true;
		return false;
	}

	bool IsOverlapping(const Rectf& rectangleA, const Rectf& rectangleB)
	{
		float rectangleARight{ rectangleA.left + rectangleA.width };
		float rectangleATop{ rectangleA.bottom + rectangleA.height };
		float rectangleBRight{ rectangleB.left + rectangleB.width };
		float rectangleBTop{ rectangleB.bottom + rectangleB.height };
		if (rectangleBRight <= rectangleA.left || rectangleARight <= rectangleB.left)
			return false;
		if (rectangleBTop <= rectangleA.bottom || rectangleATop <= rectangleB.bottom)
			return false;
		return true;
	}

	bool IsOverlapping(const Circlef& circleA, const Circlef& circleB)
	{
		float distanceBetweenCircles{ GetDistance(circleA.center, circleB.center) };
		if (distanceBetweenCircles <= circleA.radius + circleB.radius)
			return true;
		return false;
	}

	bool IsIntersecting(const Circlef& circle, const Rectf& rectangle)
	{
		Point2f rectangleCenter{ GetCenterOfRectangle(rectangle) };
		float distanceX = abs(circle.center.x - rectangleCenter.x);
		float distanceY = abs(circle.center.y - rectangleCenter.y);
		if ((rectangle.width / 2) + circle.radius < distanceX || (rectangle.height / 2) + circle.radius < distanceY)
			return false;
		if (distanceX <= (rectangle.width / 2) || distanceY <= (rectangle.height / 2))
			return true;
		float cornerDistance{ powf(distanceX - (rectangle.width / 2), 2) + powf(distanceY - (rectangle.height / 2), 2) };
		return cornerDistance <= powf(circle.radius, 2);
	}

	Point2f GetCenterOfRectangle(const Rectf& rectangle)
	{
		float x{ (rectangle.left * 2 + rectangle.width) / 2 };
		float y{ (rectangle.bottom * 2 + rectangle.height) / 2 };
		return Point2f{ x, y };
	}

#pragma endregion CollisionFunctionality

#pragma region VectorFunctionality

	Vector2f Add(Vector2f& vectorA, Vector2f& vectorB)
	{
		return Vector2f{ vectorA.x + vectorB.x, vectorA.y + vectorB.y };
	}

	Vector2f Subtract(Vector2f& vectorA, Vector2f& vectorB)
	{
		return Vector2f{ vectorA.x - vectorB.x, vectorA.y - vectorB.y };
	}

	float DotProduct(Vector2f& vectorA, Vector2f& vectorB)
	{
		return (vectorA.x * vectorB.x) + (vectorA.y * vectorB.y);
	}

	float CrossProduct(Vector2f& vectorA, Vector2f& vectorB)
	{
		return (vectorA.x * vectorB.y) - (vectorA.y * vectorB.x);
	}

	std::string ToString(Vector2f& vector)
	{
		return "[" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + "]";
	}

	Vector2f Scale(Vector2f& vectorA, float scalar)
	{
		return Vector2f{ vectorA.x * scalar, vectorA.y * scalar };
	}

	float Length(Vector2f& vector)
	{
		return sqrtf(powf(vector.x, 2) + powf(vector.y, 2));
	}

	Vector2f Normalise(Vector2f& vector)
	{
		float length = Length(vector);
		return Vector2f{ vector.x / length, vector.y / length };
	}

	float AngleBetween(Vector2f& vectorA, Vector2f& vectorB)
	{
		return atan2(CrossProduct(vectorA, vectorB), DotProduct(vectorA, vectorB));
	}

	bool AreEqual(Vector2f& vectorA, Vector2f& vectorB)
	{
		bool isLengthEqual{ abs(Length(vectorA) - Length(vectorB)) <= 0.0001f };
		bool isDirectionEqual{ abs(vectorA.x - vectorB.x) <= 0.0001f && abs(vectorA.y - vectorB.y) <= 0.0001f };
		return isLengthEqual && isDirectionEqual;
	}

#pragma endregion VectorFunctionality

#pragma region GeneralUtils

	Point2f GetMiddle(const Point2f& pointA, const Point2f& pointB)
	{
		return Point2f{ (pointA.x + pointB.x) / 2, (pointA.y + pointB.y) / 2 };
	}

	float ConvertDegreesToRadians(float degrees)
	{
		return degrees * (utils::g_Pi / 180);
	}

	float ConvertRadiansToDegrees(float radians)
	{
		return radians * (180 / utils::g_Pi);
	}

	Point2f GetCoordinatesFromRadians(float radius, float radians, Point2f offset)
	{
		float x = offset.x + (radius * cosf(radians));
		float y = offset.y + (radius * sinf(radians));
		return Point2f{ x, y };
	}

	Vector2f GetVectorToDirectionWithForce(float angle, float force)
	{
		Vector2f vector{};
		vector.x = force * cosf(angle);
		vector.y = force * sinf(angle);
		return vector;
	}

	Color4f GetRandomColour()
	{
		float r{ GetRandomNumber(0, 255) / 255 };
		float g{ GetRandomNumber(0, 255) / 255 };
		float b{ GetRandomNumber(0, 255) / 255 };
		return Color4f{ r, g, b, 1 };
	}

	float GetRandomNumber(int lowestValue, int highestValue, bool isInclusive)
	{
		int amountOfValues{ highestValue - lowestValue };
		if (isInclusive)
			amountOfValues++;
		return (float)(rand() % amountOfValues) + lowestValue;
	}

	int GetLinearIndexFrom2D(int rowIndex, int columnIndex, int nrOfColumns)
	{
		return rowIndex * nrOfColumns + columnIndex;
	}

	int GetLinearIndexFrom2D(Index2D index, int nrOfColumns)
	{
		return GetLinearIndexFrom2D(index.row, index.column, nrOfColumns);
	}

#pragma endregion GeneralUtils

#pragma region GameSpecificUtils

	float GetBarrelAngle(const Point2f& tankCenter, const Point2f& mousePosition)
	{
		float xDifference{ mousePosition.x - tankCenter.x  };
		float yDifference{ mousePosition.y - tankCenter.y };
		return atan2(yDifference, xDifference);
	}

#pragma endregion GameSpecificUtils
}