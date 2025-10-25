// candidate.c
#include "election.h"

District districts[MAX_DISTRICTS];
const char *district_names[MAX_DISTRICTS] = {"matara", "galle", "hambantota", "colombo", "kandy"};
const char party_names[MAX_PARTIES] = {'A', 'B', 'C'};

// Utility Functions
int isValidNIC(const char *nic) {
    size_t length = strlen(nic);
    if (length == 12) {
        for (int i = 0; i < 12; i++)
            if (!isdigit(nic[i])) return 0;
        return 1;
    } else if (length == 10) {
        for (int i = 0; i < 9; i++)
            if (!isdigit(nic[i])) return 0;
        char last = toupper(nic[9]);
        return (last == 'V' || last == 'X');
    }
    return 0;
}

int getBirthYearFromNIC(const char *nic) {
    char year_str[5];
    strncpy(year_str, nic, 4);
    year_str[4] = '\0';
    return atoi(year_str);
}

int isUniqueCandidateNumber(int number) {
    for (int d = 0; d < MAX_DISTRICTS; d++)
        for (int p = 0; p < MAX_PARTIES; p++)
            for (int i = 0; i < MAX_CANDIDATES; i++)
                if (districts[d].parties[p][i].number == number)
                    return 0;
    return 1;
}

int getDistrictIndex(int choice) {
    return (choice >= 1 && choice <= MAX_DISTRICTS) ? choice - 1 : -1;
}

int getPartyIndex(char party) {
    for (int i = 0; i < MAX_PARTIES; i++)
        if (toupper(party_names[i]) == toupper(party))
            return i;
    return -1;
}

int hasVoted(const char *nic) {
    FILE *fv = fopen("voters.txt", "r");
    if (!fv) return 0;
    char line[200];
    while (fgets(line, sizeof(line), fv))
        if (strstr(line, nic)) {
            fclose(fv);
            return 1;
        }
    fclose(fv);
    return 0;
}

int isCandidateRegistered(const char *nic) {
    FILE *fc = fopen("candidates.txt", "r");
    if (!fc) return 0;
    char line[200];
    while (fgets(line, sizeof(line), fc))
        if (strstr(line, nic)) {
            fclose(fc);
            return 1;
        }
    fclose(fc);
    return 0;
}

// --- Candidate Registration Function ---
void registerCandidate() {
    char name[MAX_NAME_LENGTH], nic[20], party;
    int number, district_choice, age;

    printf("\n_CANDIDATE REGISTER_\n");
    printf("Enter your name: ");
    scanf(" %[^\n]", name);

    printf("Enter your 10 or 12 digit NIC No: ");
    scanf("%s", nic);
    if (!isValidNIC(nic)) {
        printf("Invalid NIC format.\n");
        return;
    }

    printf("Enter your age: ");
    scanf("%d", &age);

    int birth_year = getBirthYearFromNIC(nic);
    int calc_age = CURRENT_YEAR - birth_year;

    if (calc_age != age) {
        printf("Age mismatch! Based on NIC, your age should be %d.\n", calc_age);
        return;
    }

    if (isCandidateRegistered(nic)) {
        printf("This NIC is already registered as a candidate.\n");
        return;
    }

    printf("Enter your candidate number: ");
    scanf("%d", &number);

    if (!isUniqueCandidateNumber(number)) {
        printf("Candidate number already exists.\n");
        return;
    }

    printf("---Choose your district---\n");
    for (int i = 0; i < MAX_DISTRICTS; i++)
        printf("%d. %s\n", i + 1, district_names[i]);
    printf("Enter your choice (1-5): ");
    scanf("%d", &district_choice);

    printf("Choose your party (A, B, C): ");
    scanf(" %c", &party);

    int d = getDistrictIndex(district_choice);
    int p = getPartyIndex(party);
    if (d == -1 || p == -1) {
        printf("Invalid district or party.\n");
        return;
    }

    for (int i = 0; i < MAX_CANDIDATES; i++) {
        if (districts[d].parties[p][i].number == 0) {
            strcpy(districts[d].parties[p][i].name, name);
            strcpy(districts[d].parties[p][i].nic, nic);
            districts[d].parties[p][i].number = number;
            districts[d].parties[p][i].votes = 0;

            FILE *fc = fopen("candidates.txt", "a");
            if (fc) {
                fprintf(fc, "Name: %s | NIC: %s | District: %s | Party: %c | Candidate_No: %d | Age: %d\n",
                        name, nic, district_names[d], party_names[p], number, age);
                fclose(fc);
            }

            printf("\nYou are successfully registered!\n");
            return;
        }
    }

    printf("No more slots available for this party in the district.\n");
}