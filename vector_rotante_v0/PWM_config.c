/**********************************************************************
 * File: EPwm_7_8_9_10_12.c -- Solution File for Lab 7, 8, 9, 10 and 12
 * Devices: TMS320F2833x
 * Author: Technical Training Organization (TTO), Texas Instruments
 * History:
 *   07/15/08 - original
 **********************************************************************/

#include "Lab.h"

// TBCTL (Time-Base Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// TBCTR MODE bits
#define TB_COUNT_UP 0x0
#define TB_COUNT_DOWN 0x1
#define TB_COUNT_UPDOWN 0x2
#define TB_FREEZE 0x3
// PHSEN bit
#define TB_DISABLE 0x0
#define TB_ENABLE 0x1
// PRDLD bit
#define TB_SHADOW 0x0
#define TB_IMMEDIATE 0x1
// SYNCOSEL bits
#define TB_SYNC_IN 0x0
#define TB_CTR_ZERO 0x1
#define TB_CTR_CMPB 0x2
#define TB_SYNC_DISABLE 0x3
// HSPCLKDIV and CLKDIV bits
#define TB_DIV1 0x0
#define TB_DIV2 0x1
#define TB_DIV4 0x2
// PHSDIR bit
#define TB_DOWN 0x0
#define TB_UP 0x1

// CMPCTL (Compare Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// LOADAMODE and LOADBMODE bits
#define CC_CTR_ZERO 0x0
#define CC_CTR_PRD 0x1
#define CC_CTR_ZERO_PRD 0x2
#define CC_LD_DISABLE 0x3
// SHDWAMODE and SHDWBMODE bits
#define CC_SHADOW 0x0
#define CC_IMMEDIATE 0x1

// AQCTLA and AQCTLB (Action-qualifier Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// ZRO, PRD, CAU, CAD, CBU, CBD bits
#define AQ_NO_ACTION 0x0
#define AQ_CLEAR 0x1
#define AQ_SET 0x2
#define AQ_TOGGLE 0x3

// DBCTL (Dead-Band Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// MODE bits
#define DB_DISABLE 0x0
#define DBA_ENABLE 0x1
#define DBB_ENABLE 0x2
#define DB_FULL_ENABLE 0x3
// POLSEL bits
#define DB_ACTV_HI 0x0
#define DB_ACTV_LOC 0x1
#define DB_ACTV_HIC 0x2
#define DB_ACTV_LO 0x3

// PCCTL (chopper control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// CHPEN bit
#define CHP_ENABLE 0x0
#define CHP_DISABLE 0x1
// CHPFREQ bits
#define CHP_DIV1 0x0
#define CHP_DIV2 0x1
#define CHP_DIV3 0x2

// TBCTL (Time-Base Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// TBCTR MODE bits
#define TB_COUNT_UP 0x0
#define TB_COUNT_DOWN 0x1
#define TB_COUNT_UPDOWN 0x2
#define TB_FREEZE 0x3
// PHSEN bit
#define TB_DISABLE 0x0
#define TB_ENABLE 0x1
// PRDLD bit
#define TB_SHADOW 0x0
#define TB_IMMEDIATE 0x1
// SYNCOSEL bits
#define TB_SYNC_IN 0x0
#define TB_CTR_ZERO 0x1
#define TB_CTR_CMPB 0x2
#define TB_SYNC_DISABLE 0x3
// HSPCLKDIV and CLKDIV bits
#define TB_DIV1 0x0
#define TB_DIV2 0x1
#define TB_DIV4 0x2
// PHSDIR bit
#define TB_DOWN 0x0
#define TB_UP 0x1

// CMPCTL (Compare Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// LOADAMODE and LOADBMODE bits
#define CC_CTR_ZERO 0x0
#define CC_CTR_PRD 0x1
#define CC_CTR_ZERO_PRD 0x2
#define CC_LD_DISABLE 0x3
// SHDWAMODE and SHDWBMODE bits
#define CC_SHADOW 0x0
#define CC_IMMEDIATE 0x1

// AQCTLA and AQCTLB (Action-qualifier Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// ZRO, PRD, CAU, CAD, CBU, CBD bits
#define AQ_NO_ACTION 0x0
#define AQ_CLEAR 0x1
#define AQ_SET 0x2
#define AQ_TOGGLE 0x3

// DBCTL (Dead-Band Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// MODE bits
#define DB_DISABLE 0x0
#define DBA_ENABLE 0x1
#define DBB_ENABLE 0x2
#define DB_FULL_ENABLE 0x3
// POLSEL bits
#define DB_ACTV_HI 0x0
#define DB_ACTV_LOC 0x1
#define DB_ACTV_HIC 0x2
#define DB_ACTV_LO 0x3

// PCCTL (chopper control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// CHPEN bit
#define CHP_ENABLE 0x0
#define CHP_DISABLE 0x1
// CHPFREQ bits
#define CHP_DIV1 0x0
#define CHP_DIV2 0x1
#define CHP_DIV3 0x2


void pwm2();

void pwm1_init();
void pwm4_init();
void modVect_init();
void pwm1_config_interrupt();

/**********************************************************************
 * Function: InitEPwm()
 *
 * Description: Initializes the Enhanced PWM modules on the F2833x
 **********************************************************************/
void InitEPwm(void)
{
	//---------------------------------------------------------------------
	//--- Must disable the clock to the ePWM modules if you
	//--- want all ePMW modules synchronized.
	//---------------------------------------------------------------------
	asm(" EALLOW"); // Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	asm(" EDIS"); // Disable EALLOW protected register access

	pwm2();

	pwm1_init();

	//pwm4_init();

	pwm1_config_interrupt();

	modVect_init();
}

void modVect_init()
{
	//=====================================================================
	// Configuration
	//=====================================================================
	// Initialization Time
	//========================// EPWM Module 1 config
	
	// EPWM Module 4 config
	EPwm4Regs.TBPRD = PROJECT_PERIOD;				// Period = 1600 TBCLK counts
	EPwm4Regs.TBPHS.half.TBPHS = 0;				   	// Set Phase register to zero
	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// Symmetrical mode
	EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;		   	// Master module
	EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; 	// Sync down-stream module
	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; 	// load on CTR=Zero
	EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; 	// load on CTR=Zero
	EPwm4Regs.AQCTLA.bit.CAU = AQ_SET;			  	// set actions for EPwm4A
	EPwm4Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; 	// enable Dead-band module
	EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   	// Active Hi complementary
	EPwm4Regs.DBFED = 50;						   	// FED = 50 TBCLKs
	EPwm4Regs.DBRED = 50;						   	// RED = 50 TBCLKs

	// EPWM Module 5 config
	EPwm5Regs.TBPRD = PROJECT_PERIOD;				// Period = 1600 TBCLK counts
	EPwm5Regs.TBPHS.half.TBPHS = 0;				  	// Set Phase register to zero
	EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// Symmetrical mode
	EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;		   	// Slave module
	EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; 		// sync flow-through
	EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; 	// load on CTR=Zero
	EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; 	// load on CTR=Zero
	EPwm5Regs.AQCTLA.bit.CAU = AQ_SET;			  	// set actions for EPWM5A
	EPwm5Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; 	// enable Dead-band module
	EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   	// Active Hi complementary
	EPwm5Regs.DBFED = 50;						   	// FED = 50 TBCLKs
	EPwm5Regs.DBRED = 50;						   	// RED = 50 TBCLKs

	// EPWM Module 6 config
	EPwm6Regs.TBPRD = PROJECT_PERIOD;				// Period = 1600 TBCLK counts
	EPwm6Regs.TBPHS.half.TBPHS = 0;				  	// Set Phase register to zero
	EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// Symmetrical mode
	EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;		   	// Slave module
	EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; 		// sync flow-through
	EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; 	// load on CTR=Zero
	EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; 	// load on CTR=Zero
	EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;			  	// set actions for EPWM6A
	EPwm6Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; 	// enable Dead-band module
	EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   	// Active Hi complementary
	EPwm6Regs.DBFED = 50;						   	// FED = 50 TBCLKs
	EPwm6Regs.DBRED = 50;						   	// RED = 50 TBCLKs

	// Run Time (Note: Example execution of one run-time instant)
	//=========================================================
	EPwm4Regs.CMPA.half.CMPA = 500; // adjust duty for output EPwm4A
	EPwm5Regs.CMPA.half.CMPA = 600; // adjust duty for output EPWM5A
	EPwm6Regs.CMPA.half.CMPA = 700; // adjust duty for output EPWM6A
}

void pwm4_init()
{
	EPwm4Regs.TBCTL.bit.CTRMODE = 0x3; // Disable the timer -> NO DEBERIA SER NECESARIA POR LA LINEA 37

	EPwm4Regs.TBCTL.all = 0xC033; // Configure timer control register
	// bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
	// bit 13        0:      PHSDIR, 0 = count down after sync event
	// bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
	// bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
	// bit 6         0:      SWFSYNC, 0 = no software sync produced
	// bit 5-4       11:     SYNCOSEL, 11 = sync-out disabled
	// bit 3         0:      PRDLD, 0 = reload PRD on counter=0 PRD=PERIOD
	// bit 2         0:      PHSEN, 0 = phase control disabled
	// bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

	EPwm4Regs.TBCTR = 0x0000;			 // Clear timer counter
	EPwm4Regs.TBPRD = PROJECT_PERIOD;	 // Levanto la frecuencia a 10 kHz PWM_HALF_PERIOD
	EPwm4Regs.TBPHS.half.TBPHS = 0x0000; // Set timer phase

	// EPwm4Regs.CMPA.half.CMPA = PWM_DUTY_CYCLE;	// Set PWM duty cycle
	EPwm4Regs.CMPA.half.CMPA = PROJECT_PERIOD * 0.5;
	// EPwm4Regs.CMPB = PROJECT_PERIOD * 0.75;

	// EPwm4Regs.ETSEL.all = 0b01100;			// Activo la interrupción EPWM
	//  bit 15        0:      SOCBEN, 0 = disable SOCB
	//  bit 14-12     000:    SOCBSEL, don't care
	//  bit 11        0:      SOCAEN, 0 = enable SOCA
	//  bit 10-8      000:    SOCASEL, 000 = SOCA on PRD event
	//  bit 7-4       0000:   reserved
	//  bit 3         1:      INTEN, 1 = enable interrupt
	//  bit 2-0       000:    INTSEL, don't care
	// EPwm4Regs.ETSEL.bit.INTEN = 1;	// habilita interrupcion
	// EPwm4Regs.ETSEL.bit.INTSEL = 2; // trigger a en el TBPRD
	// EPwm4Regs.ETPS.bit.INTPRD = 1;	// EPWMxINTn Period Select

	// **** COMPARE CONTROL REGISTER
	// EPwm4Regs.CMPCTL.all = 0x0002; // Compare control register
	// bit 15-10     0's:    reserved
	// bit 9         0:      SHDWBFULL, read-only
	// bit 8         0:      SHDWAFULL, read-only
	// bit 7         0:      reserved
	// bit 6         0:      SHDWBMODE, don't care
	// bit 5         0:      reserved
	// bit 4         0:      SHDWAMODE, 0 = shadow mode
	// bit 3-2       00:     LOADBMODE, don't care
	// bit 1-0       10:     LOADAMODE, 10 = load on zero or PRD match
	// EPwm4Regs.CMPCTL.bit.LOADBMODE = 2;
	EPwm4Regs.CMPCTL.bit.LOADAMODE = 2;

	EPwm4Regs.AQCTLA.all = 0x0060; // Action-qualifier control register A
	// EPwm4Regs.AQCTLB.all = 0x0600; // Action-qualifier control register B
	//  bit 15-12     0000:   reserved
	//  bit 11-10     00:     CBD, 00 = do nothing
	//  bit 9-8       00:     CBU, 00 = do nothing
	//  bit 7-6       01:     CAD, 01 = clear
	//  bit 5-4       10:     CAU, 10 = set
	//  bit 3-2       00:     PRD, 00 = do nothing
	//  bit 1-0       00:     ZRO, 00 = do nothing

	EPwm4Regs.AQSFRC.all = 0x0000; // Action-qualifier s/w force register
	// bit 15-8      0's:    reserved
	// bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
	// bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
	// bit 4-3       00:     ACTSFB, don't care
	// bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
	// bit 1-0       00:     ACTSFA, don't care

	EPwm4Regs.AQCSFRC.all = 0x0000; // Action-qualifier continuous s/w force register
	// bit 15-4      0's:    reserved
	// bit 3-2       00:     CSFB, 00 = forcing disabled
	// bit 1-0       00:     CSFA, 00 = forcing disabled

	EPwm4Regs.DBCTL.bit.OUT_MODE = 0; // Deadband disabled
	EPwm4Regs.PCCTL.bit.CHPEN = 0;	  // PWM chopper unit disabled

	EPwm4Regs.TZCTL.bit.TZA = 0x3; // Trip action disabled for output A
	EPwm4Regs.TZCTL.bit.TZB = 0x3; // Trip action disabled for output B

	EPwm4Regs.TBCTL.bit.CTRMODE = 0x2; // Enable the timer in count up/down mode
}

/**
 * @brief Inicializa los registros del EPWM1 para generar un PWM de 10 KHz y DC variable en los pines 00 y 01
 * El 00 responde al CMPA y el 01 responde al CMPB.
 * La escala es de 0 a 7500, siendo 0 el 100% el alto y 7500 el bajo.
 *
 * Oportunidad de mejora: no se están utilizando peescalers, por lo cual "se desperdicia" resolución del timer
 * ya que éste es de 16 bits (alcanza para 65535 cuentas)
 *
 */
void pwm1_init()
{
	EPwm1Regs.TBCTL.bit.CTRMODE = 0x3; // Disable the timer -> NO DEBERIA SER NECESARIA POR LA LINEA 37

	EPwm1Regs.TBCTL.all = 0xC033; // Configure timer control register
	// bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
	// bit 13        0:      PHSDIR, 0 = count down after sync event
	// bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
	// bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
	// bit 6         0:      SWFSYNC, 0 = no software sync produced
	// bit 5-4       11:     SYNCOSEL, 11 = sync-out disabled
	// bit 3         0:      PRDLD, 0 = reload PRD on counter=0 PRD=PERIOD
	// bit 2         0:      PHSEN, 0 = phase control disabled
	// bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

	EPwm1Regs.TBCTR = 0x0000;			 // Clear timer counter
	EPwm1Regs.TBPRD = PROJECT_PERIOD;	 // Levanto la frecuencia a 10 kHz PWM_HALF_PERIOD
	EPwm1Regs.TBPHS.half.TBPHS = 0x0000; // Set timer phase

	// EPwm1Regs.CMPA.half.CMPA = PWM_DUTY_CYCLE;	// Set PWM duty cycle
	EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD * 0.5;
	EPwm1Regs.CMPB = PROJECT_PERIOD * 0.75;

	// EPwm1Regs.ETSEL.all = 0b01100;			// Activo la interrupción EPWM
	//  bit 15        0:      SOCBEN, 0 = disable SOCB
	//  bit 14-12     000:    SOCBSEL, don't care
	//  bit 11        0:      SOCAEN, 0 = enable SOCA
	//  bit 10-8      000:    SOCASEL, 000 = SOCA on PRD event
	//  bit 7-4       0000:   reserved
	//  bit 3         1:      INTEN, 1 = enable interrupt
	//  bit 2-0       000:    INTSEL, don't care
	EPwm1Regs.ETSEL.bit.INTEN = 1;	// habilita interrupcion
	EPwm1Regs.ETSEL.bit.INTSEL = 2; // trigger a en el TBPRD
	EPwm1Regs.ETPS.bit.INTPRD = 1;	// EPWMxINTn Period Select

	// **** COMPARE CONTROL REGISTER
	// EPwm1Regs.CMPCTL.all = 0x0002; // Compare control register
	// bit 15-10     0's:    reserved
	// bit 9         0:      SHDWBFULL, read-only
	// bit 8         0:      SHDWAFULL, read-only
	// bit 7         0:      reserved
	// bit 6         0:      SHDWBMODE, don't care
	// bit 5         0:      reserved
	// bit 4         0:      SHDWAMODE, 0 = shadow mode
	// bit 3-2       00:     LOADBMODE, don't care
	// bit 1-0       10:     LOADAMODE, 10 = load on zero or PRD match
	EPwm1Regs.CMPCTL.bit.LOADBMODE = 2;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = 2;

	EPwm1Regs.AQCTLA.all = 0x0060; // Action-qualifier control register A
	EPwm1Regs.AQCTLB.all = 0x0600; // Action-qualifier control register B
	// bit 15-12     0000:   reserved
	// bit 11-10     00:     CBD, 00 = do nothing
	// bit 9-8       00:     CBU, 00 = do nothing
	// bit 7-6       01:     CAD, 01 = clear
	// bit 5-4       10:     CAU, 10 = set
	// bit 3-2       00:     PRD, 00 = do nothing
	// bit 1-0       00:     ZRO, 00 = do nothing

	EPwm1Regs.AQSFRC.all = 0x0000; // Action-qualifier s/w force register
	// bit 15-8      0's:    reserved
	// bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
	// bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
	// bit 4-3       00:     ACTSFB, don't care
	// bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
	// bit 1-0       00:     ACTSFA, don't care

	EPwm1Regs.AQCSFRC.all = 0x0000; // Action-qualifier continuous s/w force register
	// bit 15-4      0's:    reserved
	// bit 3-2       00:     CSFB, 00 = forcing disabled
	// bit 1-0       00:     CSFA, 00 = forcing disabled

	EPwm1Regs.DBCTL.bit.OUT_MODE = 0; // Deadband disabled
	EPwm1Regs.PCCTL.bit.CHPEN = 0;	  // PWM chopper unit disabled

	EPwm1Regs.TZCTL.bit.TZA = 0x3; // Trip action disabled for output A
	EPwm1Regs.TZCTL.bit.TZB = 0x3; // Trip action disabled for output B

	EPwm1Regs.TBCTL.bit.CTRMODE = 0x2; // Enable the timer in count up/down mode
}

void pwm1_config_interrupt()
{
	//---------------------------------------------------------------------
	//--- Enable the clocks to the ePWM module.
	//--- Note: this should be done after all ePWM modules are configured
	//--- to ensure synchronization between the ePWM modules.
	//---------------------------------------------------------------------
	asm(" EALLOW");						   // Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // HSPCLK to ePWM modules enabled
	PieCtrlRegs.PIECTRL.bit.ENPIE = 0;	   // Disable the PIE
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;	   // Enable EPWM1_INT in PIE group 3
	IER |= 0b0100;						   // Enable INT3 in IER to enable PIE group
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;	   // Enable the PIE
	asm(" CLRC INTM");
	asm(" EDIS"); // Disable EALLOW protected register access
}

void pwm2()
{
	//---------------------------------------------------------------------
	//------------------------------------------------------------------------- Configure ePWM2 to trigger the ADC at 48 kHz rate
	//---------------------------------------------------------------------
	EPwm2Regs.TBCTL.bit.CTRMODE = 0x3; // Disable the timer
	int a = EPwm2Regs.TBCTL.bit.CTRMODE;

	EPwm2Regs.TBCTL.all = 0xC033; // Configure timer control register
								  // bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
								  // bit 13        0:      PHSDIR, 0 = count down after sync event
								  // bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
								  // bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
								  // bit 6         0:      SWFSYNC, 0 = no software sync produced
								  // bit 5-4       11:     SYNCOSEL, 11 = sync-out disabled
								  // bit 3         0:      PRDLD, 0 = reload PRD on counter=0
								  // bit 2         0:      PHSEN, 0 = phase control disabled
								  // bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

	EPwm2Regs.TBCTR = 0x0000;			 // Clear timer counter
	EPwm2Regs.TBPRD = PROJECT_PERIOD;	 // Antes era ADC_SAMPLE_PERIOD;
	EPwm2Regs.TBPHS.half.TBPHS = 0x0000; // Set timer phase

	EPwm2Regs.CMPA.half.CMPA = PROJECT_PERIOD / 2; // Set PWM duty cycle 50%

	EPwm2Regs.ETPS.all = 0x0100; // Configure SOCA

	// bit 15-14     00:     EPWMxSOCB, read-only
	// bit 13-12     00:     SOCBPRD, don't care
	// bit 11-10     00:     EPWMxSOCA, read-only
	// bit 9-8       01:     SOCAPRD, 01 = generate SOCA on first event
	// bit 7-4       0000:   reserved
	// bit 3-2       00:     INTCNT, don't care
	// bit 1-0       00:     INTPRD, don't care

	EPwm2Regs.ETSEL.all = 0x0A00; // Enable SOCA to ADC
	// bit 15        0:      SOCBEN, 0 = disable SOCB
	// bit 14-12     000:    SOCBSEL, don't care
	// bit 11        1:      SOCAEN, 1 = enable SOCA
	// bit 10-8      010:    SOCASEL, 010 = SOCA on PRD event
	// bit 7-4       0000:   reserved
	// bit 3         0:      INTEN, 0 = disable interrupt
	// bit 2-0       000:    INTSEL, don't care

	EPwm2Regs.TBCTL.bit.CTRMODE = 0x0; // Enable the timer in count up mode
	// ---------------------------------- COSAS CAGADAS
	/*
		EPwm2Regs.AQCTLA.all = 0x0060;      // Action-qualifier control register A
	// bit 15-12     0000:   reserved
	// bit 11-10     00:     CBD, 00 = do nothing
	// bit 9-8       00:     CBU, 00 = do nothing
	// bit 7-6       01:     CAD, 01 = clear
	// bit 5-4       10:     CAU, 10 = set
	// bit 3-2       00:     PRD, 00 = do nothing
	// bit 1-0       00:     ZRO, 00 = do nothing

		EPwm2Regs.AQSFRC.all = 0x0000;      // Action-qualifier s/w force register
	// bit 15-8      0's:    reserved
	// bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
	// bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
	// bit 4-3       00:     ACTSFB, don't care
	// bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
	// bit 1-0       00:     ACTSFA, don't care

		EPwm2Regs.AQCSFRC.all = 0x0000;     // Action-qualifier continuous s/w force register
	// bit 15-4      0's:    reserved
	// bit 3-2       00:     CSFB, 00 = forcing disabled
	// bit 1-0       00:     CSFA, 00 = forcing disabled

		EPwm2Regs.DBCTL.bit.OUT_MODE = 0;   // Deadband disabled
		EPwm2Regs.PCCTL.bit.CHPEN = 0;      // PWM chopper unit disabled
		EPwm2Regs.TZCTL.bit.TZA = 0x3;      // Trip action disabled for output A

	// ----------------------------- FIN DE COSAS CAGADAS

		EPwm2Regs.TBCTL.bit.CTRMODE = 0x2;		// Enable the timer in count up mode
		*/
}
