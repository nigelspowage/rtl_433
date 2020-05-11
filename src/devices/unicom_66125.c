/* Doorbell implementation for Unicom 66125 devices
 *
 */

#include "decoder.h"

static int unicom_66125_callback(r_device *decoder, bitbuffer_t *bitbuffer)
{
    data_t *data;
    uint8_t *b;
    char id_str[4*2+1];

    // 25 bits expected, 5 minimum packet repetitions (14 expected)
    int row = bitbuffer_find_repeated_row(bitbuffer, 5, 25);

    if (row < 0 || bitbuffer->bits_per_row[row] != 25)
        return 0;

    b = bitbuffer->bb[row];

    // 24 bits, trailing bit is dropped
    sprintf(id_str, "%02x%02x%02x", b[0], b[1], b[2]);

    data = data_make(
            "model",    "",        DATA_STRING, "Uni-com 66125 (Aldi Home Protect)",
            "id",       "ID",      DATA_STRING, id_str,
            NULL);

    decoder_output_data(decoder, data);

    return 1;

}

static char *output_fields[] = {
    "model",
    "id",
    NULL
};

r_device unicom_66125 = {
    .name           = "Uni-com 66125 Doorbell",
    .modulation     = OOK_PULSE_PWM,
    .short_width    = 232,
    .long_width     = 636,
    .gap_limit      = 1560,
    .reset_limit    = 6172,
    .decode_fn      = &unicom_66125_callback,
    .disabled       = 0,
    .fields         = output_fields
};
