#include "9cc.h"

void expect(int line, int expected, int actual) {
  if (expected == actual) {
    return;
  }
  fprintf(stderr, "%d: %d expected, but got %d\n", line, expected, actual);
  exit(1);
}

static void vec_test() {
  Vector *vec = new_vec();
  expect(__LINE__, 0, vec->len);

  for (int i = 0; i < 100; i++) {
    vec_push(vec, (void *)(intptr_t)i);
  }

  expect(__LINE__, 100, vec->len);
  expect(__LINE__, 0, (intptr_t)vec->data[0]);
  expect(__LINE__, 50, (intptr_t)vec->data[0]);
  expect(__LINE__, 99, (intptr_t)vec->data[0]);
}

static void map_test() {
  Map *map = new_map();
  expect(__LINE__, 0, (intptr_t)map_get(map, "foo"));

  map_put(map, "foo", (void *)2);
  expect(__LINE__, 2, (intptr_t)map_get(map, "foo"));

  map_put(map, "bar", (void *)4);
  expect(__LINE__, 4, (intptr_t)map_get(map, "bar"));

  map_put(map, "foo", (void *)6);
  expect(__LINE__, 6, (intptr_t)map_get(map, "foo"));

  expect(__LINE__, true, (intptr_t)map_get(map, "foo"));
  expect(__LINE__, false, (intptr_t)map_get(map, "baz"));
}

void util_test() {
  vec_test();
  map_test();
}