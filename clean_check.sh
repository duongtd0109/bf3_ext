#!/bin/bash

find . -name "*.*" -print0 | while read -r -d $'\0' file
do 
    if [ "${file##*.}" == "cpp" ]; 
    then
        echo "$file"
        cpplint --filter=-whitespace,-legal/copyright "$file"
    fi
    
    if [ "${file##*.}" == "h" ];
    then
        echo "$file"
        cpplint --filter=-whitespace,-legal/copyright "$file"
    fi
done