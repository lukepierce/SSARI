
/*
 * registerFile_UnitTest.cpp
 *
 *  Created on: Feb 18, 2015
 *      Author: lpierce
 */




/*
 * IRFuncVisitor_UnitTest.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: lpierce
 */

#include "gtest/gtest.h"
#include <iostream>
#include <algorithm>
#include <string>
#include "ssari.h"
#include "./Numeric/CFunc.h"
using namespace SSARI;
using namespace std;

TEST(CFunc, Operations) {
    // Variables for Operation
    CFunc x("X");
    CFunc y("Y");
    CFunc z("Z");

    // Test Expressions
    CFunc expr1 = x + y;
    EXPECT_EQ(expr1.toString(), "(X_0+Y_0)");

    expr1 = x - y;
    EXPECT_EQ(expr1.toString(), "(X_0-Y_0)");

    expr1 = x * y;
    EXPECT_EQ(expr1.toString(), "(X_0*Y_0)");

    expr1 = x / y;
    EXPECT_EQ(expr1.toString(), "(X_0/Y_0)");

    expr1 = x + y * z;
    EXPECT_EQ(expr1.toString(), "(X_0+(Y_0*Z_0))");

}

TEST(CFunc, GetDependentVars) {
    // Variables for Operation
    CFunc x("X");
    CFunc y("Y");
    CFunc z("Z");
    CFunc one(shared_ptr<CConstant>(new CConstant("1")));

    // Test Expressions
    CFunc expr = x + y * one * z;
    vector<shared_ptr<const CVar>> dVars = expr.getDependentVars();
    vector<string> varNames;

    EXPECT_EQ(dVars.size(), 3);

    for(auto iter = dVars.begin(); iter != dVars.end(); iter++)
        varNames.push_back((*iter)->toString());

    auto fIter = find(varNames.begin(), varNames.end(), "X_0");
    EXPECT_NE(fIter, varNames.end());
    fIter = find(varNames.begin(), varNames.end(), "Y_0");
    EXPECT_NE(fIter, varNames.end());
    fIter = find(varNames.begin(), varNames.end(), "Z_0");
    EXPECT_NE(fIter, varNames.end());
}

