{
Description: Tic Tac Toe
}

Program TIC_TAC_TOE;
Uses Graph;

Const
Directory='C:TPBGI';
Cross=3;
Zero=4;
Blank=2;

Type
Myarray=Array[1..3,1..3] Of Integer;
Array1=Array[1..3,1..3] Of Char;

Var

Driver :Integer;
Mode :Integer;
A1 :Myarray;
Ar1 :Array1;
X1 :Integer;
N :Integer;
A :Integer;
Y :Integer;
Z :Integer;
Success :Boolean;
Position :Integer;
I :Integer;
Choice :Integer;

{************************************************************}

Procedure
Drawboard;

Begin

Initgraph(Driver,Mode,Directory);

Setbkcolor(DarkGray);

Setcolor (Blue);

Cleardevice ;

Line(225,25,225,300);
Line(400,25,400,300);
Line(100,125,500,125);
Line(100,200,500,200);

End;

{************************************************************}

Procedure
Check;

Begin

Success:=False;

If ((A1[1,1]*A1[2,2]*A1[3,3])=64) Then
Begin
Success:=True;
I:=1;
End;


If ((A1[1,3]*A1[2,2]*A1[3,1])=64) Then
Begin
Success:=True;
I:=1;
End;

If ((A1[1,1]*A1[2,2]*A1[3,3])=27) Then
Begin
Success:=True;
I:= 2;
End;

If ((A1[1,3]*A1[2,2]*A1[3,1])=27) Then
Begin
Success:=True;
I:=2;
End;


If ((A1[1,1]*A1[1,2]*A1[1,3])=64) or ((A1[2,1]*A1[2,2]*A1[2,3])=64) Or ((A1[3,1]*A1[3,2]*A1[3,3]=64)) Then
Begin
Success:=True;
I:=1;
End;


If ((A1[1,1]*A1[1,2]*A1[1,3]=27))Or ((A1[2,1]*A1[2,2]*A1[2,3]=27))Or ((A1[3,1]*A1[3,2]*A1[3,3]=27)) Then
Begin
Success:=True;
I:= 2;
End;


If ((A1[1,1]*A1[2,1]*A1[3,1]=64)) or ((A1[1,2]*A1[2,2]*A1[3,2]=64)) Or ((A1[1,3]*A1[2,3]*A1[3,3]=64)) Then
Begin
Success:=True;
I:=1;
End;

If ((A1[1,1]*A1[2,1]*A1[3,1]=27)) or ((A1[1,2]*A1[2,2]*A1[3,2]=27)) or ((A1[1,3]*A1[2,3]*A1[3,3]=27))Then
Begin
Success:=True;
I:=2;
End;


End;

{************************************************************}

Procedure
Check2;

Begin

Success:=False;

If (Ar1[1,1]='#') And(Ar1[2,2]='#') and (Ar1[3,3]='#') Then
Begin
Success:=True;
I:=1;
End ;


If (Ar1[1,3]='#') And (Ar1[2,2]='#') And (Ar1[3,1]='#') Then
Begin
Success:=True;
I:=1;
End ;


If (Ar1[1,1] ='*') and (Ar1[2,2]='*') And (Ar1[3,3]='*') Then
Begin
Success:=True;
I:=2;
End ;

If (Ar1[1,3]='*') And (Ar1[2,2]='*') And (Ar1[3,1]='*') Then
Begin
Success:=True;
I:=2;
End ;

If ((Ar1[1,1]='#') And (Ar1[1,2]='#') And (Ar1[1,3]='#')) or ((Ar1[2,1]='#') And (Ar1[2,2]='#') And (Ar1[2,3]='#'))
Or ((Ar1[3,1]='#') And (Ar1[3,2]='#') And (Ar1[3,3]='#')) Then
Begin
Success:=True;
I:=1;
End ;


If ((Ar1[1,1]='*') And (Ar1[1,2]='*') And (Ar1[1,3]='*')) or ((Ar1[2,1]='*') And (Ar1[2,2]='*') And (Ar1[2,3]='*'))
Or ((Ar1[3,1]='*') And (Ar1[3,2]='*') And (Ar1[3,3]='*')) Then
Begin
Success:=True;
I:=2;
End ;

If ((Ar1[1,1]='#') And (Ar1[2,1]='#') And (Ar1[3,1]='#')) or ((Ar1[1,2]='#') And (Ar1[2,2]='#') And (Ar1[3,2]='#'))
Or ((Ar1[1,3]='#') And (Ar1[2,3]='#') And (Ar1[3,3]='#')) Then
Begin
Success:=True;
I:=1;
End ;


If ((Ar1[1,1]='*') And (Ar1[2,1]='*') And (Ar1[3,1]='*')) or ((Ar1[1,2]='*') And (Ar1[2,2]='*') And (Ar1[3,2]='*'))
Or ((Ar1[1,3]='*') And (Ar1[2,3]='*') And (Ar1[3,3]='*')) Then
Begin


Success:=True;
I:=2;
End ;

End;

{************************************************************}

Procedure
UserGuide;

Begin
Writeln;
Writeln (' ','HOW TO PLAY TIC TAC TOE');
Writeln (' ','=======================');
Writeln;
Writeln ('This Game Is Based On The Following Rules');
Writeln;
Write ('1. The Numbers In Each Box Represents Its Position.');
Write ('To Play Press Your Desired Number From The Keyboard to Place Your Symbol In That Box.');
Write ('And Then Press Enter.');
Writeln;
Writeln;
Write ('2. No User Is Allowed To Take More Than One Turn Or Use The Same Position Twice');
Write (' If He Does So His Turn Will Be Cancelled.');
Writeln;

End;

{************************************************************}

Procedure
Display (Ar :Array1);

Begin
Drawboard;


For X1:=1 to 3 do
Begin
Writeln;
Writeln;
Writeln;

For Y:=1 to 3 do
Write (' ',Ar[X1,Y]);

Writeln;
Writeln;
Writeln;
End;

End;

{************************************************************}

Procedure
Twoplayer;

Begin
A:=49;
For X1:= 1 to 3 do

For Y:=1 to 3 do
Begin
Ar1[X1,Y]:=Chr(A);
A:=A+1;
End;

Display (Ar1);

Writeln ('Player 1 must play with #');
Writeln('Player 2 must play with *');

Repeat


Writeln ('Enter Position');
Readln (position);

Case Position of
1: Begin
If (Ar1[1,1]='#') or (Ar1[1,1]='*') Then
Writeln('The Position is Already Occupied')
Else
Ar1[1,1]:='*';
End;

2: Begin
If (AR1[1,2]='#') or (Ar1[1,2]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[1,2]:='*';
End;

3: Begin
If (Ar1[1,3]='#') or (Ar1[1,3]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[1,3]:='*';
End;

4: Begin
If (Ar1[2,1]='#') or (Ar1[2,1]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[2,1]:='*';
End;

5: Begin
If (Ar1[2,2]='#') or (Ar1[2,2]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[2,2]:='*';
End;

6: Begin
If (Ar1[2,3]='#') or (Ar1[2,3]='*' )THen
Writeln('The Position is Already Occupied')
Else
Ar1[2,3]:='*';
End;

7: Begin
If (Ar1[3,1]='#') or (Ar1[3,1]='*')THen
Writeln('The Position is Already Occupied')
Else
Ar1[3,1]:='*';
End;

8: Begin
If (Ar1[3,2]='#') or (Ar1[3,2]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[3,2]:='*';
End;

9: Begin
If (Ar1[3,3]='#') or (Ar1[3,3]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[3,3]:='*';
End;

End;

Drawboard;

Display (Ar1);

Check2;

For X1:= 1 to 3 do
For Y :=1 to 3 do
Begin
If (Ar1[X1,Y]='#') Or (Ar1[X1,Y]='*') Then
N:=N+1;
End;
If N=9 Then
Success:=True;
N:=0;

If Success=True Then
Exit;

Writeln ('Enter Position');
Readln (position);

Case Position of
1: Begin
If (Ar1[1,1]='#') or (Ar1[1,1]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[1,1]:='#';
End;

2: Begin
If (AR1[1,2]='#') or (Ar1[1,2]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[1,2]:='#';
End;

3: Begin
If (Ar1[1,3]='#') or (Ar1[1,3]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[1,3]:='#';
End;

4: Begin
If (Ar1[2,1]='#') or (Ar1[2,1]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[2,1]:='#';
End;

5: Begin
If (Ar1[2,2]='#') or (Ar1[2,2]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[2,2]:='#';
End;

6: Begin
If (Ar1[2,3]='#') or (Ar1[2,3]='*') THen
Writeln('The Position is Already Occupied')
Else
Ar1[2,3]:='#';
End;

7: Begin
If (Ar1[3,1]='#') or (Ar1[3,1]='*')THen
Writeln('The Position is Already Occupied')
Else
Ar1[3,1]:='#';
End;

8: Begin
If (Ar1[3,2]='#') or (Ar1[3,2]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[3,2]:='#';
End;

9: Begin
If (Ar1[3,3]='#') or (Ar1[3,3]='*')Then
Writeln('The Position is Already Occupied')
Else
Ar1[3,3]:='#';
End;

End;

Display (Ar1);

Check2;

For X1:= 1 to 3 do
For Y :=1 to 3 do
Begin
If (Ar1[X1,Y]='#') Or (Ar1[X1,Y]='*') Then
N:=N+1;
End;
If N=9 Then
Success:=True;
N:=0;

Until (success=True);

End;

{************************************************************}

Procedure
Singleplayer;

Begin

For X1:=1 to 3 do
Begin
For Y:=1 to 3 do
A1[X1,Y]:=Blank;
End;

A:=49;
For X1:=1 to 3 do
Begin
For Y:=1 to 3 do
Begin
Ar1[X1,Y]:=Chr (A) ;
A:=A+1;
End;
End;

Drawboard;

Display (Ar1);

Writeln ('Your Symbol Is *');

Repeat
Writeln ('Enter Position');
Readln (position);

Case Position of
1: Begin
If (Ar1[1,1]='#') or (Ar1[1,1]='*') then
Writeln('This position has been occupied')
Else
Begin
Ar1[1,1]:='*';
If Ar1[1,1]='*' Then
A1[1,1]:=3;
End;

End;

2: Begin
If (Ar1[1,2]='#') or (Ar1[1,2]='*') then
Writeln('This position has been occupied')
Else
Begin
Ar1[1,2]:='*';
If Ar1[1,2]='*' Then
A1[1,2]:=3;
End;
End;

3: Begin
If (Ar1[1,3]='#') or (Ar1[1,3]='*') then
Writeln('This position has been occupied')
Else
Begin
Ar1[1,3]:='*';
If Ar1[1,3]='*' Then
A1[1,3]:=3;
End;
End;

4: Begin
If (Ar1[2,1]='#') or (Ar1[2,1]='*') then
Writeln('This position has been occupied')
Else
Begin
Ar1[2,1]:='*';
If Ar1[2,1]='*' Then
A1[2,1]:=3;
End;
End;

5: Begin
If (Ar1[2,2]='#') or (Ar1[2,2]='*') then
Writeln('This position has been occupied')
Else
Begin
Ar1[2,2]:='*';
If Ar1[2,2]='*' Then
A1[2,2]:=3;
End;
End;

6: Begin
If (Ar1[2,3]='#') or (Ar1[2,3]='*') then
Writeln('This position has been occupied')
Else
Begin
Ar1[2,3]:='*';
If Ar1[2,3]='*' Then
A1[2,3]:=3;
End;
End;

7: Begin
If (Ar1[3,1]='#') or (Ar1[3,1]='*') then
Writeln('This position has been occupied')
Else
Begin
Ar1[3,1]:='*';
If Ar1[3,1]='*' Then
A1[3,1]:=3;
End;
End;

8: Begin
If (Ar1[3,2]='#') or (Ar1[3,2]='*') then
Writeln('This position has been occupied')
Else
Begin
Ar1[3,2]:='*';
If Ar1[3,2]='*' Then
A1[3,2]:=3;
End;
End;

9: Begin
If (Ar1[3,3]='#') or (Ar1[3,3]='*') then
Writeln('This position has been occupied')
Else
Begin
Ar1[3,3]:='*';
If Ar1[3,3]='*' Then
A1[3,3]:=3;
End;

End;

End;

Check;

If Success=True Then
Begin
Exit;
End;

If ((A1[1,1])*(A1[1,2])*(A1[1,3]))=32 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero;
Ar1[1,1]:='#';
End
Else
Begin
If A1[1,2]=Blank Then
Begin
A1[1,2]:=Zero;
Ar1[1,2]:='#';
End
Else
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero;
Ar1[1,3]:='#';
End;
End;
End;
End

Else
Begin
If ((A1[2,1])*(A1[2,2])*(A1[2,3]))=32 Then
Begin
If A1[2,1]=Blank Then
Begin
A1[2,1]:=Zero;
Ar1[2,1]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero ;
Ar1[2,2]:='#';
End
Else
Begin
If A1[2,3]=Blank Then
Begin
A1[2,3]:=Zero;
Ar1[2,3]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[3,1])*(A1[3,2])*(A1[3,3]))=32 Then
Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero;
Ar1[3,1]:='#';
End
Else
Begin
If A1[3,2]=Blank Then
Begin
A1[3,2]:=Zero;
Ar1[3,2]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;
End

Else
Begin
If ((A1[1,1])*(A1[2,1])*(A1[3,1]))=32 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero;
Ar1[1,1]:='#';
End
Else
Begin
If A1[2,1]=Blank Then
Begin
A1[2,1]:=Zero ;
Ar1[2,1]:='#';
End
Else
Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero;
Ar1[3,1]:='#';
End;
End;
End;
End

Else
Begin
If ((A1[1,2])*(A1[2,2])*(A1[3,2]))=32 Then
Begin
If A1[1,2]=Blank Then
Begin
A1[1,2]:=Zero;
Ar1[1,2]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero;
Ar1[2,2]:='#';
End
Else
Begin
If A1[3,2]=Blank Then
Begin
A1[3,2]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;
End

Else
Begin
If ((A1[1,3])*(A1[2,3])*(A1[3,3]))=32 Then
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero ;
Ar1[1,3]:='#';
End
Else
Begin
If A1[2,3]=Blank Then
Begin
A1[2,3]:=Zero;
Ar1[2,3]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;
End

Else
Begin
If ((A1[1,1])*(A1[2,2])*(A1[3,3]))=32 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero;
Ar1[1,1]:='#' ;
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero;
Ar1[2,2]:='#' ;
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;
End

Else
Begin
If ((A1[1,3])*(A1[2,2])*(A1[3,1]))=32 Then
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero;
Ar1[1,3]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero ;
Ar1[2,2]:='#' ;
End
Else
Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero;
Ar1[3,1]:='#';
End;
End;
End;
End

Else
Begin
If ((A1[1,1])*(A1[1,2])*(A1[1,3]))=18 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero;
Ar1[1,1]:='#';
End

Else
Begin
If A1[1,2]=Blank Then
Begin
A1[1,2]:=Zero ;
Ar1[1,2]:='#';
End
Else
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero;
Ar1[1,3]:='#';
End;
End;
End;
End
Else
Begin

If ((A1[2,1])*(A1[2,2])*(A1[2,3]))=18 Then

Begin
If A1[2,1]=Blank Then
Begin
A1[2,1]:=Zero ;
Ar1[2,1]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero ;
Ar1[2,2]:='#';
End
Else
Begin
If A1[2,3]=Blank Then
Begin
A1[2,3]:=Zero;
Ar1[2,3]:='#';
End;
End;
End;
End
Else
Begin

If ((A1[3,1])*(A1[3,2])*(A1[3,3]))=18 Then

Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero ;
Ar1[3,1]:='#';
End
Else
Begin
If A1[3,2]=Blank Then
Begin
A1[3,2]:=Zero ;
Ar1[3,2]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,1])*(A1[2,1])*(A1[3,1]))=18 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero ;
Ar1[1,1]:='#';
End
Else
Begin
If A1[2,1]=Blank Then
Begin
A1[2,1]:=Zero ;
Ar1[2,1]:='#';
End
Else
Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero;
Ar1[3,1]:='#';
End;
End;
End;
End

Else
Begin
If ((A1[1,2])*(A1[2,2])*(A1[3,2]))=18 Then
Begin
If A1[1,2]=Blank Then
Begin
A1[1,2]:=Zero;
Ar1[1,2]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero;
Ar1[2,2]:='#';
End
Else
Begin
If A1[3,2]=Blank Then
Begin
A1[3,2]:=Zero;
Ar1[3,2]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,3])*(A1[2,3])*(A1[3,3]))=18 Then
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero;
Ar1[1,3]:='#';
End
Else
Begin
If A1[2,3]=Blank Then
Begin
A1[2,3]:=Zero;
Ar1[2,3]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;

End
Else
Begin
If ((A1[1,1])*(A1[2,2])*(A1[3,3]))=18 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero;
Ar1[1,1]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero;
Ar1[2,2]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End
End;
End;
End
Else
Begin
If ((A1[1,3])*(A1[2,2])*(A1[3,1]))=18 Then
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero;
Ar1[1,3]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero ;
Ar1[2,2]:='#';
End
Else
Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero;
Ar1[3,1]:='#';
End
End;
End;
End
Else
Begin

If ((A1[1,1])*(A1[1,2])*(A1[1,3]))=16 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero ;
Ar1[1,1]:='#';
End
Else
Begin
If A1[1,2]=Blank Then
Begin
A1[1,2]:=Zero;
Ar1[1,2]:='#';
End
Else
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero;
Ar1[1,3]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[2,1])*(A1[2,2])*(A1[2,3]))=16 Then
Begin
If A1[2,1]=Blank Then
Begin
A1[2,1]:=Zero;
Ar1[2,1]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero ;
Ar1[2,2]:='#';
End
Else
Begin
If A1[2,3]=Blank Then
Begin
A1[2,3]:=Zero;
Ar1[2,3]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[3,1])*(A1[3,2])*(A1[3,3]))=16 Then
Begin
If A1[3,1]=Blank Then
Begin
Ar1[3,1]:='#';
A1[3,1]:=Zero
End
Else
Begin
If A1[3,2]=Blank Then
Begin
A1[3,2]:=Zero ;
Ar1[3,2]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End
End;
End;
End

Else
Begin
If ((A1[1,1])*(A1[2,1])*(A1[3,1]))=16 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero;
Ar1[1,1]:='#';
End
Else
Begin
If A1[2,1]=Blank Then
Begin
A1[2,1]:=Zero ;
Ar1[2,1]:='#';
End
Else
Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero;
Ar1[3,1]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,2])*(A1[2,2])*(A1[3,2]))=16 Then
Begin
If A1[1,2]=Blank Then
Begin
A1[1,2]:=Zero ;
Ar1[1,2]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero ;
Ar1[2,2]:='#';
End
Else
Begin
If A1[3,2]=Blank Then
Begin
A1[3,2]:=Zero;
Ar1[3,2]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,3])*(A1[2,3])*(A1[3,3]))=16 Then
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero;
Ar1[1,3]:='#';
End
Else
Begin
If A1[2,3]=Blank Then
Begin
A1[2,3]:=Zero ;
Ar1[2,3]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,1])*(A1[2,2])*(A1[3,3]))=16 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero ;
Ar1[1,1]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero ;
Ar1[2,2]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,3])*(A1[2,2])*(A1[3,1]))=16 Then
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero;
Ar1[1,3]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero;
Ar1[2,2]:='#';
End
Else
Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero;
Ar1[3,1]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,1])*(A1[1,2])*(A1[1,3]))=8 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero ;
Ar1[1,1]:='#';
End
Else
Begin
If A1[1,2]=Blank Then
Begin
A1[1,2]:=Zero ;
Ar1[1,2]:='#';
End
Else
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero;
Ar1[1,3]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[2,1])*(A1[2,2])*(A1[2,3]))=8 Then
Begin
If A1[2,1]=Blank Then
Begin
A1[2,1]:=Zero ;
Ar1[2,1]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero ;
Ar1[2,2]:='#';
End
Else
Begin
If A1[2,3]=Blank Then
Begin
A1[2,3]:=Zero;
Ar1[2,3]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[3,1])*(A1[3,2])*(A1[3,3]))=8 Then
Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero ;
Ar1[3,1]:='#';
End
Else
Begin
If A1[3,2]=Blank Then
Begin
A1[3,2]:=Zero ;
Ar1[3,2]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;
End

Else
Begin
If ((A1[1,1])*(A1[2,1])*(A1[3,1]))=8 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero;
Ar1[1,1]:='#';
End
Else
Begin
If A1[2,1]=Blank Then
Begin
A1[2,1]:=zero;
Ar1[2,1]:='#';
End
Else
Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero;
Ar1[3,1]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,2])*(A1[2,2])*(A1[3,2]))=8 Then
Begin
If A1[1,2]=Blank Then
Begin
A1[1,2]:=Zero;
Ar1[1,2]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero;
Ar1[2,2]:='#';
End
Else
Begin
If A1[3,2]=Blank Then
Begin
A1[3,2]:=Zero;
Ar1[3,2]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,3])*(A1[2,3])*(A1[3,3]))=8 Then
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero ;
Ar1[1,3]:='#';
End
Else
Begin
If A1[2,3]=Blank Then
Begin
A1[2,3]:=Zero ;
Ar1[2,3]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,1])*(A1[2,2])*(A1[3,3]))=8 Then
Begin
If A1[1,1]=Blank Then
Begin
A1[1,1]:=Zero ;
Ar1[1,1]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero;
Ar1[2,2]:='#';
End
Else
Begin
If A1[3,3]=Blank Then
Begin
A1[3,3]:=Zero;
Ar1[3,3]:='#';
End;
End;
End;
End
Else
Begin
If ((A1[1,3])*(A1[2,2])*(A1[3,1]))=8 Then
Begin
If A1[1,3]=Blank Then
Begin
A1[1,3]:=Zero;
Ar1[1,3]:='#';
End
Else
Begin
If A1[2,2]=Blank Then
Begin
A1[2,2]:=Zero;
Ar1[2,2]:='#';
End
Else
Begin
If A1[3,1]=Blank Then
Begin
A1[3,1]:=Zero;
Ar1[3,1]:='#';
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;
End;

Display (Ar1);

Check;

For X1:= 1 to 3 do
For Y :=1 to 3 do
Begin
If (A1[X1,Y]=Zero) Or (A1[X1,Y]=Cross) Then
N:=N+1;

End;

If N=9 Then
Success:=True;

N:=0;

Until (success=True);

End;

{************************************************************}

Begin

Initgraph (Driver,Mode,Directory);
Cleardevice;
Repeat
I:=0;
UserGuide;

Writeln;
Writeln;
Writeln ('Enter 1 For Singleplayer');
Writeln;
Writeln ('************************');
Writeln;
Writeln( 'Enter 2 for Twoplayer');
Writeln;
Writeln ('************************');
Writeln;
Writeln('Enter 3 to Exit');
Writeln;
Writeln ('************************');
Writeln;
Readln (Choice);
Cleardevice;

If Choice=1 Then
Begin

Singleplayer;

If (I=2) Then
Writeln ('You Win');

If (I=1) Then
Writeln ('Computer Wins');

If (I=0) Then
Writeln ('Game Drawn');

End;

If Choice=2 Then
Begin

Twoplayer;

If I=1 Then
Writeln ('Player 1 Wins');

If I=2 Then
Writeln ('Player 2 Wins');

If (I=0) Then
Writeln ('Game Drawn');

End;
Until Choice=3;
End.
