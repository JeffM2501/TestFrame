/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   Testframe - a Raylib/ImGui test framework
*
*   LICENSE: ZLIB
*
*   Copyright (c) 2021 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#pragma once

#include "raylib.h"
#include "rlgl.h"
#include "RaylibColors.h"

#include <algorithm>


namespace VectorTools
{
    inline Vector2 HalfVector(const Vector2& vector)
    {
        return Vector2{ vector.x * 0.5f, vector.y * 0.5f };
    }

    inline Vector3 HalfVector(const Vector3& vector)
    {
        return Vector3{ vector.x * 0.5f, vector.y * 0.5f, vector.z * 0.5f };
    }
}

namespace RectTools
{
    inline float MaxSize(const Rectangle& rect)
    {
        return std::max(rect.width, rect.height);
    }

    inline Vector2 CenterSize(const Rectangle& rect)
    {
        return Vector2{ rect.width / 2,rect.height / 2 };
    }
}

namespace ScreenTools
{
    inline Vector2 Center()
    {
        return Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    }

    inline Vector3 Center3D()
    {
        return Vector3{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f, 0 };
    }
}
namespace DrawUtils
{
    // Draw a grid centered at (0, 0, 0)
    inline void DrawGridXZ(Vector3 origin,  float slices, float spacing, Color gridColor = Colors::LightGray, Color axisColor = Colors::DarkGray)
    {
        float halfSlices = slices / 2;

        rlCheckRenderBatchLimit(((int)slices + 2) * 4);

        rlPushMatrix();
        rlTranslatef(origin.x, origin.y, origin.z);
        rlBegin(RL_LINES);
        for (float i = -halfSlices; i <= halfSlices; i++)
        {
            if (i == 0)
            {
                rlColor4ub(axisColor.r, axisColor.g, axisColor.b, axisColor.a);
                rlColor4ub(axisColor.r, axisColor.g, axisColor.b, axisColor.a);
                rlColor4ub(axisColor.r, axisColor.g, axisColor.b, axisColor.a);
                rlColor4ub(axisColor.r, axisColor.g, axisColor.b, axisColor.a);
            }
            else
            {
                rlColor4ub(gridColor.r, gridColor.g, gridColor.b, gridColor.a);
                rlColor4ub(gridColor.r, gridColor.g, gridColor.b, gridColor.a);
                rlColor4ub(gridColor.r, gridColor.g, gridColor.b, gridColor.a);
                rlColor4ub(gridColor.r, gridColor.g, gridColor.b, gridColor.a);
            }

            rlVertex3f(i * spacing, 0.0f, -halfSlices * spacing);
            rlVertex3f(i * spacing, 0.0f, halfSlices * spacing);

            rlVertex3f(-halfSlices * spacing, 0.0f, i * spacing);
            rlVertex3f(halfSlices * spacing, 0.0f, i * spacing);
        }
        rlEnd();
        rlPopMatrix();
    }

    inline void DrawGridXY(Vector3 origin, float slices, float spacing, Color gridColor = Colors::LightGray, Color axisColor = Colors::DarkGray)
    {
        float halfSlices = slices / 2;

        rlCheckRenderBatchLimit(((int)slices + 2) * 4);

        rlPushMatrix();
        rlTranslatef(origin.x, origin.y, origin.z);
        rlBegin(RL_LINES);
        for (float i = -halfSlices; i <= halfSlices; i++)
        {
            if (i == 0)
            {
                rlColor4ub(axisColor.r, axisColor.g, axisColor.b, axisColor.a);
                rlColor4ub(axisColor.r, axisColor.g, axisColor.b, axisColor.a);
                rlColor4ub(axisColor.r, axisColor.g, axisColor.b, axisColor.a);
                rlColor4ub(axisColor.r, axisColor.g, axisColor.b, axisColor.a);
            }
            else
            {
                rlColor4ub(gridColor.r, gridColor.g, gridColor.b, gridColor.a);
                rlColor4ub(gridColor.r, gridColor.g, gridColor.b, gridColor.a);
                rlColor4ub(gridColor.r, gridColor.g, gridColor.b, gridColor.a);
                rlColor4ub(gridColor.r, gridColor.g, gridColor.b, gridColor.a);
            }

            rlVertex3f(i * spacing, -halfSlices * spacing, 0.0f);
            rlVertex3f(i * spacing, halfSlices * spacing, 0.0f);

            rlVertex3f(-halfSlices * spacing, i * spacing, 0.0f);
            rlVertex3f(halfSlices * spacing, i * spacing, 0.0f);
        }
        rlEnd();
        rlPopMatrix();
    }

    inline void DrawGrid2D(Vector2 origin, int slices, int spacing, Color gridColor = Colors::LightGray, Color axisColor = Colors::Gray)
    {
        int halfSlices = slices / 2;

         rlPushMatrix();
         rlTranslatef(origin.x, origin.y, 0);
 
        for (int i = -halfSlices; i <= halfSlices; i++)
        {
            Color color = gridColor;
            if (i == 0)
                color = axisColor;

            DrawLine(i * spacing, -halfSlices * spacing, i * spacing, halfSlices * spacing, color);
            DrawLine(-halfSlices * spacing, i * spacing, halfSlices * spacing, i * spacing, color);

        }
        rlPopMatrix();
    }
}
