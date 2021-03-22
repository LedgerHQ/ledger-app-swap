/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.8 at Mon Apr 12 20:54:12 2021. */

#include "protocol.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t ledger_swap_NewTransactionResponse_fields[12] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, ledger_swap_NewTransactionResponse, payin_address, payin_address, 0),
    PB_FIELD(  2, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewTransactionResponse, payin_extra_id, payin_address, 0),
    PB_FIELD(  3, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewTransactionResponse, refund_address, payin_extra_id, 0),
    PB_FIELD(  4, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewTransactionResponse, refund_extra_id, refund_address, 0),
    PB_FIELD(  5, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewTransactionResponse, payout_address, refund_extra_id, 0),
    PB_FIELD(  6, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewTransactionResponse, payout_extra_id, payout_address, 0),
    PB_FIELD(  7, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewTransactionResponse, currency_from, payout_extra_id, 0),
    PB_FIELD(  8, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewTransactionResponse, currency_to, currency_from, 0),
    PB_FIELD(  9, BYTES   , SINGULAR, STATIC  , OTHER, ledger_swap_NewTransactionResponse, amount_to_provider, currency_to, 0),
    PB_FIELD( 10, BYTES   , SINGULAR, STATIC  , OTHER, ledger_swap_NewTransactionResponse, amount_to_wallet, amount_to_provider, 0),
    PB_FIELD( 11, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewTransactionResponse, device_transaction_id, amount_to_wallet, 0),
    PB_LAST_FIELD
};

const pb_field_t ledger_swap_UDecimal_fields[3] = {
    PB_FIELD(  1, BYTES   , SINGULAR, STATIC  , FIRST, ledger_swap_UDecimal, coefficient, coefficient, 0),
    PB_FIELD(  2, UINT32  , SINGULAR, STATIC  , OTHER, ledger_swap_UDecimal, exponent, coefficient, 0),
    PB_LAST_FIELD
};

const pb_field_t ledger_swap_NewSellResponse_fields[8] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, ledger_swap_NewSellResponse, trader_email, trader_email, 0),
    PB_FIELD(  2, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewSellResponse, in_currency, trader_email, 0),
    PB_FIELD(  3, BYTES   , SINGULAR, STATIC  , OTHER, ledger_swap_NewSellResponse, in_amount, in_currency, 0),
    PB_FIELD(  4, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewSellResponse, in_address, in_amount, 0),
    PB_FIELD(  5, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewSellResponse, out_currency, in_address, 0),
    PB_FIELD(  6, MESSAGE , SINGULAR, STATIC  , OTHER, ledger_swap_NewSellResponse, out_amount, out_currency, &ledger_swap_UDecimal_fields),
    PB_FIELD(  7, BYTES   , SINGULAR, STATIC  , OTHER, ledger_swap_NewSellResponse, device_transaction_id, out_amount, 0),
    PB_LAST_FIELD
};

const pb_field_t ledger_swap_NewFundResponse_fields[6] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, ledger_swap_NewFundResponse, account_name, account_name, 0),
    PB_FIELD(  2, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewFundResponse, in_currency, account_name, 0),
    PB_FIELD(  3, BYTES   , SINGULAR, STATIC  , OTHER, ledger_swap_NewFundResponse, in_amount, in_currency, 0),
    PB_FIELD(  4, STRING  , SINGULAR, STATIC  , OTHER, ledger_swap_NewFundResponse, in_address, in_amount, 0),
    PB_FIELD(  5, BYTES   , SINGULAR, STATIC  , OTHER, ledger_swap_NewFundResponse, device_transaction_id, in_address, 0),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 *
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(ledger_swap_NewSellResponse, out_amount) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_ledger_swap_NewTransactionResponse_ledger_swap_UDecimal_ledger_swap_NewSellResponse_ledger_swap_NewFundResponse)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 *
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(ledger_swap_NewSellResponse, out_amount) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_ledger_swap_NewTransactionResponse_ledger_swap_UDecimal_ledger_swap_NewSellResponse_ledger_swap_NewFundResponse)
#endif


/* @@protoc_insertion_point(eof) */
