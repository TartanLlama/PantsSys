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

include "PantsAsm"
include "PantsC"
include "PantsFS"
include "PantsSim"

-- External dependencies
project "fmt"
    kind "StaticLib"
    language "C++"
    flags {"C++14"}
    targetdir "build/lib/%{cfg.buildcfg}"
    includedirs {"ext/fmt"}
    files {"ext/fmt/fmt/**.h", "ext/fmt/fmt/**.cc"}


-- Clean Function --
newaction {
   trigger     = "clean",
   description = "clean the software",
   execute     = function ()
      print("clean the build...")
      os.rmdir("./build")
      os.rmdir("./obj")
      os.execute("rm *.make")
      os.execute("Makefile")
      print("done.")
   end
}    
