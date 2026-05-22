#include <iostream>
#include <qrencode.h>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>


using namespace std; 

// serialization c'est la transformation d'un objet en une séquence de bits pour le stocker ou le transmettre, et la désérialisation est le processus inverse, où la séquence de bits est convertie en un objet.

struct Ticket {
    int data; 
    time_t timestamp; 
    bool isValid; 
};


int main(){

    Ticket ticket; 
    ticket.data = 12345;
    ticket.timestamp = time(nullptr);
    ticket.isValid = true;


    // Serialize the ticket
    std::string qrData = "ID=" + std::to_string(ticket.data) + ";TIMESTAMP=" + std::to_string(ticket.timestamp) + ";VALID=" + std::to_string(ticket.isValid) ; 

    cout << "Serialized Ticket Data: " << qrData << endl;

    //save serialisation data 
    ofstream ticketFile("ticket.txt");
    ticketFile << qrData;
    ticketFile.close();

    // ECC Signature using OpenSSL
    int result = system(
        "openssl dgst -sha256 "
        "-sign ../keys/private.pem "
        "-out signature.bin "
        "ticket.txt");


    system("openssl base64 -in signature.bin -out signature.b64");

    // Read Base64 signature
    ifstream sigFile("signature.b64");

    string signature(
        (istreambuf_iterator<char>(sigFile)),
         istreambuf_iterator<char>());

    sigFile.close();

    // Final secured ticket
    ofstream finalTicket("secured_ticket.txt");

    finalTicket << qrData << endl;
    finalTicket << "SIGNATURE=" << signature;

    finalTicket.close();

    cout << "\nSecured ticket generated.\n";

    return 0;
}








    // Generate QR code
    QRcode *qrcode = QRcode_encodeString(qrData.c_str(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);

    if(qrcode == nullptr)
        {
            cerr << "Failed to generate QR code." << endl;
            return 1;
        }
    else {
        cout << "QR code generated successfully." << endl;
    }

    // Save QR code to a file//
    // Save QR code to a file
    ofstream file("ticket_qr.pbm");

    int size = qrcode->width;

    file << "P1\n";

    file << size << " " << size << "\n";

    for(int y = 0; y < size; y++)
        {
            for(int x = 0; x < size; x++)
            {
                unsigned char pixel =
                    qrcode->data[y * size + x];

                file << ((pixel & 1) ? "1 " : "0 ");
            }

            file << "\n";
        }

    file.close();
    QRcode_free(qrcode);
    return 0;






    
    




}