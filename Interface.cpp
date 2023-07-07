#include "Interface.h"


// READER FUNCTIONS - DECRYPTS DATA FROM FILE
void Interface::readTransactionRepository() {
    std::vector<std::string> data = reader.getData(transactionRepoFilePath);

    // Decrypt the data using the private key
    transactionRepo.resize(data.size() / 8);         // Resize the transaction repository to its proper size
    for(int i = 0; i < data.size(); i = i + 8) {     // j counts up to transactionRepo.size()
        int j = 0;
        transactionRepo[j].time = aes.decrypt(privateKey, transactionRepo[i]);
        transactionRepo[j].date = aes.decrypt(privateKey, transactionRepo[i + 1]);
        transactionRepo[j].sender = aes.decrypt(privateKey, transactionRepo[i + 2]);
        transactionRepo[j].receiver = aes.decrypt(privateKey, transactionRepo[i + 3]);
        transactionRepo[j].amount = stoul(aes.decrypt(privateKey, transactionRepo[i + 4]));
        transactionRepo[j].fee = stoul(aes.decrypt(privateKey, transactionRepo[i + 5]));
        transactionRepo[j].nodes = stoul(aes.decrypt(privateKey, transactionRepo[i + 6]));
        transactionRepo[j].balance = stoul(aes.decrypt(privateKey, transactionRepo[i + 7]));
        j++;
    }
}

unsigned int Interface::stealthKeyIndex(std::string privateKey) {
    unsigned int output = stoul(aes.decrypt(reader.getData(stealthKeyIndexFilePath)));  // Get the current index
    writer.createFile(std::to_string(output + 1), stealthKeyIndexFilePath);               // Add 1 to the index for next stealth key generation

    return output;
}

// TEXT FUNCTIONS
std::vector<std::string> Interface::loginSuccessText(std::string publicAddress) {
    std::vector<std::string> output{"LOGIN SUCCESS:", publicAddress};
    return output;
}

std::vector<std::string> Interface::walletBalanceText(double amount) {
    std::vector<std::string> output{"WALLET BALANCE:", std::to_string(amount) + "vlnc"};
    return output;
}

std::vector<std::string> Interface::transactionFeeInfoText(std::string expiryTime, double fee) {
    std::vector<std::string> output{
        "Fee Frozen until: " + expiryTime, 
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



// WALLETFUNCTION FRAMEWORK INTERFACING FUNCTIONS
void Interface::login() {
    while(1) {
        switch(ui.input(loginMenuText)) {
        case 1:
            std::string user = ui.input(usernameLoginText);
            std::string pass = ui.input(passwordLoginText);

            std::pair<std::string, std::string> output = wallet.generateKeyPair(user, pass);
            privateKey = output.first;
            publicKey = output.second;

            break;
        default:
            std::string privKey = ui.input(privateKeyLoginText);
            publicKey = wallet.generatePublicKey(privKey);
        }

        if(publicKey.empty()) {
            ui.message(loginFailureText);
            continue;
        }

        ui.message(loginSuccessText(publicKey));
    }
}
// "Check Balance", "Create Transaction", "Create Stealth Address", "Transaction History", "Save Transaction History", "Logout"}
void Interface::mainMenu() {
    int exit = 0;
    while(exit == 0) {
        switch(ui.menu(mainMenuText, false)) {
        case 1:
            checkBalance();
            break;
        case 2:
            createTransaction();
            break;
        case 3:
            createStealthAddress();
            break;
        case 4:
            getTransactionHistory();
            break;
        case 5:
            saveTransactionHistory();
            break;
        case 6:
            exit = 1;
            break;
        }
    }
}

void Interface::checkBalance() {
    if(transactionRepo.empty()) {
        ui.message(walletBalanceText(wallet.walletBalanceCheck(privateKey)));
        return;
    }

    ui.message(walletBalanceText(wallet.walletBalanceCheck(privateKey, transactionRepo)));
}

void Interface::createTransaction() {
    // Transaction Type
    bool transactionTypeSingle = true;
    switch(ui.menu(transactionTypeOptionsText)) { case 2: transactionTypeSingle = false; break; }

    // Number of Transactions - ensure it is less than a set number (10)
    int numOfTransactions = stoi(ui.input(numOfTransactionsText));
    while(numOfTransactions > 10)
        numOfTransactions = stoi(ui.input(numOfTransactionsText));
    
    std::vector<std::string> receiverAddress;
    std::vector<double> transactionAmount;
    for(int i = 0; i < numOfTransactions; i++) {
        // Receiver Address
        std::string receiverAddress[i] = ui.input(oneTimeReceiverAddressText);     // In future revisions, ensure this input is a valid receiver key

        // Transaction Amount
        double transactionAmount[i] = stod(ui.input(transactionAmountText));       // In future revisions, ensure the wallet has this amount
    }

    // Onion Routing
    bool useOnionRouting = false;
    int numOfOnionNodes = 0;
    ui.message(onionRoutingSelectionText);
    switch(ui.yesOrNo(false)) { 
    
    i(ui.input(numOfOnionNodesText));
    break;
    }

    // Transaction Fee
    std::pair<std::string, double> transactionFee = wallet.requestTransactionFee(transactionTypeSingle, numOfTransactions, receiverAddress[0], transactionAmount[0], useOnionRouting, numOfOnionNodes);
    ui.message(transactionFeeInfoText(transactionFee.first, transactionFee.second));
    if(ui.yesOrNo(false) == false)
        return;

    // Send Transaction
    std::pair<bool, TransactionInfo> transaction = wallet.sendTransaction(transactionTypeSingle, numOfTransactions, receiverAddress, transactionAmount, useOnionRouting, numOfOnionNodes)
    if(transaction.first == false) {    // If the transaction is invalid
        ui.message(transactionFailureText("Error with Transaction"))
        return;
    }

    // If the transaction is valid
    transactionRepo.push_back(transaction.second);              // Add the successful transaction to the repository

    ui.message(transactionSuccessTitleText);
    ui.message(transactionSuccessText(transaction.second));     // Display the transaction details
}

void Interface::createStealthAddress() {
    std::pair<std::string, std::string> output = wallet.generateStealthPair(publicKey, privateKey, stealthKeyIndex());
    ui.message(createStealthAddressText(std::string output.first, std::string output.second));
}

void Interface::getTransactionHistory() {
    ui.message(displayTransactionHistoryTitleText);
    ui.message(displayTransactionHistoryText());
    ui.message(displayTransactionHistoryFooterText);
}

void Interface::saveTransactionHistory() {
    std::vector<string> output;
    output.resize(transactionRepo.size() * 8);
    for(int i = 0; i < transactionRepo.size() * 8; i = i + 8) {
        output[i] = aes.encrypt(privateKey, transactionRepo[i].time);
        output[i + 1] = aes.encrypt(privateKey, transactionRepo[i].date);
        output[i + 2] = aes.encrypt(privateKey, transactionRepo[i].sender);
        output[i + 3] = aes.encrypt(privateKey, transactionRepo[i].receiver);
        output[i + 4] = aes.encrypt(privateKey, std::to_string(transactionRepo[i].amount));
        output[i + 5] = aes.encrypt(privateKey, std::to_string(transactionRepo[i].fee));
        output[i + 6] = aes.encrypt(privateKey, std::to_string(transactionRepo[i].nodes));
        output[i + 7] = aes.encrypt(privateKey, std::to_string(transactionRepo[i].balance));
    }

    writer.createFile(transactionRepo, transactionRepoFilePath);
    ui.message(savedHistoryText);
}


void Interface::run() {
    ui.header(headerText);

    login();

    // Decrypt and read repo file + get correct filePath for stealth key index file
    stealthKeyIndexFilePath = "stealthkeyindex-" + publicKey + vlncFileType;
    transactionRepoFilePath = "transactionRepo-" + publicKey + vlncFileType;
    readTransactionRepository();

    mainMenu();

    saveTransactionHistory();
    ui.message(loggedOutText);
}
