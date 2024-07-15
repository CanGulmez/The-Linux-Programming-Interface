# Time

Within a program, we may be interested in two kinds of time:

+ `Real time`: This is the time as measured either from dome standard point
  (`calender` time) or from some fixed point (typically the start) is the
  life of a process (`elapsed` or `wall clock` time).
+ `Process time`: This is the amount of CPU time used a process. Measuring
  process time is useful for checking or optimizing the performance of a 
  program or algorithm.

## Calendar Time

Regardless of geographic location, UNIX systems represent time internally
as a measure of seconds since the Epoch; that is, since midnight on the
morning of 1 January 1970, Universal Coordinated Time (UTC, previously 
known as Greenwich Mean Time, or GMT).

Selected conversion specifiers for `strftime()`:

|---------|------------------------------------|-------------------------| 
|Specifier|           Description              |      Example            |
|---------|------------------------------------|-------------------------| 
|  %%     | A % character                      | %                       |
|  %a     | Abbreviated weekday name           | Tue                     |
|  %A     | Full weekday name                  | Tuesday                 | 
|  %b, %h | Abbreviated month name             | Feb                     |
|  %B     | Full month name                    | February                |
|  %c     | Date and time                      | Tue Feb 1 21:39:46 2011 |
|  %d     | Day of month (2 digits, 01 to 31)  | 01                      | 
|  %D     | American date (same as %m%d%y)     | 02/01/11                | 
|  %e     | Day of month (2 character)         | 1                       | 
|  %F     | ISO date (same as %Y-%m-%d)        | 2011-02-01              |
|  %H     | Hour (24-hour clock, 2 digits)     | 21                      | 
|  %I     | Hour (12-hour clock, 2 digits)     | 09                      | 
|  %j     | Day of year (3 digits, 001 to 366) | 032                     | 
|  %m     | Decimal month (2 digits, 01 to 12) | 02                      | 
|  %M     | Minute (2 digits)                  | 39                      |
|  %p     | AM/PM                              | PM                      | 
|  %P     | am/pm (GNU extension)              | pm                      |
|  %R     | 24-hour time (same as %H:%M)       | 21:39                   |
|  %S     | Second (00 to 60)                  | 46                      |
|  %T     | Time (same as %H:%M:%S)            | 21:39:46                | 
|  %u     | Weekday number (1 to 7)            | 2                       |
|  %U     | Sunday week number (00 to 53)      | 05                      | 
|  %w     | Weekday number (0 to 6)            | 2                       | 
|  %W     | Monday week number (00 to 53)      | 05                      | 
|  %x     | Date (localized)                   | 02/01/11                |
|  %X     | Time (localized)                   | 21:39:46                | 
|  %y     | 2-digit year                       | 11                      | 
|  %Y     | 4-digit year                       | 2011                    | 
|  %Z     | Timezone name                      | CET                     | 
|---------|------------------------------------|-------------------------| 

## Locales

Ideally, all programs designed to run in more than one location should deal
with locales in order to display and input information in the user's 
preffered language and format. Thid constitutes the complex subject of
`internationalization`. In the ideal world, we would write a program once,
and then, wherever it was run, it would automatically do the right things
when performing I/O; that is, it would perform the task of `localization`.

Contents of locale-specific subdirectories:

+ LC_CTYPE     --> A file containing character classifications and rules
                   for case conversion
+ LC_COLLATE   --> A file containing the collection rules for a character
                   set
+ LC_MONETARY  --> A file containing formatting rules for monetary values
+ LC_NUMERIC   --> A file containing formatting rules for numbers other
                   than monetary values
+ LC_TIME      --> A file containing formatting rules for dates and times
+ LC_MESSAGES  --> A directory containing files specifying formats and
                   values used for affirmative and negative responses

## The Software Clocks (Jiffies)

The aacuracy of various time-related system calls described is limited to
the resolution of the system `software clock`, which measures time in
units called `jiffies`. The size of a jiffy is defined by the constant
`HZ` within the kernel source code.

## Process Time

Process time is the amount of CPU time used by a process since it was 
create. For recording purposes, the kernel separates CPU time into the
following two components:

+ `User CPU time` is the amount of time spent executing in user mode.
+ `System CPU time` is amount of time spent executing in kernel mode.

Sometimes, we reffer to process time as the `total CPU time` consumed by
the process.
