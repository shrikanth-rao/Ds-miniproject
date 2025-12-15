
# Hospital Management System


## 1. Problem Statement

Managing patients in a hospital involves handling registrations, emergency cases, searching records, assigning doctors, and safely removing patients after treatment.
Manual handling of these tasks is time-consuming and error-prone.

This project aims to design and implement a **Hospital Management System using C** that efficiently manages patient data using appropriate **data structures**, ensuring faster access, organized storage, and optimized operations.

---

## 2. Objectives

* To manage patient records efficiently
* To prioritize emergency patients based on severity
* To support fast searching of patient data
* To allow undoing the last operation
* To automatically discharge patients after treatment
* To demonstrate real-world usage of data structures in C

---

## 3. Data Structures Used

| Feature            | Data Structure Used      | Purpose                          |
| ------------------ | ------------------------ | -------------------------------- |
| Patient Records    | Linked List              | Dynamic insertion and deletion   |
| Emergency Patients | Priority Queue (Array)   | Treat patients based on severity |
| Patient Search     | Binary Search Tree (BST) | Fast search by patient ID        |
| Doctor Assignment  | Queue                    | Fair doctor allocation           |
| Undo Feature       | Stack                    | Reverse last operation           |
| File Handling      | Text File                | Persistent data storage          |

---

## 4. System Features

* Add new patients
* Display all patients
* Add emergency patients with severity levels
* Treat emergency patients based on priority
* Search patients by ID
* Undo last patient operation
* Auto-discharge patients after treatment
* Save and load patient data from files

---

## 5. Innovative / Optimized Components

* **Priority-based emergency treatment** (severity 1–5)
* **Undo functionality using stack**
* **Automatic patient discharge after treatment**
* **Fast patient lookup using BST**
* **Persistent data storage using file handling**

---

## 6. Modular Programming

The project is divided into independent modules:

* Patient Management
* Emergency Handling
* Doctor Management
* Search Module
* Undo Module
* File Handling Module

This improves readability, maintainability, and scalability.

---

## 7. Technologies Used

* Programming Language: **C**
* Compiler: GCC / Emscripten
* Concepts:

  * Linked Lists
  * Stacks
  * Queues
  * Trees
  * File Handling
  * Modular Programming

---

## 8. How to Run (Console Version)

```
gcc Main.c -o Main
./Main
```

# 9. Future Enhancements

* Web-based UI using HTML/CSS/JavaScript
* Database integration
* Doctor specialization mapping
* Appointment scheduling system
* Authentication system



# 10. Conclusion

This Hospital Management System demonstrates the effective use of **core data structures in solving real-world problems**.
It provides an optimized, modular, and scalable solution while strictly following C programming principles.


Just tell me 👍
