from conans import ConanFile, tools, CMake

class TraceAPIConan(ConanFile):
    name = "TraceAPI"
    description = "C++ trace system based on boost log"
    url = "https://github.com/systelab/cpp-trace-api"
    homepage = "https://github.com/systelab/cpp-trace-api"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "json", "configuration", "settings")
    license = "MIT"
    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"
    options = {"boost": ["1.66.0", "1.67.0", "1.72.0"], "gtest": ["1.7.0", "1.8.1", "1.10.0"]}
    default_options = {"boost":"1.72.0", "gtest":"1.10.0"}
    exports_sources = "*"

    def configure(self):
        self.options["boost"].shared = True

    def requirements(self):
        if self.options.boost == "1.66.0":
            self.requires("boost/1.66.0@conan/stable")
        elif self.options.boost == "1.67.0":
            self.requires("boost/1.67.0@conan/stable")
        else:
            self.requires(("boost/%s") % self.options.boost)

    def build_requirements(self):
        if self.options.gtest == "1.7.0":
            self.build_requires("gtest/1.7.0@systelab/stable")
        elif self.options.gtest == "1.8.1":
            self.build_requires("gtest/1.8.1")
        else:
            self.build_requires("gtest/1.10.0")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")

    def package(self):
        self.copy("*.h", dst="include/TraceAPI", src="src/TraceAPI")
        self.copy("*TraceAPI.lib", dst="lib", keep_path=False)
        self.copy("*TraceAPI.pdb", dst="lib", keep_path=False)
        self.copy("*TraceAPI.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
