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

void ParseUniforms(const char* filename, ShaderInfo& shaderInfo)
{
    std::vector<std::string> lines = GetFileLines(filename);
    for (std::string& line : lines)
    {
        if (!StartsWith("uniform ",line))
            continue;


    }
}


Shader ShaderManager::LoadShader(const char* vertextShaderPath, const char* fragmentShaderPath)
{

    return LoadShader(vertextShaderPath, fragmentShaderPath);
}

