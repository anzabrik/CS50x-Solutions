/*
2 versions of the print_winner function
1. print_winner - passes check50. BUT is wrong.
     In this function, the winner is the candidate who doesn't have anyone locked above him.
     This algorithm is wrong because if there's a candidate who doesn't have a pair, this function print it out.
     And check50 is ok with this( The example input where it fails is at the bottom of the file.
2. print_winner2 - fails check50. BUT is seemingly right.
     In this function, the winner is determined based on the graph pairs_locked. Here, we check if someone who is
     pairs_locked[i].winner (is a winner in a locked pair) is also a loser in any locked pair (pairs_locked[j].loser).
     I even sorted winners to satisfy check50 in case there're multiple sources, but still failedthe check.
     That's probably because this version  doesn't let people who are not in locked pairs AT ALL become winners.
*/


#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
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
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];
pair pairs_locked[MAX * (MAX - 1) / 2];
// int winners[MAX*MAX]; // For the "working" print_winner

int pair_count;
int candidate_count;
int pairs_locked_count;
// int winner_count; // For the "working" print_winner


// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool cycle(pair pair_in_question);
void print_winner(void);
// void print_winner2(void); // The "working" print_winner version
bool isloser(int winnner_in_question);
bool is_already_among_winners(int winner_in_question);
void sort_winners(void);

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
    print_winner();// This version passes check50, but prints "winners" who had no locked pairs at all
    // print_winner2(); // The "working" print_winner
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++) //For each candidate in candidates
    {
        if (!strcmp(candidates[i], name)) //If names coincide
        {
            ranks[rank] = i; //The number of candidate is placed in ranks (list of voter's preferences)
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // for all items in ranks - add 1 for each pair
    for (int i = 0; i < candidate_count - 1; i++)//-2 because: 5 candidates, 4 indices, first elt in pair is last but one (i=3)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]  += 1;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Loop through Preferences, then record based on: preferences[winner][loser]
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory DIDN"T REALLY CHECK THIS with debug
void sort_pairs(void)
{
    for (int j = 0; j < pair_count; j++)
    {
        pair swap;
        int swap_counter = 0;
        for (int i = 0; i < pair_count - 1; i++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                swap = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = swap;
                swap_counter = 1;
            }
        }
        if (swap_counter == 0)//if no swaps, quit
        {
            break;
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    locked[pairs[0].winner][pairs[0].loser] = true;//lock 1st pair
    pairs_locked[0] = pairs[0];
    pairs_locked_count = 1;

    for (int i = 1; i < pair_count; i++)//go through all existing pairs
    {
        pair pair_in_question = pairs[i];
        if (!cycle(pair_in_question)) //take one pair and check whether locking it creates a cycle
        {
            locked[pairs[i].winner][pairs[i].loser] = true;//if not, lock it (LATER make sure you only lock VICTORIES)
            pairs_locked[pairs_locked_count] = pairs[i];
            pairs_locked_count += 1;
        }
    }
}

bool cycle(pair p_i_q)
{
    if (p_i_q.winner == p_i_q.loser)
    {
        return true;
    }
    for (int p = pairs_locked_count - 1; p >= 0; p--)
    {
        if (p_i_q.winner == pairs_locked[p].loser)
        {
            p_i_q.winner = pairs_locked[p].winner;
            return cycle(p_i_q);
        }
    }
    return false;
}

// Print the winner of the election, the heartbreaking version
void print_winner(void)
{
    // For each locked[][i]check whether this candidate has at least a single TRUE on it vertice
    for (int i = 0; i < candidate_count; i++)
    {
        int false_counter = 0;
        for (int j = 0; j < candidate_count; j++)//for each locked[j][]
        {
            if (locked[j][i] == false)
            {
                false_counter += 1;
            }
        }
        if (false_counter == candidate_count)
        {
            printf("%s\n", candidates[i]);
            //break;//works with and without break, prints out tied candidates
        }
    }
}

/*
void print_winner2(void)
{
    winner_count = 0;
    for (int i = 0; i < pairs_locked_count; i++)//take winner whose index in pairs-locked is i
    {
        // If she is not a loser and not mentioned in the winners array
        if (!isloser(pairs_locked[i].winner) && !is_already_among_winners(pairs_locked[i].winner))
        {
            winners[winner_count] = pairs_locked[i].winner;//add her to the winners array
            winner_count += 1;
        }
    }
    //specifically to feed wicked check50 - sort candidates so as to print them in alphabetical order (when tied)
    sort_winners();
    for (int j = 0; j < winner_count; j++)
    {
        int m = winners[j];
        printf("%s\n", candidates[m]);
        //break; // With or without the break, it is not accepted
    }
}

bool isloser(int winner_in_question)
{
    for (int p = 0; p < pairs_locked_count; p++)//check against every loser in pairs-locked
    {
        if (winner_in_question == pairs_locked[p].loser)
        {
            return true;
        }
    }
    return false;
}

bool is_already_among_winners(int winner_in_question)
{
    for (int i = 0; i < winner_count; i++)
    {
        if (winner_in_question == winners[i])
        {
            return true;
        }
    }
    return false;
}

void sort_winners(void)
{
    int winner_to_swap;
    int swap_counter = 0;
    for (int j = 0; j < winner_count; j++)
    {
        for (int i = 0; i < winner_count - 1; i++)
        {
            if (winners[i] > winners[i + 1])
            {
                winner_to_swap = winners[i];
                winners[i] = winners[i + 1];
                winners[i + 1] = winner_to_swap;
                swap_counter = 1;
            }
        }
        if (swap_counter == 0)//if no swaps, quit
        {
            break;
        }
    }
}


2 voters

Alice
Bob
Charlie
David
Emily
Frankie
Geena
Henry

Henry
Geena
Charlie
David
Frankie
Emily
Bob
Alice

the approved by check50 function gives alice, bob, charlie, geena, henry,
while only one out of them is a real winner
mine gives only Charlie (and he IS really the source of the graph)
*/