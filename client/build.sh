#!/bin/bash

if [[ -d "./build" ]]; then
	rm -rf build || exit 1
fi

if [[ ! -d "./build" ]]; then
	mkdir build
	pushd build || exit 1
	cmake ..
	make || exit 1
	popd || exit 1
fi