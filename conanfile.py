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
    version = "1.5.4"       #TODO(fernando): see how to automate the version number
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
    }

    default_options = {
        "testing": False,
        "coverage": False,
        "boost_optional": False,
        "boost_variant": False,
        "coroutines": False,
    }

    def _use_boost(self):
        return self.options.testing or \
                self.options.boost_optional or \
                self.options.boost_variant

    def requirements(self):
        if _use_boost():
            self.requires("boost/1.75.0@")

    def package_id(self):
        ConanFile.package_id(self)
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


    def _check_boost_components(self):
        conans.output.info(f"self.settings.os:               {self.settings.os}")
        conans.output.info(f"self.settings.compiler:         {self.settings.compiler}")
        conans.output.info(f"self.settings.compiler.version: {self.settings.compiler.version}")

        # "compiler": "apple-clang","version": "12","os": "macos-11.0"

#     if(APPLE AND (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
#   set( stlab.boost_variant ON )
# endif()


    def configure(self):
        ConanFile.configure(self)

        _check_boost_components()

        if _use_boost():
            _configure_boost()

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        if not self.settings.compiler.cppstd:
            cmake.definitions["CMAKE_CXX_STANDARD"] = 17
            cmake.definitions["stlab.testing"] = option_on_off(self.options.testing)
            cmake.definitions["stlab.coverage"] = option_on_off(self.options.coverage)
            cmake.definitions["stlab.boost_variant"] = option_on_off(self.options.boost_variant)
            cmake.definitions["stlab.boost_optional"] = option_on_off(self.options.boost_optional)
            cmake.definitions["stlab.coroutines"] = option_on_off(self.options.coroutines)

        cmake.configure()
        cmake.build()
        
        if self.options.testing:
            cmake.test(output_on_failure=True)

    def imports(self):
        self.copy("*.dll", "./bin", "bin")
        self.copy("*.dylib", "./bin", "lib")
