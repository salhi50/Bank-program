#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct Client {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	int Balance = 0;
};

enum Action {
	ShowClients = 0,
	AddClient = 1,
	DeleteClient = 2,
	DeleteAllClients = 3,
	UpdateClient = 4,
	FindClient = 5,
	SaveClientsToFile = 6,
	Exit = 7
};

enum Alert {
	Error = 0,
	Success = 1,
	Info = 2
};

void divider(int length = 20, string pattern = "-") {
	int i;
	for (i = 0; i < length; i++) {
		cout << pattern;
	}
	cout << endl;
}

void clearScreen() {
	system("cls");
}

int readNumber(string message) {
	int n;
	cout << message;
	cin >> n;
	cin.ignore();
	return n;
}

char readChar(string message) {
	char c;
	cout << message;
	cin >> c;
	cin.ignore();
	return c;
}

string readText(string message) {
	string line;
	cout << message;
	getline(cin, line);
	return line;
}

void alert(Alert type, string message) {
	cout << "[";
	switch (type) {
    case Alert::Success:
      cout << "Success";
      break;
    case Alert::Error:
      cout << "Error";
      break;
    case Alert::Info:
      cout << "Info";
      break;
	}
	cout << "] " << message << endl;
}

bool confirm(string message) {
	char confirmed;
	cout << "\n";
	confirmed = readChar(message + " (y/n)? ");
	return confirmed == 'y' || confirmed == 'Y';
}

bool isValidAction(Action action) {
	return (int)action >= 0 && (int)action <= 7;
}

void printScreenHeading(string heading) {
	divider(30);
	cout << setw(15) << heading << endl;
	divider(30);
	cout << endl;
}

void printClientCard(Client& client) {
	cout << "\nClient details" << endl;
	divider(30, "=");
	cout << "Account number: " << client.AccountNumber << endl;
	cout << "Pin code: " << client.PinCode << endl;
	cout << "Name: " << client.Name << endl;
	cout << "Phone: " << client.Phone << endl;
	cout << "Balance: " << client.Balance << endl;
	divider(30, "=");
}

void printClientsTable(vector <Client>& clients) {
	const int numCols = 6;
	const int colWidths[numCols] = {4, 16, 16, 16, 16, 16};
	int totalWidth = 0;
	int i;
	for (i = 0; i < numCols; i++) {
		totalWidth += colWidths[i];
	}
	cout << "\n" << setw(totalWidth / 2) << "Clients list (" << clients.size() << ")" << endl;
	divider(totalWidth, "_");
	cout << "\n";
	cout << setw(colWidths[0]) << "#";
	cout << setw(colWidths[1]) << "Account Number";
	cout << setw(colWidths[2]) << "Pin code";
	cout << setw(colWidths[3]) << "Name";
	cout << setw(colWidths[4]) << "Phone";
	cout << setw(colWidths[1]) << "Balance" << endl;
	divider(totalWidth, "_");
	for (i = 0; i < clients.size(); i++) {
		cout << "\n";
		cout << setw(colWidths[0]) << i + 1;
		cout << setw(colWidths[1]) << clients[i].AccountNumber;
		cout << setw(colWidths[2]) << clients[i].PinCode;
		cout << setw(colWidths[3]) << clients[i].Name;
		cout << setw(colWidths[4]) << clients[i].Phone;
		cout << setw(colWidths[1]) << clients[i].Balance << endl;
		divider(totalWidth, "_");
	}
}

string clientToRecord(Client& client, string delim = "#//#") {
	string record = "";
	record += client.AccountNumber + delim;
	record += client.PinCode + delim;
	record += client.Name + delim;
	record += client.Phone + delim;
	record += to_string(client.Balance);
	return record;
}

int findClientIndex(string accountNumber, vector <Client> &clients) {
	int i;
	for (i = 0; i < clients.size(); i++) {
		if (clients[i].AccountNumber == accountNumber) {
			return i;
		}
	}
	return -1;
}

void deleteClient(int clientIndex, vector <Client>& clients) {
	int i;
	for (i = clientIndex; i < clients.size() - 1; i++) {
		clients[i] = clients[i + 1];
	}
	clients.pop_back();
}

void updateClient(int clientIndex, vector <Client>& clients) {
	clients[clientIndex].PinCode = readText("New Pin code: ");
	clients[clientIndex].Name = readText("New Name: ");
	clients[clientIndex].Phone = readText("New Phone: ");
	clients[clientIndex].Balance = readNumber("New Balance: ");
}

void showClientsScreen(vector <Client>& clients) {
	printClientsTable(clients);
}

void addClientScreen(vector <Client>& clients) {
	Client client;
	int index;
	printScreenHeading("Add client");
	client.AccountNumber = readText("Account number: ");
	index = findClientIndex(client.AccountNumber, clients);
	if(index == -1 || clients.empty()) {
		client.PinCode = readText("Pin code: ");
		client.Name = readText("Name: ");
		client.Phone = readText("Phone: ");
		client.Balance = readNumber("Balance: ");
		clients.push_back(client);
		alert(Alert::Success, "Added successfully");
	}
	else {
		alert(Alert::Error, "An account with this account number is already exists");
	}
}

void deleteClientScreen(vector <Client> &clients) {
	string accountNumber;
	int index;
	printScreenHeading("Delete client");
	accountNumber = readText("Enter account number: ");
	index = findClientIndex(accountNumber, clients);
	if (index == -1) {
		alert(Alert::Error, "Client not found");
	}
	else {
		printClientCard(clients[index]);
		if (confirm("Are you sure you want to delete this client")) {
			deleteClient(index, clients);
			alert(Alert::Success, "Deleted Successfully");
		}
		else {
			alert(Alert::Info, "Deletion canceled");
		}
	}
}

void deleteAllClientsScreen(vector <Client>& clients) {
	printScreenHeading("Delete all clients");
	if (confirm("Are you sure you want to delete all clients")) {
		clients.clear();
		alert(Alert::Success, "All Clients have been Successfully deleted");
	}
	else {
		alert(Alert::Info, "Deletion canceled");
	}
}

void updateClientScreen(vector <Client>& clients) {
	string accountNumber;
	int index;
	printScreenHeading("Update client");
	accountNumber = readText("Enter account number: ");
	index = findClientIndex(accountNumber, clients);
	if (index == -1) {
		alert(Alert::Error, "Client not found");
	}
	else {
		printClientCard(clients[index]);
		if (confirm("Are you sure you want to update this client")) {
			updateClient(index, clients);
			alert(Alert::Success, "Updated Successfully");
		}
		else {
			alert(Alert::Info, "Update canceled");
		}
	}
}

void findClientScreen(vector <Client>& clients) {
	string accountNumber;
	int index;
	printScreenHeading("Find client");
	accountNumber = readText("Enter account number: ");
	index = findClientIndex(accountNumber, clients);
	if (index == -1) {
		alert(Alert::Error, "Client not found");
	}
	else {
		printClientCard(clients[index]);
	}
}

void saveClientsToFileScreen(vector <Client>& clients) {
	string filepath;
	fstream file;
	printScreenHeading("Save clients to file");
	filepath = readText("File name: ");
	file.open(filepath, ios::out);
	if (file.is_open()) {
		for (Client& client : clients) {
			file << clientToRecord(client) << endl;
		}
		file.close();
		alert(Alert::Success, "Saved to " + filepath);
	}
}

void exitScreen() {
	cout << "\n\t\tProgram ended :)" << endl;
	divider(60, "_");
	cout << "\n";
	cout << "\tWritten by:    Mohammed Ali Essalihi" << endl;
	cout << "\tGithub:        https://github.com/salhi50" << endl;
	divider(60, "_");
}

void mainMenuScreen(vector <Client>& clients);

void performAction(Action action, vector <Client>& clients) {
	clearScreen();
	switch (action) {
	case Action::ShowClients:
		showClientsScreen(clients);
		break;
	case Action::AddClient:
		addClientScreen(clients);
		break;
	case Action::DeleteClient:
		deleteClientScreen(clients);
		break;
	case Action::DeleteAllClients:
		deleteAllClientsScreen(clients);
		break;
	case Action::UpdateClient:
		updateClientScreen(clients);
		break;
	case Action::FindClient:
		findClientScreen(clients);
		break;
	case Action::SaveClientsToFile:
		saveClientsToFileScreen(clients);
		break;
	case Action::Exit:
		exitScreen();
		break;
	}
	if (action != Action::Exit) {
		cout << "\nPress any key to go back to main menu...";
		system("pause>0");
		mainMenuScreen(clients);
	}
	else {
		system("pause>0");
	}
}

void mainMenuScreen(vector <Client> &clients) {
	Action action;
	clearScreen();
	printScreenHeading("Main menu screen");
	cout << "[0] Show clients list\n";
	cout << "[1] Add client\n";
	cout << "[2] Delete client\n";
	cout << "[3] Delete all clients\n";
	cout << "[4] Update client\n";
	cout << "[5] Find client\n";
	cout << "[6] Save clients to file\n";
	cout << "[7] Exit\n";
	divider(30);
	do {
		action = (Action)readNumber("Choose an action [0 to 7]: ");
		if (!isValidAction(action)) {
			alert(Alert::Error, "Out of range");
		}
	} while (!isValidAction(action));
	performAction(action, clients);
}

int main() {
	vector <Client> clients{};
	mainMenuScreen(clients);
	return 0;
}