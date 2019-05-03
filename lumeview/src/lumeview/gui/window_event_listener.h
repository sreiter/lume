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

#ifndef __H__window_event_listener
#define __H__window_event_listener

#include <vector>
#include <chrono>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <lumeview/camera/viewport.h>

namespace lumeview {

enum MouseButton {
	LEFT = 0,
	RIGHT = 1,
	MIDDLE = 2,

	NUM_BUTTONS
};


enum MouseButtonAction {
	UP = 0,
	DOWN = 1,

	NUM_ACTIONS
};

/** 
 * \note All buttons, keys, actions, mods, etc correspond to glfw
 *
 * When you override a virtual method from this class in a derived class,
 * make sure to first call the base class implementation before any other
 * operation. This makes sure, that helper methods return the correct values
 * (e.g. 'was_double_click' or 'cursor_position').
 * An exception may be that you want to compare to a previous state. In that
 * case it could make sense to store the current state of the base class before
 * calling the corresponding base class method.
 */
class WindowEventListener {
public:
	WindowEventListener ();
	virtual ~WindowEventListener ();

//	mouse
	virtual void mouse_button (int button, int action, int mods);
  	virtual void mouse_move (const glm::vec2& c);
  	virtual void mouse_scroll (const glm::vec2& o);

  	glm::vec2 cursor_position () const;

  	bool was_double_click (int button) const;
  	bool mouse_button_is_down (int button) const;
  	glm::vec2 mouse_button_down_pos (int button) const;

//	window
  	virtual void set_viewport (const camera::Viewport& vp);
  	virtual const camera::Viewport& viewport () const;

//	keyboard
  	virtual void key (int key, int scancode, int action, int mods);
  	virtual void character (unsigned int c);

private:
	using clock = std::chrono::steady_clock;

	struct MouseBtnInfo {
		MouseBtnInfo ();
		clock::time_point	lastDown;
		glm::vec2			downPos;
		bool				isDown;
		bool				wasDoubleClick;
	};

	glm::vec2					m_cursorPos;
	camera::Viewport  			m_viewport;
	std::vector<MouseBtnInfo>	m_mouseBtnInfo;
};

}// end of namespace lumeview

#endif	//__H__window_event_listener
