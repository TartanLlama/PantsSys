workspace "PantsSys"
    configurations { "Debug", "Release" }
    platforms { "Win64", "Linux" }

filter "platforms:Win64"
    system "Windows"
    architecture "x64"

filter "platforms:Linux"
    system "Linux"
    architecture "x64"

filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "PantsAsm"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "build/bin/%{cfg.buildcfg}"
    includedirs { "Common/include", "PantsAsm/include" }
    files { "PantsAsm/src/**.cpp", "PantsAsm/include/**.hpp", "Common/include/**.hpp" }
    
project "PantsVM"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "build/bin/%{cfg.buildcfg}"
    libdirs { "SDL2-2.0.4/lib/x64" }
    includedirs { "Common/include", "PantsVM/include", "SDL2-2.0.4/include" }
    links { "SDL2", "SDL2_image", "SDL2_ttf" }
    files { "PantsVM/src/**.cpp", "PantsVM/include/**.hpp", "Common/include/**.hpp" }

project "PantsC"
    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "build/bin/%{cfg.buildcfg}"
    includedirs {"PantsC/include", "ext/fmt", "ext/status_value", "Common/include"}
    links {"fmt"}
    files {"PantsC/include/**.hpp", "PantsC/src/**.cpp"}

-- External dependencies
project "fmt"
    kind "StaticLib"
    language "C++"
    flags {"C++14"}
    targetdir "build/lib/%{cfg.buildcfg}"
    includedirs {"ext/fmt"}
    files {"ext/fmt/fmt/**.h", "ext/fmt/fmt/**.cc"}
