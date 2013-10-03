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
