#include "lisp_interpreter.h"
#include <libxml/xmlreader.h>
#include <iostream>

static bool DoTest(xmlTextReaderPtr reader, procdraw::LispInterpreter *L)
{
    bool testPassed = false;

    xmlChar *expr = xmlTextReaderGetAttribute(reader, BAD_CAST "expr");
    if (expr != NULL) {
        xmlChar *expected = xmlTextReaderGetAttribute(reader, BAD_CAST "expected");
        if (expected != NULL) {
            std::string result = L->PrintToString(L->Eval(L->Read(reinterpret_cast<char*>(expr))));
            if (xmlStrEqual(expected, BAD_CAST result.c_str())) {
                testPassed = true;
            }
            else {
                testPassed = false;
                std::cerr << "EXPR: " << expr;
                std::cerr << " EXPECTED: " << expected;
                std::cerr << " ACTUAL: " << result << std::endl;
            }
            xmlFree(expected);
        }
        xmlFree(expr);
    }

    return testPassed;
}

static bool TestFunction(const char *filename)
{
    procdraw::LispInterpreter L;
    xmlTextReaderPtr reader = xmlReaderForFile(filename, NULL, 0);

    if (reader == NULL) {
        std::cerr << "File not found: " << filename << std::endl;
        return false;
    }

    bool allPassed = true;

    int ret = xmlTextReaderRead(reader);
    while (ret == 1) {
        const xmlChar *name = xmlTextReaderConstName(reader);
        if (name != NULL && xmlStrEqual(name, BAD_CAST "test")) {
            if (!DoTest(reader, &L)) {
                allPassed = false;
            }
        }
        ret = xmlTextReaderRead(reader);
    }

    xmlFreeTextReader(reader);

    return allPassed;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        return 1;
    }

    LIBXML_TEST_VERSION

    bool passed = TestFunction(argv[1]);

    xmlCleanupParser();

    return (passed ? 0 : 1);
}
