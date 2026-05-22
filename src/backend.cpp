// ========================= backend.cpp =========================

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Ticket
{
    int data;
    time_t timestamp;
    bool isValid;
};

int main()
{
    Ticket ticket;

    ticket.data = 12345;
    ticket.timestamp = time(nullptr);
    ticket.isValid = true;

    // Serialization
    string serialized =
        "ID=" + to_string(ticket.data) +
        ";TIMESTAMP=" + to_string(ticket.timestamp) +
        ";VALID=" + to_string(ticket.isValid);

    cout << "Serialized ticket:\n";
    cout << serialized << endl;

    // Save ticket
    ofstream ticketFile("../data/ticket.txt");

    ticketFile << serialized << endl;

    ticketFile.close();

    // ECC signature
    int signResult = system(
        "openssl dgst -sha256 "
        "-sign ../keys/private.pem "
        "-out ../data/signature.bin "
        "../data/ticket.txt");

    if(signResult != 0)
    {
        cerr << "ECC signature failed.\n";
        return 1;
    }

    cout << "\nECC Signature generated successfully.\n";

    // Convert signature to Base64 ONE LINE
    int base64Result = system(
        "openssl base64 -A "
        "-in ../data/signature.bin "
        "-out ../data/signature.b64");

    if(base64Result != 0)
    {
        cerr << "Base64 conversion failed.\n";
        return 1;
    }

    // Read Base64 signature
    ifstream sigFile("../data/signature.b64");

    string signature;

    getline(sigFile, signature);

    sigFile.close();

    // Final secured ticket
    ofstream finalTicket("../data/secured_ticket.txt");

    finalTicket << serialized << endl;
    finalTicket << "SIGNATURE=" << signature << endl;

    finalTicket.close();

    cout << "\nSecured ticket generated.\n";

    return 0;
}