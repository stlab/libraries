# Copyright 2020 Adobe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import glob
import json
import os
import re
from conans import ConanFile, CMake, tools
from conans.errors import ConanInvalidConfiguration, ConanException

class StlabLibrariesConan(ConanFile):
    name = "stlab"
    version = "1.5.4"       #TODO(fernando): see how to automate the version number
    license = "http://www.boost.org/users/license.html"
    url = "https://github.com/stlab/libraries"
    description = "Software Technology Lab (stlab) libraries"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "CMakeLists.txt", "LICENSE", "stlab/*", "cmake/*", "test/*"

    def requirements(self):
        self.requires("boost/1.75.0@")

    def configure(self):
        ConanFile.configure(self)
        
        self.options["boost"].shared=True
        self.options["boost"].without_chrono=False
        self.options["boost"].without_system=False
        self.options["boost"].without_timer=False
        self.options["boost"].without_test=False
        self.options["boost"].without_exception = False # required by Boost.Test

        self.options["boost"].without_atomic = True
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
        self.options["boost"].without_thread = True
        self.options["boost"].without_type_erasure = True
        self.options["boost"].without_wave = True

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        if not self.settings.compiler.cppstd:
            cmake.definitions["CMAKE_CXX_STANDARD"] = 17
        cmake.configure()
        cmake.build()
        cmake.test(output_on_failure=True)

    def imports(self):
        self.copy("*.dll", "./bin", "bin")
        self.copy("*.dylib", "./bin", "lib")
