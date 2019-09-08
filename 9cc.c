#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tokenizer

enum {
  TK_NUM = 256, // Number literal
  TK_EOF,       // End marker
};

// Token type
typedef struct {
  int ty;       // Token type
  int val;      // Number literal
  char *input;  // Token string (for stderr reporting)
} Token;

// Tokennized input is stored to this array
Token tokens[100];

void tokenize(char *p) {
  int i = 0;
  while (*p) {
    // Skip white space
    if (isspace(*p)) {
      p++;
      continue;
    }

    // + or -
    if (*p == '+' || *p == '-') {
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    // Number
    if (isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    }

    fprintf(stderr, "cannot tokennize: %s\n", p);

  }

  tokens[i].ty = TK_EOF;
}

// FRecursive-decendent parser

int pos = 0;

enum {
  ND_NUM = 256,  // Number literal
};

typedef struct Node {
  int ty;            // Node type
  struct Node *lhs;  // left-hand side
  struct Node *rhs;  // right-hand side
  int val;           // Number litelal
} Node;

Node *new_node(int op, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = op;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

// An error reporting function
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

Node *number() {
  if (tokens[pos].ty == TK_NUM) {
    return new_node_num(tokens[pos++].val);
  }
  error("numnber expected, but got %s", tokens[pos].input);
}

Node *expr() {
  Node *lhs = number();
  for (;;) {
    int op = tokens[pos].ty;
    if(op != '+' && op != '-') {
      break;
    }
    pos++;
    lhs = new_node(op, lhs, number());
  }

  if (tokens[pos].ty != TK_EOF) {
    error("stray token: %s", tokens[pos].input);
  }
  return lhs;
}

// Code generator

char *regs[] = {"rdi", "rsi", "r10", "r11", "r12", "r13", "r14", "r15", NULL};
int cur;

char *gen(Node *node) {
  if (node->ty == ND_NUM) {
    char *reg = regs[cur++];
    if (!reg) {
      error("register exhausted");
    }
    printf("  mov %s, %d\n", reg, node->val);
    return reg;
  }

  char *dst = gen(node->lhs);
  char *src = gen(node->rhs);

  switch (node->ty) {
    case '+':
      printf("  add %s, %s\n", dst, src);
      return dst;
    case '-':
      printf("  sub %s, %s\n", dst, src);
      return dst;
    default:
      assert(0 && "unkown operator");
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: 9cc <code>\n");
    return 1;
  }

  // Tokenize and parse
  tokenize(argv[1]);
  Node *node = expr();

  // Print the proluge
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");


  // Genarate code while decending the parse tree.
  printf("  mov rax, %s\n", gen(node));
  printf("  ret\n");
  return 0;
}
