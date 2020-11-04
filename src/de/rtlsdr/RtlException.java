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

public class RtlException extends Exception {
  
  private static final long serialVersionUID = 5665145146659831827L;

  public RtlException() {
    super();
  }

  public RtlException(String message, Throwable cause) {
    super(message, cause);
  }

  public RtlException(String message) {
    super(message);
  }

  public RtlException(Throwable cause) {
    super(cause);
  }

}
