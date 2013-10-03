package de.rtlsdr;

import java.nio.ByteBuffer;

public interface RtlCallback {

  public void rtlData(ByteBuffer buf, int len);
  
}
