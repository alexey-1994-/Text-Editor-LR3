/*
redraw написан очень плохо-необходимо создать массив,
копировать туда список и отслеживать изменения,перерисовывать только изменения,
тогда скорость отрисовки будет мгновенная

при копировании нескольких строк невозможно перейти к последним

при вырезании последних 2-х символов выскакивает ошибка

к четвергу постараюсь доработать

навигация:

ctrl+i - вверх
ctrl+k - вниз
ctrl+j - влево
ctrl+l - вправо

управление:

1-выделение(наощупь-пометил начало,пометил конец)
2-вставка выделенного,т.е копирование
3-удаление выделенного(работает как вырезка,содержимое не удаляется из буфера)
*/
#include <iostream>
#include <fstream>
using namespace std;
#include <conio.h>
#include <windows.h>
#include "Spisok.h"

void gotoxy(short x, short y)
{
    HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position= {x,y};
    SetConsoleCursorPosition(hStdout,position);
}

void redraw(Spisok *buf);
void save(char *name, Spisok *text);
int position_to_cursor(Spisok *text, int x, int y);
int size_string(Spisok *text, int y);

int main()
{
    char temp;
    Spisok text, selection;
    unsigned char key=0;
    int x=0,y=0,max_y=0, x_select=0,y_select=0;
    bool select=false;
    text.add_element(' ');

    while(true)
    {
        if(kbhit())
        {
            key=getch();

            if((175>=key && key>=52)||(key<=48 && key>=32) || (key>=224 && key<=241) || key==13)//исключаем управляющие кнопки
                if(true)
                {
                    x++;

                    if(key==13)
                    {
                        cout<<endl;
                        max_y++;
                        y++;
                        x=0;
                    }
                    else cout<<key;
                    text.insert_element(key,position_to_cursor(&text,x-1,y));
                    redraw(&text);
                    gotoxy(x,y);
                }
        }
        switch(key)
        {
            //--------------навигация---------------------//
        case 10:                                //ctrl+j
            if(x>0) x--;
            else
            {
                if(y>0)
                {
                    y--;
                    x=size_string(&text,y)-1;
                }
            }
            gotoxy(x,y);
            break;
        case 12:        //ctrl+l
            if(x<size_string(&text,y)-1) x++;
            else
            {
                if(y<max_y)
                {
                    x=0;
                    y++;
                }
            }
            gotoxy(x,y);
            break;
        case 11:        //ctrl+k
            if(y<max_y)
            {
                y++;
                if(size_string(&text,y)-1<x)
                    x=size_string(&text,y)-1;
                gotoxy(x,y);
            }
            break;
        case 9:          //ctrl+i
            if(y>0)
            {
                y--;
                if(size_string(&text,y)-1<x)
                    x=size_string(&text,y)-1;
                gotoxy(x,y);
            }
            break;
            //-------------управление---------------
        case 8:
            text.delete_element(position_to_cursor(&text,x,y)-1);
            x--;
            redraw(&text);
            gotoxy(x,y);
            break;   //backspace
        case '1':
            if(select)
            {
                select=false;
                if(position_to_cursor(&text,x,y)<position_to_cursor(&text,x_select,y_select))
                    for(int i=position_to_cursor(&text,x,y); i<position_to_cursor(&text,x_select,y_select); i++)
                        selection.add_element(text.find(i));
                else
                    for(int i=position_to_cursor(&text,x_select,y_select); i<position_to_cursor(&text,x,y); i++)
                        selection.add_element(text.find(i));
            }
            else
            {
                select=true;
                x_select=x;
                y_select=y;
                selection.~Spisok();
            }
            break;
        case '2':                                       //копирование
            for(int i=0; i<selection.get_size(); i++)
                text.insert_element(selection.find(i),i+position_to_cursor(&text,x,y));
            redraw(&text);
            gotoxy(x,y);
            break;
        case '3':                                        //вырезка
            for(int i=0; i<selection.get_size(); i++)
            {
                if(position_to_cursor(&text,x_select,y_select)<position_to_cursor(&text,x,y))
                    text.delete_element(i+position_to_cursor(&text,x_select,y_select));
                if(position_to_cursor(&text,x_select,y_select)>position_to_cursor(&text,x,y))
                    text.delete_element(i+position_to_cursor(&text,x,y));
            }
            x_select=x;
            y_select=y;    //повторно не вырезаем
            redraw(&text);
            gotoxy(x,y);
            break;
        }

        if(key==27)
            break;

        key=0;
        Sleep(1);
    }

    save("d.txt", &text);
    return 0;
}

void save(char *name, Spisok *text)
{
    ofstream f;
    f.open(name);

    for(int i=0; i<text->get_size(); i++)
    {
        if(text->find(i)!=13)
            f<<text->find(i);
        else f<<endl;
    }
    f.close();
}

void redraw(Spisok *buf)
{
    system("cls");
    gotoxy(0,0);
    for(int i=0; i<buf->get_size(); i++)
    {
        if(buf->find(i)==13)
            cout<<endl;
        else
            cout<<buf->find(i);
    }
}

int size_string(Spisok *text, int y)
{
    int size=1, y_t=0, i=0;

    while(y!=y_t)
    {
        if(text->find(i)==13)
            y_t++;
        i++;
    }
    while(true)
    {
        if(i>=text->get_size() || text->find(i)==13 || size>79)
            break;
        i++;
        size++;
    }

    return size;
}

int position_to_cursor(Spisok *text, int x, int y)
{
    int size=x;

    for(int i=0; i<y; i++)
        size+=size_string(text,i);

    return size;
}

