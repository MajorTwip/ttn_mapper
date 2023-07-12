#include <U8x8_Page.hpp>

U8x8_String<16> title(0,0,&display,"Bergmann_TTN-Map");
U8x8_String<5> lbl_GPS(0,1,&display,"GPS: "); U8x8_String<11> info_GPS(5,1,&display); 
U8x8_String<5> lbl_Pos(0,2,&display,"POS: "); U8x8_String<11> info_Pos(5,2,&display); 
U8x8_String<5> lbl_Alt(0,3,&display,"ALT: "); U8x8_String<11> info_Alt(5,3,&display); 
U8x8_String<5> lbl_LORA(0,4,&display,"WAN: "); U8x8_String<11> info_LORA(5,4,&display); 

U8x8_String<16> info_Time(0,7,&display); 


U8x8_Page<10> page1 = {&title,
                        &lbl_GPS, &info_GPS,
                        &lbl_Pos, &info_Pos,
                        &lbl_Alt, &info_Alt,
                        &lbl_LORA, &info_LORA,
                        
                        &info_Time};


