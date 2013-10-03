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

package de.rtlsdr;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

public class RtlDevice {

  private static final String MSG_DEVICE_ALREADY_OPEN = "device already open";
  private static final String MSG_DEVICE_NOT_OPEN = "device not open";

  public final static int TUNER_UNKNOWN = 0;
  public final static int TUNER_E4000 = 1;
  public final static int TUNER_FC0012 = 2;
  public final static int TUNER_FC0013 = 3;
  public final static int TUNER_FC2580 = 4;
  public final static int TUNER_R820T = 5;
  
  public static final Map<Integer, String> TUNER_NAMES = new HashMap<Integer, String>();
  
  static {

    System.loadLibrary("rtlsdrj");
    
    TUNER_NAMES.put(TUNER_UNKNOWN, "unknown");
    TUNER_NAMES.put(TUNER_E4000, "Elonics E4000");
    TUNER_NAMES.put(TUNER_FC0012, "Fiticomm FC0012");
    TUNER_NAMES.put(TUNER_FC0013, "Fiticomm FC0013");
    TUNER_NAMES.put(TUNER_FC2580, "SiliconMotion FC2580");
    TUNER_NAMES.put(TUNER_R820T, "Rafael Micro R820T");
  }

  private final int devIndex;
  private long devHandle;
  
  public RtlDevice(int devIndex) {
    
    this.devIndex = devIndex;
    this.devHandle = 0L;
    
  }

  public static native int getDeviceCount();
  public static native String getDeviceName(int index);
  public static native UsbStrings getDeviceUsbStrings(int index);
  public static native int getIndexBySerial(String serial);
  
  public native void open();
  public native void close();

  public native void setXtalFreq(XtalFreq freq);
  public native XtalFreq getXtalFreq();
  public native UsbStrings getUsbStrings();
  
  public native void writeEeprom(byte[] data, int offset, int len);
  public native void readEeprom(byte[] data, int offset, int len);
  
  public native void setCenterFreq(long freq);
  public native long getCenterFreq();
  
  public native void setFreqCorrection(int ppm);
  public native int getFreqCorrection();
  
  public native int getTunerType();
  public native int[] getTunerGains();
  
  public native void setTunerGain(int gain);
  public native int getTunerGain();
  
  public native void setTunerIfGain(int stage, int gain);
  public native void setTunerGainMode(int manual);
  
  public native void setSampleRate(int rate);
  public native int getSampleRate();
  
  public native void setTestMode(int on);
  public native void setAgcMode(int on);
  
  public native void setDirectSampling(int on);
  public native int getDirectSampling();
  
  public native void setOffsetTuning(int on);
  public native int getOffsetTuning();
  
  public native void resetBuffer();
  public native int readSync(ByteBuffer buf, int len);
  public native void readAsync(RtlCallback cb, int numBuffers, int bufferLen);
  public native void cancelAsync();
  
  @Override
  public String toString() {
    return "RtlDevice[devIndex=" + devIndex +
        ", devHandle=" + devHandle + "]"; 
  }
  
}
