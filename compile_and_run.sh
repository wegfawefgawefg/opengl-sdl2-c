#!/bin/bash
export LD_LIBRARY_PATH=/usr/lib64
ninja -C ./build && ./build/main
