//18.10.2019 
//Начата своя команда F. Доделана работа с сириынйм номером и адресом. Улчусшена совместимомсть с продукцией компании
//22.10.2019 
//Доделана команда F. (Без CRC). В F вшит серийник (и вроде напряжение с PID и зарезервировано под термокатализ пара значений).
//23.10.2019 
//Дописана команда F. Добавлена проверка целостности CRC, вывод реального серийника.
//01672  00000 00025 00008 01034 00344 00000 00000 00000 00000 08500444  0x08  
//КодТ    Рез  ADC_C ApADс ADC_T ApADt Рез   Рез   С-Мип рез    серийн   CRC

//23.10.2019 SW:0.08
//Измненён вывод прошивки, приветсввие при включении.
//28.11.2019 SW:0.09
//Добавлена команда ID, изменена логика флагов пересылки данных от мипа на комп. Просрал вечер.

//07.06.2022
//Освобождена память от ненужного (с 68% до 60%)




/*
AT+NWM=0 //Set p2p
AT+PFREQ=868100000 //SetFreq
AT+PSF=11 //Set SpreadFACTOR
AT+PBW=125
AT+PCR=0
AT+PPL=8
AT+PTP=20
*/
