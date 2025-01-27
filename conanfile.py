from conan import ConanFile
from conan.tools.cmake import CMake

class RTypeConan(ConanFile):
    name = "rtype"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        if self.settings.os == "Windows":
            self.requires("b2/5.2.0")
            self.requires("boost/1.86.0")
            self.requires("sfml/2.6.2")
            self.requires("nlohmann_json/3.11.3")

    def configure(self):
        if self.settings.os == "Windows":
            self.options["boost"].without_stacktrace = True
            self.options["boost"].without_url = True
            self.options["boost"].without_shared = True

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()