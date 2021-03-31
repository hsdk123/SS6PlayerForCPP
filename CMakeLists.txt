# SS6Player for Cpp (C++/ssfb2)
# 
# Copyright(C) Web Technology Corp.
#                      All rights reserved.

cmake_minimum_required (VERSION 3.8)

# Project-Name
project ("SS6PlayerForCpp_Sample")

# Composition
add_subdirectory("src")                 # Static-Library: libSS6PlayerForCpp
add_subdirectory("src/Application")     # Sample: Application-Main

