float arrayAverage(float *arr, int len) {
  float sum = 0;

  for (int i = 0; i < len; i++) {
    sum += arr[i];
  }

  float avg = sum / len;
  return avg;
}

bool arraySearch(int *arr, int val) {
  int i;
  for (i=0;i<sizeof(arr)/sizeof(int);i++) {
    if (arr[i] == val) {
      return true;
    }
  }
  return false;
}

