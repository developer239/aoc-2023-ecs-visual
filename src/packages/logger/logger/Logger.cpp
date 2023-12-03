#include "Logger.h"

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString() {
  std::time_t now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string output(20, '\0');
  std::strftime(
      &output[0],
      output.size(),
      "%d-%b-%Y %H:%M:%S",
      std::localtime(&now)
  );

  return output;
}

void Logger::Log(const std::string &message) {
  LogEntry logEntry;
  logEntry.type = LOG_INFO;
  logEntry.message = "LOG: [" + CurrentDateTimeToString() + "]: " + message;

  std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;
  messages.push_back(logEntry);
}

void Logger::Warn(const std::string& message) {
  LogEntry logEntry;
  logEntry.type = LOG_INFO;
  logEntry.message = "LOG: [" + CurrentDateTimeToString() + "]: " + message;

  std::cout << "\x1B[33m" << logEntry.message << "\033[0m" << std::endl;
  messages.push_back(logEntry);
}

void Logger::Err(const std::string &message) {
  LogEntry logEntry;
  logEntry.type = LOG_ERROR;
  logEntry.message = "ERR: [" + CurrentDateTimeToString() + "]: " + message;

  messages.push_back(logEntry);
  std::cerr << "\x1B[91m" << logEntry.message << "\033[0m" << std::endl;
}
