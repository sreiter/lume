// This file is part of lume, a C++ library for lightweight unstructured meshes
//
// Copyright (C) 2019 Sebastian Reiter <s.b.reiter@gmail.com>
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

#include <iostream>
#include "lume/mesh.h"
#include "lume/file_io.h"
#include "lume/surface_analytics.h"
#include "lume/commands/commander.h"

using std::cout;
using std::endl;

namespace lume {
namespace commands {
    // INFO
    class PrintMeshContents : public Command
    {
    public:
        PrintMeshContents ()
            : Command ("PrintMeshContents", "Gives an overview over the elements contained in a mesh.",
                       {ArgumentDesc (Type::Mesh, "mesh", "The mesh whose contents will be printed.")})
        {}

    protected:
        void run (const Arguments& args) override
        {
            auto mesh = args.get <SPMesh> ("mesh");

            std::cout << "Mesh contents:" << std::endl;
            auto grobTypes = mesh->grob_types ();
            for (auto gt : grobTypes) {
                std::cout << "  " << GrobSet (gt).name () << ": \t" << mesh->num (gt) << std::endl;
            }
        }
    };

    class IsManifoldMesh : public Command
    {
    public:
        IsManifoldMesh ()
            : Command ("IsManifoldMesh", "Prints true if the specified mesh is a manifold mesh.",
                       {ArgumentDesc (Type::Mesh, "mesh", "The mesh which will be analyzed.")})
        {}

    protected:
        void run (const Arguments& args) override
        {
            if (lume::IsManifoldMesh (*args.get <SPMesh> ("mesh")))
                cout << "Yes\n";
            else
                cout << "No\n";
        }
    };

    class IsClosedManifoldMesh : public Command
    {
    public:
        IsClosedManifoldMesh ()
            : Command ("IsClosedManifoldMesh", "Prints true if the specified mesh is a closed manifold mesh.",
                       {ArgumentDesc (Type::Mesh, "mesh", "The mesh which will be analyzed.")})
        {}

    protected:
        void run (const Arguments& args) override
        {
            if (lume::IsClosedManifoldMesh (*args.get <SPMesh> ("mesh")))
                cout << "Yes\n";
            else
                cout << "No\n";
        }
    };

    class Help : public Command
    {
    public:
        Help (std::shared_ptr <const Commander> commander)
            : Command ("Help", "Gives an overview over available commands and their parameters.")
            , m_commander (commander)
        {}

    protected:
        void run (const Arguments&) override
        {
            if (m_commander == nullptr)
            {
                cout << "No commands available.\n";
                return;
            }

            for (const auto& entry : *m_commander) {
                const auto& command = *entry.second;
                const auto& name = command.name ();

                cout << name << ":\t" << command.description () << endl;
                const auto argDescs = command.argument_descs ();
                for (const auto& argDesc : argDescs)
                {
                    cout << "\t" << argDesc.name () << ":\t\t" << argDesc.description () << endl;
                }
                cout << endl;
            }
        }

    private:
        std::shared_ptr <const Commander> m_commander;
    };

}// end of namespace commands
}// end of namespace lume

int main (int argc, char** argv)
{
    cout << "lumeshell (c) 2019 Sebastian Reiter (s.b.reiter@gmail.com)" << endl;

    int retVal = 0;

    try {
        auto commander = std::make_shared <lume::commands::Commander> ();
        commander->add <lume::commands::Help> (commander);
        commander->add <lume::commands::PrintMeshContents> ();
        commander->add <lume::commands::IsManifoldMesh> ();
        commander->add <lume::commands::IsClosedManifoldMesh> ();

        bool printHelp = true;
        if (argc >= 2)
        {
            commander->run (argv[1], argc - 2, argv + 2);
        }
    }
    catch (std::runtime_error& err)
    {
        cout << err.what () << std::endl;
        retVal = 1;
    }
    catch (...)
    {
        cout << "ERROR: An unknown error occurred during execution...\n";
        retVal = 1;
    }

    return retVal;
}