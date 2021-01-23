import copy
from cpt.packager import ConanMultiPackager

if __name__ == "__main__":
    builder = ConanMultiPackager(build_policy = "missing", archs = ["x86_64"], options = ["stlab:testing=True"])
        # apple_clang_versions= ["12.0"]
    builder.add_common_builds()

    filtered_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:
    
        if settings["compiler"] == "clang":
            opts_libdispatch = copy.deepcopy(options)
            opts_libdispatch["stlab:task_system"] = "libdispatch"
            filtered_builds.append([settings, opts_libdispatch, env_vars, build_requires, reference])

        filtered_builds.append([settings, options, env_vars, build_requires, reference])

    builder.builds = filtered_builds

    builder.run()
