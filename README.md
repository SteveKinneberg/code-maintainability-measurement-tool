# Code Maintainability Measurement Tool

## Introduction

As the name implies, the Code Maintainability Measurement Tool or CMMT, aims to
evaluate source code and generate a value that is indicative of how difficult
that code will be to maintain.  At first glance this will look similar to McCabe
Cyclomatic Complexity checkers.  There is, however, a notable difference.
McCabe Cyclomatic Complexity calculations determine the number of test cases
required to cover all the unique code paths through a function.  Software
maintainability is a bit more subjective and there are certain code constructs
in various languages that are relatively easy to maintain but will result in a
large McCabe Cyclomatic Complexity score and vice versa.  This tool aims to
focus on coding practices that lead to hard to maintain code.

*Development Note:  This will initially only support C/C++ code and the rest of this document
may reflect a bias for C/C++.*

## Scoring

Like, McCabe Cyclomatic Complexity values, larger values are worse.  Scores will
be computed for files, classes/structs, and functions.  The actual scoring is as
follows:

* 1 point for each line of code.

  This is to account for the maintenance cost of file scope comments and code.

* 1 point for each line between a pair of matching curly braces, angle brackets,
  square brackets, and parentheses.

  This counts lines of comments too because comments must be maintained and thus
  must be part of the maintenance cost.  Also, we want to discourage superfluous
  comments and encourage comments that get to the point.

  Lines of code that are nested several layers get counted once for each nested
  layer.  This provides a natural penalty for deeply nested code which is harder
  to maintain than shallowly nested code.

  Note that curly braces, angle brackets, square brackets, and parentheses are
  counted together rather than separate.  (This does not verify that the correct
  closing brace type corresponds to the opening brace type.  That is a syntactic
  evaluation left to the compilers.)

  Curly braces that encompass C++ namespaces and `extern "C"` regions will not
  count as a nesting level and will not impose an addition point for each line.

* 1 point for each flow control statement

   Flow control statements include: `if`, `else`, `for`, `do`, `while`, `try`,
  `catch`, `throw`, `return`, `goto`, `break`, `continue`, `switch`, and `case`.

* 1 point for each line between a `goto` statement and its target if the target
  is after the `goto` statement.

  This is to penalize arbitrary disjoint code flow.

  The `break` and `continue` statements will not be further penalized since
  their destination is not arbitrary.

* 5 points for each line between a `goto` statement and its target if the target
  is before the `goto` statement.

  `goto` statements that jump backward are harder to maintain and are generally
  better served by `continue` statements.

* n^2 points where n is the number of parameters to a function or template.

  This is to penalize functions and templates with a large number of explicit
  parameters.  This will not (heavily) penalize functions that use varargs or
  parameter packs.

  This penalty is applied only to where the function or template is defined, not
  declared (i.e., function prototype), nor where used (function call or template
  instantiation).
  
  Note that template parameter points apply to the template declaration and not
  the class, function, variable that is being templated.

  For closures, the number captured variables are computed as a separate set of
  parameters from the lambda's functional parameters.  In the cases where the
  capture expression is `[&]` or `[=]`, the penalty will be equivalent to 10
  parameters.  This is to account the increased maintenance cost associated with
  an increased chance of unintended consequences and the extra effort a
  developer will need to perform to track down the actual variables that have
  been captured.  This is particularly important when considering the lifetimes
  of the captured variables.

* 1 point for each scalar and binary operator (`+`, `-`, `*`, `/`, `%`, `<<`, `>>`, `&`, `|`, `^`)
  in an expression.

* n^2 points where n is the number of boolean operators (`&&` and `||`) in an
  expression.

  This is to penalize complex boolean expressions.  Boolean expressions are most
  often used performing a conditional check on multiple different states at the
  same time.  The more states that are checked, the more difficult the code will
  be to maintain.

  An n^2 penalty is chosen over a simple linear count of conditional operators
  because the simple linear count would slightly penalized code that breaks
  complex conditional expressions in to intermediary boolean variables because
  then the score for the conditional operators would remain unchanged but the
  extra lines of code would result in a higher score for code that is more
  readable.

* (n-1)^2 points where n is the number of lines in a statement.

* 1 point penalty for every 2 characters over 60 columns.

  This is to lightly discourage long-ish lines.

* 1 point penalty for every character over 80 columns.

  This is to discourage long lines.  This is cumulative with penalty for every
  character over 60 columns.

* 5 point penalty for every character over 100 columns.

  This is to strongly discourage long lines.  This is cumulative with penalty for every
  character over 60 and 80 columns.

* 10 point penalty for every character over 120 columns.

  This is to very strongly discourage very long lines.  This may seem excessive,
  especially for programs that have identifiers with very long names.  However,
  the problem is that identifiers with very long names have a negative impact on
  maintainability because they make code harder to read.

  This is cumulative with penalty for every character over 60, 80, and 100
  columns.

* 10 point penalty for every trailing white space character.

  While trailing white space may not visually impact the maintainability of
  code, it is an indication of code cleanliness which does impact maintainability.

* 100 point penalty for mixing spaces and tabs for indents on a single line
  except for the last spaces after tabs if the number of spaces is less than 8.
  
  This is to penalize mixing spaces and tabs when indenting code on a single
  line which is an indication of sloppy code.  Often, in coding standards that
  specify tabs, the indent level may be less than 8 characters thus the
  code will need spaces after the tabs to reach the correct level and thus
  should not be penalized.

* 100 point penalty for inconsistent indentation.

  This is to penalize sloppy indentation.  This includes switching between tabs
  and spaces between consecutive lines.  Inconsistent indentation makes code
  difficult to read.
  
  Indentation changes in block comment (`/* ... */`) and multi-line strings are
  ignored.

* 100 point penalty for each alternate in conditional compilation constructs
  that does not contain complete blocks/statements.

  Here is an example that would incur a 200 point penalty:

  ```
      #ifdef OPTION
          if (a == b) {             // 100 point penalty for this line
      #else
          if (a == b && c == d) {   // Another 100 point penalty for this line
      #endif
              do_something();
          }
  ```

  Some editors will be confused by the above code when applying automatic
  indenting and conditional compilation makes code more difficult to maintain
  anyway so it is best to avoid writing code such as that.


Note: Scores for nested classes/structs and functions will ignore their baseline
nesting level.  Consider the following code:

```
class foo {
  public:
    foo(int a): _a(a)
    {
        std::out << a << std::endl;
    }
  private:
    int _a;
};
```

Class 'foo' will have a score of 18, while its constructor will only have a
score of 6.

*Development Note:  Actual scoring values and thresholds may change as development progresses.*


## Usage

*Proposed:*
```
cmmt [-f # ] [<path>...]
	-a			All rather than just the 10 highest scores for each category.
	-d [#]		Detailed report.  Include reports on blocks, expressions,
				statements, etc. (Warning can produce extremely verbose output.)
	-f #		Fail if any score exceeds the given threshold.
	-o OUTPUT	Send output to OUTPUT.
	-q			Quiet.  Suppress reporting to STDOUT.  Only errors are output.
	<path>		One or more paths to directories and/or files.
```

By default, CMMT will recursively search the current directory for files to
process.  If given one or more directories, then those will be recursively
searched instead of the current directory.  If one or more files are specified
then only those files will be processed.  If both files and directories are
given then the given files will be processed along with all the files found by
recursively searching the given directories.

The `-f` option can be used to set a maximum allowed score whereby if that score
is exceeded then CMMT will exit with a non-zero exit code.  This allows CMMT to
be used a tool in build scripts so that the build script will fail if any code
exceeds a project's defined maintainability standard.


### Scoring reports

By default, CMMT will report the top 10 files, top 10 functions/methods, and
top 10 classes (or structs).  This makes it easy quickly determine which
segments of code will be the most difficult to maintain.


## Caveats

The parsing used be CMMT is fairly simplistic.  It assumes that the code
compiles cleanly.  It also assumes that each file is reasonably normal.  In
other words, it will exit with an error and not provide any scoring if it tries
to process a file with only a partial block of code.  For example, the
following 2 files will cause such an error.

File: foo.inc

```
void foo(int a)
{
    do_something():
```

File: bar.c

```
#include "foo.inc"
    do_something_else();
}
```

A compiler compiling `bar.c` will do so happily because after the `#include`
directive is processed, the resulting module is syntactically complete.  CMMT
does not insert the contents of include file into the module for processing.  It
will just simply count the `#include` line as a line of code.

## Notes

1. The master branch will always remain stable.  Development branches, however,
   may experience changes in history (i.e., `git rebase -i`).

1. The SCons files for building this project are taken from (the development
   branch of) the
   [general-scons](https://github.com/SteveKinneberg/general-scons) project.

1. The licensing of CMMT does not extend to any code it processes, even if CMMT
   is built into a standalone program as a build step in a much larger project.
   Such a configuration is ill-advised for other reasons, 
