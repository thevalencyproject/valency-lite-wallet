#include "Interface.h"


std::vector<std::string> Interface::loginSuccessText(std::string publicAddress) {
    std::vector<std::string> output{"LOGIN SUCCESS:", publicAddress};
    return output;
}

std::vector<std::string> Interface::walletBalanceText(double amount) {
    std::vector<std::string> output{"WALLET BALANCE:", std::to_string(amount) + "vlnc"};
    return output;
}

std::vector<std::string> Interface::transactionFeeInfoText(std::string currentTime, double fee) {
    std::vector<std::string> output{
        "Fee Frozen for 30s from: " + currentTime, 
        "Transaction Fee: " + std::to_string(fee), 
        "Accept Transaction?"};
    return output;
}

std::vector<std::string> Interface::transactionSuccessText(TransactionInfo info) {
    std::vector<std::string> output{
        info.time + " on the " + info.date,                        // Time and Date
        "Amount: " + std::to_string(info.amount) + "vlnc",         // Transaction Amount
        "Fee: " + std::to_string(info.fee) + "vlnc",               // Transaction Fee
        "Receiver:", info.receiver,                                // Receiver Address
        "Number of onion nodes: " + std::to_string(info.nodes)};   // # of Onion Routing Nodes
    return output;
}

std::vector<std::string> Interface::transactionFailureText(std::string error) {
    std::vector<std::string> output{"TRANSACTION FAILED", error};
    return output;
}

std::vector<std::string> Interface::createStealthAddressText(std::string publicKey, std::string privateKey) {
    std::vector<std::string> output{
        "NEW STEALTH ADDRESS",
        "Public Key:", publicKey,
        "Private Key:", privateKey};
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
