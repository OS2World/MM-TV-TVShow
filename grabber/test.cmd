/**/
pipe1="\pipe\tvshow"
call stream pipe1,'C','OPEN'
   call lineout pipe1, "SAVE test.jpg 12 3456"
/*   call lineout pipe1, "QUIT"*/
   reply=linein(pipe1)
   say "reply from daemon:" reply


call stream pipe1,'C','CLOSE'
