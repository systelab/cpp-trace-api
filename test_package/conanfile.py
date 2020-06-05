from conans import ConanFile, CMake


class TraceAPITestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package"
    options = {"boost": ["1.66.0", "1.67.0", "1.71.0"], "gtest": ["1.7.0", "1.8.1", "1.10.0"]}
    default_options = {"boost":"1.71.0", "gtest":"1.10.0"}

    def configure(self):
        self.options["TraceAPI"].boost = self.options.boost
        self.options["TraceAPI"].gtest = self.options.gtest

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        cmake = CMake(self)
        cmake.test()
