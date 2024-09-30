#!/bin/bash

# Default values:
opt_l=false
opt_d='no value given'

# It's the : after d that signifies that it takes an option argument.

while getopts ld: opt; do
    case $opt in
        l) opt_l=true ;;
        d) opt_d=$OPTARG ;;
        *) echo 'error in command line parsing' >&2
           exit 1
    esac
done

shift "$(( OPTIND - 1 ))"

# Command line parsing is done now.
# The code below acts on the used options.
# This code would typically do sanity checks,
# like emitting errors for incompatible options, 
# missing options etc.

"$opt_l" && echo 'Got the -l option'

printf 'Option -d: %s\n' "$opt_d"

if $opt_l; then
    echo 'If statement executed'
fi