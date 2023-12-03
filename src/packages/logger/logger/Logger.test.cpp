#include "Logger.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class LoggerTest : public ::testing::Test {
 protected:
  void SetUp() override { Logger::messages.clear(); }
};

TEST(LoggerTest, Log) {
  testing::internal::CaptureStdout();
  Logger::Log("This is the first log message");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_THAT(output, ::testing::HasSubstr("This is the first log message"));
  EXPECT_THAT(output, ::testing::HasSubstr("\x1B[32m"));
}
TEST(LoggerTest, Warn) {
  testing::internal::CaptureStdout();
  Logger::Warn("This is warning message");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_THAT(output, ::testing::HasSubstr("This is warning message"));
  EXPECT_THAT(output, ::testing::HasSubstr("\x1B[33m"));
}

TEST(LoggerTest, Err) {
  testing::internal::CaptureStderr();
  Logger::Err("This is an error message");
  std::string output = testing::internal::GetCapturedStderr();

  EXPECT_THAT(output, ::testing::HasSubstr("This is an error message"));
  EXPECT_THAT(output, ::testing::HasSubstr("\x1B[91m"));
}
