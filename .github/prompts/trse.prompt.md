---
mode: agent
---

# TRSE

TRSE (or its full original name “Turbo Rascal Syntax error, “;” expected but “BEGIN”) is a complete suite (IDE, compiler, programming language, resource editor) intended for developing games/demos for 8 / 16-bit line of computers, with a focus on the MOS 6502, the Motorola 68000, the (GB)Z80, the M6809 and the X86. TRSE currently supports application development for the C64, C128, VIC-20, PLUS4, NES, Gameboy, PET, ZX Spectrum, TIKI 100, Amstrad CPC 464, Atari 2600, 8086AT, Amiga 500, Atari 800, BBC Micro, Super Nintendo (SNES), Mega65, VZ200, MSX, Apple II, Commander X16, TIM-011, Videoton TVC, TRS80 CoCo3, the Vectrex, Thomson (mo5 and mo6) and the Atari ST 520 (complete list here). With the benefits of a modern IDE (error messages, code completion, syntax highlighting, sample projects and tutorials) and a bunch of fast built-in tools, it has never been easier to program for your favorite obsolete system!

TRSE runs on Windows 64-bit, Linux 64-bit and OS X. Development began on Feb 24th 2018. The TRSE framework contains a number of project examples for multiple platforms, including almost 300 runnable tutorials. TRSE also contains a real-time ray tracer that can export (compressed) data for demo and game production. In addition to this, TRSE contains a versatile image editor that can edit/import/export sprite/image/levels natively to almost all the systems supported by TRSE. TRSE also contains a rudimentary music tracker that currently supports the VIC-20, Adlib and PC speaker.

# TRSE Structure

Use the `.ras` extension for source files. Use `.tru` for units (libraries).

# TRSE Syntax

## Program structure

Programs must start with the `program` keyword. Variable declarations can happen anywhere after a var is defined. Procedures and functions are defined by their respective keywords. The main block starts with a `begin` and `end`, as does code blocks.

```
Program MyProgram;
var
    a,b,c : byte = 0;

Procedure Myprocedure(mp_i:byte);
var
  somProcedureVariable : byte;
begin
  // Do someting
end;

// This is the main block.
begin
   // Call user-defined procedures etc
   MyProcedure(2);
end.
```

## Keywords

`begin` : starts a code block
`end` : ends a code block
`program` : starts a program
`unit` : starts a unit (library)
`var` : starts a variable block
`type` : starts a user-defined type block
`const` : starts a constant block
`procedure` : starts a procedure definition
`function` : starts a function definition
`if` : starts an if conditional
`then` : follows an if conditional
`else` : follows an if conditional
`while` : starts a while loop
`do` : follows a while loop
`for` : starts a for loop
`to` : follows a for loop
`step` : follows a for loop to define step length
`fori` : starts an inclusive for loop
`unroll` : follows a for loop to unroll the loop
`case` : starts a case block
`of` : follows a case block
`break` : breaks out of a loop
`continue` : continues to the next iteration of a loop
`returnvalue` : returns a value from a procedure (semi-obsolete, use functions instead)
`return` : returns from a procedure/function
`inline` : marks a procedure/function as inline (macro-like)
`forward` : marks a function as forward-declared
`asm` : starts an inline assembly block
`endasm` : ends an inline assembly block
`record` : starts a record definition
`class` : starts a class definition
`this` : pointer to the current class object
`new` : allocates memory for a class object
`delete` : frees memory for a class object
`at` : places a variable at an absolute address
`global` : reuses a variable defined elsewhere (such as in a unit)
`pure` : enforces a parameter to be a pure variable or constant (used in inline functions)
`volatile` : marks a variable as volatile (always read/write, no optimizations)
`array` : defines an array of a given type
`of` : follows an array definition
`string` : defines a string (array of bytes)
`incbin` : includes a binary file
`incsid` : includes a SID music file (C64 only)
`incnsf` : includes an NSF music file (NES only)
`compressed` : marks data to be automatically LZ4-compressed (6502, z80, m68k, m6809 only)
`no_term` : marks a string to be non-null-terminated  
`length` : returns the length of an array/string

## Types

TRSE supports several data types:

`byte` : values 0-255. Supported on all systems.
`integer`: values 0-65535. Supported on all systems.
`long` : 32-bit double word. Supported on all x86 and m68k systems. On the 6502, `long` is a 24-bit number.
`pointer` : Points to an address in memory. Uses zero pages on the 6502, `long` on the x86/m68k and is simply an integer on the (GB)Z80
`lpointer`: 24-bit pointers for the 65c816.
`boolean`: Syntactically identical to a byte.
`string` : An array of characters. Syntactically identical to an array of bytes.
`incbin` : include a binary file
`incsid` : include SID music file (C64 only)
`incnsf` : include NSF music file (NES only)

## Constant number base format

TRSE supports 4 different kind of base numbers:

`base 10` (decimal) : no prefix, 100, 1000 etc
`base 16` (hexadecimal): prefix $, $100 = 256 etc
`base 2` (binary) : prefix %, %0111 = 7 etc
`base 8` (octal) : prefix !, !377 = 256 etc

## Type modifiers

TRSE allows types to have modifiers in order to instruct the compiler to either keep certain things in line (such as forcing a parameter to be pure numeric/variable) or modify the data (automatic lz4 compression of data, or placement in special memory blocks etc). For instance, on some systems it is necessary to attribute a specific type in order to tell TRSE where the block should be placed. This is mostly relevant for the Amiga and the Gameboy, or systems that loads programs from ROM (Vectrex, NES etc).

The type modifiers must be added as a list of keywords directly before or after the type itself (`global` must be placed before the type).

Examples:

```
Program MyProgram;
var
   myArray: array[$100] of byte chipmem; // Places data in chipmem (AMIGA only)
   sprite_data : array[4*3] of byte sprram; // Puts data in sprite ram (GAMEBOY only)
   image_data : incbin("myImage.bin") compressed; // Automatically LZ77-compresses data and includes    (allowed on all systems. Use the TRSE "decompression" library to decompress real-time.)
   myString : string = ("HELLO"); // normal null-terminated string
   myInfiniteString : string no_term = ("HI","THERE"); //no null-termination
   myVar : byte at $3030; // Place variable at an absolute value

   // "global" implies that the variable 'b' is already defined elsewhere, and will be reused (saving a byte)
   // "pure" means that the value *must* be a pure variable or constant, not an expression
   // Inline procedures famously inserts "b" as macros, hence the need for "pure" to eliminate the
   // possibility of the user inserting an advanced expressions.
   procedure MyProcedure( b : global pure byte ) inline;

   // In order to mark a memory address as volatile (ie always write / read from this address as stated, no optimizations), use the volatile keyword:
   somePort : volatile byte at $FE00;
```

## Compression

On systems using the MOS6502, Motorola 68000, Z80 or the Motorola 6809 you can automatically LZ4-compress data using the `compressed` flag. Use the `compression` library to deflate your data:

```
@use "compression/compression"
var
    image : incbin("mydata.bin") compressed;

begin
    ...
    Compression::Decompress(#image,#videomemory);
```

## Chars

A character is a single byte represented with a single letter encased in `. ‘A’ and 65 are treated as identical in TRSE.

```
myByte := 'C'; // same as myByte:=67;
if (myByte='5') then ...
```

## Arrays

TRSE supports arrays of base types (which includes addresses of symbols), records and classes.

If the number of elements in the array doesn’t match the declared count, TRSE will pad the missing items with `0`.

If you don’t know the number of elements in the array – but have a list of data – you can declare the count to be missing by using [].

```
// Preprocessor constants are basically string-replace operation performed before compile
@define somePreprocessorConstant 25

var
   // Real constant
   const someConstant : byte = 25;
   myArray1 : array[128] of integeger; // 128*2 bytes of zeros are declared
   myArray2 : array[4*someConstant] of byte = (0,5,$90, someConstant*2);
   myArray3 : array[] of integer = ($1000, $2000, $3000, someConstant*$100); // unspecified array count
   myArray4 : array[255-55] of byte = (1,2,3); // only 3 values defined, rest will be padded with zeros.
   myMonsters : array[10] of Monster; // "Monster" is either a record or class
  // Buildtable fills arrays compile-time with user-defined javascript code (i is a counter here ranging from 0-256)
   sin : array[256] of byte = BuildTable("Math.sin(i/256.0*2*3.14159)*127 +128");
   sin2 : array[256] of byte = BuildSineTable(127); // how handy!
   // Buildtable2d (indices i and j) lets you generate a 2D table:
   const height : byte = 64;
   const width : byte = 64;
   circles : array[] of byte = buildtable2d(height,width,"256/(Math.sqrt(Math.pow(i-WIDTH/2,2)/2+Math.pow(j-HEIGHT/2,2))+4)*8");
```

Arrays are accessed as such:

```
myArray[i]:=someValue*5;
```

there is no out-of bounds testing in TRSE.

Note that on the 6502 you cannot have arrays of pointers, as they must reside in the zeropage address space. Instead, use an array of integers, and simply assign a pointer value to the integer array element in order to use it, i.e.

```
var
    addresses : array[ 10 ] of integer = (#sym1, $2020, ... )
    zp : pointer;
...
zp := addresses[i];
zp[0] := ...
```

This restriction does not apply on the x86, z80 or m68k where pointers are simply 16/32 bit numbers.

## User-defined types

It is possible to define your own types in TRSE:

```
type mat4 = array[16] of integer;
rotMat : mat4;
```

## Pointers and References

Pointers are the back-bone of any programming language, and are simply variables that contain (`points to`) a memory address. A TRSE pointer p can point to any of the three standard built-in types : byte, integer or long, where the corresponding lookup p[ index ] automatically adjusts for 1,2 or 4 byte data lengths. Pointers can also point to class objects, but not to records (since these are laid out non-chunky in memory by TRSE).

The reference symbol in TRSE is #, analogous to C’s &. # returns the address of a variable/data, and can be assigned to pointers – or 16/32 bit integers/longs. Example:

```
var
   myArray[512] of byte = BuildTable(" i*2 "); // builds a table with values 0,2,4,6,8 .....
   someText : string="SOME TEXT";
   p: pointer; // default byte pointer, same as Pascal ^byte or "pointer of byte"
   ip : pointer of integer; // integer pointer, same as Pascal ^integer
   ip2 : ^integer; // same as "pointer of integer"
begin
   p:=#myArray; // p now points to myArray
   p[2] := 10; // same as myArray[2] := 10;
   p:=#myArray + i*8; // p now points to myArray + i*8
   p:=#someText + 3; // points to the 3rd letter in the string
   ip := #myIntegerArray;  // ip will access elements as integers (16-bit, or 2 byte numbers)
```

## Array of arrays/strings

String lists in TRSE are declared as a list of pointers containing the addresses of the declared strings. Example:

```
var
  strlst : array[ 3 ] of string = ("HELLO", ("YOU",10,65), "WORLD");
   zp : pointer; // a pointer
...
 printstring(strings[1],0,40); // will print out "HELLO"
  strlst[3]:="NEW STRING";
```

## Conditional statements

An if-else block in TRSE is defined as such:

```
if (a=1) then DoSomething();
if (a<>3) then  // not equals
begin
   DoSomethingElse();
   IgnoreMe(4);
end;
if (a*2=b*3) then=return() else somethingelse();
```

Note that an there should be no semicolon `;` before an `else`.

A final note: The `empty` conditional

```
if (a) then DoSomething() else DoSomethingElse();
is the same as typing `if (a<>false)`
```

## Nested conditionals

Nested conditionals must be enclosed with parentheses in order to remove any ambiguity:

```
if (a and (b<>c or (d > e))) then DoSomething();
```

This requirement also applies when all the conditionals are `and`

```
if (a and (b and (c and (d and (e>2))))) then DoSomething();
```

## Binary expressions

TRSE from version 0.16.158 supports binary expressions such as:

```
j := (a>b); // j is true (1) if a>b, if not then false (0)
someFunc( (a>3) and (a<=6)); // parameter will be true (1)  if a is 4,5 or 6, else false (0)
```

Note that binary expressions cannot appear in regular conditional expressions.

## while

While conditionals works similar to if statements:

```
while (a>0) do
begin
  PrintSomething(a);
  dec(a);
end;
```

Multiple conditionals:

Multiple conditionals needs to be nested in a hierarchy as such:

```
if (a>0 and (c<10 or d<>5)) then ...
if (j<>k*2 or (c<10 or (b=c and d=e))) then ...
```

## Cases

Cases tests a single variable for multiple values as such:

```
case i of
  0: DoSomething();
  1: begin
       HelloThere();
       b:=b+5;
     end;
  2+i : ThisIsWeird();
 else
    begin
      PerformElseBlock();
    end;
```

The else block is optional.

## Control statements

TRSE supports the usual suspects when regards to control statements in loops: break and continue

```
for i:=0 to 10 do
  if (i=5) then continue; // if i=5 then the program jumps back to the start of the for loop where i is now 6
end;
  if (i=8) then break; // breaks off the loop when i=8
```

## Procedures

Procedures are defined as such:

```
Procedure NoParameters();
begin
   // .. code here
end;

// Procedure with single parameter and variable block
procedure DoSomething(a,b:byte; zp: pointer);
var
  localVar : byte;

begin
   zp:=zp+a;
   localVar := a+b;
   // ...
end;
```

TRSE does not support string parameters. Please use a byte pointer parameter instead, but strings elsewhere.

## Functions

Functions are procedures that can return values. TRSE currently only supports ‘byte’, ‘integer’ and ‘boolean’ as return values.

```
function Calc( p : byte ) : byte;
begin
   // return some random calculation
   Calc := p*p + 2;
end;
begin
   // Call function
   i:=Calc( j+2 );
   // ...
end;
```

You can only set the return value once in the scope of the function.

## A note on Returnvalue (semi-obsolete)

ReturnValue is a semi-obsolete method that was used before TRSE supported functions.
Calling `ReturnValue( int / byte )` will load the parameter value into the internal
byte / int registers for the current CPU, and return from the function (rts/ret etc), allowing
for function-like return values as such:

```
procedure Calc( p : byte );
begin
   // return some random calculation
   Calc := p*p + 2;
end;
begin
   // Call function
   i:=Calc( j+2 );
   // ...
end;
```

Note: A major flaw with `Returnvalue` – and why functions should be used instead – is that
the compiler is unable to obtain the correct return value type. The following code will therefore fail:

```
// The compiler doesnt know whether "Calculate" returns a byte or an integer, but since
// "someInteger" is an int it assumes this incorrectly.
// Buggy on the 6502 (since the contents of "y" is unknown)
// Fatal on the Z80 since bytes are stored in "a" whereas ints in "hl"
someInteger := Calculate( j );
```

## Forward declaration

You can forward declare a function by using the `forward` keyword:

```
function Calc( p : byte ) : byte; forward;

// requires "Calc" to be defined
procedure Hello(j : byte);
begin
     k:=Calc(j);
end;

// Actual definition
function Calc( p : byte ) : byte;
begin
   // return some random calculation
   Calc := p*p + 2;
end;
```

## Inline declaration

You can declare a procedure/function as inline by using the inline keyword. This will effectively remove the procedure declaration and simply insert the function code directly. As this effectively works like a macro, note that function parameters are evaluated on the fly – meaning that if you have an advanced function you’ll get a lot of overhead. Also, since inline functions behave like macros, parameters are non-existent so you cannot access them via assembler. Inline is useful for system calls and small, fast routines.

```
// Example from the trs-80 coco3 system unit that initialises the stack pointer,
// requiring inline injection (since a jsr would fail when returning as the stack pointer is modified)
procedure InitSystem() inline;
begin
    DisableInterrupts();
    DisableROM();
    asm("
    clra
    tfr a,dp
    lds #System_stack ; modifies the stack
    ");
    SetFast();
end;

// Bad use of inline
procedure Calc( p : byte ) inline;
begin
     c:= p * p +2;
end;

// because if you do this:

 Calc(i*3 + j);

// the inlining of the parameter will effectively insert this expression:
c:= (i*3 + j) * (i*3 + j) +2;
```

Note that you can _enforce_ inline parameters to be pure – either a pure numerical value or pure variable by using the `pure` keyword

```
procedure Calc( p : pure byte ) inline;
begin
     c:= p * p +2;
end;
...
// the following will raise a compiler error because the parameter
// isn't a pure variable or number
Calc(i*3 + j);
```

## For loops

For loops in TRSE are by default non-inclusive, meaning that the below expression will count i from 0 to 9

```
for i:=0 to 10 do
begin
   PrintNumber(i); // will print 0-9
end;
```

If you prefer inclusive for, use the fori keyword:

```
fori i:=0 to 10 do
begin
   PrintNumber(i); // will output 0-10
end;
```

You can change the step length by using the step keyword:

```
for i:=0 to 10 step 2 do
begin
   PrintNumber(i); // will output 0,2,4,6,8
end;
```

## Unrolling

You can specify the compiler to unroll your loops for faster execution, at the cost of larger code:

```
for i:=0 to 5 unroll do
begin
   doSomething(i+1);
end;

// syntactically the same as
doSomething(1);
doSomething(2);
doSomething(3);
doSomething(4);
doSomething(5);
```

## Inline assembly code

TRSE allows the user to insert assembly code anywhere in a statement block. You can mix between Pascal and asm at will.

```
for i:=0 to 5 do
begin
   v += 5;
   asm("
     lda v
     asl
     tax
myLoop:
     ...

   ");
end;
```

Note that since variables are internally sometimes stored with prefixes (such as unit variables etc), you will have to be careful when accessing these variables from an inline assembly block. Variables with the same name as internal CPU registers are always prefixed (such as `hl`,`a`, `bc` on the z80 and `ax`,`bx` etc on the x86). Z80 example:

```
Unit SomeUnit;
var
   screen : ^byte;
...
 // screen variable is internally stored as "SomeUnit_screen"
   asm("
     ld hl, [ SomeUnit_screen ] ;
     ...
```

## Inline assembly unrolling

The internal OrgAsm 6502/z80 assembler allows for 1d and 2d unrolling of code. The internal unroll values are expressed through [i] and [j].

```
asm("
   repeat 50
   lda $8000 + [i*8]
   sta $4400 + [i]
   repend
");
/*
  The above code will generate the following unrolled code:
  lda $8000 + 0
  sta $4400 + 0
  lda $8000 + 8
  sta $4400 + 1
  lda $8000 + 16
  sta $4400 + 2
  ...
*/
```

## Preprocessors

TRSE has a lot of preprocessor directives defined. These can be used to give messages to the compiler before the actual parsing of the program is executed.

## Constants

Constants are preprocessors that will be string replaced before compilation occurs.

```
@define myConstant 4
@define myText "WHOO PARTY"

...
a:=@myConstant * 5; // same as 4*5
```

## Preprocessor ifdef / if / else / endif

You can define preprocessor constants and use them in preprocessor expressions as such:

```
@define L 1
@define N 10


@ifdef L // If @L is defined...
    @if N = 10    // if @N holds the value of 10
        screen_bg_col:=1;
    @else
        screen_bg_col:=2;
    @endif
@else // if L is not defined
    screen_bg_col:=4;
@endif
```

## Pre-existing preprocessor defines

You can define your own preprocessor for a given project in the project settings->target output->global defines. Simply type `@define IS_DEBUG 1` etc in order to add the preprocessor to your project.

TRSE also provides the user with some handy preprocessor defines for determining the current CPU / system:

```
...
@ifdef C64
 // the following line will only be compiled if the current project is for the C64
 screen_bg_col := black;
@endif
@ifdef CPU_Z80
 // the following line will only be compiled if the current project is for a Z80 CPU
   asm(" push bc ");
@endif
```

The syntax for cpu-specific defines are CPU_MOS6502, CPU_Z80, CPU_PX86, CPU_M68K, CPU_GBZ80 and CPU_M6809.

## Other preprocessor commands

TRSE contains a lot of preprocessor commands that can perform a variety of function. Some examples are:

```
@raiseerror "This text will halt compilation and display this message"

@raisewarning "This text will continue compilation and display this message as a warning"

@deletefile "myfile.bin"

// will export 10 sprites from mysprites.flf
// @export works on all .flf files, and will export to the system's
// native video format
@export "mysprites.flf" "mysprites.bin" 10
// will export a subregion in a file. Only works on select systems
@exportsubregion "tiles.flf" "tiles.bin" 0 0 8 8
// Sprite compiler will convert sprites to pure .asm functions for that extra speed. Only works on select systems.
@spritecompiler "images/sprites1.flf" "tank" 3 0 16 8
```

There are a lot of undocumented built-in preprocessor commands for automatically building noise data, path tools, conversion, file handling, sprite compiling etc. If you have an idea for a neat preprocessor tool command for your favourite system, don’t hesitate requesting getting it implemented into TRSE!

## Macros

Macros are a powerful way to generate Rascal code using javascript. Macros are evaluated on the first parse (before TR code is evaluated), where the javascript can produce automated code / variables / tables. Macros are defined using the @macro preprocessor command, where the first parameter is the name of the macro and the second the number of parameters. The parameters supplied to the macro are internally named p0, p1, p2 … etc, and can be accessed through javascript. Since macros aren’t part of any code per say, it is possible to insert them in variable blocks in order to include / generate data. Use the TRSE-specific `write` or `writeln` command to produce TRSE code. Usage:

```
@macro "funmacro1" 2

    j=0;
    for ( j=0;j<p1;j++ )  {
        writeln(' i:='+p0*j+'; ');
        writeln('moveto(5,5+'+j+',04); ');
        writeln('printdecimal(i,3); ');
    }

@endmacro

@macro "printsomestring" 1

    writeln('printstring("'+p0+'",0,40);');

@endmacro

...

begin
       @funmacro(3,5)   // will generate 5 separate moveto/printline and print out the value from 0-4 multiplied by 3
       @printsomestrong("HELLO")  will produce 'printstring("HELLO",0,40);'
```

## Records

A record is similar to a `typedef` in C, and lets the programmer bundle a set of variables together to a primitive object. TRSE allows for creation of these objects as arrays. Internally, each element of the record (or array) is stored as individual arrays. This allows for fast lookup on the 6502 and z80 etc, but prevents any pointers to records as the data isn’t bundled together.

Since records are internally stored as arrays of data, having arrays within records is not possible. You will need to define a class instead, which will store data sequentially instead of a distributed manner.

```
program SomeRecords;
var
    monster_type = record
        x,y : byte; // Current position of monster
        color : byte; // Current colour of monster
        health : byte;
    end;

    player : monster_type; // Declare "player" as a single monster type
    const noMonsters : byte = 10;
        // Declare an array of monsters_type
    monsters : array[ noMonsters ] of monster_type;
```

## Classes

A class is similar to a Record, but has a couple major distinctions. First, you can define methods (procedures) within the class that operates on its properties. Also, class data is stored sequentially – or chunky – as opposed to records. This makes lookup a bit slower on the 6502, but you can now have pointers to the objects. Third, it is possible to have arrays and strings located inside classes (as opposed to records).

Use the `this` pointer in order to access local class variables within a method.

```
var
    Monster = class
        x,y : byte; // Current position of monster
        color : byte; // Current colour of monster
        health : byte;

        procedure Draw();
        begin
             moveto(this.x, this.y,hi(screen_char_loc));
             screenmemory[0] := 0; // draw a @
        end;

    end;

    player : Monster; // Declare "player" as a single monster type
    const noMonsters : byte = 10;
    // Declare an array of monsters_type
    monsters : array[noMonsters] of Monster;
    pm : pointer of Monster;

  ..
pm := #monsters[3]; // Point to the 3rd monster
pm.x := 10;
pm.Draw();
monsters[3].Draw(); // same as pm.Draw();
```

Please note: Classes are still in its very early implementation, and does not yet support inheritance/constructors/private/public properties etc. Also, in order to ensure good coding practice (= I haven’t managed to write an optimizer for this yet), setting properties directly of arrays of objects is not allowed:

```
monsters[i].x := 10; // NOT allowed in TRSE
monsters[i].y := 20; // NOT allowed in TRSE
```

These calls would have to calculate i\*sizeof(Monster) on every step and set a temporary pointer to the memory location. Instead, you’ll have to do this manually – enforcing good coding standards:

```
var
   pm : pointer of Monster;
...
pm := #monsters[i]; // Points to the i'th element
pm.x := 10;
pm.y := 20;
```

## Units

A Turbo Rascal Unit file (.tru) is a re-usable library (containing rascal source code) that can be included and compiled on build. Units are extremely handy when creating re-usable pieces of code (also across all platforms). Units are not pre-compiled, but are included compile-time.

Units can be placed in 4 different levels, depending on the usage:

Project level: The .tru file is located somewhere in your project folder, and can only be accessed from your project. Example: game monster library, inventory library
System level: The .tru file (located in TRSE/units/[SYSTEM]) can be accessed from all projects for the given system (ie C64, Amstrad, X86, Amiga). These libraries should typically contain system-specific functions such as screen setup, text output, line drawing, file handling etc.
CPU level: The .tru file (located in TRSE/units/cpu*specific/[CPU]) can be accessed from all projects for any system that uses the specific CPU (ie 6502, Z80, M68K, X86 etc). These libraries should contain system-independent methods that are useful for all projects of the same CPU. Examples include: compression/decompression, math methods (matrix operations, vectors, operations etc)
Global level: the .tru file (located in TRSE/units/global) is accessible for \_all* systems using _all_ CPU types. This means that the code is completely platform independent. Examples include: string manipulation libraries, 3d engine
It is possible to mix levels of units, where global units contain `high level code` and system-specific units contain `low-level` code (perhaps with direct asm + system-specific calls). As an example, think of a wireframe 3D renderer:

A global unit containing high-level methods such as mesh creation, draw calls, sorting methods, list of of 3d objects etc. Accessible from all systems.
CPU-specific units containing mathematical matrix/vector operations. These units are used in the main global 3d egnine. (example: 6502 matrix class, z80 matrix class etc)
A system-specific graphics class for line drawing: when the `global` unit calls the respective `draw line` method from the graphics unit, the actual implementation is performed on a system level (since a drawline method would vary greatly between the C64, BBC Micro and Amiga 500).
Declaration:

```
Unit MyUnit;
var
   var1, var2 : byte;
   const ANumber : integer = 200;
   Monster = record
      x,y,health : byte;
   end;

// keyword "global" reuses var1
procedure Proc1(var1 : global byte);
begin
   // .. do some stuff
end;

end.
```

Usage:

```
Program myProgram;
@use "MyUnit"
var
   monsters : array[ MyUnit::ANumber ] of MyUnit::Monster;
...

  MyUnit::Proc1( 10 );
```

Note that you access unit variable/methods by `Unit::var`, where `::` is the default separator. `::` is also synonymous to `_`, so you can also access the members by `Unit_var` instead. Using `_` us mandatory when accessing variables/methods within assembly blocks.
