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
