program Reversi;{AppleChess}
uses crt,myunit;{wincrt}
type fromtype = record
                rec:char;
                symbol:integer;
                xpos:integer;
                ypos:integer;
                end;
     fromarr = array[1..8,1..8] of fromtype;
var play,pos,key:char;
    level,nor,nog:integer;
    game:boolean;
    i,j,count,winner,x,y:integer;
    player:array[1..2] of string;
    from:fromarr;
    barr:array[1..8]of boolean;

function checkvaild(x1,y1,count:integer):boolean;
var i,j,x,y:integer;
    a:boolean;
begin
   x:=x1 div 4+1;
   y:=y1 div 3+1;
   checkvaild:=false;
   if from[x,y].symbol=0 then
   begin
      if (x>1) and (y>1) then
         if (from[x-1,y-1].symbol)=((count+1) mod 2+1) then
            begin
               a:=true;
               i:=x-1;
               j:=y-1;
               while (i>1) and (j>1)and a do
                  begin
                     i:=i-1;
                     j:=j-1;
                     if from[i,j].symbol=(count mod 2+1)then
                        a:=false;
                  end;
               if a=false then
                  begin
                     checkvaild:=true;
                     barr[1]:=true;
                  end;
            end;
      if x>1 then
         if (from[x-1,y].symbol)=((count+1) mod 2+1) then
            begin
               a:=true;
               i:=x-1;
               while (i>1)and a do
                  begin
                     i:=i-1;
                     if from[i,y].symbol=(count mod 2+1)then
                        a:=false;
                  end;
               if a=false then
                  begin
                     checkvaild:=true;
                     barr[4]:=true;
                  end;
            end;
      if (x>1) and (y<8) then
         if (from[x-1,y+1].symbol)=((count+1) mod 2+1) then
            begin
               a:=true;
               i:=x-1;
               j:=y+1;
               while (i>1) and (j<8)and a do
                  begin
                     i:=i-1;
                     j:=j+1;
                     if from[i,j].symbol=(count mod 2+1)then
                        a:=false;
                  end;
               if a=false then
                  begin
                     checkvaild:=true;
                     barr[6]:=true;
                  end;
            end;
      if x<8 then
         if (from[x+1,y].symbol)=((count+1) mod 2+1) then
            begin
               a:=true;
               i:=x+1;
               while (i<8)and a do
                  begin
                     i:=i+1;
                     if from[i,y].symbol=(count mod 2+1)then
                        a:=false;
                  end;
               if a=false then
                  begin
                     checkvaild:=true;
                     barr[5]:=true;
                  end;
            end;
      if (x<8) and (y<8) then
         if (from[x+1,y+1].symbol)=((count+1) mod 2+1) then
            begin
               a:=true;
               i:=x+1;
               j:=y+1;
               while (i<8) and (j<8)and a do
                  begin
                     i:=i+1;
                     j:=j+1;
                     if from[i,j].symbol=(count mod 2+1)then
                        a:=false;
                  end;
               if a=false then
                  begin
                     checkvaild:=true;
                     barr[8]:=true;
                  end;
            end;
      if (x<8) and (y>1) then
         if (from[x+1,y-1].symbol)=((count+1) mod 2+1) then
            begin
               a:=true;
               i:=x+1;
               j:=y-1;
               while (i<8) and (j>1)and a do
                  begin
                     i:=i+1;
                     j:=j-1;
                     if from[i,j].symbol=(count mod 2+1)then
                        a:=false;
                  end;
               if a=false then
                  begin
                     checkvaild:=true;
                     barr[3]:=true;
                  end;
            end;
      if y>1 then
         if (from[x,y-1].symbol)=((count+1) mod 2+1) then
            begin
               a:=true;
               j:=y-1;
               while (j>1)and a do
                  begin
                     j:=j-1;
                     if from[x,j].symbol=(count mod 2+1)then
                        a:=false;
                  end;
               if a=false then
                  begin
                     checkvaild:=true;
                     barr[2]:=true;
                  end;
            end;
      if y<8 then
         if (from[x,y+1].symbol)=((count+1) mod 2+1) then
            begin
               a:=true;
               j:=y+1;
               while (j<8)and a do
                  begin
                     j:=j+1;
                     if from[x,j].symbol=(count mod 2+1)then
                        a:=false;
                  end;
               if a=false then
                  begin
                     checkvaild:=true;
                     barr[7]:=true;
                  end;
            end;
   end;
end;
procedure apple(x,y,count:integer);
begin
   if odd(count) then
      begin
         writexy(from[x,y].xpos,from[x,y].ypos,lightred,'O');
         nor:=nor+1;
      end
   else
      begin
         writexy(from[x,y].xpos,from[x,y].ypos,lightgreen,'O');
         nog:=nog+1;
      end;
end;
procedure gamebody(mode:char);
var count,i,j,x,y,xlim1,xlim2,ylim1,ylim2:integer;
    next,go,b,c,d:boolean;
    ppos:char;
begin
   b:=false;
   count:=0;
   repeat
      for i:=1 to 8 do
         barr[i]:=false;
      count:=count+1;
      go:= false;
      writexy (36,3,yellow,player[1]);
      gotoxy(36,4);
      write(nor:10);
      writexy (36,6,yellow,player[2]);
      gotoxy(36,7);
      write(nog:10);
      for i:=1 to 8 do
         for j:= 1 to 8 do
            if checkvaild((i-1)*4,(j-1)*3,count) then
               go:=true;
      if not go then
         begin
            writexy(36,16,lightred,'Sorry,no legal move for you.');
            writexy(36,17,lightred,'You will skip this turn.');
            writexy(36,18,lightred,'Press enter to continue.');
            for i:=1 to 8 do
               for j:= 1 to 8 do
                  if not checkvaild((i-1)*4,(j-1)*3,(count+1)) then
                     game:=false;
            if game=false then
               begin
                  writexy(36,16,lightred,'Sorry,both players have no legal move.');
                  writexy(36,17,lightred,'The game will stop here.');
                  writexy(36,18,lightred,'Press enter to continue.');
               end;
            readln;
         end;
         for i:=1 to 8 do
         barr[i]:=false;
      clrwindowscr(36,11,75,20);
      if odd(count) then
         writexy(36,12,lightcyan,player[1])
      else
         writexy(36,12,lightcyan,player[2]);
      while go do
      repeat
         next:=false;
         c:=false;
         cursorbig;
         x:=18;
         y:=17;
         case mode of
            '1':begin xlim1:=12; xlim2:=22; ylim1:=9; ylim2:=16; end;
            '2':begin xlim1:=8; xlim2:=26; ylim1:=6; ylim2:=19; end;
            '3':begin xlim1:=4; xlim2:=30; ylim1:=3; ylim2:=22; end;
            '4':begin xlim1:=4; xlim2:=30; ylim1:=3; ylim2:=22; end;
         end;
         gotoxy(x,y);
         repeat
            key:=readkey;
            case ord(key) of
               0:begin
                    key:=readkey;
                    case ord(key) of
                       75:if x>xlim1 then x:=x-4;
                       77:if x<xlim2 then x:=x+4;
                       80:if y<ylim2 then y:=y+3;
                       72:if y>ylim1 then y:=y-3;
                    end;
                 end;
               13:begin
                     if not checkvaild(x,y,count)then
                        begin
                           writexy(36,16,lightred,'Sorry,the position is not vaild.');
                           writexy(36,17,lightred,'Please choose another position.');
                        end
                     else
                        begin
                           c:=true;
                           from[x div 4+1,y div 3+1].symbol:=count mod 2 +1;
                           apple(x div 4+1,y div 3+1,count);
                        end;
                  end;
            end;
         gotoxy(x,y);
       until not c;
   until not c;
   i:=x div 4-1;
   j:=y div 3-1;
   if barr[1] then
   repeat
      i:=i-1;
      j:=j-1;
      apple(i,j,count);
   until from[i-1,j-1].symbol=(count mod 2+1);
   i:=x div 4-1;
   j:=y div 3-1;
   if barr[2] then
   repeat
      j:=j-1;
      apple(i,j,count);
   until from[i,j-1].symbol=(count mod 2+1);
   i:=x div 4-1;
   j:=y div 3-1;
   if barr[3] then
   repeat
      i:=i+1;
      j:=j-1;
      apple(i,j,count);
   until from[i+1,j-1].symbol=(count mod 2+1);
   i:=x div 4-1;
   j:=y div 3-1;
   if barr[4] then
   repeat
      i:=i-1;
      apple(i,j,count);
   until from[i-1,j].symbol=(count mod 2+1);
   i:=x div 4-1;
   j:=y div 3-1;
   if barr[5] then
   repeat
      i:=i+1;
      apple(i,j,count);
   until from[i+1,j].symbol=(count mod 2+1);
   i:=x div 4-1;
   j:=y div 3-1;
   if barr[6] then
   repeat
      i:=i-1;
      j:=j+1;
      apple(i,j,count);
   until from[i-1,j+1].symbol=(count mod 2+1);
   i:=x div 4-1;
   j:=y div 3-1;
   if barr[7] then
   repeat
      j:=j+1;
      apple(i,j,count);
   until from[i,j+1].symbol=(count mod 2+1);
   i:=x div 4-1;
   j:=y div 3-1;
   if barr[8] then
   repeat
      i:=i+1;
      j:=j+1;
      apple(i,j,count);
   until from[i+1,j+1].symbol=(count mod 2+1);
   until game=false;
end;

begin
     cursoroff;
     textcolor(lightgreen);
    { appleani(3,5);}//ascii art txt required
     writexy(22,10,yellow,'>> Welcome to Apple Chess program. <<');
     textcolor(lightred);
     {appleani(60,5);}//ascii art txt required
     delay(500);
   repeat
     game:=true;
     count:=0;
     nor:=2;
     nog:=2;
       for j:=1 to 8 do
          for i:=1 to 8 do
             begin
                count:=count+1;
                from[i,j].xpos:=i*4-2;
                from[i,j].ypos:=j*3-1;
                from[i,j].symbol:=0;
                from[i,j].rec:=chr(ord('0')+count);
             end;
       clrscr;
       makewindow(35,11,75,21,cyan);
       rule;
   repeat
       writexy(36,12,lightcyan,'Please choose mode:');
       writexy(36,13,lightcyan,'1. Two players (4*4)');
       writexy(36,14,lightcyan,'2. Two players (6*6)');
       writexy(36,15,lightcyan,'3. Two players (8*8) ');
       writexy(36,16,lightcyan,'4. Play with computer (8*8) ');
       gotoxy(36,17);
       cursoron;
       readln(play);
       cursoroff;
       if (play<'1') or (play>'4') then
          writexy(36,18,lightred,'Wrong enter. Please enter again.');
       writexy(36,17,black,'                                     ');
   until (play='1')or(play='2')or(play='3')or(play='4');
     clrwindowscr(36,11,75,20);
     cursoroff;
     makefrom(play);
       from[5,4].symbol:=2;
       from[4,4].symbol:=1;
       from[4,5].symbol:=2;
       from[5,5].symbol:=1;
       writexy(from[5,4].xpos,from[5,4].ypos,lightred,'O');
       writexy(from[4,4].xpos,from[4,4].ypos,lightgreen,'O');
       writexy(from[4,5].xpos,from[4,5].ypos,lightred,'O');
       writexy(from[5,5].xpos,from[5,5].ypos,lightgreen,'O');
       if play<>'4'then
          begin
             player[1]:='Player1';
             player[2]:='Player2';
          end
       else
          begin
             player[1]:='Player';
             player[2]:='Computer';
          end;
       clrwindowscr(36,1,75,9);
       writexy(36,1,yellow,'Players');
       gamebody(play);
       makewindow(1,22,75,25,magenta);
       writexy(2,23,lightmagenta,'Do you want to play again? (Y/N)');
       cursoron;
       readln(play);
    until ucase(play)='N';
       cursoroff;
cursoroff;
end.
