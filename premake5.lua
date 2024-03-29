workspace "TestFrame"
	configurations { "Debug","Debug.DLL", "Release", "Release.DLL" }
	platforms { "x64"}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Debug.DLL"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter "configurations:Release.DLL"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter { "platforms:x64" }
		architecture "x86_64"
		
	targetdir "bin/%{cfg.buildcfg}/"
	
	defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}
		
project "raylib"
		filter "configurations:Debug.DLL OR Release.DLL"
			kind "SharedLib"
			defines {"BUILD_LIBTYPE_SHARED"}
			
		filter "configurations:Debug OR Release"
			kind "StaticLib"
			
		filter "action:vs*"
			defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
			links {"winmm"}
			
		filter "action:gmake*"
			links {"pthread", "GL", "m", "dl", "rt", "X11"}
			
		filter{}
		
		location "build"
		language "C++"
		targetdir "bin/%{cfg.buildcfg}"
		cppdialect "C++17"
		
		includedirs { "raylib/src", "raylib/src/external/glfw/include"}
		vpaths 
		{
			["Header Files"] = { "raylib/src/**.h"},
			["Source Files/*"] = {"raylib/src/**.c"},
		}
		files {"raylib/src/*.h", "raylib/src/*.c"}
		
project "rlExtrasCPP"
	kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter{}
	
	location "raylibExtras/rlExtrasCPP/"
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
	
	includedirs { "raylib/src","rlExtrasCPP"}
	vpaths 
	{
		["Header Files"] = { "raylibExtras/rlExtrasCPP/**.h"},
		["Source Files"] = {"raylibExtras/rlExtrasCPP/**.cpp"},
	}
	files {"raylibExtras/rlExtrasCPP/**.h", "raylibExtras/rlExtrasCPP/**.cpp"}
	
project "rlImGui"
	kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter{}
	
	location "raylibExtras/rlImGui/"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	includedirs { "raylib/src","raylibExtras/rlImGui", "raylibExtras/imgui"}
	vpaths 
	{
		["Header Files"] = { "raylibExtras/lImGui/**.h"},
		["Source Files"] = {"raylibExtras/rlImGui/**.cpp"},
		["ImGui Files"] = { "raylibExtras/imgui/*.h","raylibExtras/imgui/*.cpp" },
	}
	files {"raylibExtras/imgui/*.h", "raylibExtras/imgui/*.cpp", "raylibExtras/rlImGui/**.cpp", "raylibExtras/rlImGui/**.h"}
    
project "clip"
	kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
				
	filter{}
	
	location "clip/"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	includedirs { "clip"}
	vpaths 
	{
		["Header Files"] = { "clip/**.h"},
		["Source Files"] = {"clip/**.cpp"},
	}
    
    files {"clip/*.h", "clip/image.cpp", "clip/clip.cpp"}
	filter "action:vs*"
        files {"clip/clip_win.cpp"}
    filter "action:gmake*"
        files {"clip/clip_x11.cpp"}
		links {"x11", "libpng", "xcb", "pthread"}

project "Application"
	kind "WindowedApp"
	location "./"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	cppdialect "C++17"
	
	includedirs {"src"}
	vpaths 
	{
		["Header Files"] = { "Application/*.h"},
		["Source Files"] = {"Application/*.c", "Application/*.cpp"},
	}
	files {"Application/*.c", "Application/*.cpp", "Application/*.h", "Application/views/**.cpp", "Application/views/**.h"}

	links {"raylib", "rlExtrasCPP", "rlImGui", "clip"}
	
	includedirs { "Application", "Application/views", "raylib/src", "raylibExtras/rlExtrasCPP",  "raylibExtras/rlImGui", "raylibExtras/imgui", "clip"}
    
	defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		dependson {"raylib"}
		links {"winmm", "raylib.lib", "kernel32","	Shlwapi"}
		libdirs {"bin/%{cfg.buildcfg}"}
        characterset ("MBCS")
		
	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}