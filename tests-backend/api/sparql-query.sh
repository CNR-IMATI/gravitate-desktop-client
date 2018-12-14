#!/bin/bash

HOSTNAME="https://gravitate-1.cytera.cyi.ac.cy"
ACCEPT_HEADER="Accept: application/sparql-results+json"
CONTENT_TYPE_HEADER="Content-Type: application/sparql-query;charset=UTF-8"
# QUERY_FILE="query-demo-data.rq"
QUERY_FILE="sparql-3d-metadata.rq"

test_sparql_query() {

    HOST=$1
    AUTH=$2
    QUERY_FILE=$3
    URL="${HOST}/sparql"

    
    echo "Sending request to: $URL ..."
    echo "=== Header ==="
    echo "$CONTENT_TYPE_HEADER"
    echo "$ACCEPT_HEADER"
    echo ""

    echo "=== SPARQL Query ==="
    cat "$QUERY_FILE"
    echo ""

    echo "=== HTTP Reply ==="
    json_string=`curl -sv -X POST "$URL" \
    -H "$ACCEPT_HEADER" \
    -H "$CONTENT_TYPE_HEADER" \
    --user "$AUTH" \
    --data-binary "@$QUERY_FILE"`
    
    try_print_json "$json_string"
    echo ""
}