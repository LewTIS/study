#!/bin/bash



function func1
{ 
    local input_file=$1;
     if [ ! -f "$input_file" ]; then
        echo "Error:input file does not exist"
        exit 1
    fi

    while read -r line
        do
         echo $line;
    done < "$input_file"
    
}
input_file=""
while getopts "i:" opt 
do 
    case $opt in 
        i) 
            input_file=$OPTARG    
            ;;
        \?) 
            echo "$0:illegal option " >&2
            echo "Uage:$0 -i input_fileaaa" >&2
            exit 1
            ;;
        :)
            echo "$0:option -$opt requires an argument222" >&2
            echo "Uage:$0 -i input_file" >&2
            exit 1
            ;;
    esac
done
if [ -z "$input_file" ]; then
    echo "Usage: $0: [-i input_file]1111" 
    exit 1
fi
func1 "$input_file"
