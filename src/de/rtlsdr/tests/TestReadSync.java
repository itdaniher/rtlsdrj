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

import java.nio.ByteBuffer;

import de.rtlsdr.RtlDevice;
import de.rtlsdr.utils.HexDump;

public class TestReadSync {

  public static void main(String[] args) {

    try {

      int devIndex = 0;
      
      System.out.println("using device at index " + devIndex +
           " (" + RtlDevice.getDeviceName(devIndex) + ")");
      RtlDevice dev = new RtlDevice(devIndex);

      dev.open();

      dev.setSampleRate(48000);
      dev.setCenterFreq(88000000);
      
      System.out.println("dev.resetBuffer()...");
      dev.resetBuffer();      
      
      System.out.println("dev.readSync()...");
      ByteBuffer buf = ByteBuffer.allocateDirect(1024);
      int read = dev.readSync(buf, buf.capacity());
      
      byte[] samples = new byte[read];
      buf.get(samples);

      System.out.println("dev.readSync(): " + read + " bytes read.");
      System.out.println(HexDump.dumpHexString(samples));
      
      dev.close();

    } catch (Throwable t) {

      System.out.println(" ------------ Error in Main ------------ ");
      t.printStackTrace(System.out);

    }

  }

}
