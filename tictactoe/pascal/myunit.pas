unit myunit;
interface
uses crt;

procedure clearbkgr(color : byte);
procedure makewindow(x1, y1, x2, y2, color : byte);
procedure writexy(x, y,color : byte ; mess : string);
procedure begining;
procedure X(x,y:integer);
procedure O(x,y:integer);
function  ucase(st:string):string;

implementation

procedure clearbkgr(color : byte);
begin
  cursoroff;
  textbackground(color);
  clrscr;
end;

{ Invisible to unit user, this is internal function }
function fill(s : string; count : byte; what: char):string;
begin
  fillchar(s,count+1,what);
  s[0]:=chr(count);
  fill:=s;
end;

procedure makewindow(x1, y1, x2, y2, color : byte);
var
  s  : string;
  i  : byte;
begin
  cursoroff;
  textattr:=color;
  s:=fill(s,x2-x1-1,#205);
  gotoxy(x1,y1); write(#201+s+#187);
  gotoxy(x1,y2); write(#200+s+#188);
  s:=fill(s,x2-x1-1,' ');
  s:=#186+s+#186;
  for i:=1 to y2-y1-1 do
  begin
    gotoxy(x1,y1+i); write(s);
  end;
end;
procedure makefrom;
begin
   cursoroff;
   makewindow(1,1,10,7,green);
   makewindow(11,1,20,7,green);
   makewindow(21,1,30,7,green);
   makewindow(1,8,10,14,green);
   makewindow(11,8,20,14,green);
   makewindow(21,8,30,14,green);
   makewindow(1,15,10,21,green);
   makewindow(11,15,20,21,green);
   makewindow(21,15,30,21,green);
end;

procedure X(x,y:integer);
var i:integer;
begin
   cursoroff;
   textcolor(lightred);
   for i := 0 to 4 do
      begin
         gotoxy(x,y+i);
         case i  of
            0: write(' X    X ');
            1: write('  X  X  ');
            2: write('   XX   ');
            3: write('  X  X  ');
            4: write(' X    X ');
         end;
      end;
end;

procedure O(x,y:integer);
var i:integer;
begin
   cursoroff;
   textcolor(white);
   for i := 0 to 4 do
      begin
         gotoxy(x,y+i);
         if i mod 4 = 0 then
            write('  OOOO  ')
         else
            write(' O    O ');
      end;
end;

procedure rule;
begin
   makewindow(31,1,75,10,yellow);
   writexy(32,1,yellow,'rule');
   writexy (32,3,yellow,'1.The numbers in each box');
   writexy(34,4,yellow,'represents its position.');
   writexy (32,5,yellow,'2.Please enter the number');
   writexy(34,6,yellow,'to place your symbol.');
   writexy (32,7,yellow,'3.You are not allowed put your symbol');
   writexy(34,8,yellow,'in the same position twice.');
end;

procedure begining;
var i:integer;
begin
   makefrom;
       for i:= 2 to 6 do
          writexy(6,i,lightgreen,'1');
       for i:= 2 to 6 do
          if not odd(i) then
             writexy(14,i,lightgreen,'22222')
          else
             writexy(15+i-i mod 3*3,i,lightgreen,'2');
       for i:= 2 to 6 do
          if not odd(i) then
             writexy(24,i,lightgreen,'33333')
          else
             writexy(28,i,lightgreen,'3');
       for i:= 9 to 13 do
          begin
             writexy(8,i,lightgreen,'4');
             writexy(i-5,11,lightgreen,'4');
             if i <11 then
                writexy(4,i,lightgreen,'4');
          end;
       for i:= 2 to 6 do
          if not odd(i) then
             writexy(14,i+7,lightgreen,'55555')
          else
             writexy(14+i mod 3 *2,7+i,lightgreen,'5');
       for i:= 9 to 13 do
          if odd(i) then
             writexy(24,i,lightgreen,'66666');
       writexy(24,10,lightgreen,'6');
       writexy(24,12,lightgreen,'6   6');
       for i:=0 to 4 do
          begin
             writexy(4+i,16,lightgreen,'7');
             writexy(8,16+i,lightgreen,'7');
          end;
       for i:= 16 to 20 do
          if not odd(i) then
             writexy(14,i,lightgreen,'88888')
          else
             writexy(14,i,lightgreen,'8   8');
       for i:= 16 to 20 do
          if not odd(i) then
             writexy(24,i,lightgreen,'99999');
       writexy(24,17,lightgreen,'9   9');
       writexy(28,19,lightgreen,'9');
   rule;
end;

procedure writexy(x, y, color : byte; mess : string);
begin
   cursoroff;
  textcolor(color);
  gotoxy(x,y); write(mess);
end;

function ucase(st:string):string;
var
  i : byte;
begin
  for i:=1 to length(st) do st[i]:=upcase(st[i]);
  ucase:=st;
end;

begin
  { Leave this empty if you don't have any initialization code }
end.
