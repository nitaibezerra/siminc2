<?php
/**
 * Zend Framework
 *
 * LICENSE
 *
 * This source file is subject to the new BSD license that is bundled
 * with this package in the file LICENSE.txt.
 * It is also available through the world-wide-web at this URL:
 * http://framework.zend.com/license/new-bsd
 * If you did not receive a copy of the license and are unable to
 * obtain it through the world-wide-web, please send an email
 * to license@zend.com so we can send you a copy immediately.
 *
 * @category   Zend
 * @package    Zend_Validate
 * @copyright  Copyright (c) 2005-2012 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 * @version    $Id: Abstract.php 25105 2012-11-07 20:33:22Z rob $
 */

/**
 * @see Zend_Validate_Interface
 */
require_once APPRAIZ . 'www/gerador/Zend/Zend/Validate/Interface.php';

/**
 * @category   Zend
 * @package    Zend_Validate
 * @copyright  Copyright (c) 2005-2012 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 */
abstract class Zend_Validate_Abstract implements Zend_Validate_Interface
{
    /**
     * The value to be validated
     *
     * @var mixed
     */
    protected $_value;

    /**
     * Additional variables available for validation failure messages
     *
     * @var array
     */
    protected $_messageVariables = array();

    /**
     * Validation failure message template definitions
     *
     * @var array
     */
    protected $_messageTemplates = array();

    /**
     * Array of validation failure messages
     *
     * @var array
     */
    protected $_messages = array();

    /**
     * Flag indidcating whether or not value should be obfuscated in error
     * messages
     * @var bool
     */
    protected $_obscureValue = false;

    /**
     * Array of validation failure message codes
     *
     * @var array
     * @deprecated Since 1.5.0
     */
    protected $_errors = array();

    /**
     * Translation object
     * @var Zend_Translate
     */
    protected $_translator;

    /**
     * Default translation object for all validate objects
     * @var Zend_Translate
     */
    protected static $_defaultTranslator;

    /**
     * Is translation disabled?
     * @var Boolean
     */
    protected $_translatorDisabled = false;

    /**
     * Limits the maximum returned length of a error message
     *
     * @var Integer
     */
    protected static $_messageLength = -1;

    /**
     * Returns array of validation failure messages
     *
     * @return array
     */
    public function getMessages()
    {
        return $this->_messages;
    }

    /**
     * Returns an array of the names of variables that are used in constructing validation failure messages
     *
     * @return array
     */
    public function getMessageVariables()
    {
        return array_keys($this->_messageVariables);
    }

    /**
     * Returns the message templates from the validator
     *
     * @return array
     */
    public function getMessageTemplates()
    {
        return $this->_messageTemplates;
    }

    /**
     * Sets the validation failure message template for a particular key
     *
     * @param  string $messageString
     * @param  string $messageKey     OPTIONAL
     * @return Zend_Validate_Abstract Provides a fluent interface
     * @throws Zend_Validate_Exception
     */
    public function setMessage($messageString, $messageKey = null)
    {
        if ($messageKey === null) {
            $keys = array_keys($this->_messageTemplates);
            foreach($keys as $key) {
                $this->setMessage($messageString, $key);
            }
            return $this;
        }

        if (!isset($this->_messageTemplates[$messageKey])) {
            require_once APPRAIZ . 'www/gerador/Zend/Zend/Validate/Exception.php';
            throw new Zend_Validate_Exception("No message template exists for key '$messageKey'");
        }

        $this->_messageTemplates[$messageKey] = $messageString;
        return $this;
    }

    /**
     * Sets validation failure message templates given as an array, where the array keys are the message keys,
     * and the array values are the message template strings.
     *
     * @param  array $messages
     * @return Zend_Validate_Abstract
     */
    public function setMessages(array $messages)
    {
        foreach ($messages as $key => $message) {
            $this->setMessage($message, $key);
        }
        return $this;
    }

    /**
     * Magic function returns the value of the requested property, if and only if it is the value or a
     * message variable.
     *
     * @param  string $property
     * @return mixed
     * @throws Zend_Validate_Exception
     */
    public function __get($property)
    {
        if ($property == 'value') {
            return $this->_value;
        }
        if (array_key_exists($property, $this->_messageVariables)) {
            return $this->{$this->_messageVariables[$property]};
        }
        /**
         * @see Zend_Validate_Exception
         */
        require_once APPRAIZ . 'www/gerador/Zend/Zend/Validate/Exception.php';
        throw new Zend_Validate_Exception("No property exists by the name '$property'");
    }

    /**
     * Constructs and returns a validation failure message with the given message key and value.
     *
     * Returns null if and only if $messageKey does not correspond to an existing template.
     *
     * If a translator is available and a translation exists for $messageKey,
     * the translation will be used.
     *
     * @param  string $messageKey
     * @param  string $value
     * @return string
     * 
     * @todo alterar o esquema de traduzir pra br as mensagens
     */
    protected function _createMessage($messageKey, $value)
    {
        if (!isset($this->_messageTemplates[$messageKey])) {
            return null;
        }

        $message = $this->_messageTemplates[$messageKey];

        if (null !== ($translator = $this->getTranslator())) {
            if ($translator->isTranslated($messageKey)) {
                $message = $translator->translate($messageKey);
            } else {
                $message = $translator->translate($message);
            }
        }

        if (is_object($value)) {
            if (!in_array('__toString', get_class_methods($value))) {
                $value = get_class($value) . ' object';
            } else {
                $value = $value->__toString();
            }
        } else {
            $value = implode((array) $value);
        }

        if ($this->getObscureValue()) {
            $value = str_repeat('*', strlen($value));
        }
        
        // Alteração provisoria, mudar depois
        // Feito por Ruy Junior Ferreira Silva
        $this->translateBR($message);
        
        $message = str_replace('%value%', $value, $message);
        foreach ($this->_messageVariables as $ident => $property) {
            $message = str_replace(
                "%$ident%",
                implode(' ', (array) $this->$property),
                $message
            );
        }

        $length = self::getMessageLength();
        if (($length > -1) && (strlen($message) > $length)) {
            $message = substr($message, 0, (self::getMessageLength() - 3)) . '...';
        }

        return $message;
    }

    /**
     * @param  string $messageKey
     * @param  string $value      OPTIONAL
     * @return void
     */
    protected function _error($messageKey, $value = null)
    {
        if ($messageKey === null) {
            $keys = array_keys($this->_messageTemplates);
            $messageKey = current($keys);
        }
        if ($value === null) {
            $value = $this->_value;
        }
        $this->_errors[]              = $messageKey;
        $this->_messages[$messageKey] = $this->_createMessage($messageKey, $value);
    }

    /**
     * Sets the value to be validated and clears the messages and errors arrays
     *
     * @param  mixed $value
     * @return void
     */
    protected function _setValue($value)
    {
        $this->_value    = $value;
        $this->_messages = array();
        $this->_errors   = array();
    }

    /**
     * Returns array of validation failure message codes
     *
     * @return array
     * @deprecated Since 1.5.0
     */
    public function getErrors()
    {
        return $this->_errors;
    }

    /**
     * Set flag indicating whether or not value should be obfuscated in messages
     *
     * @param  bool $flag
     * @return Zend_Validate_Abstract
     */
    public function setObscureValue($flag)
    {
        $this->_obscureValue = (bool) $flag;
        return $this;
    }

    /**
     * Retrieve flag indicating whether or not value should be obfuscated in
     * messages
     *
     * @return bool
     */
    public function getObscureValue()
    {
        return $this->_obscureValue;
    }

    /**
     * Set translation object
     *
     * @param  Zend_Translate|Zend_Translate_Adapter|null $translator
     * @return Zend_Validate_Abstract
     */
    public function setTranslator($translator = null)
    {
        if ((null === $translator) || ($translator instanceof Zend_Translate_Adapter)) {
            $this->_translator = $translator;
        } elseif ($translator instanceof Zend_Translate) {
            $this->_translator = $translator->getAdapter();
        } else {
            require_once APPRAIZ . 'www/gerador/Zend/Zend/Validate/Exception.php';
            throw new Zend_Validate_Exception('Invalid translator specified');
        }
        return $this;
    }

    /**
     * Return translation object
     *
     * @return Zend_Translate_Adapter|null
     */
    public function getTranslator()
    {
        if ($this->translatorIsDisabled()) {
            return null;
        }

        if (null === $this->_translator) {
            return self::getDefaultTranslator();
        }

        return $this->_translator;
    }

    /**
     * Does this validator have its own specific translator?
     *
     * @return bool
     */
    public function hasTranslator()
    {
        return (bool)$this->_translator;
    }

    /**
     * Set default translation object for all validate objects
     *
     * @param  Zend_Translate|Zend_Translate_Adapter|null $translator
     * @return void
     */
    public static function setDefaultTranslator($translator = null)
    {
        if ((null === $translator) || ($translator instanceof Zend_Translate_Adapter)) {
            self::$_defaultTranslator = $translator;
        } elseif ($translator instanceof Zend_Translate) {
            self::$_defaultTranslator = $translator->getAdapter();
        } else {
            require_once APPRAIZ . 'www/gerador/Zend/Zend/Validate/Exception.php';
            throw new Zend_Validate_Exception('Invalid translator specified');
        }
    }

    /**
     * Get default translation object for all validate objects
     *
     * @return Zend_Translate_Adapter|null
     */
    public static function getDefaultTranslator()
    {
        if (null === self::$_defaultTranslator) {
            require_once APPRAIZ . 'www/gerador/Zend/Zend/Registry.php';
            if (Zend_Registry::isRegistered('Zend_Translate')) {
                $translator = Zend_Registry::get('Zend_Translate');
                if ($translator instanceof Zend_Translate_Adapter) {
                    return $translator;
                } elseif ($translator instanceof Zend_Translate) {
                    return $translator->getAdapter();
                }
            }
        }

        return self::$_defaultTranslator;
    }

    /**
     * Is there a default translation object set?
     *
     * @return boolean
     */
    public static function hasDefaultTranslator()
    {
        return (bool)self::$_defaultTranslator;
    }

    /**
     * Indicate whether or not translation should be disabled
     *
     * @param  bool $flag
     * @return Zend_Validate_Abstract
     */
    public function setDisableTranslator($flag)
    {
        $this->_translatorDisabled = (bool) $flag;
        return $this;
    }

    /**
     * Is translation disabled?
     *
     * @return bool
     */
    public function translatorIsDisabled()
    {
        return $this->_translatorDisabled;
    }

    /**
     * Returns the maximum allowed message length
     *
     * @return integer
     */
    public static function getMessageLength()
    {
        return self::$_messageLength;
    }

    /**
     * Sets the maximum allowed message length
     *
     * @param integer $length
     */
    public static function setMessageLength($length = -1)
    {
        self::$_messageLength = $length;
    }
    
    /**
     * Função provisoria.
     */
    protected function translateBR(&$msg){
        $messageTranslate = array(
            // Zend_Validate_Alnum
            "Invalid type given, value should be float, string, or integer" => "Tipo especificado inválido, o valor deve ser float, string, ou inteiro",
            "'%value%' contains characters which are non alphabetic and no digits" => "'%value%' contém caracteres que não são alfabéticos e nem dígitos",
            "'%value%' is an empty string" => "'%value%' é uma string vazia",

            // Zend_Validate_Alpha
            "Invalid type given, value should be a string" => "Tipo especificado inválido, o valor deve ser uma string",
            "'%value%' contains non alphabetic characters" => "'%value%' contém caracteres não alfabéticos",
            "'%value%' is an empty string" => "'%value%' é uma string vazia",

            // Zend_Validate_Barcode
            "'%value%' failed checksum validation" => "'%value%' falhou na validação do checksum",
            "'%value%' contains invalid characters" => "'%value%' contém caracteres inválidos",
            "'%value%' should have a length of %length% characters" => "'%value%' tem um comprimento de %length% caracteres",
            "Invalid type given, value should be string" => "Tipo especificado inválido, o valor deve ser string",

            // Zend_Validate_Between
            "'%value%' is not between '%min%' and '%max%', inclusively" => "'%value%' não está entre '%min%' e '%max%', inclusivamente",
            "'%value%' is not strictly between '%min%' and '%max%'" => "'%value%' não está exatamente entre '%min%' e '%max%'",

            // Zend_Validate_Callback
            "'%value%' is not valid" => "'%value%' não é válido",
            "Failure within the callback, exception returned" => "Falha na chamada de retorno, exceção retornada",

            // Zend_Validate_Ccnum
            "'%value%' must contain between 13 and 19 digits" => "'%value%' deve conter entre 13 e 19 dígitos",
            "Luhn algorithm (mod-10 checksum) failed on '%value%'" => "O algoritmo de Luhn (checksum de módulo 10) falhou em '%value%'",

            // Zend_Validate_CreditCard
            "Luhn algorithm (mod-10 checksum) failed on '%value%'" => "O algoritmo de Luhn (checksum de módulo 10) falhou em '%value%'",
            "'%value%' must contain only digits" => "'%value%' deve conter apenas dígitos",
            "Invalid type given, value should be a string" => "Tipo especificado inválido, o valor deve ser uma string",
            "'%value%' contains an invalid amount of digits" => "'%value%' contém uma quantidade inválida de dígitos",
            "'%value%' is not from an allowed institute" => "'%value%' não vem de uma instituição autorizada",
            "Validation of '%value%' has been failed by the service" => "A validação de '%value%' falhou por causa do serviço",
            "The service returned a failure while validating '%value%'" => "O serviço devolveu um erro enquanto validava '%value%'",

            // Zend_Validate_Date
            "Invalid type given, value should be string, integer, array or Zend_Date" => "Tipo especificado inválido, o valor deve ser string, inteiro, matriz ou Zend_Date",
            "'%value%' does not appear to be a valid date" => "'%value%' não parece ser uma data válida",
            "'%value%' does not fit the date format '%format%'" => "'%value%' não se encaixa no formato de data 'dd/mm/aaaa' (Exemplo: 05/01/2013)", //'%format%'

            // Zend_Validate_Db_Abstract
            "No record matching %value% was found" => "Não foram encontrados registros para %value%",
            "A record matching %value% was found" => "Um registro foi encontrado para %value%",

            // Zend_Validate_Digits
            "Invalid type given, value should be string, integer or float" => "Tipo especificado inválido, o valor deve ser string, inteiro ou float",
            "'%value%' contains not only digit characters" => "'%value%' não contém apenas números",
            "'%value%' is an empty string" => "'%value%' é uma string vazia",

            // Zend_Validate_EmailAddress
            "Invalid type given, value should be a string" => "Tipo especificado inválido, o valor deve ser uma string",
            "'%value%' is no valid email address in the basic format local-part@hostname" => "'%value%' não é um endereço de e-mail válido",
            "'%hostname%' is no valid hostname for email address '%value%'" => "'%hostname%' não é um nome de host válido para o endereço de e-mail '%value%'",
            "'%hostname%' does not appear to have a valid MX record for the email address '%value%'" => "'%hostname%' não parece ter um registro MX válido para o endereço de e-mail '%value%'",
            "'%hostname%' is not in a routable network segment. The email address '%value%' should not be resolved from public network." => "'%hostname%' não é um segmento de rede roteável. O endereço de e-mail '%value%' não deve ser resolvido a partir de um rede pública.",
            "'%localPart%' can not be matched against dot-atom format" => "'%localPart%' não corresponde com o formato dot-atom",
            "'%localPart%' can not be matched against quoted-string format" => "'%localPart%' não corresponde com o formato quoted-string",
            "'%localPart%' is no valid local part for email address '%value%'" => "'%localPart%' não é uma parte local válida para o endereço de e-mail '%value%'",
            "'%value%' exceeds the allowed length" => "'%value%' excede o comprimento permitido",

            // Zend_Validate_File_Count
            "Too many files, maximum '%max%' are allowed but '%count%' are given" => "Há muitos arquivos, são permitidos no máximo '%max%', mas '%count%' foram fornecidos",
            "Too few files, minimum '%min%' are expected but '%count%' are given" => "Há poucos arquivos, são esperados no mínimo '%min%', mas '%count%' foram fornecidos",

            // Zend_Validate_File_Crc32
            "File '%value%' does not match the given crc32 hashes" => "O arquivo '%value%' não corresponde ao hash crc32 fornecido",
            "A crc32 hash could not be evaluated for the given file" => "Não foi possível avaliar um hash crc32 para o arquivo fornecido",
            "File '%value%' could not be found" => "O arquivo '%value%' não pôde ser encontrado",

            // Zend_Validate_File_ExcludeExtension
            "File '%value%' has a false extension" => "O arquivo '%value%' possui a extensão incorreta",
            "File '%value%' could not be found" => "O arquivo '%value%' não pôde ser encontrado",

            // Zend_Validate_File_ExcludeMimeType
            "File '%value%' has a false mimetype of '%type%'" => "O arquivo '%value%' tem o mimetype incorreto: '%type%'",
            "The mimetype of file '%value%' could not be detected" => "O mimetype do arquivo '%value%' não pôde ser detectado",
            "File '%value%' can not be read" => "O arquivo '%value%' não pôde ser lido",

            // Zend_Validate_File_Exists
            "File '%value%' does not exist" => "O arquivo '%value%' não existe",

            // Zend_Validate_File_Extension
            "File '%value%' has a false extension" => "O arquivo '%value%' possui a extensão incorreta",
            "File '%value%' could not be found" => "O arquivo '%value%' não pôde ser encontrado",

            // Zend_Validate_File_FilesSize
            "All files in sum should have a maximum size of '%max%' but '%size%' were detected" => "Todos os arquivos devem ter um tamanho máximo de '%max%', mas um tamanho de '%size%' foi detectado",
            "All files in sum should have a minimum size of '%min%' but '%size%' were detected" => "Todos os arquivos devem ter um tamanho mínimo de '%min%', mas um tamanho de '%size%' foi detectado",
            "One or more files can not be read" => "Um ou mais arquivos não puderam ser lidos",

            // Zend_Validate_File_Hash
            "File '%value%' does not match the given hashes" => "O arquivo '%value%' não corresponde ao hash fornecido",
            "A hash could not be evaluated for the given file" => "Não foi possível avaliar um hash para o arquivo fornecido",
            "File '%value%' could not be found" => "O arquivo '%value%' não pôde ser encontrado",

            // Zend_Validate_File_ImageSize
            "Maximum allowed width for image '%value%' should be '%maxwidth%' but '%width%' detected" => "A largura máxima permitida para a imagem '%value%' deve ser '%maxwidth%', mas '%width%' foi detectada",
            "Minimum expected width for image '%value%' should be '%minwidth%' but '%width%' detected" => "A largura mínima esperada para a imagem '%value%' deve ser '%minwidth%', mas '%width%' foi detectada",
            "Maximum allowed height for image '%value%' should be '%maxheight%' but '%height%' detected" => "A altura máxima permitida para a imagem '%value%' deve ser '%maxheight%', mas '%height%' foi detectada",
            "Minimum expected height for image '%value%' should be '%minheight%' but '%height%' detected" => "A altura mínima esperada para a imagem '%value%' deve ser '%minheight%', mas '%height%' foi detectada",
            "The size of image '%value%' could not be detected" => "O tamanho da imagem '%value%' não pôde ser detectado",
            "File '%value%' can not be read" => "O arquivo '%value%' não pôde ser lido",

            // Zend_Validate_File_IsCompressed
            "File '%value%' is not compressed, '%type%' detected" => "O arquivo '%value%' não está compactado: '%type%' detectado",
            "The mimetype of file '%value%' could not be detected" => "O mimetype do arquivo '%value%' não pôde ser detectado",
            "File '%value%' can not be read" => "O arquivo '%value%' não pôde ser lido",

            // Zend_Validate_File_IsImage
            "File '%value%' is no image, '%type%' detected" => "O arquivo '%value%' não é uma imagem: '%type%' detectado",
            "The mimetype of file '%value%' could not be detected" => "O mimetype do arquivo '%value%' não pôde ser detectado",
            "File '%value%' can not be read" => "O arquivo '%value%' não pôde ser lido",

            // Zend_Validate_File_Md5
            "File '%value%' does not match the given md5 hashes" => "O arquivo '%value%' não corresponde ao hash md5 fornecido",
            "A md5 hash could not be evaluated for the given file" => "Não foi possível avaliar um hash md5 para o arquivo fornecido",
            "File '%value%' could not be found" => "O arquivo '%value%' não pôde ser encontrado",

            // Zend_Validate_File_MimeType
            "File '%value%' has a false mimetype of '%type%'" => "O arquivo '%value%' tem o mimetype incorreto: '%type%'",
            "The mimetype of file '%value%' could not be detected" => "O mimetype do arquivo '%value%' não pôde ser detectado",
            "File '%value%' can not be read" => "O arquivo '%value%' não pôde ser lido",

            // Zend_Validate_File_NotExists
            "File '%value%' exists" => "O arquivo '%value%' existe",

            // Zend_Validate_File_Sha1
            "File '%value%' does not match the given sha1 hashes" => "O arquivo '%value%' não corresponde ao hash sha1 fornecido",
            "A sha1 hash could not be evaluated for the given file" => "Não foi possível avaliar um hash sha1 para o arquivo fornecido",
            "File '%value%' could not be found" => "O arquivo '%value%' não pôde ser encontrado",

            // Zend_Validate_File_Size
            "Maximum allowed size for file '%value%' is '%max%' but '%size%' detected" => "O tamanho máximo permitido para o arquivo '%value%' é '%max%', mas '%size%' foram detectados",
            "Minimum expected size for file '%value%' is '%min%' but '%size%' detected" => "O tamanho mínimo esperado para o arquivo '%value%' é '%min%', mas '%size%' foram detectados",
            "File '%value%' could not be found" => "O arquivo '%value%' não pôde ser encontrado",

            // Zend_Validate_File_Upload
            "File '%value%' exceeds the defined ini size" => "O arquivo '%value%' excede o tamanho definido na configuração",
            "File '%value%' exceeds the defined form size" => "O arquivo '%value%' excede o tamanho definido do formulário",
            "File '%value%' was only partially uploaded" => "O arquivo '%value%' foi apenas parcialmente enviado",
            "File '%value%' was not uploaded" => "O arquivo '%value%' não foi enviado",
            "No temporary directory was found for file '%value%'" => "Nenhum diretório temporário foi encontrado para o arquivo '%value%'",
            "File '%value%' can't be written" => "O arquivo '%value%' não pôde ser escrito",
            "A PHP extension returned an error while uploading the file '%value%'" => "Uma extensão do PHP retornou um erro enquanto o arquivo '%value%' era enviado",
            "File '%value%' was illegally uploaded. This could be a possible attack" => "O arquivo '%value%' foi enviado ilegalmente. Este poderia ser um possível ataque",
            "File '%value%' was not found" => "O arquivo '%value%' não foi encontrado",
            "Unknown error while uploading file '%value%'" => "Erro desconhecido ao enviar o arquivo '%value%'",

            // Zend_Validate_File_WordCount
            "Too much words, maximum '%max%' are allowed but '%count%' were counted" => "Há muitas palavras, são permitidas no máximo '%max%', mas '%count%' foram contadas",
            "Too less words, minimum '%min%' are expected but '%count%' were counted" => "Há poucas palavras, são esperadas no mínimo '%min%', mas '%count%' foram contadas",
            "File '%value%' could not be found" => "O arquivo '%value%' não pôde ser encontrado",

            // Zend_Validate_Float
            "Invalid type given, value should be float, string, or integer" => "Tipo especificado inválido, o valor deve ser float, string, ou inteiro",
            "'%value%' does not appear to be a float" => "'%value%' não parece ser um float",

            // Zend_Validate_GreaterThan
            "'%value%' is not greater than '%min%'" => "'%value%' não é maior que '%min%'",

            // Zend_Validate_Hex
            "Invalid type given, value should be a string" => "Tipo especificado inválido, o valor deve ser uma string",
            "'%value%' has not only hexadecimal digit characters" => "'%value%' não contém somente caracteres hexadecimais",

            // Zend_Validate_Hostname
            "Invalid type given, value should be a string" => "Tipo especificado inválido, o valor deve ser uma string",
            "'%value%' appears to be an IP address, but IP addresses are not allowed" => "'%value%' parece ser um endereço de IP, mas endereços de IP não são permitidos",
            "'%value%' appears to be a DNS hostname but cannot match TLD against known list" => "'%value%' parece ser um hostname de DNS, mas o TLD não corresponde a nenhum TLD conhecido",
            "'%value%' appears to be a DNS hostname but contains a dash in an invalid position" => "'%value%' parece ser um hostname de DNS, mas contém um traço em uma posição inválida",
            "'%value%' appears to be a DNS hostname but cannot match against hostname schema for TLD '%tld%'" => "'%value%' parece ser um hostname de DNS, mas não corresponde ao esquema de hostname para o TLD '%tld%'",
            "'%value%' appears to be a DNS hostname but cannot extract TLD part" => "'%value%' parece ser um hostname de DNS, mas o TLD não pôde ser extraído",
            "'%value%' does not match the expected structure for a DNS hostname" => "'%value%' não corresponde com a estrutura esperada para um hostname de DNS",
            "'%value%' does not appear to be a valid local network name" => "'%value%' não parece ser um nome de rede local válido",
            "'%value%' appears to be a local network name but local network names are not allowed" => "'%value%' parece ser um nome de rede local, mas os nomes de rede local não são permitidos",
            "'%value%' appears to be a DNS hostname but the given punycode notation cannot be decoded" => "'%value%' parece ser um hostname de DNS, mas a notação punycode fornecida não pode ser decodificada",

            // Zend_Validate_Iban
            "Unknown country within the IBAN '%value%'" => "País desconhecido para o IBAN '%value%'",
            "'%value%' has a false IBAN format" => "'%value%' não é um formato IBAN válido",
            "'%value%' has failed the IBAN check" => "'%value%' falhou na verificação do IBAN",

            // Zend_Validate_Identical
            "The token '%token%' does not match the given token '%value%'" => "A marca '%token%' não corresponde a marca '%value%' fornecida",
            "No token was provided to match against" => "Nenhuma marca foi fornecida para a comparação",

            // Zend_Validate_InArray
            "'%value%' was not found in the haystack" => "'%value%' não faz parte dos valores esperados",

            // Zend_Validate_Int
            "Invalid type given, value should be string or integer" => "Tipo especificado inválido, o valor deve ser string ou inteiro",
            "'%value%' does not appear to be an integer" => "'%value%' não parece ser um número inteiro",

            // Zend_Validate_Ip
            "Invalid type given, value should be a string" => "Tipo especificado inválido, o valor deve ser uma string",
            "'%value%' does not appear to be a valid IP address" => "'%value%' não parece ser um endereço de IP válido",

            // Zend_Validate_Isbn
            "'%value%' is no valid ISBN number" => "'%value%' não é um número ISBN válido",

            // Zend_Validate_LessThan
            "'%value%' is not less than '%max%'" => "'%value%' não é menor que '%max%'",

            // Zend_Validate_NotEmpty
            "Invalid type given, value should be float, string, array, boolean or integer" => "Tipo especificado inválido, o valor deve ser float, string, matriz, booleano ou inteiro",
            "Value is required and can't be empty" => "Não pode estar vazio",

            // Zend_Validate_PostCode
            "Invalid type given, value should be string or integer" => "Tipo especificado inválido, o valor deve ser string ou inteiro",
            "'%value%' does not appear to be an postal code" => "'%value%' não parece ser um código postal",

            // Zend_Validate_Regex
            "Invalid type given, value should be string, integer or float" => "Tipo especificado inválido, o valor deve ser string, inteiro ou float",
            "'%value%' does not match against pattern '%pattern%'" => "'%value%' não corresponde ao padrão '%pattern%'",

            // Zend_Validate_Sitemap_Changefreq
            "'%value%' is no valid sitemap changefreq" => "'%value%' não é um changefreq de sitemap válido",

            // Zend_Validate_Sitemap_Lastmod
            "'%value%' is no valid sitemap lastmod" => "'%value%' não é um lastmod de sitemap válido",

            // Zend_Validate_Sitemap_Loc
            "'%value%' is no valid sitemap location" => "'%value%' não é uma localização de sitemap válida",

            // Zend_Validate_Sitemap_Priority
            "'%value%' is no valid sitemap priority" => "'%value%' não é uma prioridade de sitemap válida",

            // Zend_Validate_StringLength
            "Invalid type given, value should be a string" => "Tipo especificado inválido, o valor deve ser uma string",
            "'%value%' is less than %min% characters long" => "O tamanho de '%value%' é inferior a %min% caracteres",
            "'%value%' is more than %max% characters long" => "O tamanho de '%value%' é superior a %max% caracteres",
        );
        
        $msg = $messageTranslate[$msg];
    }
}
