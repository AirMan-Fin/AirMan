/*
 * lcd_port.cpp
 *
 *  Created on: 26.1.2016
 *      Author: krl
 */


#include "arduino.h"
bool flRit;

extern "C" {
void RIT_IRQHandler(void) {
// Acknowledge interrupt by clearing RIT interrupt flag
	Chip_RIT_ClearIntStatus(LPC_RITIMER);
// disable RIT – we want a single interrupt after the wait is over
	Chip_RIT_Disable(LPC_RITIMER);
// set a flag to notify main program
	flRit = 1;
}
}

static const int dPort[] = { 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 };
static const int dPin[] = { 10, 9, 29, 9, 10, 16, 3, 0, 24, 0, 27, 28, 12, 14 };

void digitalWrite(int pin, int val) {
	if (pin > 13)
		return;
	Chip_GPIO_SetPinState(LPC_GPIO, dPort[pin], dPin[pin], val);
}

bool digitalRead(int pin) {
	if (pin > 13)
		return 0;
	return !Chip_GPIO_GetPinState(LPC_GPIO, dPort[pin], dPin[pin]);
}

void pinMode(int pin, int mode) {
	if (mode == 1) {
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, dPort[pin], dPin[pin]);
	} else {
		Chip_IOCON_PinMuxSet(LPC_IOCON, dPort[pin], dPin[pin],
				(IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, dPort[pin], dPin[pin]);
	}
}

void enRit() {
	//NVIC_EnableIRQ(RITIMER_IRQn);
	Chip_RIT_Init(LPC_RITIMER);
}

void delay(int us) {
	// calculate compare value ???
	uint64_t uss = Chip_Clock_GetSysTickClockRate() / 1000 * us;
	// disable RIT – compare value may only be changed when RIT is disabled
	Chip_RIT_Disable(LPC_RITIMER);
	// clear wait flag (the variable that ISR will set)
	flRit = 0;
	// set compare value to RIT
	Chip_RIT_SetCompareValue(LPC_RITIMER, uss);
	// clear RIT counter (so that counting starts from zero)
	Chip_RIT_SetCounter(LPC_RITIMER, 0);
	// enable RIT
	Chip_RIT_Enable(LPC_RITIMER);
	// wait until flag is set
	while (!Chip_RIT_GetIntStatus(LPC_RITIMER))
		;
	// Acknowledge interrupt by clearing RIT interrupt flag
		Chip_RIT_ClearIntStatus(LPC_RITIMER);
	// disable RIT – we want a single interrupt after the wait is over
		Chip_RIT_Disable(LPC_RITIMER);
}

void delayMicroseconds(int us) {
	// calculate compare value ???
	uint64_t uss = Chip_Clock_GetSysTickClockRate() / 1000000 * us;
	// disable RIT – compare value may only be changed when RIT is disabled
	Chip_RIT_Disable(LPC_RITIMER);
	// clear wait flag (the variable that ISR will set)
	flRit = 0;
	// set compare value to RIT
	Chip_RIT_SetCompareValue(LPC_RITIMER, uss);
	// clear RIT counter (so that counting starts from zero)
	Chip_RIT_SetCounter(LPC_RITIMER, 0);
	// enable RIT
	Chip_RIT_Enable(LPC_RITIMER);
	// wait until flag is set
	while (!Chip_RIT_GetIntStatus(LPC_RITIMER))
		;
	// Acknowledge interrupt by clearing RIT interrupt flag
		Chip_RIT_ClearIntStatus(LPC_RITIMER);
	// disable RIT – we want a single interrupt after the wait is over
		Chip_RIT_Disable(LPC_RITIMER);
}

Button::Button(int pp) {
	pin = pp;
	pinMode(pp, 0);
}

bool Button::read() {
	bool now = digitalRead((uint8_t) pin);
	//printf("d:%d\n",now);
	if (now != prevState) {
		prevState = now;
		return now;
	}
	return 0;
}

extern "C" {
void ADC0A_IRQHandler(void) {
	uint32_t pending;

	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC0);

	/* Sequence A completion interrupt */
	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
		adcdone = true;
	}

	/* Clear any pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC0, pending);
}

} // extern "C"

CHIP_SWM_PIN_FIXED analogName[]= {SWM_FIXED_ADC0_0,SWM_FIXED_ADC0_1,SWM_FIXED_ADC0_2,SWM_FIXED_ADC0_3,SWM_FIXED_ADC0_4,SWM_FIXED_ADC0_5,SWM_FIXED_ADC0_6,SWM_FIXED_ADC0_7};

AnalogPort::AnalogPort(int pin2) {
	pin = pin2;

	/* Setup ADC for 12-bit mode and normal power */
	Chip_ADC_Init(LPC_ADC0, 0);

	/* Setup for maximum ADC clock rate */
	Chip_ADC_SetClockRate(LPC_ADC0, ADC_MAX_SAMPLE_RATE);

	/* For ADC0, sequencer A will be used without threshold events.
	 It will be triggered manually  */
	Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX,
			(ADC_SEQ_CTRL_CHANSEL(pin2)
					| ADC_SEQ_CTRL_MODE_EOS));

	/* For ADC0, select analog input pint for channel 0 on ADC0 */
	Chip_ADC_SetADC0Input(LPC_ADC0, 0);

	/* Use higher voltage trim for both ADC */
	Chip_ADC_SetTrim(LPC_ADC0, ADC_TRIM_VRANGE_HIGHV);

	/* Assign ADC0_0 to PIO1_8 via SWM (fixed pin) and ADC0_3 to PIO0_5 */
	Chip_SWM_EnableFixedPin (analogName[pin2]);

	/* Need to do a calibration after initialization and trim */
	Chip_ADC_StartCalibration(LPC_ADC0);
	while (!(Chip_ADC_IsCalibrationDone(LPC_ADC0))) {
	}

	/* Clear all pending interrupts and status flags */
	Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));

	/* Enable sequence A completion interrupts for ADC0 */
	Chip_ADC_EnableInt(LPC_ADC0, ADC_INTEN_SEQA_ENABLE);

	/* Enable related ADC NVIC interrupts */
	NVIC_EnableIRQ (ADC0_SEQA_IRQn);

	/* Enable sequencer */
	Chip_ADC_EnableSequencer(LPC_ADC0, ADC_SEQA_IDX);

	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2);

}

int AnalogPort::read() {
	Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
	while (!adcdone)
		__WFI();
	adcdone = false;

	uint32_t a0 = Chip_ADC_GetDataReg(LPC_ADC0, pin);
	int d0 = ADC_DR_RESULT(a0);
	return d0;
}



