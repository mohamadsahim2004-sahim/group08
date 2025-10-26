#include "election.h"

void loadVotes() {
    FILE *fv = fopen("vote_details.txt", "r");
    if (!fv) return;
    char district_name[30];
    char party;
    int cand_no;

    while (fscanf(fv, "%[^|]|%c|%d\n", district_name, &party, &cand_no) == 3) {
        for (int d = 0; d < MAX_DISTRICTS; d++) {
            if (strcmp(district_name, district_names[d]) == 0) {
                int p = getPartyIndex(party);
                if (p != -1) {
                    for (int i = 0; i < MAX_CANDIDATES; i++) {
                        if (districts[d].parties[p][i].number == cand_no) {
                            districts[d].parties[p][i].votes++;
                            break;
                        }
                    }
                }
            }
        }
    }
    fclose(fv);
}

void vote() {
    char name[MAX_NAME_LENGTH], nic[20];
    int age, district_choice, vote_num;

    printf("Enter your age: ");
    scanf("%d", &age);
    if (age < 18) {
        printf("You are not eligible to vote.\n");
        return;
    }

    printf("Enter your name: ");
    scanf(" %[^\n]", name);
    printf("Enter your 9(V) or 12-digit NIC: ");
    scanf("%s", nic);

    if (!isValidNIC(nic)) {
        printf("Invalid NIC format.\n");
        return;
    }

    int birth_year = getBirthYearFromNIC(nic);
    int calc_age = CURRENT_YEAR - birth_year;
    if (calc_age != age) {
        printf("Age did not match! Based on NIC, your age should be %d.\n", calc_age);
        return;
    }

    if (hasVoted(nic)) {
        printf("You have already voted.\n");
        return;
    }

    printf("\n---Choose your district---\n");
    for (int i = 0; i < MAX_DISTRICTS; i++)
        printf("%d. %s\n", i + 1, district_names[i]);
    printf("Enter your choice (1-5): ");
    scanf("%d", &district_choice);

    int d = getDistrictIndex(district_choice);
    if (d == -1) {
        printf("Invalid district.\n");
        return;
    }

    printf("\n__VOTE__\n");
    printf("     Ballot Paper\n\n");

    for (int p = 0; p < MAX_PARTIES; p++) {
        printf("  Party %c   Candidate No\n", party_names[p]);
        for (int i = 0; i < MAX_CANDIDATES; i++) {
            if (districts[d].parties[p][i].number != 0)
                printf("%d. %s \t%02d\n", i + 1, districts[d].parties[p][i].name, districts[d].parties[p][i].number);
            else
                printf("%d. candidate \t00\n", i + 1);
        }
        printf("\n");
    }

    printf("Enter candidate number: ");
    scanf("%d", &vote_num);

    for (int p = 0; p < MAX_PARTIES; p++) {
        for (int i = 0; i < MAX_CANDIDATES; i++) {
            if (districts[d].parties[p][i].number == vote_num) {
                districts[d].parties[p][i].votes++;

                FILE *fv = fopen("voter_details.txt", "a");
                if (fv) {
                    fprintf(fv, "Name: %s | NIC: %s | District: %s | Age: %d\n",
                            name, nic, district_names[d], age);
                    fclose(fv);
                }

                printf("Vote accepted!\n");
                return;
            }
        }
    }

    printf("Invalid candidate number.\n");
}
