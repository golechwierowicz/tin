czujnix_proto = Proto("czujnix", "CzujniX Protocol")


function buffer_offset(buffer, offset)
    return function(off, num)
        return buffer(offset + off, num)
    end
end

function read_string(buffer, maxlen, tree)
    if maxlen < 4 then return nil, 4 end
    local len = buffer(0, 4):int()
    if maxlen < 4 + len then return nil, 4 + len end
    local subtree = tree:add(buffer(0, 4 + len), "String")
    subtree:add(buffer(0, 4), "Length: ", len)
    subtree:add(buffer(4, len), "Data: ", buffer(4, len):string())
    return subtree, 4 + len
end

function read_array(buffer, maxlen, tree, element_reader)
    if maxlen < 4 then return 0 end

    local len = buffer(0, 4):int()
    buffer = buffer_offset(buffer, 4)
    local total_read = 4

    local subtree = tree:add(buffer(0, total_read), "Array")

    while len > 0 do
        local item, item_len = element_reader(buffer, maxlen - total_read, subtree)

        if item == nil then
            return nil, total_read + item_len
        else
            total_read = total_read + item_len
        end

        len = len - 1
    end

    subtree:set_len(total_read)
    return subtree, total_read
end

local BLOCK_TYPES = {
    SENSOR_COMMON = 1,
    SENSOR_SMOKE = 2,
    SENSOR_INFRARED = 3,
    SENSOR_TEMPERATURE = 4,
    CENTRAL_SERVER_FIRE_ALERT = 5,
    CENTRAL_SERVER_HEARTBEAT = 6,
    CNT_SENSOR_CONFIG = 7,
    CNT_SENSOR_REQUEST_CONFIG = 8
}

local BLOCK_NAMES = {}
for k, v in pairs(BLOCK_TYPES) do
    BLOCK_NAMES[v] = k
end

local BLOCK_PARSERS = {
    [BLOCK_TYPES.SENSOR_COMMON] = function(buffer, maxlen, tree)
        if maxlen < 13 then
            return nil, 13
        end

        local timestamp = buffer(0, 8)
        local lat = buffer(8, 2)
        local long = buffer(10, 2)
        local is_alive = buffer(12, 1)

        tree:add(timestamp, "Timestamp: " .. timestamp:int64())
        tree:add(lat, "Latitude: " .. lat:int())
        tree:add(long, "Longitude: " .. long:int())
        tree:add(is_alive, "IsAlive: " .. (is_alive:int() ~= 0))

        return 13
    end,
    [BLOCK_TYPES.SENSOR_SMOKE] = function(buffer, maxlen, tree)
        if maxlen < 1 then
            return nil, 1
        end

        local smoke_exists = buffer(0, 1)
        tree:add(smoke_exists, "SmokeExists: " .. (smoke_exists:int() ~= 0))

        return 1
    end,
    [BLOCK_TYPES.SENSOR_INFRARED] = function(buffer, maxlen, tree)
        if maxlen < 2 then
            return nil, 2
        end

        local sensor_version = buffer(0, 1)
        local infrared_value = buffer(1, 1)

        tree:add(sensor_version, "SensorVersion: " .. sensor_version:int())
        tree:add(infrared_value, "InfraredValue: " .. infrared_value:int())

        return 2
    end,
    [BLOCK_TYPES.SENSOR_TEMPERATURE] = function(buffer, maxlen, tree)
        if maxlen < 2 then
            return nil, 2
        end

        local temperature = buffer(0, 2)

        tree:add(temperature, "Temperature: " .. temperature:int())

        return 2
    end,
    [BLOCK_TYPES.CENTRAL_SERVER_FIRE_ALERT] = function(buffer, maxlen, tree)
        if maxlen < 16 then
            return nil, 16
        end

        local timestamp = buffer(0, 8)
        local lat = buffer(8, 2)
        local long = buffer(10, 2)
        local alertsCount = buffer(12, 4)

        tree:add(timestamp, "Timestamp: " .. timestamp:int64())
        tree:add(lat, "Latitude: " .. lat:int())
        tree:add(long, "Longitude: " .. long:int())
        tree:add(alertsCount, "AlertsCount: " .. alertsCount:int())

        return 16
    end,
    [BLOCK_TYPES.CENTRAL_SERVER_HEARTBEAT] = function(buffer, maxlen, tree)
        if maxlen < 4 then
            return nil, 4
        end

        local id = buffer(0, 4)

        tree:add(id, "Id: " .. id:int())

        return 4
    end,
    [BLOCK_TYPES.CNT_SENSOR_CONFIG] = function(buffer, maxlen, tree)
        local ips_subtree = tree:add(buffer(0, 1), "CentralsIps")
        local ips, ips_size = read_array(buffer, maxlen, ips_subtree, read_string)
        ips_subtree:set_len(ips_size)

        if ips == nil then
            return nil, ips_size
        end

        local sensor_id = buffer(ips_size, 8)
        tree:add(sensor_id, "SensorId: " .. sensor_id:int64())

        local cnt_ip_subtree = tree:add(buffer(ips_size + 8, 1))
        local cnt_ip, cnt_ip_size = read_string(buffer_offset(buffer, ips_size + 8),
            maxlen - ips_size - 8,
            cnt_ip_subtree)
        cnt_ip_subtree:set_len(cnt_ip_size)

        if cnt_ip == nil then
            return nil, ips_size + 8 + cnt_ip_size
        end

        return ips_size + 8 + cnt_ip_size
    end,
    [BLOCK_TYPES.CNT_SENSOR_REQUEST_CONFIG] = function(buffer, maxlen, tree)
        return 0
    end
}

function czujnix_proto.dissector(buffer, pinfo, tree)
    pinfo.cols.protocol = "CZUJNIX"

    local subtree = tree:add(trivial_proto, buffer(), "CzuniX Protocol Data")

    local total_len = buffer:len()
    local bytes_consumed = 0

    while bytes_consumed < total_len do
        local block_type = buffer(0, 4):int()
        if BLOCK_NAMES[block_type] == nil then
            return 0
        end

        local block_tree = subtree:add(buffer(0, 4), BLOCK_NAMES[block_type])

        bytes_consumed = bytes_consumed + 4
        buffer = buffer_offset(buffer, 4)
        local read, required = BLOCK_PARSERS[block_type](buffer, total_len - bytes_consumed, block_tree)

        if read == nil then
            pktinfo.desegment_offset = bytes_consumed
            pktinfo.desegment_len = required
            return
        else
            bytes_consumed = bytes_consumed + read
            buffer = buffer_offset(buffer, read)
            block_tree:set_len(4 + read)
        end
    end

    return bytes_consumed
end

local tcp_table = DissectorTable.get("tcp.port")
for _, v in ipairs({ ... }) do
    tcp_table:add(v, ring_proto)
end
