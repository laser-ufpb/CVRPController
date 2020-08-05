#include "LKH.h"

/*
 * The WriteTour function writes a tour to file. The tour 
 * is written in TSPLIB format to file FileName. 
 * 
 * The tour is written in "normal form": starting at node 1,
 * and continuing in direction of its lowest numbered
 * neighbor.
 * 
 * Nothing happens if FileName is 0. 
 */

// void WriteTour(char *FileName, int *Tour, GainType Cost)
// {
//     FILE *stdout;
//     int i, j, k, n, Forward, a, b;
//     char *FullFileName;
//     time_t Now;

//     if (CurrentPenalty != 0 && MTSPObjective == -1 &&
//         ProblemType != CCVRP && ProblemType != TRP &&
//         ProblemType != MLP)
//         return;
//     if (FileName == 0)
//         return;
//     FullFileName = FullName(FileName, Cost);
//     Now = time(&Now);
//     if (TraceLevel >= 1)
//         printff("Writing%s: \"%s\" ... ",
//                 FileName == TourFileName ? " TOUR_FILE" :
//                 FileName == OutputTourFileName ? " OUTPUT_TOUR_FILE" : "",
//                 FullFileName);
//     assert(stdout = fopen(FullFileName, "w"));
//     if (CurrentPenalty == 0) {
//         fprintf(stdout, "NAME : %s." GainFormat ".tour\n", Name, Cost);
//         fprintf(stdout, "COMMENT : Length = " GainFormat "\n", Cost);
//     } else {
//         fprintf(stdout, "NAME : %s." GainFormat "_" GainFormat ".tour\n",
//                 Name, CurrentPenalty, Cost);
//         fprintf(stdout,
//                 "COMMENT : Cost = " GainFormat "_" GainFormat "\n",
//                 CurrentPenalty, Cost);
//     }
//     fprintf(stdout, "COMMENT : Found by LKH [Keld Helsgaun] %s",
//             ctime(&Now));
//     fprintf(stdout, "TYPE : TOUR\n");
//     fprintf(stdout, "DIMENSION : %d\n", DimensionSaved);
//     fprintf(stdout, "TOUR_SECTION\n");

//     n = DimensionSaved;
//     for (i = 1; i < n && Tour[i] != 1; i++);
//     Forward = Asymmetric ||
//         Tour[i < n ? i + 1 : 1] < Tour[i > 1 ? i - 1 : Dimension];
//     for (j = 1; j <= n; j++) {
//         if ((a = Tour[i]) <= n)
//             fprintf(stdout, "%d\n", 
//                     ProblemType != STTSP ? a : NodeSet[a].OriginalId);
//         if (Forward) {
//             if (++i > n)
//                 i = 1;
//         } else if (--i < 1)
//             i = n;
//         if (ProblemType == STTSP) {
//             b = Tour[i];
//             for (k = 0; k < NodeSet[a].PathLength[b]; k++)
//                 fprintf(stdout, "%d\n", NodeSet[a].Path[b][k]);
//         }
//     }
//     fprintf(stdout, "-1\nEOF\n");
//     fclose(stdout);
//     free(FullFileName);
//     if (TraceLevel >= 1)
//         printff("done\n");
// }

void WriteTour(char *FileName, int *Tour, GainType Cost)
{
    FILE *TourFile;
    int i, j, k, n, Forward, a, b;
    char *FullFileName;
    time_t Now;

    if (CurrentPenalty != 0 && MTSPObjective == -1 &&
        ProblemType != CCVRP && ProblemType != TRP &&
        ProblemType != MLP)
        return;
    if (FileName == 0)
        return;
    FullFileName = FullName(FileName, Cost);
    Now = time(&Now);
    if (TraceLevel >= 1)
        printff("Writing%s: \"%s\" ... ",
                FileName == TourFileName ? " TOUR_FILE" :
                FileName == OutputTourFileName ? " OUTPUT_TOUR_FILE" : "",
                FullFileName);
    assert(TourFile = fopen(FullFileName, "w"));

    int rid = 1;
    fprintf(stdout, "\nRoute #%d: ", rid); rid++;
    n = DimensionSaved;
    
    for (i = 1; i < n && Tour[i] != 1; i++);
    Forward = Asymmetric ||
        Tour[i < n ? i + 1 : 1] < Tour[i > 1 ? i - 1 : Dimension];
    
    for (j = 1; j <= n; j++) {
        if ((a = Tour[i]) <= n) {
            int vId = ProblemType != STTSP ? a : NodeSet[a].OriginalId;
            if(vId <= Dim){
                if(vId != 1)
                    fprintf(stdout, "%d ", vId-1);
            }else{
                fprintf(stdout, "\nRoute #%d: ", rid);
                rid++;
            }
        }
        if (Forward) {
            if (++i > n)
                i = 1;
        } else if (--i < 1)
            i = n;
    
        if (ProblemType == STTSP) {
            b = Tour[i];
            for (k = 0; k < NodeSet[a].PathLength[b]; k++)
                fprintf(stdout, "%d\n", NodeSet[a].Path[b][k]);
        }
    }
        fprintf(stdout,"\nCost " GainFormat "\n", Cost);
    fclose(TourFile);
    free(FullFileName);
    if (TraceLevel >= 1)
        printff("done\n");
    fflush(stdout);
}

/*
 * The FullName function returns a copy of the string Name where all 
 * occurrences of the character '$' have been replaced by Cost.        
 */

char *FullName(char *Name, GainType Cost)
{
    char *NewName = 0, *CostBuffer, *Pos;

    if (!(Pos = strstr(Name, "$"))) {
        assert(NewName = (char *) calloc(strlen(Name) + 1, 1));
        strcpy(NewName, Name);
        return NewName;
    }
    assert(CostBuffer = (char *) malloc(400));
    if (CurrentPenalty != 0)
        sprintf(CostBuffer, GainFormat "_" GainFormat,
                CurrentPenalty, Cost);
    else
        sprintf(CostBuffer, GainFormat, Cost);
    do {
        free(NewName);
        assert(NewName =
               (char *) calloc(strlen(Name) + strlen(CostBuffer) + 1, 1));
        strncpy(NewName, Name, Pos - Name);
        strcat(NewName, CostBuffer);
        strcat(NewName, Pos + 1);
        Name = NewName;
    }
    while ((Pos = strstr(Name, "$")));
    free(CostBuffer);
    return NewName;
}
