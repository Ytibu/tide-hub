#include "my_module.h"

#include <sqlite3.h>

namespace blog {

static tide::Logger::ptr g_logger = TIDE_LOG_ROOT();

MyModule::MyModule()
    :tide::Module("tide-blog", "1.0", "") {
}

bool MyModule::onLoad() {
    TIDE_LOG_INFO(g_logger) << "onLoad";
    return true;
}

bool MyModule::onUnload() {
    TIDE_LOG_INFO(g_logger) << "onUnload";
    return true;
}

bool MyModule::onServerReady() {
    TIDE_LOG_INFO(g_logger) << "onServerReady";
    
    return true;
}

static int32_t handle(tide::http::HttpRequest::ptr req, tide::http::HttpResponse::ptr rsp, tide::http::HttpSession::ptr session) {
    TIDE_LOG_INFO(g_logger) << "handle";
    rsp->setBody("hello status");
    return 0;
}


bool MyModule::onServerUp() {
    TIDE_LOG_INFO(g_logger) << "onServerUp";

    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);


    return true;
}

}

extern "C" {

tide::Module* CreateModule() {
    tide::Module* module = new blog::MyModule;
    TIDE_LOG_INFO(blog::g_logger) << "CreateModule " << module;
    return module;
}

void DestoryModule(tide::Module* module) {
    TIDE_LOG_INFO(blog::g_logger) << "DestoryModule " << module;
    delete module;
}

}