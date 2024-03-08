// USER INCLUDES
#include <SI_EFM8SB2_Register_Enums.h>
#include <efm8_time.h>


#define ALARM_PERIOD_SEC 10
#define ON 1U
#define OFF 0U

#define CURRENT_TIME "2024:03:10:12:30:30"

uint8_t save;
static uint16_t extend = 0U;
uint32_t result = 0U;

time_t local;
const char* format = "%Y-%m-%d";
char* date_string;

uint8_t index = 0U;

void send_uart(uint8_t uart_data)
{
 SBUF0 = uart_data;
 while(!SCON0_TI);
 SCON0_TI = OFF;
}

uint8_t read_rtc(uint8_t address)
{
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);
	RTC0ADR = address;
	RTC0ADR |= RTC0ADR_BUSY__SET;
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);
	return RTC0DAT;
}

void write_rtc(uint8_t address, uint8_t value)
{
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);
	RTC0ADR = address;
	RTC0DAT = value | RTC0CN0_ALRM__SET;
}

//-----------------------------------------------------------------------------
// RTC0ALARM_ISR
//-----------------------------------------------------------------------------
SI_INTERRUPT (RTC0ALARM_ISR, RTC0ALARM_IRQn)
{

	save = read_rtc(RTC0CN0);
	write_rtc(RTC0CN0, save);

	extend++;

}

//-----------------------------------------------------------------------------
// INT0_ISR
//-----------------------------------------------------------------------------
//
// INT0 ISR Content goes here. Remember to clear flag bits:
// TCON::IE0 (External Interrupt 0)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (INT0_ISR, INT0_IRQn)
{

  date_string = seconds_to_string(&result);

	while (date_string[index] != '\0')
	{
	  send_uart(date_string[index]);
	  index++;
	}

	index = OFF;


}

