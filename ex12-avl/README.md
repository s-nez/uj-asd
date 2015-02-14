# Exercise 12 - AVL Tree
## Problem description
Write a program that creates an AVL tree from integers ranged [0, 999].
The program reads numbers from the standard input and checks for their
existence within the tree. If the number is not in the tree, it's inserted
and deleted otherwise.

##Technical requirements
The program should contain the following methods/procedures:
* SEARCH
* INSERT
* DELETE
* rotL
* rotR
* rotLR
* rotRL

After each operation, a summary with the following information
is to be displayed:
* kind of operation (Insert o Delete)
* input value
* rotations (which element was rotated in which way
or 'bez rot' if no rotations occured)
* key contained in the root
* the tree height

If the input is "-1", the program should print the tree,
if it's "-2", the program should terminate.

Tree nodes should only contain the following fields:
* INFO (key)
* HEIGHT (height of the subtree rooted at that node)
* LEFT, RIGHT (pointers to subtrees)

When a node with two non-empty subtrees is being deleted, it should be
replaced with its inorder successor.

Recurrence is allowed.

##Sample input and output
````
\> 21
Insert 21, bez rot, r=21, h=0
\> 22
Insert 22, bez rot, r=21, h=1
\> 23
Insert 23, rotL(21), r=22, h=1
\> 24
Insert 24, bez rot, r=22, h=2
\> 25
Insert 25, rotL(23), r=22, h=2
\> 26
Insert 26, rotL(22), r=24, h=2
\> -1
      .-----(24)--.       
  .-(22)--.     (25)--.   
(21)    (23)        (26)
\> 27
Insert 27, rotL(25), r=24, h=2
\> 28
Insert 28, bez rot, r=24, h=3
\> 29
Insert 29, rotL(27), r=24, h=3
\> 30
Insert 30, rotL(26), r=24, h=3
\> -1
      .-----(24)--------------.               
  .-(22)--.           .-----(28)--.       
(21)    (23)      .-(26)--.     (29)--.   
                (25)    (27)        (30)
\> 40
Insert 40, rotL(29), r=24, h=3
\> 39
Insert 39, rotL(24), r=28, h=3
\> 38
Insert 38, rotR(40), r=28, h=3
\> 37
Insert 37, rotRL(30), r=28, h=3
\> 36
Insert 36, bez rot, r=28, h=4
\> -1
              .-------------(28)------------------.                   
      .-----(24)------.               .---------(38)--.           
  .-(22)--.       .-(26)--.       .-(30)------.     (39)--.   
(21)    (23)    (25)    (27)    (29)      .-(37)        (40)
                                        (36)
\> 35
Insert 35, rotR(37), r=28, h=4
\> 34
Insert 34, rotRL(30), r=28, h=4
\> 33
Insert 33, rotR(38), r=28, h=4
\> 32
Insert 32, rotR(34), r=28, h=4
\> 31
Insert 31, rotRL(30), r=28, h=4
\> -1
              .-------------(28)--------------------------.                           
      .-----(24)------.                       .---------(35)----------.           
  .-(22)--.       .-(26)--.           .-----(32)--.           .-----(38)--.       
(21)    (23)    (25)    (27)      .-(30)--.     (33)--.     (36)--.     (39)--.   
                                (29)    (31)        (34)        (37)        (40)
\> 27
Delete 27, bez rot, r=28, h=4
\> 24
Delete 24, 24<->25, bez rot, r=28, h=4
\> 26
Delete 26, rotR(25), r=28, h=4
\> 21
Delete 21, rotRL(22), rotL(28), r=35, h=4
\> 35
Delete 35, 35<->36, bez rot, r=36, h=4
\> -1
              .-------------------------(36)------.                           
      .-----(28)--------------.               .-(38)--.   
  .-(23)--.           .-----(32)--.         (37)    (39)--.   
(22)    (25)      .-(30)--.     (33)--.                 (40)
                (29)    (31)        (34)
\> -2
````
