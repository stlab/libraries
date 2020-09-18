# Build from the latest ubuntu release
FROM ubuntu:latest AS base

# Install items requiring root access

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install --no-install-recommends -y \
    apt-utils \
    fswatch \
    g++ \
    git \
    libreadline-dev \
    make \
    npm \
    rbenv \
    wget \
    zlib1g-dev \
&& rm -rf /var/lib/apt/lists/*

RUN npm install -g npm@latest
RUN npm install -g browser-sync

# Create a user "app" so everything is not running at root
RUN useradd -ms /bin/bash app
USER app
WORKDIR /home/app

# Set UTF language (assumed by jekyll)
ENV LC_ALL "C.UTF-8"
ENV LANG "en_US.UTF-8"
ENV LANGUAGE "en_US.UTF-8"

# Install ruby environment
RUN echo 'eval "$(rbenv init -)"' >> ~/.bashrc
RUN mkdir -p "$(rbenv root)"/plugins
RUN git clone https://github.com/rbenv/ruby-build.git "$(rbenv root)"/plugins/ruby-build
#  Install latest ruby
RUN rbenv install $(rbenv install -l | grep -v - | tail -1)
RUN rbenv global  $(rbenv install -l | grep -v - | tail -1)
#  Install bundler in global ruby
RUN (eval "$(rbenv init -)"; gem install bundler)

# build the final files
FROM base AS full
USER app
WORKDIR /home/app

RUN mkdir ./install
WORKDIR ./install
COPY ./Gemfile .
COPY ./Gemfile.lock .
COPY ./.ruby-version .

RUN  (eval "$(rbenv init -)"; \
    rbenv install `cat .ruby-version`; \
    gem install bundler; \
    rbenv rehash; \
    bundle install)

WORKDIR /home/app

EXPOSE 3000 3001

# Add version file last to avoid cache invalidation for minor releases
ADD ./tools/docker-tools/VERSION .
