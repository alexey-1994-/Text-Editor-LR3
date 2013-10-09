#include <iostream>
using namespace std;
#include "Spisok.h"

bool Spisok::add_element(Other data)
{
    Spis *temp=new Spis;
    if(temp==0)
        return false;

    if(size==0)
    {
        begin=temp;
        end=temp;
    }
    end->next=temp;
    end=temp;
    end->data=data;
    end->next=0;

    size++;
    return true;
}

bool Spisok::insert_element(Other data, int position)
{
    Spis *temp=new Spis, *tmp=new Spis;
    if(temp==0)
        return false;
    if(position>size || position<0)
        return false;

    if(position==0)
    {
        temp->next=begin;                 //перестановка с begin
        begin=temp;
        temp->data=data;
        size++;
        return true;
    }

    tmp=begin;                            //переходим к заданной позиции
    for(int i=0; i<position-1; i++)
        tmp=tmp->next;

    temp->next=tmp->next;                 //перестановка
    tmp->next=temp;
    temp->data=data;

    size++;
    return true;
}

bool Spisok::delete_element(int position)
{
    if(position>size || position<0)
        return false;
    Spis *tmp_1=new Spis, *tmp_2=new Spis;

    if(position==0)
    {
        tmp_1=begin->next;
        delete begin;
        begin=tmp_1;
        size--;
        return true;
    }

    tmp_1=begin;                            //переходим к заданной позиции
    for(int i=0; i<position-1; i++)
        tmp_1=tmp_1->next;

    tmp_2=tmp_1->next;
    tmp_1->next=tmp_2->next;
    delete tmp_2;
    size--;
    return true;
}

void Spisok::show()
{
    Spis *temp=new Spis;

    temp=begin;
    for(int i=0; i<size; i++)
    {
        cout<<temp->data<<' ';
        temp=temp->next;
    }
}

Other Spisok::find(int position)
{
    Spis *temp=new Spis;

    temp=begin;
    for(int i=0; i<position; i++)
        temp=temp->next;

    return temp->data;
}

Spisok::~Spisok()
{
    int tmp=size;

    for(int i=0; i<tmp; i++)
        delete_element(0);
}

