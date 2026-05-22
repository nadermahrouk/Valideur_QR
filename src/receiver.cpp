#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

int main()
{
    // Lire le ticket sécurisé
    ifstream file("../data/secured_ticket.txt");

    if(!file.is_open())
    {
        cerr << "Impossible d'ouvrir secured_ticket.txt\n";
        return 1;
    }

    // Lire première ligne = ticket
    string ticketLine;
    getline(file, ticketLine);

    // Lire deuxième ligne = signature
    string signatureLine;
    getline(file, signatureLine);

    file.close();

    // Vérifier format
    string prefix = "SIGNATURE=";

    if(signatureLine.find(prefix) != 0)
    {
        cerr << "Format SIGNATURE invalide\n";
        return 1;
    }

    // Extraire Base64
    string signatureBase64 =
        signatureLine.substr(prefix.size());

    cout << "\nSignature extraite :\n";
    cout << signatureBase64 << endl;

    // Sauvegarder ticket seul
    ofstream ticketFile("../data/received_ticket.txt");

    ticketFile << ticketLine;

    ticketFile.close();

    // Sauvegarder Base64
    ofstream sig64("../data/received_signature.b64");

    sig64 << signatureBase64;

    sig64.close();

    // Décoder Base64 -> binaire
    int decodeResult = system(
        "openssl enc -base64 -d "
        "-in ../data/received_signature.b64 "
        "-out ../data/received_signature.bin");

    if(decodeResult != 0)
    {
        cerr << "\nErreur decode Base64\n";
        return 1;
    }

    // Vérification ECC
    int verifyResult = system(
        "openssl dgst -sha256 "
        "-verify ../keys/public.pem "
        "-signature ../data/received_signature.bin "
        "../data/received_ticket.txt");

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