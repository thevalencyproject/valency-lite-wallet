#include "Interface.h"


std::vector<std::string> Interface::loginSuccessText(std::string publicAddress) {
    std::vector<std::string> output;

    output.push_back("LOGIN SUCCESS:");
    output.push_back(publicAddress);

    return output;
}

std::vector<std::string> Interface::walletBalanceText(double amount) {
    std::vector<std::string> output;

    output.push_back("WALLET BALANCE:");
    output.push_back(std::to_string(amount) + "vlnc");

    return output;
}

std::vector<std::string> Interface::transactionFeeInfoText(std::string currentTime, double fee) {
    std::vector<std::string> output;

    output.push_back("Fee Frozen for 30s from: " + currentTime);
    output.push_back("Transaction Fee: " + std::to_string(fee));
    output.push_back("Accept Transaction?");

    return output;
}

std::vector<std::string> Interface::transactionSuccessText(TransactionInfo info) {
    std::vector<std::string> output;

    output.push_back(info.time + " on the " + info.date);                       // Time and Date
    output.push_back("Amount: " + std::to_string(info.amount) + "vlnc");        // Transaction Amount
    output.push_back("Fee: " + std::to_string(info.fee) + "vlnc");              // Transaction Fee
    output.push_back("Receiver" + info.receiver);                               // Receiver Address
    output.push_back("Number of onion nodes: " + std::to_string(info.nodes));   // # of Onion Routing Nodes

    return output;
}

std::vector<std::string> Interface::transactionFailureText(std::string error) {
    std::vector<std::string> output;

    output.push_back("TRANSACTION FAILED");
    output.push_back(error);

    return output;
}

std::vector<std::string> Interface::createStealthAddressText(std::string publicKey, std::string privateKey) {
    std::vector<std::string> output;

    output.push_back("NEW STEALTH ADDRESS");
    output.push_back("Public Key:");
    output.push_back(publicKey);
    output.push_back("Private Key:");
    output.push_back(privateKey);

    return output;
}

std::vector<std::string> Interface::displayTransactionHistoryText() {
    std::vector<std::string> output;

    for(int i = 0; i < transactionRepo.size(); i++) {
        output.push_back("TRANSACTION " + std::to_string(i + 1));                            // Transaction Number
        output.push_back(transactionRepo[i].time + " on the " + transactionRepo[i].date);    // Transaction Time
        output.push_back(transactionRepo[i].amount + "vlnc to:");                            // Transaction Amount
        output.push_back(transactionRepo[i].receiver);                                       // Transaction Receiver Address
        output.push_back("");
    }
    output.pop_back();  // Remove the last spacing element

    return output;
}
