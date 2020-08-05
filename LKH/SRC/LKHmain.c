#include "LKH.h"
#include "Genetic.h"
#include "BIT.h"

/*
 * This file contains the main function of the program.
 */

int main(int argc, char *argv[])
{
    
    GainType Cost, OldOptimum;
    double Time, LastTime = GetTime();
    Node *N;
    int i;

    /* Read the specification of the problem */
    if (argc >= 2)
        ParameterFileName = argv[1];
    ReadParameters();
    
    const char *Xnames[100] = {"X-n101-k25", "X-n106-k14", "X-n110-k13", "X-n115-k10", "X-n120-k6", "X-n125-k30", "X-n129-k18", 
        "X-n134-k13", "X-n139-k10", "X-n143-k7", "X-n148-k46", "X-n153-k22", "X-n157-k13", "X-n162-k11", "X-n167-k10", "X-n172-k51", "X-n176-k26",
        "X-n181-k23", "X-n186-k15", "X-n190-k8", "X-n195-k51", "X-n200-k36", "X-n204-k19", "X-n209-k16", "X-n214-k11", "X-n219-k73", "X-n223-k34",
        "X-n228-k23", "X-n233-k16", "X-n237-k14", "X-n242-k48", "X-n247-k50", "X-n251-k28", "X-n256-k16", "X-n261-k13", "X-n266-k58", "X-n270-k35",
       "X-n275-k28", "X-n280-k17", "X-n284-k15", "X-n289-k60", "X-n294-k50", "X-n298-k31", "X-n303-k21", "X-n308-k13", "X-n313-k71", "X-n317-k53",
        "X-n322-k28", "X-n327-k20", "X-n331-k15", "X-n336-k84", "X-n344-k43", "X-n351-k40", "X-n359-k29", "X-n367-k17", "X-n376-k94", "X-n384-k52", 
        "X-n393-k38", "X-n401-k29", "X-n411-k19", "X-n420-k130", "X-n429-k61", "X-n439-k37", "X-n449-k29", "X-n459-k26", "X-n469-k138", 
        "X-n480-k70", "X-n491-k59", "X-n502-k39", "X-n513-k21", "X-n524-k153", "X-n536-k96", "X-n548-k50", "X-n561-k42", "X-n573-k30",
         "X-n586-k159", "X-n599-k92", "X-n613-k62", "X-n627-k43", "X-n641-k35", "X-n655-k131", "X-n670-k130", "X-n685-k75", "X-n701-k44",
          "X-n716-k35", "X-n733-k159", "X-n749-k98", "X-n766-k71", "X-n783-k48", "X-n801-k40", "X-n819-k171", "X-n837-k142", "X-n856-k95", 
          "X-n876-k59", "X-n895-k37", "X-n916-k207", "X-n936-k151", "X-n957-k87", "X-n979-k58", "X-n1001-k43"};
 
    int goodNbOfVehicles[100] = {26, 14, 13, 10, 6, 30, 18, 13, 10, 7, 47, 23, 13, 11, 10, 53, 26, 23, 15, 8, 53, 36, 19, 16,
     11, 73, 34, 23, 17, 14, 48, 51, 28, 16, 13, 58, 36, 28, 17, 15, 61, 51, 31, 21, 13, 72, 53, 28, 20, 15, 86, 43, 41, 29, 17,
      94, 53, 38, 29, 19, 130, 62, 37, 29, 26, 140, 70, 59, 39, 21, 155, 96, 50, 42, 30, 159, 93, 62, 43, 35, 131, 133, 75, 44,
       35, 160, 98, 71, 48, 40, 172, 142, 95, 59, 37, 207, 158, 87, 58, 43};

    int ii;
    for (ii = 0; ii < 100; ii++) {
        if(strstr(ParameterFileName, Xnames[ii]) != NULL) {
            Salesmen = goodNbOfVehicles[ii];
            printf("X instance detected. Predefined #vehicles %d will be used!\n", Salesmen);
        }
    }

    MaxMatrixDimension = 20000;
    MergeWithTour = Recombination == IPT ? MergeWithTourIPT :
        MergeWithTourGPX2;
    ReadProblem();
    // printf(">>>>>>>>>>>> %d\n", Salesmen);
    if (SubproblemSize > 0) {
        if (DelaunayPartitioning)
            SolveDelaunaySubproblems();
        else if (KarpPartitioning)
            SolveKarpSubproblems();
        else if (KCenterPartitioning)
            SolveKCenterSubproblems();
        else if (KMeansPartitioning)
            SolveKMeansSubproblems();
        else if (RohePartitioning)
            SolveRoheSubproblems();
        else if (MoorePartitioning || SierpinskiPartitioning)
            SolveSFCSubproblems();
        else
            SolveTourSegmentSubproblems();
        return EXIT_SUCCESS;
    }
    AllocateStructures();
    if (ProblemType == TSPTW)
        TSPTW_Reduce();
    if (ProblemType == VRPB || ProblemType == VRPBTW)
        VRPB_Reduce();
    if (ProblemType == PDPTW)
        PDPTW_Reduce();
    CreateCandidateSet();
    InitializeStatistics();

    if (Norm != 0 || Penalty) {
        Norm = 9999;
        BestCost = PLUS_INFINITY;
        BestPenalty = CurrentPenalty = PLUS_INFINITY;
    } else {
        /* The ascent has solved the problem! */
        Optimum = BestCost = (GainType) LowerBound;
        UpdateStatistics(Optimum, GetTime() - LastTime);
        RecordBetterTour();
        RecordBestTour();
        CurrentPenalty = PLUS_INFINITY;
        BestPenalty = CurrentPenalty = Penalty ? Penalty() : 0;
        WriteTour(OutputTourFileName, BestTour, BestCost);
        WriteTour(TourFileName, BestTour, BestCost);
        Runs = 0;
    }

    /* Find a specified number (Runs) of local optima */

    for (Run = 1; Run <= Runs; Run++) {

        if (GetTime() - LastTime >= TimeLimit) {
            break;
        }
        // LastTime = GetTime();
        Cost = FindTour(LastTime);      /* using the Lin-Kernighan heuristic */
        if (MaxPopulationSize > 1 && !TSPTW_Makespan) {
            /* Genetic algorithm */
            int i;
            for (i = 0; i < PopulationSize; i++) {
                GainType OldPenalty = CurrentPenalty;
                GainType OldCost = Cost;
                Cost = MergeTourWithIndividual(i);
                if (TraceLevel >= 1 &&
                    (CurrentPenalty < OldPenalty ||
                     (CurrentPenalty == OldPenalty && Cost < OldCost))) {
                    if (CurrentPenalty)
                        printff("  Merged with %d: Cost = " GainFormat,
                                i + 1, Cost);
                    else
                        printff("  Merged with %d: Cost = " GainFormat "_"
                                GainFormat, i + 1, CurrentPenalty, Cost);
                    if (Optimum != MINUS_INFINITY && Optimum != 0) {
                        if (ProblemType != CCVRP && ProblemType != TRP &&
                            ProblemType != MLP &&
                            MTSPObjective != MINMAX &&
                            MTSPObjective != MINMAX_SIZE)
                            printff(", Gap = %0.4f%%",
                                    100.0 * (Cost - Optimum) / Optimum);
                        else
                            printff(", Gap = %0.4f%%",
                                    100.0 * (CurrentPenalty - Optimum) /
                                    Optimum);
                    }
                    printff("\n");
                }
            }
            if (!HasFitness(CurrentPenalty, Cost)) {
                if (PopulationSize < MaxPopulationSize) {
                    AddToPopulation(CurrentPenalty, Cost);
                    if (TraceLevel >= 1)
                        PrintPopulation();
                } else if (SmallerFitness(CurrentPenalty, Cost,
                                          PopulationSize - 1)) {
                    i = ReplacementIndividual(CurrentPenalty, Cost);
                    ReplaceIndividualWithTour(i, CurrentPenalty, Cost);
                    if (TraceLevel >= 1)
                        PrintPopulation();
                }
            }
        } else if (Run > 1 && !TSPTW_Makespan)
            Cost = MergeTourWithBestTour();
        if (CurrentPenalty < BestPenalty ||
            (CurrentPenalty == BestPenalty && Cost < BestCost)) {
            BestPenalty = CurrentPenalty;
            BestCost = Cost;
            RecordBetterTour();
            RecordBestTour();
            WriteTour(TourFileName, BestTour, BestCost);
        }
        OldOptimum = Optimum;
        if (!Penalty ||
            (MTSPObjective != MINMAX && MTSPObjective != MINMAX_SIZE)) {
            if (CurrentPenalty == 0 && Cost < Optimum)
                Optimum = Cost;
        } else if (CurrentPenalty < Optimum)
            Optimum = CurrentPenalty;
        if (Optimum < OldOptimum) {
            printff("*** New OPTIMUM = " GainFormat " ***\n\n", Optimum);
            if (FirstNode->InputSuc) {
                Node *N = FirstNode;
                while ((N = N->InputSuc = N->Suc) != FirstNode);
            }
        }
        Time = fabs(GetTime() - LastTime);
        UpdateStatistics(Cost, Time);
        if (TraceLevel >= 1 && Cost != PLUS_INFINITY) {
            printff("Run %d: ", Run);
            StatusReport(Cost, LastTime, "");
            printff("\n");
        }
        if (StopAtOptimum && MaxPopulationSize >= 1) {
            if (ProblemType != CCVRP && ProblemType != TRP &&
                ProblemType != MLP &&
                MTSPObjective != MINMAX &&
                MTSPObjective != MINMAX_SIZE ?
                CurrentPenalty == 0 && Cost == Optimum :
                CurrentPenalty == Optimum) {
                Runs = Run;
                break;
            }
        }
        if (PopulationSize >= 2 &&
            (PopulationSize == MaxPopulationSize ||
             Run >= 2 * MaxPopulationSize) && Run < Runs) {
            Node *N;
            int Parent1, Parent2;
            Parent1 = LinearSelection(PopulationSize, 1.25);
            do
                Parent2 = LinearSelection(PopulationSize, 1.25);
            while (Parent2 == Parent1);
            ApplyCrossover(Parent1, Parent2);
            N = FirstNode;
            do {
                if (ProblemType != HCP && ProblemType != HPP) {
                    int d = C(N, N->Suc);
                    AddCandidate(N, N->Suc, d, INT_MAX);
                    AddCandidate(N->Suc, N, d, INT_MAX);
                }
                N = N->InitialSuc = N->Suc;
            }
            while (N != FirstNode);
        }
        SRandom(++Seed);
    }
    // PrintStatistics();
    // if (Salesmen > 1) {
    //     if (Dimension == DimensionSaved) {
    //         for (i = 1; i <= Dimension; i++) {
    //             N = &NodeSet[BestTour[i - 1]];
    //             (N->Suc = &NodeSet[BestTour[i]])->Pred = N;
    //         }
    //     } else {
    //         for (i = 1; i <= DimensionSaved; i++) {
    //             Node *N1 = &NodeSet[BestTour[i - 1]];
    //             Node *N2 = &NodeSet[BestTour[i]];
    //             Node *M1 = &NodeSet[N1->Id + DimensionSaved];
    //             Node *M2 = &NodeSet[N2->Id + DimensionSaved];
    //             (M1->Suc = N1)->Pred = M1;
    //             (N1->Suc = M2)->Pred = N1;
    //             (M2->Suc = N2)->Pred = M2;
    //         }
    //     }
    //     CurrentPenalty = BestPenalty;
    //     MTSP_Report(BestPenalty, BestCost);
    //     MTSP_WriteSolution(MTSPSolutionFileName, BestPenalty, BestCost);
    //     SINTEF_WriteSolution(SINTEFSolutionFileName, BestCost);
    // }
    // if (ProblemType == ACVRP ||
    //     ProblemType == BWTSP ||
    //     ProblemType == CCVRP ||
    //     ProblemType == CTSP ||
    //     ProblemType == CVRP ||
    //     ProblemType == CVRPTW ||
    //     ProblemType == MLP ||
    //     ProblemType == M_PDTSP ||
    //     ProblemType == M1_PDTSP ||
    //     MTSPObjective != -1 ||
    //     ProblemType == ONE_PDTSP ||
    //     ProblemType == OVRP ||
    //     ProblemType == PDTSP ||
    //     ProblemType == PDTSPL ||
    //     ProblemType == PDPTW ||
    //     ProblemType == RCTVRP ||
    //     ProblemType == RCTVRPTW ||
    //     ProblemType == SOP ||
    //     ProblemType == TRP ||
    //     ProblemType == TSPTW ||
    //     ProblemType == VRPB ||
    //     ProblemType == VRPBTW || ProblemType == VRPPD) {
    //     printff("Best %s solution:\n", Type);
    //     CurrentPenalty = BestPenalty;
    //     SOP_Report(BestCost);
    // }
    // printff("\n");
    return EXIT_SUCCESS;
}
