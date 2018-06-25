---
layout: post
title: Implement an Interpreter using Bison, Flex, and Automake
category: software
image: /pics/gnu.jpg
---

This is a small example on how to implement an interpreter using [Flex][1], [Bison][2] (formerly known as Yacc), and the [Autotools][3]. The example is based on [Ben Reichard's course material][4].

<h4>Dependencies</h4>

First you need to install the C compiler, the lexer, and the LALR parser generator for this project.
It also helps to install the readline wrapper utility.

{% highlight shell %}
sudo aptitude install build-essential bison flex rlwrap
{% endhighlight %}

<h4>Build system</h4>

You need to create the file *Makefile.dist* with the folowing content.

{% highlight make %}
all:
	aclocal
	autoheader
	libtoolize --force
	automake -a --foreign
	autoconf

configure:
	./configure
{% endhighlight %}

Then you create the file *configure.ac* with the following content.

{% highlight text %}
AC_INIT(aclocal.m4)
AM_INIT_AUTOMAKE([calculator], [1.0.0])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_YACC
AC_PROG_LEX
AM_PROG_LIBTOOL
AM_CONFIG_HEADER(config.h)
AC_CHECK_HEADERS([stdio.h])
AC_OUTPUT(Makefile)
{% endhighlight %}

Finally you create the file *Makefile.am* with the following content.

{% highlight make %}
SUFFIXES = .c .h .y .l

ACLOCAL_AMFLAGS = -I m4

AM_YFLAGS = -d

bin_PROGRAMS = calculator

calculator_SOURCES = calculator.c calc_bison.y calc_flex.l
calculator_LDFLAGS = 
calculator_LDADD =

noinst_HEADERS = calculator.h

BUILT_SOURCES = calc_bison.h

EXTRA_DIST = Makefile.dist configure.ac

CLEANFILES = *~

MAINTAINERCLEANFILES = aclocal.m4 config.guess config.sub configure \
	install-sh ltmain.sh Makefile.in missing mkinstalldirs stamp-h.in \
	libtool config.cache config.h config.h.in acinclude.m4 depcomp \
	ylwrap

maintainer-clean-local:
	-rm -rf m4
{% endhighlight %}

This completes the setup of the build system.

<h4>Implementation</h4>

First create the file *calc_bison.y* with the implementation of the parser.

{% highlight c %}
%{
#include <stdio.h>

void yyerror(char *s) {
  fprintf(stderr, "%s\n", s);
}

int sym[26];
%}

%union {
  int number;
  int var;
};

%type <number> expression
%token <var> VAR
%token <number> NUMBER

%%

start: expression '\n' { printf("%d\n\n", $1); } start
     | /* NULL */
     ;

expression: NUMBER
          | VAR                       { $$ = sym[$1]; }
          | '-' expression            { $$ = -$2; }
          | expression '+' expression { $$ = $1 + $3; }
          | expression '-' expression { $$ = $1 - $3; }
          | expression '*' expression { $$ = $1 * $3; }
          | '(' expression ')'        { $$ = $2; }
          | VAR '=' expression        { sym[$1] = $3; $$ = $3; }
          ;
{% endhighlight %}

Then create the file *calc_flex.l* with the implementation of the lexer (tokenizer).

{% highlight c %}
%{
#include "calc_bison.h"
%}

%option noyywrap
%option always-interactive

%%

[0-9]+     { yylval.number = atoi(yytext); return NUMBER; }

[a-z]      { yylval.var = *yytext - 'a'; return VAR; }

[-+()*\n=] { return *yytext; }

[ \t]      ;

.          yyerror("Unknown character");

%%
{% endhighlight %}

Then create the header file *calculator.h* for the parser.

{% highlight c %}
#ifndef CALCULATOR_H
#define CALCULATOR_H

int yyparse(void);
extern int sym[26];

#endif
{% endhighlight %}

Finally create the file *calculator.c* with the main program.

{% highlight c %}
#include "calculator.h"

int main(void)
{
  int i;
  for (i=0; i<26; i++) sym[i] = 0;
  yyparse();
  return 0;
}
{% endhighlight %}

<h4>Compiling and running it</h4>

Above program is built using the following steps.

{% highlight shell %}
make -f Makefile.dist
./configure
make
{% endhighlight %}

You can run the calculator as follows.

{% highlight shell %}
./calculator
{% endhighlight %}

Alternatively you can run the interpreter with [rlwrap][4] to get command line history.

{% highlight shell %}
rlwrap ./calculator
{% endhighlight %}

Here is a sample session using the calculator program.

{% highlight text %}
1
1

1 + 2
3

a = 1 + 2
3

b = a * 3
9

(1 + 2) * 3
9

(x = b) + 1
10

x
9
{% endhighlight %}

The code is also [available on Github][6]. You can get a copy using [Git][7]:

{% highlight shell %}
git clone https://github.com/wedesoft/calculator.git
{% endhighlight %}

Enjoy!

**See also**

* [OMeta (Parsing Expression Grammars)][8]
* [Treetop (PEG parser for Ruby)][9]
* [Sample LALR(1) parser implemented in Racket][11]
* [Earley parser][12]
* [Lexers in Racket][13]

[1]: http://flex.sourceforge.net/
[2]: http://www.gnu.org/software/bison/
[3]: http://airs.com/ian/configure/
[4]: http://www-bcf.usc.edu/~breichar/teaching/2011cs360/calculator.pdf
[5]: http://freecode.com/projects/rlwrap
[6]: https://github.com/wedesoft/calculator
[7]: http://gitscm.com/
[8]: http://tinlizzie.org/ometa/
[9]: http://treetop.rubyforge.org/
[10]: http://matt.might.net/articles/parsing-with-derivatives/
[11]: https://gist.github.com/danking/1068185
[12]: http://en.wikipedia.org/wiki/Earley_parser
[13]: http://matt.might.net/articles/lexers-in-racket/
