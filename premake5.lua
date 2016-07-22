workspace "PantsSys"
    configurations { "Debug", "Release" }

project "PantsAsm"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    includedirs { "Common/include" }

    files { "PantsAsm/src/**.cpp", "PantsAsm/include/**.hpp", "Common/include/**.hpp" }
    
project "PantsVM"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    includedirs { "Common/include" }
    links { "SDL2" }

    files { "PantsVM/src/**.cpp", "PantsVM/include/**.hpp", "Common/include/**.hpp" }
    
