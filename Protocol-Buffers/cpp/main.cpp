#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <google/protobuf/util/json_util.h>

#include "tls/v3/private_key_provider.pb.h"
#include "tpm_private_key_provider_config.pb.h"

using namespace google::protobuf;
using namespace std;


void writeFile(string const &fpath) {
    ywen::TPMPrivateKeyProviderConfig tpm_provider_config;
    tpm_provider_config.set_idkey_file("idkey_file");
    tpm_provider_config.set_idkey_auth_type("idkey_auth_type");
    tpm_provider_config.set_idkey_auth("idkey_auth");
    tpm_provider_config.set_srk_auth_type("srk_auth_type");
    tpm_provider_config.set_srk_auth("srk_auth");

    tls::v3::PrivateKeyProvider provider;

    provider.set_provider_name("tpm_provider_ywen");

    // For type of `Any`, the generated code doesn't provide the corresponding
    // `set_xxx()` method. The trick is to use `mutable_xxx()` method to return
    // the pointer and manipulate the pointer.
    // See https://protobuf.dev/programming-guides/proto3/#oneof-features
    ::google::protobuf::Any * p_any = provider.mutable_typed_config();

    // See https://protobuf.dev/reference/cpp/cpp-generated/#any
    p_any->PackFrom(tpm_provider_config);

    std::string s;
    // See https://protobuf.dev/reference/cpp/api-docs/google.protobuf.util.json_util/
    util::MessageToJsonString(provider, &s);

    // Write the new address book back to disk.
    fstream output(fpath, ios::out | ios::trunc);
    output << s;
}

void readFile(string const &fpath) {
    ostringstream oss;
    fstream input(fpath, ios::in);
    oss << input.rdbuf();

    tls::v3::PrivateKeyProvider provider;

    // See https://protobuf.dev/reference/cpp/api-docs/google.protobuf.util.json_util/
    util::JsonStringToMessage(oss.str(), &provider);

    const ::google::protobuf::Any & any = provider.typed_config();

    ywen::TPMPrivateKeyProviderConfig tpm_provider_config;
    any.UnpackTo(&tpm_provider_config);

    cout
        << "provider_name: " << provider.provider_name() << endl
        << "typed_config: " << endl
        << "  idkey_file     : " << tpm_provider_config.idkey_file() << endl
        << "  idkey_auth_type: " << tpm_provider_config.idkey_auth_type() << endl
        << "  idkey_auth     : " << tpm_provider_config.idkey_auth() << endl
        << "  srk_auth_type  : " << tpm_provider_config.srk_auth_type() << endl
        << "  srk_auth       : " << tpm_provider_config.srk_auth() << endl
        ;
}

string usage(string const &prog_name) {
    ostringstream oss;

    oss << "Usage: " << prog_name << " <read/write> <FPATH>"
        << endl;

    return oss.str();
}

int main(int argc, char * argv[]) {
    string prog_name(argv[0]);

    if (argc < 3) {
        cerr << "Not enough arguments" << endl;
        cout << usage(prog_name) << endl;
        return 1;
    }

    if (argc > 3) {
        cerr << "Too many arguments" << endl;
        cout << usage(prog_name) << endl;
        return 1;
    }

    string op(argv[1]);
    string fpath(argv[2]);

    if (op == "read") {
        readFile(fpath);
    } else if (op == "write") {
        writeFile(fpath);
    } else {
        cerr << "Unrecognized operation: " << op << endl;
    }

    return 0;
}
