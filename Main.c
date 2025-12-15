#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// PATIENT LINKED LIST

struct Patient {
    int id;
    char name[30];
    int age;
    struct Patient *next;
};

struct Patient *head = NULL;

//EMERGENCY PRIORITY QUEUE

struct Emergency {
    int id;
    char name[30];
    int severity; // 1 (Low) to 5 (Critical)
};

struct Emergency emergencyQueue[50];
int eqSize = 0;

// BST FOR SEARCH

struct BST {
    int id;
    char name[30];
    struct BST *left, *right;
};

struct BST *root = NULL;

//DOCTOR QUEUE

struct Doctor {
    int id;
    char name[30];
};

#define MAX_DOCTORS 5
struct Doctor doctors[MAX_DOCTORS];
int dFront = 0, dRear = MAX_DOCTORS;

//UNDO STACK

struct Undo {
    int patientId;
};

struct Undo undoStack[50];
int top = -1;

//FUNCTION DECLARATIONS

void addPatient();
void displayPatients();
void savePatientsToFile();
void loadPatientsFromFile();
void autoDischarge(int id);

void addEmergency();
void treatEmergency();

struct BST* insertBST(struct BST *node, int id, char name[]);
void searchPatient(struct BST *node, int id);

void pushUndo(int id);
void undoLast();

void initDoctors();
void assignDoctor();

//ADD PATIENT

void addPatient() {
    struct Patient *newNode = (struct Patient*)malloc(sizeof(struct Patient));
    printf("Enter Patient ID: ");
    scanf("%d", &newNode->id);
    printf("Enter Name: ");
    scanf("%s", newNode->name);
    printf("Enter Age: ");
    scanf("%d", &newNode->age);

    newNode->next = head;
    head = newNode;

    root = insertBST(root, newNode->id, newNode->name);

    pushUndo(newNode->id);  // Add to undo stack

    savePatientsToFile();
    printf("Patient added successfully!\n");
}

//DISPLAY PATIENTS

void displayPatients() {
    struct Patient *temp = head;
    if (!temp) {
        printf("No patients found.\n");
        return;
    }
    printf("\nPatient List:\n");
    while (temp) {
        printf("ID: %d | Name: %s | Age: %d\n",
               temp->id, temp->name, temp->age);
        temp = temp->next;
    }
}

// FILE HANDLING

void savePatientsToFile() {
    FILE *fp = fopen("patients.txt", "w");
    struct Patient *temp = head;
    while (temp) {
        fprintf(fp, "%d %s %d\n",
                temp->id, temp->name, temp->age);
        temp = temp->next;
    }
    fclose(fp);
}

void loadPatientsFromFile() {
    FILE *fp = fopen("patients.txt", "r");
    if (!fp) return;

    struct Patient *temp;
    while (!feof(fp)) {
        temp = (struct Patient*)malloc(sizeof(struct Patient));
        if (fscanf(fp, "%d %s %d",
                   &temp->id, temp->name, &temp->age) != 3)
            break;
        temp->next = head;
        head = temp;
        root = insertBST(root, temp->id, temp->name);
    }
    fclose(fp);
}

//EMERGENCY FUNCTIONS

void addEmergency() {
    if (eqSize >= 50) {
        printf("⚠ Emergency queue full! Cannot add more.\n");
        return;
    }

    struct Emergency e;
    printf("Enter Patient ID: ");
    scanf("%d", &e.id);
    printf("Enter Name: ");
    scanf("%s", e.name);
    printf("Enter Severity (1-5): ");
    scanf("%d", &e.severity);

    int i = eqSize - 1;
    while (i >= 0 && emergencyQueue[i].severity < e.severity) {
        emergencyQueue[i + 1] = emergencyQueue[i];
        i--;
    }
    emergencyQueue[i + 1] = e;
    eqSize++;

    if (eqSize >= 48)  // capacity alert
        printf("⚠ Emergency queue almost full!\n");

    printf("Emergency patient added!\n");
}

void treatEmergency() {
    if (eqSize == 0) {
        printf("No emergency patients.\n");
        return;
    }

    struct Emergency e = emergencyQueue[0];
    printf("Treating Patient ID: %d | Name: %s | Severity: %d\n",
           e.id, e.name, e.severity);

    assignDoctor();
    autoDischarge(e.id);

    for (int i = 1; i < eqSize; i++)
        emergencyQueue[i - 1] = emergencyQueue[i];
    eqSize--;
}

// BST FUNCTIONS

struct BST* insertBST(struct BST *node, int id, char name[]) {
    if (!node) {
        struct BST *temp = (struct BST*)malloc(sizeof(struct BST));
        temp->id = id;
        strcpy(temp->name, name);
        temp->left = temp->right = NULL;
        return temp;
    }
    if (id < node->id)
        node->left = insertBST(node->left, id, name);
    else if (id > node->id)
        node->right = insertBST(node->right, id, name);
    return node;
}

void searchPatient(struct BST *node, int id) {
    if (!node) {
        printf("Patient not found.\n");
        return;
    }
    if (id == node->id) {
        printf("Patient Found: ID: %d | Name: %s\n", node->id, node->name);
        return;
    }
    if (id < node->id)
        searchPatient(node->left, id);
    else
        searchPatient(node->right, id);
}

// UNDO FEATURE

void pushUndo(int id) {
    undoStack[++top].patientId = id;
}

void undoLast() {
    if (top < 0) {
        printf("Nothing to undo.\n");
        return;
    }

    int id = undoStack[top--].patientId;
    autoDischarge(id);
    printf("Undo completed successfully.\n");
}

//AUTO DISCHARGE

void autoDischarge(int id) {
    struct Patient *temp = head, *prev = NULL;
    while (temp) {
        if (temp->id == id) {
            if (prev)
                prev->next = temp->next;
            else
                head = temp->next;
            free(temp);
            printf("Patient ID %d discharged.\n", id);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

//DOCTOR MANAGEMENT

void initDoctors() {
    for (int i = 0; i < MAX_DOCTORS; i++) {
        doctors[i].id = i + 1;
        sprintf(doctors[i].name, "Doctor_%d", i + 1);
    }
}

void assignDoctor() {
    if (dFront == dRear) {
        printf("No doctors available currently.\n");
        return;
    }
    struct Doctor d = doctors[dFront++];
    printf("Assigned %s to patient.\n", d.name);
}

// MAIN MENU

int main() {
    int choice, id;
    loadPatientsFromFile();
    initDoctors();

    while (1) {
        printf("\n--- HOSPITAL MANAGEMENT SYSTEM ---\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Add Emergency Patient\n");
        printf("4. Treat Emergency Patient\n");
        printf("5. Undo Last Operation\n");
        printf("6. Search Patient\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addPatient(); break;
            case 2: displayPatients(); break;
            case 3: addEmergency(); break;
            case 4: treatEmergency(); break;
            case 5: undoLast(); break;
            case 6:
                printf("Enter Patient ID to search: ");
                scanf("%d", &id);
                searchPatient(root, id);
                break;
            case 7: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}
