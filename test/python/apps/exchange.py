from base64 import urlsafe_b64encode
from contextlib import contextmanager
from typing import Generator, Optional, Dict, Callable, Iterable
from enum import IntEnum, Enum, auto
from dataclasses import dataclass

from ragger.backend.interface import BackendInterface, RAPDU
from ragger.error import ExceptionRAPDU

from ..common import LEDGER_TEST_PRIVATE_KEY

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric.utils import decode_dss_signature

from .ethereum import ETH_PACKED_DERIVATION_PATH, ETH_CONF, ETH_CONF_DER_SIGNATURE
from .litecoin import LTC_PACKED_DERIVATION_PATH, LTC_CONF, LTC_CONF_DER_SIGNATURE
from .bitcoin import BTC_PACKED_DERIVATION_PATH, BTC_CONF, BTC_CONF_DER_SIGNATURE
from .exchange_subcommands import SWAP_SPECS, SELL_SPECS, FUND_SPECS
from .pb.exchange_pb2 import NewFundResponse, NewSellResponse, NewTransactionResponse
from ..utils import concatenate


class Command(IntEnum):
    GET_VERSION                  = 0x02
    START_NEW_TRANSACTION        = 0x03
    SET_PARTNER_KEY              = 0x04
    CHECK_PARTNER                = 0x05
    PROCESS_TRANSACTION_RESPONSE = 0x06
    CHECK_TRANSACTION_SIGNATURE  = 0x07
    CHECK_PAYOUT_ADDRESS         = 0x08
    CHECK_REFUND_ADDRESS         = 0x09
    START_SIGNING_TRANSACTION    = 0x0A


class Rate(IntEnum):
    FIXED    = 0x00
    FLOATING = 0x01


class SubCommand(IntEnum):
    SWAP = 0x00
    SELL = 0x01
    FUND = 0x02



ERRORS = (
    ExceptionRAPDU(0x6A80, "INCORRECT_COMMAND_DATA"),
    ExceptionRAPDU(0x6A81, "DESERIALIZATION_FAILED"),
    ExceptionRAPDU(0x6A82, "WRONG_TRANSACTION_ID"),
    ExceptionRAPDU(0x6A83, "INVALID_ADDRESS"),
    ExceptionRAPDU(0x6A84, "USER_REFUSED"),
    ExceptionRAPDU(0x6A85, "INTERNAL_ERROR"),
    ExceptionRAPDU(0x6A86, "WRONG_P1"),
    ExceptionRAPDU(0x6A87, "WRONG_P2"),
    ExceptionRAPDU(0x6E00, "CLASS_NOT_SUPPORTED"),
    ExceptionRAPDU(0x6D00, "INVALID_INSTRUCTION"),
    ExceptionRAPDU(0x9D1A, "SIGN_VERIFICATION_FAIL")
)

@dataclass
class Identity:
    partner_privkey: ec.EllipticCurvePrivateKey
    partner_credentials: bytes
    partner_signed_credentials: bytes

def generate_partner_keys(curve, partner_name: bytes = b"Default_Partner") -> Identity:
    partner_privkey = ec.generate_private_key(curve, backend=default_backend())
    partner_pubkey = partner_privkey.public_key().public_bytes(
        encoding=serialization.Encoding.X962,
        format=serialization.PublicFormat.UncompressedPoint
    )
    device_number = int.from_bytes(LEDGER_TEST_PRIVATE_KEY, "big")
    device_privkey = ec.derive_private_key(private_value=device_number, curve=ec.SECP256K1(), backend=default_backend())
    partner_credentials = len(partner_name).to_bytes(1, "big") + partner_name + partner_pubkey
    partner_signed_credentials = device_privkey.sign(partner_credentials, ec.ECDSA(hashes.SHA256()))

    return Identity(partner_privkey, partner_credentials, partner_signed_credentials)



class ExchangeClient:
    CLA = 0xE0
    def __init__(self,
                 client: BackendInterface,
                 rate: Rate,
                 subcommand: SubCommand):
        if not isinstance(client, BackendInterface):
            raise TypeError('client must be an instance of BackendInterface')
        if not isinstance(rate, Rate):
            raise TypeError('rate must be an instance of Rate')
        if not isinstance(subcommand, SubCommand):
            raise TypeError('subcommand must be an instance of SubCommand')

        self._client = client
        self._rate = rate
        self._subcommand = subcommand
        self._transaction_id: Optional[bytes] = None
        self._transaction: bytes = b""
        self._payout_currency: str = ""
        self._refund_currency: Optional[str] = None

        if self._subcommand == SubCommand.SWAP:
            self.subcommand_specs = SWAP_SPECS
        elif self._subcommand == SubCommand.SELL:
            self.subcommand_specs = SELL_SPECS
        elif self._subcommand == SubCommand.FUND:
            self.subcommand_specs = FUND_SPECS

        self._partner_identity = generate_partner_keys(self.subcommand_specs.curve)



    @property
    def rate(self) -> Rate:
        return self._rate

    @property
    def subcommand(self) -> SubCommand:
        return self._subcommand

    @property
    def transaction_id(self) -> bytes:
        return self._transaction_id or b""

    def _exchange(self, ins: int, payload: bytes = b"") -> RAPDU:
        return self._client.exchange(self.CLA, ins, p1=self.rate,
                                     p2=self.subcommand, data=payload)

    @contextmanager
    def _exchange_async(self, ins: int, payload: bytes = b"") -> Generator[RAPDU, None, None]:
        with self._client.exchange_async(self.CLA, ins, p1=self.rate,
                                         p2=self.subcommand, data=payload) as response:
            yield response

    def get_version(self) -> RAPDU:
        return self._exchange(Command.GET_VERSION)

    def init_transaction(self) -> RAPDU:
        response = self._exchange(Command.START_NEW_TRANSACTION)
        self._transaction_id = response.data
        return response

    def set_partner_key(self) -> RAPDU:
        return self._exchange(Command.SET_PARTNER_KEY, self._partner_identity.partner_credentials)

    def check_partner_key(self) -> RAPDU:
        return self._exchange(Command.CHECK_PARTNER, self._partner_identity.partner_signed_credentials)

    def process_transaction(self, conf: Dict, fees: bytes) -> RAPDU:
        assert self.subcommand_specs.check_conf(conf)

        self._transaction = self.subcommand_specs.create_transaction(conf, self.transaction_id)

        self._payout_currency = conf[self.subcommand_specs.payout_field]
        if self.subcommand_specs.refund_field:
            self._refund_currency = conf[self.subcommand_specs.refund_field]

        payload = concatenate(self._transaction, fees)
        return self._exchange(Command.PROCESS_TRANSACTION_RESPONSE, payload=payload)

    def check_transaction(self) -> RAPDU:
        # preparing payload to sign
        payload_to_sign = self.subcommand_specs.compute_payload(self._transaction)

        signature = self.subcommand_specs.encode_signature(self._partner_identity.partner_privkey, payload_to_sign)

        return self._exchange(Command.CHECK_TRANSACTION_SIGNATURE, payload=signature)

    def _ticker_to_coin_payload(self, ticker) -> bytes:
        ticker_to_conf = {
            "ETH": (ETH_CONF, ETH_CONF_DER_SIGNATURE, ETH_PACKED_DERIVATION_PATH),
            "BTC": (BTC_CONF, BTC_CONF_DER_SIGNATURE, BTC_PACKED_DERIVATION_PATH),
            "LTC": (LTC_CONF, LTC_CONF_DER_SIGNATURE, LTC_PACKED_DERIVATION_PATH),
        }
        assert ticker in ticker_to_conf
        conf, signature, derivation_path = ticker_to_conf[ticker]
        return concatenate(conf) + signature + concatenate(derivation_path)

    def check_address(self, right_clicks: int, accept: bool = True) -> None:
        command = Command.CHECK_PAYOUT_ADDRESS
        payload = self._ticker_to_coin_payload(self._payout_currency)

        if self._refund_currency:
            # If refund adress has to be checked, send CHECk_PAYOUT_ADDRESS first
            self._exchange(command, payload=payload)
            command = Command.CHECK_REFUND_ADDRESS
            payload = self._ticker_to_coin_payload(self._refund_currency)

        with self._exchange_async(command, payload=payload):
            for _ in range(right_clicks):
                self._client.right_click()
            if not accept:
                self._client.right_click()
            self._client.both_click()

    def start_signing_transaction(self) -> RAPDU:
        self._exchange(Command.START_SIGNING_TRANSACTION)
