#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <google/protobuf/struct.pb.h>
#include <google/protobuf/util/json_util.h>

#include "tls/v3/private_key_provider.pb.h"

using namespace google::protobuf;
using namespace std;


void readStruct(string const &fpath) {
    ostringstream oss;
    fstream input(fpath, ios::in);
    oss << input.rdbuf();

    tls::v3::PrivateKeyProvider provider;

    // See https://protobuf.dev/reference/cpp/api-docs/google.protobuf.util.json_util/
    util::Status ret = util::JsonStringToMessage(oss.str(), &provider);
    cout << "JsonStringToMessage return value: " << ret << endl;

    const ::google::protobuf::Any & any = provider.typed_config();

    ::google::protobuf::Struct tpm_provider_config;
    any.UnpackTo(&tpm_provider_config);

    std::string idkey_file;
    std::string idkey_auth_type;
    std::string idkey_auth;
    std::string srk_auth_type;
    std::string srk_auth;
    for (auto it: tpm_provider_config.fields()) {
        string const & name = it.first;
        ::google::protobuf::Value const & value = it.second;

        // NOTE(ywen):
        // 1). `has_xxx_value()` is a private method of class `Value` so I
        //  can't call it.
        // 2). Protobuf uses camel case naming convention but they don't seem
        //  to offer a function to convert snake case to camel case.
        if ("idkeyFile" == name) {
            idkey_file = value.string_value();
        } else if ("idkeyAuthType" == name) {
            idkey_auth_type = value.string_value();
        } else if ("idkeyAuth" == name) {
            idkey_auth = value.string_value();
        } else if ("srkAuthType" == name) {
            srk_auth_type = value.string_value();
        } else if ("srkAuth" == name) {
            srk_auth = value.string_value();
        } else {
            throw std::runtime_error("unexpected field: " + name);
        }
    }

    cout
        << "provider_name: " << provider.provider_name() << endl
        << "typed_config: " << endl
        << "  idkey_file     : " << idkey_file << endl
        << "  idkey_auth_type: " << idkey_auth_type << endl
        << "  idkey_auth     : " << idkey_auth << endl
        << "  srk_auth_type  : " << srk_auth_type << endl
        << "  srk_auth       : " << srk_auth << endl
        ;
}

string usage(string const &prog_name) {
    ostringstream oss;

    oss << "Usage: " << prog_name << " <FPATH>"
        << endl;

    return oss.str();
}

int main(int argc, char * argv[]) {
    string prog_name(argv[0]);

    if (argc < 2) {
        cerr << "Not enough arguments" << endl;
        cout << usage(prog_name) << endl;
        return 1;
    }

    if (argc > 2) {
        cerr << "Too many arguments" << endl;
        cout << usage(prog_name) << endl;
        return 1;
    }

    string fpath(argv[1]);

    readStruct(fpath);

    return 0;
}
