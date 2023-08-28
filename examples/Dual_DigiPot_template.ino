/*************************************************************
    Dual Digital Pot (100K)
     <https://www.dfrobot.com/wiki/index.php/Dual_Digital_Pot_(100K)_SKU:_DFR0520>
 *************************************************************
   This example serves as a template to control the MCP42100 dual
   digital pot through 3-wire SPI.

   Created 2017-8-31
   By Henry Zhao <Henry.zhao@dfrobot.com>
   Updated 2023-08-27
   By Justin Pavan

   GNU Lesser General Public License.
   See <http://ww.gnu.org/licenses/> for details.
   All above must be included in any redistribution.
 ************************************************************/

/******************** Device Introduction **********************
   The MCP42100 has dual potentiometer x (x=0,1).
     Ax - Potentiometer terminal Ax
     Wx - Potentiometer Wiper
     Bx - Potentiometer terminal Bx

     SI - Serial Data Input
     SCK - Serial Clock
     CS - Chip Select

     The MCP42100 is SPI-compatible,and two bytes should be sent to control it.
     The first byte specifies the potentiometer (POT0: 0x11, POT1: 0x12, both: 0x13).
     The second byte specifies resistance value for the pot (0 - 255).
 ************************************************************/

/*********************** Circuit Connections *******************
     Digital Pot | Arduino UNO R3
         CS      |    D10 (SS)
         SI      |    D11 (MOSI)
         CLK     |    D13 (SCK)
         VCC     |      VCC
         GND     |      GND
 ************************************************************/

/***********************Resistances Calculation**************
   Rwa(Dn) =Rab*(256 - Dn) / 256 + Rw
   Rwb(Dn) =Rab*Dn / 256 + Rw

   Rwa - resistance between Terminal A and wiper W
   Rwb - resistance between Terminal B and wiper W
   Rab - overall resistance for the pot （=100KΩ, typical）
   Rw - wiper resistance （=125Ω,typical; =175Ω max）
   Dn - 8-bit value in data register for pot number n （= 0 - 255）
 ************************************************************/

/*********************** Notice ********************************
   1.Resistor terminals A, B and W have no restrictions on 
     polarity with respect to each other.
   2.Current through terminals A, B and W should not exceed ±1mA.
   3.Voltages on terminals A, B and W should be within 0 - VCC.
************************************************************/

#include <SPI.h>

/*********************** PIN Definitions *************************/
// Set pin 10 as the slave select for the digital pot:
const int CS_PIN = 10;

/*********************** MCP42XXX Commands ************************/
// Potentiometer select byte
const int POT0_SEL = 0x11;
const int POT1_SEL = 0x12;
const int BOTH_POT_SEL = 0x13;

// Shutdown the device to put it into power-saving mode.
// In this mode, terminal A is open-circuited and the B and W terminals are shorted together.
// Send a new command and value to exit shutdown mode.
const int POT0_SHUTDOWN = 0x21;
const int POT1_SHUTDOWN = 0x22;
const int BOTH_POT_SHUTDOWN = 0x23;

/*********************** Customized Variables **********************/
// Resistance value byte (0 - 255)
// The wiper is reset to the mid-scale position upon power-up, i.e. POT0_Dn = POT1_Dn = 128
int POT0_Dn = 128;
int POT1_Dn = 128;
int BOTH_POT_Dn = 128;

// Function Declaration
void DigitalPotWrite(int cmd, int value);     // Send the command and the resistance value through SPI

void setup()
{
  pinMode(CS_PIN, OUTPUT);    // Set the CS_PIN as an output:
  SPI.begin();     // Initialize the SPI interface
  DigitalPotWrite(BOTH_POT_SHUTDOWN, BOTH_POT_Dn);
}

void loop()
{

  DigitalPotWrite(POT0_SEL, POT0_Dn);                 // Set the resistance of POT0
  DigitalPotWrite(POT1_SEL, POT1_Dn);                 // Set the resistance of POT1
  //DigitalPotWrite(BOTH_POT_SEL, BOTH_POT_Dn);         // Set the resistance of both potentiometers

  //DigitalPotWrite(POT0_SHUTDOWN, POT0_Dn);              // Put POT0 into shutdown mode, ignore the second parameter
  //DigitalPotWrite(POT1_SHUTDOWN, POT1_Dn);              // Put POT1 into shutdown mode, ignore the second parameter
  //DigitalPotWrite(BOTH_POT_SHUTDOWN, BOTH_POT_Dn);      // Put both potentiometers into shutdown mode, ignore the second parameter

}

void DigitalPotWrite(int cmd, int val)
{
  // Constrain input value within 0 - 255
  val = constrain(val, 0, 255);
  // Set the CS pin to low to select the chip:
  digitalWrite(CS_PIN, LOW);
  // Send the command and value via SPI:
  SPI.transfer(cmd);
  SPI.transfer(val);
  // Set the CS pin high to execute the command:
  digitalWrite(CS_PIN, HIGH);
}
