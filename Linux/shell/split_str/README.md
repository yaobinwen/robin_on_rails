# Split A String

## Overview

This is a copy-and-paste from [this Stack Overflow post](https://stackoverflow.com/a/15988793/630364) which is an awesome answer. The POSIX way of splitting a string is:

```shell
${var#*SubStr}  # drops substring from start of string up to first occurrence of `SubStr`
${var##*SubStr} # drops substring from start of string up to last occurrence of `SubStr`
${var%SubStr*}  # drops substring from last occurrence of `SubStr` to end of string
${var%%SubStr*} # drops substring from first occurrence of `SubStr` to end of string
```

For example (see `split_str.sh`):

```shell
#!/bin/sh

STR="bla@some.com;john@home.com;Full Name <fulnam@other.org>"

C1="${STR%%;*}"
echo "STR = $STR"
echo "C1 = $C1"

STR="${STR#*;}"

C2="${STR%%;*}"
echo "STR = $STR"
echo "C2 = $C2"

STR="${STR#*;}"

C3="${STR%%;*}"
echo "STR = $STR"
echo "C3 = $C3"
```

The output is:

```
STR = bla@some.com;john@home.com;Full Name <fulnam@other.org>
 C1 = bla@some.com
STR = john@home.com;Full Name <fulnam@other.org>
 C2 = john@home.com
STR = Full Name <fulnam@other.org>
 C3 = Full Name <fulnam@other.org>
```

## Compatible answer

There are a lot of different ways to do this in [tag:bash].

However, it's important to first note that `bash` has many *special* features (so-called *[bashisms][1]*) that won't work in any other [tag:shell].

In particular, *arrays*, *associative arrays*, and *pattern substitution*, which are used in the solutions in this post as well as others in the thread, are *bashisms* and may not work under other *shells* that many people use.

For instance: on my *Debian GNU/Linux*, there is a *standard* shell called [tag:dash]; I know many people who like to use another shell called [tag:ksh]; and there is also a special tool called [tag:busybox] with his own shell interpreter ([tag:ash]).

### Requested string

The string to be split in the above question is:

```shell
IN="bla@some.com;john@home.com"
```

I will use a modified version of this string to ensure that my solution is robust to strings containing whitespace, which could break other solutions:

```shell
IN="bla@some.com;john@home.com;Full Name <fulnam@other.org>"
```

### Split string based on delimiter in [tag:bash] (version >=4.2)

In *pure* `bash`, we can create an *array* with elements split by a temporary value for *[IFS][2]* (the *input field separator*). The IFS, among other things, tells `bash` which character(s) it should treat as a delimiter between elements when defining an array:

```shell
IN="bla@some.com;john@home.com;Full Name <fulnam@other.org>"

# save original IFS value so we can restore it later
oIFS="$IFS"
IFS=";"
declare -a fields=($IN)
IFS="$oIFS"
unset oIFS
```

In newer versions of `bash`, prefixing a command with an IFS definition changes the IFS for that command *only* and resets it to the previous value immediately afterwards. This means we can do the above in just one line: 

```shell
IFS=\; read -a fields <<<"$IN"
# after this command, the IFS resets back to its previous value (here, the default):
set | grep ^IFS=
# IFS=$' \t\n'
```

We can see that the string `IN` has been stored into an array named `fields`, split on the semicolons:

```shell
set | grep ^fields=\\\|^IN=
# fields=([0]="bla@some.com" [1]="john@home.com" [2]="Full Name <fulnam@other.org>")
# IN='bla@some.com;john@home.com;Full Name <fulnam@other.org>'
```

(We can also display the contents of these variables using `declare -p`:)

```shell
declare -p IN fields
# declare -- IN="bla@some.com;john@home.com;Full Name <fulnam@other.org>"
# declare -a fields=([0]="bla@some.com" [1]="john@home.com" [2]="Full Name <fulnam@other.org>")
```

Note that `read` is the *quickest* way to do the split because there are no *forks* or external resources called.

Once the array is defined, you can use a simple loop to process each field (or, rather, each element in the array you've now defined):

```shell
# `"${fields[@]}"` expands to return every element of `fields` array as a separate argument
for x in "${fields[@]}" ;do
    echo "> [$x]"
    done
# > [bla@some.com]
# > [john@home.com]
# > [Full Name <fulnam@other.org>]
```

Or you could drop each field from the array after processing using a *shifting* approach, which I like:

```shell
while [ "$fields" ] ;do
    echo "> [$fields]"
    # slice the array 
    fields=("${fields[@]:1}")
    done
# > [bla@some.com]
# > [john@home.com]
# > [Full Name <fulnam@other.org>]
```

And if you just want a simple printout of the array, you don't even need to loop over it:

```shell
printf "> [%s]\n" "${fields[@]}"
# > [bla@some.com]
# > [john@home.com]
# > [Full Name <fulnam@other.org>]
```

### Update: recent [tag:bash] >= 4.4

In newer versions of `bash`, you can also play with the command `mapfile`:

```shell
mapfile -td \; fields < <(printf "%s\0" "$IN")
```

This syntax preserve special chars, newlines and empty fields!

If you don't want to include empty fields, you could do the following:

```shell
mapfile -td \; fields <<<"$IN"
fields=("${fields[@]%$'\n'}")   # drop '\n' added by '<<<'
```

With `mapfile`, you can also skip declaring an array and implicitly "loop" over the delimited elements, calling a function on each:

```shell
myPubliMail() {
    printf "Seq: %6d: Sending mail to '%s'..." $1 "$2"
    # mail -s "This is not a spam..." "$2" </path/to/body
    printf "\e[3D, done.\n"
}

mapfile < <(printf "%s\0" "$IN") -td \; -c 1 -C myPubliMail
```

(Note: the `\0` at end of the format string is useless if you don't care about empty fields at end of the string or they're not present.)

```shell
mapfile < <(echo -n "$IN") -td \; -c 1 -C myPubliMail

# Seq:      0: Sending mail to 'bla@some.com', done.
# Seq:      1: Sending mail to 'john@home.com', done.
# Seq:      2: Sending mail to 'Full Name <fulnam@other.org>', done.
```

*Or* you could use `<<<`, and in the function body include some processing to drop the newline it adds:

```shell
myPubliMail() {
    local seq=$1 dest="${2%$'\n'}"
    printf "Seq: %6d: Sending mail to '%s'..." $seq "$dest"
    # mail -s "This is not a spam..." "$dest" </path/to/body
    printf "\e[3D, done.\n"
}

mapfile <<<"$IN" -td \; -c 1 -C myPubliMail

# Renders the same output:
# Seq:      0: Sending mail to 'bla@some.com', done.
# Seq:      1: Sending mail to 'john@home.com', done.
# Seq:      2: Sending mail to 'Full Name <fulnam@other.org>', done.

```

### Split string based on delimiter in [tag:shell]

If you can't use `bash`, or if you want to write something that can be used in many different shells, you often **can't** use *bashisms* -- and this includes the arrays we've been using in the solutions above.

However, we don't need to use arrays to loop over "elements" of a string. There is a syntax used in many shells for deleting substrings of a string from the *first* or *last* occurrence of a pattern. Note that `*` is a wildcard that stands for zero or more characters:

(The lack of this approach in any solution posted so far is the main reason I'm writing this answer ;)

```shell
${var#*SubStr}  # drops substring from start of string up to first occurrence of `SubStr`
${var##*SubStr} # drops substring from start of string up to last occurrence of `SubStr`
${var%SubStr*}  # drops substring from last occurrence of `SubStr` to end of string
${var%%SubStr*} # drops substring from first occurrence of `SubStr` to end of string
```
 
As explained by [Score_Under][3]:

>  `#` and `%` delete the shortest possible matching substring from the *start* and *end* of the string respectively, and
>
>  `##` and `%%` delete the longest possible matching substring.

Using the above syntax, we can create an approach where we extract substring "elements" from the string by deleting the substrings up to or after the delimiter.

The codeblock below works well in [tag:bash] (including Mac OS's `bash`), [tag:dash], [tag:ksh], and [tag:busybox]'s [tag:ash]:

```shell
IN="bla@some.com;john@home.com;Full Name <fulnam@other.org>"
while [ "$IN" ] ;do
    # extract the substring from start of string up to delimiter.
    # this is the first "element" of the string.
    iter=${IN%%;*}
    echo "> [$iter]"
    # if there's only one element left, set `IN` to an empty string.
    # this causes us to exit this `while` loop.
    # else, we delete the first "element" of the string from IN, and move onto the next.
    [ "$IN" = "$iter" ] && \
        IN='' || \
        IN="${IN#*;}"
  done
# > [bla@some.com]
# > [john@home.com]
# > [Full Name <fulnam@other.org>]
```

Have fun!

  [1]: http://mywiki.wooledge.org/Bashism
  [2]: https://mywiki.wooledge.org/IFS
  [3]: https://stackoverflow.com/posts/comments/47972612?noredirect=1
