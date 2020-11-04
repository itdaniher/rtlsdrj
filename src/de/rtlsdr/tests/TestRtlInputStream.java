package de.rtlsdr.tests;

import java.io.InputStream;

import de.rtlsdr.RtlDevice;
import de.rtlsdr.RtlInputStream;

public class TestRtlInputStream {

  public static void main(String[] args) {

    try {

      System.out.println("Main started");

      RtlDevice dev = new RtlDevice(0);

      dev.open();
      dev.setSampleRate(262144);
      dev.setCenterFreq(88000000);

      System.out.println("Main: creating the InputStream");
      InputStream is = new RtlInputStream(dev, 0, 0);
      Thread.sleep(4000);
      
      System.out.println("Main: closing the InputStream");
      is.close();
      Thread.sleep(1500);

      System.out.println("Main: closing the RtlDevice");
      dev.close();

      System.out.println("Main finished");

    } catch (Throwable t) {

      System.out.println(" --- error ---");
      t.printStackTrace(System.out);

    }

  }

}
