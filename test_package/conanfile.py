import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class TraceAPITestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    required_conan_version = ">=2.0.0"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def layout(self):
        cmake_layout(self)

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
        cmake.configure()
        cmake.build()

    def test(self):
        cmake = CMake(self)
        cmake.test()
