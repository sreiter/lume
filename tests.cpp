// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2017 Sebastian Reiter, G-CSC Frankfurt <s.b.reiter@gmail.com>

#include "custom_exception.h"
#include "grob.h"
#include "file_io.h"
#include "tests.h"
#include "topology.h"
#include "neighborhoods.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

namespace slimesh {
namespace tests {

DECLARE_CUSTOM_EXCEPTION (TestError, SlimeshError);

#define FAIL(msg)	{std::stringstream ss; ss << msg; throw(TestError(ss.str()));}
#define COND_FAIL(cond, msg)	if(cond){FAIL(msg);}

namespace impl {

	/// tests whether the grob_type() method of a GrobDesc of a given grob_t returns the correct grob_type.
	static void TestGrobDescType (grob_t gt)
	{
		COND_FAIL (GrobDesc (gt).grob_type () != gt,
		           "Bad GrobDesc::grob_type() for grob type " << gt);
	}

	static void TestGrobDescTypes (grob_set_t gs)
	{
		for(auto gt : GrobSet(gs)) {
			TestGrobDescType (gt);
		}
	}
}

static void TestGrobDescs ()
{
	impl::TestGrobDescTypes (VERTICES);
	impl::TestGrobDescTypes (EDGES);
	impl::TestGrobDescTypes (FACES);
	impl::TestGrobDescTypes (CELLS);

//todo: lots more to be tested
}


static void TestCreateMeshFromFile ()
{
	vector<string> testMeshNames {"test_meshes/quad.stl",
								  "test_meshes/tris_and_quads.ugx",
								  "test_meshes/elems_refined_rim.ugx",
								  "test_meshes/elems_refined.ugx",
								  "test_meshes/bunny.stl"};

	for(auto meshName : testMeshNames) {
		try {
			SPMesh mesh = CreateMeshFromFile (meshName);
			cout << "    ok: '" << meshName << "'" << endl;
		}
		catch (SlimeshError& e) {
			cout << "    fail: '" << meshName << "'" << endl;
			FAIL ("Failed for '" << meshName << "' with message: " << e.what());
		}
	}
}


namespace impl {
	void TestGrobArrayLength (Mesh& mesh, grob_t grobType)
	{
		if (!mesh.has (grobType))
			return;

		GrobArray& ga = *mesh.inds (grobType);
		IndexArrayAnnex& ia = ga.underlying_array ();

		COND_FAIL (ia.tuple_size() != ga.grob_desc().num_corners(),
		           "Tuple size of underlying index array does not match the number of corners of the given GrobArray");
		COND_FAIL (ga.size() != ia.num_tuples(),
		           "Size of GrobArray does not match the number of tuples of the underlying index array");

	}

	void TestGrobArrayLength (Mesh& mesh, GrobSet grobSet)
	{
		for(auto gt : grobSet) {
			TestGrobArrayLength (mesh, gt);			
		}
	}
}// end of namespace


static void TestGrobArrays ()
{
	vector<string> testMeshNames {"test_meshes/quad.stl",
								  "test_meshes/tris_and_quads.ugx",
								  "test_meshes/elems_refined_rim.ugx",
								  "test_meshes/elems_refined.ugx"};

	for(auto meshName : testMeshNames) {
		try {
			SPMesh mesh = CreateMeshFromFile (meshName);
			impl::TestGrobArrayLength (*mesh, GrobSet(VERTICES));
			impl::TestGrobArrayLength (*mesh, GrobSet(EDGES));
			impl::TestGrobArrayLength (*mesh, GrobSet(FACES));
			impl::TestGrobArrayLength (*mesh, GrobSet(CELLS));

			cout << "    ok: '" << meshName << "'" << endl;
		}
		catch (SlimeshError& e) {
			cout << "    fail: '" << meshName << "'" << endl;
			FAIL ("Failed for '" << meshName << "' with message: " << e.what());
		}
	}
}


namespace impl {
	void TestGrobIterator (Mesh& mesh, grob_t grobType)
	{
		if (!mesh.has (grobType))
			return;

		GrobArray& grobArray = *mesh.inds (grobType);
		IndexArrayAnnex& indArray = grobArray.underlying_array ();

		index_t indCounter = 0;
		for(auto grob : grobArray) {
			for(index_t i = 0; i < grob.num_corners(); ++i) {
				COND_FAIL (grob.corner (i) != indArray [indCounter + i],
				           "Corner index mismatch between GrobIterator and raw array offsets into underlying index array.");
			}
			indCounter += grob.num_corners();
		}
	}

	void TestGrobIterator (Mesh& mesh, GrobSet grobSet)
	{
		for(auto gt : grobSet) {
			TestGrobIterator (mesh, gt);			
		}
	}
}// end of namespace

static void TestGrobIterator ()
{
	vector<string> testMeshNames {"test_meshes/tris_and_quads.ugx",
								  "test_meshes/elems_refined_rim.ugx",
								  "test_meshes/elems_refined.ugx"};

	for(auto meshName : testMeshNames) {
		try {
			SPMesh mesh = CreateMeshFromFile (meshName);
			impl::TestGrobIterator (*mesh, GrobSet(VERTICES));
			impl::TestGrobIterator (*mesh, GrobSet(EDGES));
			impl::TestGrobIterator (*mesh, GrobSet(FACES));
			impl::TestGrobIterator (*mesh, GrobSet(CELLS));

			cout << "    ok: '" << meshName << "'" << endl;
		}
		catch (SlimeshError& e) {
			cout << "    fail: '" << meshName << "'" << endl;
			FAIL ("Failed for '" << meshName << "' with message: " << e.what());
		}
	}
}


namespace impl {
	static void TestFillElemIndexMap (SPMesh mesh, GrobSet grobSet)
	{
		GrobHashMap <index_t> indexMap;
		index_t baseInds[NUM_GROB_TYPES];

		if (!mesh->has (grobSet))
			return;

		FillElemIndexMap (indexMap, baseInds, *mesh, grobSet);

		for(auto gt : grobSet) {
			const index_t baseInd = baseInds [gt];
			index_t index = 0;

			for(auto grob : *mesh->inds (gt)) {
				const index_t providedIndex = indexMap[grob] - baseInd;
				COND_FAIL(providedIndex != index,
				          "Index lookup mismatch for grob type '" << GrobDesc(gt).name()
				          << "': Current index is " << index <<
				          " but GrobHashMap provides " << indexMap[grob]);
				++index;
			}
		}
	}
}// end of namespace impl

static void TestFillElemIndexMap ()
{
	vector<string> testMeshNames {"test_meshes/tris_and_quads.ugx",
								  "test_meshes/tet_refined.ugx",
								  "test_meshes/elems_refined.ugx"};

	for(auto meshName : testMeshNames) {
		try {
			SPMesh mesh = CreateMeshFromFile (meshName);

			impl::TestFillElemIndexMap (mesh, VERTICES);
			impl::TestFillElemIndexMap (mesh, EDGES);
			impl::TestFillElemIndexMap (mesh, FACES);
			impl::TestFillElemIndexMap (mesh, CELLS);

			cout << "    ok: '" << meshName << "'" << endl;
		}
		catch (SlimeshError& e) {
			cout << "    fail: '" << meshName << "'" << endl;
			FAIL ("Failed for '" << meshName << "' with message: " << e.what());
		}
	}
}


static void TestGrobValences ()
{
	const string meshName ("test_meshes/tet_refined.ugx");
	try {
		SPMesh mesh = CreateMeshFromFile (meshName);
		GrobHashMap <index_t> valences;
		ComputeGrobValences (valences, *mesh, FACES, CELLS);

		vector <index_t> numFacesWithValenceN;
		for(auto gt : GrobSet (FACES)) {
			for(auto grob : *mesh->inds (gt)) {
				const index_t valence = valences [grob];
				if (valence >= numFacesWithValenceN.size())
					numFacesWithValenceN.resize (valence + 1, 0);
				++numFacesWithValenceN[valence];
			}
		}

		COND_FAIL (numFacesWithValenceN[0] > 0,
		           "There should be no faces with valence 0 in this grid");

		COND_FAIL (numFacesWithValenceN[1] != 16,
		           "There should be exactly 16 faces with valence 1 in this grid");

		COND_FAIL (numFacesWithValenceN[2] != 8,
		           "There should be exactly 8 faces with valence 2 in this grid");

		cout << "    ok: '" << meshName << "'" << endl;
	}
	catch (SlimeshError& e) {
		cout << "    fail: '" << meshName << "'" << endl;
		FAIL ("Failed for '" << meshName << "' with message: " << e.what());
	}
}


namespace impl {
	static void TestNeighborhoods (SPMesh mesh, GrobSet grobs, GrobSet nbrGrobs)
	{
	//	valences are used to check whether the individual neighborhoods have
	//	correct size. Note that 'ComputeGrobValences' is tested in a different test.
		GrobHashMap <index_t> valences;
		ComputeGrobValences (valences, *mesh, grobs, nbrGrobs);

		Neighborhoods nbrhds (mesh, grobs, nbrGrobs);

		for(auto gt : grobs) {
			index_t counter = 0;
			for(auto grob : *mesh->inds (gt)) {
				const GrobIndex gi (gt, counter++);
				const index_t numNbrs = nbrhds.neighbors (gi).size();

				COND_FAIL (numNbrs != valences [grob],
				           "Mismatch between the number of neighbors ("
				           << numNbrs << ") in a neighborhood of '"
				           << GrobName (gt) << "' and the valence of that grob ("
				           << valences [grob] << ")");
			}
		}


	}
}// end of namespace impl

static void TestNeighborhoods ()
{
	vector<string> testMeshNames {"test_meshes/tris_and_quads.ugx",
								  "test_meshes/tet_refined.ugx",
								  "test_meshes/elems_refined.ugx"};

	for(auto& meshName : testMeshNames) {
		try {
			SPMesh mesh = CreateMeshFromFile (meshName);
		
			// impl::TestNeighborhoods (mesh, VERTICES, EDGES);
			// impl::TestNeighborhoods (mesh, VERTICES, FACES);
			// impl::TestNeighborhoods (mesh, VERTICES, CELLS);
			// impl::TestNeighborhoods (mesh, EDGES, FACES);
			// impl::TestNeighborhoods (mesh, EDGES, CELLS);
			impl::TestNeighborhoods (mesh, FACES, CELLS);

			cout << "    ok: '" << meshName << "'" << endl;
		}
		catch (SlimeshError& e) {
			cout << "    fail: '" << meshName << "'" << endl;
			FAIL ("Failed for '" << meshName << "' with message: " << e.what());
		}
	}
}


// namespace impl {
// 	void TestAssociatedElemMap (Mesh& mesh, GrobSet grobs, GrobSet nbrGrobs)
// 	{
// 		if (!mesh.has (grobs) || !mesh.has (nbrGrobs))
// 			return;

// 		vector <index_t> elems;
// 		vector <index_t> offsets;
// 	}
// }// end of namespace impl


// void TestAssociatedElemMap ()
// {
// 	vector<string> testMeshNames {"test_meshes/tris_and_quads.ugx",
// 								  "test_meshes/tet_refined.ugx",
// 								  "test_meshes/elems_refined.ugx"};

// 	for(auto meshName : testMeshNames) {
// 		try {
// 			SPMesh mesh = CreateMeshFromFile (meshName);

// 			impl::TestAssociatedElemMap (*mesh, VERTICES, EDGES);
// 			impl::TestAssociatedElemMap (*mesh, VERTICES, FACES);
// 			impl::TestAssociatedElemMap (*mesh, VERTICES, CELLS);
// 			impl::TestAssociatedElemMap (*mesh, EDGES, FACES);
// 			impl::TestAssociatedElemMap (*mesh, EDGES, CELLS);
// 			impl::TestAssociatedElemMap (*mesh, FACES, CELLS);

// 			cout << "    ok: '" << meshName << "'" << endl;
// 		}
// 		catch (SlimeshError& e) {
// 			cout << "    fail: '" << meshName << "'" << endl;
// 			FAIL ("Failed for '" << meshName << "' with message: " << e.what());
// 		}
// 	}
// }


namespace impl {
	static void TestCreateBoundaryMesh (const string& meshName, const string& rimMeshName)
	{
		try {
			SPMesh mesh = CreateMeshFromFile (meshName);
			SPMesh bndMesh = CreateBoundaryMesh (mesh, CELLS);

			SPMesh rimMesh = CreateMeshFromFile (rimMeshName);
			COND_FAIL (bndMesh->num (TRI) != rimMesh->num (TRI),
			           "Mismatch between number of triangles in created boundary mesh ("
			           << bndMesh->num (TRI) << ") and loaded rim mesh (" << rimMesh->num (TRI) << ")");

			COND_FAIL (bndMesh->num (QUAD) != rimMesh->num (QUAD),
			           "Mismatch between number of quadrilaterals in created boundary mesh ("
			           << bndMesh->num (QUAD) << ") and loaded rim mesh (" << rimMesh->num (QUAD) << ")");

			cout << "    ok: '" << meshName << "' and '" << rimMeshName << "'" << endl;
		}
		catch (SlimeshError& e) {
			FAIL ("Failed for '" << meshName << "' and '" << rimMeshName << "' with message: " << e.what());
		}
	}
}// end of namespace impl

static void TestCreateBoundaryMesh ()
{
	impl::TestCreateBoundaryMesh ("test_meshes/tet_refined.ugx",
	                        	  "test_meshes/tet_refined_rim.ugx");

	impl::TestCreateBoundaryMesh ("test_meshes/elems_refined.ugx",
	                        	  "test_meshes/elems_refined_rim.ugx");

}



namespace impl {
	static void RunTest (int& testCounter, int& failedTestCounter, void (*testFunction)(), const char* testFunctionName)
	{
		++testCounter;
		cout.flush ();

		cout << "->  " << testFunctionName << endl;
		try {
			testFunction ();
			cout << "SUCCESS" << endl;
		}
		catch (TestError& err) {
			++failedTestCounter;
			cout << "FAILED ('" << testFunctionName << "'). Message below:" << endl;
			cout << err.what () << endl << endl;
		}
		catch (std::runtime_error& err) {
			++failedTestCounter;
			cout << "FAILED ('" << testFunctionName << "'). UNEXPECTED INTERNAL ERROR. Message below:" << endl;
			cout << err.what () << endl << endl;
		}
		catch (...) {
			++failedTestCounter;
			cout << "FAILED ('" << testFunctionName << "'). UNKNOWN INTERNAL ERROR!" << endl << endl;
		}
	}
}

#define RUN_TEST(testCounter, failedTestCounter, testFunctionName) impl::RunTest (testCounter, failedTestCounter, &testFunctionName, #testFunctionName);



bool RunTests ()
{
	int numTests = 0;
	int numFailed = 0;

	cout << "RUNNING TESTS" << endl;

	RUN_TEST(numTests, numFailed, TestGrobDescs);
	RUN_TEST(numTests, numFailed, TestCreateMeshFromFile);
	RUN_TEST(numTests, numFailed, TestGrobArrays);
	RUN_TEST(numTests, numFailed, TestGrobIterator);
	RUN_TEST(numTests, numFailed, TestFillElemIndexMap);
	RUN_TEST(numTests, numFailed, TestGrobValences);
	RUN_TEST(numTests, numFailed, TestNeighborhoods);
	RUN_TEST(numTests, numFailed, TestCreateBoundaryMesh);

	cout << endl << "TESTS DONE: " << numTests << " tests were run, "
		 << numFailed << " failed." << endl << endl;

	return numFailed == 0;
}


}//	end of namespace tests
}//	end of namespace slimesh
