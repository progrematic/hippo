workspace "hippo"
    startproject "hippoeditor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"

-- External Dependencies
externals = {}
externals["sdl2"] = "external/sdl2"


project "hippo"
    location "hippo"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir(tdir)
    objdir(odir)
    
    files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    sysincludedirs
    {
        "%{prj.name}/include/hippo",
        "%{externals.sdl2}/include"
    }

    flags
    {
        "FatalWarnings"
    }

    filter {"system:windows", "configurations:*"}
        systemversion "latest"

        defines
        {
            "HIPPO_PLATFORM_WINDOWS"
        }

    filter {"system:macosx", "configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "HIPPO_PLATFORM_MAC"
        }

    filter {"system:linux", "configurations:*"}
        defines
        {
            "HIPPO_PLATFORM_LINUX"
        }
    
    filter "configurations:Debug"
        defines
        {
            "HIPPO_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines
        {
            "HIPPO_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"

project "hippoeditor"
    location "hippoeditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    links "hippo"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    sysincludedirs
    {
        "hippo/include"
    }

    flags
    {
        "FatalWarnings"
    }

    filter {"system:windows", "configurations:*"}
        systemversion "latest"

        defines
        {
            "HIPPO_PLATFORM_WINDOWS"
        }

        libdirs
        {
            "%{externals.sdl2}/lib"
        }

        links
        {
            "SDL2"
        }


    filter {"system:macosx", "configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "HIPPO_PLATFORM_MAC"
        }

    filter {"system:linux", "configurations:*"}
        defines
        {
            "HIPPO_PLATFORM_LINUX"
        }
    
    filter "configurations:Debug"
        defines
        {
            "HIPPO_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines
        {
            "HIPPO_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"