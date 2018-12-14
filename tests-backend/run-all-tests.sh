#!/bin/bash

source ./include/lib.sh
source ./api/clipboard-api.sh
source ./api/metadata-api.sh
source ./api/sparql-query.sh

# Configuration

# Default hostname
HOST="https://gravitate-2.cytera.cyi.ac.cy"


# WARNING: Do not edit below this line unless you know what you are doing

AUTH=$1

check_params $AUTH

container_id="http://gravitate-2.datafnatic.com/container/setContainer/464c4c0a-4fd5-4d3b-b4c1-aa31a6762066"

test_clipboard $HOST $AUTH $container_id
test_metadata $HOST $AUTH
test_sparql_query $HOST $AUTH ./sparql/sparql-3d-metadata.rq
