<?php

//include_once 'RestServicosReceitaFederal.php';

/**
 * Consome o Webservice da Receita Federal
 * 
 * @copyright Ministério da Cultura
 * @author Hepta/Minc - Rafael Jose da Costa Gloria
 * @since 11/01/2017
 * @version 1.0
 */
class RestReceitaFederal {

    const urlPessoaFisica = "pessoa_fisica/consultar/";
    const urlPessoaJuridica = "pessoa_juridica/consultar/";
    const urlForcar = "?forcarBuscaNaReceita=true";

    /**
     * Endereço do Webservice.
     * 
     * @var string 
     */
    protected $baseUrl;
    
    /**
     * Nome usado para se conectar com o WebService da Receita Federal.
     * 
     * @var string
     */
    protected $user;
    
    /**
     * Senha de acesso ao Webservice.
     * 
     * @var string
     */
    protected $password;

    public function getBaseUrl() {
        return $this->baseUrl;
    }

    public function getUser() {
        return $this->user;
    }

    public function getPassword() {
        return $this->password;
    }

    public function setBaseUrl(type $baseUrl) {
        $this->baseUrl = $baseUrl;
        return $this;
    }

    public function setUser($user) {
        $this->user = $user;
        return $this;
    }

    public function setPassword($password) {
        $this->password = $password;
        return $this;
    }
    
    /**
     * Metodo chamado quando o objeto da classe e instanciado
     *
     * @return VOID
     */
    public function __construct() {
        # Carrega credenciais de acesso ao serviço
        $this->baseUrl = WS_RF_BASE_URL;
        $this->user = WS_RF_USER;
        $this->password = WS_RF_SENHA;
    }
    
    /**
     * @author Alysson Vicuña de Oliveira
     *
     * @param $cnpj - CNPJ a ser consultado
     * @param bool $forcarBuscaReceita - Define se deve ir na Base da receita federal, mesmo já existindo o CPF na base do MINC
     * @param bool $returnJSON - Define se o retorno sera um JSON ou Array de Objetos
     * @return ArrayObject|mixed - Resultado da consulta em Json ou ArrayObject
     */
    public function consultarPessoaJuridicaReceitaFederal($cnpj, $forcarBuscaReceita = false, $returnJSON = false) {
        $cnpj = $this->retirarMascara($cnpj);
        if (15 == strlen($cnpj) && !isRfCnpj($cnpj)) {
            throw new InvalidArgumentException("CPF/CNPJ inválido");
        }

        $url = $this->baseUrl . self::urlPessoaJuridica . $cnpj;
        if ($forcarBuscaReceita) {
            $url .= self::urlForcar;
        }

        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, $url);
        curl_setopt($curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        curl_setopt($curl, CURLOPT_USERPWD, "$this->user:$this->password");
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
        $resultCurl = curl_exec($curl);
        curl_close($curl);
        $result = new ArrayObject(json_decode($resultCurl, true));

        if ($returnJSON) {
            $retornoResultado = $resultCurl; #Retorno do Formato JSON
        } else {
            $retornoResultado = (object)(array)$result; #Retorno Objeto
        }

        return $retornoResultado;
    }

    /**
     * @author Alysson Vicuña de Oliveira
     *
     * @param $cpf - CPF a ser consultado
     * @param bool $forcarBuscaReceita - Define se deve ir na Base da receita federal, mesmo já existindo o CPF na base do MINC
     * @param bool $returnJSON - Define se o retorno sera um JSON ou Array de Objetos
     * @return ArrayObject|mixed - Resultado da consulta em Json ou ArrayObject
     */
    public function consultarPessoaFisicaReceitaFederal($cpf, $forcarBuscaReceita = false, $returnJSON = false) {
        $cpf = $this->retirarMascara($cpf);
        
        if (11 == strlen($cpf) && !isRfCPF($cpf)) {
            throw new InvalidArgumentException("CPF/CNPJ inválido");
        }

        $url = $this->baseUrl . self::urlPessoaFisica . $cpf;
        if ($forcarBuscaReceita) {
            $url .= self::urlForcar;
        }

        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, $url);
        curl_setopt($curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        curl_setopt($curl, CURLOPT_USERPWD, "$this->user:$this->password");
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
        $resultCurl = curl_exec($curl);
        curl_close($curl);
        $result = new ArrayObject(json_decode($resultCurl, true));

        if ($returnJSON) {
            $retornoResultado = $resultCurl; #Retorno do Formato JSON
        } else {
            $retornoResultado = (object)(array)$result; #Retorno Objeto
        }

        return $retornoResultado;
    }

    /**
     * Retira os caracteres de mascara do texto. Por exemplo: '.', '/', '-'.
     * 
     * @param string $texto
     * @return string
     */
    protected function retirarMascara($texto) {
        $chars = array(".", "/", "-");
        $resultado = str_replace($chars, "", $texto);
        
        return $resultado;
    }
    
    /**
     * Metodo chamado quando o objeto da classe e serializado
     *
     * @return VOID
     */
    public function __sleep() {
        return;
    }

    /**
     * Metodo chamado quando o objeto da classe e unserializado
     *
     * @return VOID
     */
    public function __wakeup() {
        return;
    }

    /**
     * Caso o metodo nao seja encontrado
     *
     * @param STRING $strMethod
     * @param ARRAY $arrParameters
     * @return VOID
     */
    public function __call($strMethod, $arrParameters) {
        debug("O metodo " . $strMethod . " nao foi encontrado na classe " . get_class($this) . ".<br />" . __FILE__ . "(linha " . __LINE__ . ")", 1);
    }

}

/**
 * Verifica se o CPF é válido.
 * 
 * @param string $cpf
 * @return boolean
 */
function isRfCPF($cpf) {
    $cpf = str_pad(preg_replace('[^0-9]', '', $cpf), 11, '0', STR_PAD_LEFT);

    if (strlen($cpf) != 11 || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '') {
        return false;
    } else {   // Calcula os numeros para verificar se o CPF é verdadeiro
        for ($t = 9; $t < 11; $t++) {
            for ($d = 0, $c = 0; $c < $t; $c++) {
                $d += $cpf{$c} * (($t + 1) - $c);
            }

            $d = ((10 * $d) % 11) % 10;

            if ($cpf{$c} != $d) {
                return false;
            }
        }

        return true;
    }
}

/**
 * Verifica se o CNPJ é válido.
 * 
 * @param string $cnpj "00.000.000/0000-00", "00000000000000", "00 000 000 0000 00" etc...
 * @return boolean
 */
function isRfCnpj($cnpj) {
    // Etapa 1: Cria um array com apenas os digitos numéricos,
    // Isso permite receber o cnpj em diferentes formatos como:
    // "00.000.000/0000-00", "00000000000000", "00 000 000 0000 00" etc...
    $num = array();
    $j = 0;
    for ($i = 0; $i < (strlen($cnpj)); $i++) {
        if (is_numeric($cnpj[$i])) {
            $num[$j] = $cnpj[$i];
            $j++;
        }
    }

    //Etapa 2: Conta os dígitos, um Cnpj válido possui 14 dígitos numéricos.
    if (count($num) != 14) {
        return false;
    }

    //Etapa 3: O número 00000000000 embora não seja um cnpj real resultaria um cnpj válido
    // após o calculo dos dígitos verificares e por isso precisa ser filtradas nesta etapa.
    if ($num[0] == 0 && $num[1] == 0 && $num[2] == 0 && $num[3] == 0 && $num[4] == 0 && $num[5] == 0 && $num[6] == 0 && $num[7] == 0 && $num[8] == 0 && $num[9] == 0 && $num[10] == 0 && $num[11] == 0) {
        return false;
    }

    //Etapa 4: Calcula e compara o primeiro dígito verificador.
    else {
        $j = 5;
        for ($i = 0; $i < 4; $i++) {
            $multiplica[$i] = $num[$i] * $j;
            $j--;
        }
        $soma = array_sum($multiplica);
        $j = 9;
        for ($i = 4; $i < 12; $i++) {
            $multiplica[$i] = $num[$i] * $j;
            $j--;
        }
        $soma = array_sum($multiplica);
        $resto = $soma % 11;
        if ($resto < 2) {
            $dg = 0;
        } else {
            $dg = 11 - $resto;
        }
        if ($dg != $num[12]) {
            return false;
        }
    }

    //Etapa 5: Calcula e compara o segundo dígito verificador.
    if (!isset($isCnpjValid)) {
        $j = 6;
        for ($i = 0; $i < 5; $i++) {
            $multiplica[$i] = $num[$i] * $j;
            $j--;
        }
        $soma = array_sum($multiplica);
        $j = 9;
        for ($i = 5; $i < 13; $i++) {
            $multiplica[$i] = $num[$i] * $j;
            $j--;
        }
        $soma = array_sum($multiplica);
        $resto = $soma % 11;
        if ($resto < 2) {
            $dg = 0;
        } else {
            $dg = 11 - $resto;
        }
        if ($dg != $num[13]) {
            return false;
        } else {
            return true;
        }
    }
}

/**
 * Formata a data para o formato americano.
 * 
 * @param string $data
 * @return string
 */
function formatDateWS($data)
{
	// retorna a data no formato yyyy-mm-dd
	return substr($data,6,4).'-'.substr($data,3,2).'-'.substr($data,0,2);
}
