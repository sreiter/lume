// This file is part of lumeview, a lightweight viewer for unstructured meshes
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

#ifndef __H__CAMERA_
#define __H__CAMERA_

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace lumeview {

class Camera {
  public:
    Camera ();

    void set_scale (const glm::vec3& scale);
    void set_rotation (const glm::quat& rot);
    void set_translation (const glm::vec3& trans);

    const glm::vec3& scale () const;
    const glm::quat& rotation () const;
    const glm::vec3& translation () const;

    void scale (const glm::vec3& scaleFac);
    void rotate (const glm::quat& rot);
    void translate (const glm::vec3& offset);

    glm::vec3 right () const;
    glm::vec3 up () const;
    glm::vec3 forward () const;

    glm::vec3 from () const;
    glm::vec3 to () const;

    glm::mat4 view_matrix () const;

  private:
    glm::vec3 m_scale;
    glm::quat m_rot;
    glm::vec3 m_trans;
};

}// end of namespace lumeview

#endif  //__H__CAMERA_
