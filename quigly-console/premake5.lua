project "quigly-console"
	kind "ConsoleApp"
	language "C"
	targetdir ("%{wks.location}/" .. outputdir .. "/%{prj.name}/%{cfg.buildcfg}/")
	objdir ("%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}")
	targetname "quigly-console"
	toolset "clang"

	pic "On"
	buildoptions { "-fPIC" }

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
		optimize "Speed"

	filter "system:linux"
		links { "m", "dl", "SDL3" }
