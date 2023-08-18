/* This file is a part of The Valency Project which is released under the GNU Lesser General Public License v2.1
   Go to https://github.com/thevalencyproject/valency-lite-wallet/blob/main/LICENSE for further license details.        */
   
#ifndef INTERFACE_H
#define INTERFACE_H

#include <vector>
#include <string>
#include "valency-core/ui/console-ui/ConsoleUI.h"
#include "valency-network/structures/TransactionInfo.h"
#include "valency-network/wallet-functions/WalletFunctions.h"
#include "valency-core/file-recognition/file-writer/FileWriter.h"
#include "valency-core/file-recognition/file-reader/FileReader.h"
#include "valency-core/cryptography/aes-encryption/AES-Encryption.h"


class Interface {
private:
    ConsoleUI ui(0);
    FileReader reader;
    FileWriter writer;
    AESEncryption aes(256);
    WalletFunctions wallet;

    std::string privateKey;
    std::string publicKey;

    std::string vlncFileType = ".vlnc";
    std::string stealthKeyIndexFilePath = "stealthkeyindex-";    // The file path to the stealth key index file (minus publickey for individual identification)
    std::string transactionRepoFilePath = "transactionRepo-";    // The file path to the transaction repository (minus publickey for individual identification)
    std::vector<TransactionInfo> transactionRepo;                // Holds all the previous transactions
    
    unsigned int stealthKeyIndex(std::string privateKey);    // Reads the stealth key index for this wallet privateKey and increments it 
    void readTransactionRepository();                        // Reads the transaction repository file from this wallet - encrypted in file using AES with private key

    // CUSTOM CONSOLE-UI TEXT
    std::vector<std::string> headerText{"{THE VALENCY NETWORK}", "Lite-Wallet"};    // Header

    std::vector<std::string> loginMenuText{"Login using User + Pass", "Login using Private Key", "Exit"};   // Login Menu
    std::string usernameLoginText = "Enter the Username";
    std::string passwordLoginText = "Enter the Password";
    std::string privateKeyLoginText = "Enter the Private Key";
    std::vector<std::string> loginSuccessText(std::string publicAddress);
    std::string loginFailureText = "LOGIN FAILURE";

    std::vector<std::string> mainMenuText{"Check Balance", "Create Transaction", "Create Stealth Address", "Transaction History", "Save Transaction History", "Logout"};    // Main Menu

    std::vector<std::string> walletBalanceText(double amount);  // Check Balance

    std::vector<std::string> transactionTypeOptionsText{"Single Transaction", "Multi-Transaction"};     // Create Transaction

    std::string numOfTransactionsText = "Enter the number of Transactions";

    std::string oneTimeReceiverAddressText = "Enter the Receiver Address:";

    std::string transactionAmountText = "Enter the Transaction Amount(vlnc):";

    std::string onionRoutingSelectionText = "Use Onion Routing + Bridging?:";

    std::string numOfOnionNodesText = "Enter the # of Onion Nodes (4-6):";

    std::vector<std::string> transactionFeeInfoText(std::string expiryTime, double fee);

    std::string transactionSuccessTitleText = "TRANSACTION SUCCESS";
    std::vector<std::string> transactionSuccessText(TransactionInfo info);

    std::vector<std::string> transactionFailureText(std::string error);

    std::vector<std::string> createStealthAddressText(std::string publicKey, std::string privateKey);   // Create Stealth Address - use prev transaction data appended to private key as seed (Winternitz Signature)

    std::string displayTransactionHistoryTitleText = "TRANSACTION HISTORY";             // Transaction History
    std::vector<std::string> displayTransactionHistoryText();
    std::string displayTransactionHistoryFooterText = "END OF TRANSACTION HISTORY";

    std::string savedHistoryText = "SAVED TRANSACTION HISTORY";     // Save Transaction History

    std::string loggedOutText = "LOGGED OUT SUCCESSFULLY";     // Logout

    // UI Interface Functions
    void login();
    void mainMenu();

    void checkBalance();
    void createTransaction();
    void createStealthAddress();
    void getTransactionHistory();
    void saveTransactionHistory();

public:
    Interface() {};

    void run();
};

#endif
