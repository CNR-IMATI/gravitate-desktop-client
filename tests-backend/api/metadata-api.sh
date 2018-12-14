#!/bin/bash

test_metadata() {
    HOST=$1
    AUTH=$2
    
    ARTEFACT_ID="http://collection.britishmuseum.org/id/object/GAA58683"
    URL="${HOST}/gravapi/fragmentmetadata/ch?uri=${ARTEFACT_ID}"

    echo ">>> GET $URL..."

    # curl -X GET "$URL" --user "$AUTH" | jq -C
    #json_string=$(curl -sv -X GET "$URL" --user "$AUTH") # | jq -C
    json_string=$(curl_get $AUTH "$URL") # | jq -C
    
    try_print_json "$json_string"
    
    echo ""
}