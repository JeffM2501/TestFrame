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

#include "shader_manager.h"


std::vector<std::string> GetFileLines(const char* fileName)
{
    std::vector<std::string> lines;

    FILE* fp = fopen(fileName, "r");
    if (fp == nullptr)
        return lines;

    std::string line;
    char c = '1';
    while (fread(&c,1,1,fp) != 0)
    {
        if (c == '\n')
        {
            if (line.size() > 0)
                lines.push_back(line);
            line.clear();
        }
        else
        {
            line += c;
        }
    }
    if (line.size() > 0)
        lines.push_back(line);

    fclose(fp);
    return lines;
}

bool StartsWith(const char* prefix, const std::string& text)
{
    size_t l = strlen(prefix);
    if (text.size() < l)
        return false;

    return strncmp(prefix, text.c_str(), l) == 0;
}

std::string ReadWord(const std::string& text, size_t& offset)
{
    std::string value;
    while (offset < text.length())
    {
        if (text.c_str()[offset] == ' ' || text.c_str()[offset] == ';' || text.c_str()[offset] == '/')
            return value;
        else
            value += text.c_str()[offset++];
    }

    return value;
}

bool ParseUniformLine(const std::string& text, std::string& uniformType, std::string& uniformName)
{
    size_t offset = 8;
    uniformType = ReadWord(text, offset);
    ++offset;

    uniformName = ReadWord(text, offset);

    return uniformName.size() > 0 && uniformType.size();
}

UniformTypes ClassifyUniformType(const std::string& str)
{
    if (str == "bool")
        return UniformTypes::Bool;
    if (str == "int")
        return UniformTypes::Int;
    if (str == "float")
        return UniformTypes::Float;
    if (str == "vec2")
        return UniformTypes::Vec2;
    if (str == "vec3")
        return UniformTypes::Vec3;
    if (str == "vec4")
        return UniformTypes::Vec4;
    if (str == "mat4")
        return UniformTypes::Mat4;
    if (str == "sampler2D")
        return UniformTypes::Sampler2D;
    if (str == "samplerCube")
        return UniformTypes::SamplerCube;

    return UniformTypes::Unknown;
}

void ParseUniforms(const char* filename, ShaderInfo& shaderInfo)
{
    std::vector<std::string> lines = GetFileLines(filename);
    for (std::string& line : lines)
    {
        if (!StartsWith("uniform ",line))
            continue;

        std::string uniformType, uniformName;
        if (ParseUniformLine(line, uniformType, uniformName))
        {
            UniformInfo info;
            info.UniformType = ClassifyUniformType(uniformType);
            info.Name = uniformName;

            shaderInfo.Uniforms.emplace_back(std::move(info));
        }
    }
}

void ParseShader(const char* filename, ShaderInfo& shaderInfo)
{
    shaderInfo.Name = GetFileNameWithoutExt(filename);
    shaderInfo.PathName = filename;

    ParseUniforms(filename, shaderInfo);
}

void ShaderManager::Clear()
{
    ShaderCache.clear();
}

ShaderInstance& ShaderManager::GetShader(int materialIndex)
{
    return ShaderCache[materialIndex];
}

ShaderInstance& ShaderManager::LoadShader(int materialIndex, const char* vertextShaderPath, const char* fragmentShaderPath)
{
    ShaderInstance newInstance;
    if (vertextShaderPath == nullptr)
    {
        newInstance.VertexShader.Name = "Default";
        newInstance.VertexShader.PathName = "";
    }
    else
    {
        ParseShader(vertextShaderPath, newInstance.VertexShader);
    }

    if (fragmentShaderPath == nullptr)
    {
        newInstance.FragmentShader.Name = "Default";
        newInstance.FragmentShader.PathName = "";
    }
    else
    {
        ParseShader(fragmentShaderPath, newInstance.FragmentShader);
    }

    std::map<int, ShaderInstance>::iterator itr = ShaderCache.find(materialIndex);

    if (itr != ShaderCache.end())
    {
        UnloadShader(itr->second.RaylibShader);
        ShaderCache.erase(itr);
    }

    newInstance.RaylibShader = ::LoadShader(vertextShaderPath, fragmentShaderPath);

    ShaderCache[materialIndex] = newInstance;

    return ShaderCache[materialIndex];
}