#!/usr/bin/env bash

cd .. && diff -u build/index.js ~/.config/tiled/extensions/leveltest.js > patching/dist.patch