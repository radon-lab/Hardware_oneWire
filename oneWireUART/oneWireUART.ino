#include "oneWire.h"

int main(void)  //инициализация
{
  for (;;) {
    
  }
  return 0;
}

//--------------------------------------------------------------------------------------
void dallas_requestTemp(void) {
  if (oneWireReset()) return;
  oneWireWrite(0xCC); //обращение ко всем датчикам
  oneWireWrite(0x44); //запрос на преобразование температуры
}
//--------------------------------------------------------------------------------------
float dallas_readTemp(void) {
  if (oneWireReset()) return 0;
  oneWireWrite(0xCC); //обращение ко всем датчикам
  oneWireWrite(0xBE); //запрос на чтение температуры
  return ((float)(oneWireRead() | (oneWireRead() << 8)) * 0.0625); //преобразуем в градусы
}
