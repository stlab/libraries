#!/usr/bin/env python

import argparse
import contextlib
import os
import platform
import shutil
import subprocess
import sys

PYENV_PYTHON_VERSION = '3.8.6'
CONAN_PACKAGE_TOOLS_VERSION = '0.37.0'
# CONAN_VERSION = '1.43.0'
CONAN_VERSION = '1.48.1'

DEFAULT_BUILD_CONFIG = 'Debug'
DEFAULT_BUILD_DIR = 'build'
DEFAULT_CXX_LANGUAGE_VERSION = 17


@contextlib.contextmanager
def pushd(new_dir):
    # From https://stackoverflow.com/questions/6194499/pushd-through-os-system
    previous_dir = os.getcwd()
    os.chdir(new_dir)
    try:
        yield
    finally:
        os.chdir(previous_dir)


def shell_cmd(cmd):
    # TODO: logging
    process = subprocess.Popen(cmd)
    process.communicate()
    return process.returncode


def detected_platform():
    return platform.system()


def executing_in_gh_actions():
    return "GITHUB_ACTIONS" in os.environ


def build_dir_context(args):
    os.makedirs(args.build_dir, exist_ok=True)
    return pushd(args.build_dir)


def install_tooling_dependencies(args):
    if args.platform == 'Darwin':
        shutil.which('brew') or shell_cmd(
            ["/bin/bash", "-c", "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"])
        shell_cmd(["brew", "outdated", "pyenv",
                  "||", "brew", "upgrade", "pyenv"])
        shutil.which('cmake') or shell_cmd(["brew", "install", "cmake"])
        shell_cmd(["$(pyenv init -)"])
        shell_cmd(["pyenv", "install", PYENV_PYTHON_VERSION])
        shell_cmd(["pyenv", "virtualenv", PYENV_PYTHON_VERSION, "conan"])
        shell_cmd(["pyenv", "rehash"])
        shell_cmd(["pyenv", "activate", "conan"])

    if args.platform == 'Darwin' or args.platform == 'Linux':
        shell_cmd(['pip', 'install', '--upgrade', 'pip'])
        shell_cmd(['pip', 'install', 'conan_package_tools=={}'.format(
            CONAN_PACKAGE_TOOLS_VERSION)])
        shell_cmd(['pip', 'install', 'conan=={}'.format(CONAN_VERSION)])
        shutil.which('cmake') or shell_cmd(["pip", "install", "cmake"])

    if args.platform == 'Windows':
        shell_cmd(["set", "PATH=%PYTHON%;%PYTHON%/Scripts/;%PATH%;"])
        shell_cmd(["python.exe", "--version"])
        shell_cmd(["cmake", "--version"])
        shell_cmd(["python.exe", "-m", "pip", "install", "--upgrade", "pip"])
        shell_cmd(["pip.exe", "install", "conan_package_tools=={}".format(
            CONAN_PACKAGE_TOOLS_VERSION)])
        shell_cmd(["pip.exe", "install", "conan=={}".format(CONAN_VERSION)])

    shell_cmd(['conan', '--version'])
    shell_cmd(['conan', 'user'])


def set_environment_variables(args):
    def local_setter(k, v):
        # TODO: logging
        os.environ[k] = v

    def gh_actions_setter(k, v):
        shell_cmd(["echo", "{}={}".format(k, v), ">>",
                  "$Env:GITHUB_ENV" if args.os == 'Windows' else "$GITHUB_ENV"])

    set = gh_actions_setter if executing_in_gh_actions() else local_setter

    if args.compiler == 'gcc':
        set("CONAN_GCC_VERSION", args.compiler_version)
        set("CONAN_DOCKER_IMAGE", "conanio/gcc{}".format(args.compiler_version))
    elif args.compiler == 'clang':
        set("CONAN_CLANG_VERSIONS", args.compiler_version)
        set("CONAN_DOCKER_IMAGE", "conanio/clang{}".format(args.compiler_version))
    elif args.compiler == 'apple-clang':
        set("CONAN_APPLE_CLANG_VERSIONS", args.compiler_version)
    elif args.compiler == 'Visual Studio':
        set("CONAN_VISUAL_VERSIONS", args.compiler_version)
        set("CMAKE_TOOLSET", args.cmake_toolset)


def install_stlab_dependencies(args):
    with build_dir_context(args):
        # REVISIT - should we turn off testing=True for non-test builds?
        shell_cmd(["conan", "install", "..", "--build=missing", "-s", "build_type={}".format(
            args.build_config), "-o", "testing=True", "-s", "compiler.cppstd={}".format(args.cxx_std)])


def install_command(args):
    print("INSTALL: {}".format(args))
    install_tooling_dependencies(args)
    set_environment_variables(args)
    install_stlab_dependencies(args)
    pass


def build_command(args):
    print("BUILD: {}".format(args))
    with build_dir_context(args):
        shell_cmd(["cmake", "-G", "Unix Makefiles", "-D",
                  "CMAKE_BUILD_TYPE={}".format(args.build_config), "-D", "stlab_testing=ON", ".."])
        shell_cmd(["cmake", "--build", "."])


def test_command(args):
    print("TEST: {}".format(args))
    with build_dir_context(args):
        shell_cmd(["ctest", "-C", args.build_config, "--output-on-failure"])


def main(argv):
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command')

    install = subparsers.add_parser(
        'install', help='Install dependencies and set required env vars.')
    build = subparsers.add_parser(
        'build', help='Generate project(s) and compile.')

    # TODO make test auto-run install and build, if it hasn't been run yet.
    test = subparsers.add_parser('test', help='Run the test suite.')

    # Universal Arguments
    for cmd in [install, build, test]:
        cmd.add_argument("--os", type=str)
        cmd.add_argument("--platform", type=str, default=detected_platform())
        cmd.add_argument("--build-dir", type=str, default=DEFAULT_BUILD_DIR)
        cmd.add_argument("--build-config", type=str,
                         default=DEFAULT_BUILD_CONFIG)
        cmd.add_argument("--compiler", type=str)
        cmd.add_argument("--compiler-version", type=str)
        cmd.add_argument("--cmake-toolset", type=str)
        cmd.add_argument('--cxx-std', nargs='?', type=int,
                         default=DEFAULT_CXX_LANGUAGE_VERSION, help="[14, 17] Default {}".format(DEFAULT_CXX_LANGUAGE_VERSION))

    # Command-specific Arguments
    install.add_argument('--dry-run', action="store_true",
                         help="Show the required install steps, but do not perform any installations.")

    args = parser.parse_args(argv[1:])

    {
        'install': install_command,
        'build': build_command,
        'test': test_command,
        None: lambda _: parser.print_help()
    }[args.command](args)


if __name__ == "__main__":
    main(sys.argv)
