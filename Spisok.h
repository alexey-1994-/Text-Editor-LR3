#ifndef SPISOK_H_INCLUDED
#define SPISOK_H_INCLUDED

typedef char Other;

class Spisok
{
public:
    bool add_element(Other data);
    bool insert_element(Other data, int position);  //вставить элемент после н-ного числа
    bool delete_element(int position);

    Other find(int position);

    int get_size(){return size;};
    void show();

    Spisok():begin(0),size(0){};
    ~Spisok();
private:
    struct Spis
    {
        Other data;
        Spis *next;
    };
    Spis *begin;
    Spis *end;
    int size;
};

#endif // SPISOK_H_INCLUDED
