# STLab CI Configuration

The CI configuration in `workflows/build_and_test.yml` strives to be a minimal wrapper around the root-level `stlab.py` script, which will be invoked thrice (once each for `install`, `build`, and `test` subcommands) for each configuration listed in `.matrix.json`.

Please avoid adding platform-specific build configuration directly in `build_and_test.yml` - this makes it difficult for developers to reliably reproduce our build configuration on a new machine. Instead, add the necessary platform-specifics to the root-level `stlab.py`, so the CI system, and our developers, can benefit from the change. 

## Supported Versions

Our CI system supports the matrix of operating systems, compilers, and compiler versions listed in `./matrix.json`. Other platforms may be supported but are not actively maintained.

## Running Locally

We're currently evaluating [nektos/act](https://github.com/nektos/act) for local verification of CI steps. Installation instructions can be found through the above link. It will require a Docker installation. 

**`act` only supports Linux builds**. You can run the Linux CI steps on any machine that supports Docker. This author cannot find a competitive alternative to `act` which does support macOS, Windows, and Linux.

Please watch [this issue](https://github.com/nektos/act/issues/97) for updates. Improvements to `act` to support Windows and macOS appear to be on [their roadmap](https://github.com/nektos/act/projects/1#card-36460391). 







