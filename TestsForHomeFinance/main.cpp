#define CATCH_CONFIG_MAIN

#include "DataStorage.hpp"
#include "catch.hpp"
#include <DataManager.hpp>
#include <DataStorage.h>
#include <DataStorage.h>
#include "IDataManager.h"
#include "IDataStorage.h"
#include "Utils.h"
#include "Database.h"

static Account accountAppend = Account(-1, "testAppendAccount", 2, 2);
static Category categoryAppened = Category(-1, "testCategoryAppened", 1, true, 1);
static Operation operationAppened  = Operation(-1, std::string("Simple basic description"), 2, 2, std::string("03.04.2024"), 220.99);

TEST_CASE("Creating data base", "[dataManager]") {
    auto dbPrepare = !Utils::fileExists("D:/testDataBase.hfproject") || std::remove("D:/testDataBase.hfproject") == 0;
    REQUIRE(dbPrepare);
    auto dataManager = DataManager::dataManager();
    REQUIRE(dataManager != nullptr);
    auto result = dataManager->openProject("D:/testDataBase.hfproject");
    REQUIRE(result == OpenProjectResult::Created);
}


TEST_CASE("Reading data after creating database", "[dataManager]") {
    auto dataManager = DataManager::dataManager();
    REQUIRE(dataManager != nullptr);
    auto result = dataManager->openProject("D:/testDataBase.hfproject");
    REQUIRE(result == OpenProjectResult::Opened);

    auto accounts = dataManager->accounts();
    auto categories = dataManager->categories();
    auto operations = dataManager->operations();

    REQUIRE(accounts->size() == 3);
    REQUIRE(accounts->at(0).name == std::string("Наличные деньги"));

    REQUIRE(categories->size() == 8);
    REQUIRE(categories->at(3).name == std::string("Аптека"));
    REQUIRE(operations->size() == 0);
}

TEST_CASE("Appending data to database", "[dataManager]") {
    auto dataManager = DataManager::dataManager();
    REQUIRE(dataManager != nullptr);
    auto result = dataManager->openProject("D:/testDataBase.hfproject");
    REQUIRE(result == OpenProjectResult::Opened);

    dataManager->appendAccount(accountAppend);
    auto accounts = dataManager->accounts();
    REQUIRE(accounts->size() == 4);
    REQUIRE(accounts->at(3).name == std::string("testAppendAccount"));

    dataManager->appendCategory(categoryAppened);
    auto categories = dataManager->categories();
    REQUIRE(categories->size() == 9);
    REQUIRE(categories->at(8).name == std::string("testCategoryAppened"));

    dataManager->appendOperation(operationAppened);
    auto operations = dataManager->operations();
    REQUIRE(operations->size() == 1);
    REQUIRE(operations->at(0).description == std::string("Simple basic description"));
}

TEST_CASE("Renaming data in database", "[dataManager]") {
    auto dataManager = DataManager::dataManager();
    REQUIRE(dataManager != nullptr);
    auto result = dataManager->openProject("D:/testDataBase.hfproject");
    REQUIRE(result == OpenProjectResult::Opened);

    auto accounts = dataManager->accounts();
    auto categories = dataManager->categories();
    auto operations = dataManager->operations();

    dataManager->renameAccount("newAccountName", 4);
    REQUIRE(accounts->size() == 4);
    REQUIRE(accounts->at(3).name == std::string("newAccountName"));

    dataManager->renameCategory("newCategoryName", 9);
    REQUIRE(categories->size() == 9);
    REQUIRE(categories->at(8).name == std::string("newCategoryName"));

    dataManager->changeOperation(Operation(1, std::string("Simple basic description changed"), 2, 2, std::string("03.04.2024"), 220.99));
    REQUIRE(operations->size() == 1);
    REQUIRE(operations->at(0).description == std::string("Simple basic description changed"));

    dataManager->removeOperation(1);
    REQUIRE(operations->size() == 0);
}


TEST_CASE("Creating data base DataStorage", "[dataStorage]") {
    auto dbPrepare = !Utils::fileExists("D:/testDataBase2.hfproject") || std::remove("D:/testDataBase2.hfproject") == 0;
    REQUIRE(dbPrepare);
    auto dataStorage = DataStorage::dataStorage();
    REQUIRE(dataStorage != nullptr);
    auto result = dataStorage->openProject("D:/testDataBase2.hfproject");
    REQUIRE(result == OpenProjectResult::Created);
}


TEST_CASE("Loading data after creating database (DataStorage)", "[dataStorage]") {
    auto dataStorage = DataStorage::dataStorage();
    REQUIRE(dataStorage != nullptr);
    auto result = dataStorage->openProject("D:/testDataBase2.hfproject");
    REQUIRE(result == OpenProjectResult::Opened);

    auto accounts = std::make_shared<Accounts>();
    auto categories = std::make_shared<Categories>();
    auto operations = std::make_shared<Operations>();
    bool accountsLoadResult = dataStorage->loadAccounts(accounts);
    bool categoriesLoadResult = dataStorage->loadCategories(categories);
    bool operationsLoadResult = dataStorage->loadOperations(operations);

    REQUIRE(accounts->size() == 3);
    REQUIRE(categories->size() == 8);
    REQUIRE(operations->size() == 0);
    REQUIRE(accountsLoadResult == true);
    REQUIRE(categoriesLoadResult == true);
    REQUIRE(operationsLoadResult == true);
}

TEST_CASE("Appending data to database (DataStorage)", "[dataStorage]") {
    auto dataStorage = DataStorage::dataStorage();
    REQUIRE(dataStorage != nullptr);
    auto result = dataStorage->openProject("D:/testDataBase2.hfproject");
    REQUIRE(result == OpenProjectResult::Opened);

    int id;
    REQUIRE(dataStorage->appendAccount(accountAppend, id));
    REQUIRE(id == 4);
    REQUIRE(dataStorage->appendCategory(categoryAppened, id));
    REQUIRE(id == 9);
    REQUIRE(dataStorage->appendOperation(operationAppened, id));
    REQUIRE(id == 1);


    auto accounts = std::make_shared<Accounts>();
    auto categories = std::make_shared<Categories>();
    auto operations = std::make_shared<Operations>();
    bool accountsLoadResult = dataStorage->loadAccounts(accounts);
    bool categoriesLoadResult = dataStorage->loadCategories(categories);
    bool operationsLoadResult = dataStorage->loadOperations(operations);

    REQUIRE(accounts->size() == 4);
    REQUIRE(categories->size() == 9);
    REQUIRE(operations->size() == 1);
    REQUIRE(accountsLoadResult == true);
    REQUIRE(categoriesLoadResult == true);
    REQUIRE(operationsLoadResult == true);

    REQUIRE(accounts->at(3).name == accountAppend.name);
    REQUIRE(categories->at(8).name == categoryAppened.name);
    REQUIRE(operations->at(0).description == operationAppened.description);
}

TEST_CASE("Renaming data in database (DataStorage)", "[dataStorage]") {
    auto dataStorage = DataStorage::dataStorage();
    REQUIRE(dataStorage != nullptr);
    auto result = dataStorage->openProject("D:/testDataBase2.hfproject");
    REQUIRE(result == OpenProjectResult::Opened);

    dataStorage->renameAccount("newAccountName", 4);
    dataStorage->renameCategory("newCategoryName", 9);
    dataStorage->changeOperation(Operation(1, std::string("Simple basic description changed"), 2, 2, std::string("03.04.2024"), 220.99));

    auto accounts = std::make_shared<Accounts>();
    auto categories = std::make_shared<Categories>();
    auto operations = std::make_shared<Operations>();
    bool accountsLoadResult = dataStorage->loadAccounts(accounts);
    bool categoriesLoadResult = dataStorage->loadCategories(categories);
    bool operationsLoadResult = dataStorage->loadOperations(operations);

    REQUIRE(accounts->size() == 4);
    REQUIRE(categories->size() == 9);
    REQUIRE(operations->size() == 1);
    REQUIRE(accountsLoadResult == true);
    REQUIRE(categoriesLoadResult == true);
    REQUIRE(operationsLoadResult == true);

    REQUIRE(accounts->at(3).name == std::string("newAccountName"));
    REQUIRE(categories->at(8).name == std::string("newCategoryName"));
    REQUIRE(categories->at(8).name == std::string("newCategoryName"));
    REQUIRE(operations->at(0).description == std::string("Simple basic description changed"));
}

