from conans import ConanFile, CMake, tools


class CppreflectConan(ConanFile):
    name = "cppreflect"
    version = "0.0.6"
    license = "<Put the package license here>"
    url = "https://github.com/chibidev/reflgen"
    description = "<Description of Cppreflect here>"
    # settings = 
    # options = {}
    # default_options = ""
    generators = "cmake"
    exports_sources = "lib/*"
    no_copy_source = True

    def package(self):
        self.copy("*.hpp", dst="include", src="lib")
