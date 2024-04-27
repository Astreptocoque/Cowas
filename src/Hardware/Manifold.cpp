#include <Arduino.h>
#include "Manifold.h"
#include "Button.h"
#include "C_output.h"
#include <SPI.h>

extern C_output output;
extern Motor manifold_motor;
extern Button button_start;
//----- MOTOR -----//
const uint16_t speed = 40; // Refers to power, min 70 to move


motor_direction direction = down; // 1 or -1
int nb_turns = 0;
float current_angle;
float deg_encoderPosition = 0;// encoder position in degrees
float deg_previousEncoderPosition = 0;

// first element is the purge angle. The following elements are the sterivex angles in the order we want them to be used.
float sterivex_angle[15];


/**
 * @brief Constructor for the manifold slots
 * @param pin_control Output connection on the board of the slot
 * @param _ID id of the manifold slot
 */
void Manifold_slot::begin(byte _pin_control, int _id)
{
    ID = _id;
    state = available;
    pin_control = _pin_control;
    // pinMode(pin_control, OUTPUT); // Assigned a pin to each slot
    if (VERBOSE_INIT){output.println("Slot manifold " + String(ID) + " initiated");}
}

int Manifold_slot::get_id()
{
    return ID;
}

void Manifold_slot::change_state(slot_state changed_state)
{
    state=changed_state;
}

slot_state Manifold_slot::get_state()
{
    return state;
}


/**
 * @brief Constructor for the manifold
 */
void Manifold::begin()
{
    for(int i=0; i < NB_SLOT; i++){
        slots[i].begin(MANIFOLD_PIN[i], i);
    }   
    pinMode(ENCODER_MANIFOLD, OUTPUT);
    digitalWrite(ENCODER_MANIFOLD, HIGH);

    // Creation of array with sterivex position in angle
    int omitted_angle_nb = 12; //angle[12] is omitted as there is no sterivex at this angles
    for (int i = 0; i < 16; i++)
    {
        if (i < omitted_angle_nb)
        {
        sterivex_angle[i] = purge_angle + i * 22.5;
        if (sterivex_angle[i] > 360)
            sterivex_angle[i] = sterivex_angle[i] - 360;
        }
        else if (i > omitted_angle_nb)
        {
        sterivex_angle[i - 1] = purge_angle + i * 22.5;
        if (sterivex_angle[i - 1] > 360)
            sterivex_angle[i - 1] = sterivex_angle[i - 1] - 360;
        }
    }
    if (VERBOSE_INIT){output.println("Manifold initiated");}
}

int Manifold::get_id(int i)
{
    return slots[i].get_id();
}

void Manifold::change_state(int i, slot_state changed_state)
{
    slots[i].change_state(changed_state);
}

slot_state Manifold::get_state(int i)
{
    return slots[i].get_state();
}

void Manifold::reload()
{
    for(int i=0; i < NB_SLOT; i++){
          slots[i].change_state(available);
      }   
}

void rotateMotor(int index)
{
  // if the manifold is not connected to the system, exit function
  if(MANIFOLD_USE == false){
    return;
  }

  // SETUP//
  bool end_rotation = false;
  nb_turns = 0; //counts number of turns in case the position is <0deg or >360deg.
  float angle_to_reach = sterivex_angle[index];

  readEncoder(true);
  readEncoder(false);
  directionDetermination(angle_to_reach);

  manifold_motor.start(speed, direction);


  //ROTATE//
  while (end_rotation == false)
  {
    readEncoder(false);

    if(VERBOSE_MANIFOLD){output.print("   Angle to reach: ");
    output.print(angle_to_reach);
    output.println(2);}

    if ((direction == down && current_angle >= (angle_to_reach-angle_offset_pos)) ||
        (direction == up && current_angle <= (angle_to_reach+angle_offset_neg)))
    {
      manifold_motor.stop();
      end_rotation = true;
    }

    delay(1);//smaller delay -> better precision
  }
}

void directionDetermination(float goal_angle)
{
  if (((goal_angle - 180.0) <= current_angle) && (current_angle <= goal_angle)) {
    direction = down;
  }
  else if ((goal_angle < current_angle) && (current_angle < (goal_angle + 180.0))) {
    direction = up;
  }
  //special case: the current_angle is not contained in both of the above ranges (-180deg < goal_angle < +180deg)
  //in this case we first try to add 1 turn (+360deg) or remove one (-360deg)
  else
  {
    current_angle = current_angle + 360;
    if (((goal_angle - 180.0) <= current_angle) && (current_angle <= goal_angle))
    {
      direction = down;
      nb_turns = nb_turns + 1;
    }
    else if ((goal_angle < current_angle) && (current_angle < (goal_angle + 180.0)))
    {
      direction = up;
      nb_turns = nb_turns + 1;
    }
    else
    {
      current_angle = current_angle - 2 * 360;
      if (((goal_angle - 180.0) <= current_angle) && (current_angle <= goal_angle))
      {
        direction = down;
        nb_turns = nb_turns - 1;
      }
      else if ((goal_angle < current_angle) && (current_angle < (goal_angle + 180.0)))
      {
        direction = up;
        nb_turns = nb_turns - 1;
      }
    }
  }
}


void readEncoder(bool init_setup)
{
  uint16_t encoderPosition; //a 16 bit variable to hold the encoders position (goes up to 4096)
  uint8_t attempts; //count how many times we've tried to obtain the position in case there are errors

  deg_previousEncoderPosition = deg_encoderPosition;

  //if you want to set the zero position before beggining uncomment the following function call
  //setZeroSPI(ENCODER_MANIFOLD);

  //set attemps counter at 0 so we can try again if we get bad position
  attempts = 0;

  //this function gets the encoder position and returns it as a uint16_t
  //send the function either res12 or res14 for your encoders resolution
  encoderPosition = getPositionSPI(ENCODER_MANIFOLD, RES12);

  //if the position returned was 0xFFFF we know that there was an error calculating the checksum
  //make 3 attempts for position. we will pre-increment attempts because we'll use the number later and want an accurate count
  while (encoderPosition == 0xFFFF && ++attempts < 3)
  {
    encoderPosition = getPositionSPI(ENCODER_MANIFOLD, RES12); //try again
  }

  if (encoderPosition == 0xFFFF) //position is bad, let the user know how many times we tried
  {
    if(VERBOSE_MANIFOLD){output.print("Encoder 0 error. Attempts: ");
    output.println(attempts);
    output.println(DEC);} //print out the number in decimal format. attempts - 1 is used since we post incremented the loop
  }
  else //position was good, print to serial stream
  {
    deg_encoderPosition = encoderPosition * encoder_to_deg;

    if (!init_setup)
    {
      //increments or decrements nb_turns, if the 0/360deg point have been exceeded
      if ((deg_encoderPosition > 350) && (deg_previousEncoderPosition < 10))
        nb_turns = nb_turns - 1;
      else if ((deg_encoderPosition < 10) && (deg_previousEncoderPosition > 350))
        nb_turns = nb_turns + 1;
      current_angle = deg_encoderPosition + 360 * nb_turns;
    }

    if(VERBOSE_MANIFOLD){output.print("Encoder: ");
    output.print(current_angle);
    output.print(2);}
  }
}

/*
   This function gets the absolute position from the AMT22 encoder using the SPI bus. The AMT22 position includes 2 checkbits to use
   for position verification. Both 12-bit and 14-bit encoders transfer position via two bytes, giving 16-bits regardless of resolution.
   For 12-bit encoders the position is left-shifted two bits, leaving the right two bits as zeros. This gives the impression that the encoder
   is actually sending 14-bits, when it is actually sending 12-bit values, where every number is multiplied by 4.
   This function takes the pin number of the desired device as an input
   This funciton expects res12 or res14 to properly format position responses.
   Error values are returned as 0xFFFF
*/
uint16_t getPositionSPI(uint8_t encoder, uint8_t resolution)
{
  uint16_t currentPosition;       //16-bit response from encoder
  bool binaryArray[16];           //after receiving the position we will populate this array and use it for calculating the checksum

  //get first byte which is the high byte, shift it 8 bits. don't release line for the first byte
  currentPosition = spiWriteRead(AMT22_NOP, encoder, false) << 8;

  //this is the time required between bytes as specified in the datasheet.
  //We will implement that time delay here, however the arduino is not the fastest device so the delay
  //is likely inherantly there already
  delayMicroseconds(3);

  //OR the low byte with the currentPosition variable. release line after second byte
  currentPosition |= spiWriteRead(AMT22_NOP, encoder, true);

  //run through the 16 bits of position and put each bit into a slot in the array so we can do the checksum calculation
  for (int i = 0; i < 16; i++) binaryArray[i] = (0x01) & (currentPosition >> (i));

  //using the equation on the datasheet we can calculate the checksums and then make sure they match what the encoder sent
  if ((binaryArray[15] == !(binaryArray[13] ^ binaryArray[11] ^ binaryArray[9] ^ binaryArray[7] ^ binaryArray[5] ^ binaryArray[3] ^ binaryArray[1]))
      && (binaryArray[14] == !(binaryArray[12] ^ binaryArray[10] ^ binaryArray[8] ^ binaryArray[6] ^ binaryArray[4] ^ binaryArray[2] ^ binaryArray[0])))
  {
    //we got back a good position, so just mask away the checkbits
    currentPosition &= 0x3FFF;
  }
  else
  {
    currentPosition = 0xFFFF; //bad position
  }


  //If the resolution is 12-bits, and wasn't 0xFFFF, then shift position, otherwise do nothing
  if ((resolution == RES12) && (currentPosition != 0xFFFF)) currentPosition = currentPosition >> 2;

  return currentPosition;
}

/*
   This function sets the state of the SPI line. It isn't necessary but makes the code more readable than having digitalWrite everywhere
   This function takes the pin number of the desired device as an input
*/
void setCSLine (uint8_t encoder, uint8_t csLine)
{
  digitalWrite(encoder, csLine);
}


/*
   This function does the SPI transfer. sendByte is the byte to transmit.
   Use releaseLine to let the spiWriteRead function know if it should release
   the chip select line after transfer.
   This function takes the pin number of the desired device as an input
   The received data is returned.
*/
uint8_t spiWriteRead(uint8_t sendByte, uint8_t encoder, uint8_t releaseLine)
{
  //holder for the received over SPI
  uint8_t data;

  //set cs low, cs may already be low but there's no issue calling it again except for extra time
  setCSLine(encoder , LOW);

  //There is a minimum time requirement after CS goes low before data can be clocked out of the encoder.
  //We will implement that time delay here, however the arduino is not the fastest device so the delay
  //is likely inherantly there already
  delayMicroseconds(3);

  //send the command
  data = SPI.transfer(sendByte);
  delayMicroseconds(3); //There is also a minimum time after clocking that CS should remain asserted before we release it
  setCSLine(encoder, releaseLine); //if releaseLine is high set it high else it stays low

  return data;
}

void calibrateEncoder(uint16_t speed){
    output.println("Calibrating encoder of the manifold");
    output.println("Be ready to press button when motor is aligned with slot 0");
    output.println("Press button or type text to start motor");

    while (!Serial.available())
    {
      delay(10);
    }

    output.println("Starting to rotate");
    Serial.flush();
    delay(3000);

    manifold_motor.start(speed, down);
    
    while (Serial.available())
    {
      Serial.read();
    }
    
    while (!Serial.available()){
        delay(1);
    }

    manifold_motor.stop();

    // read encoder value and convert to degrees
    uint16_t pos = getPositionSPI(ENCODER_MANIFOLD, RES12) * encoder_to_deg;
    Serial.print("Manifold encoder value : ");
    Serial.println(pos);
    
    output.println("Please replace the purge angle of manifold by : ");
    output.println(pos);
}
