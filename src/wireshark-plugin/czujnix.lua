czujnix_proto = Proto("czujnix", "CzujniX Protocol")


function buffer_offset(buffer, offset)
    return function(off, num)
        return buffer(offset + off, num)
    end
end

function read_string(buffer, maxlen, tree)
    if maxlen < 1 then return nil, 4 end
    local len = buffer(0, 1):int()
    if maxlen < 1 + len then return nil, 1 + len end
    local subtree = tree:add(buffer(0, 1 + len), "String")
    subtree:add(buffer(0, 1), "Length: ", len)
    subtree:add(buffer(1, len), "Data: ", buffer(1, len):string())
    return subtree, 1 + len
end

function read_array(buffer, maxlen, tree, element_reader)
    if maxlen < 8 then return 0 end

    local len = buffer(0, 8):int64()
    buffer = buffer_offset(buffer, 8)
    local total_read = 8

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
        tree:add(is_alive, "IsAlive: " .. (is_alive:int() ~= 0 and "true" or "false"))

        return 13
    end,
    [BLOCK_TYPES.SENSOR_SMOKE] = function(buffer, maxlen, tree)
        if maxlen < 8 then
            return nil, 8
        end

        local smoke_exists = buffer(0, 8)
        tree:add(smoke_exists, "SmokeValue: " .. smoke_exists:float())

        return 8
    end,
    [BLOCK_TYPES.SENSOR_INFRARED] = function(buffer, maxlen, tree)
        if maxlen < 8 then
            return nil, 8
        end

        local infrared_value = buffer(0, 8)

        tree:add(infrared_value, "InfraredValue: " .. infrared_value:float())

        return 8
    end,
    [BLOCK_TYPES.SENSOR_TEMPERATURE] = function(buffer, maxlen, tree)
        if maxlen < 8 then
            return nil, 8
        end

        local temperature = buffer(0, 8)

        tree:add(temperature, "Temperature: " .. temperature:float())

        return 8
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
        local port_id = buffer(0, 2)
        tree:add(port_id, "PortId: " .. port_id:int())
        buffer = buffer_offset(buffer, 2)
        maxlen = maxlen - 2

        local cnt_ip_subtree = tree:add(buffer(0, 1), "CentralId")
        local cnt_ip, cnt_ip_size = read_string(buffer, maxlen, cnt_ip_subtree)
        if cnt_ip == nil then
            return nil, cnt_ip_size
        end
        cnt_ip_subtree:set_len(cnt_ip_size)
        buffer = buffer_offset(buffer, cnt_ip_size)
        maxlen = maxlen - cnt_ip_size

        local ips_subtree = tree:add(buffer(0, 1), "CentralsIps")
        local ips, ips_size = read_array(buffer, maxlen, ips_subtree, read_string)
        ips_subtree:set_len(ips_size)

        if ips == nil then
            return nil, ips_size
        end

        return ips_size + 2 + cnt_ip_size
    end,
    [BLOCK_TYPES.CNT_SENSOR_REQUEST_CONFIG] = function(buffer, maxlen, tree)
        if maxlen < 2+4 then
            return nil, 2+4
        end

        local port_id = buffer(0, 2)
        local sensor_id = buffer(2, 4)

        tree:add(port_id, "PortId: " .. port_id:int())
        tree:add(sensor_id, "SensorId: " .. sensor_id:int())
        return 2+4
    end
}

function czujnix_proto.dissector(buffer, pinfo, tree)
    pinfo.cols.protocol = "CZUJNIX"

    local subtree = tree:add(czujnix_proto, buffer(), "CzuniX Protocol Data")

    local total_len = buffer:len()
    local bytes_consumed = 0

    while bytes_consumed < total_len do
        local block_type = buffer(0, 4)
        if BLOCK_NAMES[block_type:int()] == nil then
            return 0
        end
        local block_size = buffer(4, 4)

        local block_tree = subtree:add(buffer(0, 8), BLOCK_NAMES[block_type:int()])
        block_tree:add(block_size, "BlockSize: " .. block_size:int())

        bytes_consumed = bytes_consumed + 8
        buffer = buffer_offset(buffer, 8)
        local read, required = BLOCK_PARSERS[block_type:int()](buffer, total_len - bytes_consumed, block_tree)

        if read == nil then
            pinfo.desegment_offset = bytes_consumed
            pinfo.desegment_len = required
            return
        else
            bytes_consumed = bytes_consumed + read
            buffer = buffer_offset(buffer, read)
            block_tree:set_len(4 + read)
        end
    end

    return bytes_consumed
end

local tcp_table = DissectorTable.get("udp.port")
for _, v in ipairs({ ... }) do
    tcp_table:add(v, czujnix_proto)
end
