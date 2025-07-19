project "quigly-console"
	kind "ConsoleApp"
	language "C"
	targetdir ("%{wks.location}/" .. outputdir .. "/%{prj.name}/%{cfg.system}-%{cfg.buildcfg}")
	objdir ("%{wks.location}/obj/%{prj.name}/%{cfg.system}-%{cfg.buildcfg}")
	targetname "quigly-console"
	toolset "clang"

	pic "On"
	buildoptions { "-fPIC", "-fno-fast-math" }

	files
	{
		"src/**.c"
	}

	includedirs
	{
		"%{wks.location}/quigly-console/src"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		optimize "Debug"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "system:linux"
		links { "m", "dl", "SDL3" }

	filter "system:emscripten"
		links { "SDL3" }
