import sys
import procdraw_utils as utils

expected_num_files = int(sys.argv[1])
cpp_files = sys.argv[2:]

reporter = utils.CheckResultTapReporter(expected_num_files)
checker = utils.SourceFileChecker()
for file in cpp_files:
    with open(file) as file_in:
        reporter.add(checker.check(file, file_in, "//"))
reporter.print_summary()
if reporter.is_fail():
    sys.exit(1)
