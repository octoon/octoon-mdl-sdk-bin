import os
import shutil

from conans import ConanFile, CMake, tools
from conans.errors import ConanInvalidConfiguration
from conan.tools.microsoft import msvc_runtime_flag

required_conan_version = ">=1.35.0"

class MdlSdkConan(ConanFile):
    name = "mdl-sdk"
    version = "2021.1.2"
    description = "<Description of MdlSdk here>"
    url = "None"
    license = "None"
    author = "None"
    topics = None

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": False,
    }

    def source(self):
        pass

    def export_sources(self):
        shutil.copytree("binaries/mdl-sdk-349500.8766a", os.path.join(self.export_sources_folder, "mdl-sdk-bin"))

    @property
    def _source_subfolder(self):
        return "source_subfolder"

    @property
    def _build_subfolder(self):
        return "build_subfolder"
    
    @property
    def _is_msvc(self):
        return str(self.settings.compiler) in ["Visual Studio", "msvc"]
    
    def validate(self):
        if self.settings.os not in ["Windows", "Linux", "Macos", "Android", "iOS"]:
            raise ConanInvalidConfiguration("Current os is not supported")

        build_type = self.settings.build_type
        if build_type not in ["Debug", "RelWithDebInfo", "Release"]:
            raise ConanInvalidConfiguration("Current build_type is not supported")

        if self.settings.compiler == "Visual Studio" and tools.Version(self.settings.compiler.version) < 9:
            raise ConanInvalidConfiguration("Visual Studio versions < 9 are not supported")

        if self._is_msvc:
            allowed_runtimes = ["MDd", "MTd"] if build_type == "Debug" else ["MD", "MT"]
            if msvc_runtime_flag(self) not in allowed_runtimes:
                raise ConanInvalidConfiguration(
                    "Visual Studio runtime {0} is required for {1} build type".format(
                        " or ".join(allowed_runtimes),
                        build_type,
                    )
                )

    def build(self):
        pass

    def package(self):
        mdl_bin_subfolder = os.path.join(self.build_folder, "mdl-sdk-bin")

        self.copy(pattern="mdl-sdk-bin/license.txt", dst="licenses", src=mdl_bin_subfolder, keep_path=False)
        self.copy("*.h", dst="include", src=os.path.join(mdl_bin_subfolder, "include"))
        if self.settings.os == "Windows":
            self.copy("*.lib", dst="lib", src=os.path.join(mdl_bin_subfolder, "nt-x86-64", "lib"),keep_path=False)
            self.copy("*.dll", dst="bin", src=os.path.join(mdl_bin_subfolder, "nt-x86-64", "lib"), keep_path=False)
            self.copy("*.exe", dst="bin", src=os.path.join(mdl_bin_subfolder, "nt-x86-64", "bin"), keep_path=False)
        elif self.settings.os == "linux" and self.settings.arch == "x86_64":
            self.copy("*.so", dst="lib", src=os.path.join(mdl_bin_subfolder, "linux-x86-64", "lib"),keep_path=False)
            self.copy("*", dst="bin", src=os.path.join(mdl_bin_subfolder, "linux-x86-64", "bin"), keep_path=False)
        else:
            raise ConanInvalidConfiguration("Current os/arch is not supported")

    def package_info(self):
        # self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.libs = ["mdl-sdk"]
