- premenovanie všetkých pinov podľa novej schémy viď. `docs/weekly_notes/06/pin_verification_todo.md`
- overené ENCA_EXTI10 a ENCB piny
- úspešné overenie 32.768 kHz LSE rezonátora
- úspešné overenie funkčnosti interného RTC modulu avšak po SWRST alebo HWRST sa uložený čas vymaže aj s pripojeným VBAT nevieme prečo
- povolenie ENCA_EXTI10 interruptu citlivého na nábežnú aj na dobežnú hranu
- __io_putchar threadsafe wrapper potrebuje ďalšie debugovanie 
- označenie pre actu::bridge a actu::lin_source podľa umiestnenia na front a rear
- poistka vyhárala kvôli skratu DAC_OUT1_CSF a VDD, po jeho odstránení poistka už nevyhára