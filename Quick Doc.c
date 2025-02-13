#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Patient
{
    char username[100];
    char mobile[12];
    char password[15];
};

typedef struct
{
    char name[50];
    char degree[50];
    char specialist[50];
    char working[100];
    char chamber[50];
    char location[100];
    char time[50];
    char contact[11];
} DoctorDetails;

DoctorDetails doctors[200];
int numDoctors = 0;

const char categories[10][50] =
{
    "General Physician", "Pediatrician", "Cardiologist", "Orthopedic Surgeon", "Dermatologist",
    "Gynecologist", "Neurologist", "Psychiatrist", "Ophthalmologist", "Dentist"
};

char areas[10][100] =
{
    "MEHEDIBUG", "OXYGEN", "PANCHLAISH", "PAHARTALI", "AGRABAD",
    "ANDARKILLA", "KHULSHI", "GEC Mor", "MEDICAL CIRCLE", "MURADPUR"
};


void createAccount(struct Patient *p);
void login(struct Patient *p);
void selectArea(struct Patient *p);
void selectCategory(int areaIndex, struct Patient *p);
void displayDoctors(int areaIndex, int categoryIndex, struct Patient *p);
void displayDoctorDetails(int index, struct Patient *p);
void takeAppointment(int doctorIndex, struct Patient *p);
void loadDoctorDetails();
void showAppointmentHistory(struct Patient *p);
void welcome();


int main()
{
    int choice;
    struct Patient p = {0};

    while (1)
    {
        system("cls");
        welcome();
        printf("1. Create account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');  // To Clear the buffer/wrong input.
            sleep(2);
            continue;
        }

        switch (choice)
        {
        case 1:
            createAccount(&p);
            break;
        case 2:
            login(&p);
            while (1)
            {
                int loggedInChoice;
                while (1)
                {
                    system("cls");
                    printf("1. Take An Appoinrment\n");
                    printf("2. My Appointment History\n");
                    printf("3. Logout\n");
                    printf("Enter your choice: ");
                    if (scanf("%d", &loggedInChoice) != 1)
                    {
                        printf("Invalid input. Please enter a number.\n");
                        while (getchar() != '\n');  // To Clear the buffer/wrong input.
                        sleep(2);
                        continue;
                    }

                    switch (loggedInChoice)
                    {
                    case 1:
                        loadDoctorDetails();
                        selectArea(&p);
                        break;
                    case 2:
                        showAppointmentHistory(&p);
                        break;
                    case 3:
                        strcpy(p.username, "");  // Clear the username to log out
                        break;
                    default:
                        printf("Invalid choice!\n");
                        sleep(2);
                        break;
                    }

                    if (loggedInChoice == 3)
                        break;
                }
            }
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice!\n");
            sleep(2);
            break;
        }
    }

    return 0;
}

void welcome()
{

    printf("\n\n\n\n\n");
    printf("\n\t\t\t        **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**");
    printf("\n\t\t\t        ||==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==||");
    printf("\n\t\t\t        ||                 WELCOME                   ||");
    printf("\n\t\t\t        ||              To Our Project               ||");
    printf("\n\t\t\t        ||                QUICK DOC                  ||");
    printf("\n\t\t\t        ||         Take Your Own Appointment         ||");
    printf("\n\t\t\t        ||==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==||");
    printf("\n\t\t\t        **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**");
    printf("\n\n\t\t\t               C241080 HASAN||C241042 ALVIR");

    printf("\n\n\n\t\t\t                 Enter Any Key To Continue\n");
    getch();
    system("cls");


}

void createAccount(struct Patient *p)
{
    system("cls");
    printf("Create a new account:\n");
    printf("Enter username: ");
    scanf("%s", p->username);
    printf("Enter mobile: ");
    scanf("%s", p->mobile);
    printf("Enter password: ");
    scanf("%s", p->password);

    // Check if username already exists
    FILE *file = fopen("patient.txt", "r+");
    if (file == NULL)
    {
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char storedUsername[100], storedMobile[11], storedPassword[15];
        sscanf(line, "%99[^,],%14[^,],%99[^\n]", storedUsername, storedMobile, storedPassword);

        // Check if username already exists
        if (strcmp(storedUsername, p->username) == 0)
        {
            fclose(file);
            printf("Username '%s' already exists. Please choose a different username.\n", p->username);
            sleep(2);
            return;
        }
    }

    // If username is unique, append to file
    fprintf(file, "%s,%s,%s\n", p->username, p->mobile, p->password);
    fclose(file);
    printf("Account created successfully.\n");
    sleep(2);
}


 void login(struct Patient *p) {
    system("cls");

    char username[100], password[15];
    int retry = 1;  // Flag to control retry loop

    while (retry) {
        printf("Log into your account:\n");
        printf("Enter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        // Check if username and password match
        FILE *file = fopen("patient.txt", "r");
        if (file == NULL) {
            printf("Error opening file.\n");
            return;
        }

        char line[256];
        int found = 0;
        while (fgets(line, sizeof(line), file)) {
            char storedUsername[100], storedMobile[11], storedPassword[15];
            sscanf(line, "%99[^,],%10[^,],%14[^\n]", storedUsername, storedMobile, storedPassword); //String Scan


            if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, password) == 0) {
                printf("Login successful!\n");
                strcpy(p->username, username);  // Copy username to patient structure
                found = 1;
                retry = 0;
                sleep(2);
                break;
            } else if (strcmp(storedUsername, username) == 0) {
                // Username exists but password does not match
                found = 2;
                break;
            }
        }

        if (found == 0) {
            printf("Incorrect username or password! Please try again.\n");
            sleep(2);
        } else if (found == 2) {
            printf("Incorrect password for username '%s'! Please try again.\n", username);
            sleep(2);
        }

        fclose(file);
    }
}


void selectArea(struct Patient *p)
{
    int choice;

    while (1)
    {
        system("cls");
        printf("Select an area:\n");
        for (int i = 0; i < 10; i++)
        {
            printf("%d. %s\n", i + 1, areas[i]);
        }
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear the wrong input
            sleep(2);
            continue;
        }

        if (choice >= 1 && choice <= 10)
        {
            selectCategory(choice - 1, p);
            break;
        }
        else
        {
            printf("Invalid choice.\n");
            sleep(2);
        }
    }
}

void selectCategory(int areaIndex, struct Patient *p)
{
    int choice;

    while (1)
    {
        system("cls");
        printf("Select a doctor category:\n");
        for (int i = 0; i < 10; i++)
        {
            printf("%d. %s\n", i + 1, categories[i]);
        }
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear the wrong input
            sleep(2);
            continue;
        }

        if (choice >= 1 && choice <= 10)
        {
            displayDoctors(areaIndex, choice - 1, p);

            break;
        }
        else
        {
            printf("Invalid choice.\n");
            sleep(2);
        }
    }
}

void displayDoctors(int areaIndex, int categoryIndex, struct Patient *p)
{
    system("cls");
    printf("List of doctors:\n");
    int found = 0;
    for (int i = 0; i < numDoctors; i++)
    {
        if (strcmp(doctors[i].specialist, categories[categoryIndex]) == 0 &&
                strcmp(doctors[i].location, areas[areaIndex]) == 0)
        {
            printf("%d. %s\n", i + 1, doctors[i].name);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No doctors found in this category and area.\n");
        sleep(2);
        return;
    }

    int choice;
    printf("Enter the number of the doctor you want to select: ");
    if (scanf("%d", &choice) != 1)
    {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n'); // Clear the wrong input
        sleep(2);
        return;
    }

    if (choice >= 1 && choice <= numDoctors)
    {
        displayDoctorDetails(choice - 1, p);
    }
    else
    {
        printf("Invalid choice.\n");
        sleep(2);
    }
}

void displayDoctorDetails(int index, struct Patient *p)
{
    system("cls");
    printf("Doctor Details:\n");
    printf("Name: %s\n", doctors[index].name);
    printf("Degree: %s\n", doctors[index].degree);
    printf("Specialist: %s\n", doctors[index].specialist);
    printf("Working at: %s\n", doctors[index].working);
    printf("Chamber: %s\n", doctors[index].chamber);
    printf("Location: %s\n", doctors[index].location);
    printf("Time: %s\n", doctors[index].time);
    printf("Contact: %s\n", doctors[index].contact);

    printf("\nDo you want to take an appointment with this doctor?\n");
    printf("1. Yes\n");
    printf("2. no\n");
    printf("Choose One : ");
    int choice;
    if (scanf("%d", &choice) != 1)
    {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n'); // Clear the wrong input
        sleep(2);
        return;
    }


    switch (choice)
        {
        case 1:
            takeAppointment(index, p);
        case 2:
             selectArea(&p);
        }


}

void takeAppointment(int doctorIndex, struct Patient *p)
{
    FILE *file = fopen("appointments.txt", "a"); //a is w and create mode
    if (file == NULL)
    {
        return;
    }

    fprintf(file, "Appointment with: %s\n", doctors[doctorIndex].name);
    fprintf(file, "Doctor Details:\n");
    fprintf(file, "Name: %s\n", doctors[doctorIndex].name);
    fprintf(file, "Degree: %s\n", doctors[doctorIndex].degree);
    fprintf(file, "Specialist: %s\n", doctors[doctorIndex].specialist);
    fprintf(file, "Working at: %s\n", doctors[doctorIndex].working);
    fprintf(file, "Chamber: %s\n", doctors[doctorIndex].chamber);
    fprintf(file, "Location: %s\n", doctors[doctorIndex].location);
    fprintf(file, "Time: %s\n", doctors[doctorIndex].time);
    fprintf(file, "Contact: %s\n", doctors[doctorIndex].contact);
    fprintf(file, "Patient Username: %s\n", p->username);
    fprintf(file, "------------------------------------\n");

    fclose(file);

    printf("Appointment booked successfully.\n");
    sleep(2);
    exit(0);
}


void showAppointmentHistory(struct Patient *p)
{
    system("cls");
    FILE *file = fopen("appointments.txt", "r");
    if (file == NULL)
    {
        printf("No appointment history found.\n");
        sleep(2);
        return;
    }

    char line[256];
    int found = 0;
    printf("Appointment History for %s:\n", p->username);
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, p->username) != NULL)
        {
            found = 1;
            printf("%s", line);
            for (int i = 0; i < 8; i++)
            {
                if (fgets(line, sizeof(line), file))
                {
                    printf("%s", line);
                }
            }
        }
    }

    if (!found)
    {
        printf("No appointment history found for %s.\n", p->username);
    }

    fclose(file);
    printf("\nPress any key to return to the main menu.\n");
    getch();
}


void loadDoctorDetails()
{
    FILE *file = fopen("doctors.txt", "r");
    if (file == NULL)
    {
        return;
    }

    while (fscanf(file, "%49[^,],%49[^,],%49[^,],%99[^,],%49[^,],%99[^,],%49[^,],%19[^\n]\n",
                  doctors[numDoctors].name, doctors[numDoctors].degree, doctors[numDoctors].specialist,
                  doctors[numDoctors].working, doctors[numDoctors].chamber, doctors[numDoctors].location,
                  doctors[numDoctors].time, doctors[numDoctors].contact) != EOF)
    {
        numDoctors++;
    }

    fclose(file);
}

