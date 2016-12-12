rem start spispectrumlive.exe "Desktop Microphone (2- HD-5000 " 0 0 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 2 100 1920 1080 150
rem start spispectrumlive.exe "Desktop Microphone (2- HD-5000 " 0 0 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 1024 400 150
rem start spispectrumlive.exe "Desktop Microphone (2- HD-5000 " 0 0 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 1920 1080 150
rem start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 1920 1080 150
rem start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 2 100 1024 400 150
rem start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 2 100 1920 1080 150
rem start spispectrumlive.exe "E-MU ASIO" 0 1 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 1920 1080 150
rem start spispectrumlive.exe "E-MU ASIO" 0 1 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 1024 400 150
rem start spispectrumlive.exe "E-MU ASIO" 0 1 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 512 200 150
rem start spispectrumlive.exe "E-MU ASIO" 0 1 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 256 100 150
rem start spispectrumlive.exe "E-MU ASIO" 2 3 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 1920 1080 150
rem start spispectrumlive.exe "E-MU ASIO" 2 3 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 1024 400 150
rem start spispectrumlive.exe "E-MU ASIO" 4 5 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 1920 1080 150
rem start spispectrumlive.exe "E-MU ASIO" 6 7 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 1920 1080 150
rem
rem FOR /L %%A IN (1,1,4) DO (
rem   ECHO %%A
rem   start spispectrumlive.exe "E-MU ASIO" 0 1 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 256 100 150
rem
rem   PING 1.1.1.1 -n 1 -w 30000 >NUL
rem )
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 1 7 "audiospispectrumliveclass" "audiospispectrumlive 1" "" "" 0 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 256 1 7 "audiospispectrumliveclass" "audiospispectrumlive 2" "" "" 0 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 512 1 7 "audiospispectrumliveclass" "audiospispectrumlive 3" "" "" 0 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 768 1 7 "audiospispectrumliveclass" "audiospispectrumlive 4" "" "" 0 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL

start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 100 8 "audiospispectrumliveclass" "audiospispectrumlive 5" "" "" 1 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 256 100 8 "audiospispectrumliveclass" "audiospispectrumlive 6" "" "" 1 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 512 100 8 "audiospispectrumliveclass" "audiospispectrumlive 7" "" "" 1 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 768 100 8 "audiospispectrumliveclass" "audiospispectrumlive 8" "" "" 1 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL

start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 200 9 "audiospispectrumliveclass" "audiospispectrumlive 9" "" "" 2 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 256 200 9 "audiospispectrumliveclass" "audiospispectrumlive 10" "" "" 2 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 512 200 9 "audiospispectrumliveclass" "audiospispectrumlive 11" "" "" 2 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 768 200 9 "audiospispectrumliveclass" "audiospispectrumlive 12" "" "" 2 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL

start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 300 10 "audiospispectrumliveclass" "audiospispectrumlive 13" "" "" 3 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 256 300 10 "audiospispectrumliveclass" "audiospispectrumlive 14" "" "" 3 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 512 300 10 "audiospispectrumliveclass" "audiospispectrumlive 15" "" "" 3 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 768 300 10 "audiospispectrumliveclass" "audiospispectrumlive 16" "" "" 3 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL

start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 400 7 "audiospispectrumliveclass" "audiospispectrumlive 17" "" "" 4 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 256 400 7 "audiospispectrumliveclass" "audiospispectrumlive 18" "" "" 4 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 512 400 7 "audiospispectrumliveclass" "audiospispectrumlive 19" "" "" 4 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 768 400 7 "audiospispectrumliveclass" "audiospispectrumlive 20" "" "" 4 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL

start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 500 8 "audiospispectrumliveclass" "audiospispectrumlive 21" "" "" 5 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 256 500 8 "audiospispectrumliveclass" "audiospispectrumlive 22" "" "" 5 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 512 500 8 "audiospispectrumliveclass" "audiospispectrumlive 23" "" "" 5 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 768 500 8 "audiospispectrumliveclass" "audiospispectrumlive 24" "" "" 5 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL

start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 600 9 "audiospispectrumliveclass" "audiospispectrumlive 25" "" "" 6 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 256 600 9 "audiospispectrumliveclass" "audiospispectrumlive 26" "" "" 6 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 512 600 9 "audiospispectrumliveclass" "audiospispectrumlive 27" "" "" 6 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 768 600 9 "audiospispectrumliveclass" "audiospispectrumlive 28" "" "" 6 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL

start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 700 10 "audiospispectrumliveclass" "audiospispectrumlive 29" "" "" 7 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 256 700 10 "audiospispectrumliveclass" "audiospispectrumlive 30" "" "" 7 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 512 700 10 "audiospispectrumliveclass" "audiospispectrumlive 31" "" "" 7 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 768 700 10 "audiospispectrumliveclass" "audiospispectrumlive 32" "" "" 7 10 256 100 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
