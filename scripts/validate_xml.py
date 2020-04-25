import sys
import procdraw_utils as utils

schema_file = sys.argv[1]
xml_file = sys.argv[2]

reporter = utils.CheckResultTapReporter(1)
reporter.add(utils.validate_xml(schema_file, xml_file))
reporter.print_summary()
if reporter.is_fail():
    sys.exit(1)
