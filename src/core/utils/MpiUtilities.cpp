//
//  MpiUtilities.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 4/2/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "MpiUtilities.h"
#include <iostream>
#include <sstream>
#include <string>

#ifdef RB_MPI
#include <mpi.h>
#endif

void RevBayesCore::MpiUtilities::DebugWait(int rank) {
    
#ifdef RB_MPI
    char	a;
    if(rank == 0) {
    	scanf("%c", &a);
    	printf("%d: Starting now\n", rank);
    }
    
    MPI_Bcast(&a, 1, MPI_BYTE, 0, MPI_COMM_WORLD);
    printf("%d: Starting now\n", rank);
#endif
}

void RevBayesCore::MpiUtilities::DebugMsg(const std::stringstream& s) {
#ifdef RB_MPI
#ifdef DEBUG_MPI_MCA
    int pid = MPI::COMM_WORLD.Get_rank();
    std::cout << pid << "   before: " << s.str() << "\n";
    MPI::COMM_WORLD.Barrier();
    std::cout << pid << "   after:  " << s.str() << "\n";
#endif
#endif
}

void RevBayesCore::MpiUtilities::DebugMsg(const std::string& s) {
#ifdef RB_MPI
#ifdef DEBUG_MPI_MCA
    int pid = MPI::COMM_WORLD.Get_rank();
    std::cout << pid << "   before: " << s << "\n";
    MPI::COMM_WORLD.Barrier();
    std::cout << pid << "   after:  " << s << "\n";
#endif
#endif
}

void RevBayesCore::MpiUtilities::DebugMsg(const std::string& s, int x) {
#ifdef RB_MPI
#ifdef DEBUG_MPI_MCA
    std::stringstream ss;
    ss << s << " " << x;
    int pid = MPI::COMM_WORLD.Get_rank();
    std::cout << pid << "   before: " << ss.str() << "\n";
    MPI::COMM_WORLD.Barrier();
    std::cout << pid << "   after:  " << ss.str() << "\n";
#endif
#endif
}

void RevBayesCore::MpiUtilities::DebugMsg(const std::string& s, double x) {
#ifdef RB_MPI
#ifdef DEBUG_MPI_MCA
    std::stringstream ss;
    ss << s << " " << x;
    int pid = MPI::COMM_WORLD.Get_rank();
    std::cout << pid << "   before: " << ss.str() << "\n";
    MPI::COMM_WORLD.Barrier();
    if (pid == 0) std::cout << "\n";
    MPI::COMM_WORLD.Barrier();
    std::cout << pid << "   after:  " << ss.str() << "\n";
#endif
#endif
}

void RevBayesCore::MpiUtilities::DebugMsgPid(const std::string& s, int p) {
#ifdef RB_MPI
#ifdef DEBUG_MPI_MCA
    int pid = MPI::COMM_WORLD.Get_rank();
    MPI::COMM_WORLD.Barrier();
    if (pid == p) {
        std::cout << s;
    }
    MPI::COMM_WORLD.Barrier();
#endif
#endif
}