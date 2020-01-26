// Copyright 2020 Simon Bates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
