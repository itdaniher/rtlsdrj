package de.rtlsdr.tests;

import de.rtlsdr.RtlDevice;
import de.rtlsdr.XtalFreq;
import de.rtlsdr.utils.HexDump;

public class Test {

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
      
      XtalFreq xtalFreq = new XtalFreq(28800001, 28800002);
      System.out.println("dev.setXtalFreq(" + xtalFreq + ")...");
      dev.setXtalFreq(xtalFreq); 
      System.out.println("dev.getXtalFreq(): " + dev.getXtalFreq());      

      System.out.println("dev.getUsbStrings(): " + dev.getUsbStrings()); 
      
      System.out.println("\nEEPROM:");
      byte[] eeprom = new byte[0x100];
      dev.readEeprom(eeprom, 0, 0x100);
      System.out.println(HexDump.dumpHexString(eeprom));
      
      long centerFreq = 99000000L;
      System.out.println("dev.setCenterFreq(" + centerFreq + ")...");
      dev.setCenterFreq(centerFreq);
      System.out.println("dev.getCenterFreq(): " + dev.getCenterFreq()); 

      int freqCorr = 500;
      System.out.println("dev.setFreqCorrection(" + freqCorr + ")...");
      dev.setFreqCorrection(freqCorr);
      System.out.println("dev.getFreqCorrection(): " + dev.getFreqCorrection()); 
      System.out.println("dev.getXtalFreq(): " + dev.getXtalFreq());  
      
      dev.close();
      System.out.println("close(): RtlDevice=" + dev);
      
      System.out.println("--- finished ---");

    } catch (Throwable t) {

      System.out.println("--- handled exception ---");
      t.printStackTrace(System.out);

    }

  }

}
