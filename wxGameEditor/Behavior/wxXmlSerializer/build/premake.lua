project.name = "wxXmlSerializer"

-- Configurations. All I am doing is reordering them so that
project.configs = { "Release", "Debug" }

-- Add packages here.
dopackage( "../src" )
dopackage( "../samples/Sample1" )
dopackage( "../samples/Sample4" )
dopackage( "../samples/Sample2" )
dopackage( "../samples/Sample3" )

