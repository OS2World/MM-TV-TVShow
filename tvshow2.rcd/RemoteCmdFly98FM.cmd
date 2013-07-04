/* Primitive remote command handler */
/* for LifeView FlyVideo 98FM card */
/* Thanks to Lech Wiktor Piotrowski (lewhoo(at)teamos2.org.pl) */
/* remotemask 0x1ff								*/
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
     if token2="48" then call lineout pipe1, "NEXTCHANNEL"
     /* prev channel */
     if token2="C8" then call lineout pipe1, "PREVCHANNEL"
     /* 1 channel */
     if token2="C0" then call lineout pipe1, "CHANNEL 1"
     /* 2 channel */
     if token2="20" then call lineout pipe1, "CHANNEL 2"
     /* 3 channel */
     if token2="A0" then call lineout pipe1, "CHANNEL 3"
     /* 4 channel */
     if token2="E0" then call lineout pipe1, "CHANNEL 4"
     /* 5 channel */
     if token2="10" then call lineout pipe1, "CHANNEL 5"
     /* 6 channel */
     if token2="90" then call lineout pipe1, "CHANNEL 6"
     /* 7 channel */
     if token2="D0" then call lineout pipe1, "CHANNEL 7"
     /* 8 channel */
     if token2="30" then call lineout pipe1, "CHANNEL 8"
     /* 9 channel */
     if token2="B0" then call lineout pipe1, "CHANNEL 9"
     /* 0 channel */
     if token2="F0" then call lineout pipe1, "CHANNEL 0"
    end

end
call stream pipe1,'C','CLOSE'
call stream pipe2,'C','CLOSE'
