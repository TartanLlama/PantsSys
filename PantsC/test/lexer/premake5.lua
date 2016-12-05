function addGeneratedTests(base, prj)
   local project = premake.project
   
   for cfg in project.eachconfig(prj) do
      table.foreachi(cfg.files, function(filename)
                        if path.hasextension(filename, {".pant"}) then
                           table.insert(cfg.files, prj.basedir .. "/testcase_" .. path.getbasename(filename) .. ".cpp")
                        end
                    end)
   end
   
   return base(prj)
end

project "PantsCLexerTest"
    premake.override(premake.oven, "bakeFiles", addGeneratedTests)

    kind "ConsoleApp"
    language "C++"
    flags {"C++14"}
    targetdir "../../../build/bin/%{cfg.buildcfg}"
    includedirs {"../../include", "../../../ext/fmt", "../../../ext",
                 "../../../Common/include", "../../../ext/catch"}
    links {"fmt"}
    prebuildcommands { "cd %{prj.basedir} && python generate_tests.py" }
    postbuildcommands { "rm %{prj.basedir}/testcase_*" }
    files {"CatchMain.cpp", "../../src/Lexer.cpp", "cases/**.pant"}


