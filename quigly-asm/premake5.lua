project "quigly-asm"
	kind "ConsoleApp"
	language "C"
	targetdir ("%{wks.location}/" .. outputdir)
	objdir ("%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}")
	targetname "quigly-asm"
	toolset "clang"

	pic "On"
	buildoptions { "-fPIC" }

	files
	{
		"src/**.c"
	}

	includedirs
	{
		"%{wks.location}/asm/src"
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
