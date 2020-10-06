workspace "Discount-Calculator"
	platforms {"x86", "x86_64"}
	configurations {"Debug", "Release"}

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x86_64"
		architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
wxdir = "D:/dev/sdk/wxWidgets-3.0.5"

project "Calculator" 
kind "WindowedApp"
	language "C++"
	cppdialect "C++17" 

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp",
		"res/*.rc",
	}

	includedirs
	{
		wxdir .. "/include",
		wxdir .. "/include/msvc"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "platforms:x86"
		libdirs 
		{
			wxdir .. "/lib/vc_lib" 
		}

	filter "platforms:x86_64"
		libdirs 
		{
			wxdir .. "/lib/vc_x64_lib" 
		}
	