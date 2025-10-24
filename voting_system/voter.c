// vote.c
#include "election.h"

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
    printf("Enter your NIC: ");
    scanf("%s", nic);

    if (!isValidNIC(nic)) {
        printf("Invalid NIC format.\n");
        return;
    }

    int birth_year = getBirthYearFromNIC(nic);
    int calc_age = CURRENT_YEAR - birth_year;
    if (calc_age != age) {
        printf("Age mismatch! Based on NIC, your age should be %d.\n", calc_age);
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
    for (int p = 0; p < MAX_PARTIES; p++) {
        printf("Party %c\n", party_names[p]);
        for (int i = 0; i < MAX_CANDIDATES; i++) {
            if (districts[d].parties[p][i].number != 0)
                printf("%d. %s\t%d\n", i + 1, districts[d].parties[p][i].name,
                       districts[d].parties[p][i].number);
        }
    }

    printf("Enter candidate number: ");
    scanf("%d", &vote_num);

    for (int p = 0; p < MAX_PARTIES; p++) {
        for (int i = 0; i < MAX_CANDIDATES; i++) {
            if (districts[d].parties[p][i].number == vote_num) {
                districts[d].parties[p][i].votes++;

                FILE *fv = fopen("voters.txt", "a");
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
