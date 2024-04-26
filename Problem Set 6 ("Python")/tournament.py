# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000
teams = []


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")
    # TODO: Read teams into memory from file
    with open(sys.argv[1]) as file:
        file_reader = csv.DictReader(file, fieldnames=["team", "rating"])
        for row in file_reader:
            single_team_dict = dict()
            team_name = row["team"]
            try:
                team_rating = int(row["rating"])
            except ValueError:
                continue
            single_team_dict["team"] = team_name
            single_team_dict["rating"] = team_rating
            teams.append(single_team_dict)

    counts = {}
    number_of_wins = 0
    # TODO: Simulate N tournaments and keep track of win counts
    for i in range(N):
        one_winner = simulate_tournament(teams)
        if one_winner in counts:
            counts[one_winner] += 1
        else:
            counts[one_winner] = 1

    # Print each team"s chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # TODO
    tournament_winners = simulate_round(teams)
    while len(tournament_winners) != 1:
        tournament_winners = simulate_round(tournament_winners)
    tournament_winner_dict = tournament_winners[0]
    tournament_winner = tournament_winner_dict["team"]
    return tournament_winner


if __name__ == "__main__":
    main()
