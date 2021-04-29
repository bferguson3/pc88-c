1   REM examples/psg
50  AD=&HC000
51  REM CLEAR ,&HB3FF
55  PRINT "Writing to $C000" ;
60  FOR K = 1 TO 39 : REM rows of data
100 READ A$
105 FOR I = 0 TO 33 : REM 34 bytes per row
108 B=0:C=0:D=0
110 HA$ = MID$(A$,1+(I*2),1) : HB$ = MID$(A$,2+(I*2),1)
115 IF HA$>="A"AND HA$<="F" THEN U=7 ELSE U=0
116 IF HB$>="A"AND HB$<="F" THEN Y=7 ELSE Y=0
120 B = (ASC(HA$)-&H30-U)*16 : C = ASC(HB$)-&H30-Y : D = B+C
130 POKE AD+I,D
140 NEXT I
144 AD = AD+34 : PRINT ".";
145 NEXT K
150 PRINT " Done." : END
1000 DATA 21E403E5CDC1C121E603E3CDC1C1F1C9F5C5D5E5FDE5F321E403E5CDC1C121E603E3
1100 DATA CDC1C1F1FBFDE1E1D1C1F1ED45F5C5D5E5FDE5F321E403E5CDC1C121E603E3CDC1C1
1200 DATA F1FBFDE1E1D1C1F1ED45210200394E79E60F5F79E6F04F0600CB28CB19CB28CB19CB
1300 DATA 28CB19CB28CB193E099330047BC6075F3E0991300479C6074F7BC6305F79C6302103
1400 DATA 00394E234602696023730303AF02C921E8FF39F9F32100D83600CD3DC244CB78280A
1500 DATA 2191C1E5CDBDC2F1181BCB70280A2196C1E5CDBDC2F1180D78E6C02808219AC1E5CD
1600 DATA BDC2F1210000394D4421100039EBFD211300FD39FD7300FD7201FD211700FD39FD36
1700 DATA 0000FD211700FD39FD7E00D60F3070C5D5FD66002E44E5CDC1C1214533E3CDC1C1F1
1800 DATA D1C17921170039FD211500FD3986FD770078CE00FD23FD7700C5D53E45F533CDB3C1
1900 DATA 7D33D1C1FD211500FD39FD6E00FD6601776B62C5D5E5F533CD4EC0F133D1C1211300
2000 DATA 397E23666FC5D5E5CDBDC2F1D1C1FD211700FD39FD340018832110C02202F3212FC0
2100 DATA 2204F321E403E5CDC1C121E603E3CDC1C1F1FB18FE21180039F9C956315320005632
2200 DATA 200056314820003100E021C8F32204D8FD2106D8FD360000C397C0FD210200FD39FD
2300 DATA 7E004FED786FC9DDE5DD210000DD39DD4E04DD7E05ED79DDE1C93E39D3313AD779FE
2400 DATA 323829282BFE33282BFE34282BFE38382B282D3AD879FE0D282AFE31282AFE32282A
2500 DATA FE33282AFE34282A182C3E00182A3E0118263E0218223E03181E3E04181A3E051816
2600 DATA 3E0618123E07180E3E08180A3E0918063E0A18023E0B3E3BD3312E00C93E31F533CD
2700 DATA B3C14533AF0E00C53E6EF533CDB3C133C126007DB16F7CB067C92A04D8FD210200FD
2800 DATA 39FD7E00772A04D8232204D8FD2106D8FD34003A06D8D650C0FD3600002104D87EC6
2900 DATA 2877D02334C92A04D8FD210200FD39FD7E00772A04D823232204D8FD2106D8FD3400
3000 DATA 3A06D8D650C0FD3600002104D87EC62877D02334C9DDE5DD210000DD39F50E003A06
3100 DATA D83203D8DD6E04DD66050600097EB7286B0CFE0A203C2A04D87DC6385F7CCE0CC521
3200 DATA 7800E557D5CDA6C4F1F1EBC1217800BFED523A03D816005F19E3DD7EFE2104D88677
3300 DATA DD7EFF238E773A03D83206D818B22A04D8772A04D8232204D8FD2106D8FD34003A06
3400 DATA D8D6502099FD3600002104D87EC62877308C23341888DDF9DDE1C9DDE5DD210000DD
3500 DATA 39F50E003A06D83203D8DD6E04DD66050600094678B7287C692C78D60A203D4D2A04
3600 DATA D87DC6385F7CCE0CC5217800E557D5CDA6C4F1F1EBC1217800BFED523A03D816005F
3700 DATA 19E3DD7EFE2104D88677DD7EFF238E773A03D83206D818AE594DDD6E04DD66051600
3800 DATA 197E2A04D8772A04D8232204D8FD2106D8FD34003A06D8D6502089FD3600002104D8
3900 DATA 7EC62877D25CC32334C35CC3DDF9DDE1C9DDE5DD210000DD39DD4E05060069602909
4000 DATA 2909290929292901C8F309DD4E040600092204D8DD7E043206D8DDE1C9DDE5DD2100
4100 DATA 00DD39DD4E0506006960290929092909292929EB21C8F319EBDD6E04260029192204
4200 DATA D8DD7E04873206D8DDE1C91118F40E0079D618D0060078D61430083E801213130418
4300 DATA F321500019EB0C18E5210300394E060069602909290929092929290118F4097ED680
4400 DATA 2804232318F7FD210200FD39FD7E007723FD7E0277C9210300395E2B6ECDC0C4EBC9
4500 DATA F1E1D1D5E5F5CDC3C4EBC9F1E1D1D5E5F5180A210300395E2B6E2600547BE680B220
4600 DATA 100610ED6A17933001833FED6A10F65FC906097D6C2600CB1DED6AED523001193F17
4700 DATA 10F5CB10505FEBC90000000000000000000000000000000000000000000000000000�