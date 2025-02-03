#include <cs50.h>
#include <stdio.h>
#include <string.h>

// MAX number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] - preferences[j][i] > 0)
            {
                pairs[count].winner = i;
                pairs[count].loser = j;
                count++;
            }
            else if (preferences[i][j] - preferences[j][i] < 0)
            {
                pairs[count].winner = j;
                pairs[count].loser = i;
                count++;
            }
        }
    }
    pair_count = count;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int temp;
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i; j < pair_count - 1; j++)
        {
            if (!(preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[j + 1].winner][pairs[j + 1].loser]))
            {
                temp = pairs[j].winner;
                pairs[j].winner = pairs[j + 1].winner;
                pairs[j + 1].winner = temp;

                temp = pairs[j].loser;
                pairs[j].loser = pairs[j + 1].loser;
                pairs[j + 1].loser = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
bool DFSUtil(int node, int colors[], int aim)
{
    colors[node] = 1;

    if (locked[node][aim])
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[node][i] && colors[i] == 0)
        {
            if (DFSUtil(i, colors, aim))
            {
                return true;
            }
        }
    }

    colors[node] = 2;
    return false;
}
void lock_pairs(void)
{
    int colors[candidate_count];

    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            colors[j] = 0;
        }

        locked[pairs[i].winner][pairs[i].loser] = true;
        if (DFSUtil(pairs[i].winner, colors, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }

    return;
}
// Print the winner of the election
void print_winner(void)
{
    bool temp;
    int winner = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        temp = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                temp = true;
                break;
            }
        }
        if (!temp)
        {
            winner = i;
            break;
        }
    }

    printf("%s\n", candidates[winner]);
    return;
}
