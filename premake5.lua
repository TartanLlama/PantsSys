workspace "PantsSys"
    configurations { "Debug", "Release" }
    platforms { "Win64", "Linux", "OSX" }

    flags {"ExtraWarnings", "FatalWarnings"}
    
filter "platforms:Win64"
    system "Windows"
    architecture "x64"
    -- 4814 = constxpr doesn't imply const (used in optional)
    -- 4129 = unrecognied character escape sequence (used in catch)
    -- 4702 = unreachable code (exists in optional, used for maintainability in default cases of switches)
    disablewarnings{"4814", "4129", "4702"}

    -- No checked iterators
    defines {"_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS"}


filter "platforms:Linux"
    system "Linux"
    architecture "x64"

filter "platforms:OSX"
    system "MacOSX"
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

include "ext"

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
      os.execute("standardese --output.format html PantsC/include -I ext/fmt -I ext -I Common/include -I PantsC/include")
    end
}
