#define WIRE_RESET_SPEED 207
#define WIRE_DATA_SPEED 16

#define WIRE_SLOT_RESET 0xF0
#define WIRE_SLOT_SET 0xFF
#define WIRE_SLOT_CLEAR 0x00

//----------------------------------Инициализация oneWire----------------------------------
void oneWireInit(void) //инициализация oneWire
{
  UBRR0 = WIRE_RESET_SPEED; //устанавливаем битрейт
  UCSR0A = (0x01 << U2X0); //устанавливаем удвоение скорости
  UCSR0B = ((0x01 << TXEN0) | (0x01 << RXEN0));
  UCSR0C = ((0x01 << UCSZ01) | (0x01 << UCSZ00));
}
//----------------------------------Сброс шины----------------------------------
boolean oneWireReset(void) //сброс шины
{
  UBRR0 = WIRE_RESET_SPEED; //устанавливаем скорость UART для сброса шины
  UCSR0A |= (0x01 << TXC0); //сбрасываем флаг передачи
  UDR0 = WIRE_SLOT_RESET; //отправляем сигнал сброса
  while (UCSR0A & (0x01 << TXC0)); //ждем окончания передачи
  return UDR0 == WIRE_SLOT_RESET; //возвращаем результат сброса
}
//----------------------------------Отправка данных----------------------------------
void oneWireWrite(uint8_t data) //отправка данных
{
  UBRR0 = WIRE_DATA_SPEED; //устанавливаем скорость UART для передачи данных
  for (uint8_t i = 0; i < 8; i++) {
    UCSR0A |= (0x01 << TXC0); //сбрасываем флаг передачи
    UDR0 = (data & (0x01 << i)) ? WIRE_SLOT_SET : WIRE_SLOT_CLEAR; //отправляем бит данных
    while (UCSR0A & (0x01 << TXC0)); //ждем окончания передачи
  }
}
//----------------------------------Прием данных----------------------------------
uint8_t oneWireRead(void) //прием данных
{
  uint8_t data = 0; //буфер приема данных
  UBRR0 = WIRE_DATA_SPEED; //устанавливаем скорость UART для приема данных
  for (uint8_t i = 0; i < 8; i++) {
    UCSR0A |= (0x01 << TXC0); //сбрасываем флаг передачи
    UDR0 = WIRE_SLOT_SET; //отправляем сигнал начала чтения бита
    while (UCSR0A & (0x01 << TXC0)); //ждем окончания передачи
    data |= (UDR0 != WIRE_SLOT_SET) ? 0x00 : (0x01 << i); //записываем принятый бит
  }
  return data; //возвращаемя байт данных
}
