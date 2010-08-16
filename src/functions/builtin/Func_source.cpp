/**
 * @file
 * This file contains the implementation of Func_source, which is
 * the function used to read commands (source) from a file.
 *
 * @brief Implementation of Func_source
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Func_source.h"
#include "Model.h"
#include "Parser.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "StringVector.h"
#include "Workspace.h"

#include <fstream>


/** Clone object */
RbObject* Func_source::clone(void) const {

    return new Func_source(*this);
}


/** Execute function */
RbObject* Func_source::executeOperation(const std::vector<DAGNode*>& args) {

    /* Open file */
    std::string filename = ((RbString*)(args[0]->getValue()))->getValue();
    std::ifstream inFile(filename.c_str());
    
    if (!inFile)
        throw RbException("Could not open file '" + filename + "'");

    /* Initialize */
    std::string commandLine;
    std::cout << std::endl;
    std::cout << "Processing file '" << filename << "' ..." << std::endl;
    Parser::getParser().reset();

    /* Command-processing loop */
    while (inFile.good()) {

        // Read a line
        std::string line;
        getline(inFile, line);

        // Process the line (here we continue processing even if errors are encountered)
        if (Parser::getParser().processCommand(line) == 2)
            throw RbException("Problem processing file '" + filename + "'");
    }

    /* Return control */
    std::cout << "Processing of file '" << filename << "' completed" << std::endl;
    std::cout << std::endl;

    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_source::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules.push_back(new ArgumentRule("file", RbString_name));
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const StringVector& Func_source::getClass(void) const {

    static StringVector rbClass = StringVector(Func_source_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const std::string& Func_source::getReturnType(void) const {

    return RbNULL_name;
}


