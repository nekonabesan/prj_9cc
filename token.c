#include "9cc.h"

Map *keywords;

// Tokenizer
static Token *add_token(Vector *v, int ty, char *input) {
  Token *t = malloc(sizeof(Token));
  t->ty = ty;
  t->input = input;
  vec_push(v, t);
  return t;
}

// Tokennized input is stored to this array
static Vector *scan(char *p) {
  Vector *v = new_vec();

  int i = 0;
  while (*p) {
    // Skip white space
    if (isspace(*p)) {
      p++;
      continue;
    }

    // Single-lattor token
    if (strchr("+-*/;", *p)) {
      add_token(v, *p, p);
      i++;
      p++;
      continue;
    }

    // keyword
    if (isalpha(*p) || *p == '_') {
      int len = 1;
      while (isalpha(p[len]) || isdigit(p[len]) || p[len] == '_') {
        len++;
      }

      char *name = strndup(p, len);
      int ty = (intptr_t)map_get(keywords, name);
      if (!ty) {
        error("unkown identifire: %s", name);
      }

      add_token(v, ty, p);
      i++;
      p += len;
      continue;
    }

    // Number
    if (isdigit(*p)) {
      Token *t = add_token(v, TK_NUM, p);
      t->val = strtol(p, &p, 10);
      i++;
      continue;
    }

    error("Cannot tokennize: %s", p);
  }

  add_token(v, TK_EOF, p);
  return v;
}

Vector *tokenize(char *p) {
  keywords = new_map();
  map_put(keywords, "return", (void *)TK_RETURN);

  return scan(p);
}
