from collections import OrderedDict

class UnitInfo:

    def __init__(self):
        self.fields = OrderedDict()
        self.entries = dict()

    def add_field(self, key, ctype, name):
        self.fields[key] = { "ctype": ctype, "name": name }

    def add_entry(self, data):
        key = data["enum"]
        self.entries[key] = data

    def set_null_value(self, data):
        self.null_value = data

    def null_value_data(self):
        return self.format_entry(self.null_value)

    def uri_mappings(self):
        mapping = sorted(list(( (i["uri"], i["enum"]) for i in self.entries.values() )))
        out = []
        for uri, enum in mapping:
            out.append("    if(uri == \"{}\") return {};".format(uri, enum) )
        return "\n".join(out)

    def format_field(self, field):
        return "    " + "{0} {1};".format(field["ctype"], field["name"])

    def struct_scheme(self):
        struct_fields = list(map( self.format_field, self.fields.values() ))
        return "\n".join(struct_fields)

    def struct_data(self):
        formatted_entries = []
        for i in sorted(self.entries):
            data = self.entries[i]
            formatted_entries.append(self.format_data(data))
        return "\n".join(formatted_entries)

    def get_enum_values(self):
        values = sorted(list( "    " + k for k in self.entries.keys() ))

        if(self.null_value is not None):
            values.append("    " + self.null_value["enum"])

        return ",\n".join(values)

    def format_entry(self, data):
        formatted_data = []
        for k in self.fields.keys():
            value = data[k]
            if(value == None):
                continue;
            formatted_data.append(self.format_field_by_type(k, data[k]))
        return "{ " + ", ".join(formatted_data) + " }"

    def format_data(self, data):
        return "    case {}: return {};".format(data["enum"], self.format_entry(data))

        # data_item_str = []
        # for v in data.values():
        #     data_item_str = format_data_item(field)

    def format_field_by_type(self, name, value):
        if(name == None or value == None):
            return ""

        field = self.fields[name]
        if(field == None):
            return ""

        t = field["ctype"]

        if(t == "std::string"):
            return '"{}"'.format(value)
        else:
            return str(value)
