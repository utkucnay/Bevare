@echo off
mkdir .out
cmake -S . -O .out\ -G Ninja -Wno-dev -DCMAKE_POLICY_VERSION_MINIMUM=3.5
ninja -C %cd%\.out\
