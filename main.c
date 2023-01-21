 //Including the header files
 #include <avr/io.h> //input/output
  #define F_CPU 1000000 //crystal frequency 1.0 MHz 
 #include <util/delay.h> //delay
 #include <stdlib.h> //standard library

 #define E 5 //giving name “enable”  to 5th pin of PORTD, since it is connected to LCD enable pin
 #define RS  6 //giving name “registerselection” to 6th pin of PORTD, since it is connected to LCD RS pin
 
 char u[2];//global variable
 /****FUNCTION Prototypes******/
 void send_a_command(unsigned char command);
 void send_a_character(unsigned char character);
 void send_a_string(char *string_of_characters);
 void display(int num,int i);
 
 //Defining the uart_init function for the Serial port
 void uart_init(){
	 UBRRL =0xC; //set baud rate 4800
	 UCSRB |=(1<<RXEN)|(1<<TXEN);
	 UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
 }


/**** Main function******/
 int main(void)
 {
	 //putting portB and portD as output pins
	 DDRB = 0xFF; 
	 DDRD = 0xFF;
	 
	 //giving delay of 50ms
	 _delay_ms(50);
	 
	 //Taking portA as input.
	 DDRA = 0;
	 
	 //setting the reference of ADC
	 ADMUX |=(1<<REFS0)|(1<<REFS1);
	 //enabling the ADC, setting free running mode, setting prescalar 128
	 ADCSRA |=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	 	 
	 int COUNTA = 0;//storing digital output
	 char SHOWA [3];//displaying digital output as temperature in 16*2 lcd
	 
	 send_a_command(0x01); //Clear Screen 0x01 = 00000001
	 
	 _delay_ms(50);//giving delay of 50ms
	 
	 send_a_command(0x38);//telling LCD we are using 8bit command /data mode
	 
	 _delay_ms(50);//giving delay of 50ms
	 
	 send_a_command(0b00001111);//LCD SCREEN ON and courser blinking
     _delay_ms(50);//giving delay of 50ms
	 
	 //Initializing Local Variables
	 int min = 95;//setting minimum value
	 int max = 0;//maximum value
	 //Declaring variables
	 int tens;
	 int units;
	 
	 //transmit data
	 uart_init();//calling function to set the UART 
	 int i=0;//test variable for min value
	 while(1)//infinite loop
	 {
         ADCSRA |=(1<<ADSC);//starting the ADC conversion
		 
		 COUNTA = (0.475 * ADC);
		
		 //Initializing the units & tens
		 units=COUNTA/10;
		 tens=COUNTA%10;
		 
		 //Calling Display Function
		 display(units,1);
		 display(tens,0);							
		 
		 //transmit data to SERIAL
		 //Wait until whole data from register UDR is not completely transmit
         while ( ! ( UCSRA & (1<<UDRE) ) ) ;
         UDR = u[1];//units
		 while ( ! ( UCSRA & (1<<UDRE) ) ) ;
		 UDR = u[0];//tens
		 while ( ! ( UCSRA & (1<<UDRE) ) ) ;
		 UDR = '\n';//newline
		 _delay_ms(100);//giving delay of 50ms
		 
		 //MAX & MIN function, finding the max & min value from the data 
		 if(COUNTA>=max)
		 {
			max=COUNTA;
		 }
		 
		 if(COUNTA<=min&&i)
		 {
			   min=COUNTA;
		  }
		  else{
			  i=1;
		  }
		  /***********Displaying Data on LCD (16x2)************/
		 send_a_string ("Max =");
		 //command for putting variable number in LCD(variable number, in which character to replace, which base is variable(ten here as we are counting number in base10))
		 itoa(max,SHOWA,10);//integer to string
		 
		 //telling the display to show character(replaced by variable number) of first person after positioning the courser on LCD
		 send_a_string(SHOWA);
		 send_a_string (" ,Min =");
		 itoa(min,SHOWA,10);
		 send_a_string(SHOWA);
		 
		 send_a_command(0x80 + 0x40 + 0); // shifting cursor  to 1st  shell  of second line
		 send_a_string ("Temp(*C) = ");// displaying name
		
		 //int data to string
		 itoa(COUNTA,SHOWA,10); 
		 send_a_string(SHOWA); 
		 send_a_command(0x80 + 0);//retuning to first line first shell
		 
	 }//while loop ends
	 
 }//main function ends

/************* Defining the Functions *************/
//Command function
 void send_a_command(unsigned char command)
 {
	 PORTB = command;
	 //putting 0 in RS to tell LCD we are sending command
	 PORTD &= ~ (1<<RS); 
	 
	 //telling LCD to receive command /data at the port
	 PORTD |= (1<<E); 
	 _delay_ms(50); //giving delay of 50ms
	 
	 //telling LCD we completed sending data
	 PORTD &= ~(1<<E);
	 PORTB= 0;
 }
 
 //Character Function
 void send_a_character(unsigned char character)
 {
	 PORTB= character;
	 //telling LCD we are sending data not commands
	 PORTD |= (1<<RS);
	 
	 //telling LCD to start receiving command/data
	 PORTD |= (1<<E);
	 _delay_ms(50); //giving delay of 50ms
	 
	 //telling LCD we completed sending data/command
	 PORTD &= ~(1<<E);
	 PORTB = 0;
	 
 }

//String
 void send_a_string(char *string_of_characters)
 {
	 while(*string_of_characters > 0)
	 {
		 send_a_character(*string_of_characters++);
	 }
 }
 
 //Display on Serial Port
  void display(int num,int i){
	  if(num==0){u[i]='0';}
	  else if(num==1){u[i]='1';}
	  else if(num==2){u[i]='2';}
	  else if(num==3){u[i]='3';}
	  else if(num==4){u[i]='4';}
	  else if(num==5){u[i]='5';}
	  else if(num==6){u[i]='6';}
	  else if(num==7){u[i]='7';}
	  else if(num==8){u[i]='8';}
	  else if(num==9){u[i]='9';}
  }



