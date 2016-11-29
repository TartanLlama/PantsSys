project "PantsSim"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "../build/bin/%{cfg.buildcfg}"
    libdirs { "../SDL2-2.0.4/lib/x64" }
    includedirs { "../Common/include", "include", "../SDL2-2.0.4/include" }
    links { "SDL2", "SDL2_image", "SDL2_ttf" }
    files { "src/**.cpp", "include/**.hpp", "include/**.hpp", "../Common/src/**.cpp" }

project "PantsSimTest"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "../build/bin/%{cfg.buildcfg}"
    libdirs { "../SDL2-2.0.4/lib/x64" }
    includedirs { "../Common/include", "include", "../SDL2-2.0.4/include", "../ext/catch" }
    links { "SDL2", "SDL2_image", "SDL2_ttf" }
    files { "test/**.cpp", "src/**.cpp", "include/**.hpp", "../Common/include/**.hpp", "../Common/src/**.cpp" }
    excludes { "src/main.cpp" }
