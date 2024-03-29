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
    options = {"boost": ["1.66.0", "1.67.0", "1.72.0", "1.75.0", "1.76.0"], "gtest": ["1.7.0", "1.8.1", "1.10.0"]}
    default_options = {"boost":"1.76.0", "gtest":"1.10.0"}
    exports_sources = "*"
    required_conan_version = ">=1.33.1"

    def configure(self):
        self.options["boost"].shared = True

    def requirements(self):
        if self.options.boost == "1.66.0":
            self.requires("boost/1.66.0@conan/stable")
        elif self.options.boost == "1.67.0":
            self.requires("boost/1.67.0@conan/stable")
        elif self.options.boost == "1.72.0":
            self.requires("boost/1.72.0#50e13743ed3c867fe95f151e113bc1ba")
        elif self.options.boost == "1.75.0":
            self.requires("boost/1.75.0#32c81e4e89c54b95b2c2c198fe3cb65f")
        elif self.options.boost == "1.76.0":
            self.requires("boost/1.76.0#329123a826d0a53fef8318ec3e729a52")
        else:
            self.requires(("boost/%s") % self.options.boost)

    def build_requirements(self):
        if self.options.gtest == "1.7.0":
            self.build_requires("gtest/1.7.0@systelab/stable")
        elif self.options.gtest == "1.8.1":
            self.build_requires("gtest/1.8.1")
        elif self.options.gtest == "1.10.0":
            self.build_requires("gtest/1.10.0#0c895f60b461f8fee0da53a84d659131")
        else:
            self.build_requires(f"gtest/{self.options.gtest}")

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
