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


def run_cmd(cmd, shell=False):
    # TODO: logging
    process = subprocess.run(cmd, shell=shell)
    return process.returncode


def pip_install(platform, package):
    if package == 'pip':
        if platform == 'Windows':
            return run_cmd(["python.exe", "-m", "pip", "install", "--upgrade", "pip"])
        else:
            return run_cmd(['pip', 'install', '--upgrade', 'pip'])

    executable = "pip.exe" if platform == 'Windows' else 'pip'
    return run_cmd([executable, "install", package])


def detected_platform():
    return platform.system()


def detected_platform_compiler():
    return {
        'Darwin': ('apple-clang', '12.0'),
        'Linux': ('gcc', '10'),
        'Windows': ('Visual Studio', '16')
    }[detected_platform()]


def executing_in_gh_actions():
    return "GITHUB_ACTIONS" in os.environ


def build_dir_context(args):
    os.makedirs(args.build_dir, exist_ok=True)
    return pushd(args.build_dir)


# TODO
# def setup_command(args):
#     with pushd('build'):
#         # REVISIT(demarco) Redundant with install_stlab_dependencies? Can we converge  on a single command here?
#         run_cmd(["conan", "install", "..", "--build=missing", "-s", f"compiler={args.compiler}", "-s", f"compiler.cppstd={args.cxx_std}",
#                  "-s", f"build_type={args.config}", "-o", "testing=True"])
#     if args.ide == 'xcode':
#         pass
#     elif args.ide == 'msvc':
#         # TODO: check if this file exists?
#         run_cmd(["call", "%VC_ROOT%vcvarsall.bat", "x86_amd64"])

#         build_dir = "build_release" if args.config == 'release' else 'build_debug'
#         compiler_runtime = "MD" if args.config == 'release' else 'MDd'

#         with pushd(build_dir):
#             run_cmd(["conan", "install", "..", "-s",
#                     f"compiler={args.compiler}", "-s", f"compiler.version={args.compiler_version}", "-s", f"build_type={args.config}", "-s", f""])


def install_tooling_dependencies(args):
    if args.platform == 'Darwin':
        shutil.which('brew') or run_cmd(
            ["/bin/bash", "-c", "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"])

        if run_cmd(["brew", "outdated", "pyenv"]):
            run_cmd(["brew", "upgrade", "pyenv"])

        if run_cmd(["brew", "outdated", "pyenv-virtualenv"]):
            run_cmd(["brew", "install", "pyenv-virtualenv"])

        if shutil.which('pyenv'):
            subprocess.run(';'.join([
                "eval \"$(pyenv init --path)\"",
                "eval \"$(pyenv init -)\"",
                f"pyenv install {PYENV_PYTHON_VERSION}",
                f"pyenv virtualenv {PYENV_PYTHON_VERSION} conan",
                "pyenv rehash",
                "pyenv activate conan"
            ]), shell=True)

    if args.platform == 'Windows':
        run_cmd(["set", "PATH=%PYTHON%;%PYTHON%/Scripts/;%PATH%;"], shell=True)
        run_cmd(["python.exe", "--version"])
        run_cmd(["cmake", "--version"])

    pip_install(args.platform, "pip")
    pip_install(
        args.platform, f'conan_package_tools=={CONAN_PACKAGE_TOOLS_VERSION}')
    pip_install(args.platform, f'conan=={CONAN_VERSION}')
    shutil.which('cmake') or pip_install(args.platform, "cmake")
    shutil.which('ninja') or pip_install(args.platform, "ninja")

    run_cmd(['conan', '--version'])
    run_cmd(['conan', 'user'])


def set_environment_variables(args):
    def local_setter(k, v):
        # TODO: logging
        os.environ[k] = v

    def gh_actions_setter(k, v):
        run_cmd(["echo", f"{k}={v}", ">>",
                 "$Env:GITHUB_ENV" if args.platform == 'Windows' else "$GITHUB_ENV"])

    set = gh_actions_setter if executing_in_gh_actions() else local_setter

    if args.compiler == 'gcc':
        set("CONAN_GCC_VERSION", args.compiler_version)
        set("CONAN_DOCKER_IMAGE", f"conanio/gcc{args.compiler_version}")
    elif args.compiler == 'clang':
        set("CONAN_CLANG_VERSIONS", args.compiler_version)
        set("CONAN_DOCKER_IMAGE", f"conanio/clang{args.compiler_version}")
    elif args.compiler == 'apple-clang':
        set("CONAN_APPLE_CLANG_VERSIONS", args.compiler_version)
    elif args.compiler == 'Visual Studio':
        set("CONAN_VISUAL_VERSIONS", args.compiler_version)
        set("CMAKE_TOOLSET", args.cmake_toolset)


def install_stlab_dependencies(args):
    with build_dir_context(args):
        # REVISIT - should we turn off testing=True for non-test builds?
        run_cmd(["conan", "install", "..", "--build=missing", "-s", f"compiler={args.compiler}", "-s", f"compiler.cppstd={args.cxx_std}",
                 "-s", f"build_type={args.config}", "-o", "testing=True"])


def install_command(args):
    print(f"INSTALL: {args}")
    install_tooling_dependencies(args)
    set_environment_variables(args)
    install_stlab_dependencies(args)


def build_command(args):
    print(f"BUILD: {args}")
    with build_dir_context(args):
        run_cmd(["cmake", "-G", "Ninja", "-D",
                 f"CMAKE_BUILD_TYPE={args.config}", "-D", "stlab_testing=ON", ".."])
        run_cmd(["cmake", "--build", "."])


def test_command(args):
    print(f"TEST: {args}")
    with build_dir_context(args):
        run_cmd(["ctest", "-C", args.config, "--output-on-failure"])


def main(argv):
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command')

    # setup = subparsers.add_parser(
    #     'setup', help="Setup platform-specific project files")
    # setup_project = setup.add_subparsers(dest='ide')
    # xcode = setup_project.add_parser('xcode')
    # msvc = setup_project.add_parser('msvc')

    # for cmd in [xcode, msvc]:
    #     cmd.add_argument('--cxx-std', choices={14, 17},
    #                      default=DEFAULT_CXX_LANGUAGE_VERSION, help=f"C++ Version. Default: {DEFAULT_CXX_LANGUAGE_VERSION}")

    install = subparsers.add_parser(
        'install', help='Install dependencies and set required env vars.')
    build = subparsers.add_parser(
        'build', help='Generate project(s) and compile.')
    # TODO make test auto-run install and build, if it hasn't been run yet.
    test = subparsers.add_parser('test', help='Run the test suite.')

    (default_compiler, default_compiler_version) = detected_platform_compiler()

    # Universal Arguments
    for cmd in [install, build, test]:
        cmd.add_argument("--build-dir", type=str, default=DEFAULT_BUILD_DIR)
        cmd.add_argument("--cmake-toolset", type=str)
        cmd.add_argument("--compiler", type=str,
                         default=default_compiler)
        cmd.add_argument("--compiler-version", type=str,
                         default=default_compiler_version)
        cmd.add_argument("--config", choices={"debug", "release"},
                         default=DEFAULT_BUILD_CONFIG)
        cmd.add_argument('--cxx-std', nargs='?', choices={14, 17},
                         default=DEFAULT_CXX_LANGUAGE_VERSION, help=f"[14, 17] Default {DEFAULT_CXX_LANGUAGE_VERSION}")
        cmd.add_argument(
            "--platform", choices={"Darwin", "Linux", "Windows"}, default=detected_platform())

    args = parser.parse_args(argv[1:])
    if args.cmake_toolset == 'None':
        args.cmake_toolset = None

    {
        # 'setup': setup_command,
        'install': install_command,
        'build': build_command,
        'test': test_command,
        None: lambda _: parser.print_help()
    }[args.command](args)


if __name__ == "__main__":
    main(sys.argv)
