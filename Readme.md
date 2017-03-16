Introduction
=====================

A simple UI application wrote in Qt using [Omnikey CT-API library](https://www.hidglobal.com/sites/default/files/drivers/CT-API_V4_0_2_2A.exe)

Explanation
-------------------------
HOST:CT:Command:Comment --> Means we are sending command to the card reader/terminal
HOST:ICC:Command:Comment --> Means we are sending command to the smart car

Usage, some of commands below to the Card Terminal
--------------------------
HOST:CT:20FF00010101:Enable auto detection

HOST:CT:20FF00010100:Disable auto detection

HOST:CT:2011000000:RESET_CT

HOST:CT:201201000105:REQ_ICC for 5 seconds

HOST:CT:2013004600:GET_STAT46

HOST:CT:2013008000:GET_STAT80

Usage, some of commands below to the ICC
---------------------------
HOST:ICC:00A4040008A00000000300000000:Select GP card manager
