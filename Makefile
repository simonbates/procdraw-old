OUT_DIR = build

CPPFLAGS = /EHsc

all: $(OUT_DIR)\Procdraw.exe $(OUT_DIR)\procdraw_tests.exe

test: $(OUT_DIR)\procdraw_tests.exe
    $(OUT_DIR)\procdraw_tests.exe

# Google Test

GTEST_DIR = vendor\googletest-1.8.1\googletest

GTEST_OBJS = \
    $(OUT_DIR)\googletest\gtest-all.obj \
    $(OUT_DIR)\googletest\gtest_main.obj

{$(GTEST_DIR)\src}.cc{$(OUT_DIR)\googletest}.obj::
    @ if NOT EXIST "$(OUT_DIR)\googletest" mkdir "$(OUT_DIR)\googletest"
    $(CPP) /MP /c $(CPPFLAGS) /Fo$(OUT_DIR)\googletest\ \
        /I$(GTEST_DIR)\include /I$(GTEST_DIR) \
        $<

$(OUT_DIR)\gtest.lib: $(GTEST_OBJS)
    lib /OUT:$(OUT_DIR)\gtest.lib $(GTEST_OBJS)

# pugixml

PUGIXML_DIR = vendor\pugixml-1.9

$(OUT_DIR)\pugixml.obj: $(PUGIXML_DIR)\src\pugixml.cpp
    $(CPP) /MP /c $(CPPFLAGS) /Fo$(OUT_DIR)\ $(PUGIXML_DIR)\src\pugixml.cpp

# procdraw_lib

PROCDRAW_LIB_OBJS = \
    $(OUT_DIR)\src\Color.obj \
    $(OUT_DIR)\src\D3D11Graphics.obj \
    $(OUT_DIR)\src\Interpreter.obj \
    $(OUT_DIR)\src\Printer.obj \
    $(OUT_DIR)\src\ProcdrawApp.obj \
    $(OUT_DIR)\src\ProcdrawMath.obj \
    $(OUT_DIR)\src\Reader.obj \
    $(OUT_DIR)\src\VirtualMachine.obj \
    $(OUT_DIR)\src\WinUtils.obj

$(PROCDRAW_LIB_OBJS): src\*.h

{src}.cpp{$(OUT_DIR)\src}.obj::
    @ if NOT EXIST "$(OUT_DIR)\src" mkdir "$(OUT_DIR)\src"
    $(CPP) /MP /c $(CPPFLAGS) /Fo$(OUT_DIR)\src\ /DUNICODE /D_UNICODE $<

$(OUT_DIR)\procdraw_lib.lib: $(PROCDRAW_LIB_OBJS)
    lib /OUT:$(OUT_DIR)\procdraw_lib.lib $(PROCDRAW_LIB_OBJS)

# Procdraw executable

$(OUT_DIR)\Procdraw.exe: winmain\WinMain.cpp $(OUT_DIR)\procdraw_lib.lib
    @ if NOT EXIST "$(OUT_DIR)\winmain" mkdir "$(OUT_DIR)\winmain"
    $(CPP) $(CPPFLAGS) /Fe$(OUT_DIR)\Procdraw.exe /Fo$(OUT_DIR)\winmain\ \
        /DUNICODE /D_UNICODE \
        /Isrc \
        winmain\WinMain.cpp \
        $(OUT_DIR)\procdraw_lib.lib d3d11.lib D3DCompiler.lib

# Tests

TEST_DATA_DIR = "\"tests/test_data"\"
FUNCTION_DOCS_FILE = "\"Documentation/manual/function_docs.xml"\"

TEST_OBJS = \
    $(OUT_DIR)\tests\ColorTests.obj \
    $(OUT_DIR)\tests\DocTester.obj \
    $(OUT_DIR)\tests\DocTesterTests.obj \
    $(OUT_DIR)\tests\FunctionDocsTests.obj \
    $(OUT_DIR)\tests\InterpreterPrintTests.obj \
    $(OUT_DIR)\tests\InterpreterReadTests.obj \
    $(OUT_DIR)\tests\InterpreterTests.obj \
    $(OUT_DIR)\tests\PrintTo.obj \
    $(OUT_DIR)\tests\ProcdrawManual.obj \
    $(OUT_DIR)\tests\ProcdrawMathTests.obj \
    $(OUT_DIR)\tests\TestUtils.obj \
    $(OUT_DIR)\tests\VirtualMachineTests.obj

$(TEST_OBJS): src\*.h tests\*.h

{tests}.cpp{$(OUT_DIR)\tests}.obj::
    @ if NOT EXIST "$(OUT_DIR)\tests" mkdir "$(OUT_DIR)\tests"
    $(CPP) /MP /c $(CPPFLAGS) /Fo$(OUT_DIR)\tests\ \
        /DUNICODE /D_UNICODE \
        /DPROCDRAW_TEST_DATA_DIR=$(TEST_DATA_DIR) \
        /DPROCDRAW_FUNCTION_DOCS_FILE=$(FUNCTION_DOCS_FILE) \
        /Isrc /I$(GTEST_DIR)\include /I$(PUGIXML_DIR)\src \
        $<

$(OUT_DIR)\procdraw_tests.exe: $(TEST_OBJS) $(OUT_DIR)\procdraw_lib.lib $(OUT_DIR)\gtest.lib $(OUT_DIR)\pugixml.obj
    link /OUT:$(OUT_DIR)\procdraw_tests.exe \
        /SUBSYSTEM:CONSOLE \
        $(TEST_OBJS) \
        $(OUT_DIR)\procdraw_lib.lib \
        $(OUT_DIR)\gtest.lib \
        $(OUT_DIR)\pugixml.obj
