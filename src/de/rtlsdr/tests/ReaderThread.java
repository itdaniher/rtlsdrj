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

import de.rtlsdr.RtlCallback;
import de.rtlsdr.RtlDevice;
import de.rtlsdr.utils.HexDump;

public class ReaderThread extends Thread implements RtlCallback {

  RtlDevice dev;

  public ReaderThread(RtlDevice dev) {
    this.dev = dev;
  }

  @Override
  public void run() {

    try {

      System.out.println(" ------------ ReaderThread started ------------ ");

      dev.resetBuffer();
      dev.readAsync(this, 8, 4 * 512);

      System.out.println(" ------------ ReaderThread finished ------------ ");

    } catch (Throwable t) {

      System.out.println(" ------------ ReaderThread aborted ------------ ");
      t.printStackTrace(System.out);

    }

  }

  @Override
  public void rtlData(ByteBuffer buf, int len) {

    byte[] data = new byte[len];
    buf.get(data);

    System.out.println(" ------------ rtlData (" + len + " bytes) ------------ ");
    System.out.println(HexDump.dumpHexString(data, 0, 32));
    System.out.println("  ...");

  }

}
