/*
 * Copyright (C) 2013 by Robert Schoch <r.schoch@t-online.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package de.rtlsdr.tests;

import de.rtlsdr.RtlDevice;
import de.rtlsdr.XtalFreq;
import de.rtlsdr.utils.HexDump;

public class TestSettings {

  public static void main(String[] args) {

    try {

      System.out.println("--- test of the static methods ---");

      System.out.println("getDeviceCount(): " + RtlDevice.getDeviceCount());
      System.out.println("getDeviceName(): " + RtlDevice.getDeviceName(0));
      System.out.println("getDeviceUsbStrings(): " + RtlDevice.getDeviceUsbStrings(0));
      System.out.println("getIndexBySerial(): " + RtlDevice.getIndexBySerial("00000991"));
      
      System.out.println("--- test of the instance methods ---");
      
      RtlDevice dev = new RtlDevice(0);

      dev.open();
      System.out.println("open(): RtlDevice=" + dev);
      
      XtalFreq xtalFreq = new XtalFreq(28800000, 28800000);
      System.out.println("dev.setXtalFreq(" + xtalFreq + ")...");
      dev.setXtalFreq(xtalFreq); 
      System.out.println("dev.getXtalFreq(): " + dev.getXtalFreq());      

      System.out.println("dev.getUsbStrings(): " + dev.getUsbStrings()); 
      
      System.out.println("\nEEPROM:");
      byte[] eeprom = new byte[0x100];
      dev.readEeprom(eeprom, 0, 0x100);
      System.out.println(HexDump.dumpHexString(eeprom));

      // eeprom[0xFF] = -1;
      // dev.writeEeprom(eeprom, 0, 0x100);
      
      long centerFreq = 99000000L;
      System.out.println("dev.setCenterFreq(" + centerFreq + ")...");
      dev.setCenterFreq(centerFreq);
      System.out.println("dev.getCenterFreq(): " + dev.getCenterFreq()); 

      int freqCorr = -100;
      System.out.println("dev.setFreqCorrection(" + freqCorr + ")...");
      dev.setFreqCorrection(freqCorr);
      System.out.println("dev.getFreqCorrection(): " + dev.getFreqCorrection()); 
      System.out.println("dev.getXtalFreq(): " + dev.getXtalFreq());  
      
      System.out.println("dev.getTunerType(): " + RtlDevice.TUNER_NAMES.get(dev.getTunerType()));
      
      int[] gains = dev.getTunerGains();
      System.out.println("dev.getTunerGains()...");
      for(int gain:gains) System.out.print(" " + gain);
      System.out.println();
      
      int gain = -58;
      System.out.println("dev.setTunerGain(" + gain + ")...");
      dev.setTunerGain(-58);
      System.out.println("dev.getTunerGain(): " + dev.getTunerGain());  
      
      int ifStage = 1; int ifGain = 0;
      System.out.println("dev.setTunerIfGain(" + ifStage + "," + ifGain + ")...");      
      dev.setTunerIfGain(ifStage, ifGain);

      int gainMode = 1;
      System.out.println("dev.setTunerGainMode(" + gainMode + ")...");      
      dev.setTunerGainMode(gainMode);
      
      int sampleRate = 256000;
      System.out.println("dev.getSampleRate(" + sampleRate + ")...");
      dev.setSampleRate(sampleRate);
      System.out.println("dev.getSampleRate(): " + dev.getSampleRate());

      System.out.println("dev.setTestMode()...");
      dev.setTestMode(1);
      dev.setTestMode(0);
      
      System.out.println("dev.setAgcMode(1)...");
      dev.setAgcMode(1);
      System.out.println("dev.setAgcMode(0)...");
      dev.setAgcMode(0);

      System.out.println("dev.setDirectSampling(1)...");
      dev.setDirectSampling(1);
      System.out.println("dev.getDirectSampling(): " + dev.getDirectSampling());

      System.out.println("dev.setDirectSampling(2)...");
      dev.setDirectSampling(2);
      System.out.println("dev.getDirectSampling(): " + dev.getDirectSampling());
      
      System.out.println("dev.setDirectSampling(0)...");
      dev.setDirectSampling(0);
      System.out.println("dev.getDirectSampling(): " + dev.getDirectSampling());
      
      System.out.println("dev.setOffsetTuning(1)...");
      dev.setOffsetTuning(1);
      System.out.println("dev.getOffsetTuning(): " + dev.getOffsetTuning());
      
      System.out.println("dev.setOffsetTuning(0)...");
      dev.setOffsetTuning(0);
      System.out.println("dev.getOffsetTuning(): " + dev.getOffsetTuning());

      dev.close();
      System.out.println("dev.close(): RtlDevice=" + dev);
      
      System.out.println("--- finished ---");

    } catch (Throwable t) {

      System.out.println("--- exception handled ---");
      t.printStackTrace(System.out);

    }

  }

}
