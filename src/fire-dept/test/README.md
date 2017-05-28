# UDP Test

## To run the test:

1. Run `fire-dept` executable. This will listen on `localhost:4099` for messages.
2. From the project root dir run `python src/fire-dept/test/udp_test.py`.

## Expected output:

The executable will log recieved messages from both ipv4 and ipv6 calls.
```
DebugBlock [u8: 0x69, i64: 0x1122334455667788, str: ipv4]
DebugBlock [u8: 0x96, i64: 0x8877665544332211, str: ipv6]
```

It should also complain with a warning about an unknown block type.