#include <stdexcept>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( const uint64_t capacity ) : capacity_(capacity + 1), front(0), back(0), closed(false), errored(false), bytes_pushed_(0), bytes_popped_(0), buffer(capacity + 1)
{
  buffer.resize(capacity);
}

bool ByteStream::full() const {
  return (back + 1) % capacity_ == front;
}

void Writer::push( const string& data )
{
  // Your code here.
  size_t const s = data.size();
  for (size_t i = 0; i < s; i++) {
    if (full()) {
      return;
    }
    buffer[back] = data[i];
    back = (back + 1) % capacity_;
    bytes_pushed_++;
  }
}

void Writer::close()
{
  // Your code here.
  closed = true;
}

void Writer::set_error()
{
  // Your code here.
  errored = true;
}

bool Writer::is_closed() const
{
  // Your code here.
  return closed;
}

uint64_t Writer::available_capacity() const
{
  uint64_t filled;
  if (back >= front) {
    filled = back - front;
  } else {
    filled = capacity_ - (front - back);
  }
  // capacity_ includes one extra slot to differentiate full/empty,
  // so the real usable capacity is capacity_ - 1.
  return (capacity_ - 1) - filled;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return bytes_pushed_;
}

string_view Reader::peek() const
{
  if (front <= back) {
    // contiguous data
    return {&buffer[front], back - front};
  }     // wrapped: only return the part until end of buffer
    return {&buffer[front], capacity_ - front};

}

bool Reader::is_finished() const
{
  return bytes_buffered() == 0 && closed;
}

bool Reader::has_error() const
{
  // Your code here.
  return errored;
}

void Reader::pop( const uint64_t len )
{
  // Your code here.
  for (uint64_t i = 0; i < len; i++) {
    if (back == front) {
      return;
    }
    front = (front + 1) % capacity_;
    bytes_popped_++;
  }
}

uint64_t Reader::bytes_buffered() const
{
  if (back >= front) {
    return back - front;
  }
  return capacity_ - (front - back);
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return bytes_popped_;
}
