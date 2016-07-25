workspace "PantsSys"
    configurations { "Debug", "Release" }

project "PantsAsm"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    includedirs { "Common/include", "PantsAsm/include"}

    files { "PantsAsm/src/**.cpp", "PantsAsm/include/**.hpp", "Common/include/**.hpp" }
    
project "PantsVM"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    includedirs { "Common/include", "PantsVM/include"}
    links { "SDL2", "SDL2_image", "SDL2_ttf" }

    files { "PantsVM/src/**.cpp", "PantsVM/include/**.hpp", "Common/include/**.hpp" }
    
