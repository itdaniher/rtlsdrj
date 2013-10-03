package de.rtlsdr;

public class XtalFreq {

  private int rtlFreq;
  private int tunerFreq;
  
  public XtalFreq() {}

  public XtalFreq(int rtlFreq, int tunerFreq) {
    this.rtlFreq = rtlFreq;
    this.tunerFreq = tunerFreq;
  }

  public int getRtlFreq() {
    return rtlFreq;
  }

  public int getTunerFreq() {
    return tunerFreq;
  }

  public void setRtlFreq(int rtlFreq) {
    this.rtlFreq = rtlFreq;
  }

  public void setTunerFreq(int tunerFreq) {
    this.tunerFreq = tunerFreq;
  }
  
  @Override
  public String toString() {
    return "[rtlFreq=" + rtlFreq +
        ", tunerFreq=" + tunerFreq + "]"; 
  }
  
}
