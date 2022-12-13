/*
 *   $Id: pwm.ino,v 1.3 2022/12/13 10:08:07 gaijin Exp $
 *
 * Simple, crude, blocking PWM for the onboard LED
 * pin on the Pico-W (which is on the wireless
 * module and -not- on the RP2040 itself, so we can't
 * just use analogWrite() to drive it).
 *
 * Note that TenCycles() itself is blocking, so there
 * are no delays in these calling functions.
 */

#define MSSTEP		30	// Millisecs at each step value.
#define MAXCYCLE	10
#define MINCYCLE	0


/*
 * Write batched cycles of PWM at a given
 * brightness level.
 */
void DoPWM(uint8_t blevel) {
  /*
   * Batch our pin writes into blocks of 10ms.
   */
  for (uint8_t i = 0; i < (MSSTEP / 10); i++) {
    TenCycles(blevel);
  }
}
     

/*
 * Bit-banged PWM for non-PWM pin (Pico-W LED).
 */
void TenCycles(uint8_t level) {
  for (uint8_t i = MINCYCLE; i < MAXCYCLE; i++ ) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(level);
    
    digitalWrite(LED_BUILTIN, LOW);
    delay(MAXCYCLE - level);
  }
}

/*
 * Crude up/down fader using "manual" PWM on a 
 * non-PWM pin.
 */
void fader() {
  uint8_t i;
  // Fade up from 0.
  for (i = 0; i < 10; i++) {
  	DoPWM(i);
  }

  // Fade back down from 10.
  for (i = 10; i > 0; i--) {
  	DoPWM(i);
  }
}
