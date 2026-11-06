import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.files import collect_libs, copy

class TraceAPIConan(ConanFile):
    name = "trace-api"
    description = "C++ trace system based on boost log"
    url = "https://github.com/systelab/cpp-trace-api"
    homepage = "https://github.com/systelab/cpp-trace-api"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "json", "configuration", "settings")
    license = "MIT"
    settings = "os", "compiler", "build_type", "arch"
    required_conan_version = ">=2.0.0"

    def requirements(self):
        self.requires("boost/1.85.0")

    def build_requirements(self):
        self.test_requires("gtest/1.14.0")

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()

        build_type = str(self.settings.build_type)
        for dep in self.dependencies.values():
            for bindir in dep.cpp_info.bindirs:
                copy(self, "*.dll", src=bindir, dst=os.path.join(self.build_folder, "bin", build_type))
            for libdir in dep.cpp_info.libdirs:
                copy(self, "*.dll", src=libdir, dst=os.path.join(self.build_folder, "bin", build_type))
                copy(self, "*.dylib*", src=libdir, dst=os.path.join(self.build_folder, "bin"))
                copy(self, "*.so*", src=libdir, dst=os.path.join(self.build_folder, "bin"))

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder=self.source_folder)
        cmake.build()

    def package(self):
        copy(self, "*.h", src=os.path.join(self.source_folder, "src", "TraceAPI"),
             dst=os.path.join(self.package_folder, "include", "TraceAPI"))
        for pattern in ("*TraceAPI.lib", "*TraceAPI.pdb", "*TraceAPI.a"):
            copy(self, pattern, src=self.build_folder,
                 dst=os.path.join(self.package_folder, "lib"), keep_path=False)

    def package_info(self):
        self.cpp_info.libs = collect_libs(self)
        self.cpp_info.set_property("cmake_file_name", "TraceAPI")
        self.cpp_info.set_property("cmake_target_name", "TraceAPI::TraceAPI")
