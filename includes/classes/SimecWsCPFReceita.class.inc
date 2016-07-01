<?php

/**
 * SimecWsCPFReceita class
 * 
 *  
 * 
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class SimecWsCPFReceita extends SoapClient {

  private static $classmap = array(
                                   );

  public function SimecWsCPFReceita($wsdl = "http://ws.mec.gov.br/PessoaFisica/wsdl", $options = array()) {
    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    parent::__construct($wsdl, $options);
  }

  /**
   * Retorna dados de pessoa física pelo CPF 
   *
   * @param string $cpf
   * @return string
   */
  public function solicitarDadosResumidoPessoaFisicaPorCpf($cpf) {
    return $this->__soapCall('solicitarDadosResumidoPessoaFisicaPorCpf', array($cpf),       array(
            'uri' => 'urn:PessoaFisicaServices',
            'soapaction' => ''
           )
      );
  }

  /**
   * Busca dados de pessoa fisica diretamente da receita, sem passar pelo banco de dados 
   *
   * @param string $cpf
   * @return string
   */
  public function solicitarDadosReceitaPorCpf($cpf) {
    return $this->__soapCall('solicitarDadosReceitaPorCpf', array($cpf),       array(
            'uri' => 'urn:PessoaFisicaServices',
            'soapaction' => ''
           )
      );
  }

  /**
   * Retorna dados completo de pessoa física pelo CPF 
   *
   * @param string $cpf
   * @return string
   */
  public function solicitarDadosPessoaFisicaPorCpf($cpf) {
    return $this->__soapCall('solicitarDadosPessoaFisicaPorCpf', array($cpf),       array(
            'uri' => 'urn:PessoaFisicaServices',
            'soapaction' => ''
           )
      );
  }

  /**
   * Retorna dados de Endereço da pessoa física pelo CPF 
   *
   * @param string $cpf
   * @return string
   */
  public function solicitarDadosEnderecoPessoaFisicaPorCpf($cpf) {
    return $this->__soapCall('solicitarDadosEnderecoPessoaFisicaPorCpf', array($cpf),       array(
            'uri' => 'urn:PessoaFisicaServices',
            'soapaction' => ''
           )
      );
  }

  /**
   * Retorna dados de contato de pessoa física por CPF 
   *
   * @param string $cpf
   * @return string
   */
  public function solicitarDadosContatoPessoaFisicaPorCpf($cpf) {
    return $this->__soapCall('solicitarDadosContatoPessoaFisicaPorCpf', array($cpf),       array(
            'uri' => 'urn:PessoaFisicaServices',
            'soapaction' => ''
           )
      );
  }

}
?>