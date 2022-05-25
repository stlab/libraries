#!/usr/bin/env python

import argparse
import contextlib
import os
import subprocess
import sys

# From https://stackoverflow.com/questions/6194499/pushd-through-os-system
@contextlib.contextmanager
def pushd(new_dir):
    previous_dir = os.getcwd()
    os.chdir(new_dir)
    try:
        yield
    finally:
        os.chdir(previous_dir)

def shell_cmd(*cmd_components):
    process = subprocess.Popen([c for c in cmd_components])
    process.communicate()
    return process.returncode

def main(argv):
    parser = argparse.ArgumentParser()
    parser.add_argument('--cxx-std', nargs='?', const=17, type=int)
    args = parser.parse_args(argv[1:])

    print(args)

    os.makedirs("build_tests", exist_ok=True)
    with pushd("build_tests"):
        shell_cmd("conan", "install", "..", "--build=missing", "-s", "build_type=Debug", "-o", "testing=True", "-s", "compiler.cppstd={}".format(args.cxx_std))
        shell_cmd("cmake", "-G", "Unix Makefiles", "-D", "CMAKE_BUILD_TYPE=debug", "-D", "stlab_testing=ON", "..")
        shell_cmd("cmake", "--build", ".")
        shell_cmd("ctest", "-C", "Debug", "--output-on-failure")

if __name__ == "__main__":
    main(sys.argv)