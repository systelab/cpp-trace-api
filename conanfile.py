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
    required_conan_version = ">=1.33.1"

    def requirements(self):
        self.requires("boost/1.85.0#0734cd0dd8fe650aa3ae64bb51bca54d")

    def build_requirements(self):
        self.build_requires("gtest/1.14.0#4372c5aed2b4018ed9f9da3e218d18b3")

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
