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
	
	includedirs { "raylib/src","raylibExtras/rlImGui", "raylibExtras/imGui"}
	vpaths 
	{
		["Header Files"] = { "rraylibExtras/lImGui/**.h"},
		["Source Files"] = {"raylibExtras/rlImGui/**.cpp"},
		["ImGui Files"] = { "raylibExtras/imGui/*.h","raylibExtras/imGui/*.cpp" },
	}
	files {"raylibExtras/imGui/*.h", "raylibExtras/imGui/*.cpp", "raylibExtras/rlImGui/**.cpp", "raylibExtras/rlImGui/**.h"}

	
project "Application"
	kind "WindowedApp"
	location "./"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	cppdialect "C++17"
	
	includedirs {"src"}
	vpaths 
	{
		["Header Files"] = { "**.h"},
		["Source Files"] = {"**.c", "**.cpp"},
	}
	files {"Application/**.c", "Application/**.cpp", "Application/**.h"}

	links {"raylib", "rlExtrasCPP", "rlImGui"}
	
	includedirs { "%{wks.name}", "raylib/src", "raylibExtras/rlExtrasCPP",  "raylibExtras/rlImGui", "raylibExtras/imGui"}
	defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		dependson {"raylib"}
		links {"winmm", "raylib.lib", "kernel32"}
		libdirs {"bin/%{cfg.buildcfg}"}
		
	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}