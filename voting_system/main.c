#include "election.h"

int main() {
    loadCandidates();
    loadVotes();

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
        }
    }
}