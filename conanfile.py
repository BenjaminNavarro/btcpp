import os
from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake
from conan.tools.files import copy

class BtcppRecipe(ConanFile):
    name = "btcpp"
    version = "0.1.0"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    exports_sources = "libs*", "apps*", "tests*", "deps*0", "cmake*", "CMakeLists.txt", "LICENSE"

    def requirements(self):
        self.requires("catch2/3.15.3")
        self.requires("pugixml/1.16")
        self.requires("cppzmq/4.11.0")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "*.hpp", os.path.join(self.source_folder, "libs", "btcpp", "btcpp"), os.path.join(self.package_folder, "include", "btcpp"))
        copy(self, "*.hpp", os.path.join(self.source_folder, "libs", "btgui", "btgui"), os.path.join(self.package_folder, "include", "btgui"))
        copy(self, "*.hpp", os.path.join(self.source_folder, "libs", "btnet", "btnet"), os.path.join(self.package_folder, "include", "btnet"))
        copy(self, "*.so", self.build_folder, os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, "*.a", self.build_folder, os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, "*.dylib", self.build_folder, os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, "*.dll", self.build_folder, os.path.join(self.package_folder, "lib"), keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["btcpp", "btgui", "btnet"]