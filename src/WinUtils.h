/* Copyright (c) 2014, 2015, 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <DirectXMath.h>
#include <comdef.h>
#include <string>
#include <vector>

namespace Procdraw {

inline void ThrowOnFail(HRESULT hr)
{
    if (FAILED(hr)) {
        throw _com_error(hr);
    }
}

void TriangleNormal(DirectX::XMFLOAT3* normal, const DirectX::XMFLOAT3* vertex1, const DirectX::XMFLOAT3* vertex2, const DirectX::XMFLOAT3* vertex3);

} // namespace Procdraw
