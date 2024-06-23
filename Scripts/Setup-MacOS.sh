#!/bin/bash

pushd ..
Vendor/Binaries/Premake/MacOS/premake5 --file=Build.lua xcode4
popd
