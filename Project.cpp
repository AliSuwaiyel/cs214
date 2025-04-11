#include <iostream>
using namespace std;

const int MAX = 10;

class Patient {
private:
    string name;
    int id;
public:
    Patient() {
        name = "Unknown";
        id = 0;
    }

    Patient(string name, int id) {
        this->name = name;
        this->id = id;
    }

    void new_patient(string name, int id) {
        this->name = name;
        this->id = id;
    }

    string get_name() {
        return name;
    }

    int get_id() {
        return id;
    }
};

class Stack {
private:
    struct node {
        Patient patient;
        node* head;
    } *top;
    int count;

public:
    Stack() {
        top = NULL;
        count = 0;
    }

    void push(Patient item) {
        if (count == MAX) {
            cout << "Stack is full" << endl;
            return;
        }

        node* temp = new node;
        if (temp == NULL) {
            cout << "Memory allocation failed" << endl;
            return;
        }

        temp->patient = item;
        temp->head = top;
        top = temp;
        count++;
    }

    Patient pop() {
        if (top == NULL) {
            cout << "Stack is empty" << endl;
            return Patient();
        }

        node* temp = top;
        Patient item = temp->patient;
        top = top->head;
        delete temp;
        count--;

        return item;
    }

    string get_name() {
        if (top == NULL) {
            cout << "Stack is empty" << endl;
            return "";
        }
        return top->patient.get_name();
    }

    Patient get_with_id(int id) {
        node* temp = top;
        while (temp != NULL) {
            if (temp->patient.get_id() == id) {
                return temp->patient;
            }
            temp = temp->head;
        }

        cout << "Patient not found" << endl;
        return Patient();
    }

    Patient remove_by_id(int id, bool& found) {
        node* current = top;
        node* prev = NULL;
        found = false;

        while (current != NULL) {
            if (current->patient.get_id() == id) {
                found = true;
                if (prev == NULL) {
                    top = current->head;
                }
                else {
                    prev->head = current->head;
                }

                Patient p = current->patient;
                delete current;
                count--;
                return p;
            }

            prev = current;
            current = current->head;
        }

        return Patient();
    }
};

void new_patient(Stack& stack) {
    string name;
    int id;
    cout << "Enter patient name: ";
    cin >> name;
    cout << "Enter patient ID: ";
    cin >> id;
    Patient p(name, id);
    stack.push(p);
}

void move_patient(Stack& stack, Patient normal_patients[], int& count) {
    if (count >= MAX) {
        cout << "No space to move patient out of emergency." << endl;
        return;
    }

    int id;
    cout << "Enter the ID of the patient to move: ";
    cin >> id;

    bool found = false;
    Patient p = stack.remove_by_id(id, found);
    if (found) {
        normal_patients[count] = p;
        count++;
        cout << "Patient moved out of emergency room: " << p.get_name() << endl;
    }
    else {
        cout << "Patient with ID " << id << " not found in emergency room." << endl;
    }
}

void get_patient_info(Stack& stack) {
    int id;
    cout << "Enter patient ID: ";
    cin >> id;
    Patient p = stack.get_with_id(id);
    cout << "Patient name: " << p.get_name() << endl;
    cout << "Patient ID: " << p.get_id() << endl;
}

int main() {
    Stack stack;
    Patient normal_patients[MAX];
    int count = 0;

    cout << "Welcome to the Emergency Room" << endl;

    while (true) {
        cout << "\nPlease select an option:" << endl;
        cout << "1: New Patient" << endl;
        cout << "2: Move Patient out of emergency room (by ID)" << endl;
        cout << "3: Get Patient Information" << endl;
        cout << "4: Exit" << endl;

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            new_patient(stack);
            break;
        case 2:
            move_patient(stack, normal_patients, count);
            break;
        case 3:
            get_patient_info(stack);
            break;
        case 4:
            cout << "Exiting program." << endl;
            return 0;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }
}
