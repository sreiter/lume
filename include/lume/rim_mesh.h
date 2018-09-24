// This file is part of lume, a C++ library for lightweight unstructured meshes
//
// Copyright (C) 2018 Sebastian Reiter
// Copyright (C) 2018 G-CSC, Goethe University Frankfurt
// Author: Sebastian Reiter <s.b.reiter@gmail.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef __H__lume_rim_mesh
#define __H__lume_rim_mesh

#include <functional>
#include "mesh.h"

namespace lume {

class Neighborhoods;

void CreateRimMesh (SPMesh rimMeshOut,
                    SPMesh mesh,
                    GrobSet grobSet,
                    const std::function <bool (const GrobIndex& gi)>&
                    	visFunc = [](const GrobIndex&){return true;},
                    const std::function <void (const GrobIndex& rimGrob, const GrobIndex& srcGrob)>&
                      	gotRimGrobFunc = [](const GrobIndex&, const GrobIndex&){},
					const Neighborhoods* nbrhds = nullptr);


void CreateRimMesh (SPMesh rimMeshOut,
                    SPMesh mesh,
                    GrobSet grobSet,
                    const std::function <void (const GrobIndex& rimGrob, const GrobIndex& srcGrob)>& gotRimGrobFunc,
					const Neighborhoods* nbrhds = nullptr);

SPMesh CreateRimMesh (SPMesh mesh,
                      GrobSet grobSet,
                      const std::function <bool (const GrobIndex& gi)>&
                      	  visFunc = [](const GrobIndex&){return true;},
                      const std::function <void (const GrobIndex& rimGrob, const GrobIndex& srcGrob)>&
                          gotRimGrobFunc = [](const GrobIndex&, const GrobIndex&){},
					  const Neighborhoods* nbrhds = nullptr);


SPMesh CreateRimMesh (SPMesh mesh,
                      GrobSet grobSet,
                      const std::function <void (const GrobIndex& rimGrob, const GrobIndex& srcGrob)>& gotRimGrobFunc,
					  const Neighborhoods* nbrhds = nullptr);


}//	end of namespace lume

#endif	//__H__lume_rim_mesh
