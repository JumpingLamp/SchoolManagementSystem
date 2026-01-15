#pragma once
#include "MainEntity.h"
class UserEntity :
    public MainEntity
{
    protected:
        std::string name, phone, address, ic, password;
    public:
		UserEntity() : MainEntity(), name(""), phone(""), address(""), ic(""), password("") {}
		UserEntity(std::string id) : MainEntity(id), name(""), phone(""), address(""), ic(""), password("") {};
        UserEntity(std::string id, std::string name, std::string phone, std::string address, std::string ic, std::string password)
            : MainEntity(id), name(name), phone(phone), address(address), ic(ic), password(password) {
        }
        void create(sql::Connection& conn) override = 0;
        std::unique_ptr<sql::ResultSet> read(sql::Connection& conn) override = 0;
        void update(sql::Connection& conn) override = 0;
		void remove(sql::Connection& conn) override = 0;
        virtual  std::unique_ptr<sql::ResultSet> searchByID(sql::Connection& conn) override = 0;
		virtual ~UserEntity() = default;
};

