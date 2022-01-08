#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// Maximum voters and candidates
#define MAXVOTERS 100
#define MAXCANDIDATES 9


int preferences[MAXVOTERS][MAXCANDIDATES];


typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAXCANDIDATES];

// Counts voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAXCANDIDATES)
    {
        printf("The maximum number of candidates is %i\n", MAXCANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAXVOTERS)
    {
        printf("The maximum number of voters is %i\n", MAXVOTERS);
        return 3;
    }

    for (int i = 0; i < voter_count; i++)
    {

        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // loops until a winner is declared
    while (true)
    {
        tabulate();

        bool won = print_winner();
        if (won)
        {
            break;
        }

        // last place candidate is removed 
        int min = find_min();
        bool tie = is_tie(min);

        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminates candidate with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}


bool vote(int voter, int rank, string name)
{
    bool exist = false;
    for (int i = 0; i < candidate_count; i++)
    {
     
        if (strcmp(name, candidates[i].name) == 0)
        {
            
            preferences[voter][rank] = i;
            exist = true;
            break;
        
        }
    }
    return exist;
}


void tabulate(void)
{
    
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (candidates[preferences[i][j]].eliminated == false)
            {
                candidates[preferences[i][j]].votes += 1;
                break;
            }
        }
        
    }

    return;
}

// Prints out election winner 
bool print_winner(void)
{
    
    for (int i = 0; i < candidate_count; i++)
    {
        string most = candidates[i].name;
        if (candidates[i].votes > voter_count / 2)
        {
            printf("%s\n", most);
            return true;
        }
    }
    return false;
}

int find_min(void)
{
    int minvotes = voter_count;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false && candidates[i].votes < minvotes)
        {
            minvotes = candidates[i].votes;
        }
    }
    return minvotes;
}


bool is_tie(int minvotes)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false && candidates[i].votes != minvotes)
        {
            return false;
        }
    }
    return true;
}

// Eliminates candidate in last place
void eliminate(int minvotes)
{
    for (int i = 0; i < candidate_count; i++)
        if (candidates[i].votes == minvotes)
        {
            candidates[i].eliminated = true;
        }
    return;
}
