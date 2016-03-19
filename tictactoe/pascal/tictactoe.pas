program Noname1;
uses myunit,crt;
type fromtype = record
          rec:char;
          symbol:integer;
          xpos:integer;
          ypos:integer;
          end;
     fromarr=array [1..3,1..3]of fromtype;
var play,pos:char;
    from:fromarr;
    game:boolean;
    i,j,count,winner:integer;
    player:array[1..2] of string;

procedure clrwindow;
var i:integer;
begin
   for i:=12 to 18 do
      begin
         gotoxy(32,i);
         write('':40);
      end;
end;

procedure checkresult;
var i,j,count:integer;
begin
   for i:=1 to 3 do
      if (from[1,i].symbol*from[2,i].symbol*from[3,i].symbol = 8)or(from[i,1].symbol*from[i,2].symbol*from[i,3].symbol = 8) then
         begin
            winner:=1;
            game:=false;
         end;
      if (from[1,3].symbol*from[2,2].symbol*from[3,1].symbol = 8)or(from[1,1].symbol*from[2,2].symbol*from[3,3].symbol = 8)then
         begin
            winner:=1;
            game:=false;
         end;
   for i:=1 to 3 do
      if (from[1,i].symbol*from[2,i].symbol*from[3,i].symbol = 27)or(from[i,1].symbol*from[i,2].symbol*from[i,3].symbol = 27) then
         begin
            winner:=2;
            game:=false;
         end;
      if (from[1,3].symbol*from[2,2].symbol*from[3,1].symbol = 27)or(from[1,1].symbol*from[2,2].symbol*from[3,3].symbol = 27)then
         begin
            winner:=2;
            game:=false;
         end;
  count:=0;
  for i:=1 to 3 do
     for j:=1 to 3 do
        if from[i,j].symbol <> 1 then
           count:=count+1;
   if (count=9)and(game=true) then
      begin
         game:= false;
         winner:=0;
      end;
   if game=false then
   begin
      clrwindow;
   if winner=0 then
     writexy(32,13,lightcyan,'Tied. No one win this turn.')
   else
      begin
         writexy(32,13,lightcyan,player[winner]);
         writexy(32,14,lightcyan,'Win in this turn.');
      end;
   end;
end;

procedure gamebody(mode:char);
var count,i,j:integer;
    next,go,b:boolean;
    ppos:char;
begin
   b:=false;
   count:=0;
   repeat
      count:=count+1;
      clrwindow;
      if odd(count) then
         writexy(32,12,lightcyan,player[1])
      else
         writexy(32,12,lightcyan,player[2]);
      repeat
         next:=false;
         writexy(32,13,lightcyan,'Please enter the position you want.');
         gotoxy(32,14);
         cursoron;
         if odd(count)or (mode='1') then
            readln(pos)
         else
            begin
               ppos:=pos;
               randomize;
               pos:=chr(49+random(9));
               if mode>'2' then
               repeat
                  randomize;
                  pos:=chr(49+random(9));
                  go:=true;
                  if (mode='4')and (count=2) then
                     case ppos of
                        '1':pos:='5';
                        '3':pos:='5';
                        '5':pos:=chr(49+random(5)*2);
                        '7':pos:='5';
                        '9':pos:='5';
                     end;
                  if (mode='4')and (count=2)and (from[2,2].symbol = 3) then
                     b:=true;
                  if (mode='4')and (count=4) and b then
                     case ppos of
                        '1':pos:=chr(50+random(4)*2);
                        '3':pos:=chr(50+random(4)*2);
                        '7':pos:=chr(50+random(4)*2);
                        '9':pos:=chr(50+random(4)*2);
                     end;
                  for i:=1 to 3 do
                     if (from[1,i].symbol*from[2,i].symbol*from[3,i].symbol = 4) then
                         for j:=1 to 3 do
                             if from[j,i].symbol = 1 then
                                pos:=from[j,i].rec;
                  for i:=1 to 3 do
                     if (from[i,1].symbol*from[i,2].symbol*from[i,3].symbol = 4) then
                         for j:=1 to 3 do
                             if from[i,j].symbol = 1 then
                                pos:=from[i,j].rec;
                  if (from[1,3].symbol*from[2,2].symbol*from[3,1].symbol = 4) then
                      for i:=1 to 3 do
                          for j:= 3 downto 1 do
                              if from[i,j].symbol = 1 then
                                 pos:=from[i,j].rec;
                  if (from[1,1].symbol*from[2,2].symbol*from[3,3].symbol = 4) then
                      for i:=1 to 3 do
                          if from[i,i].symbol = 1 then
                               pos:=from[i,i].rec;
                  for i:=1 to 3 do
                     for j:=1 to 3 do
                        if from[i,j].rec=pos then
                           if from[i,j].symbol<>1 then
                              go:=false;
               until go;
               write(pos);
            end;
         if (pos>='1')and (pos<='9')then
            next:=true
         else
            writexy(32,16,lightred,'Please enter number(1~9).');
      until next;
      for i:=1 to 3 do
         for j:=1 to 3 do
            if from[i,j].rec=pos then
               if from[i,j].symbol<>1 then
                  begin
                     if odd(count)or (mode='1')then
                        begin
                           writexy(32,16,lightred,'Sorry,the position is filled.');
                           writexy(32,17,lightred,'You missed a chance.');
                           writexy(32,18,lightred,'Press enter to let next one play.');
                           textColor(Black);
                           cursoroff;
                           readln;
                        end
                     else
                           writexy(32,16,lightred,'Computer missed a chance.');
                  end
               else
                  if odd(count)then
                     begin
                     X(from[i,j].xpos,from[i,j].ypos);
                     from[i,j].symbol:=2;
                     end
                  else
                     begin
                     O(from[i,j].xpos,from[i,j].ypos);
                     from[i,j].symbol:=3;
                     end;
      checkresult;
      if not odd(count)and game and(mode<>'1') then
         begin
            writexy(32,18,lightred,'Computer finished. Press enter to play.');
            textColor(Black);
            readln;
         end;
   until game=false;
end;

begin
    repeat
       clrscr;
       game:=true;
       count:=0;
       for j:=1 to 3 do
          for i:=1 to 3 do
             begin
                count:=count+1;
                from[i,j].xpos:=i*10-8;
                from[i,j].ypos:=(j-1)* 7 + 2;
                from[i,j].symbol:=1;
                from[i,j].rec:=chr(ord('0')+count);
             end;
       begining;
       makewindow(31,11,75,21,cyan);
repeat
       writexy(32,12,lightcyan,'Please choose mode:');
       writexy(32,13,lightcyan,'1. Two players');
       writexy(32,14,lightcyan,'2. Play with computer (easy) ');
       writexy(32,15,lightcyan,'3. Play with computer (middle) ');
       writexy(32,16,lightcyan,'4. Play with computer (hard) ');
       gotoxy(32,17);
       cursoron;
       readln(play);
       cursoroff;
       if (play<'1') or (play>'4') then
          writexy(32,18,lightred,'Wrong enter. Please enter again.');
       writexy(32,17,black,'                                     ');
until (play='1')or(play='2')or(play='3')or(play='4');
       clrwindow;
       cursoron;
       if play='1'then
          begin
             player[1]:='Player1';
             player[2]:='Player2';
          end
       else
          begin
             player[1]:='Player';
             player[2]:='Computer';
          end;
       gamebody(play);
       makewindow(1,22,75,25,magenta);
       writexy(2,23,lightmagenta,'Do you want to play again? (Y/N)');
       cursoron;
       readln(play);
    until ucase(play)='N';
       cursoroff;
end.
