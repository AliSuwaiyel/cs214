#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
using namespace std;



class Patient {
private:
    string name;
    int id;
    int priority;
    string diagnosis;
    string description;

public:
    Patient() : name(""), id(0), priority(0), diagnosis("Not determined"), description("---") {}
    Patient(string name, int id, int priority) : name(name), id(id), priority(priority), diagnosis("Not determined"), description("---") {}

    void set_diagnosis(string diagnosis) { this->diagnosis = diagnosis; }
    void set_description(string description) { this->description = description; }

    string get_name() { return name; }
    int get_id() { return id; }
    int get_priority() { return priority; }
    string get_diagnosis() { return diagnosis; }
    string get_description() { return description; }

    friend ostream& operator<<(ostream& os, const Patient& p) {
        os << "Name: " << p.name
            << ", ID: " << p.id
            << ", Priority: " << p.priority
            << ", Diagnosis: " << p.diagnosis
            << ", Description: " << p.description;
        return os;
    }
};

class patientList {
private:
    int count;
    int MAX;
    Patient heap[10];
    Patient overflowStack[10];
    int overflowTop;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    patientList(int MAX) : count(0), MAX(MAX), overflowTop(-1) {}

    void addToOverflow(Patient p);
    void manageOverFlow();
    void addPatient();
    void removePatient();
	void removePatient(int id);
    void viewPatients();
    void nextPatient();
    void saveInFile(const string& filename);
    void saveAsFormattedReport(const string& file);
    void loadFromFile(const string& filename);
};

void patientList::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].get_priority() < heap[parent].get_priority()) {
            swap(heap[index], heap[parent]);
            index = parent;
        }
        else {
            break;
        }
    }
}

void patientList::heapifyDown(int index) {
    while (2 * index + 1 < count) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (heap[left].get_priority() < heap[smallest].get_priority())
            smallest = left;
        if (right < count && heap[right].get_priority() < heap[smallest].get_priority())
            smallest = right;

        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            index = smallest;
        }
        else {
            break;
        }
    }
}

void patientList::addToOverflow(Patient p) {
    if (overflowTop < 9) {
        overflowTop++;
        overflowStack[overflowTop] = p;
        cout << "Patient added to overflow." << endl;
    }
    else {
        cout << "Overflow stack is full!! Cannot add more patients." << endl;
    }
}

void patientList::manageOverFlow() {
    if (overflowTop == -1) return;

    cout << "Adding a patient from overflow to the ER..." << endl;
    Patient overflowPatient = overflowStack[overflowTop];
    overflowTop--;
    heap[count++] = overflowPatient;
    heapifyUp(count - 1);
    cout << "Patient added from overflow to the ER." << endl;
}

void patientList::addPatient() {
    string name;
    int id, priority;

    cout << "Enter patient name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter ID: ";
    cin >> id;

    while (true) {
        cout << "Enter priority (1 being most urgent and 5 being least): ";
        cin >> priority;
        if (priority >= 1 && priority <= 5) break;
        else cout << "Invalid priority level!! Try again..." << endl;
    }

    Patient p(name, id, priority);

    if (count == MAX) {
        cout << "Emergency Room is Full!! Adding to overflow..." << endl;
        addToOverflow(p);
        return;
    }

    heap[count++] = p;
    heapifyUp(count - 1);
    cout << "Patient added successfully!!!!" << endl;
}

void patientList::removePatient() {
    if (count == 0) {
        cout << "No patients to remove." << endl;
        return;
    }

    int id;
    cout << "Enter ID of patient to remove: ";
    cin >> id;

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (heap[i].get_id() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Patient not found." << endl;
        return;
    }

    count--;
    heap[index] = heap[count];
    heapifyDown(index);
    manageOverFlow();
    cout << "Patient removed." << endl;
}
void patientList::removePatient(int id) {
    if (count == 0) {
        cout << "No patients to remove." << endl;
        return;
    }

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (heap[i].get_id() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Patient not found." << endl;
        return;
    }

    count--;
    heap[index] = heap[count];
    heapifyDown(index);
    manageOverFlow();
    cout << "Patient removed." << endl;
}

void patientList::viewPatients() {
    if (count == 0) {
        cout << "Emergency Room is Empty" << endl;
    }
    else {
        cout << "There are " << count << " patients in the ER:" << endl;
        for (int i = 0; i < count; i++) {
            cout << heap[i] << endl;
        }
    }

    if (overflowTop != -1) {
        cout << "There are " << overflowTop + 1 << " overflow patients:" << endl;
        for (int i = 0; i <= overflowTop; i++) {
            cout << overflowStack[i] << endl;
        }
    }
}

void patientList::nextPatient() {
    if (count == 0) {
        cout << "No patients in the emergency room." << endl;
        return;
    }

    Patient& p = heap[0];
    cout << "Next patient (most urgent):" << endl;
    cout << p << endl;

    int choice;
    cout << "1. Evaluate\n2. Exit to main menu\nChoose: ";
    cin >> choice;

    if (choice == 1) {
        string diagnosis, description;

        string commonDiseases[5] = {
            "Flu",
            "Fracture",
            "Infection",
            "Heart Attack",
            "Headache"
        };

        // Show common diseases first
        cout << "Common Diseases:\n";
        for (int i = 0; i < 5; i++) {
            cout << i + 1 << ". " << commonDiseases[i] << endl;
        }

        char isCommon;
        cout << "\nIs the diagnosis a common disease? (y/n): ";
        cin >> isCommon;
        cin.ignore(); // <<=== FLUSH after char input

        if (isCommon == 'y' || isCommon == 'Y') {
            int diagChoice;
            cout << "Enter the number of the disease (1-5): ";
            cin >> diagChoice;
            cin.ignore(); // <<=== FLUSH after number input

            if (diagChoice >= 1 && diagChoice <= 5) {
                diagnosis = commonDiseases[diagChoice - 1];
            }
            else {
                cout << "Invalid choice. Enter diagnosis manually: ";
                getline(cin, diagnosis);
            }
        }
        else {
            cout << "Enter diagnosis manually: ";
            getline(cin, diagnosis);
        }

        cout << "Enter description: ";
        getline(cin, description);

        p.set_diagnosis(diagnosis);
        p.set_description(description);
        cout << "Patient evaluated." << endl;

		cout << "Do you want to remove this patient from the ER? (y/n): ";
		char removeChoice;
		cin >> removeChoice;
		if (removeChoice == 'y' || removeChoice == 'Y') {
			removePatient(heap[0].get_id());
		}
	}
	else if (choice == 2) {
		cout << "Returning to main menu..." << endl;
	}
	else {
		cout << "Invalid choice. Returning to main menu..." << endl;

    }
}

void patientList::saveInFile(const string& file) {
    ofstream outrFile(file); //write mode
    if (!outrFile) {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    outrFile << count << endl;
    for (int i = 0; i < count; i++) {
        outrFile << heap[i].get_name() << " "
            << heap[i].get_id() << " "
            << heap[i].get_priority() << endl
            << heap[i].get_diagnosis() << endl
            << heap[i].get_description() << endl << endl;
    }

    outrFile << overflowTop + 1 << endl;
    for (int i = 0; i <= overflowTop; i++) {
        outrFile << overflowStack[i].get_name() << " "
            << overflowStack[i].get_id() << " "
            << overflowStack[i].get_priority() << endl
            << overflowStack[i].get_diagnosis() << endl
            << overflowStack[i].get_description() << endl << endl;
    }

    outrFile.close();
    cout << "Data saved to file successfully." << endl;
}

void patientList::saveAsFormattedReport(const string& file) {
    ofstream outFile(file);
    if (!outFile) {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    outFile << "======================= Emergency Room Patients =======================\n";
    outFile << "Total in ER: " << count << "\n";
    outFile << "-----------------------------------------------------------------------\n";
    outFile << "| Name           | ID     | Priority | Diagnosis     | Description     |\n";
    outFile << "-----------------------------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        outFile << "| ";
        outFile.width(14); outFile.setf(ios::left); outFile << heap[i].get_name();
        outFile << "| ";
        outFile.width(7); outFile << heap[i].get_id();
        outFile << "| ";
        outFile.width(9); outFile << heap[i].get_priority();
        outFile << "| ";
        outFile.width(14); outFile << heap[i].get_diagnosis();
        outFile << "| ";
        outFile.width(17); outFile << heap[i].get_description();
        outFile << "|\n";
    }

    outFile << "-----------------------------------------------------------------------\n";

    if (overflowTop >= 0) {
        outFile << "\n========================= Overflow Patients ===========================\n";
        outFile << "Total in Overflow: " << overflowTop + 1 << "\n";
        outFile << "-----------------------------------------------------------------------\n";
        outFile << "| Name           | ID     | Priority | Diagnosis     | Description     |\n";
        outFile << "-----------------------------------------------------------------------\n";

        for (int i = 0; i <= overflowTop; i++) {
            outFile << "| ";
            outFile.width(14); outFile.setf(ios::left); outFile << overflowStack[i].get_name();
            outFile << "| ";
            outFile.width(7); outFile << overflowStack[i].get_id();
            outFile << "| ";
            outFile.width(9); outFile << overflowStack[i].get_priority();
            outFile << "| ";
            outFile.width(14); outFile << overflowStack[i].get_diagnosis();
            outFile << "| ";
            outFile.width(17); outFile << overflowStack[i].get_description();
            outFile << "|\n";
        }

        outFile << "-----------------------------------------------------------------------\n";
    }

    outFile.close();
    cout << "Formatted report saved successfully." << endl;
}


void patientList::loadFromFile(const string& file) {
    ifstream inFile(file);
    if (!inFile) {
        cout << " Sorry no saved data found. " << endl;
        return;
    }

    count = 0;
    overflowTop = -1;

    int heapCount;
    inFile >> heapCount;
    inFile.ignore();

    for (int i = 0; i < heapCount; i++) {
        string name, diagnosis, description;
        int id, priority;

        inFile >> name >> id >> priority;
        inFile.ignore();
        getline(inFile, diagnosis);
        getline(inFile, description);
        Patient p(name, id, priority);
        p.set_diagnosis(diagnosis);
        p.set_description(description);
        heap[count++] = p;
    }

    int overflowCount;
    inFile >> overflowCount;
    inFile.ignore();

    for (int i = 0; i < overflowCount; i++) {
        string name, diagnosis, description;
        int id, priority;
        inFile >> name >> id >> priority;
        inFile.ignore();
        getline(inFile, diagnosis);
        getline(inFile, description);
        Patient p(name, id, priority);
        p.set_diagnosis(diagnosis);
        p.set_description(description);
        overflowStack[++overflowTop] = p;
    }

    inFile.close();
    cout << "Data loaded from file successfully." << endl;
}

int main() {
    patientList er(10);
    int choice;

    er.loadFromFile("patients.txt");

    while (true) {
        system("cls");
        cout << "========= Emergency Room Patient Management System =========" << endl;
        cout << "|-1. Add New Patient" << endl;
        cout << "|-2. Remove Patient by ID" << endl;
        cout << "|-3. View All Patients" << endl;
        cout << "|-4. View Next Patient" << endl;
        cout << "|-5. Export Formatted Report\n";
        cout << "|-6. Exit" << endl;
        cout << "-----Enter an operation to perform: ";
        cin >> choice;

        switch (choice) {
        case 1: er.addPatient(); break;
        case 2: er.removePatient(); break;
        case 3: er.viewPatients(); break;
        case 4: er.nextPatient(); break;
		case 5:
			er.saveAsFormattedReport("formatted_report.txt");
			break;
        case 6:
            er.saveInFile("patients.txt");
            cout << "Exiting program..." << endl;
            return 0;
        default:
            cout << "Invalid choice!!! Try again..." << endl;
        }
        system("pause");
    }
}
