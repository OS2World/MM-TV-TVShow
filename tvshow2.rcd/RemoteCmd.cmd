/* Sample of remote command handler */
/* for AverMedia TVPhone98 card */
pipe1="\pipe\tvshow"
pipe2="\pipe\tvshowremote"
call stream pipe1,'C','OPEN'
call stream pipe2,'C','OPEN'
do forever
   reply=linein(pipe2)
   parse var reply token1 token2
   say reply 
   say token1 token2
   if token1="RCCODE:" then
    do
     /* next channel */
     if token2="C4F000" then call lineout pipe1, "NEXTCHANNEL"
     /* prev channel */
     if token2="44F000" then call lineout pipe1, "PREVCHANNEL"
    end
 
end
call stream pipe1,'C','CLOSE'
call stream pipe2,'C','CLOSE'