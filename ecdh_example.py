from tinyec import registry
import secrets

# Get x coordinate from point
def compress(pubKey):
    return pubKey.x

# Instantiate NIST P-521 curve
curve = registry.get_curve('secp521r1')

# Step 1 ---  Alice Generates private and public key
alicePrivKey = secrets.randbelow(curve.field.n)
alicePubKey = alicePrivKey * curve.g

print("Alice public key:", compress(alicePubKey))

# Step 2 ---  Bob Generates private and public key
bobPrivKey = secrets.randbelow(curve.field.n)
bobPubKey = bobPrivKey * curve.g
print("Bob public key:", compress(bobPubKey))

# Step 3 ----- Alice and Bob exchange public key s----------------


# Step 4 --- Compute shared key ---------
aliceSharedKey = alicePrivKey * bobPubKey
print("Alice shared key:", compress(aliceSharedKey))

bobSharedKey = bobPrivKey * alicePubKey
print("Bob shared key:", compress(bobSharedKey))

print("Equal shared keys:", aliceSharedKey == bobSharedKey)