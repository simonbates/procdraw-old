from __future__ import print_function
import sys
from lxml import etree

schema_file = sys.argv[1]
xml_file = sys.argv[2]

schema_doc = etree.parse(schema_file)
validator = etree.RelaxNG(schema_doc)

xml_doc = etree.parse(xml_file)

if validator.validate(xml_doc):
    print('OK {:s}'.format(xml_file))
else:
    print(validator.error_log.last_error, file=sys.stderr)
    sys.exit(1)
