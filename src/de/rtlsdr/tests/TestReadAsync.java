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

public class TestReadAsync {

  public static void main(String[] args) {

    try {

      int devIndex = 0;
      
      System.out.println("using device at index " + devIndex +
           " (" + RtlDevice.getDeviceName(devIndex) + ")");
      RtlDevice dev = new RtlDevice(devIndex);

      dev.open();

      dev.setSampleRate(48000);
      dev.setCenterFreq(88000000);
      
      System.out.println("The ReaderThread should be started now - waiting 2s. ...");
      ReaderThread t = new ReaderThread(dev);
      t.start();
      
      Thread.sleep(2000);
      
      System.out.println("The ReaderThread should be stopped now - waiting 8s. ...");
      dev.cancelAsync();
      Thread.sleep(8000);

      dev.close();

    } catch (Throwable t) {

      System.out.println(" ------------ Error in Main ------------ ");
      t.printStackTrace(System.out);

    }

  }

}
