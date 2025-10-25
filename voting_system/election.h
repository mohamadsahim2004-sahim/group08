#ifndef ELECTION_H
#define ELECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CANDIDATES 5
#define MAX_DISTRICTS 5
#define MAX_PARTIES 5
#define MAX_NAME_LENGTH 20
#define PASSWORD "official@1234"
#define CURRENT_YEAR 2025

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

extern District districts[MAX_DISTRICTS];
extern const char *district_names[MAX_DISTRICTS];
extern const char party_names[MAX_PARTIES];

// --- Shared Utility Functions ---
int isValidNIC(const char *nic);
int getBirthYearFromNIC(const char *nic);
int isUniqueCandidateNumber(int number);
int getDistrictIndex(int choice);
int getPartyIndex(char party);
int hasVoted(const char *nic);
int isCandidateRegistered(const char *nic);

// --- Module Functions ---
void registerCandidate();
void vote();
void showOfficials();

#endif
