// Copyright 2022 Nicolas Mellado
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// -------------------------------------------------------------------------- //
//
// Authors: Nicolas Mellado
//
// This test runs io routines and check data integrity
// This test is part of the implementation of the library OpenGR.

#include "gr/io/io.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string>

#ifndef CXX_FILESYSTEM_HAVE_FS
#  error std::filesystem is required to compile this file
#endif
#if CXX_FILESYSTEM_IS_EXPERIMENTAL
#  include <experimental/filesystem>
#else
#  include <filesystem>
#endif

#include "testing.h"


using namespace std;
using namespace gr;

using Scalar = float;

/*!
  Read a configuration file from Standford 3D shape repository and
  output a set of filename and eigen transformations
  */
inline string
extractFilesAndTrFromStandfordConfFile(){
    const string confFilePath = "./datasets/bunny/data/bun.conf"; // the first 3d model of this file will be loaded.
    VERIFY (CXX_FILESYSTEM_NAMESPACE::exists(confFilePath) && CXX_FILESYSTEM_NAMESPACE::is_regular_file(confFilePath));

    // extract the working directory for the configuration path
    const string workingDir = CXX_FILESYSTEM_NAMESPACE::path(confFilePath).parent_path().string();
    VERIFY (CXX_FILESYSTEM_NAMESPACE::exists(workingDir));

    // read the configuration file and call the matching process
    std::string line;
    std::ifstream confFile;
    confFile.open(confFilePath);
    VERIFY (confFile.is_open());

    vector<string> tokens;
    do {
        getline(confFile, line);
        istringstream iss(line);
        tokens = {istream_iterator<string>{iss}, istream_iterator<string>{}};
    } while (tokens[0].compare("bmesh") != 0);

    // here we know that the tokens are:
    // [0]: keyword, must be bmesh
    // [1]: 3D object filename
    // [2-4]: target translation with previous object
    // [5-8]: target quaternion with previous object
    VERIFY (tokens.size() == 9);
    string res = CXX_FILESYSTEM_NAMESPACE::path(confFilePath).parent_path().string()+string("/")+tokens[1];
    VERIFY(CXX_FILESYSTEM_NAMESPACE::exists(res) && CXX_FILESYSTEM_NAMESPACE::is_regular_file(res));

    confFile.close();

    return res;
}


int main(int argc, const char **argv) {
    using std::string;

    if(!Testing::init_testing(1, argv)) return EXIT_FAILURE;

    string filename = extractFilesAndTrFromStandfordConfFile();
    string tempfilename = "temp.ply";

    cout << "Loading " << filename << endl;

    std::vector<gr::Point3D<Scalar> > v, v2;
    std::vector<Eigen::Matrix2f> tex_coords, tex_coords2;
    std::vector<typename gr::Point3D<Scalar>::VectorType> normals, normals2;
    std::vector<tripple> tris, tris2;
    std::vector<std::string> mtls, mtls2;

    IOManager manager;
    manager.ReadObject(filename, v, tex_coords, normals, tris, mtls);
    manager.WriteObject(tempfilename, v, tex_coords, normals, tris, mtls);
    manager.ReadObject(tempfilename, v2, tex_coords2, normals2, tris2, mtls2);

    // check if we get the same file after saving and loading again
    VERIFY(std::equal(v.cbegin(), v.cend(), v2.cbegin()));
    VERIFY(std::equal(tex_coords.cbegin(), tex_coords.cend(), tex_coords2.cbegin()));
    VERIFY(std::equal(normals.cbegin(), normals.cend(), normals2.cbegin()));
    VERIFY(std::equal(tris.cbegin(), tris.cend(), tris2.cbegin()));
    VERIFY(std::equal(mtls.cbegin(), mtls.cend(), mtls2.cbegin()));

    return EXIT_SUCCESS;
}
