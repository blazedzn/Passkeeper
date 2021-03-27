#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <filesystem>

std::string hash_password(std::string password);
std::string dehash_password(const std::string& password);
std::string hash_file(std::string text);
std::string dehash_file(const std::string& text);

int main() {
    //пароль от самой программы
    std::string main_pass{"123"};
    std::string user_pass;
    std::cout << "Enter password to access the program: ";
    std::cin >> user_pass;

    if (user_pass != main_pass) {
        return 1;
    }
    char choice{};
    std::cout << "\nBlazedzn's Passkeeper\n\nMenu:\nA - Add service to database\nG - Get service password from database\nS - Get all services from database\nD - Delete service from database\nN - Generate a random password\nB - Backup database\nR - Restore database from backup\nQ - Quit\n\n";
    do {
        std::cin >> choice;

        switch (choice) {
            case 'A':
            case 'a': {
                std::ifstream read_file("passkeeper.txt"); //открываем файл для чтения
                //если файла нету, то
                if (read_file.is_open() == 0) {
                    std::cout << "Database is missing, creating a new one" << std::endl;
                    //создаем его
                    std::ofstream file("passkeeper.txt");
                }

                std::string service_name{};
                std::string password_to_hash{};
                std::string hashed_password{};

                std::cout << "Enter service name: ";
                std::cin.ignore();
                std::getline(std::cin, service_name);

                std::string file_text{};
                std::string dehashed_file_text{};
                //переменная "заглушка", которая нужна для чтения файла построчно
                std::string trash{};

                //читаем данные с файла и сохраняем их в переменную
                while (std::getline(read_file, trash)) {
                    file_text += trash;
                    file_text.push_back('\n');
                }
                //расшифровываем содержимое файла
                dehashed_file_text = dehash_file(file_text);

                //Проверяем, нет ли такого сервиса в бд:
                //переводим dehashed_file_text из string в istringstream (для последующей итерации)
                std::istringstream stream{dehashed_file_text};
                //counter (нужен для сохранения кол-ва симолов в тексте)
                int counter_service{0};
                //вектор, в котором будем сохранять названия сервисов, которые есть в бд
                std::vector<std::string> services{};
                bool is_exists {false};

                //итерируемся через кажду строку
                while (std::getline(stream, trash)) {
                    //к counter_service прибавляем длинну каждой строки
                    counter_service += trash.length();
                    //если в расматриваемой строке есть "Service: "
                    if (trash.find("Service: ") != std::string::npos) {
                        //добавляем название в вектор:
                        //dehashed_file_text.substr => подстрока из всего текста
                            //pos: counter_service-trash.length() + 9 =>
                                // counter_service(считает каждый символ в строке дo if) - длина самой строки + 9 (длинна "Service: ")
                            //n: trash.length()-9 =>
                                //длинна всей строки - 9 (длинна "Service: ")
                        services.push_back(dehashed_file_text.substr(counter_service - trash.length() + 9, trash.length() - 9));
                    }
                    //добавляем к counter_service единицу (так как в конце строки присутствует white space, который игнорируется)
                    counter_service += 1;
                }

                //итерируемся через вектор и сравниваем сервис который хотим добавить с сервисами вектора
                for(int i{0}; i < services.size(); i++){
                    //если такой сервис уже есть
                    if(service_name == services.at(i)){
                        std::cout << "A service with this name is already in the database, give it a different name" << std::endl;
                        is_exists = true;
                        //выходим из цикла
                        break;
                    }
                }
                //если совпало, то прекращаем исполнение кода
                if(is_exists){
                    break;
                }

                std::cout << "Enter password to this service: ";
                std::getline(std::cin, password_to_hash);

                //играничение на длинну пароля
                if(password_to_hash.size() <= 3){
                    std::cout << "Password must be at least 4 characters long" << std::endl;
                    break;
                }

                bool is_space{false};

                //проверяем, есть ли пробел в пароле
                for(int i{0}; i < password_to_hash.length(); i++){
                    //если есть пароль
                    if(password_to_hash.at(i) == ' '){
                        std::cout << "Password must not contain spaces" << std::endl;
                        is_space = true;
                        break;
                    }
                }

                //если is_space == true, то прекращаем выполнение кода
                if(is_space) break;

                //открываем файл с правами на запись
                std::ofstream write_file("passkeeper.txt");
                //хэшируем и сохраняем хэшированный текст в переменную hashed_final_text => предыдущий текст + "\nService: " + название сервиса, который ввёл пользователь + "\nPassword: " + пароль который ввёл пользователь(хэшированный)
                std::string hashed_final_text = hash_file(dehashed_file_text + "\nService: " + service_name + "\nPassword: " + hash_password(password_to_hash) + "\n");

                //записываем это всё в файл
                write_file << hashed_final_text;
                //закрываем файл
                read_file.close(); write_file.close();

                std::cout << "Records to service " << service_name << " successfully created" << std::endl;

                break;
            }

            case 'G':
            case 'g': {
                std::ifstream read_file("passkeeper.txt"); //открываем файл для чтения
                //если файла нету, то
                if (read_file.is_open() == 0) {
                    std::cout << "Database is missing, creating a new one" << std::endl;
                    //создаем его
                    std::ofstream file("passkeeper.txt");
                }

                std::string service_name{};

                std::cout << "Enter service name you want to see password: ";
                std::cin.ignore();
                std::getline(std::cin, service_name);

                std::string file_text{};
                //переменная "заглушка", которая нужна для чтения файла построчно
                std::string trash{};

                //читаем данные с файла и сохраняем их в переменную
                while (std::getline(read_file, trash)) {
                    file_text += trash;
                    file_text.push_back('\n');
                }

                //Дешифруем полученный текст (текст файла)
                file_text = dehash_file(file_text);
                std::vector<std::string> services_vector{};
                unsigned int counter{1};

                //читаем текст
                while(file_text.find("Service: ", counter) != std::string::npos){
                    //находим подстроку "Service:" и добавляем к ней 9, что бы получить начало название самого сервиса
                    unsigned int start_pos = file_text.find("Service: ", counter)+9;
                    //конец подстроки сервиса: ищем символ новой строки начиная от start_pos
                    unsigned int end_pos = file_text.find('\n', start_pos);
                    //добавляем название в вектор
                    services_vector.push_back(file_text.substr(start_pos, end_pos-start_pos));
                    //обновляем counter
                    counter = start_pos;
                }

                bool is_found = false;
                for(int i{0}; i < services_vector.size(); i++){
                    if(services_vector.at(i) == service_name){
                        //ставим is_found на true, что бы не сработала следующая проверка
                        is_found = true;

                        //начало подстроки с паролем
                            //file_text.find(services_vector.at(i)) => название сервиса
                            //services_vector.at(i).length() => длинна названия сервиса
                            //11 => длинна подстроки "Password: "
                        unsigned int start_pos = file_text.find(services_vector.at(i)) + services_vector.at(i).length() + 11;
                        //ищем конец строки начиная с start_pos
                        unsigned int end_pos = file_text.find('\n', start_pos);
                        //получаем хэшированный пароль
                        std::string hashed_password = file_text.substr(start_pos, end_pos-start_pos);
                        //тут и так всё понятно
                        std::string dehashed_password = dehash_password(hashed_password);
                        //и тут тоже
                        std::cout << "Password for service " << service_name << " is " << dehashed_password << std::endl;
                    }
                }
                //если сервис не найден, то выводим сообщение с этим в консоль
                if(!is_found) std::cout << "This service is missing from the database" << std::endl;

                break;
            }
            case 'S':
            case 's': {
                std::ifstream read_file("passkeeper.txt"); //открываем файл для чтения
                //если файла нету, то
                if (read_file.is_open() == 0) {
                    std::cout << "Database is missing, creating a new one" << std::endl;
                    //создаем его
                    std::ofstream file("passkeeper.txt");
                }

                //мусорная переменная (нужна для чтения файла)
                std::string trash{};
                //текст зашифрованного файла
                std::string hashed_file_text{};

                //читаем данные с файла и сохраняем их в переменную
                while (std::getline(read_file, trash)) {
                    hashed_file_text += trash;
                    hashed_file_text.push_back('\n');
                }
                trash = "";

                //расшифрованый текст
                std::string dehashed_file_text = dehash_file(hashed_file_text);
                //переводим dehashed_file_text из string в istringstream  (для последующей итерации)
                std::istringstream stream{dehashed_file_text};
                //counter (нужен для сохранения кол-ва симолов в тексте)
                int counter_service{0};

                std::cout << "All services in database:\n" << std::endl;

                //итерируемся через кажду строку
                while (std::getline(stream, trash)) {
                    //к counter_service прибавляем длинну каждой строки
                    counter_service += trash.length();
                    //если в расматриваемой строке есть "Service: "
                    if (trash.find("Service: ") != std::string::npos) {
                        //выводим название сервиса в консоль:
                            //dehashed_file_text.substr => подстрока из всего текста
                            //pos: counter_service-trash.length() + 9 =>
                                // counter_service(считает каждый символ в строке дo if) - длина самой строки + 9 (длинна "Service: ")
                            //n: trash.length()-9 =>
                                //длинна всей строки - 9 (длинна "Service: ")
                        std::cout << dehashed_file_text.substr(counter_service - trash.length() + 9, trash.length() - 9)<< std::endl;
                    }
                    //добавляем к counter_service единицу (так как в конце строки присутствует white space, который игнорируется)
                    counter_service += 1;
                }

                break;
            }
            case 'D':
            case 'd':{
                std::ifstream read_file("passkeeper.txt"); //открываем файл для чтения
                //если файла нету, то
                if (read_file.is_open() == 0) {
                    std::cout << "Database is missing, creating a new one" << std::endl;
                    //создаем его
                    std::ofstream file("passkeeper.txt");
                }

                std::string service_name{};

                std::cout << "Enter service name you want to delete: ";
                std::cin.ignore();
                std::getline(std::cin, service_name);

                std::string file_text{};
                std::string trash{};

                //читаем данные с файла и сохраняем их в переменную
                while (std::getline(read_file, trash)) {
                    file_text += trash;
                    file_text.push_back('\n');
                }

                //Дешифруем полученный текст (текст файла)
                file_text = dehash_file(file_text);

                std::vector<std::string> services_vector{};
                unsigned int counter{1};
                bool is_found = false;

                //читаем текст
                while(file_text.find("Service: ", counter) != std::string::npos){
                    //находим подстроку "Service:" и добавляем к ней 9, что бы получить начало название самого сервиса
                    unsigned int start_pos = file_text.find("Service: ", counter)+9;
                    //конец подстроки сервиса: ищем символ новой строки начиная от start_pos
                    unsigned int end_pos = file_text.find('\n', start_pos);
                    //добавляем название в вектор
                    services_vector.push_back(file_text.substr(start_pos, end_pos-start_pos));
                    //обновляем counter
                    counter = start_pos;
                }

                for(int i{0}; i < services_vector.size(); i++){
                    if(service_name == services_vector[i]){
                        is_found = true;

                        //получаем позицию начала названия сервиса
                        std::string token = file_text.substr(0, file_text.find(service_name));
                        //получаем позицию начала пароля => длина токена + длина текста сервиса + 11 (11 - лишние символы, которые нам не нужны)
                        unsigned int password_start_pos = token.length() + service_name.length() + 11;
                        //получаем позицию конца пароля (от новой строки до начала пароля - начало пароля)
                        unsigned int password_end_pos = file_text.find('\n', password_start_pos) - password_start_pos;

                        //хэшированный пароль
                        std::string hashed_password = file_text.substr(password_start_pos, password_end_pos);

                        //получаем позицию начала названия сервиса
                        unsigned int start_pos = file_text.find("Service: " + service_name);
                        //получаем позицию конца "блока" с сервисом:
                        //start_pos - начало позиции
                        //9 - длинна строки "Service: "
                        //service_name.length() - длинна названия сервиса
                        //11 - длинна строки "Password: "
                        //hashed_password.length() - длинна хэшированного пароля
                        //2 - два таба, которые разделяют "блоки"
                        unsigned int end_pos = start_pos + 9 + service_name.length() + 11 + hashed_password.length() + 2;

                        //складываем финальный текст
                        std::string second_part{};
                        std::string first_part{};
                        //если ты пытаешься удалить последний по списку сервис
                        //конечная позиция > длинну файла
                        if (end_pos > file_text.length()) {
                            //первая часть:
                            //0 - начало позиции
                            //start_pos-1 - позиция начала названия сервиса, который нужно удалить минус еденица (лишний таб)
                            first_part = file_text.substr(0, start_pos-1);
                            //вторая часть:
                            //end_pos-1 - конец позиции "блока" с сервисом, который нужно удалить минус единица (по факту просто конец файла)
                            //file_text.length()-end_pos - сколько символов осталось занести в подстроку
                            second_part = file_text.substr(end_pos - 1, file_text.length() - end_pos);
                        } else {
                            //первая часть:
                            //0 - начало позиции
                            //start_pos - озицию начала названия сервиса, который нужно удалить
                            first_part = file_text.substr(0, start_pos);
                            //вторая часть:
                            //end_pos - конец позиции "блока" с сервисом, который нужно удалить
                            //file_text.length()-end_pos - сколько символов осталось занести в подстроку
                            second_part = file_text.substr(end_pos, file_text.length() - end_pos);
                        }

                        //финальный текст
                        std::string edited_text = first_part + second_part;

                        //хэшируем финальный текст
                        std::string hashed_edited_text = hash_file(edited_text);

                        //открываем файл с правами на запись
                        std::ofstream write_file("passkeeper.txt");

                        //записываем это всё в файл
                        write_file << hashed_edited_text;
                        //закрываем файл
                        read_file.close(); write_file.close();

                        std::cout << "Record " << service_name << " successfully deleted" << std::endl;

                    }
                }
                //если сервис не найден, то выводим сообщение с этим в консоль
                if(!is_found) std::cout << "This service is missing from the database" << std::endl;
                break;

            }
            case 'N':
            case 'n':{
                //символы, доступные для генерации пароля
                std::string reference{"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789!@#$%^&*()-_=+[];<>/"};
                std::string pass_length_str{};
                std::string generated_password{};

                std::cout << "Enter length of your new password: ";
                std::cin >> pass_length_str;

                bool is_int = true;
                for(int i{0}; i<pass_length_str.length();i++){
                    //проверка на то, что бы pass_length_char был int, а не char, потому что тогда программа уйдёт в бесконечный цикл
                    if(!isdigit(pass_length_str.at(i))){
                        //если в строке будет найдена буква, то ставим is_int на false
                        is_int = false;
                        std::cout << "Length of new password should be an integer" << std::endl;
                        break;
                    }
                }
                if(!is_int) break;

                //конвертируем str в int
                int pass_length = std::stoi(pass_length_str);

                if(pass_length <= 0){
                    std::cout << "Length of new password should be greater than zero" << std::endl;
                }

                //задаем сид для генератора пароля
                srand(time(nullptr));

                for(int i{0}; i < pass_length; i++){
                    //добавляем в переменную generated_password рандомный символ из reference
                    generated_password.push_back(reference.at(rand()%reference.length()));
                }
                std::cout << "Your generated password is: "<< generated_password << std::endl;

                break;
            }
            case 'B':
            case 'b':{
                std::ifstream read_file("passkeeper.txt"); //открываем файл для чтения
                //если файла нету, то
                if (read_file.is_open() == 0) {
                    std::cout << "Database is missing, creating a new one" << std::endl;
                    //создаем его
                    std::ofstream file("passkeeper.txt");
                }

                //текущий путь исполняемого файла сконвертированного в string
                std::string current_path = std::filesystem::current_path().string();
                //создаем папку
                std::filesystem::create_directory("PasskeeperBackups/");
                //открываем файл, который нужно бэкапнуть в режиме только для чтения
                std::ifstream file_to_backup("passkeeper.txt");
                //текст из этого файла
                std::string text_to_backup{};
                std::string trash{};
                while (getline (file_to_backup, text_to_backup)) {
                    text_to_backup += trash;
                }

                //время сейчас
                auto now = std::chrono::system_clock::now();
                //время сейчас в time_t
                auto in_time_t = std::chrono::system_clock::to_time_t(now);

                //переводим время с unix в human type
                std::stringstream stream;
                stream << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
                std::string current_time = stream.str();

                //путь, куда будет сохранен бэкап (PasskeeperBackups/ относительно исполняемого файла) название файла backup + время сейчас + .txt
                //raw_file_location, так как current_time такого типа -> 2021-03-15 19:17:06.txt, а, как известно, в винде нельзя создавать папки со знаком ":"
                std::string raw_file_location = "PasskeeperBackups/" + current_time + ".txt";
                //отфильтрованный путь, куда будем сохранять бэкап (без знаков ":")
                std::string file_location{};

                for(int i{0}; i< raw_file_location.length(); i++){
                    if(raw_file_location.at(i) == ':'){
                        file_location.push_back('_');
                    } else{
                        file_location.push_back(raw_file_location.at(i));
                    }
                }

                //создаем бэкап
                std::ofstream file(file_location);

                //записываем это всё в файл
                file << text_to_backup;
                //закрываем файл
                read_file.close(); file.close();

                std::cout<< "The backup has been successfully created and saved in " <<current_path + '\u005c' + file_location << std::endl;

                break;
            }
            case 'R':
            case 'r':{
                std::ifstream read_file("passkeeper.txt"); //открываем файл для чтения
                //если файла нету, то
                if (read_file.is_open() == 0) {
                    std::cout << "Database is missing, creating a new one" << std::endl;
                    //создаем его
                    std::ofstream file("passkeeper.txt");
                }

                std::cout << "Available backups: " << std::endl;

                std::string path = "PasskeeperBackups/";
                int counter{0};
                std::vector<std::filesystem::path> backups_vector{};

                //отлавливаем ошибку на открытие файла
                try{
                    //итерируемся через файлы в папке
                    for (const auto & iterator : std::filesystem::directory_iterator(path)) {
                        std::cout << counter << " - " << iterator.path() << std::endl;
                        //сохраняем пусть к файлу в вектор
                        backups_vector.push_back(iterator.path());
                        counter++;
                    }
                } catch (const std::exception &ex) {
                    std::cout << "Error occurred. Unable to open folder with backups, maybe you don't have them yet. More information about an error:\n" << ex.what() << std::endl;
                    break;
                }

                std::cout << "Which one you want to restore? (Enter number)" << std::endl;
                std::string file_to_restore_str{};
                std::cin >> file_to_restore_str;
                bool is_int = true;
                for(int i{0}; i< file_to_restore_str.length(); i++){
                    //проверка на то, что бы file_to_restore был int, а не char, потому что тогда программа уйдёт в бесконечный цикл
                    if(!isdigit(file_to_restore_str.at(i))){
                        is_int = false;
                        std::cout << "Index of backup should be an integer" << std::endl;
                        break;
                    }
                }
                if(!is_int) break;

                //конвертируем str в int
                int file_to_restore = std::stoi(file_to_restore_str);

                if (file_to_restore >= counter or file_to_restore < 0){
                    std::cout << "Enter a valid number of backup you want to restore" << std::endl;
                    break;
                }
                //получаем путь к файлу через вектор
                std::ifstream backup_file(backups_vector.at(file_to_restore));

                std::string text_from_backup{};
                std::string trash{};
                //читаем текст из файла бэкапа
                while (getline(backup_file, text_from_backup)) {
                    text_from_backup += trash;
                }
                //закрываем его
                backup_file.close();

                //открываем основную бд
                std::ofstream restore_file("passkeeper.txt");
                restore_file << text_from_backup;
                //закрываем
                restore_file.close();

                std::cout << "Backup " << backups_vector.at(file_to_restore) << " was successfully restored" << std::endl;

                break;
            }
            default:
                std::cout << "Incorrect choice" << std::endl;
                break;

        }
    } while (choice != 'Q' and choice != 'q');
}


std::string hash_password(std::string password){
    //хэш карта
    std::map<char, std::string> hash_map = {
                                     {'0', "HN?Pv3"},
                                     {'1', "DJDfjh"},
                                     {'2', "udGH4!"},
                                     {'3', "SKUr8_"},
                                     {'4', "G8g?pZ"},
                                     {'5', "d7A4G+"},
                                     {'6', "8-=%We"},
                                     {'7', "_s2x8R"},
                                     {'8', "z4Z^$b"},
                                     {'9', ".^4sSF"},
                                     {'0', "X+@g8]"},
                                     {'a', "rAG=3F"},
                                     {'b', "8J)bAZ"},
                                     {'c', "FEL8=a"},
                                     {'d', "G7fE*Q"},
                                     {'e', "B{68%r"},
                                     {'f', "*2dX(b"},
                                     {'g', "s(B8uE"},
                                     {'h', "_Fwd3q"},
                                     {'i', "hCU!4Q"},
                                     {'j', "_K^7qn"},
                                     {'k', "a?2Y@j"},
                                     {'l', "$]6fPX"},
                                     {'m', "(B]vT4"},
                                     {'n', "@4R/7w"},
                                     {'o', "d%2@P("},
                                     {'p', "qE2!U&"},
                                     {'q', "%Vm=6K"},
                                     {'r', ".X7p!E"},
                                     {'s', "V53tP="},
                                     {'t', "bD>7Xp"},
                                     {'u', "W{(J7a"},
                                     {'v', "{h8HM6"},
                                     {'w', "6Vu{G4"},
                                     {'x', "7B9N*y"},
                                     {'y', "=U7^Cm"},
                                     {'z', "]B49tp"},
                                     {'A', ".=Nd(9"},
                                     {'B', "[M4fgS"},
                                     {'C', "W_sX7V"},
                                     {'D', "8Q2z}s"},
                                     {'E', "&g{y6Y"},
                                     {'F', "$9J}rK"},
                                     {'G', "$2MF^c"},
                                     {'H', "4e-{=A"},
                                     {'I', "jDN$3B"},
                                     {'J', "93V%dP"},
                                     {'K', "+E^W9d"},
                                     {'L', "PR8Z>r"},
                                     {'M', "g{R@39"},
                                     {'N', "4JX$y3"},
                                     {'O', "Gvk3)!"},
                                     {'P', "pU9/sh"},
                                     {'Q', "2Efr=)"},
                                     {'R', "UEh(4>"},
                                     {'S', "}5xE!D"},
                                     {'T', "zTb+]6"},
                                     {'U', "eMQ6x("},
                                     {'V', "t[Lc3D"},
                                     {'W', "3}MEKb"},
                                     {'X', "H%9UFs"},
                                     {'Y', "fDa)4m"},
                                     {'Z', "M7L!qS"},
                                     {'!', "!9tXc7"},
                                     // "
                                     {'\u0022', "Kpg3t%"},
                                     {'#', "-^bA*3"},
                                     {'$', "Sk4/Pd"},
                                     {'%', "hc7-4X"},
                                     {'&', "fm2G!U"},
                                     // '
                                     {'\u0027', "j*43@V"},
                                     {'(', "V6T^Fm"},
                                     {')', "43}jAU"},
                                     {'*', "hFDY_9"},
                                     {'+', "LyZ@6T"},
                                     {',', "W9@_c}"},
                                     {'-', "Bn7)9Q"},
                                     {'.', "y(XH^4"},
                                     {':', "Lz7^3{"},
                                     {';', "bgL9=H"},
                                     {'<', "m6-GJv"},
                                     {'=', "%JXc{5"},
                                     {'>', "JY!4^x"},
                                     {'?', "by+kS2"},
                                     {'@', "Qx/R8q"},
                                     {'[', "5j(kvH"},
                                     {']', "^7wE8F"},
                                     // Backslash
                                     {'\u005C', "p*=uD8"},
                                     {'^', "G/9zyL"},
                                     {'_', "pwR}2."},
                                     {'`', "Q-Kf]9"},
                                     {'{', "aBxz$4"},
                                     {'|', "Kb2@^)"},
                                     {'}', "wKA87!"},
                                     {'~', "G[2Zk4"},
                                     {'/', "Ijgh^f"}
    };
    std::string hashed_password{};

    //Заменяем каждый символ в пароле на соответствующий ему из карты
    for(int i{0}; i<password.length(); i++){
        hashed_password.append(hash_map[password[i]]);
    }

    return hashed_password;
}

std::string dehash_password(const std::string& password){
    //хэш карта
    std::map<std::string , char> hash_map = {
            {"HN?Pv3", '0'},
            {"DJDfjh", '1'},
            {"udGH4!", '2'},
            {"SKUr8_", '3'},
            {"G8g?pZ", '4'},
            {"d7A4G+", '5'},
            {"8-=%We", '6'},
            {"_s2x8R", '7'},
            {"z4Z^$b", '8'},
            {".^4sSF", '9'},
            {"X+@g8]", '0'},
            {"rAG=3F", 'a'},
            {"8J)bAZ", 'b'},
            {"FEL8=a", 'c'},
            {"G7fE*Q", 'd'},
            {"B{68%r", 'e'},
            {"*2dX(b", 'f'},
            {"s(B8uE", 'g'},
            {"_Fwd3q", 'h'},
            {"hCU!4Q", 'i'},
            {"_K^7qn", 'j'},
            {"a?2Y@j", 'k'},
            {"$]6fPX", 'l'},
            {"(B]vT4", 'm'},
            {"@4R/7w", 'n'},
            {"d%2@P(", 'o'},
            {"qE2!U&", 'p'},
            {"%Vm=6K", 'q'},
            {".X7p!E", 'r'},
            {"V53tP=", 's'},
            {"bD>7Xp", 't'},
            {"W{(J7a", 'u'},
            {"{h8HM6", 'v'},
            {"6Vu{G4", 'w'},
            {"7B9N*y", 'x'},
            {"=U7^Cm", 'y'},
            {"]B49tp", 'z'},
            {".=Nd(9", 'A'},
            {"[M4fgS", 'B'},
            {"W_sX7V", 'C'},
            {"8Q2z}s", 'D'},
            {"&g{y6Y", 'E'},
            {"$9J}rK", 'F'},
            {"$2MF^c", 'G'},
            {"4e-{=A", 'H'},
            {"jDN$3B", 'I'},
            {"93V%dP", 'J'},
            {"+E^W9d", 'K'},
            {"PR8Z>r", 'L'},
            {"g{R@39", 'M'},
            {"4JX$y3", 'N'},
            {"Gvk3)!", 'O'},
            {"pU9/sh", 'P'},
            {"2Efr=)", 'Q'},
            {"UEh(4>", 'R'},
            {"}5xE!D", 'S'},
            {"zTb+]6", 'T'},
            {"eMQ6x(", 'U'},
            {"t[Lc3D", 'V'},
            {"3}MEKb", 'W'},
            {"H%9UFs", 'X'},
            {"fDa)4m", 'Y'},
            {"M7L!qS", 'Z'},
            {"!9tXc7", '!'},
            // "
            {"Kpg3t%", '\u0022'},
            {"-^bA*3", '#'},
            {"Sk4/Pd", '$'},
            {"hc7-4X", '%'},
            {"fm2G!U", '&'},
            // '
            {"j*43@V", '\u0027'},
            {"V6T^Fm", '('},
            {"43}jAU", ')'},
            {"hFDY_9", '*'},
            {"LyZ@6T", '+'},
            {"W9@_c}", ','},
            {"Bn7)9Q", '-'},
            {"y(XH^4", '.'},
            {"Lz7^3{", ':'},
            {"bgL9=H", ';'},
            {"m6-GJv", '<'},
            {"%JXc{5", '='},
            {"JY!4^x", '>'},
            {"by+kS2", '?'},
            {"Qx/R8q", '@'},
            {"5j(kvH", '['},
            {"^7wE8F", ']'},
            // Backslash
            {"p*=uD8", '\u005C'},
            {"G/9zyL", '^'},
            {"pwR}2.", '_'},
            {"Q-Kf]9", '`'},
            {"aBxz$4", '{'},
            {"Kb2@^)", '|'},
            {"wKA87!", '}'},
            {"G[2Zk4", '~'},
            {"Ijgh^f", '/'}
    };

    std::string dehashed_password{};
    //длина хэша на один символ (в нашем случае это 6 хэш-символов на один обычный символ), если ты будешь делать свою хэш карту, то учти что нужно изменить значение этой переменной
    int hash_len{6};

    for(int i{0}; i<password.length()/hash_len; i++){
        //Добавляем в конец нашего dehashed_password значение с карты
            //hash_map.find() => ищём ключ
            //text.substr(i*hash_len, hash_len) => название ключа для поиска
            //->second => значение этого ключа
        int start = i*hash_len;
        int step = hash_len;
        std::string hashed_letter = password.substr(start, step);
        dehashed_password.push_back(hash_map.find(hashed_letter)->second);
    }

    return dehashed_password;
}

std::string hash_file(std::string text){
    //хэш карта
    std::map<char, std::string> hash_map = {
                                        {'0', "6@{bR="},
                                        {'1', "qT28]+"},
                                        {'2', "{8]Vz+"},
                                        {'3', "2$A;gv"},
                                        {'4', "3Hn$>$"},
                                        {'5', "-Hp%59"},
                                        {'6', "wgT0<R"},
                                        {'7', "cQLAV*"},
                                        {'8', "mz(jvp"},
                                        {'9', "C>G.a$"},
                                        {'0', "=bU]^e"},
                                        {'a', "[Gm}mc"},
                                        {'b', "dGdJ{o"},
                                        {'c', "ug{<#@"},
                                        {'d', "Xe5v9f"},
                                        {'e', "#<qDxz"},
                                        {'f', "<[}vsx"},
                                        {'g', "#e.W0f"},
                                        {'h', "e+{+^."},
                                        {'i', "S:%Xl}"},
                                        {'j', ")MAL}1"},
                                        {'k', "re@8Z<"},
                                        {'l', "GTf>.Z"},
                                        {'m', "1zdGfb"},
                                        {'n', ">:TuKE"},
                                        {'o', "!h<Mj?"},
                                        {'p', "Ctl.nH"},
                                        {'q', "!8-{<f"},
                                        {'r', "bzo<zo"},
                                        {'s', "ul2]e?"},
                                        {'t', "jGrtcj"},
                                        {'u', "<E9!:5"},
                                        {'v', "k.Je-("},
                                        {'w', "dfS<Tc"},
                                        {'x', "dQNH3{"},
                                        {'y', "J<(zeR"},
                                        {'z', "efCI]!"},
                                        {'A', "XG=:<A"},
                                        {'B', "B(RB-G"},
                                        {'C', "og/#kT"},
                                        {'D', "c+ZpjQ"},
                                        {'E', "dI9v5e"},
                                        {'F', "wT:DH4"},
                                        {'G', "?tIXEU"},
                                        {'H', "Z1ljb0"},
                                        {'I', "eLGroG"},
                                        {'J', "4Mv:Q6"},
                                        {'K', "yHqKSh"},
                                        {'L', "]J{R.e"},
                                        {'M', "rNYbPq"},
                                        {'N', "cF0-F)"},
                                        {'O', ".K=8!>"},
                                        {'P', "oXW+3*"},
                                        {'Q', "jgdfFk"},
                                        {'R', "KI<#xH"},
                                        {'S', "y@6(eY"},
                                        {'T', "xL}qRS"},
                                        {'U', "G/-1Bo"},
                                        {'V', "nl@?/["},
                                        {'W', "i5w5H+"},
                                        {'X', "xZYJUo"},
                                        {'Y', "wXo8#v"},
                                        {'Z', ",X96fL"},
                                        {'!', "dZC-gE"},
                                        // "
                                        {'\u0022', "CjKOsw"},
                                        {'#', "^6p>&#"},
                                        {'$', "&%*9Up"},
                                        {'%', ".bX[Dn"},
                                        {'&', "S/inT$"},
                                        // '
                                        {'\u0027',"u2GBlT"},
                                        {'(', "D<3-Z:"},
                                        {')', "%J?3wT"},
                                        {'*', "9FG){."},
                                        {'+', "K?q=kd"},
                                        {',', "huMK8N"},
                                        {'-', "VZ<>!G"},
                                        {'.', "w%3bQ>"},
                                        {':', "kSl=pu"},
                                        {';', "ZjQtd("},
                                        {'<', "kDlmF,"},
                                        {'=', "TC3KU%"},
                                        {'>', "gA}?vV"},
                                        {'?', "UM]OoH"},
                                        {'@', "!bKBC7"},
                                        {'[', "%E)jz["},
                                        {']', "+AeN,}"},
                                        // Backslash
                                        {'\u005c', "nd8WV&"},
                                        {'^', "IItB7V"},
                                        {'_', "e*#]8d"},
                                        {'`', "u#[]n,"},
                                        {'{', "=rQnL$"},
                                        {'|', ")a:2ET"},
                                        {'}', "d06CY1"},
                                        {'~', "lFyppI"},
                                        {'\n',"OfH%6N"},
                                        {' ', "PHnd5S"},
                                        {'/', "a&kfHD"}
    };
    std::string hashed_text{};

    //Заменяем каждый символ в тексте на соответствующий ему из map
    for(int i{0}; i<text.length(); i++){
        hashed_text.append(hash_map[text[i]]);
    }

    return hashed_text;
}

std::string dehash_file(const std::string& text){
    std::map<std::string, char> hash_map = {
            {"6@{bR=", '0'},
            {"qT28]+", '1'},
            {"{8]Vz+", '2'},
            {"2$A;gv", '3'},
            {"3Hn$>$", '4'},
            {"-Hp%59", '5'},
            {"wgT0<R", '6'},
            {"cQLAV*", '7'},
            {"mz(jvp", '8'},
            {"C>G.a$", '9'},
            {"=bU]^e", '0'},
            {"[Gm}mc", 'a'},
            {"dGdJ{o", 'b'},
            {"ug{<#@", 'c'},
            {"Xe5v9f", 'd'},
            {"#<qDxz", 'e'},
            {"<[}vsx", 'f'},
            {"#e.W0f", 'g'},
            {"e+{+^.", 'h'},
            {"S:%Xl}", 'i'},
            {")MAL}1", 'j'},
            {"re@8Z<", 'k'},
            {"GTf>.Z", 'l'},
            {"1zdGfb", 'm'},
            {">:TuKE", 'n'},
            {"!h<Mj?", 'o'},
            {"Ctl.nH", 'p'},
            {"!8-{<f", 'q'},
            {"bzo<zo", 'r'},
            {"ul2]e?", 's'},
            {"jGrtcj", 't'},
            {"<E9!:5", 'u'},
            {"k.Je-(", 'v'},
            {"dfS<Tc", 'w'},
            {"dQNH3{", 'x'},
            {"J<(zeR", 'y'},
            {"efCI]!", 'z'},
            {"XG=:<A", 'A'},
            {"B(RB-G", 'B'},
            {"og/#kT", 'C'},
            {"c+ZpjQ", 'D'},
            {"dI9v5e", 'E'},
            {"wT:DH4", 'F'},
            {"?tIXEU", 'G'},
            {"Z1ljb0", 'H'},
            {"eLGroG", 'I'},
            {"4Mv:Q6", 'J'},
            {"yHqKSh", 'K'},
            {"]J{R.e", 'L'},
            {"rNYbPq", 'M'},
            {"cF0-F)", 'N'},
            {".K=8!>", 'O'},
            {"oXW+3*", 'P'},
            {"jgdfFk", 'Q'},
            {"KI<#xH", 'R'},
            {"y@6(eY", 'S'},
            {"xL}qRS", 'T'},
            {"G/-1Bo", 'U'},
            {"nl@?/[", 'V'},
            {"i5w5H+", 'W'},
            {"xZYJUo", 'X'},
            {"wXo8#v", 'Y'},
            {",X96fL", 'Z'},
            {"dZC-gE", '!'},
            {"CjKOsw", '\u0022'},
            {"^6p>&#", '#'},
            {"&%*9Up", '$'},
            {".bX[Dn", '%'},
            {"S/inT$", '&'},
            {"u2GBlT", '\u0027'},
            {"D<3-Z:", '('},
            {"%J?3wT", ')'},
            {"9FG){.", '*'},
            {"K?q=kd", '+'},
            {"huMK8N", ','},
            {"VZ<>!G", '-'},
            {"w%3bQ>", '.'},
            {"kSl=pu", ':'},
            {"ZjQtd(", ';'},
            {"kDlmF,", '<'},
            {"TC3KU%", '='},
            {"gA}?vV", '>'},
            {"UM]OoH", '?'},
            {"!bKBC7", '@'},
            {"%E)jz[", '['},
            {"+AeN,}", ']'},
            {"nd8WV&", '\u005c'},
            {"IItB7V", '^'},
            {"e*#]8d", '_'},
            {"u#[]n,", '`'},
            {"=rQnL$", '{'},
            {")a:2ET", '|'},
            {"d06CY1", '}'},
            {"lFyppI", '~'},
            {"OfH%6N",'\n'},
            {"PHnd5S", ' '},
            {"a&kfHD", '/'}
    };
    std::string dehashed_text{};
    //длина хэша на один символ (в нашем случае это 6 хэш-символов на один обычный символ), если ты будешь делать свою хэш карту, то учти что нужно изменить значение этой переменной
    int hash_len{6};

    for(int i{0}; i<text.length()/hash_len; i++){
        //Добавляем в конец нашего dehashed_text значение с карты
            //hash_map.find() => ищём ключ
            //text.substr(i*hash_len, hash_len) => название ключа для поиска
            //->second => значение этого ключа
        dehashed_text.push_back(hash_map.find(text.substr(i*hash_len, hash_len))->second);
    }

    return dehashed_text;
}

//только в конце заметил что интерфейс программы на английском, а комментарии на русском. Ахереть я гений конечно.