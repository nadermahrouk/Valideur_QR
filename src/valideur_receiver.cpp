// ========================= valideur_receiver.cpp =========================

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

int main()
{
    // Open secured ticket
    ifstream file("../data/secured_ticket.txt");

    if(!file.is_open())
    {
        cerr << "Impossible d'ouvrir secured_ticket.txt\n";
        return 1;
    }

    // Read serialized ticket
    string ticketLine;
    getline(file, ticketLine);

    // Read signature line
    string signatureLine;
    getline(file, signatureLine);

    file.close();

    // Check signature format
    string prefix = "SIGNATURE=";

    if(signatureLine.find(prefix) != 0)
    {
        cerr << "Format SIGNATURE invalide\n";
        return 1;
    }

    // Extract Base64 signature
    string signatureBase64 =
        signatureLine.substr(prefix.size());

    cout << "\nSignature extraite :\n";
    cout << signatureBase64 << endl;

    // Save received ticket
    ofstream ticketFile("../data/received_ticket.txt");

    ticketFile << ticketLine << endl;

    ticketFile.flush();
    ticketFile.close();

    // Save Base64 signature
    ofstream sig64("../data/received_signature.b64");

    sig64 << signatureBase64;

    sig64.flush();
    sig64.close();

    // Decode Base64 -> binary
    int decodeResult = system(
        "openssl enc -base64 -d -A "
        "-in ../data/received_signature.b64 "
        "-out ../data/received_signature.bin");

    cout << "\nDecode result = "
         << decodeResult << endl;

    if(decodeResult != 0)
    {
        cerr << "\nErreur decode Base64\n";
        return 1;
    }

    // ECC verification
    int verifyResult = system(
        "openssl dgst -sha256 "
        "-verify ../keys/public.pem "
        "-signature ../data/received_signature.bin "
        "../data/received_ticket.txt");

    cout << "\nVerify result = "
         << verifyResult << endl;

    if(verifyResult == 0)
    {
        cout << "\nTICKET VALIDE\n";
    }
    else
    {
        cout << "\nFRAUDE DETECTEE\n";
    }

    return 0;
}