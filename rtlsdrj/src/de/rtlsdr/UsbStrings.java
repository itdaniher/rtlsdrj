package de.rtlsdr;

public class UsbStrings {

  private String manufacturer;
  private String product;
  private String serial;
  
  public UsbStrings() {}

  public UsbStrings(String manufacturer, String product, String serial) {
    this.manufacturer = manufacturer;
    this.product = product;
    this.serial = serial;
  }
  
  public String getManufacturer() {
    return manufacturer;
  }

  public String getProduct() {
    return product;
  }

  public String getSerial() {
    return serial;
  }

  public void setManufacturer(String manufacturer) {
    this.manufacturer = manufacturer;
  }

  public void setProduct(String product) {
    this.product = product;
  }

  public void setSerial(String serial) {
    this.serial = serial;
  }

  @Override
  public String toString() {
    return "[manufacturer=" + manufacturer +
        ", product=" + product + ", serial=" + serial + "]"; 
  }
  
}
