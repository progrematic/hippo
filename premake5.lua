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
externals["maclibs"] = "external/maclibs"
externals["sdl2"] = "external/sdl2"
externals["spdlog"] = "external/spdlog"
externals["glad"] = "external/glad"

-- Process Glad before anything else
include "external/glad"

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
        "%{prj.name}/include/**.hpp",
        "%{prj.name}/include/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/**.natvis"
    }
    
    sysincludedirs
    {
        "%{prj.name}/include",
        "%{externals.sdl2}/include",
        "%{externals.spdlog}/include",
        "%{externals.glad}/include"
    }

    flags
    {
        "FatalWarnings"
    }

    defines
    {
        "GLFW_INCLUDE_NONE" -- Ensures glad doesn't include glfw
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
        "hippo/include",
        "%{externals.spdlog}/include"
    }

    flags
    {
        "FatalWarnings"
    }

    postbuildcommands
    {
        "python3 " .. path.getabsolute("%{prj.name}") .. "/postbuild.py config=%{cfg.buildcfg} prj=%{prj.name}"
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
            "SDL2",
            "glad"
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

        abspath = path.getabsolute("%{externals.maclibs}")
        linkoptions {"-F " .. abspath}
        
        links
        {
            "SDL2.framework",
            "glad"
        }

    filter {"system:linux", "configurations:*"}
        defines
        {
            "HIPPO_PLATFORM_LINUX"
        }

        links
        {
            "SDL2",
            "glad",
            "dl"
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

project "PongV1"
    location "PongV1"
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
        "hippo/include",
        "%{externals.spdlog}/include"
    }

    flags
    {
        "FatalWarnings"
    }

    postbuildcommands
    {
        "python3 " .. path.getabsolute("%{prj.name}") .. "/postbuild.py config=%{cfg.buildcfg} prj=%{prj.name}"
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
            "SDL2",
            "glad"
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

        abspath = path.getabsolute("%{externals.maclibs}")
        linkoptions {"-F " .. abspath}
        
        links
        {
            "SDL2.framework",
            "glad"
        }

    filter {"system:linux", "configurations:*"}
        defines
        {
            "HIPPO_PLATFORM_LINUX"
        }

        links
        {
            "SDL2",
            "glad",
            "dl"
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