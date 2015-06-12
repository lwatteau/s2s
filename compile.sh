#!/bin/bash

g++ -std=c++14 -lboost_program_options -o bin/s2s src/main.cpp src/config.cpp src/parser.cpp src/cmdline_parser.cpp
