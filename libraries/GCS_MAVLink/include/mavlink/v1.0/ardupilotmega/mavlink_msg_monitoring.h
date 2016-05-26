// MESSAGE MONITORING PACKING

#define MAVLINK_MSG_ID_MONITORING 159

typedef struct __mavlink_monitoring_t
{
 float data_value_1; ///< data value monitor 1
 float voltage_1; ///< raw voltage if available, zero otherwise
 float data_value_2; ///< data value monitor 2
 float voltage_2; ///< raw voltage if available, zero otherwise
 float data_value_3; ///< data value monitor 3
 float voltage_3; ///< raw voltage if available, zero otherwise
 float data_value_4; ///< data value monitor 4
 float voltage_4; ///< raw voltage if available, zero otherwise
} mavlink_monitoring_t;

#define MAVLINK_MSG_ID_MONITORING_LEN 32
#define MAVLINK_MSG_ID_159_LEN 32

#define MAVLINK_MSG_ID_MONITORING_CRC 50
#define MAVLINK_MSG_ID_159_CRC 50



#define MAVLINK_MESSAGE_INFO_MONITORING { \
	"MONITORING", \
	8, \
	{  { "data_value_1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_monitoring_t, data_value_1) }, \
         { "voltage_1", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_monitoring_t, voltage_1) }, \
         { "data_value_2", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_monitoring_t, data_value_2) }, \
         { "voltage_2", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_monitoring_t, voltage_2) }, \
         { "data_value_3", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_monitoring_t, data_value_3) }, \
         { "voltage_3", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_monitoring_t, voltage_3) }, \
         { "data_value_4", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_monitoring_t, data_value_4) }, \
         { "voltage_4", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_monitoring_t, voltage_4) }, \
         } \
}


/**
 * @brief Pack a monitoring message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param data_value_1 data value monitor 1
 * @param voltage_1 raw voltage if available, zero otherwise
 * @param data_value_2 data value monitor 2
 * @param voltage_2 raw voltage if available, zero otherwise
 * @param data_value_3 data value monitor 3
 * @param voltage_3 raw voltage if available, zero otherwise
 * @param data_value_4 data value monitor 4
 * @param voltage_4 raw voltage if available, zero otherwise
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_monitoring_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       float data_value_1, float voltage_1, float data_value_2, float voltage_2, float data_value_3, float voltage_3, float data_value_4, float voltage_4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MONITORING_LEN];
	_mav_put_float(buf, 0, data_value_1);
	_mav_put_float(buf, 4, voltage_1);
	_mav_put_float(buf, 8, data_value_2);
	_mav_put_float(buf, 12, voltage_2);
	_mav_put_float(buf, 16, data_value_3);
	_mav_put_float(buf, 20, voltage_3);
	_mav_put_float(buf, 24, data_value_4);
	_mav_put_float(buf, 28, voltage_4);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MONITORING_LEN);
#else
	mavlink_monitoring_t packet;
	packet.data_value_1 = data_value_1;
	packet.voltage_1 = voltage_1;
	packet.data_value_2 = data_value_2;
	packet.voltage_2 = voltage_2;
	packet.data_value_3 = data_value_3;
	packet.voltage_3 = voltage_3;
	packet.data_value_4 = data_value_4;
	packet.voltage_4 = voltage_4;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MONITORING_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MONITORING;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MONITORING_LEN, MAVLINK_MSG_ID_MONITORING_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MONITORING_LEN);
#endif
}

/**
 * @brief Pack a monitoring message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param data_value_1 data value monitor 1
 * @param voltage_1 raw voltage if available, zero otherwise
 * @param data_value_2 data value monitor 2
 * @param voltage_2 raw voltage if available, zero otherwise
 * @param data_value_3 data value monitor 3
 * @param voltage_3 raw voltage if available, zero otherwise
 * @param data_value_4 data value monitor 4
 * @param voltage_4 raw voltage if available, zero otherwise
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_monitoring_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           float data_value_1,float voltage_1,float data_value_2,float voltage_2,float data_value_3,float voltage_3,float data_value_4,float voltage_4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MONITORING_LEN];
	_mav_put_float(buf, 0, data_value_1);
	_mav_put_float(buf, 4, voltage_1);
	_mav_put_float(buf, 8, data_value_2);
	_mav_put_float(buf, 12, voltage_2);
	_mav_put_float(buf, 16, data_value_3);
	_mav_put_float(buf, 20, voltage_3);
	_mav_put_float(buf, 24, data_value_4);
	_mav_put_float(buf, 28, voltage_4);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MONITORING_LEN);
#else
	mavlink_monitoring_t packet;
	packet.data_value_1 = data_value_1;
	packet.voltage_1 = voltage_1;
	packet.data_value_2 = data_value_2;
	packet.voltage_2 = voltage_2;
	packet.data_value_3 = data_value_3;
	packet.voltage_3 = voltage_3;
	packet.data_value_4 = data_value_4;
	packet.voltage_4 = voltage_4;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MONITORING_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MONITORING;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MONITORING_LEN, MAVLINK_MSG_ID_MONITORING_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MONITORING_LEN);
#endif
}

/**
 * @brief Encode a monitoring struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param monitoring C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_monitoring_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_monitoring_t* monitoring)
{
	return mavlink_msg_monitoring_pack(system_id, component_id, msg, monitoring->data_value_1, monitoring->voltage_1, monitoring->data_value_2, monitoring->voltage_2, monitoring->data_value_3, monitoring->voltage_3, monitoring->data_value_4, monitoring->voltage_4);
}

/**
 * @brief Encode a monitoring struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param monitoring C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_monitoring_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_monitoring_t* monitoring)
{
	return mavlink_msg_monitoring_pack_chan(system_id, component_id, chan, msg, monitoring->data_value_1, monitoring->voltage_1, monitoring->data_value_2, monitoring->voltage_2, monitoring->data_value_3, monitoring->voltage_3, monitoring->data_value_4, monitoring->voltage_4);
}

/**
 * @brief Send a monitoring message
 * @param chan MAVLink channel to send the message
 *
 * @param data_value_1 data value monitor 1
 * @param voltage_1 raw voltage if available, zero otherwise
 * @param data_value_2 data value monitor 2
 * @param voltage_2 raw voltage if available, zero otherwise
 * @param data_value_3 data value monitor 3
 * @param voltage_3 raw voltage if available, zero otherwise
 * @param data_value_4 data value monitor 4
 * @param voltage_4 raw voltage if available, zero otherwise
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_monitoring_send(mavlink_channel_t chan, float data_value_1, float voltage_1, float data_value_2, float voltage_2, float data_value_3, float voltage_3, float data_value_4, float voltage_4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MONITORING_LEN];
	_mav_put_float(buf, 0, data_value_1);
	_mav_put_float(buf, 4, voltage_1);
	_mav_put_float(buf, 8, data_value_2);
	_mav_put_float(buf, 12, voltage_2);
	_mav_put_float(buf, 16, data_value_3);
	_mav_put_float(buf, 20, voltage_3);
	_mav_put_float(buf, 24, data_value_4);
	_mav_put_float(buf, 28, voltage_4);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MONITORING, buf, MAVLINK_MSG_ID_MONITORING_LEN, MAVLINK_MSG_ID_MONITORING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MONITORING, buf, MAVLINK_MSG_ID_MONITORING_LEN);
#endif
#else
	mavlink_monitoring_t packet;
	packet.data_value_1 = data_value_1;
	packet.voltage_1 = voltage_1;
	packet.data_value_2 = data_value_2;
	packet.voltage_2 = voltage_2;
	packet.data_value_3 = data_value_3;
	packet.voltage_3 = voltage_3;
	packet.data_value_4 = data_value_4;
	packet.voltage_4 = voltage_4;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MONITORING, (const char *)&packet, MAVLINK_MSG_ID_MONITORING_LEN, MAVLINK_MSG_ID_MONITORING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MONITORING, (const char *)&packet, MAVLINK_MSG_ID_MONITORING_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MONITORING_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_monitoring_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float data_value_1, float voltage_1, float data_value_2, float voltage_2, float data_value_3, float voltage_3, float data_value_4, float voltage_4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, data_value_1);
	_mav_put_float(buf, 4, voltage_1);
	_mav_put_float(buf, 8, data_value_2);
	_mav_put_float(buf, 12, voltage_2);
	_mav_put_float(buf, 16, data_value_3);
	_mav_put_float(buf, 20, voltage_3);
	_mav_put_float(buf, 24, data_value_4);
	_mav_put_float(buf, 28, voltage_4);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MONITORING, buf, MAVLINK_MSG_ID_MONITORING_LEN, MAVLINK_MSG_ID_MONITORING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MONITORING, buf, MAVLINK_MSG_ID_MONITORING_LEN);
#endif
#else
	mavlink_monitoring_t *packet = (mavlink_monitoring_t *)msgbuf;
	packet->data_value_1 = data_value_1;
	packet->voltage_1 = voltage_1;
	packet->data_value_2 = data_value_2;
	packet->voltage_2 = voltage_2;
	packet->data_value_3 = data_value_3;
	packet->voltage_3 = voltage_3;
	packet->data_value_4 = data_value_4;
	packet->voltage_4 = voltage_4;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MONITORING, (const char *)packet, MAVLINK_MSG_ID_MONITORING_LEN, MAVLINK_MSG_ID_MONITORING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MONITORING, (const char *)packet, MAVLINK_MSG_ID_MONITORING_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MONITORING UNPACKING


/**
 * @brief Get field data_value_1 from monitoring message
 *
 * @return data value monitor 1
 */
static inline float mavlink_msg_monitoring_get_data_value_1(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field voltage_1 from monitoring message
 *
 * @return raw voltage if available, zero otherwise
 */
static inline float mavlink_msg_monitoring_get_voltage_1(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field data_value_2 from monitoring message
 *
 * @return data value monitor 2
 */
static inline float mavlink_msg_monitoring_get_data_value_2(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field voltage_2 from monitoring message
 *
 * @return raw voltage if available, zero otherwise
 */
static inline float mavlink_msg_monitoring_get_voltage_2(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field data_value_3 from monitoring message
 *
 * @return data value monitor 3
 */
static inline float mavlink_msg_monitoring_get_data_value_3(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field voltage_3 from monitoring message
 *
 * @return raw voltage if available, zero otherwise
 */
static inline float mavlink_msg_monitoring_get_voltage_3(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field data_value_4 from monitoring message
 *
 * @return data value monitor 4
 */
static inline float mavlink_msg_monitoring_get_data_value_4(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field voltage_4 from monitoring message
 *
 * @return raw voltage if available, zero otherwise
 */
static inline float mavlink_msg_monitoring_get_voltage_4(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Decode a monitoring message into a struct
 *
 * @param msg The message to decode
 * @param monitoring C-struct to decode the message contents into
 */
static inline void mavlink_msg_monitoring_decode(const mavlink_message_t* msg, mavlink_monitoring_t* monitoring)
{
#if MAVLINK_NEED_BYTE_SWAP
	monitoring->data_value_1 = mavlink_msg_monitoring_get_data_value_1(msg);
	monitoring->voltage_1 = mavlink_msg_monitoring_get_voltage_1(msg);
	monitoring->data_value_2 = mavlink_msg_monitoring_get_data_value_2(msg);
	monitoring->voltage_2 = mavlink_msg_monitoring_get_voltage_2(msg);
	monitoring->data_value_3 = mavlink_msg_monitoring_get_data_value_3(msg);
	monitoring->voltage_3 = mavlink_msg_monitoring_get_voltage_3(msg);
	monitoring->data_value_4 = mavlink_msg_monitoring_get_data_value_4(msg);
	monitoring->voltage_4 = mavlink_msg_monitoring_get_voltage_4(msg);
#else
	memcpy(monitoring, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MONITORING_LEN);
#endif
}
