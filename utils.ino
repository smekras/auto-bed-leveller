float arrayAverage(float *arr, int len) {
  float sum = 0;

  for (int i = 0; i < len; i++) {
    sum += arr[i];
  }

  float avg = sum / len;
  return avg;
}

bool arraySearch(int *arr, int val) {
  //int x = sizeof(arr)/sizeof(int);
  for (int i=0;i<4;i++) {
    if (arr[i] == val) {
      return true;
    }
  }
  return false;
}

