/* Copyright (c) 2014, 2015, 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "WinUtils.h"

namespace Procdraw {

void TriangleNormal(DirectX::XMFLOAT3* normal, const DirectX::XMFLOAT3* vertex1, const DirectX::XMFLOAT3* vertex2, const DirectX::XMFLOAT3* vertex3)
{
    auto v1 = XMLoadFloat3(vertex1);
    auto v2 = XMLoadFloat3(vertex2);
    auto v3 = XMLoadFloat3(vertex3);
    auto n = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(v2, v1), DirectX::XMVectorSubtract(v3, v1));
    XMStoreFloat3(normal, DirectX::XMVector3Normalize(n));
}

} // namespace Procdraw
