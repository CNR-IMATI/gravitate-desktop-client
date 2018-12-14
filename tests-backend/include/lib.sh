#!/bin/bash

function check_params() {
    if [ -z $1 ]
    then
      echo "Usage: $(basename $0) <user>:<password>"
      exit
    fi
}

function try_print_json() {
    json=$1
    
    if jq . >/dev/null 2>&1 <<<"$json"; then
        echo "<=="
        echo "$json" | jq -C
    else
        echo "<== Error: $json"
    fi
}  

function curl_get() {
    auth=$1
    url=$2
    shift; shift
    curl -sv --user "$auth" -X GET "$url" "$@"
}
