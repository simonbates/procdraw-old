/* Copyright (c) 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "VirtualMachine.h"
#include <iostream>

namespace Procdraw {

void PrintTo(const ObjType& type, std::ostream* os);

}
