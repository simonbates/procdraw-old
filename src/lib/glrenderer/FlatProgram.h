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

#pragma once

#include <glad/glad.h>

namespace Procdraw {

class FlatProgram {
public:
    FlatProgram();
    ~FlatProgram();
    GLuint Program()
    {
        return program_;
    };
    GLint WorldViewProjectionLoc()
    {
        return worldViewProjectionLoc_;
    };
    GLint LightDirectionLoc()
    {
        return lightDirectionLoc_;
    };
    GLint LightColorLoc()
    {
        return lightColorLoc_;
    }
    GLint AmbientLightColorLoc()
    {
        return ambientLightColorLoc_;
    };
    GLint MaterialColorLoc()
    {
        return materialColorLoc_;
    };

private:
    GLuint program_;
    GLint worldViewProjectionLoc_;
    GLint lightDirectionLoc_;
    GLint lightColorLoc_;
    GLint ambientLightColorLoc_;
    GLint materialColorLoc_;
};

} // namespace Procdraw
