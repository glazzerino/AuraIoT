#pragma once
extern const char b64_alphabet[];

int base64_encode(char *output, char *input, int inputLen);

int base64_decode(char *output, char *input, int inputLen);


int base64_enc_len(int inputLen);

int base64_dec_len(char *input, int inputLen);
/**
 * const char* ssid = "IZZI-9135";
const char* password = "chapie123";

String FIREBASE_HOST = "aura-iot-c6bc8.firebaseio.com";
String FIREBASE_AUTH = "nUIJvAcFmcmfJEPpwlFXdTwmLjRjPtFUBbzFfLfk";
*/