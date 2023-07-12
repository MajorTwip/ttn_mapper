#pragma once
#include <Arduino.h>
#include <U8x8lib.h>

class U8x8_String_Base{
    public:
        virtual void draw() = 0;
};

template <uint8_t itemnum>
class U8x8_Page
{
    public:
        U8x8_Page(){};
        U8x8_Page(std::initializer_list<U8x8_String_Base*> items){
            for(auto it = items.begin();it!=items.end();it++){
                this->items[index++]=*it;
            }
        }
        void add(U8x8_String_Base * item){
            items[index++]=item;
        }
        void render(){
            for(uint8_t i = 0;i<index;i++){
                items[i]->draw();
            }
        }

    private:
        U8x8_String_Base * items[itemnum];
        uint8_t index = 0;
};

template <uint8_t size>
class U8x8_String:public U8x8_String_Base{
    public:
    
        U8x8_String(uint8_t x, uint8_t y, U8X8 * display,const char * initial_val = "N/A"):x(x),y(y),display(display){
            strcpy(value,initial_val);
            value[size]=0;
        };
        
        void draw(){
            display->drawString(x,y,value);
        };

        void updateRaw(const char * val, uint8_t len){
            memcpy(value,val,min(size,len));
            value[size]=0;
            draw();
        }

        void update(String val){
            String tmp = val.substring(0,size-1);
            while(tmp.length()<size)tmp+=" ";
            updateRaw(tmp.c_str(),size);
        }

    private:
        char value[size+1];    
        const uint8_t x, y;
        U8X8 * display = nullptr;
};