#include "tests_eeprom_fake.h"

#include "EEPROM.h"

void run_tests()
{
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_write);
  RUN_TEST(test_write_then_read);
  RUN_TEST(test_put_then_get);
  UNITY_END(); // stop unit testing
}

void test_write(void)
{
  int index = 0;

  EEPROM.write(index, 10);

  TEST_ASSERT_EQUAL(10, eeprom_data[0]);
  TEST_ASSERT_EQUAL(0, eeprom_data[1]);
  TEST_ASSERT_EQUAL(0, eeprom_data[2]);
  TEST_ASSERT_EQUAL(0, eeprom_data[3]);
}

void test_write_then_read(void)
{
  int index = 0;
  EEPROM.write(index, 10);

  int result = EEPROM.read(index);
  TEST_ASSERT_EQUAL(10, result);
}

void test_put_then_get(void)
{
  int index = 0;

  struct TestData {
    int a = 123;
    char b = 45;
    bool c = true;
  };

  TestData data;

  // Put test data into eeprom
  EEPROM.put(index, data);

  // Modify test data
  data.a = 456;
  data.b = 67;
  data.c = false;

  // Get test data from eeprom
  EEPROM.get(index, data);

  TEST_ASSERT_EQUAL(123, data.a);
  TEST_ASSERT_EQUAL(45, data.b);
  TEST_ASSERT_EQUAL(true, data.c);
}

void setUp (void) {}

void tearDown (void) {}

int main(int argc, char **argv) {
  run_tests();
  return 0;
}