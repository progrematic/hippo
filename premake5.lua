workspace "hippo"
    startproject "hippoeditor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

project "hippoeditor"
    location "hippoeditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    flags
    {
        "FatalWarnings"
    }