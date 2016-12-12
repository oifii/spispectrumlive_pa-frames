rem start spispectrumlive.exe "Desktop Microphone (2- HD-5000 " 0 0 3600.0.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 2 100 1920 1080 150
rem start spispectrumlive.exe "Desktop Microphone (2- HD-5000 " 0 0 3600.0.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 1024 400 150
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
start spispectrumlive.exe "E-MU ASIO" 0 1 3600.0 1 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 256 100 150
PING 1.1.1.1 -n 1 -w 15000 >NUL
start spispectrumlive.exe "E-MU ASIO" 2 3 3600.0 256 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 256 100 150
PING 1.1.1.1 -n 1 -w 15000 >NUL
start spispectrumlive.exe "E-MU ASIO" 0 1 3600.0 512 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 256 100 150
PING 1.1.1.1 -n 1 -w 15000 >NUL
start spispectrumlive.exe "E-MU ASIO" 2 3 3600.0 768 1 3 "audiospispectrumliveclass" "audiospispectrumlive title" "" "" 1 100 256 100 150
