#!/bin/bash

find . -name "*.*" -print0 | while read -r -d $'\0' file
do 
    if [ "${file##*.}" == "sh" ];
    then
        shellcheck "$file"
    fi
done