from cpt.packager import ConanMultiPackager

if __name__ == "__main__":
    builder = ConanMultiPackager(build_policy = "missing", archs = ["x86_64"], options = ["stlab:testing=True"])
        # apple_clang_versions= ["12.0"]
    builder.add_common_builds()
    builder.run()
