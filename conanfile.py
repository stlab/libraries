# Copyright 2020 Adobe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import glob
import json
import os
import re
from conans import ConanFile, CMake, tools
from conans.errors import ConanInvalidConfiguration, ConanException

def option_on_off(option):
    return "ON" if option else "OFF"

class StlabLibrariesConan(ConanFile):
    name = "stlab"
    version = "1.5.5"       #TODO(fernando): see how to automate the version number
    license = "http://www.boost.org/users/license.html"
    url = "https://github.com/stlab/libraries"
    description = "Software Technology Lab (stlab) libraries"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "CMakeLists.txt", "LICENSE", "stlab/*", "cmake/*", "test/*"

    options = {
        "testing": [True, False],
        "coverage": [True, False],
        "boost_optional": [True, False],
        "boost_variant": [True, False],
        "coroutines": [True, False],
        "task_system": ["portable", "libdispatch", "emscripten", "pnacl", "windows", "auto"],
    }

    default_options = {
        "testing": False,
        "coverage": False,
        "boost_optional": False,
        "boost_variant": False,
        "coroutines": False,
        "task_system": "auto",
    }

    def _log(self, str):
        self.output.info(str)
        self.output.warn(str)

    def _use_boost(self):
        return self.options.testing or \
                self.options.boost_optional or \
                self.options.boost_variant

    def _requires_libdispatch(self):
        # On macOS it is not necessary to use the libdispatch conan package, because the library is included in the OS.
        return self.options.task_system == "libdispatch" and \
               self.settings.os != "Macos"

    def requirements(self):
        if self._use_boost():
            self.requires("boost/1.75.0@")

        if self._requires_libdispatch():
            self.requires("libdispatch/5.3.2@")

    def package_id(self):
        ConanFile.package_id(self)
        self.info.header_only()
        self.info.options.testing = "ANY"
        self.info.options.coverage = "ANY"
        self.info.options.boost_optional = "ANY"
        self.info.options.boost_variant = "ANY"
        self.info.options.coroutines = "ANY"

    def _configure_boost(self):
        self.options["boost"].shared = True

        self.options["boost"].without_system = False
        self.options["boost"].without_test = not self.options.testing
        self.options["boost"].without_exception = not self.options.testing # required by Boost.Test

        self.options["boost"].without_atomic = True
        self.options["boost"].without_chrono = True
        self.options["boost"].without_container = True
        self.options["boost"].without_context = True
        self.options["boost"].without_contract = True
        self.options["boost"].without_coroutine = True
        self.options["boost"].without_date_time = True
        self.options["boost"].without_fiber = True
        self.options["boost"].without_filesystem = True
        self.options["boost"].without_graph = True
        self.options["boost"].without_graph_parallel = True
        self.options["boost"].without_iostreams = True
        self.options["boost"].without_json = True
        self.options["boost"].without_locale = True
        self.options["boost"].without_log = True
        self.options["boost"].without_math = True
        self.options["boost"].without_mpi = True
        self.options["boost"].without_nowide = True
        self.options["boost"].without_program_options = True
        self.options["boost"].without_python = True
        self.options["boost"].without_random = True
        self.options["boost"].without_regex = True
        self.options["boost"].without_serialization = True
        self.options["boost"].without_stacktrace = True
        self.options["boost"].without_timer = True
        self.options["boost"].without_thread = True
        self.options["boost"].without_type_erasure = True
        self.options["boost"].without_wave = True

    def _fix_boost_components(self):
        if self.settings.os != "Macos": return
        if self.settings.compiler != "apple-clang": return
        if float(str(self.settings.compiler.version)) >= 12: return

        #
        # On Apple we have to force the usage of boost.variant, because Apple's implementation of C++17 is not complete.
        #
        self._log("Apple-Clang versions less than 12 do not correctly support std::optional or std::variant, so we will use boost::optional and boost::variant instead.")
        self.options.boost_optional = True
        self.options.boost_variant = True

    # TODO(fernando): pnacl
    def _default_task_system(self):
        if self.settings.os == "Macos":
            return "libdispatch"

        if self.settings.os == "Windows":
            return "windows"

        if self.settings.os == "Emscripten":
            return "emscripten"

        return "portable"

    def _configure_task_system_auto(self):
        self.options.task_system = self._default_task_system()

    def _configure_task_system_libdispatch(self):
        if self.settings.os == "Linux":
            if self.settings.compiler != "clang":
                self.options.task_system = self._default_task_system()
                self._log("Libdispatch requires Clang compiler on Linux. The task system is changed to {}.".format(self.options.task_system))
                return

        elif self.settings.os != "Macos":
            self.options.task_system = self._default_task_system()
            self._log("Libdispatch is not supported on {}. The task system is changed to {}.".format(self.settings.os, self.options.task_system))
            return

    def _configure_task_system_windows(self):
        if self.settings.os != "Windows":
            self.options.task_system = self._default_task_system()
            self._log("Libdispatch is not supported on {}. The task system is changed to {}.".format(self.settings.os, self.options.task_system))
            return

    def _configure_task_system_emscripten(self):
        if self.settings.os != "Emscripten":
            self.options.task_system = self._default_task_system()
            self._log("Libdispatch is not supported on {}. The task system is changed to {}.".format(self.settings.os, self.options.task_system))
            return

    def _configure_task_system(self):
        if self.options.task_system == "auto":
            self._configure_task_system_auto()
        elif self.options.task_system == "libdispatch":
            self._configure_task_system_libdispatch()
        elif self.options.task_system == "windows":
            self._configure_task_system_windows()
        elif self.options.task_system == "emscripten":
            self._configure_task_system_emscripten()

    def configure(self):
        ConanFile.configure(self)

        self._fix_boost_components()

        if self._use_boost():
            self._configure_boost()

        self._configure_task_system()
        self.output.info("Task System: {}.".format(self.options.task_system))

    def build(self):
        if self.options.testing:
            cmake = CMake(self)
            cmake.verbose = True
            if not self.settings.compiler.cppstd:
                cmake.definitions["CMAKE_CXX_STANDARD"] = 17
                cmake.definitions["stlab.testing"] = option_on_off(self.options.testing)
                cmake.definitions["stlab.coverage"] = option_on_off(self.options.coverage)
                cmake.definitions["stlab.boost_variant"] = option_on_off(self.options.boost_variant)
                cmake.definitions["stlab.boost_optional"] = option_on_off(self.options.boost_optional)
                cmake.definitions["stlab.coroutines"] = option_on_off(self.options.coroutines)
                cmake.definitions["stlab.task_system"] = self.options.task_system

            cmake.configure()
            cmake.build()
            cmake.test(output_on_failure=True)

    def package(self):
        self.copy("*.hpp")

    def imports(self):
        self.copy("*.dll", "./bin", "bin")
        self.copy("*.dylib", "./bin", "lib")

