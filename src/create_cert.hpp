#ifndef CREATE_CERT_HPP
#define CREATE_CERT_HPP
#include <string>

class CertCreator {
private:
    static std::string ca, ca_key, csr, key, passphrase, root;

    explicit CertCreator(std::string pass);

public:
    static CertCreator &getInstance(std::string pass = "pass");

    CertCreator(const CertCreator &obj) = delete;

    void create_cert(std::string IPAddress, int port, const std::string &root);

private:
    static void handleCACertificate(std::string ca = "../config/ca.pem");

    static void handleCAKey(std::string ca_key = "../config/ca-key.pem");

    static void handleCSR(std::string csr = "../config/cert.csr");

    static void handleKey(std::string key = "../config/cert-key.pem");

    static void addEXTFile(std::string &IPAdress);
};

#endif // CREATE_CERT_HPP
