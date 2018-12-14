#!/bin/bash

test_clipboard() {

    URL=$1
    AUTH=$2
    container_id=$3

    urls=(
      "$URL/clipgroups/groups"
      "$URL/clipgroups/clipboard"
      "$URL/clipgroups/container?uri=$container_id"
    )

    for url in ${urls[@]}; do
      echo ">>> GET $url"
      # json_string=`curl -sv -X GET "$url" --user "$AUTH"`
      json_string=$(curl_get $AUTH "$url")
      try_print_json "$json_string"
      echo ""
      echo ""
    done
}