#include "stdafx.h"
#include "a11y.h"
#include "win_util.h"
#include <windows.h>
#include <Oleacc.h>
#include <comdef.h>
#include <iostream>

_COM_SMARTPTR_TYPEDEF(IAccessible, __uuidof(IAccessible));

namespace procdraw {

    BOOL CALLBACK EnumWindowsFunc(_In_  HWND hwnd, _In_  LPARAM lParam)
    {
        IAccessiblePtr ppvObject;
        ThrowOnFail(AccessibleObjectFromWindow(hwnd, OBJID_WINDOW, __uuidof(IAccessible),
            reinterpret_cast<void**>(&ppvObject)));

        VARIANT self;
        self.vt = VT_I4;
        self.lVal = CHILDID_SELF;

        _bstr_t accName;
        HRESULT hr = ppvObject->get_accName(self, &accName.GetBSTR());
        ThrowOnFail(hr);
        if (hr == S_OK) {
            std::cout << accName << std::endl;
        }
        return true;
    }

    void DumpWindows()
    {
        EnumWindows(EnumWindowsFunc, NULL);
    }

}
