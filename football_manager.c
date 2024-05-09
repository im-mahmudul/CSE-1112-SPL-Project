#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEAMS 10
#define MAX_PLAYERS_PER_TEAM 20


typedef struct {
    char player_name[50];
    int player_number;
    char player_position[10];
    int goals_scored;
    int goals_conceded;
    int number_of_minutes_played;
} Player;


typedef struct {
    char team_name[50];
    Player players[MAX_PLAYERS_PER_TEAM];
    int num_players;
} Team;


typedef struct {
    char team_name[50];
    int most_goals_scored;
    int least_goals_conceded;
} Statistics;


Team teams[MAX_TEAMS];
int num_teams = 0;


Statistics stats;


void addPlayerToTeam();
void saveTeamsToFile();
void loadTeamsFromFile();
void displayPlayer();
void displayTeam();
void updateStatistics();
void displayStatistics();
void saveStatisticsToFile();
void loadStatisticsFromFile();


void addPlayerToTeam() {
    if (num_teams >= MAX_TEAMS) {
        printf("Maximum teams reached. Cannot add more teams.\n");
        return;
    }

    Player new_player;
    printf("Enter player name: ");
    fgets(new_player.player_name, sizeof(new_player.player_name), stdin);
    new_player.player_name[strcspn(new_player.player_name, "\n")] = '\0';
    printf("Enter player number: ");
    scanf("%d", &new_player.player_number);
    getchar();
    printf("Enter player position: ");
    fgets(new_player.player_position, sizeof(new_player.player_position), stdin);
    new_player.player_position[strcspn(new_player.player_position, "\n")] = '\0';
    printf("Enter goals scored: ");
    scanf("%d", &new_player.goals_scored);
    getchar();
    printf("Enter goals conceded: ");
    scanf("%d", &new_player.goals_conceded);
    getchar();
    printf("Enter number of minutes played: ");
    scanf("%d", &new_player.number_of_minutes_played);
    getchar();

    char team_name[50];
    printf("Enter team name to add player to: ");
    fgets(team_name, sizeof(team_name), stdin);
    team_name[strcspn(team_name, "\n")] = '\0';

    int i;
    for (i = 0; i < num_teams; i++) {
        if (strcmp(teams[i].team_name, team_name) == 0) {
            if (teams[i].num_players < MAX_PLAYERS_PER_TEAM) {
                teams[i].players[teams[i].num_players] = new_player;
                teams[i].num_players++;
                printf("Player added successfully to team %s\n", team_name);
                saveTeamsToFile();
                updateStatistics();
                return;
            } else {
                printf("Cannot add player. Team %s already has maximum players.\n", team_name);
                return;
            }
        }
    }


    strcpy(teams[num_teams].team_name, team_name);
    teams[num_teams].players[0] = new_player;
    teams[num_teams].num_players = 1;
    num_teams++;
    printf("New team created: %s\n", team_name);
    saveTeamsToFile();
    updateStatistics();
}


void saveTeamsToFile() {
    FILE *file = fopen("football_database.txt", "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    int i, j;
    for (i = 0; i < num_teams; i++) {
        fprintf(file, "Team: %s\n", teams[i].team_name);
        for (j = 0; j < teams[i].num_players; j++) {
            fprintf(file, "Player: %s,%d,%s,%d,%d,%d\n",
                    teams[i].players[j].player_name,
                    teams[i].players[j].player_number,
                    teams[i].players[j].player_position,
                    teams[i].players[j].goals_scored,
                    teams[i].players[j].goals_conceded,
                    teams[i].players[j].number_of_minutes_played);
        }
    }

    fclose(file);
    printf("Team information saved to file successfully.\n");
}


void loadTeamsFromFile() {
    FILE *file = fopen("football_database.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    num_teams = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Team:")) {
            sscanf(line, "Team: %[^\n]", teams[num_teams].team_name);
            teams[num_teams].num_players = 0;
            num_teams++;
        } else if (strstr(line, "Player:")) {
            sscanf(line, "Player: %[^,],%d,%[^,],%d,%d,%d",
                    teams[num_teams - 1].players[teams[num_teams - 1].num_players].player_name,
                    &teams[num_teams - 1].players[teams[num_teams - 1].num_players].player_number,
                    teams[num_teams - 1].players[teams[num_teams - 1].num_players].player_position,
                    &teams[num_teams - 1].players[teams[num_teams - 1].num_players].goals_scored,
                    &teams[num_teams - 1].players[teams[num_teams - 1].num_players].goals_conceded,
                    &teams[num_teams - 1].players[teams[num_teams - 1].num_players].number_of_minutes_played);
            teams[num_teams - 1].num_players++;
        }
    }

    fclose(file);
    printf("Team information loaded from file successfully.\n");
}


void updateStatistics() {
    int max_goals_scored = -1;
    int min_goals_conceded = INT_MAX;

    int i, j;
    for (i = 0; i < num_teams; i++) {
        for (j = 0; j < teams[i].num_players; j++) {
            if (teams[i].players[j].goals_scored > max_goals_scored) {
                max_goals_scored = teams[i].players[j].goals_scored;
                strcpy(stats.team_name, teams[i].team_name);
                stats.most_goals_scored = max_goals_scored;
            }
            if (teams[i].players[j].goals_conceded < min_goals_conceded) {
                min_goals_conceded = teams[i].players[j].goals_conceded;
                strcpy(stats.team_name, teams[i].team_name);
                stats.least_goals_conceded = min_goals_conceded;
            }
        }
    }
    saveStatisticsToFile();
}


void displayStatistics() {
    loadStatisticsFromFile();

    printf("--- Statistics ---\n");
    printf("Team with most goals scored: %s\n", stats.team_name);
    printf("Most goals scored: %d\n", stats.most_goals_scored);
    printf("Team with least goals conceded: %s\n", stats.team_name);
    printf("Least goals conceded: %d\n", stats.least_goals_conceded);
}


void saveStatisticsToFile() {
    FILE *file = fopen("stats.txt", "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "Team with most goals scored: %s\n", stats.team_name);
    fprintf(file, "Most goals scored: %d\n", stats.most_goals_scored);
    fprintf(file, "Team with least goals conceded: %s\n", stats.team_name);
    fprintf(file, "Least goals conceded: %d\n", stats.least_goals_conceded);

    fclose(file);
    printf("Statistics saved to file successfully.\n");
}


void loadStatisticsFromFile() {
    FILE *file = fopen("stats.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    fscanf(file, "Team with most goals scored: %[^\n]\n", stats.team_name);
    fscanf(file, "Most goals scored: %d\n", &stats.most_goals_scored);
    fscanf(file, "Team with least goals conceded: %[^\n]\n", stats.team_name);
    fscanf(file, "Least goals conceded: %d\n", &stats.least_goals_conceded);

    fclose(file);
    printf("Statistics loaded from file successfully.\n");
}


void displayPlayer() {
    loadTeamsFromFile();

    char player_name[50];
    printf("Enter player name: ");
    fgets(player_name, sizeof(player_name), stdin);
    player_name[strcspn(player_name, "\n")] = '\0';

    int i, j;
    for (i = 0; i < num_teams; i++) {
        for (j = 0; j < teams[i].num_players; j++) {
            if (strcmp(teams[i].players[j].player_name, player_name) == 0) {
                printf("--- Player Information ---\n");
                printf("Name: %s\n", teams[i].players[j].player_name);
                printf("Number: %d\n", teams[i].players[j].player_number);
                printf("Position: %s\n", teams[i].players[j].player_position);
                printf("Goals Scored: %d\n", teams[i].players[j].goals_scored);
                printf("Goals Conceded: %d\n", teams[i].players[j].goals_conceded);
                printf("Minutes Played: %d\n", teams[i].players[j].number_of_minutes_played);
                return;
            }
        }
    }
    printf("Player not found.\n");
}


void displayTeam() {
    loadTeamsFromFile();

    printf("--- Team Information ---\n");
    int i, j;
    for (i = 0; i < num_teams; i++) {
        printf("Team: %s\n", teams[i].team_name);
        for (j = 0; j < teams[i].num_players; j++) {
            printf("Player: %s, Number: %d\n",
                    teams[i].players[j].player_name,
                    teams[i].players[j].player_number);
        }
        printf("\n");
    }
}

int main() {
    int choice;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Add player to team\n");
        printf("2. Display player information\n");
        printf("3. Display team information\n");
        printf("4. Display statistics\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addPlayerToTeam();
                break;
            case 2:
                displayPlayer();
                break;
            case 3:
                displayTeam();
                break;
            case 4:
                displayStatistics();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    } while (choice != 5);

    return 0;
}
