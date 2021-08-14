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
    Vec2,
    Vec3,
    Vec4,
    Mat4,
    Sampler2D,
    SamplerCube,
};

struct UniformInfo
{
    std::string Name;
    UniformTypes UniformType = UniformTypes::Unknown;
    bool UpdateEachFrame = false;
    void* ValuePtr = nullptr;

    void DeleteValue()
    {
        delete[](ValuePtr);
    }

    void InitDefaultValue()
    {
        switch (UniformType)
        {
        case UniformTypes::Unknown:
            ValuePtr = nullptr;
            break;
        case UniformTypes::Int:
        case UniformTypes::Bool:
            ValuePtr = new int[1];
            memset(ValuePtr, 0, sizeof(int));
            break;
        case UniformTypes::Float:
            ValuePtr = new float [1];
            memset(ValuePtr, 0, sizeof(float));
            break;
        case UniformTypes::Vec2:
            ValuePtr = new float[2];
            memset(ValuePtr, 0, sizeof(float) * 2);
            break;
        case UniformTypes::Vec3:
            ValuePtr = new float[3];
            memset(ValuePtr, 0, sizeof(float) * 3);
            break;
        case UniformTypes::Vec4:
            ValuePtr = new float[4];
            memset(ValuePtr, 0, sizeof(float) * 4);
            ((float*)ValuePtr)[3] = 1.0f;
            break;
        case UniformTypes::Mat4:
            ValuePtr = new float[16];
            memset(ValuePtr, 0, sizeof(float) * 16);
            ((float*)ValuePtr)[0] = 1.0f;
            ((float*)ValuePtr)[5] = 1.0f;
            ((float*)ValuePtr)[10] = 1.0f;
            break;
        case UniformTypes::Sampler2D:
            ValuePtr = new int[1];
            memset(ValuePtr, 0, sizeof(int));
            break;
        case UniformTypes::SamplerCube:
            ValuePtr = new int[1];
            memset(ValuePtr, 0, sizeof(int));
            break;
        default:
            break;
        }
    }
};

class ShaderInfo
{
public:
    ~ShaderInfo()
    {
        for (auto& info : Uniforms)
            info.DeleteValue();
    }

    std::string PathName;
    std::string Name;
    ShaderTypes ShaderType = ShaderTypes::Unknown;

    std::vector<UniformInfo> Uniforms;

    const char* GetFileName()
    {
        if (PathName.size() == 0)
            return nullptr;

        return PathName.c_str();
    }
};

class ShaderInstance
{
public:
    size_t RefCount = 0;

    Shader  RaylibShader = { 0 };

    ShaderInfo VertexShader;
    ShaderInfo FragmentShader;
};

class ShaderManager
{
public:
    std::map<int, ShaderInstance> ShaderCache;

    void Clear();
    ShaderInstance& GetShader(int materialIndex);
    ShaderInstance& LoadShader(int materialIndex, const char* vertextShaderPath, const char* fragmentShaderPath);
};

