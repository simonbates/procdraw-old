#include "lisp_interpreter.h"
#include <libxml/xmlreader.h>
#include <iostream>
#include <stdlib.h>

static bool TestExample(xmlTextReaderPtr reader, procdraw::LispInterpreter *L, int *numTests)
{
    bool testPassed = false;

    xmlChar *expr = xmlTextReaderGetAttribute(reader, BAD_CAST "expr");
    if (expr != NULL) {
        xmlChar *expectedValue = xmlTextReaderGetAttribute(reader, BAD_CAST "value");
        if (expectedValue != NULL) {
            std::string result = L->PrintToString(L->Eval(L->Read(reinterpret_cast<char*>(expr))));
            *numTests = *numTests + 1;
            if (xmlStrEqual(expectedValue, BAD_CAST result.c_str())) {
                testPassed = true;
            }
            else {
                testPassed = false;
                std::cerr << "EXPR: " << expr;
                std::cerr << " EXPECTED: " << expectedValue;
                std::cerr << " ACTUAL: " << result << std::endl;
            }
            xmlFree(expectedValue);
        }
        xmlFree(expr);
    }

    return testPassed;
}

static bool TestFunction(const char *filename, int *numTests)
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
        if (name != NULL && xmlStrEqual(name, BAD_CAST "example")) {
            if (!TestExample(reader, &L, numTests)) {
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
    if (argc != 3) {
        return 1;
    }

    LIBXML_TEST_VERSION

    int numTests = 0;
    bool passed = TestFunction(argv[1], &numTests);

    xmlCleanupParser();

    if (atoi(argv[2]) != numTests) {
        std::cerr << "EXPECTED " << argv[2] << " TESTS BUT " << numTests << " WERE RUN" << std::endl;
        return 1;
    }

    return (passed ? 0 : 1);
}
