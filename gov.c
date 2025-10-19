#include <stdio.h>
#include <string.h>
#include <conio.h> // Windows only

// Function prototypes
void bootScreen();
void accessMenu();
void registerUser();
int loginUser(char loggedUser[], char role[]);
void inputPassword(char password[]);
void viewBulletinBoard();
void citizenPage(char username[]);
int typeofAppeal();
void submitAppeal(char username[]);
void governmentPage();
void governmentRespondToAppeal();
const char* getAppealType(int type);

int main() {
    bootScreen();
    accessMenu();
    return 0;
}

// Boot screen
void bootScreen() {
    printf("==============================================\n");
    printf("     CITIZEN FEEDBACK AND APPEAL SYSTEM       \n");
    printf("==============================================\n");
    printf("          Welcome to the System!              \n");
    printf("        Connecting Citizens & Government      \n");
    printf("----------------------------------------------\n");
    printf("           Developed by: Rhey Gwapo           \n");
    printf("      Course: Programming Logic and Design    \n");
    printf("==============================================\n\n");
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
                printf("\b \b"); // Erase asterisk
            }
        } else {
            password[i++] = ch;
            printf("*"); // Show asterisk
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
        printf("============ MAIN MENU ============\n");
        printf("1. Register\n"); // Automatically Register as a Citizen
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser(loggedUser, role)) {
                    if (strcmp(role, "citizen") == 0) {
                        citizenPage(loggedUser);
                    } else if (strcmp(role, "government") == 0) {
                        governmentPage();
                    } else {
                        printf("Unknown role. Access denied.\n");
                    }
                }
                break;
            case 3:
                printf("Exiting system... Goodbye!\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n\n");
        }
    }
}

// Registration (automatic citizen role)
void registerUser() {
    FILE *pFile = fopen("user.txt", "a");
    if (pFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char username[50], password[50];
    printf("\n============ REGISTRATION PAGE ============\n");
    printf("Enter your username: ");
    scanf("%s", username);
    getchar(); // clear newline
    inputPassword(password); // Hidden password input

    fprintf(pFile, "%s,%s,citizen\n", username, password);
    fclose(pFile);

    printf("\nRegistration successful! You are now a CITIZEN.\n\n");
    citizenPage(username); // automatically go to citizen page
}

// Login function (reads role)
int loginUser(char loggedUser[], char role[]) {
    FILE *pFile = fopen("user.txt", "r");
    if (pFile == NULL) {
        printf("No registered users found. Please register first.\n\n");
        return 0;
    }

    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[50];
    char line[200];

    printf("\n============ LOGIN PAGE ============\n");
    printf("Enter your username: ");
    scanf("%s", username);
    getchar(); // clear leftover newline
    inputPassword(password); // Hidden password input

    int found = 0;

    while (fgets(line, sizeof(line), pFile)) {
        line[strcspn(line, "\n")] = 0; // Remove newline
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

// Function to map appeal type number to text
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
//Balik na sad ang Bulletin Board
void viewBulletinBoard(){
        FILE *file = fopen("appeals.txt", "r");

    printf("\n=================================================================================================================\n");
    printf("                                           BULLETIN BOARD                                                        \n");
    printf("=================================================================================================================\n");
    printf("%-15s | %-20s | %-45s | %-20s\n", "User", "Type of Appeal", "Description", "Response");
    printf("-----------------------------------------------------------------------------------------------------------------\n");

    if (file) {
        char user[50], desc[500], response[500];
        int type;
        char line[1024];
        while (fgets(line, sizeof(line), file)) {
            if (sscanf(line, "User: %[^|]| Type: %d | Description: %[^|]| Response: %[^\n]", 
                       user, &type, desc, response) == 4) {
                printf("%-15s | %-20s | %-45s | %-20s\n", user, getAppealType(type), desc, response);
            }
        }
        fclose(file);
    } else {
        printf("No appeals found.\n");
    }
    printf("=================================================================================================================\n\n");

}

// Citizen page
void citizenPage(char username[]) {
    int choice;
    while (1) {
        viewBulletinBoard(); // para dara diretso after mag submit og appeal
        printf("============ CITIZEN PAGE ============\n");
        printf("Welcome, %s!\n", username);
        printf("1. Submit an Appeal\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                submitAppeal(username);
                break;
            case 2:
                printf("Logging out...\n\n");
                return;
            default:
                printf("Invalid choice. Try again.\n\n");
        }
    }
}

// Choose type of appeal
int typeofAppeal() {
    int type;
    printf("============ Choose the type of appeal ============\n");
    printf("1. Infrastructure Issues\n");
    printf("2. Public Services\n");
    printf("3. Environmental Concerns\n");
    printf("4. Social Services\n");
    printf("5. Others\n");
    printf("Enter choice: ");
    scanf("%d", &type);
    return type;
}

// Submit appeal
void submitAppeal(char username[]) {
    FILE *file = fopen("appeals.txt", "a");
    if (!file) { printf("Error saving appeal.\n"); return; }

    int type = typeofAppeal();
    char desc[500];
    getchar(); // clear newline
    printf("Enter your appeal description:");
    fgets(desc, sizeof(desc), stdin);
    desc[strcspn(desc, "\n")] = '\0';

    fprintf(file, "User: %s | Type: %d | Description: %s | Response: Pending\n", username, type, desc);
    fclose(file);

    printf("Your appeal has been submitted!\n\n");
}

// Government page
void governmentPage() {
    int choice;
    while (1) {
        viewBulletinBoard(); // same sa pikas basahi na lang cy
        printf("========== GOVERNMENT PAGE ==========\n");
        printf("1. Respond to an Appeal\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                governmentRespondToAppeal();
                return;
            case 2:
                printf("Logging out...\n\n");
                return;
            default:
                printf("Invalid choice. Try again.\n\n");
        }
    }
}

// Respond to appeal
void governmentRespondToAppeal() {
    char username[50], response[500];
    char tempFile[] = "appeals_temp.txt";

    printf("Enter citizen username to respond: ");
    scanf("%s", username);
    getchar(); // clear newline

    printf("Enter your response: ");
    fgets(response, sizeof(response), stdin);
    response[strcspn(response, "\n")] = '\0';

    FILE *file = fopen("appeals.txt", "r");
    FILE *temp = fopen(tempFile, "w");
    if (!file || !temp) { printf("Error opening file.\n"); return; }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, username) && strstr(line, "Response: Pending")) {
            char *pos = strstr(line, "Response: Pending");
            if (pos) {
                strcpy(pos, "Response: ");
                strcat(line, response);
                strcat(line, "\n");
            }
        }
        fputs(line, temp);
    }

    fclose(file);
    fclose(temp);
    remove("appeals.txt");
    rename(tempFile, "appeals.txt");

    printf("Response submitted successfully!\n\n");
}
