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

#include "window_event_listener.h"

using namespace std;

namespace lumeview
{

using namespace camera;

WindowEventListener::MouseBtnInfo::MouseBtnInfo () :
		downPos (0),
		isDown (false),
		wasDoubleClick (false)
{
}


WindowEventListener::WindowEventListener () :
	m_cursorPos (0),
	m_viewport (0, 0, 1, 1)
{
}

WindowEventListener::~WindowEventListener ()
{

}


void WindowEventListener::mouse_button (int btn, int action, int mods)
{
	static const double dblClickInterval = 250.0;
	if (btn < 0)
		return;

	if(btn >= (int)m_mouseBtnInfo.size()){
		m_mouseBtnInfo.resize (btn + 1);
	}

	if(action == MouseButtonAction::DOWN) {
		const auto curTime = chrono::steady_clock::now();
		const double interval = chrono::duration <double, milli> (curTime - m_mouseBtnInfo [btn].lastDown).count();
		m_mouseBtnInfo [btn].wasDoubleClick = (interval < dblClickInterval);
		m_mouseBtnInfo [btn].lastDown = curTime;
		m_mouseBtnInfo [btn].downPos = m_cursorPos;
		m_mouseBtnInfo [btn].isDown = true;
	}
	else if (action == MouseButtonAction::UP) {
		m_mouseBtnInfo [btn].wasDoubleClick = false;
		m_mouseBtnInfo [btn].isDown = false;
	}
}

void WindowEventListener::mouse_move (const glm::vec2& c)
{
	m_cursorPos = c;
}

void WindowEventListener::mouse_scroll (const glm::vec2& o)
{

}

glm::vec2 WindowEventListener::cursor_position () const
{
	return m_cursorPos;
}

bool WindowEventListener::was_double_click (int btn) const
{
	if (btn >= 0 && btn < m_mouseBtnInfo.size ())
		return m_mouseBtnInfo [btn].wasDoubleClick;
	return false;
}

bool WindowEventListener::mouse_button_is_down (int btn) const
{
	if (btn >= 0 && btn < m_mouseBtnInfo.size ())
		return m_mouseBtnInfo [btn].isDown;
	return false;
}

glm::vec2 WindowEventListener::mouse_button_down_pos (int btn) const
{
	if (btn >= 0 && btn < m_mouseBtnInfo.size ())
		return m_mouseBtnInfo [btn].downPos;
	return glm::vec2(0);
}

void WindowEventListener::set_viewport (const Viewport& vp)
{
	m_viewport = vp;
}

const Viewport& WindowEventListener::viewport () const
{
	return m_viewport;
}

void WindowEventListener::key (int key, int scancode, int action, int mods)
{
}

void WindowEventListener::character (unsigned int c)
{
}

}// end of namespace lumeview
