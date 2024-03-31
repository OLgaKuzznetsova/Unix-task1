make clean
python3 ./create_file_A.py
make
./task1 A B
gzip -k A
gzip -k B
gzip -cd B.gz | ./task1 C
./task1 A D -b 100

echo -e "\n Размер файла А 4194305 байт. Количество блоков 8200.\n\n" >> result.txt
stat A >> result.txt
echo -e "\n\nРазмер файла А.gz и количество блоков меньше чем в файле А.\n\n" >> result.txt
stat A.gz >> result.txt
echo -e "\n\nРазмер файла В такой же, как размер файла А. Количество блоков в файле В 24 - 8 блоков для каждой 1 в файле А.\n\n" >> result.txt
stat B >> result.txt
echo -e "\n\nРазмер файла В.gz и количество блоков совпадаают с файлом A.gz.\n\n" >> result.txt
stat B.gz >> result.txt
echo -e "\n\nРазмер файла С и количество блоков совпадаают с файлом В.\n\n" >> result.txt
stat C >> result.txt
echo -e "\n\nРазмер файла D совпадает с размеров файла А. Количество блоков 32 из-за размера блока в программе 100 байт, который не является степенью двойки.\n\n" >> result.txt
stat D >> result.txt
