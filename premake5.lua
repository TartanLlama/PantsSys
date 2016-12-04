workspace "PantsSys"
    configurations { "Debug", "Release" }
    platforms { "Win64", "Linux" }

    flags {"ExtraWarnings"}

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

-- External dependencies
project "fmt"
    kind "StaticLib"
    language "C++"
    flags {"C++14"}
    targetdir "build/lib/%{cfg.buildcfg}"
    includedirs {"ext/fmt"}
    files {"ext/fmt/fmt/**.h", "ext/fmt/fmt/**.cc"}


include "PantsAsm"
include "PantsC"
include "PantsFS"
include "PantsSim"

-- Clean Function --
newaction {
   trigger     = "clean",
   description = "clean the software",
   execute     = function ()
      print("clean the build...")
      os.rmdir("./build")
      os.rmdir("./obj")
      os.execute("rm *.make")
      os.execute("rm Makefile")
      print("done.")
   end
}


-- Documentation generation --
newaction {
    trigger     = "docs",
    description = "generate the documentation",
    execute     = function ()
      print("generating documentation...")
      os.execute("standardese PantsAsm/include")
    end
}
