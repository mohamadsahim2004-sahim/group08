#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CANDIDATES 5
#define MAX_DISTRICTS 5
#define MAX_PARTIES 3
#define MAX_NAME_LENGTH 20
#define PASSWORD "official@1234"
#define CURRENT_YEAR 2025 // NEW: define current year

// --- Data Structures ---
typedef struct {
    char name[MAX_NAME_LENGTH];
    char nic[20];
    int number;
    int votes;
} Candidate;

typedef struct {
    Candidate parties[MAX_PARTIES][MAX_CANDIDATES];
} District;

District districts[MAX_DISTRICTS];

const char *district_names[MAX_DISTRICTS] = {"matara", "galle", "hambantota", "colombo", "kandy"};
const char party_names[MAX_PARTIES] = {'A', 'B', 'C'};

// --- Utility Functions ---

// Validate 12-digit numeric NIC
int isValidNIC(const char *nic) {
    size_t length = strlen(nic);

    // Check for the 12-character format (new NIC)
    if (length == 12) {
        for (int i = 0; i < 12; i++) {
            if (nic[i] < '0' || nic[i] > '9') {
                return 0; // Not all characters are digits
            }
        }
        return 1; // Valid 12-digit NIC
    }
    
    // Check for the 10-character format (old NIC)
    else if (length == 10) {
        // The first 9 characters must be digits
        for (int i = 0; i < 9; i++) {
            if (nic[i] < '0' || nic[i] > '9') {
                return 0; // Not all first 9 characters are digits
            }
        }
        
        // The 10th character must be 'V' or 'X' (case-insensitive check added for robustness)
        char lastChar = toupper(nic[9]);
        if (lastChar == 'V' || lastChar == 'X') {
            return 1; // Valid 10-character NIC
        } else {
            return 0; // Last character is not 'V' or 'X'
        }
    }
    
    // Any other length is invalid
    return 0;
}

// Extract birth year from NIC (first 4 digits)
int getBirthYearFromNIC(const char *nic) {
    char year_str[5];
    strncpy(year_str, nic, 4);
    year_str[4] = '\0';
    return atoi(year_str);
}

// Ensure candidate number is unique across all districts/parties
int isUniqueCandidateNumber(int number) {
    for (int d = 0; d < MAX_DISTRICTS; d++) {
        for (int p = 0; p < MAX_PARTIES; p++) {
            for (int i = 0; i < MAX_CANDIDATES; i++) {
                if (districts[d].parties[p][i].number == number) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int getDistrictIndex(int choice) {
    if (choice >= 1 && choice <= MAX_DISTRICTS)
        return choice - 1;
    return -1;
}

int getPartyIndex(char party) {
    for (int i = 0; i < MAX_PARTIES; i++) {
        if (party_names[i] == party || party_names[i] == party - 32)
            return i;
    }
    return -1;
}

int hasVoted(const char *nic) {
    FILE *fv = fopen("voters.txt", "r");
    if (fv == NULL) return 0;
    char line[200];
    while (fgets(line, sizeof(line), fv)) {
        if (strstr(line, nic) != NULL) {
            fclose(fv);
            return 1;
        }
    }
    fclose(fv);
    return 0;
}

int isCandidateRegistered(const char *nic) {
    FILE *fc = fopen("candidates.txt", "r");
    if (fc == NULL) return 0;
    char line[200];
    while (fgets(line, sizeof(line), fc)) {
        if (strstr(line, nic) != NULL) {
            fclose(fc);
            return 1;
        }
    }
    fclose(fc);
    return 0;
}


// Main Menu
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
            case 1: vote();
             break;
            case 2: registerCandidate();
             break;
            case 3: showOfficials();
             break;
            case 4: printf("Exiting...\n");
             return 0;
            default: printf("Invalid choice!\n");
             break;
        }
    }
}