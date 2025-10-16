#include <stdio.h>
#include <string.h>
#include <conio.h> // Windows only

// Function prototypes
void bootScreen();
void accessMenu();
void registerUser();
int loginUser(char loggedUser[], char role[]);
void inputPassword(char password[]);
void citizenPage(char username[]);
void governmentPage();
int typeofAppeal();
void submitAppeal(char username[]);
void governmentRespondToAppeal();

// Convert appeal type number to readable text
const char* getAppealType(int type) {
    switch (type) {
        case 1: return "Infrastructure";
        case 2: return "Public Services";
        case 3: return "Environmental";
        case 4: return "Social Services";
        case 5: return "Others";
        default: return "Unknown";
    }
}

// Boot screen
void bootScreen() {
    printf("=============================================================\n");
    printf("             CITIZEN FEEDBACK AND APPEAL SYSTEM              \n");
    printf("=============================================================\n");
    printf("      Welcome! Connecting Citizens and the Government.       \n");
    printf("-------------------------------------------------------------\n");
    printf("              Developed by: Rhey Gwapo                      \n");
    printf("        Course: Programming Logic and Design                 \n");
    printf("=============================================================\n\n");
}

// Hidden password input
void inputPassword(char password[]) {
    char ch;
    int i = 0;
    printf("Enter your password: ");
    while ((ch = getch()) != '\r') { // Stop at Enter key
        if (ch == '\b') { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

// Main menu for register/login
void accessMenu() {
    int choice;
    char loggedUser[100] = "";
    char role[50] = "";

    while (1) {
        printf("\n==================== MAIN MENU ====================\n");
        printf("1. Register (Citizen)\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("===================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser(loggedUser, role)) {
                    if (strcmp(role, "citizen") == 0)
                        citizenPage(loggedUser);
                    else if (strcmp(role, "government") == 0)
                        governmentPage();
                    else
                        printf("Unknown role. Access denied.\n");
                }
                break;
            case 3:
                printf("Exiting system... Goodbye!\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

// Registration (automatic citizen role)
void registerUser() {
    FILE *pFile = fopen("user.txt", "a");
    if (!pFile) {
        printf("Error opening file!\n");
        return;
    }

    char username[50], password[50];
    printf("\n==================== REGISTRATION ====================\n");
    printf("Enter your username: ");
    scanf("%s", username);
    getchar();
    inputPassword(password);

    fprintf(pFile, "%s,%s,citizen\n", username, password);
    fclose(pFile);

    printf("\nRegistration successful! You are now a CITIZEN.\n\n");
    citizenPage(username);
}

// Login function
int loginUser(char loggedUser[], char role[]) {
    FILE *pFile = fopen("user.txt", "r");
    if (!pFile) {
        printf("No registered users found. Please register first.\n\n");
        return 0;
    }

    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[50];
    char line[200];

    printf("\n======================= LOGIN =======================\n");
    printf("Enter your username: ");
    scanf("%s", username);
    getchar();
    inputPassword(password);

    int found = 0;
    while (fgets(line, sizeof(line), pFile)) {
        line[strcspn(line, "\n")] = 0;
        sscanf(line, "%[^,],%[^,],%s", fileUser, filePass, fileRole);
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            found = 1;
            strcpy(loggedUser, fileUser);
            strcpy(role, fileRole);
            printf("Login successful! Role: %s\n\n", role);
            break;
        }
    }

    fclose(pFile);
    if (!found) {
        printf("Invalid username or password.\n\n");
        return 0;
    }
    return 1;
}

// Citizen dashboard
void citizenPage(char username[]) {
    FILE *file = fopen("appeals.txt", "r");
    char user[50], desc[500], response[500];
    int type;

    printf("\n=============================================================\n");
    printf("                     CITIZEN DASHBOARD                       \n");
    printf("=============================================================\n");
    printf("Welcome, %s!\n", username);
    printf("Below are all submitted appeals and their current status.\n");
    printf("-------------------------------------------------------------\n");
    printf("%-15s | %-18s | %-35s | %-15s\n", "User", "Appeal Type", "Description", "Response");
    printf("-------------------------------------------------------------\n");

    if (file) {
        char line[1024];
        while (fgets(line, sizeof(line), file)) {
            if (sscanf(line, "User: %[^|]| Type: %d | Description: %[^|]| Response: %[^\n]",
                       user, &type, desc, response) == 4) {
                printf("%-15s | %-18s | %-35s | %-15s\n", user, getAppealType(type), desc, response);
            }
        }
        fclose(file);
    } else {
        printf("No appeals found yet.\n");
    }

    printf("=============================================================\n\n");

    int choice;
    while (1) {
        printf("1. Submit an Appeal\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                submitAppeal(username);
                return;
            case 2:
                printf("Logging out...\n\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

// Submit appeal
void submitAppeal(char username[]) {
    FILE *file = fopen("appeals.txt", "a");
    if (!file) { printf("Error saving appeal.\n"); return; }

    int type = typeofAppeal();
    char desc[500];
    getchar();
    printf("Enter your appeal description:\n");
    fgets(desc, sizeof(desc), stdin);
    desc[strcspn(desc, "\n")] = '\0';

    fprintf(file, "User: %s | Type: %d | Description: %s | Response: Pending\n", username, type, desc);
    fclose(file);

    printf("\nYour appeal has been submitted successfully!\n\n");
}

// Appeal types
int typeofAppeal() {
    int type;
    printf("\n============= CHOOSE TYPE OF APPEAL =============\n");
    printf("1. Infrastructure Issues\n");
    printf("2. Public Services\n");
    printf("3. Environmental Concerns\n");
    printf("4. Social Services\n");
    printf("5. Others\n");
    printf("=================================================\n");
    printf("Enter choice: ");
    scanf("%d", &type);
    return type;
}

// Government dashboard
void governmentPage() {
    FILE *file = fopen("appeals.txt", "r");
    char user[50], desc[500], response[500];
    int type;

    printf("\n=============================================================\n");
    printf("                    GOVERNMENT DASHBOARD                     \n");
    printf("=============================================================\n");
    printf("Below are all submitted citizen appeals and their status.\n");
    printf("-------------------------------------------------------------\n");
    printf("%-15s | %-18s | %-35s | %-15s\n", "User", "Appeal Type", "Description", "Response");
    printf("-------------------------------------------------------------\n");

    if (file) {
        char line[1024];
        while (fgets(line, sizeof(line), file)) {
            if (sscanf(line, "User: %[^|]| Type: %d | Description: %[^|]| Response: %[^\n]",
                       user, &type, desc, response) == 4) {
                printf("%-15s | %-18s | %-35s | %-15s\n", user, getAppealType(type), desc, response);
            }
        }
        fclose(file);
    } else {
        printf("No appeals found yet.\n");
    }

    printf("=============================================================\n\n");

    int choice;
    while (1) {
        printf("1. Respond to an Appeal\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                governmentRespondToAppeal();
                return;
            case 2:
                printf("Logging out...\n\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

// Respond to appeal
void governmentRespondToAppeal() {
    char username[50], response[500];
    char tempFile[] = "appeals_temp.txt";

    printf("Enter citizen username to respond: ");
    scanf("%s", username);
    getchar();

    printf("Enter your response: ");
    fgets(response, sizeof(response), stdin);
    response[strcspn(response, "\n")] = '\0';

    FILE *file = fopen("appeals.txt", "r");
    FILE *temp = fopen(tempFile, "w");
    if (!file || !temp) { printf("Error opening file.\n"); return; }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, username) && strstr(line, "Response: Pending")) {
            char updatedLine[1024];
            sscanf(line, "User: %[^|]| Type: %*d | Description: %[^|]| Response: %*[^\n]",
                   username, response);
            snprintf(updatedLine, sizeof(updatedLine), "User: %s | Type: %d | Description: %s | Response: %s\n",
                     username, typeofAppeal(), response, response);
            fputs(updatedLine, temp);
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);
    remove("appeals.txt");
    rename(tempFile, "appeals.txt");

    printf("\nResponse submitted successfully!\n\n");
}
