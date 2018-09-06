// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2017 Sebastian Reiter, G-CSC Frankfurt <s.b.reiter@gmail.com>

#include "custom_exception.h"
#include "tests.h"

#include <iostream>

using namespace std;
using namespace lume;

int main (int argc, char** argv)
{
	int retVal = 0;

	try {
		if (!lume::tests::RunTests ())
			throw LumeError ("lume::RunTests did not succeed!");
	}
	catch (std::exception& e) {
		cout << "\nAn ERROR occurred during execution:\n";
		cout << e.what() << endl << endl;
		retVal = 1;
	}

	return retVal;
}