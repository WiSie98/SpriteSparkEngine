#!/bin/bash

pushd ..
Vendor/Binaries/Premake/MacOS/premake5 --file=Build.lua vs2022
popd
