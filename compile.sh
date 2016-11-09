#!/bin/zsh

# Was too lazy to write a makefile :(

function log {
	echo "[+] $1"
}

log "Cleaning objects ..."

mkdir obj
rm obj/*
rm dump

for file in src/*
do
	obj=$(echo $file | sed "s/src/obj/" | sed "s/\.c/\.o/")
	log "Compiling $file -> $obj"
	gcc $file -c -o  $obj -Wall -std=c99
done

log "Compiling final executable ..."

gcc obj/* -o dump
