#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

class DataSource {
 public:
  virtual ~DataSource() = default;
  virtual std::string fetch(int id) = 0;
};

class Report {
 public:
  explicit Report(DataSource& source) : source_(source) {}
  std::string title_for(int id) { return "Title: " + source_.fetch(id); }
 private:
  DataSource& source_;
};

class MockDataSource : public DataSource {
 public:
  MOCK_METHOD(std::string, fetch, (int id), (override));
};

TEST(ReportTest, UsesFetchedValue) {
  MockDataSource mock;
  EXPECT_CALL(mock, fetch(42))
      .WillOnce(::testing::Return("Answer"));

  Report report(mock);
  EXPECT_EQ(report.title_for(42), "Title: Answer");
}

TEST(ReportTest, HandlesDifferentId) {
  MockDataSource mock;
  EXPECT_CALL(mock, fetch(7))
      .WillOnce(::testing::Return("Seven"));

  Report report(mock);
  EXPECT_EQ(report.title_for(7), "Title: Seven");
}
