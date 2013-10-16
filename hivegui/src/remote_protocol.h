#if !defined(REMOTE_PROTOCOL__)
#define REMOTE_PROTOCOL__

#include "stdafx.h"
enum CMD {
    VIDEO = 1,
    AUDIO
  };

namespace remote { 
	class MultiplexPacket {
	public:
		MultiplexPacket(){};
		int channel_id;
		int cmd;//enum CMD
		string chanel_name;
		msgpack::type::raw_ref data ;
		MSGPACK_DEFINE(channel_id,chanel_name,data);
	};

	class ClientResolution {
	public:
		// Legacy width and height of the client in Density-Independent Pixels
		int dips_width ;
		int dips_height ;

		// Width and height of the client in device pixels.
		int width ;
		int height ;
		// Horizontal and vertical DPI of the screen. If either of these is zero or
		// unset, the corresponding DPI should be assumed to be 96 (Windows' default)
		int x_dpi ;
		int y_dpi ;
		MSGPACK_DEFINE(dips_width,dips_height,width,height,x_dpi,y_dpi);
	};
	class VideoControl {
	public:
		bool enable;
		MSGPACK_DEFINE(enable);
	};

	class AudioControl {
	public:
		bool enable;
		MSGPACK_DEFINE(enable);
	};
	
	class CursorShapeInfo {
	public:
	 int width ;
	 int height ;

	// X,Y coordinates (relative to upper-left corner) of the cursor hotspot.
	int hotspot_x ;
	int hotspot_y ;

	// Cursor pixmap data in 32-bit BGRA format.
	msgpack::type::raw_ref data ;
	MSGPACK_DEFINE(width,height,hotspot_x,hotspot_y,data);
	};

	class Capabilities {
	public:
	// List of capabilities supported by the sender (case sensitive, capabilities
	// are separated by spaces).
		string capabilities ;
		MSGPACK_DEFINE(capabilities);
	};

	class PairingRequest {
	public:
	// Human-readable name of the client.
	 string client_name ;
	 MSGPACK_DEFINE(client_name);
	};	

	class PairingResponse {
	public:
	// Unique identifier for this client.
	  string client_id ;

	// Shared secret for this client.
	 string shared_secret ;
	 MSGPACK_DEFINE(client_id,shared_secret);
};

class ExtensionMessage {
	public:
  // The message type. This is used to dispatch the message to the correct
  // recipient.
   string type ;

  // String-encoded message data. The client and host must agree on the encoding
  // for each message type; different message types need not shared the same
  // encoding.
   string data ;
    MSGPACK_DEFINE(type,data);
};

// The keyboard (Caps/Num) lock states.
  
class KeyEvent {

public:
	enum LockStates {
    LOCK_STATES_CAPSLOCK ,
    LOCK_STATES_NUMLOCK 
  };
  // The Windows Virtual Key code.
  // int keycode ;
   bool pressed ;

  // The USB key code.
  // The upper 16-bits are the USB Page (0x07 for key events).
  // The lower 16-bits are the USB Usage ID (which identifies the actual key).
   unsigned int usb_keycode ;

  // The keyboard lock states.
   unsigned int lock_states ;
   MSGPACK_DEFINE(pressed,usb_keycode,lock_states);
};

// Defines a mouse event class on the event channel. 
class MouseEvent {

  enum MouseButton {
    BUTTON_UNDEFINED = 0,
    BUTTON_LEFT = 1,
    BUTTON_MIDDLE = 2,
    BUTTON_RIGHT = 3,
    BUTTON_MAX = 4,
  };
public:
  // Mouse position information.
   int x ;
   int y ;

  // Mouse button event.
   int button ; //todo MouseButton
   bool button_down ;

  // Mouse wheel information.
  // These values encode the number of pixels and 'ticks' of movement that
  // would result from the wheel event on the client system.
   float wheel_delta_x ;
   float wheel_delta_y ;
   float wheel_ticks_x ;
   float wheel_ticks_y ;

  // Mouse movement information. Provided only when mouse lock is engaged.
   int delta_x ;
   int delta_y ;
   MSGPACK_DEFINE(x,y,button,button_down,wheel_delta_x,wheel_delta_y,wheel_ticks_x,wheel_ticks_y);
};

// Defines an event that sends clipboard data between peers.
class ClipboardEvent {
public:
  // The MIME type of the data being sent.
   string mime_type ;

  // The data being sent.
   msgpack::type::raw_ref data ;
   MSGPACK_DEFINE(mime_type,data);
};



class ControlMessage {
	public:
   ClipboardEvent clipboard_event ;
   ClientResolution client_resolution ;
   CursorShapeInfo cursor_shape ;
   VideoControl video_control ;
   AudioControl audio_control ;
   Capabilities capabilities ;
   PairingRequest pairing_request ;
   PairingResponse pairing_response ;
   ExtensionMessage extension_message ;
   MSGPACK_DEFINE(clipboard_event,client_resolution,cursor_shape,video_control,audio_control,capabilities,pairing_request,pairing_response,extension_message);
};

// Defines an event message on the event channel.
class EventMessage {
	public:
   long long sequence_number ;  // Client timestamp for event.
   KeyEvent key_event ;
   MouseEvent mouse_event ;
   MSGPACK_DEFINE(sequence_number,key_event,mouse_event);
};


class VideoPacketFormat {
public:
  // Identifies how the image was encoded.
  enum Encoding {
    ENCODING_INVALID = -1,
    ENCODING_VERBATIM = 0,
    ENCODING_ZLIB = 1,
    ENCODING_VP8 = 2
  };

  // X,Y coordinates (in screen pixels) for origin of this update.
   int x ;
   int y ;

  // Width, height (in screen pixels) for this update.
   int width ;
   int height ;

  // The encoding used for this image update.
  // Encoding encoding;
  int encoding;
  // Width and height of the whole screen.
   int screen_width ;
   int screen_height ;

  // Horizontal and vertical DPI of the screen. If either of these is zero or
  // unset, the corresponding DPI should be assumed to be 96 (Windows' default)
   int x_dpi ;
   int y_dpi ;
   MSGPACK_DEFINE(x,y,width,height,encoding,screen_width,screen_height,x_dpi,y_dpi);
};

// TODO(hclam): Remove this class once we can obtain dirty rects from libvpx.
class Rect {
	public:
   int x ;
   int y ;
   int width;
   int height ;
   MSGPACK_DEFINE(x,y,width,height);
};

class VideoPacket {
public:
  // Bitmasks for use in the flags field below.
  //
  // The encoder may fragment one update into multiple partitions.
  // Each partition may be divided into multiple packets depending on
  // how the encoder outputs data. Thus, one update can logically
  // consist of multiple packets. The FIRST_PACKET and LAST_PACKET
  // flags are used to indicate the start and end of a partition. The
  // LAST_PARTITION flag is set for the last packet in the last
  // partition. Here are notable consequences:
  //  * Both FIRST_PACKET and LAST_PACKET may be set if an update is only
  //    one packet long.
  //  * The VideoPacketFormat is only supplied in a FIRST_PACKET.
  //  * LAST_PARTITION can be set only in packet that has LAST_PACKET set.
  //  * An local update cannot change format between a FIRST_PACKET and
  //    a LAST_PACKET.
  //  * All packets in one logical update must be processed in order, and
  //    packets may not be skipped.
  enum Flags {
    FIRST_PACKET = 1,
    LAST_PACKET = 2,
    LAST_PARTITION = 4,
	FIRST_LAST_PARTITION = 5 //only one packet,FIRST_PACKET|LAST_PARTITION
  };
   int flags;  //default 0

  // The sequence number of the partial data for updating a rectangle. default 0
   int sequence_number ;

   int timestamp ; //default 0

  // This is provided on the first packet of the rectangle data, when
  // the flags has FIRST_PACKET set.
   VideoPacketFormat format ;

   msgpack::type::raw_ref data ;

  

  // Time in milliseconds spent in capturing this video frame.
   int capture_time_ms ;

  // Time in milliseconds spent in encoding this video frame.
   int encode_time_ms ;

  // The most recent sequence number received from the client on the event
  // channel.
   long long client_sequence_number ;

 
   MSGPACK_DEFINE(flags,sequence_number,timestamp,format,data,capture_time_ms,encode_time_ms,client_sequence_number);
};


class AudioPacket {
public:
   int timestamp ;

  // Packets with raw data must have exactly one data field.
  // For those packets, samples are signed and represented using little endian.
  // Some encoders (eg. Speex) may add multiple data fields to separate encoded
  // frames.

  std::vector<msgpack::type::raw_ref> data   ;

  enum Encoding {
    ENCODING_INVALID = -1,
    ENCODING_RAW = 0,
    ENCODING_OPUS = 1,
    ENCODING_SPEEX = 2
  };

   int encoding;

  enum SamplingRate {
    SAMPLING_RATE_INVALID = -1,
    SAMPLING_RATE_44100 = 44100,
    SAMPLING_RATE_48000 = 48000,
  };

   int sampling_rate ;

  enum BytesPerSample {
    BYTES_PER_SAMPLE_INVALID = -1,
    BYTES_PER_SAMPLE_2 = 2
  };

   int bytes_per_sample ;

  enum Channels {
    CHANNELS_INVALID = -1,
    CHANNELS_MONO = 1,
    CHANNELS_STEREO = 2
  };

   int channels ;
   MSGPACK_DEFINE(timestamp,data,encoding,sampling_rate,bytes_per_sample,channels);
};


}
#endif