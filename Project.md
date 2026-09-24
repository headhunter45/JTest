# JTest — A C++ Test Framework That Feels Like Jasmine

This document is the single point of reference for anyone new to the **JTest**
project. It explains what we are building, why, how it maps to the JavaScript
testing framework **Jasmine**, and how the pieces fit together today. If you
only read one file, read this one first.

---

## 1. What is JTest, and who is it for?

**One-sentence summary:** JTest is a C++ unit-testing framework whose API is
deliberately modelled on JavaScript's Jasmine, so that anyone who already knows
Jasmine can write C++ tests without relearning the vocabulary.

**The job-to-be-done:** a developer testing C++ code wants the same comfort,
readability, and rhythm they get from Jasmine — `describe`/`it`/`expect`/`toBe` —
but in a native, header-and-source C++ form, compiled locally with `clang++`.

**Who the users are:**
- **Test authors** — C++ developers who write test files using `describe`,
  `it`, `expect(...).toBe(...)` etc.
- **Framework maintainers** — the small team (you) that extends JTest itself.

**What it is NOT (yet):** it is not a drop-in for GoogleTest, not a CI runner,
and not complete. It is an evolving design. See [§8 Current State & Roadmap](#8-current-state--roadmap).

---

## 2. Why the name "Jasmine"? (the vocabulary mapping)

Jasmine is one of the most widely recognised behaviour-driven testing (BDD)
frameworks in JavaScript. JTest reuses its three pillars one-for-one, plus the
Jasmine "pending" and "failure" idioms. The mapping below is the core mental
model of the whole project.

| Concept in JTest   | Jasmine equivalent | What it does                                             |
|--------------------|--------------------|----------------------------------------------------------|
| `describe(label, makeTests)` | `describe` | Names a group of related tests; can nest.                |
| `xdescribe(...)`   | `xdescribe`       | Same as `describe`, but the whole group is **disabled**. |
| `it(label, fn)`    | `it` / `spec`     | Declares a single test case that runs `fn`.              |
| `xit(label, fn)`   | `xit`             | Declares a test that is **disabled** (skipped).          |
| `expect(x)`        | `expect`          | Returns an `Expectable<T>` you assert on.                 |
| `.toBe(matcher)`   | `toBe`            | Assert equality / truth via a custom matcher function.    |
| `.toBeTrue/False/Null/Equal(...)` | `toBe` family | Type-specific boolean/null/equality matchers.            |
| `.toThrow(...)`    | `toThrow`         | Assert a callable throws (optionally a matching exception).|
| `.nevermore()`     | `.not`            | **Inverts** the next assertion (negation). See §4.4.     |
| `expect(x).nevermore().toEqual(y)` | `expect(x).not.toBe(y)` | Negative assertion. |
| `beforeAll/afterAll/beforeEach/afterEach` | same names | Lifecycle hooks, supplied via `DescribeOptions`. |
| `fail(reason)`     | `fail`            | Force a test to fail with a message.                     |
| `pending(reason)`  | `pending`/`xit`   | Mark a test as pending (skipped) with a reason.          |

> **Naming note for maintainers:** JTest can't use Jasmine's `.not()` because
> `not` is awkward in C++. The current chosen name is **`nevermore()`**. A shortlist
> of alternatives was tracked in `include/JTest/Expectable.h` — revisit before
> anything depends on it.

---

## 3. Project layout

```
JTest/
├── Project.md            <-- you are here: the team reference doc
├── README.md             <-- high-level, plus a TODO list (see §8)
├── LICENSE               <-- MIT
├── Makefile              <-- builds the framework + the example test binary
├── include/JTest/        <-- public headers (the API surface)
├── src/JTest/            <-- framework implementation (.cpp)
├── examples/             <-- example test sources that exercise the API
└── build/                <-- generated objects/binaries (gitignored target dir)
```

### 3.1 Headers (`include/JTest/`) — the public API

| Header | Responsibility |
|--------|----------------|
| `JTest.h` | Umbrella header. Declares the top-level functions: `execute`, `describe`, `xdescribe`, `it`, `xit`, `fail`, `pending`, and the `expect<T>(T)` template. **Start here.** |
| `Expectable.h` | The `Expectable<T>` matcher class — all the `toBe/toBeEqual/toBeTrue/.../toThrow` matchers plus negation. Header-only, templated. |
| `TestBundle.h` | A node in the test tree: holds `tests_` (leaf tests), `children_` (nested bundles), the four lifecycle hooks, and `disabled_`. |
| `Test.h` | A single leaf test: `label_`, `test_method_` (a `test_fn`), `disabled_`. |
| `TestResults.h` | Accumulated counters: `total / passed / failed / skipped`, with `operator+`, `+=`, and `operator<<` for reporting. |
| `TestOptions.h` | Placeholder for per-test options — currently an empty class. |
| `DescribeOptions.h` | Per-group options: `beforeAll/afterAll/beforeEach/afterEach` setters (fluent, return `*this`). |
| `ConfigureFunction.h` | `configure_fn = std::function<void()>` and a `combine(...)` helper that chains two optional hooks. |
| `TestFunction.h` | `test_fn = std::function<void()>` — the signature every `it(...)` body is wrapped in. |
| `TestStatus.h` | `enum TestStatus { Unknown, Passed, Failed, Skipped }`. |
| `TestException.h` | Base exception (`std::runtime_error`) carrying label/path/function/file/line; the foundation of test failures. |
| `TestFailedException.h` | `TestException` subclass = a reported test **failure** (`reason()`). |
| `TestPendingException.h` | `TestException` subclass = a **skipped/pending** test. |
| `FailedExpectation.h` | `TestFailedException` subclass thrown by a matcher when an assertion fails. |

### 3.2 Sources (`src/JTest/`)

`.cpp` implementations for: `ConfigureFunction`, `DescribeOptions`,
`FailedExpectation`, `JTest` (the runtime: `describe/it/execute`), `Test`,
`TestBundle`, `TestException`, `TestFailedException`, `TestOptions`,
`TestPendingException`, `TestResults`, `TestStatus`.

### 3.3 Examples (`examples/`)

- `example.cpp` — the **example test driver**. Contains several `describe` blocks
  and a `main()` that runs them, collects a `TestResults`, and prints it. This is
  the file you read to learn the API by example.
- `ClassToTest.h` / `ClassToTest.cpp` — currently **empty stubs** (a `MyNS`
  namespace placeholder). Intended to be the first "class under test".

---

## 4. How the API works (concepts)

### 4.1 Describing and nesting

A test file builds a tree of `describe` blocks. Each `describe` takes a label and a
**factory function** (`make_testbundle_fn`) that returns the inner
`TestBundle`. Nesting is done by returning `TestBundle`s (or other `describe`
results) from those factories.

```cpp
describe("ClassToTest", [](){
    return TestBundle(
        {
            it("should do the thing", [](){
                // body
            }),
            it("should do the other thing", [](){
                pending("we haven't made the other thing yet");
            }),
        },
        DescribeOptions()
            .beforeEach([](){ /* setup */ })
            .afterEach([](){ /* teardown */ })
            .beforeAll([](){ /* once before all */ })
            .afterAll([](){ /* once after all */ }));
});
```

`xdescribe(...)` / `xit(...)` are the "disabled" variants.

### 4.2 Expect and matchers

`expect(actual)` returns an `Expectable<T>`. You chain a matcher onto it.

```cpp
expect(MyAddFunction(2, 2)).toEqual(4);      // true → pass; false → throw
expect(x).toBeTrue();                        // boolean-ish
expect(ptr).toBeNull();                      // pointer
expect(fn).toThrow(std::runtime_error("...")); // callable throws
```

Matchers implemented today:
- `toBe(matcher)` — matcher is `std::optional<std::string>(const T&)`; returns a
  value on failure → throws `FailedExpectation`.
- `toBeFalse()`, `toBeTrue()`, `toBeNull()`, `toEqual(value)`.
- `toThrow()` / `toThrow(exception)` / `toThrow(string)` / `toThrow(matcher)` —
  several overloads for "assert this callable throws".

### 4.3 Negation (the `not` problem)

`Expectable::nevermore()` flips the internal `is_inverted_` flag so the next
matcher asserts the negative — JTest's stand-in for Jasmine's `.not()`.

```cpp
expect(2 + 2).nevermore().toEqual(5);   // passes: they are not equal
```

### 4.4 Running tests

The top-level entry point is `execute(...)`. There are two overloads:

- `execute(TestBundle bundle, path="")` — runs a group; recursively handles
  `beforeAll → children → tests (each wrapped in beforeEach/afterEach) → afterAll`.
- `execute(Test test, bundle_label)` — runs one test; increments the right
  counter and prints a line.

A typical `main()` (see `examples/example.cpp`) accumulates results and prints
them:

```cpp
TestResults results;
results += run_group_a(args);
// ...
std::cout << results << std::endl;
```

### 4.5 Failure model (exceptions carry the signal)

JTest uses **exceptions to flow test outcomes**, then catches them at the
`execute(Test...)` level to turn them into counters + output:
- A matcher fails → throws `FailedExpectation` (a `TestFailedException`).
- `fail(reason)` → throws `TestFailedException`.
- `pending(reason)` → throws `TestPendingException`.
- Any uncaught `std::exception` / `...` → counted as a failure ("Unhandled
  exception running test").

Result line formats (seen in real output — see §7):
- `🚧 Pending Test: <path>::<label>`
- `❌ Failed Test: <path>::<label> reason: <reason>`
- `🔥 Unhandled exception running test: <path>::<label> what: <what>`
- Final summary: `Total: N, Passed: N, Failed: N, Skipped: N`

### 4.6 Lifecycle hooks and inheritance

`beforeAll/afterAll/beforeEach/afterEach` are stored on a `TestBundle` as
`maybe_configure_fn` (an `optional<configure_fn>`). `combine(first, second)`
chains two hooks so parent hooks run relative to child hooks. `describe` merges
`DescribeOptions` hooks into the returned bundle via `combine`.

---

## 5. Building and running

Toolchain: **clang++, C++17, macOS / `clang++`** is assumed. The `Makefile`
targets `clang++` with `-std=c++17` and `-arch arm64`.

```bash
make            # build framework objects + examples/example  (all: target)
make run        # build then execute the example test binary
make clean      # wipe + recreate the build/ directories
```

- `make` builds `build/examples/example` by linking the compiled framework
  objects (`build/JTest/*.o`) with the example object.
- `make run` is `make`'s `run` target: build the example and run it.
- **No external test runner is wired up** yet — the "runner" is `main()` in
  `examples/example.cpp`, which hand-calls `execute(...)` for each group. This is
  intentional for now; an automated/self-testing harness is on the roadmap.

### 5.1 Real run output (verified)

Running `make run` against `examples/example.cpp` (as it stands) prints:

```
🚧 Pending Test: ClassToTest::<anonymous>::should do the other thing
❌ Failed Test: ClassToTest::<anonymous>::should not do the bad thing reason: it did the bad thing
🔥 Unhandled exception running test: ClassToTest::<anonymous>::should throw an exception if we do the other bad thing what: Bad thing happen.
Total: 4, Passed: 1, Failed: 2, Skipped: 1
```

This is the current expected output: one passing, two failing, one pending.

---

## 6. Design notes & conventions to follow

These are the "gotchas" that save the next person hours:

1. **Headers include their own forward-declarations and then re-open the namespace
   after including dependencies.** This pattern (declare `class Foo;` → include →
   reopen `namespace`) appears throughout `include/JTest/`. Match it.
2. **`it`/`xit` return `TestBundle` (not `Test`)** today. This is a known rough
   edge; the plan is to return `Test` and change `describe` to accept a variant.
   Do not "fix" this unilaterally.
3. **The matcher `toBeTruthy/Equal/Null/False` and `toThrow` families use
   `std::enable_if`/`std::is_convertible`/`std::is_invocable`** to compile only
   when the relevant operation makes sense for `T`. When adding a matcher, mirror
   this SFINAE style.
4. **Negation is `nevermore()`, not `not()`** — `not` is not usable as C++
   method syntax. Keep that decision until it's revisited.
5. **Outcome = exception.** Don't invent a second "return a status" path; route
   failures/pending through the existing exception types and the `execute`
   catch-alls.
6. **Fully-qualified names use `::`** between path and label
   (`ClassToTest::<anonymous>::should do the other thing`). Anonymous bundles
   print as `<anonymous>`.
7. **`TestOptions` is a placeholder** — per-test options aren't wired yet.
8. **`Expectable::make_failure_message` only uses `operator<<` for `T`** — a
   known limitation; failure text assumes a streamable type.

---

## 7. How to add a test (recipe for test authors)

1. Put your tests in `examples/` (or wherever the driver runs them).
2. Each "suite file" exposes one or more `TestResults <name>(const vector<string>&)`
   functions, each calling `execute(describe(...), args)` and returning the
   `TestResults`.
3. Register the suite in `main()` by adding `results += <name>(args);` and
   uncommenting it (see `examples/example.cpp` lines around `main`).
4. Build & run: `make run`. Read the printed line per test, then the totals line.

---

## 8. Current State & Roadmap

This project is **early-stage**. The following come directly from `README.md`
(the authoritative TODO list) and the `TODO` comments in the code. Treat these
as the open work; do not assume they're done.

**Framework behaviour**
- Make `Expectable::toThrow` work with arguments, member functions, static
  member functions, and possibly constructors/destructors.
- Report **all** errors at the end instead of printing inline (prerequisite for
  parallel execution).
- Make JTest **self-testing** (possibly namespace magic to run a special
  instance).
- **Mocks** and **Spies** — design, then add.

**API cleanups / design decisions**
- `Expectable<T>`:
  - Clean up failure messages.
  - Only use `ostream` in failure messages when `operator<<` is defined for `T`.
  - Pick the final name for negation (`nevermore()` is the placeholder).
- `describe`/`xdescribe` / `it`/`xit`:
  - Make `it`/`xit` return a `Test` instead of a `TestBundle`.
  - Make `describe`/`xdescribe` accept a variant of `Test` or `TestBundle`.
  - Make `TestOptions` actually useful.
  - Set the label/path at the `it`/`describe` call site, not at execute time.
  - A disabled bundle should report all child tests as skipped/disabled.
  - **Execute tests in parallel.**
  - Verify `before*`/`after*` inheritance: parent `before*` before child,
    parent `after*` after child.
  - When reporting an unhandled exception in a test, include the fully
    qualified test name and label.

**Reporting**
- `DescribeOptions`: add `operator<<` and a `print(ostream&)`.
- `TestResults`: add (currently commented-out) `skipped`/`failed`/`errored`
  vectors to hold detailed per-test info; consider making the counters private.

**Error reporting plumbing**
- `FailedExpectation`: figure out how to capture the label/path when creating an
  expectation from an `expect<T>(T)` call (thread it out of the executing
  `execute` method), then add it when the `FailedExpectation` is caught.

---

## 9. Quick glossary (for new team members)

- **TestBundle** — a node in the test tree (a group). Holds tests, child groups,
  and the four lifecycle hooks.
- **Test** — a leaf: label + `test_fn` + disabled flag.
- **test_fn** — `std::function<void()>`; the body of an `it`.
- **configure_fn / maybe_configure_fn** — `std::function<void()>` / its
  `optional`; the body of a lifecycle hook.
- **Expectable<T>** — the fluent matcher object returned by `expect(x)`.
- **TestResults** — the counters (`total/passed/failed/skipped`) accumulated as
  tests run and printed at the end.
- **combine(a, b)** — chains two optional hooks into one.

---

## 10. How to keep this document fresh

This file is **living documentation**. When you change the public API in
`include/JTest/`, update §2 (vocabulary), §3 (layout), or §6 (conventions). When
you close a README `TODO`, remove it from §8. When you add a new header or a new
matcher, add it to §3.1 / §4.2.
