#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CANDIDATES 5
#define MAX_DISTRICTS 5
#define MAX_PARTIES 3
#define MAX_NAME_LENGTH 20
#define PASSWORD "official@1234"
#define CURRENT_YEAR 2025

// ----------- Data Structures -----------
typedef struct {
    char name[MAX_NAME_LENGTH];
    char nic[20];
    int number;
    int votes;
} Candidate;

typedef struct {
    Candidate parties[MAX_PARTIES][MAX_CANDIDATES];
} District;

// Global arrays
District districts[MAX_DISTRICTS];
const char *district_names[MAX_DISTRICTS] = {"matara", "galle", "hambantota", "colombo", "kandy"};
const char party_names[MAX_PARTIES] = {'A', 'B', 'C'};

// ----------- Shared Utility Functions -----------
int isValidNIC(const char *nic) {
    size_t length = strlen(nic);
    if (length == 12) {
        for (int i = 0; i < 12; i++) if (!isdigit(nic[i])) return 0;
        return 1;
    } else if (length == 10) {
        for (int i = 0; i < 9; i++) if (!isdigit(nic[i])) return 0;
        char lastChar = toupper(nic[9]);
        return (lastChar == 'V' || lastChar == 'X');
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
        if (toupper(party) == party_names[i])
            return i;
    return -1;
}

int hasVoted(const char *nic) {
    FILE *fv = fopen("voters.txt", "r");
    if (!fv) return 0;
    char line[200];
    while (fgets(line, sizeof(line), fv))
        if (strstr(line, nic)) { fclose(fv); return 1; }
    fclose(fv);
    return 0;
}

int isCandidateRegistered(const char *nic) {
    FILE *fc = fopen("candidates.txt", "r");
    if (!fc) return 0;
    char line[200];
    while (fgets(line, sizeof(line), fc))
        if (strstr(line, nic)) { fclose(fc); return 1; }
    fclose(fc);
    return 0;
}

// ----------- Function Declarations (from other files) -----------
void registerCandidate();
void vote();
void showOfficials();

// ----------- Main Menu -----------
int main() {
    int choice;
    while (1) {
        printf("\n--------------------------------------------------\n");
        printf("|      Welcome to the Election Voting System      |\n");
        printf("--------------------------------------------------\n");
        printf("1. Voting\n");
        printf("2. Candidate Register\n");
        printf("3. Officials\n");
        printf("4. Exit\n");
        printf("--------------------------------------------------\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: vote(); break;
            case 2: registerCandidate(); break;
            case 3: showOfficials(); break;
            case 4: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice!\n"); break;
        }
    }
}
