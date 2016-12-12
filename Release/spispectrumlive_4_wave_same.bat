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
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 600.0 1 1 8 "audiospispectrumliveclass" "audiospispectrumlive 1" "" "" 0 10 1024 512 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 600.0 1024 1 8 "audiospispectrumliveclass" "audiospispectrumlive 2" "" "" 1 10 1024 512 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 600.0 1 512 8 "audiospispectrumliveclass" "audiospispectrumlive 3" "" "" 2 10 1024 512 150
PING 1.1.1.1 -n 1 -w 5000 >NUL
start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 600.0 1024 512 8 "audiospispectrumliveclass" "audiospispectrumlive 4" "" "" 3 10 1024 512 150
PING 1.1.1.1 -n 1 -w 5000 >NUL

rem start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 1 100 8 "audiospispectrumliveclass" "audiospispectrumlive 5" "" "" 1 10 1024 512 150
rem PING 1.1.1.1 -n 1 -w 15000 >NUL
rem start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 256 100 8 "audiospispectrumliveclass" "audiospispectrumlive 6" "" "" 1 10 1024 512 150
rem PING 1.1.1.1 -n 1 -w 15000 >NUL
rem start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 512 100 8 "audiospispectrumliveclass" "audiospispectrumlive 7" "" "" 1 10 1024 512 150
rem PING 1.1.1.1 -n 1 -w 15000 >NUL
rem start spispectrumlive.exe "Wave (E-MU E-DSP Audio Processo" 0 0 3600.0 768 100 8 "audiospispectrumliveclass" "audiospispectrumlive 8" "" "" 1 10 1024 512 150
rem PING 1.1.1.1 -n 1 -w 15000 >NUL
