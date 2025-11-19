template<int RES>
void test_filter_config();


void test_filters() {
  HAL_Delay(3000);
  printf("start test_filters ... \n");

  for (int i = 0; i < 1000; ++i) {
    test_filter_config<4>();
    test_filter_config<8>();
    test_filter_config<10>();
    test_filter_config<12>();
    HAL_Delay(1000);
  }
}


template<int RES>
void test_filter_config() {
  printf("start test_config [res: %d]... \n", RES);

  LowPassFilter<long, RES> angleAbsolute(50000);
  printf("Impulse response: \n");
  printf("time, value \n");

  // do impulse response
  //angleAbsolute.value = 10000;

  for (int i = 0; i < 1000; ++i) {
    printf("%d, %ld \n", i*100, angleAbsolute.value);
    angleAbsolute.update(100*i, 500);
    HAL_Delay(1);
  }
}