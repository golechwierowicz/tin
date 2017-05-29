# UDP Test

## To run the test:

1. Run `fire-dept` executable. This will listen on `localhost:4099` for messages.
2. From the project root dir run `python src/fire-dept/test/udp_test.py`.

## Expected output:

The executable will log recieved messages.
```
CentralServerHeartbeat [id=123]
CentralServerFireAlert [timestamp=1; latitude=2; longitude=3; alerts_count=4]
```

It should also complain with a warning about an unknown block type.