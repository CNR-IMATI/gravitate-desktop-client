#!/bin/env python3

import rdflib
import os.path
import os
import textwrap
from string import Template
import re
from decimal import Decimal
from UnitInfo import UnitInfo

qudt_schemas = [
    "OVG_units-qudt-v1.1.ttl",
    "SCHEMA_QUDT-DATATYPES-v2.0.ttl",
    "SCHEMA_QUDT-SCIENCE-v2.0.ttl",
    "SCHEMA_QUDT-v2.0.ttl",
    "VOCAB_QUDT-DISCIPLINES-v2.0.ttl",
    "VOCAB_QUDT-UNITS-BASE-v2.0.ttl",
    "VOCAB_QUDT-UNITS-SPACE-AND-TIME-v2.0.ttl"
]

query1 = """PREFIX quantitykind: <http://qudt.org/vocab/quantitykind/>
    PREFIX qudt: <http://qudt.org/schema/qudt/>
    SELECT DISTINCT ?unit ?label ?abbreviation ?coefficient
    WHERE {
        ?unit qudt:hasQuantityKind ?kind;
            rdfs:label ?label;
            qudt:abbreviation ?abbreviation;
            qudt:conversionMultiplier ?coefficient.
       VALUES ?kind { quantitykind:Length quantitykind:Area quantitykind:Volume }
   }
"""

query2 = """PREFIX quantitykind: <http://qudt.org/vocab/quantitykind/>
    PREFIX qudt: <http://qudt.org/schema/qudt/>
    SELECT DISTINCT ?unit ?label ?abbreviation ?coefficient
    WHERE {
        ?unit rdf:type ?unitType;
            rdfs:label ?label;
            qudt:abbreviation ?abbreviation;
            qudt:conversionMultiplier ?coefficient.
            VALUES ?unitType { qudt:LengthUnit qudt:AreaUnit qudt:VolumeUnit }
   }
"""


def read_qudt_graph():
    g = rdflib.Graph()
    for s in qudt_schemas:
        if( not os.path.isfile(s) ):
            print("file '{}' not found".format(s))
            continue
        g.load(s, format='turtle')
    return g

def get_units(graph):
    res = graph.query(query2)
    return res

def format_enum(label):
    # Remove illegal characters
    label = label.replace('(', ' ')
    label = label.replace(')', ' ')

    # Remove extra spaces between words
    label = re.sub(' +',' ',label)
    label = label.strip()

    # Replace spaces with underscores
    label = label.replace(' ', '_')

    return label.upper()

def main():
    print("Loading QUDT ontology...")
    g = read_qudt_graph()
    print("{} triples read".format( len(g) ))

    units = get_units(g)

    output_file = "../qudt_units.h"

    configure_header("qudt_units.h.in", output_file, units)

    #write_header_file(units, filename)
    print("Generated '{}'".format(output_file))

def configure_header(input_file, output_file, units):

    with open(input_file, "r") as f:
        file_content = f.read()

    if(file_content == None):
        return

    tpl = Template(file_content)

    mapping = generate_mapping(units)

    output_code = tpl.substitute(mapping)

    with open(output_file, "w") as f:
        f.write(output_code)

def remove_duplicates(l):
    return sorted(list(set(l)))


def generate_mapping(units):

    info = UnitInfo()

    info.add_field("enum", "Unit", "unit")
    info.add_field("uri", "std::string", "uri")
    info.add_field("label", "std::string", "label")
    info.add_field("abbreviation", "std::string", "abbreviation")
    info.add_field("conversionMultiplier", "double", "conversionMultiplier")

    unknown = {
        "enum": "UNKNOWN",
        "uri": "",
        "label": "Unknown",
        "abbreviation": "??",
        "conversionMultiplier": 0.0
    }

    info.set_null_value(unknown)

    for u in units:
        u.label = u.label.strip()

        enum = format_enum(u.label)
        unit_data = {}

        unit_data["label"] = str(u.label)
        unit_data["enum"] = enum
        unit_data["conversionMultiplier"] = float(u.coefficient)
        unit_data["abbreviation"] = str(u.abbreviation)
        unit_data["uri"] = str(u.unit)

        info.add_entry(unit_data)

    return {
        "ENUM_UNITS": info.get_enum_values(),
        "FROM_URI": info.uri_mappings(),
        "UNIT_SCHEME": info.struct_scheme(),
        "UNIT_INFO": info.struct_data(),
        "UNKNOWN_INFO": info.null_value_data()
    }

if(__name__ == "__main__"):
    main()
