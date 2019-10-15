package de.rtlsdr;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;


public class RtlInputStream extends InputStream {

  private final RtlDevice rtlDevice;
  private final ReaderThread readerThread;
 
  private final int numBuffers;
  private final int bufferSize;
  private final byte[][] buffers;

  private int readIndex; // buffer index for the read (skip) methods to fetch the bytes
  private int fillIndex; // buffer index for the RtlCallback to fill the buffers
  
  /**
   * Creates a new RtlInputStream for the given RtlDevice
   * @param rtlDevice - an valid RtlDevice instance
   * @param numBuffers - the number of buffers in this class, if set to 0
   *  it uses 4 as default. But we call rtlsdr_read_async() always width 0 to use
   *  the default of 32 buffers there.  
   * @param bufferSize - must be multiple of 512, default buffer length is 262144
   *  This is the size for a single buffer in this class. We call rtlsdr_read_async()
   *  width the same value.
   */
  public RtlInputStream(RtlDevice rtlDevice, int numBuffers, int bufferSize) {
    
    this.rtlDevice = rtlDevice;
    
    if(numBuffers == 0) this.numBuffers = 4; else this.numBuffers = numBuffers;
    if(bufferSize == 0) this.bufferSize = 262144; else this.bufferSize = bufferSize;
    this.buffers = new byte[this.numBuffers][this.bufferSize];
    
    readIndex = 0;
    fillIndex = 0;
    
    readerThread = new ReaderThread();
    readerThread.start();
    
  }
  
  private class ReaderThread extends Thread implements RtlCallback {
    
    long nanoTime;
    
    public void run() {
      System.out.println("ReaderThread: started");
      rtlDevice.resetBuffer();
      // Warning: Values above 62 for numBuffers causes crashes!
      // The file hs_err_pidXXXX.log says: Problematic frame:
      // C  [libusb-1.0.dll+0xed94]
      rtlDevice.readAsync(this, 0, bufferSize);
      System.out.println("ReaderThread: stopped");
    }

    @Override
    public void rtlData(ByteBuffer buf, int len) {
      
      long newTime = System.nanoTime();
      long diffTime = newTime - nanoTime;
      nanoTime = newTime;
      
      System.out.println("ReaderThread: rtlData(): " + len + " bytes, " +
          (diffTime>0?(len*500000000L)/diffTime:"???") + " kSPS (I+Q)");
      
    }
    
  }
  
  @Override
  public int available() throws IOException {
    // TODO Auto-generated method stub
    return super.available();
  }

  @Override
  public void close() throws IOException {
    if(readerThread.isAlive()) rtlDevice.cancelAsync();
  }

  @Override
  public int read() throws IOException {
    return 0;
  }

  @Override
  public int read(byte[] b) throws IOException {
    // TODO Auto-generated method stub
    return super.read(b);
  }

  @Override
  public int read(byte[] b, int off, int len) throws IOException {
    // TODO Auto-generated method stub
    return super.read(b, off, len);
  }

  @Override
  public long skip(long n) throws IOException {
    // TODO Auto-generated method stub
    return super.skip(n);
  }

}
