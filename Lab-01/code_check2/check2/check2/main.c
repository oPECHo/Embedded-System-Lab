#include <avr/io.h>
#define F_CPU 1.2E6
#include <util/delay.h>

uint8_t bin_to_hex(uint8_t bin) {
	switch (bin) {
		case 0b0000: return 0x0;
		case 0b0001: return 0x1;
		case 0b0010: return 0x2;
		case 0b0011: return 0x3;
		case 0b0100: return 0x4;
		case 0b0101: return 0x5;
		case 0b0110: return 0x6;
		case 0b0111: return 0x7;
		case 0b1000: return 0x8;
		case 0b1001: return 0x9;
		case 0b1010: return 0xA;
		case 0b1011: return 0xB;
		case 0b1100: return 0xC;
		case 0b1101: return 0xD;
		case 0b1110: return 0xE;
		case 0b1111: return 0xF;
		default: return 0x0;
	}
}

void display_hex(uint8_t hex) {
	switch (hex) {
		case 0x1: // Display '1'
		PORTC = 0b00000110;
		PORTB &= 0xFE;
		break;
		case 0x2: // Display '2'
		PORTC = 0b01011011;
		PORTB |= 0x01;
		break;
		case 0x3: // Display '3'
		PORTC = 0b01001111;
		PORTB |= 0x01;
		break;
		case 0x4: // Display '4'
		PORTC = 0b01100110;
		PORTB |= 0x01;
		break;
		case 0x5: // Display '5'
		PORTC = 0b01101101;
		PORTB |= 0x01;
		break;
		case 0x6: // Display '6'
		PORTC = 0b01111101;
		PORTB |= 0x01;
		break;
		case 0x7: // Display '7'
		PORTC = 0b00000111;
		PORTB &= 0xFE;
		break;
		case 0x8: // Display '8'
		PORTC = 0b01111111;
		PORTB |= 0x01;
		break;
		case 0x9: // Display '9'
		PORTC = 0b01101111;
		PORTB |= 0x01;
		break;
		case 0xA: // Display 'A'
		PORTC = 0b01110111;
		PORTB |= 0x01;
		break;
		case 0xB: // Display 'B'
		PORTC = 0b01111100;
		PORTB |= 0x01;
		break;
		case 0xC: // Display 'C'
		PORTC = 0b00111001;
		PORTB &= 0xFE;
		break;
		case 0xD: // Display 'D'
		PORTC = 0b01011110;
		PORTB |= 0x01;
		break;
		case 0xE: // Display 'E'
		PORTC = 0b01111001;
		PORTB |= 0x01;
		break;
		case 0xF: // Display 'F'
		PORTC = 0b01110001;
		PORTB |= 0x01;
		break;
		default:
		PORTC = 0b00111111; // Blank if not valid hex digit
		PORTB &= 0xFE;
		break;
	}
}

int main(void) {
	uint8_t sw1, sw2;
	
	DDRD = 0x00; 
	DDRB = 0x0F; 
	DDRC = 0x3F; 

	while (1) {
		uint8_t d = PIND;
		sw1 = d & 0x0F; 
		sw2 = (d >> 4) & 0x0F; 

		uint8_t hex_sw1 = bin_to_hex(sw1);
		uint8_t hex_sw2 = bin_to_hex(sw2);

		if (hex_sw1 == hex_sw2) {
			// Case 1: sw1 == sw2
			PORTB = 0x04; // Turn on PB2 LED
			display_hex(hex_sw1);
		} else if (hex_sw1 < hex_sw2) {
			// Case 2: sw1 < sw2
			PORTB = 0x02; // Turn on PB1 LED
			display_hex(hex_sw2 - hex_sw1);
		} else {
			// Case 3: sw1 > sw2
			PORTB = 0x08; // Turn on PB3 LED
			display_hex(hex_sw1 - hex_sw2);
		}

		_delay_ms(100); // Delay for stability
	}

	return 0;
}
