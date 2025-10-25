#include "election.h"

void showOfficials() {
    char password[30];
    int district_choice;

    printf("Enter your password: ");
    scanf("%s", password);

    if (strcmp(password, PASSWORD) != 0) {
        printf("Incorrect password!\n");
        return;
    }

    printf("---Choose your district---\n");
    for (int i = 0; i < MAX_DISTRICTS; i++)
        printf("%d. %s\n", i + 1, district_names[i]);
    printf("Enter choice (1-5): ");
    scanf("%d", &district_choice);

    int d = getDistrictIndex(district_choice);
    if (d == -1) {
        printf("Invalid district!\n");
        return;
    }

    printf("\n---%s DISTRICT RESULTS---\n", district_names[d]);
    int max_votes = -1;
    char winner_name[MAX_NAME_LENGTH] = "";
    char winner_party = ' ';
    int winner_number = 0;

    for (int p = 0; p < MAX_PARTIES; p++) {
        printf("  Party %c  Cand_No  Votes\n\n", party_names[p]);
        for (int i = 0; i < MAX_CANDIDATES; i++) {
            if (districts[d].parties[p][i].number != 0) {
                int current_votes = districts[d].parties[p][i].votes;
                printf("%d. %s\t%02d    %02d\n", i + 1, districts[d].parties[p][i].name,
                                                districts[d].parties[p][i].number, current_votes);
                if (current_votes > max_votes) {
                    max_votes = current_votes;
                    strcpy(winner_name, districts[d].parties[p][i].name);
                    winner_party = party_names[p];
                    winner_number = districts[d].parties[p][i].number;
                }
            } else {
                printf("%d. candidate\t00      00\n", i + 1);
            }
        }
        printf("\n");
    }

    printf("============================================\n");
    printf("     %s DISTRICT ELECTION RESULT\n", district_names[d]);
    printf("============================================\n");
    if (max_votes <= 0)
        printf("No votes yet or no candidates registered.\n");
    else {
        printf("WINNER: %s\n", winner_name);
        printf("Party: %c\nCandidate No: %02d\nVotes: %d\n",
               winner_party, winner_number, max_votes);
    }
    printf("============================================\n");
}
