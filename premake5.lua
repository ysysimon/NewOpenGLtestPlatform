workspace "NewOpenGLtestPlatform"
	architecture "X64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "OpenGLtestPlatform"
	location "OpenGLtestPlatform"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.c"
	}

	includedirs
	{
		"%{prj.name}/Dependencies/include",
		"%{prj.name}/src/vendor",
		"%{prj.name}/src/"
	}

	libdirs
	{
		"%{prj.name}/Dependencies/lib"
	}
	
	links
	{
		"glfw3.lib",
		"opengl32.lib",
		"assimp-vc142-mt.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		defines "_DEBUG"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		defines "NDEBUG"
		optimize "on"

