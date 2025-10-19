# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring,line-too-long
import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, CMakeDeps, cmake_layout
from conan.tools.files import copy


class IoUringSoupBinCppRecipe(ConanFile):
    name = "io-uring-soup-bin"
    version = "0.0.1-dev"
    package_type = "library"

    # Optional metadata
    license = "Apache-2.0"
    url = "https://github.com/lshlyapnikov/io-uring-soup-bin"
    homepage = "https://github.com/lshlyapnikov/io-uring-soup-bin"
    description = "C++ Lock-free Demultiplexer Queue"
    topics = ("libraries", "cpp", "io_uring", "SoupBinTCP", "soup", "bin", "tcp", "low-latency", "networking")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "test/*"

    def requirements(self):
        self.requires("boost/1.88.0")
        self.requires("xxhash/0.8.3")
        self.requires("hdrhistogram-c/0.11.8")
        self.requires("gtest/1.16.0")
        self.requires("rapidcheck/cci.20231215")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
        self.options["boost"].shared = False

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        # Create the include directory in the package folder
        for package_dir in ["core", "example", "util", "test"]:
            copy(self, "*.h",
                src=os.path.join(self.source_folder, f"src/{package_dir}"),
                dst=os.path.join(self.package_folder, f"include/{package_dir}")
            )
        # Create the lib directory in the package folder
        copy(self, pattern="*.a",
             src=self.build_folder,
             dst=os.path.join(self.package_folder, "lib"),
             keep_path=False
        )

    def package_info(self):
        self.cpp_info.libs = ["io-uring-soup-bin"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.includedirs = ["include"]
