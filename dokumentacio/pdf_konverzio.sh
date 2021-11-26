#!/bin/sh
#https://learnbyexample.github.io/customizing-pandoc/
pandoc dokumentacio.md -f gfm -V linkcolor:blue -V geometry:margin=2cm --toc -o dokumentacio.pdf
