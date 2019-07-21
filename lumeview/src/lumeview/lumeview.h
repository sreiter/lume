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

#ifndef __H__lumeview_lumeview
#define __H__lumeview_lumeview

#include <memory>
#include <lumeview/gui/window_event_listener.h>
#include <lumeview/editor/editor.h>

namespace lumeview {

class Lumeview : public WindowEventListener
{
public:
	Lumeview ();
    ~Lumeview ();

	void mouse_button (int button, int action, int mods) override;
  	void mouse_move (const glm::vec2& c) override;
  	void mouse_scroll (const glm::vec2& o) override;

  	void set_viewport (const camera::Viewport& vp) override;

  	void key (int key, int scancode, int action, int mods) override;
  	void character (unsigned int c) override;

    void add_editor (std::shared_ptr <editor::Editor> editor);

  	void process_gui ();

  	void render ();

private:
    using base_t = WindowEventListener;

private:
    template <class Function>
    void call_editor (Function&& function)
    {
        if (m_activeEditor != nullptr)
            function (*m_activeEditor);
    }

    template <class Function>
    void call_editors (Function&& function)
    {
        for (auto& editor : m_editors)
            if (editor != nullptr)
                function (*editor);
    }

private:
	WindowEventListener*                           m_imguiListener;
    std::vector <std::shared_ptr <editor::Editor>> m_editors;
    std::shared_ptr <editor::Editor>               m_activeEditor;

    bool  m_guiShowScene {true};
    bool  m_guiShowDemo  {false};
};

}//	end of namespace lumeview

#endif	//__H__lumeview_lumeview
