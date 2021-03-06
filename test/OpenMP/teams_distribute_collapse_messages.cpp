// RUN: %clang_cc1 -verify -fopenmp %s

void foo() {
}

bool foobool(int argc) {
  return argc;
}

struct S1; // expected-note {{declared here}}

template <class T, typename S, int N, int ST> // expected-note {{declared here}}
T tmain(T argc, S **argv) { //expected-note 2 {{declared here}}
#pragma omp target
#pragma omp teams distribute collapse // expected-error {{expected '(' after 'collapse'}}
  for (int i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST];

#pragma omp target
#pragma omp teams distribute collapse ( // expected-error {{expected expression}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (int i = ST; i < N; i++)

    argv[0][i] = argv[0][i] - argv[0][i-ST];
#pragma omp target
#pragma omp teams distribute collapse () // expected-error {{expected expression}}
  for (int i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST];

// expected-error@+4 {{expected ')'}} expected-note@+4 {{to match this '('}}
// expected-error@+3 2 {{expression is not an integral constant expression}}
// expected-note@+2 2 {{read of non-const variable 'argc' is not allowed in a constant expression}}
#pragma omp target
#pragma omp teams distribute collapse (argc 
  for (int i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST];

// expected-error@+2 2 {{argument to 'collapse' clause must be a strictly positive integer value}}
#pragma omp target
#pragma omp teams distribute collapse (ST // expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (int i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST];

#pragma omp target
#pragma omp teams distribute collapse (1)) // expected-warning {{extra tokens at the end of '#pragma omp teams distribute' are ignored}}
  for (int i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST];

#pragma omp target
#pragma omp teams distribute collapse ((ST > 0) ? 1 + ST : 2) // expected-note 2 {{as specified in 'collapse' clause}}
  for (int i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST]; // expected-error 2 {{expected 2 for loops after '#pragma omp teams distribute', but found only 1}}

// expected-error@+4 2 {{directive '#pragma omp teams distribute' cannot contain more than one 'collapse' clause}}
// expected-error@+3 2 {{argument to 'collapse' clause must be a strictly positive integer value}}
// expected-error@+2 2 {{expression is not an integral constant expression}}
#pragma omp target
#pragma omp teams distribute collapse (foobool(argc)), collapse (true), collapse (-5)
  for (int i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST];

#pragma omp distribute collapse (S) // expected-error {{'S' does not refer to a value}}
  for (int i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST];

// expected-error@+2 2 {{expression is not an integral constant expression}}
#pragma omp target
#pragma omp teams distribute collapse (argv[1]=2) // expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (int i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST];

#pragma omp target
#pragma omp teams distribute collapse (1)
  for (int i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST];

#pragma omp target
#pragma omp teams distribute collapse (N) // expected-error {{argument to 'collapse' clause must be a strictly positive integer value}}
  for (T i = ST; i < N; i++)
    argv[0][i] = argv[0][i] - argv[0][i-ST];

#pragma omp target
#pragma omp teams distribute collapse (2) // expected-note {{as specified in 'collapse' clause}}
  foo(); // expected-error {{expected 2 for loops after '#pragma omp teams distribute'}}
  return argc;
}

int main(int argc, char **argv) {
#pragma omp target
#pragma omp teams distribute collapse // expected-error {{expected '(' after 'collapse'}}
  for (int i = 4; i < 12; i++)
    argv[0][i] = argv[0][i] - argv[0][i-4];

#pragma omp target
#pragma omp teams distribute collapse ( // expected-error {{expected expression}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (int i = 4; i < 12; i++)
    argv[0][i] = argv[0][i] - argv[0][i-4];

#pragma omp target
#pragma omp teams distribute collapse () // expected-error {{expected expression}}
  for (int i = 4; i < 12; i++)
    argv[0][i] = argv[0][i] - argv[0][i-4];

#pragma omp target
#pragma omp teams distribute collapse (4 // expected-error {{expected ')'}} expected-note {{to match this '('}} expected-note {{as specified in 'collapse' clause}}
  for (int i = 4; i < 12; i++)
    argv[0][i] = argv[0][i] - argv[0][i-4]; // expected-error {{expected 4 for loops after '#pragma omp teams distribute', but found only 1}}

#pragma omp target
#pragma omp teams distribute collapse (2+2)) // expected-warning {{extra tokens at the end of '#pragma omp teams distribute' are ignored}}  expected-note {{as specified in 'collapse' clause}}
  for (int i = 4; i < 12; i++)
    argv[0][i] = argv[0][i] - argv[0][i-4]; // expected-error {{expected 4 for loops after '#pragma omp teams distribute', but found only 1}}

#pragma omp target
#pragma omp teams distribute collapse (foobool(1) > 0 ? 1 : 2) // expected-error {{expression is not an integral constant expression}}
  for (int i = 4; i < 12; i++)
    argv[0][i] = argv[0][i] - argv[0][i-4];

// expected-error@+4 {{expression is not an integral constant expression}}
// expected-error@+3 2 {{directive '#pragma omp teams distribute' cannot contain more than one 'collapse' clause}}
// expected-error@+2 2 {{argument to 'collapse' clause must be a strictly positive integer value}}
#pragma omp target
#pragma omp teams distribute collapse (foobool(argc)), collapse (true), collapse (-5) 
  for (int i = 4; i < 12; i++)
    argv[0][i] = argv[0][i] - argv[0][i-4];

#pragma omp target
#pragma omp teams distribute collapse (S1) // expected-error {{'S1' does not refer to a value}}
  for (int i = 4; i < 12; i++)
    argv[0][i] = argv[0][i] - argv[0][i-4];

// expected-error@+2 {{expression is not an integral constant expression}}
#pragma omp target
#pragma omp teams distribute collapse (argv[1]=2) // expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (int i = 4; i < 12; i++)
    argv[0][i] = argv[0][i] - argv[0][i-4];

// expected-error@+4 {{statement after '#pragma omp teams distribute' must be a for loop}}
// expected-note@+2 {{in instantiation of function template specialization 'tmain<int, char, -1, -2>' requested here}}
#pragma omp target
#pragma omp teams distribute collapse(collapse(tmain<int, char, -1, -2>(argc, argv) // expected-error 2 {{expected ')'}} expected-note 2 {{to match this '('}}
  foo();

#pragma omp target
#pragma omp teams distribute collapse (2) // expected-note {{as specified in 'collapse' clause}}
  foo(); // expected-error {{expected 2 for loops after '#pragma omp teams distribute'}}

// expected-note@+1 {{in instantiation of function template specialization 'tmain<int, char, 1, 0>' requested here}}
  return tmain<int, char, 1, 0>(argc, argv);
}

