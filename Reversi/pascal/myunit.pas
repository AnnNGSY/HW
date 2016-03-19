unit myunit;
interface
uses crt;

procedure clrwindowscr(x1,y1,x2,y2:integer);
procedure appleani(x,y:integer);
procedure clearbkgr(color : byte);
procedure makewindow(x1, y1, x2, y2, color : byte);
procedure writexy(x, y,color : byte ; mess : string);
procedure begining(level:char);
function  ucase(st:string):string;
procedure makefrom(level:char);
procedure rule;

implementation

procedure clrwindowscr(x1,y1,x2,y2:integer);
var i,x,y:integer;
begin
   gotoxy(x1,y1);
   x:=x2-x1;
   y:=y2-y1;
   for i:= 1 to y do
      begin
      gotoxy(x1,y1+i);
      writeln('':x);
      end;
end;

procedure appleani(x,y:integer);
var f:text;
    animation:string;
    count:integer;
begin
   assign(f,'appleani.txt');{lost the ascii art txt......}
   count:=0;
   reset(f);
   while not eof(f) do
      begin
         count:=count+1;
         readln(f,animation);
         gotoxy(x,y+count mod 11-1);
         writeln(animation);
         if count mod 11=0 then
         begin
            delay(200);
            clrwindowscr(x,y,16+x,y+10);
         end;
      end;
   close(f);
end;

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
procedure makefrom(level:char);
var lv,i,j:integer;
begin
   cursoroff;
   case level of
      '1':lv:=6;
      '2':lv:=7;
      '3':lv:=8;
      '4':lv:=8;
   end;
   if lv=8 then
      for i:=1 to lv do
         for j:=1 to lv do
            makewindow(i*4-3,j*3-2,i*4,j*3,green);
   if lv=7 then
      for i:=2 to lv do
         for j:=2 to lv do
            makewindow(i*4-3,j*3-2,i*4,j*3,green);
   if lv=6 then
      for i:=3 to lv do
         for j:=3 to lv do
            makewindow(i*4-3,j*3-2,i*4,j*3,green);
   
   sound(1);
end;

procedure rule;
begin
   makewindow(35,1,75,10,yellow);
   writexy(36,1,yellow,'rule');
   writexy (36,3,yellow,'1.Please use ''Up'',''Down'',''Left''');
   writexy(38,4,yellow,'and ''Right'' button on the keybroad');
   writexy (38,5,yellow,'to choose position. ');
   writexy(36,6,yellow,'2.Please press ''Enter'' to place symbol.');
   writexy (36,7,yellow,'3.You are not allowed put your symbol');
   writexy(38,8,yellow,'in the same position twice.');
end;

procedure begining(level:char);
var i:integer;
begin
   makefrom(level);
   i:=1+i;
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
