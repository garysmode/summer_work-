#include "pch.h" // подклчение библиотек
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS

#define ENTER 13 // объявление клавиш
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79


using namespace std;
using namespace System;
using namespace System::IO;

struct User {
    char nicname[20]; // ник игрока
    char pass[15]; // пароль
    long summ; // количество денег на счету
    char date_of_registration[11]; // дата регистрации аккаунта
    char country[25]; // страна проживания пользователя
    long number_of_games; // количество купленных видеоигр
    long games[20]; // закодированные названия видеоигр
};
struct Game
{
    char name_game[44]; // название видеоигры
};

struct sp {
    char nicname[20]; // ник игрока
    char country[25]; // страна проживания пользователя
    long number_of_games; // количество купленных видеоигр
    struct sp* sled; // указатель на следующий элемент в списке
    struct sp* pred; // указатель на предыдущий элемент в списке
}; // шаблон, глобальное описание структуры двустороннего списка



// шаблоны функций
int menu(int); // функция меню
void Users_games(struct User*, struct Game*, int); // Функция просмотра приобретенных пользователем видеоигр
void Who_has_more(struct User*, int); // Функция поиска пользователя с наибольшим количеством видеоигр
void Who_has_prog(struct User*, struct Game*, int, int); // Функция поиска соответствия (проверяется наличие доступа к видеоигре у пользователей)
void Alf_sp(struct User*, int, sp**); // Функция формирования алфавитного и обратного списков
void vstavka(struct User*, char*, char*, long, sp**); // функция вставки в список
void diagram(struct User*, int, sp**); // Функция составления диаграммы (показывает соотношение количества видеоигр у пользователей)
void dif_question(User*, int); // Функция проверки наличия пользователей из России с одинаковым количеством видеоигр

int WindowWidth = 1024;
int WindowHeight = 100;

int main(array<System::String^>^ args)
{
    setlocale(LC_CTYPE, "Russian"); // установка кодировки, поддерживающей Руссский язык
    Console::CursorVisible::set(false); // отключение видимости курсора
    Console::BufferHeight = WindowHeight; // установка высоты буфера равной высоте окна
    Console::BufferWidth = WindowWidth; // установка ширины буфера равной высоте окна

    //задание пунктов меню
    char dan[7][75] = {
    "Какие видеоигы есть у пользователя?                                       ",
    "У кого больше всего видеоигр?                                             ",
    "Есть ли такая видеоигра у пользователей?                                  ",
    "Алфавитный и обратный список всех пользователей                           ",
    "Есть ли пользователи из России с одинаковым количеством видеоигр          ",
    "Соотношение количества видеоигр у пользователей                           ",
    "Выход                                                                     "
    };
    // пустая линия для создания меню
    char BlankLine[100] = "                                                                            ";

    int USERS_NC; // объявление переменной, обозначающей количество позиций в списке пользователей
    int GAMES_NC; // объявление глобальной переменной, обозначающей количество позиций в списке видеоигр
    struct sp *spisok = 0;
    // Чтение файла Users.dat
    FILE* in; // объявление переменной под файл
    User* users = 0; // объявление переменной под масив данных
    if ((in = fopen("Users.dat", "r")) == NULL) // открытие файла
    {
        // в случае неудачного открытия файла
        printf("\nФайл Users.dat не открыт !");
        getch(); exit(1);
    }
    fscanf(in, "%d", &USERS_NC); // считывани количества эллементов в файле
    users = (User*)malloc(USERS_NC * sizeof(User)); // выделение памяти
    for (int i = 0; i < USERS_NC; i++) {
        fscanf(in, "%s%s%ld%s%s%ld", users[i].nicname,
            users[i].pass, &users[i].summ,
            users[i].date_of_registration, users[i].country, &users[i].number_of_games);
        for (int j = 0; j < users[i].number_of_games; j++)
        {
            fscanf(in, "%ld", &users[i].games[j]);
        }

    }
    // Вывод таблицы
    printf("Ник игрока   Пароль        Кол-во денег Дата регистрации Кол-во игр");
    for (int i = 0; i < USERS_NC; i++)
        printf("\n%-12s %-13s %-12ld %-16s %5ld",
            users[i].nicname,
            users[i].pass, users[i].summ,
            users[i].date_of_registration, users[i].number_of_games);
    getch();

    //чтение файла Games.dat
    FILE* games_file; // объявление переменной под файл
    Game* games = 0; // объявление переменной под масив данных
    if ((games_file = fopen("Games.dat", "r")) == NULL) // открытие файла
    {
        printf("\nФайл Games.dat не открыт !");
        getch(); exit(1);
    }
    fscanf(games_file, "%d", &GAMES_NC); // считывани количества эллементов в файле
    games = (struct Game*)malloc(GAMES_NC * sizeof(struct Game)); // выделение памяти
    for (int i = 0; i < GAMES_NC; i++) {
        fscanf(games_file, "%s", games[i].name_game);
    }

    // Реализация меню
    int n; // вспомагательная переменная
    while (1) // в цикле создаем область для вопросов и окрашиваем ее в цвета
    {
        // устанавливаем для букв цвет и подсветку для выбранного вопроса
        Console::ForegroundColor = ConsoleColor::Black;
        Console::BackgroundColor = ConsoleColor::DarkBlue;
        Console::Clear();
        Console::ForegroundColor = ConsoleColor::White;
        Console::BackgroundColor = ConsoleColor::DarkCyan;
        // устанавливаем первую точку, откуда будем закрашивать область меню
        Console::CursorLeft = 10;
        Console::CursorTop = 4;
        printf(BlankLine);
        // размещение вопросов
        for (int i = 0; i < 7; i++)
        {
            Console::CursorLeft = 10;
            Console::CursorTop = i + 5;
            printf(" %s ", dan[i]);
        }
        // устанавливаем последнюю точку, до куда будем закрашивать область меню
        Console::CursorLeft = 10;
        Console::CursorTop = 12;
        printf(BlankLine);
        n = menu(7); // выбор вопроса в меню
        switch (n) {
        case 1: Users_games(users, games, USERS_NC); break;
        case 2: Who_has_more(users, USERS_NC); break;
        case 3: Who_has_prog(users, games, USERS_NC, GAMES_NC); break;
        case 4: Alf_sp(users, USERS_NC, &spisok); break;
        case 5: dif_question(users, USERS_NC); break;
        case 6: diagram(users, USERS_NC, &spisok); break;
        case 7: exit(0);
        }
    } // конец while (1)

    return 0;
}


int menu(int n)
{
    //задание пунктов меню
    char dan[7][75] = {
    "Какие видеоигы есть у пользователя?                                       ",
    "У кого больше всего видеоигр?                                             ",
    "Есть ли такая видеоигра у пользователей?                                  ",
    "Алфавитный и обратный список всех пользователей                           ",
    "Есть ли пользователи из России с одинаковым количеством видеоигр          ",
    "Соотношение количества видеоигр у пользователей                           ",
    "Выход                                                                     "
    };
    int y1 = 0, y2 = n - 1; // устанавливаем выделение для строчки
    char c = 1;
    while (c != ESC) // при нажатии кнопки ESC меню закроется
    {
        switch (c) {
        case DOWN: y2 = y1; y1++; break; // программирование кнопки DOWN, которая позволяет перемещаться вниз по меню
        case UP: y2 = y1; y1--; break; // программирование кнопки UP, которая позволяет перемещаться вверх по меню
        case ENTER: return y1 + 1; // программирование кнопки ENTER, которая позволяет выбрать вопрос из меню
        case HOME: y2 = y1; y1 = 0; break; // программирование кнопки HOME, которая позволяет вернуться на первый вопрос меню
        case END: y2 = y1; y1 = n - 1; break; // программирование кнопки END, которая позволяет вернуться на последний вопрос меню
        }
        if (y1 > n - 1) { y2 = n - 1; y1 = 0; } // условие, когда срабатывает кнопка DOWN
        if (y1 < 0) { y2 = 0; y1 = n - 1; } // условие, когда срабатывает кнопка UP
        // при выборе вопроса буквы окрашиваются в белый цвет 
        Console::ForegroundColor = ConsoleColor::White;
        // при выборе вопроса задний план за буквами окрашивается в синий цвет
        Console::BackgroundColor = ConsoleColor::Blue;
        Console::CursorLeft = 11;
        Console::CursorTop = y1 + 5;
        printf("%s", dan[y1]);
        Console::ForegroundColor = ConsoleColor::White;
        Console::BackgroundColor = ConsoleColor::DarkCyan;
        Console::CursorLeft = 11;
        Console::CursorTop = y2 + 5;
        printf("%s", dan[y2]);
        c = getch();
    } // конец while (c != ESC)
    exit(0);
}

void Users_games(struct User* users, struct Game* games, int USERS_NC) {
    char nic[20]; // строка в которую пользователь записывает ник игрока 
    User* find; // переменная для найденного игрока
    Console::Clear();
    Console::BackgroundColor = ConsoleColor::DarkCyan;
    Console::ForegroundColor = ConsoleColor::White;
    Console::CursorTop = 2;
    Console::CursorLeft = 49;

    printf("Введите ник игрока: ");
    SetConsoleCP(1251); // замена кодировки для коректного ввода
    scanf("%20s", nic); // ввод пользователем ника игрока
    SetConsoleCP(866); // замена кодировки для коректного вывода
    // перебор ников из файла
    for (int i = 0; i < USERS_NC; i++) {
        if (!(strcmp(users[i].nicname, nic))) { // сравнение введеного ника и ника из списка
            find = &users[i];
            break;
        }
        if (i == USERS_NC - 1) { // если ника нет, выводиться уведомление 
            Console::CursorLeft = 49;
            printf("Такого пользователя нет!");
            getch();
            return;
        }
    }
    int j = 0; // переменная номера выводимой игры
    int curs = 5; // переменная расположения курсора по вертикали
    for (int i = 0; i < find->number_of_games; i++, curs++) { // выведение купленных игр 
        j = find->games[i]; // запись номера видеоигры из списка
        Console::CursorLeft = 10;
        Console::CursorTop = curs;
        printf("%s", games[j].name_game);

    }

    getch();
}

void Who_has_more(struct User* users, int USERS_NC) {
    Console::CursorLeft = 11;
    Console::CursorTop = 15;
    Console::BackgroundColor = ConsoleColor::DarkGray;
    User best; // переменная для найденного игрока
    best.number_of_games = 0; // устанавливаем значение по умолчанию

    for (int i = 0; i < USERS_NC; i++) // перебор игроков
    {
        // сравнение колличества видеоигр между предыдущим игроком с наибольшим количеством и следующим игроком
        if (best.number_of_games < users[i].number_of_games) {
            best = users[i]; // замена игрока с наибольшим колличеством видеоигр
        }
    }
    // вывод ника игрока и количества видеоигр
    printf("Больше всего видеоигр у игрока: %s %d", best.nicname, best.number_of_games);

    getch();
};

void Who_has_prog(struct User* users, struct Game* games, int USERS_NC, int GAMES_NC) {
    Console::CursorLeft = 11;
    Console::CursorTop = 15;
    Console::BackgroundColor = ConsoleColor::DarkGray;

    Game find; // переменная для записи найденной игры
    printf("Введите название видеоигры: ");
    SetConsoleCP(1251); // замена кодировки для коректного ввода
    scanf("%20s", find.name_game); // ввод пользователем названии игры
    SetConsoleCP(866); // замена кодировки для коректного вывода
    int f; // переменная для записи номера найденной игры из списка
    for (f = 0; f < GAMES_NC; f++) // перебор игр из списка
    {
        // сравнение названия игры из списка с названием введеным пользователем
        if (!(strcmp(games[f].name_game, find.name_game)))
        {
            break;
        }
        // если игры, введеной пользователем, нет выводится уведомление
        if (f == GAMES_NC - 1) {
            Console::CursorLeft = 11;
            Console::CursorTop = 16;
            printf("Такой игры нет в базе данных!");
            getch();
            return;
        }
    }

    int count = 0; // счетчик количества игроков с необходимой видеоигрой

    for (int i = 0; i < USERS_NC; i++) // перебор игроков
    {
        for (int j = 0; j < users[i].number_of_games; j++) // перебор видеоигр игрока
        {
            if (users[i].games[j] == f) // сравнение номера видеоигры игрока и номера необходимой видеоигры 
            {
                count++; // при совпадении увеличение счетчика на 1
                break;
            }
        }
    }
    // вывод количества игроков 
    Console::CursorLeft = 11;
    Console::CursorTop = 16;
    printf("Эта игра есть у %d пользователей", count);

    getch();
}

void Alf_sp(User* users, int USERS_NC, sp** spisok) {
    struct sp* nt; // указатель следущего элемента списка
    struct sp* z; // указатель предыдущего элемента списка
    //struct sp* spis; // указатель начала списка
    if (!*spisok) // условие составления нового списка
    {
        for (int i = 0; i < USERS_NC; i++) // перебор всех пользователей
        {
            vstavka(users, users[i].nicname, users[i].country, users[i].number_of_games, spisok); // вставка пользователя в список
        }
    }

    Console::ForegroundColor = ConsoleColor::Black;
    Console::BackgroundColor = ConsoleColor::Gray;
    Console::Clear();
    // вывод алфавитного и обратного списков
    printf("\n Алфавитный список пользователей");
    printf("\n ===============================\n");
    for (nt = *spisok, z = 0; nt != 0; z = nt, nt = nt->sled)
        printf("\n %-20s %-25s", nt->nicname, nt->country);

    int cursortop = 3; // переменная расположения курсора по вертикали
    Console::CursorTop = 1;
    Console::CursorLeft = 35;
    printf(" Обратный алфавитный список пользователей");
    Console::CursorLeft = 35;
    Console::CursorTop = 2;
    printf(" ========================================");
    for (nt = z; nt != 0; nt = nt->pred) {
        cursortop++; // перевод на строку ниже
        Console::CursorLeft = 35;
        Console::CursorTop = cursortop;
        printf(" %-20s %-25s", nt->nicname, nt->country);
    }
    getch();
}

void vstavka(struct User* users, char* nic, char* country, long numb, sp** spisok)
{
    struct sp* nov, * nt, * z = 0;
    for (nt = *spisok;
        nt != 0 && strcmp(nt->nicname, nic) < 0;
        z = nt, nt = nt->sled);

    nov = (struct sp*)malloc(sizeof(struct sp));
    strcpy(nov->nicname, nic);
    nov->sled = nt;
    nov->pred = z;
    nov->number_of_games = numb;
    strcpy(nov->country, country);

    if (!z) *spisok = nov;
    else z->sled = nov;
    if (nt) nt->pred = nov;

    return;
}

void dif_question(User* users, int USERS_NC) {

    for (int i = 0; i < USERS_NC; i++)
    {
        if (!(strcmp(users[i].country, "Россия")))
        {
            for (int j = i + 1; j < USERS_NC; j++)
            {
                if (!(strcmp(users[i].country, "Россия")) && (users[i].number_of_games == users[j].number_of_games))
                {
                    Console::CursorLeft = 11;
                    Console::CursorTop = 15;
                    Console::BackgroundColor = ConsoleColor::DarkGray;
                    printf("Такие игроки есть");
                    Console::CursorLeft = 11;
                    Console::CursorTop = 16;
                    printf("%s %s", users[i].nicname, users[j].nicname);
                    getch();
                    return;
                }
            }

        }

    }
}

void diagram(struct User* users, int USERS_NC, sp** spisok)
{
    struct sp* nt;
    int len, NColor;
    long sum = 0;
    char str1[20];
    char str2[20];
    System::ConsoleColor Color;
    Console::ForegroundColor = ConsoleColor::Black;
    Console::BackgroundColor = ConsoleColor::White;
    Console::Clear();

    for (int i = 0; i < USERS_NC; i++) {
        sum = users[i].number_of_games + sum;
    }

    if (!*spisok)
        for (int i = 0; i < USERS_NC; i++)
            vstavka(users, users[i].nicname, users[i].country, users[i].number_of_games, spisok);
    Color = ConsoleColor::Black;
    NColor = 0;

    int i = 0;
    for (nt = *spisok; nt != 0; nt = nt->sled, i++)
    {
        sprintf(str1, "%s", nt->nicname);
        sprintf(str2, "%3.1f%%", (nt->number_of_games * 100. / sum));
        Console::ForegroundColor = ConsoleColor::Black;
        Console::BackgroundColor = ConsoleColor::White;
        Console::CursorLeft = 5; Console::CursorTop = i + 1;
        printf(str1);
        Console::CursorLeft = 20;
        printf("%s", str2);
        Console::BackgroundColor = ++Color; NColor++;
        Console::CursorLeft = 30;
        for (len = 0; len < nt->number_of_games * 100 / sum; len++) printf(" ");
        if (NColor == 14)
        {
            Color = ConsoleColor::Black; NColor = 0;
        }
    }
    getch();
    return;
}