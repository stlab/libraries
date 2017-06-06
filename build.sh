#!/bin/bash

bundle exec jekyll build && htmlproofer ./_site
