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

#include <map>
#include <string>
#include <vector>

enum class ShaderTypes
{
    Unknown = 0,
    Vertex,
    Fragment,
};

enum class UniformTypes
{
    Unknown = 0,
    Bool,
    Int,
    Float,
    MaterialMapIndex,
    Projection,
    View,
};

struct UniformInfo
{
    std::string Name;
    UniformTypes UniformType = UniformTypes::Unknown;
    bool UpdateEachFrame = false;
    void* ValuePtr = nullptr;
};

struct ShaderInfo
{
    std::string PathName;
    std::string Name;
    ShaderTypes ShaderType = ShaderTypes::Unknown;

    std::vector<UniformInfo> Uniforms;
};

struct ShaderInstance
{
    size_t RefCount = 0;

    Shader  RaylibShader = { 0 };

    ShaderInfo VertexShader;
    ShaderInfo FragmentShader;
};

class ShaderManager
{
public:
    std::map<int, ShaderInstance> ShaderCache;

    Shader LoadShader(const char* vertextShaderPath, const char* fragmentShaderPath);
};

