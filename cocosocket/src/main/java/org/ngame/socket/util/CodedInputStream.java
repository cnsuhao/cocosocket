/**
 * google protobuf 的varint32算法
 */
package org.ngame.socket.util;

import java.io.IOException;

public final class CodedInputStream
{

  /**
   * Create a new CodedInputStream wrapping the given byte array.
   *
   * @param buf
   * @return
   */
  public static CodedInputStream newInstance(final byte[] buf)
  {
    return newInstance(buf, 0, buf.length);
  }

  /**
   * Create a new CodedInputStream wrapping the given byte array slice.
   *
   * @param buf
   * @param off
   * @param len
   * @return
   */
  public static CodedInputStream newInstance(final byte[] buf, final int off, final int len)
  {
    CodedInputStream result = new CodedInputStream(buf, off, len);
    try
    {
      // Some uses of CodedInputStream can be more efficient if they know
      // exactly how many bytes are available.  By pushing the end point of the
      // buffer as a limit, we allow them to get this information via
      // getBytesUntilLimit().  Pushing a limit that we know is at the end of
      // the stream can never hurt, since we can never past that point anyway.
      result.pushLimit(len);
    } catch (IllegalArgumentException ex)
    {
      // The only reason pushLimit() might throw an exception here is if len
      // is negative. Normally pushLimit()'s parameter comes directly off the
      // wire, so it's important to catch exceptions in case of corrupt or
      // malicious data. However, in this case, we expect that len is not a
      // user-supplied value, so we can assume that it being negative indicates
      // a programming error. Therefore, throwing an unchecked exception is
      // appropriate.
      throw new IllegalArgumentException(ex);
    }
    return result;
  }
  // =================================================================
  /**
   * Read a raw Varint from the stream. If larger than 32 bits, discard the
   * upper bits.
   *
   * @return
   * @throws java.io.IOException
   */
  public int readRawVarint32() throws IOException
  {
    byte tmp = readRawByte();
    if (tmp >= 0)
    {
      return tmp;
    }
    int result = tmp & 0x7f;
    if ((tmp = readRawByte()) >= 0)
    {
      result |= tmp << 7;
    } else
    {
      result |= (tmp & 0x7f) << 7;
      if ((tmp = readRawByte()) >= 0)
      {
        result |= tmp << 14;
      } else
      {
        result |= (tmp & 0x7f) << 14;
        if ((tmp = readRawByte()) >= 0)
        {
          result |= tmp << 21;
        } else
        {
          result |= (tmp & 0x7f) << 21;
          result |= (tmp = readRawByte()) << 28;
          if (tmp < 0)
          {
            // Discard upper 32 bits.
            for (int i = 0; i < 5; i++)
            {
              if (readRawByte() >= 0)
              {
                return result;
              }
            }
            throw new IOException();
          }
        }
      }
    }
    return result;
  }

  // -----------------------------------------------------------------
  private final byte[] buffer;
  private int bufferSize;
  private int bufferSizeAfterLimit;
  private int bufferPos;
  /**
   * The total number of bytes read before the current buffer. The total bytes
   * read up to the current position can be computed as
   * {@code totalBytesRetired + bufferPos}. This value may be negative if
   * reading started in the middle of the current buffer (e.g. if the
   * constructor that takes a byte array and an offset was used).
   */
  private int totalBytesRetired;

  /**
   * The absolute position of the end of the current message.
   */
  private int currentLimit = Integer.MAX_VALUE;
  private final int sizeLimit = DEFAULT_SIZE_LIMIT;
  private static final int DEFAULT_SIZE_LIMIT = 64 << 20;  // 64MB

  private CodedInputStream(final byte[] buffer, final int off, final int len)
  {
    this.buffer = buffer;
    bufferSize = off + len;
    bufferPos = off;
    totalBytesRetired = -off;
  }

  /**
   * Sets {@code currentLimit} to (current position) + {@code byteLimit}. This
   * is called when descending into a length-delimited embedded message.
   *
   * <p>
   * Note that {@code pushLimit()} does NOT affect how many bytes the
   * {@code CodedInputStream} reads from an underlying {@code InputStream} when
   * refreshing its buffer. If you need to prevent reading past a certain point
   * in the underlying {@code InputStream} (e.g. because you expect it to
   * contain more data after the end of the message which you need to handle
   * differently) then you must place a wrapper around your {@code InputStream}
   * which limits the amount of data that can be read from it.
   *
   * @param byteLimit
   * @return the old limit.
   */
  public int pushLimit(int byteLimit) throws IllegalArgumentException
  {
    if (byteLimit < 0)
    {
      throw new IllegalArgumentException();
    }
    byteLimit += totalBytesRetired + bufferPos;
    final int oldLimit = currentLimit;
    if (byteLimit > oldLimit)
    {
      throw new IllegalArgumentException();
    }
    currentLimit = byteLimit;
    recomputeBufferSizeAfterLimit();
    return oldLimit;
  }

  private void recomputeBufferSizeAfterLimit()
  {
    bufferSize += bufferSizeAfterLimit;
    final int bufferEnd = totalBytesRetired + bufferSize;
    if (bufferEnd > currentLimit)
    {
      // Limit is in current buffer.
      bufferSizeAfterLimit = bufferEnd - currentLimit;
      bufferSize -= bufferSizeAfterLimit;
    } else
    {
      bufferSizeAfterLimit = 0;
    }
  }
  /**
   * Called with {@code this.buffer} is empty to read more bytes from the input.
   * If {@code mustSucceed} is true, refillBuffer() guarantees that either there
   * will be at least one byte in the buffer when it returns or it will throw an
   * exception. If {@code mustSucceed} is false, refillBuffer() returns false if
   * no more bytes were available.
   */
  private boolean refillBuffer(final boolean mustSucceed) throws IOException
  {
    if (totalBytesRetired + bufferSize == currentLimit)
    {
      // Oops, we hit a limit.
      if (mustSucceed)
      {
        throw new IOException();
      } else
      {
        return false;
      }
    }
    totalBytesRetired += bufferSize;
    bufferPos = 0;
    bufferSize = -1;
    if (bufferSize == 0 || bufferSize < -1)
    {
      throw new IllegalStateException(
              "InputStream#read(byte[]) returned invalid result: " + bufferSize
              + "\nThe InputStream implementation is buggy.");
    }
    if (bufferSize == -1)
    {
      bufferSize = 0;
      if (mustSucceed)
      {
        throw new IOException();
      } else
      {
        return false;
      }
    } else
    {
      recomputeBufferSizeAfterLimit();
      final int totalBytesRead = totalBytesRetired + bufferSize + bufferSizeAfterLimit;
      if (totalBytesRead > sizeLimit || totalBytesRead < 0)
      {
        throw new IOException();
      }
      return true;
    }
  }

  /**
   * Read one byte from the input.
   *
   * @return
   * @throws java.io.IOException
   */
  public byte readRawByte() throws IOException
  {
    if (bufferPos == bufferSize)
    {
      refillBuffer(true);
    }
    return buffer[bufferPos++];
  }
}
