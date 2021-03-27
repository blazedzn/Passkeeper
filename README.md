<img src="https://i.imgur.com/NVRx2jF.png" alt="Logo" title="Passkeeper" align="left" height="50" />

 # Passkeeper
---------------------
![GitHub](https://img.shields.io/github/license/blazedzn/Passkeeper?color=%23eddd02&style=flat-square)

🇷🇺
- [Требования](#requirements_ru)
- [Как это работает?](#how_it_works_ru)
- [Возможности](#functionality_ru)
- [Установка/Компиляция](#installation_ru)
- [Примечание](#note_ru)

<a name="requirements_ru"><h2>Требования</h2></a>
1. C++ 17 
2. Mingw, либо любой другой компилятор

<a name="how_it_works_ru"><h2>Как это работает?</h2></a>
При добавлении сервиса в текстовый файл все символы поочерёдно заменяются на соответствующие им из хэш-карты. Важно заметить, что пароли имеют отдельную хэш-карту и хэширутся два раза. К примеру, условный пароль «12345» сначала хэшируется функцией hash_password, из-за чего на выходе мы получаем «DJDfjhudGH4!SKUr8_G8g?pZd7A4G+» и потом эта строка хешируется ещё раз, но уже функцией hash_file и на выходе образуется «c+ZpjQ4Mv:Q6c+ZpjQ<[}vsx)MAL}1e+{+^.<E9!:5Xe5v9f?tIXEUZ1ljb03Hn$>$dZC-gEy@6(eYyHqKShG/-1Bobzo<zomz(jvpe*#]8d?tIXEUmz(jvp#e.W0fUM]OoHCtl.nH,X96fLXe5v9fcQLAV*XG=:<A3Hn$>$?tIXEUK?q=kd». То есть введя из 5-ти символов на выходе получаем 180 символов хэша.

Сама же "база данных" (по факту обычный текстовик) в сыром виде имеет +-[такой](https://i.imgur.com/axHAW1a.png) вид 

В расшифрованном виде:
> Service: 12345
>>Password: DJDfjhudGH4!SKUr8_G8g?pZd7A4G+
>
>Service: test
>>Password: bD>7XpB{68%rV53tP=bD>7XphCU!4Q(B]vT4

Так же программа защищена паролем, который нужно обязательно ввести при её запуске (*строка 19 в исходном коде*). Если ввести его не правильно, то программа закроется.

<a name="functionality_ru"><h2>Возможности</h2></a>
1. A - Добавить сервис в "базу данных"
2. G - Получить пароль от сервиса с "базы данных"
3. S - Получить список всех сервисов
4. D - Удалить сервис с "базы данных"
5. N - Сгенерировать рандомный пароль
6. B - Сделать бэкап "базы данных"
<br>*Будет сохранен в папку PasskeeperBackups относительно исполняемого файла* 
<br>*После бэкапа в консоль могут вывестись ["битые"](https://i.imgur.com/x2dTKa9.png) символы. Связано это с тем, что командная строка не может нормально воспроизвести кириллицу. На работу это никак не влияет*
7. R - Восстановить бэкап
8. Q - Выйти из программы

<a name="installation_ru"><h2>Установка/Компиляция</h2></a>
1. Качаем/клонируем репо к себе на пк
2. Разархивируем содержимое 
3. Переходим папку с исходниками и открываем командную строку
4. Компилируем main.cpp командой
<br><code>g++ -std=c++17 -o passkeeper.exe main.cpp</code>
<br>*Подробнее на гифке:*
<br>![Tutorial](https://s4.gifyu.com/images/tutorial.gif)

<a name="note_ru"><h2>Примечание</h2></a>
Программа предназначалась для личного использования, но если вдруг ты хочешь ею воспользоваться, то настоятельно рекомендую изменить под себя следующее:

* Основной пароль от программы (*строка 19*)
* Хэш-карту hash_password (*строка 653*)
* Хэш-карту dehash_password (*строка 675*) 
* Длинну на хэш в dehash_password (*строка 780*)
* Хэш-карту hash_file (*строка 796*) 
* Хэш-карту dehash_password (*строка 910*) 
* Длинну на хэш в dehash_file (*строка 1012*)
---------------------
🇺🇸
- [Requirements](#requirements_en)
- [How it works?](#how_it_works_en)
- [Functionality](#functionality_en)
- [Installation](#installation_en)
- [Note](#note_en)

<a name="requirements_en"><h2>Requirements</h2></a>
1. C++ 17 
2. Mingw, or any other compiler

<a name="how_it_works_en"><h2>How it works?</h2></a>
When you add a service to a program, all characters are alternately replaced with the corresponding ones from the hash-map. It is important to note that passwords have a separate hash map and are hashed twice. For example, the conditional password «12345» is first hashed by the hash_password function, which is why we get at the output «DJDfjhudGH4!SKUr8_G8g?pZd7A4G+» and then this string is hashed again, but already the hash_file function and the output is formed «c+ZpjQ4Mv:Q6c+ZpjQ<[}vsx)MAL}1e+{+^.<E9!:5Xe5v9f?tIXEUZ1ljb03Hn$>$dZC-gEy@6(eYyHqKShG/-1Bobzo<zomz(jvpe*#]8d?tIXEUmz(jvp#e.W0fUM]OoHCtl.nH,X96fLXe5v9fcQLAV*XG=:<A3Hn$>$?tIXEUK?q=kd». That is, having entered 5 characters, at the output we get 180 hash characters.

The very same "database" (in fact .txt file) in its raw form has approximately [this](https://i.imgur.com/axHAW1a.png) view

Decrypted:
>Service: 12345
>>Password: DJDfjhudGH4!SKUr8_G8g?pZd7A4G+
>
>Service: test
>>Password: bD>7XpB{68%rV53tP=bD>7XphCU!4Q(B]vT4

The program is also protected by a password, which must be entered when starting it (*line 19 in the source code*). If you enter it incorrectly, the program will close.

<a name="functionality_en"><h2>Functionality</h2></a>
1. A - Add service to database
2. G - Get service password from database
3. S - Get all services from database
4. D - Delete service from database
5. N - Generate a random password
6. B - Backup database
<br> *Backup will be saved in PasskeeperBackups folder relative to executable file*
7. R - Restore database from backup
8. Q - Quit

<a name="installation_en"><h2>Installation</h2></a>
1. Download/clone repo to your PC
2. Extract folder from .zip
3. Open this folder and launch windows command line within this place
4. Compile main.cpp by this command
<br><code>g++ -std=c++17 -o passkeeper.exe main.cpp</code>
<br>*Detailed on gif:*
<br>![Tutorial](https://s4.gifyu.com/images/tutorial.gif)

<a name="note_en"><h2>Note</h2></a>
I created this program for personal use, but if you want to use it, then I strongly recommend changing the following for yourself:

* The main password for the program (*line 19*)
* hash-map hash_password (*line 653*)
* hash-map dehash_password (*line 675*) 
* hash length in dehash_password (*line 780*)
* hash-map hash_file (*line 796*) 
* hash-map dehash_password (*line 910*) 
* hash length dehash_file (*line 1012*)
