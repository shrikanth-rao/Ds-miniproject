#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/// =====================
/// PATIENT LINKED LIST
/// =====================
struct Patient {
    int id;
    char name[30];
    int age;
    struct Patient *next;
};
struct Patient *head = NULL;

/// =====================
/// EMERGENCY PRIORITY QUEUE
/// =====================
struct Emergency {
    int id;
    char name[30];
    int severity;
};
struct Emergency emergencyQueue[50];
int eqSize = 0;

/// =====================
/// BST FOR SEARCH
/// =====================
struct BST {
    int id;
    char name[30];
    struct BST *left, *right;
};
struct BST *root = NULL;

/// =====================
/// DOCTOR QUEUE
/// =====================
struct Doctor {
    int id;
    char name[30];
};
#define MAX_DOCTORS 5
struct Doctor doctors[MAX_DOCTORS];
int dFront = 0, dRear = 0; // circular queue

/// =====================
/// UNDO STACK
/// =====================
struct Undo {
    int patientId;
};
struct Undo undoStack[50];
int top = -1;

/// =====================
/// FUNCTION DECLARATIONS
/// =====================
void addPatient();
void displayPatients();
void deletePatient();
void savePatientsToFile();
void loadPatientsFromFile();
void clearPatientsFile();
void autoDischarge(int id);

void addEmergency();
void treatEmergency();

struct BST* insertBST(struct BST *node, int id, char name[]);
struct BST* deleteBST(struct BST *node, int id);
struct BST* findMin(struct BST *node);
void searchPatient(struct BST *node, int id);

void pushUndo(int id);
void undoLast();

void initDoctors();
void assignDoctor();

int isValidName(char name[]);
int isIdExists(int id);
int isValidAge(int age);

void exitProgram();

/// =====================
/// VALIDATIONS
/// =====================
int isValidName(char name[]) {
    for (int i = 0; name[i]; i++)
        if (!isalpha(name[i]))
            return 0;
    return 1;
}

int isIdExists(int id) {
    struct Patient *temp = head;
    while (temp) {
        if (temp->id == id)
            return 1;
        temp = temp->next;
    }
    return 0;
}

int isValidAge(int age) {
    return age > 0 && age <= 120;
}

/// =====================
/// ADD PATIENT
/// =====================
void addPatient() {
    struct Patient *newNode = malloc(sizeof(struct Patient));
    int valid;

    // ID input with uniqueness check
    do {
        printf("Enter Patient ID: ");
        scanf("%d", &newNode->id);
        valid = !isIdExists(newNode->id);
        if (!valid) printf(" ID already exists! Enter unique ID.\n");
    } while (!valid);

    // Name input with validation
    do {
        printf("Enter Name: ");
        scanf("%s", newNode->name);
        valid = isValidName(newNode->name);
        if (!valid) printf(" Invalid name! Alphabets only.\n");
    } while (!valid);

    // Age input with validation
    do {
        printf("Enter Age: ");
        scanf("%d", &newNode->age);
        valid = isValidAge(newNode->age);
        if (!valid) printf(" Invalid age! Enter 1-120.\n");
    } while (!valid);

    newNode->next = head;
    head = newNode;

    root = insertBST(root, newNode->id, newNode->name);
    pushUndo(newNode->id);

    printf(" Patient added successfully!\n");
}

/// =====================
/// DISPLAY PATIENTS
/// =====================
void displayPatients() {
    struct Patient *temp = head;
    if (!temp) {
        printf("No patients found.\n");
        return;
    }
    while (temp) {
        printf("ID: %d | Name: %s | Age: %d\n",
               temp->id, temp->name, temp->age);
        temp = temp->next;
    }
}

/// =====================
/// DELETE PATIENT
/// =====================
void deletePatient() {
    int id;
    struct Patient *temp = head, *prev = NULL;
    if (!head) { printf("Patient list empty.\n"); return; }

    printf("Enter Patient ID to delete: ");
    scanf("%d", &id);

    while (temp) {
        if (temp->id == id) {
            if (prev) prev->next = temp->next;
            else head = temp->next;

            free(temp);
            root = deleteBST(root, id);
            printf(" Patient deleted successfully.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf(" Patient not found.\n");
}

/// =====================
/// FILE HANDLING
/// =====================
void savePatientsToFile() {
    FILE *fp = fopen("patients.txt", "w");
    struct Patient *temp = head;
    while (temp) {
        fprintf(fp, "%d %s %d\n", temp->id, temp->name, temp->age);
        temp = temp->next;
    }
    fclose(fp);
}
void loadPatientsFromFile() {
    FILE *fp = fopen("patients.txt", "r");
    if (!fp) return;
    struct Patient *temp;
    while (1) {
        temp = malloc(sizeof(struct Patient));
        if (fscanf(fp, "%d %s %d", &temp->id, temp->name, &temp->age) != 3) {
            free(temp); break;
        }
        temp->next = head;
        head = temp;
        root = insertBST(root, temp->id, temp->name);
    }
    fclose(fp);
}
void clearPatientsFile() {
    FILE *fp = fopen("patients.txt", "w");
    if (fp) fclose(fp);
}

/// =====================
/// EMERGENCY
/// =====================
void addEmergency() {
    struct Emergency e;
    int valid;

    // ID input
    do {
        printf("Enter Patient ID: ");
        scanf("%d", &e.id);
        valid = !isIdExists(e.id);
        if (!valid) printf(" ID already exists in system!\n");
    } while (!valid);

    // Name input
    do {
        printf("Enter Name: ");
        scanf("%s", e.name);
        valid = isValidName(e.name);
        if (!valid) printf(" Invalid name!\n");
    } while (!valid);

    // Severity input
    do {
        printf("Enter Severity (1-5): ");
        scanf("%d", &e.severity);
        valid = (e.severity >= 1 && e.severity <= 5);
        if (!valid) printf(" Severity must be 1-5!\n");
    } while (!valid);

    // Insert into priority queue
    int i = eqSize - 1;
    while (i >= 0 && emergencyQueue[i].severity < e.severity) {
        emergencyQueue[i + 1] = emergencyQueue[i];
        i--;
    }
    emergencyQueue[i + 1] = e;
    eqSize++;
    printf(" Emergency patient added.\n");
}

void treatEmergency() {
    if (eqSize == 0) { printf("No emergency patients.\n"); return; }
    struct Emergency e = emergencyQueue[0];
    printf("Treating %s (Severity %d)\n", e.name, e.severity);

    assignDoctor();
    autoDischarge(e.id);

    for (int i = 1; i < eqSize; i++)
        emergencyQueue[i - 1] = emergencyQueue[i];
    eqSize--;
}

/// =====================
/// BST FUNCTIONS
/// =====================
struct BST* insertBST(struct BST *node, int id, char name[]) {
    if (!node) {
        struct BST *temp = malloc(sizeof(struct BST));
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
struct BST* findMin(struct BST *node) {
    while (node && node->left) node = node->left;
    return node;
}
struct BST* deleteBST(struct BST *node, int id) {
    if (!node) return NULL;
    if (id < node->id) node->left = deleteBST(node->left, id);
    else if (id > node->id) node->right = deleteBST(node->right, id);
    else {
        if (!node->left) { struct BST *t=node->right; free(node); return t; }
        if (!node->right){ struct BST *t=node->left; free(node); return t; }
        struct BST* t=findMin(node->right);
        node->id=t->id;
        strcpy(node->name, t->name);
        node->right=deleteBST(node->right, t->id);
    }
    return node;
}
void searchPatient(struct BST *node, int id) {
    if (!node) { printf("Patient not found.\n"); return; }
    if (id==node->id){ printf("Patient Found: %s\n", node->name); return; }
    if (id<node->id) searchPatient(node->left,id);
    else searchPatient(node->right,id);
}

/// =====================
/// UNDO
/// =====================
void pushUndo(int id) { if(top<49) undoStack[++top].patientId=id; }
void undoLast() {
    if(top<0){ printf("Nothing to undo.\n"); return; }
    autoDischarge(undoStack[top--].patientId);
}

/// =====================
/// AUTO DISCHARGE
/// =====================
void autoDischarge(int id){
    struct Patient *temp=head,*prev=NULL;
    while(temp){
        if(temp->id==id){
            if(prev) prev->next=temp->next; else head=temp->next;
            free(temp);
            root=deleteBST(root,id);
            return;
        }
        prev=temp; temp=temp->next;
    }
}

/// =====================
/// DOCTOR MANAGEMENT
/// =====================
void initDoctors() {
    for(int i=0;i<MAX_DOCTORS;i++){
        doctors[i].id=i+1;
        sprintf(doctors[i].name,"Doctor_%d",i+1);
    }
    dFront=0; dRear=0;
}
void assignDoctor(){
    if((dRear+1)%MAX_DOCTORS==dFront){ printf("No doctors available.\n"); return; }
    printf("Assigned %s\n",doctors[dRear].name);
    dRear=(dRear+1)%MAX_DOCTORS; // circular queue
}

/// =====================
/// EXIT
/// =====================
void exitProgram(){
    int ch;
    printf("Save patient data before exit?\n1.Yes 2.No\n");
    scanf("%d",&ch);
    if(ch==1) savePatientsToFile();
    else clearPatientsFile();
    exit(0);
}

/// =====================
/// MAIN
/// =====================
int main(){
    int choice,id;
    loadPatientsFromFile();
    initDoctors();

    while(1){
        printf("\n1.Add Patient\n2.Display Patients\n3.Add Emergency\n");
        printf("4.Treat Emergency\n5.Undo\n6.Search\n7.Delete Patient\n8.Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1: addPatient(); break;
            case 2: displayPatients(); break;
            case 3: addEmergency(); break;
            case 4: treatEmergency(); break;
            case 5: undoLast(); break;
            case 6:
                printf("Enter ID: "); scanf("%d",&id);
                searchPatient(root,id);
                break;
            case 7: deletePatient(); break;
            case 8: exitProgram(); break;
            default: printf("Invalid choice!\n");
        }
    }
}
