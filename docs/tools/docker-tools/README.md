# Using the docker image

## Setup

### Install Docker
If you don't already have Docker installed, [install Docker](https://docs.docker.com/get-docker/).

### Building the docker image

To build the docker image, first, update the VERSION variable below (please use semantic versioning). Add a [release note](#release-notes).

Specify the ruby version to match the latest stable - https://www.ruby-lang.org/en/downloads/

macOS and Linux:

```bash
VERSION="1.0.3"
VOLUME="stlab.libraries"
RUBY_VERSION="3.2.2"
```

Windows:

```powershell
$VERSION="1.0.3"
$VOLUME="stlab.libraries"
$RUBY_VERSION="3.2.2"

$PSDefaultParameterValues = @{'Out-File:Encoding' = 'Ascii'}
```

```
echo $VERSION > ./docs/tools/docker-tools/VERSION
echo $RUBY_VERSION > ./docs/.ruby-version

# build the base image, no-cache is used so the latest tools are installed
docker build --build-arg RUBY_VERSION=$RUBY_VERSION --file ./docs/tools/docker-tools/Dockerfile --target base --tag $VOLUME . --no-cache

# update the docs environment (see below for using local theme)
docker run --mount type=bind,source="$(pwd)",target=/mnt/host --tty --interactive $VOLUME bash
```

At the docker prompt, execute the following:

```
cd /mnt/host
git config --global --add safe.directory /mnt/host
./docs/tools/docs/update.sh --lock
exit

# build the final image
docker build --build-arg RUBY_VERSION=$RUBY_VERSION --file ./docs/tools/docker-tools/Dockerfile --target full --tag $VOLUME .
```

## Running the Docker image

To run the docker image, execute the following.

```
docker run --mount type=bind,source="$(pwd)",target=/mnt/host --tty --interactive --publish 3000-3001:3000-3001 $VOLUME bash
```

This should leave you at a bash prompt that looks like this:

```
app@fc7590a63ba3:~$
```

The hex number is the docker image container ID and may be different. Going foreward I refer to this as the _docker_ prompt to distinguish it from the _local_ prompt.

## Build the documentation site

To build or rebuild the complete documentation site locally, execute the following from the docker prompt:

```
cd /mnt/host
./docs/tools/docs/prepare.sh
```

## Run a local server for the site

Once the site has been prepared, you can run it to see how it looks. From the docker prompt enter:

```
cd /mnt/host
./docs/tools/docs/start.sh
```

To view the site, open a browser to `http://localhost:3000`. The site will auto-rebuild and refresh as files are changed.

## Tips

If you want to open another terminal on the running image use:

```
docker ps
docker exec -it <container id> bash
```

To test a local copy of the Jekyll theme

Edit Gemfile
Edit _config.yml

```
docker run --mount type=bind,source="$(pwd)",target=/mnt/host \
    --mount type=bind,source=$HOME/Projects/github.com/adobe/hyde-theme,target=/mnt/themes \
    --tty --interactive --publish 3000-3001:3000-3001 \
    $VOLUME bash
```

### Release Notes

- 1.0.0 - Initial release for Jekyll
- 1.0.1 - Updating tool set
- 1.0.2 - Updating in for Hyde 2.0
- 1.0.3 - Updating Jekyll to 4.2.0 for new Hyde and moving to GitHub Actions.
