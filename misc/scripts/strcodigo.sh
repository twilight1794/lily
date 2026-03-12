#!/bin/sh
if [ ${#} -lt 1 ]; then return 2; fi
base_dir="$(git rev-parse --show-toplevel 2>/dev/null)"
if [ -z "$base_dir" ]; then return 3; fi
ret_val=0
for c in "$@"; do
    val=$(awk "BEGIN { FS=\",[[:blank:]]*/\\*\\*<[[:blank:]]*\"; COD=0; CODF=1; } /COD_/{if (\"$c\"==COD) { gsub(\"^[[:blank:]]*\", \"\", \$1); gsub(\"[[:blank:]]*\\*/\",\"\", \$2); print \"\\x1b[1m\"\$1\":\\x1b[0m \"\$2; CODF=0; } COD++;} END { exit CODF; }"< "$base_dir/src/common/estado.h")
    if [ "$?" -eq 1 ]; then ret_val=1; fi
    if [ -n "$val" ]; then
        if [ ${#} -gt 1 ]; then printf "\e[2m%s:\e[0m " "$c"; fi
        echo "$val"
    fi
done
return $ret_val;
