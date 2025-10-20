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
int typeofAppeal();
void submitAppeal(char username[]);
void viewBulletinBoard();
void governmentPage();
void governmentRespondToAppeal();
void rateGovernment();                      
float computeAverageSatisfaction();
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
        printf("1. Register (Citizen)\n");
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
    printf("\n==== REGISTRATION PAGE ====\n");
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

    printf("\n==== LOGIN PAGE ====\n");
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

// Citizen page
void citizenPage(char username[]) {
    int choice;

    while (1) {
        printf("==== CITIZEN PAGE ====\n");
        printf("1. Submit an Appeal\n");
        printf("2. View Bulletin Board\n");
        printf("3. Logout\n");
        printf("4. Rate Government");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                submitAppeal(username);
                break;
            case 2:
                viewBulletinBoard();
                break;
            case 3:
                printf("Logging out...\n\n");
            case 4: 
                rateGovernment();
                break;

                return;
            default:
                printf("Invalid choice. Try again.\n\n");
        }
    }
}

// Submit appeal
void submitAppeal(char username[]) {
    FILE *file = fopen("appeals.txt", "a");
    if (!file) { printf("Error saving appeal.\n"); return; }

    int type = typeofAppeal();
    char desc[500];
    getchar(); // clear newline
    printf("Enter your appeal description:\n");
    fgets(desc, sizeof(desc), stdin);
    desc[strcspn(desc, "\n")] = '\0';

    fprintf(file, "User: %s | Type: %d | Description: %s | Response: Pending\n", username, type, desc);
    fclose(file);

    printf("Your appeal has been submitted!\n\n");
}

// Choose type of appeal
int typeofAppeal() {
    int type;
    printf("========== Choose the type of appeal ==========\n");
    printf("1. Infrastructure Issues\n");
    printf("2. Public Services\n");
    printf("3. Environmental Concerns\n");
    printf("4. Social Services\n");
    printf("5. Others\n");
    printf("Enter choice: ");
    scanf("%d", &type);
    return type;
}
void rateGovernment() {
    FILE *file = fopen("ratings.txt", "a");
    if (!file) { printf("Error saving rating.\n"); return; }

    int rating;
    do {
        printf("Rate government response (1–5): ");
        scanf("%d", &rating);
    } while (rating < 1 || rating > 5);

    fprintf(file, "%d\n", rating);
    fclose(file);
    printf("Thank you for your feedback!\n\n");
}
float computeAverageSatisfaction() {
 FILE *file = fopen("ratings.txt", "r");
    if (!file) {
        printf("No ratings yet.\n");
        return 0;
    }

    int rating, count = 0, total = 0;
    while (fscanf(file, "%d", &rating) == 1) {
        total += rating;
        count++;
    }
    fclose(file);

    if (count == 0) return 0;
    return (float)total / count;
}   
// View all appeals
void viewBulletinBoard() {
    FILE *file = fopen("appeals.txt", "r");
    if (!file) { printf("No appeals found.\n"); return; }

    char line[1024];
    printf("\n======= BULLETIN BOARD =======\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("==============================\n\n");
    fclose(file);
}

// Government page
void governmentPage() {
    int choice;

    while (1) {
        printf("==== GOVERNMENT PAGE ====\n");
        printf("1. View Bulletin Board\n");
        printf("2. Respond to an Appeal\n");
        printf("3. Logout\n");
        printf("4. View Ratings");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewBulletinBoard();
                break;
            case 2:
                governmentRespondToAppeal();
                break;
            case 3:
                printf("Logging out...\n\n");
                return;
            case 4:
                printf("Average Satisfaction Rating: %.2f / 5\n\n", computeAverageSatisfaction());
             break;
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
