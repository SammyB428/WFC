/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2019, Samuel R. Blackburn
**
** "You can get credit for something or get it done, but not both."
** Dr. Richard Garwin
**
** BSD License follows.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** Redistributions of source code must retain the above copyright notice,
** this list of conditions and the following disclaimer. Redistributions
** in binary form must reproduce the above copyright notice, this list
** of conditions and the following disclaimer in the documentation and/or
** other materials provided with the distribution. Neither the name of
** the WFC nor the names of its contributors may be used to endorse or
** promote products derived from this software without specific prior
** written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** $Workfile: CCrypto.hpp $
** $Revision: 25 $
** $Modtime: 2/24/02 4:16p $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#if ! defined( CRYPTO_API_CLASS_HEADER )

#define CRYPTO_API_CLASS_HEADER

// 2000-10-28
// Thanks go to Danny Smith (danny_r_smith_2001@yahoo.co.nz) for adding
// the capability to use a non-Microsoft compiler (namely gcc)

#if ( _MSC_VER >= 1020 ) || defined( WFC_HAVE_WINCRYPT )

#include "wfc_crypto.h"

class CCryptography
{
   protected:

      DWORD m_ErrorCode{ 0 };

   public:

      CCryptography() noexcept;
      virtual ~CCryptography();

      inline constexpr [[nodiscard]] _Check_return_ DWORD GetErrorCode(void) const noexcept { return(m_ErrorCode); }
};

// Need forward declaration because of Catch-22...
class CCryptographicKey;

class CCryptographicHash : public CCryptography
{
   protected:

      HCRYPTHASH m_Hash{ NULL };

      bool m_AutomaticallyDestroy{ false };

   public:

       CCryptographicHash(_In_ CCryptographicHash const&) = delete;
       CCryptographicHash& operator=(_In_ CCryptographicHash const&) = delete;
       
       // Construction

      CCryptographicHash() noexcept;
      CCryptographicHash( _In_ HCRYPTHASH source_handle, _In_ bool automatically_destroy = true ) noexcept;
      virtual ~CCryptographicHash();

      static constexpr DWORD const MD2{ CALG_MD2 };
      static constexpr DWORD const MD4{ CALG_MD4 };
      static constexpr DWORD const MD5{ CALG_MD5 };
      static constexpr DWORD const SHAS{ CALG_SHA };
      static constexpr DWORD const SHA1{ CALG_SHA1 };
      static constexpr DWORD const MAC{ CALG_MAC };

      // Properties

      enum _Parameters
      {
         parameterAlgorithmIdentifier = HP_ALGID,
         parameterLength              = HP_HASHSIZE,
         parameterValue               = HP_HASHVAL,
#if defined( HP_TLS1PRF_SEED )
         parameterHMACInformation     = HP_HMAC_INFO,
         parameterTLSPRFLabel         = HP_TLS1PRF_LABEL,
         parameterTLSPRFSeed          = HP_TLS1PRF_SEED
#endif // HP_TLS1PRF_SEED
      };

      // Methods

      virtual _Check_return_ bool Destroy( void ) noexcept; // CryptDestroyHash
      virtual _Check_return_ bool FromHandle( _In_ HCRYPTHASH source_handle, _In_ bool automatically_destroy = TRUE ) noexcept;
      virtual _Check_return_ HCRYPTHASH GetHandle( void ) const noexcept;
      virtual _Check_return_ bool GetParameter( _In_ uint32_t const parameter_to_get, __inout_bcount( buffer_length ) uint8_t * buffer, __inout uint32_t& buffer_length, _In_ uint32_t const flags = 0 ) noexcept; // CryptGetKeyParam

      // GetParameter helpers

      virtual _Check_return_ bool GetAlgorithmIdentifier( _Out_ DWORD& identifier ) noexcept;
      virtual _Check_return_ bool GetLength( _Out_ uint32_t& length ) noexcept;
      virtual _Check_return_ bool GetValue( _Out_ std::vector<uint8_t>& value ) noexcept;
      virtual _Check_return_ bool Hash(_In_ std::vector<uint8_t> const& data_to_compute_a_hash_on, _In_ DWORD const flags = CRYPT_USERDATA) noexcept; // CryptHashData
      virtual _Check_return_ bool Hash( __in_bcount( buffer_size ) uint8_t const * buffer, _In_ DWORD const buffer_size, _In_ DWORD const flags = CRYPT_USERDATA ) noexcept; // CryptHashData
      virtual _Check_return_ bool Hash( _In_ CCryptographicKey const& key_to_hash, _In_ DWORD const flags = 0 ) noexcept; // CryptHashSessionKey
 
      // All of the Setxxx methods parameters cannot be properly typed because
      // the CryptSetHashParam() API is mis-typed. The buffer parameter should
      // be const but it is not. Looks like they didn't think it through enough.

      virtual _Check_return_ bool SetParameter( _In_ DWORD const parameter_to_set, _Inout_ BYTE *buffer, _In_ DWORD const flags = 0 ) noexcept; // CryptSetKeyParam

      // SetParameter helpers

      virtual _Check_return_ bool SetValue(_Inout_ std::vector<uint8_t>& value ) noexcept;
      virtual _Check_return_ bool Sign( _In_ DWORD const which_key_to_sign_with, _In_ std::wstring const& password, _Inout_ std::vector<uint8_t>& signature, _In_ DWORD const flags = 0 ) noexcept; // CryptSignHash
      virtual _Check_return_ bool VerifySignature(_Inout_ std::vector<uint8_t>& signature, _Inout_ CCryptographicKey& public_key_to_verify_with, _Inout_ std::wstring& password, _In_ DWORD const flags = 0 ) noexcept; // CryptVerifySignature
};

class CCryptographicAlgorithm
{
   public:

      // Construction

      CCryptographicAlgorithm() noexcept;
      explicit CCryptographicAlgorithm( _In_ CCryptographicAlgorithm const& source ) noexcept;
      explicit CCryptographicAlgorithm( _In_ PROV_ENUMALGS const& source ) noexcept;
      virtual ~CCryptographicAlgorithm();

      // Properties

      enum _Classes
      {
         classAny            = ALG_CLASS_ANY,
         classSignature      = ALG_CLASS_SIGNATURE,
         classMessageEncrypt = ALG_CLASS_MSG_ENCRYPT,
         classDataEncrypt    = ALG_CLASS_DATA_ENCRYPT,
         classHash           = ALG_CLASS_HASH,
         classKeyExchange    = ALG_CLASS_KEY_EXCHANGE,
         classAll            = ALG_CLASS_ALL
      };

      enum _Types
      {
         typeAny                      = ALG_TYPE_ANY,
         typeDigitalSignatureStandard = ALG_TYPE_DSS,
         typeRSA                      = ALG_TYPE_RSA,
         typeBlock                    = ALG_TYPE_BLOCK,
         typeStream                   = ALG_TYPE_STREAM,
         typeDiffieHellman            = ALG_TYPE_DH,
         typeSecureChannel            = ALG_TYPE_SECURECHANNEL
      };

      enum _RSASubIdentifiers
      {
         rsaAny             = ALG_SID_RSA_ANY,
         rsaPKCS            = ALG_SID_RSA_PKCS,
         rsaMicrosoftAtWork = ALG_SID_RSA_MSATWORK,
         rsaEntrust         = ALG_SID_RSA_ENTRUST,
         rsaPGP             = ALG_SID_RSA_PGP
      };

      enum _DSSSubIdentifiers
      {
         dssAny                    = ALG_SID_DSS_ANY,
         dssPKCS                   = ALG_SID_DSS_PKCS,
         dssDefenseMessagingSystem = ALG_SID_DSS_DMS
      };

      enum _BlockCipherSubIdentifiers
      {
         blockDigitalEncryptionStandard = ALG_SID_DES,
         blockRC2                       = ALG_SID_RC2,
         blockTripleDES                 = ALG_SID_3DES,
         blockDESX                      = ALG_SID_DESX,
         blockIDEA                      = ALG_SID_IDEA,
         blockCAST                      = ALG_SID_CAST,
         blockSaferSK64                 = ALG_SID_SAFERSK64,
         blockSaferSK128                = ALG_SID_SAFERSK128,
         block3DES112                   = ALG_SID_3DES_112,
         blockCylinkMEK                 = ALG_SID_CYLINK_MEK,
         blockRC5                       = ALG_SID_RC5,
         blockSkipJack                  = ALG_SID_SKIPJACK,
         blockTEK                       = ALG_SID_TEK
      };

      enum _StreamSubIdentifiers
      {
         streamRC4  = ALG_SID_RC4,
         streamSeal = ALG_SID_SEAL
      };

      enum _HashSubIdentifiers
      {
         hashMD2       = ALG_SID_MD2,
         hashMD4       = ALG_SID_MD4,
         hashMD5       = ALG_SID_MD5,
         hashSHA       = ALG_SID_SHA,
         hashMAC       = ALG_SID_MAC,
         hashRipeMD    = ALG_SID_RIPEMD,
         hashRipeMD160 = ALG_SID_RIPEMD160,
         hashSSLMD5    = ALG_SID_SSL3SHAMD5,
         hashTLS1PRF   = ALG_SID_TLS1PRF
      };

      DWORD Identifier{ 0 };
      DWORD BitLength{ 0 };
      std::wstring Name;

      // Methods

      virtual void Copy( _In_ CCryptographicAlgorithm const& source ) noexcept;
      virtual void Copy( _In_ PROV_ENUMALGS const& source ) noexcept;
      virtual void Copy( _In_ PROV_ENUMALGS const * source ) noexcept;
      virtual void Empty( void ) noexcept;
      virtual _Check_return_ bool IsDataEncrypt( void ) const noexcept;
      virtual _Check_return_ bool IsHash( void ) const noexcept;
      virtual _Check_return_ bool IsKeyExchange( void ) const noexcept;
      virtual _Check_return_ bool IsMessageEncrypt( void ) const noexcept;
      virtual _Check_return_ bool IsSignature( void ) const noexcept;

      // Operators

      virtual _Check_return_ CCryptographicAlgorithm& operator=( _In_ CCryptographicAlgorithm const& source ) noexcept;
      virtual _Check_return_ CCryptographicAlgorithm& operator=( _In_ PROV_ENUMALGS const& source ) noexcept;

      // Instrumentation

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

class CCryptographicKey : public CCryptography
{
   protected:

       HCRYPTKEY m_Key{ 0 };

      bool m_AutomaticallyDestroy{ false };

   public:

       CCryptographicKey(_In_ CCryptographicKey const&) = delete;
       CCryptographicKey& operator=(_In_ CCryptographicKey const&) = delete;
       
       // Construction

      CCryptographicKey() noexcept;
      CCryptographicKey( _In_ HCRYPTKEY source_handle, _In_ bool automatically_destroy = true )  noexcept;
      virtual ~CCryptographicKey();

      // Properties
      enum class Parameter : uint32_t
      {
         AlgorithmIdentifier           = KP_ALGID,
         BlockLengthInBytes            = KP_BLOCKLEN,
         Salt                          = KP_SALT,
         Permissions                   = KP_PERMISSIONS,
         InitializationVector          = KP_IV,
         PaddingMode                   = KP_PADDING,
         CipherMode                    = KP_MODE,
         NumberOfBitsProcessedPerCycle = KP_MODE_BITS,

#if defined( KP_PUB_EX_VAL )
         // New Parameters
         KeyLengthInBits                  = KP_KEYLEN,
         LengthOfSaltInBytes              = KP_SALT_EX,
         DSSDiffieHellmanPValue           = KP_P,
         DSSDiffieHellmanGValue           = KP_G,
         DSSQValue                        = KP_Q,
         DiffieHellmanXValue              = KP_X,
         YValue                           = KP_Y,
         FortezzaRAValue                  = KP_RA,
         FortezzaRBValue                  = KP_RB,
         RSAEnvelopeInformation           = KP_INFO,
         RC2EffectiveKeyLength            = KP_EFFECTIVE_KEYLEN,
         SecureChannelAlgorithm           = KP_SCHANNEL_ALG,
         SecureChannelClientRandomData    = KP_CLIENT_RANDOM,
         SecureChannelServerRandomData    = KP_SERVER_RANDOM,
         RP                               = KP_RP,
         PrecompiledMD5                   = KP_PRECOMP_MD5,
         PrecmopiledSHA                   = KP_PRECOMP_SHA,
         SecureChannelCertificateDataPCT1 = KP_CERTIFICATE,
         SecureChannelClearKeyDataPCT1    = KP_CLEAR_KEY,
         PubExLength                      = KP_PUB_EX_LEN,
         PubExValue                       = KP_PUB_EX_VAL,
#endif // KP_PUB_EX_VAL

#if defined( KP_HIGHEST_VERSION ) // New for 52
         KeyVal                           = KP_KEYVAL,
         AdministratorPin                 = KP_ADMIN_PIN,
         KeyExchangePin                   = KP_KEYEXCHANGE_PIN,
         SignaturePin                     = KP_SIGNATURE_PIN,
         PreHash                          = KP_PREHASH,
         OptimalAsymmetricEncryptionPadding = KP_OAEP_PARAMS,
         CryptographicMessageSyntaxKeyInformation = KP_CMS_KEY_INFO,
         CryptographicMessageSyntaxDiffieHellmanKeyInformation = KP_CMS_DH_KEY_INFO,
         Public                           = KP_PUB_PARAMS,
         Verify                           = KP_VERIFY_PARAMS,
         HighestVersion                   = KP_HIGHEST_VERSION,
#endif // KP_HIGHEST_VERSION
      };

      enum class CipherMode : uint32_t
      {
         ElectronicCodebook                             = CRYPT_MODE_ECB,
         CipherBlockChaining                            = CRYPT_MODE_CBC,
         OutputFeedback                                 = CRYPT_MODE_OFB,
         CipherFeedback                                 = CRYPT_MODE_CFB,
         CiphertextStealing                             = CRYPT_MODE_CTS,
         AnsiCipherBlockChaningInterleaved              = CRYPT_MODE_CBCI,
         AnsiCipherFeedbackPipelined                    = CRYPT_MODE_CFBP,
         AnsiOutputFeedbackPipelined                    = CRYPT_MODE_OFBP,
         AnsiCipherBlockChainingAndOFMasking            = CRYPT_MODE_CBCOFM,
         AnsiCipherBlockChainingAndOFMaskingInterleaved = CRYPT_MODE_CBCOFMI
      };

      enum _Permissions
      {
         permissionEncrypt = CRYPT_ENCRYPT,
         permissionDecrypt = CRYPT_DECRYPT,
         permissionExport  = CRYPT_EXPORT,
         permissionRead    = CRYPT_READ,
         permissionWrite   = CRYPT_WRITE,
         permissionMAC     = CRYPT_MAC,
#if defined( CRYPT_EXPORT_KEY )
         permissionExportKey = CRYPT_EXPORT_KEY,
         permissionImportKey = CRYPT_IMPORT_KEY,
#endif // CRYPT_EXPORT_KEY
#if defined( CRYPT_ARCHIVE )
         permissionArchiveKey = CRYPT_ARCHIVE,
#endif // CRYPT_ARCHIVE
      };

      enum class KeyFormat : uint32_t
      {
         Unused        = 0, // For unencrypted key blobs like public keys
         Simple        = SIMPLEBLOB,
         PublicKey     = PUBLICKEYBLOB,
         PrivateKey    = PRIVATEKEYBLOB,
         PlainText     = PLAINTEXTKEYBLOB,
         Opaque        = OPAQUEKEYBLOB,
         PublicKeyEx   = PUBLICKEYBLOBEX,
         SymmetricWrap = SYMMETRICWRAPKEYBLOB,
         KeyState      = 0xC, // KEYSTATEBLOB
      };

      // Methods

      virtual _Check_return_ bool Decrypt(_In_ std::vector<uint8_t> const& data_to_decrypt,
                                  _Out_ std::vector<uint8_t>&             decrypted_data,
                                  _In_ bool const               this_is_the_last_chunk_of_data_to_be_decrypted = true,
                                  __in_opt CCryptographicHash * hash_p = nullptr,
                                  _In_ DWORD const              flags  = 0,
                                  _In_ bool const              use_wfc_trim = true ) noexcept; // CryptDecrypt

      virtual _Check_return_ bool Destroy( void ) noexcept; // CryptDestroyKey

      virtual _Check_return_ bool Encrypt( std::vector<uint8_t> const&          data_to_encrypt,
          std::vector<uint8_t>&          encrypted_data,
                                  bool                 this_is_the_last_chunk_of_data_to_be_encrypted = true,
                                  CCryptographicHash * hash_p = nullptr,
                                  DWORD                flags  = 0,
                                  bool                 encode_size = true ) noexcept; // CryptEncrypt

      virtual _Check_return_ bool Export( CCryptographicKey& key_for_whoever_we_are_exporting_to,
          std::vector<uint8_t>& key_in_exported_form,
          KeyFormat export_format = KeyFormat::Simple,
                           DWORD       flags         = 0 ) noexcept;
      virtual _Check_return_ bool FromHandle( _In_ HCRYPTKEY source_handle, _In_ bool automatically_destroy = TRUE ) noexcept;
      virtual _Check_return_ HCRYPTKEY GetHandle( void ) const noexcept;
      virtual _Check_return_ bool GetParameter( _In_ Parameter const parameter_to_get, __inout_bcount( buffer_length ) uint8_t * buffer, __inout DWORD& buffer_length, _In_ DWORD const flags = 0 ) noexcept; // CryptGetKeyParam

      // Helpers for GetParameter

      virtual _Check_return_ bool GetAlgorithmIdentifier( _Out_ DWORD& identifier ) noexcept;
      virtual _Check_return_ bool GetBlockLength( _Out_ DWORD& block_length ) noexcept;
      virtual _Check_return_ bool GetCipherMode( _Out_ DWORD& cipher_mode ) noexcept;
      virtual _Check_return_ bool GetInitializationVector( _Out_ std::vector<uint8_t>& initialization_vector ) noexcept;
      virtual _Check_return_ bool GetNumberOfBitsProcessedPerCycle( _Out_ DWORD& number_of_bits ) noexcept;
      virtual _Check_return_ bool GetPaddingMode( _Out_ DWORD& padding_mode ) noexcept;
      virtual _Check_return_ bool GetPermissions( _Out_ DWORD& permissions ) noexcept;
      virtual _Check_return_ bool GetSalt( _Out_ std::vector<uint8_t>& salt ) noexcept;

      // All of the Setxxx methods parameters cannot be properly typed because
      // the CryptSetKeyParam() API is mis-typed. The buffer parameter should
      // be const but it is not. Looks like they didn't think it through enough.

      virtual _Check_return_ bool SetParameter( _In_ Parameter const parameter_to_set, _In_ uint8_t const * buffer, _In_ DWORD const flags = 0 ) noexcept; // CryptSetKeyParam

      // Helpers for SetParameter
      virtual _Check_return_ bool SetPermissions( _In_ DWORD const permissions ) noexcept;
      virtual _Check_return_ bool SetSalt( _In_ uint8_t const * buffer ) noexcept;
      virtual _Check_return_ bool SetInitializationVector( _In_ BYTE const * buffer ) noexcept;
      virtual _Check_return_ bool SetPaddingMode( _In_ DWORD const mode ) noexcept;
      virtual _Check_return_ bool SetCipherMode( _In_ CipherMode const mode ) noexcept;
      virtual _Check_return_ bool SetNumberOfBitsProcessedPerCycle( _In_ DWORD const number_of_bits ) noexcept;
};

// 2002-09-30
// Microsoft got rid of these definitions in the July 2002 Platform SDK, so much for not breaking existing code...

#if ! defined( PROV_STT_MER )
#define PROV_STT_MER   (7)
#define PROV_STT_ACQ   (8)
#define PROV_STT_BRND  (9)
#define PROV_STT_ROOT (10)
#define PROV_STT_ISS  (11)
#endif

class CCryptographicProvider : public CCryptography
{
   protected:

       HCRYPTPROV m_CryptographicProvider{ 0 };

      void m_Initialize( void ) noexcept;

   public:

       CCryptographicProvider(_In_ CCryptographicProvider const&) = delete;
       CCryptographicProvider& operator=(_In_ CCryptographicProvider const&) = delete;
 
       // Construction

      CCryptographicProvider() noexcept;
      virtual ~CCryptographicProvider();

      // Properties

      enum _Providers
      {
         RsaFull                    = PROV_RSA_FULL,
         RsaSignature               = PROV_RSA_SIG,
         DigitalSignatureStandard   = PROV_DSS,
         Clipper                    = PROV_FORTEZZA,
         MicrosoftMail              = PROV_MS_MAIL,
         STTMer                     = PROV_STT_MER,
         STTAcq                     = PROV_STT_ACQ,
         STTBrnd                    = PROV_STT_BRND,
         STTRoot                    = PROV_STT_ROOT,
         STTIss                     = PROV_STT_ISS,
         SecureSocketsLayer         = PROV_SSL,
         RSASChannel                = PROV_RSA_SCHANNEL,
         DSSDiffieHellman           = PROV_DSS_DH,
         ECDSASignature             = PROV_EC_ECDSA_SIG,
         ECNRASignature             = PROV_EC_ECNRA_SIG,
         ECDSAFull                  = PROV_EC_ECDSA_FULL,
         ECNRAFull                  = PROV_EC_ECNRA_FULL,
         SpyrusLynks                = PROV_SPYRUS_LYNKS,
         DiffieHellmanSecureChannel = PROV_DH_SCHANNEL,
         RandomNumberGenerator      = PROV_RNG,
         IntelSecurity              = PROV_INTEL_SEC
      };

      enum _Parameters
      {
         parameterEnumerateAlgorithms      = PP_ENUMALGS,
         parameterEnumerateContainers      = PP_ENUMCONTAINERS,
         parameterImplementationType       = PP_IMPTYPE,
         parameterName                     = PP_NAME,
         parameterVersion                  = PP_VERSION,
         parameterCurrentKeyContainerName  = PP_CONTAINER,
         parameterClientWindowHandle       = PP_CLIENT_HWND,

#if defined( PP_ENUMALGS_EX )
         parameterChangePassword           = PP_CHANGE_PASSWORD,
         parameterKeysetSecurityDescriptor = PP_KEYSET_SEC_DESCR,
         parameterCertificateChain         = PP_CERTCHAIN,
         parameterKeyTypeSubType           = PP_KEY_TYPE_SUBTYPE,
         parameterProviderType             = PP_KEY_TYPE_SUBTYPE,
         parameterKeyStorage               = PP_KEYSTORAGE,
         parameterApplicationCertificate   = PP_APPLI_CERT,
         parameterSymmetricKeySize         = PP_SYM_KEYSIZE,
         parameterSessionKeySize           = PP_SESSION_KEYSIZE,
         parameterUserInterfacePrompt      = PP_UI_PROMPT,
         parameterEnumerateAlgorithmsEx    = PP_ENUMALGS_EX,
         parameterContextInformation       = PP_CONTEXT_INFO,
         parameterKeyExchangeKeySize       = PP_KEYEXCHANGE_KEYSIZE,
         parameterSignatureKeySize         = PP_SIGNATURE_KEYSIZE,
         parameterKeyExchangeAlgorithm     = PP_KEYEXCHANGE_ALG,
         parameterSignatureAlgorithm       = PP_SIGNATURE_ALG,
         parameterDeleteKey                = PP_DELETEKEY,
#endif // PP_ENUMALGS_EX
         parameterEnumerateMandatoryRoots  = PP_ENUMMANDROOTS,
         parameterEnumerateElectedRoots    = PP_ENUMELECTROOTS,
         parameterKeySetType               = PP_KEYSET_TYPE,
         parameterAdministratorPersonalIdentificationNumber = PP_ADMIN_PIN,
         parameterKeyExchangePersonalIdentificationNumber = PP_KEYEXCHANGE_PIN,
         parameterSignaturePersonalIdentificationNumber = PP_SIGNATURE_PIN,
         parameterSignatureKeySizeIncrement   = PP_SIG_KEYSIZE_INC,
         parameterKeyExchangeKeySizeIncrement = PP_KEYX_KEYSIZE_INC,
         parameterUniqueContainer             = PP_UNIQUE_CONTAINER,
         parameterSGCInformation              = PP_SGC_INFO,
         parameterUseHardwareRandomNumberGenerator = PP_USE_HARDWARE_RNG,
         parameterKeySpecification                 = PP_KEYSPEC,
         parameterEnumerateSigningProtection       = PP_ENUMEX_SIGNING_PROT
      };

      enum _Flags
      {
         VerifyContext = CRYPT_VERIFYCONTEXT,
         NewKeySet     = CRYPT_NEWKEYSET,
         DeleteKeySet  = CRYPT_DELETEKEYSET,
         MachineKeySet = CRYPT_MACHINE_KEYSET,
         Silent        = CRYPT_SILENT
      };

      enum _KeyFlags
      {
         keyflagExportable     = CRYPT_EXPORTABLE,
         keyflagCreateSalt     = CRYPT_CREATE_SALT,
         keyflagUserProtected  = CRYPT_USER_PROTECTED,
         keyflagCryptUpdateKey = CRYPT_UPDATE_KEY,
         keyflagNoSalt         = CRYPT_NO_SALT,
         keyflagPreGenerated   = CRYPT_PREGEN,
         keyflagRecipient      = CRYPT_RECIPIENT,
         keyflagInitiator      = CRYPT_INITIATOR,
         keyflagOnLine         = CRYPT_ONLINE,
         keyflagSF             = CRYPT_SF,
         keyflagCreateInitialVector = CRYPT_CREATE_IV,
         keyflagKeyExchangeKey = CRYPT_KEK,
         keyflagDataKey        = CRYPT_DATA_KEY,
         keyflagVolatile       = CRYPT_VOLATILE,
         keyflagSGCKey         = CRYPT_SGCKEY
      };

      // Methods
      
      virtual _Check_return_ bool  Open( __in_z_opt LPCTSTR container_name = nullptr,
          __in_z_opt LPCTSTR provider_name  = nullptr,
                          _In_ DWORD   provider_type  = RsaFull,
                          _In_ DWORD   flags          = 0 ) noexcept; // CryptAcquireContext
      virtual void  Close( void ) noexcept; // CryptReleaseContext
      virtual bool  CreateHash( CCryptographicAlgorithm const& which_algorithm_you_want_the_key_for,
                                CCryptographicKey const&       key_for_algorithms_that_need_it,
                                      CCryptographicHash&      destination_hash,
                                      DWORD                    flags = 0 ) noexcept;
      virtual bool  CreateKey( CCryptographicAlgorithm const& which_algorithm_you_want_the_key_for,
                                     CCryptographicKey&       destination_key,
                                     DWORD                    flags = CRYPT_EXPORTABLE,
                                     WORD                     number_of_bits_in_key = 0 ) noexcept; // CryptGenKey
      virtual bool  CreateKeySet( LPCTSTR container_name = nullptr, LPCTSTR provider_name = nullptr, DWORD provider_type = RsaFull ) noexcept;

      virtual bool  ImportKey(std::vector<uint8_t>& exported_key_data,
                               CCryptographicKey& key,
          CCryptographicKey::KeyFormat format = CCryptographicKey::KeyFormat::Simple,
                               DWORD       flags  = 0 ) noexcept; // CryptImportKey

      virtual bool  DeriveKey( CCryptographicAlgorithm const& algorithm,
                               CCryptographicHash const&      hash,
                                     CCryptographicKey&       key,
                                     DWORD                    flags = keyflagExportable ) noexcept;
      virtual void  EnumerateAlgorithms( DWORD& enumerator ) noexcept;
      virtual bool  GenerateRandomBytes(std::vector<uint8_t>& bytes, DWORD number_of_bytes = 0 ) noexcept; // CryptGenRandom
      virtual bool  GetNext( DWORD& enumerator, CCryptographicAlgorithm& algorithm ) noexcept;
      virtual bool  GetParameter( DWORD const what_to_get, std::vector<uint8_t>& buffer, DWORD const flags ) noexcept; // CryptGetProvParam
      virtual bool  GetUserKey( DWORD which_user_key_to_get, CCryptographicKey& key ) noexcept;
      virtual bool  SetDefault( std::wstring const& name, DWORD type ) noexcept; // CryptSetProvider
      virtual HCRYPTPROV GetHandle( void ) const noexcept;

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

      virtual void Dump( CDumpContext& dump_context ) const;

#endif // _DEBUG
};

#endif // _MSC_VER

#endif // CRYPTO_API_CLASS_HEADER
