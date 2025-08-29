# `stlab` Documentation

This site is available at [stlab.cc](https://stlab.cc).

Pull requests for typos, examples, and other improvements are welcome. To file an issue, please use the [libraries repository](https://github.com/stlab/libraries).

## Building the Documentation

To run a local Jekyll server, see the instructions in the docker-tools [README](../tools/docker-tools/README.md).

## Running Hyde in Docker

<!-- 
Configure the build as follows:

```
cmake --preset=hyde-build-docs
``` 
-->

Login to [GitHub Packages registry](https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-container-registry#authenticating-with-a-personal-access-token-classic)

```bash
echo $CR_PAT | docker login ghcr.io --password-stdin -u USERNAME 
```

Fetch the latest image and run it:

```bash
HYDE_VERSION=2.0.1
docker pull ghcr.io/adobe/hyde:$HYDE_VERSION

docker run --platform linux/x86_64 --mount type=bind,source="$(pwd)/..",target=/mnt/host \
    --tty --interactive \
    ghcr.io/adobe/hyde:$HYDE_VERSION bash
```

From the docker prompt

```
cd /mnt/host/libraries/docs
./generate_docs.sh
./about.sh
```

<!-- \[ this is from the old docs - need to update the docs and script.

> (or, simply `-u`) to generate the boilerplate for it. Then, fill in any fields marked as `__MISSING__`. Fields marked as `__OPTIONAL__` may be omitted.
> \] -->
