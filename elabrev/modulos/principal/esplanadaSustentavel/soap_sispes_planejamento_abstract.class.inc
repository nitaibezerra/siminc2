<?php
class inserirMetaFinanceira {
  public $arg0; // credencialDTO
  public $arg1; // metaFinanceiraDTO
}

class credencialDTO {
  public $senha; // string
  public $usuario; // string
}

class metaFinanceiraDTO {
  public $anoExercicio; // int
  public $codigoUO; // string
  public $percentual; // decimal
}

class inserirMetaFinanceiraResponse {
  public $return; // retornoMetaDTO
}

class retornoMetaDTO {
  public $mensagem; // string
}

class inserirMetaFisica {
  public $arg0; // credencialDTO
  public $arg1; // metaFisicaDTO
}

class metaFisicaDTO {
  public $anoExercicio; // int
  public $codigoUO; // string
  public $produto; // string
  public $valor; // int
}

class inserirMetaFisicaResponse {
  public $return; // retornoMetaDTO
}

class consultarEntidadesDoOrgao {
  public $arg0; // credencialDTO
  public $arg1; // string
  public $arg2; // int
}

class consultarEntidadesDoOrgaoResponse {
  public $return; // retornoEntidadesDTO
}

class retornoEntidadesDTO {
  public $codigoOrgao; // string
  public $mensagem; // string
  public $nomeOrgao; // string
  public $uos; // unidadeOrcamentariaDTO
}

class unidadeOrcamentariaDTO {
  public $codigo; // string
  public $entidades; // entidadeDTO
  public $nome; // string
}

class entidadeDTO {
  public $codigo; // int
  public $nome; // string
}

class consultarTiposDespesaExercicioAtual {
  public $arg0; // credencialDTO
}

class consultarTiposDespesaExercicioAtualResponse {
  public $return; // retornoTipoDespesaDTO
}

class retornoTipoDespesaDTO {
  public $listaTipoDespesa; // tipoDespesaDTO
  public $mensagem; // string
}

class tipoDespesaDTO {
  public $id; // int
  public $nome; // string
}

class inserirMetaEntidade {
  public $arg0; // credencialDTO
  public $arg1; // metaEntidadeDTO
}

class metaEntidadeDTO {
  public $anoExercicio; // int
  public $codigoEntidade; // int
  public $codigoUo; // string
  public $despesas; // despesaEntidadeDTO
}

class despesaEntidadeDTO {
  public $codigoDespesa; // int
  public $valor; // decimal
}

class inserirMetaEntidadeResponse {
  public $return; // retornoMetaEntidadeDTO
}

class retornoMetaEntidadeDTO {
  public $mensagem; // string
}


/**
 * MetaWSImplService class
 * 
 *  
 * 
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class MetaWSImplService extends SoapClient {

  private static $classmap = array(
                                    'inserirMetaFinanceira' => 'inserirMetaFinanceira',
                                    'credencialDTO' => 'credencialDTO',
                                    'metaFinanceiraDTO' => 'metaFinanceiraDTO',
                                    'inserirMetaFinanceiraResponse' => 'inserirMetaFinanceiraResponse',
                                    'retornoMetaDTO' => 'retornoMetaDTO',
                                    'inserirMetaFisica' => 'inserirMetaFisica',
                                    'metaFisicaDTO' => 'metaFisicaDTO',
                                    'inserirMetaFisicaResponse' => 'inserirMetaFisicaResponse',
                                    'consultarEntidadesDoOrgao' => 'consultarEntidadesDoOrgao',
                                    'consultarEntidadesDoOrgaoResponse' => 'consultarEntidadesDoOrgaoResponse',
                                    'retornoEntidadesDTO' => 'retornoEntidadesDTO',
                                    'unidadeOrcamentariaDTO' => 'unidadeOrcamentariaDTO',
                                    'entidadeDTO' => 'entidadeDTO',
                                    'consultarTiposDespesaExercicioAtual' => 'consultarTiposDespesaExercicioAtual',
                                    'consultarTiposDespesaExercicioAtualResponse' => 'consultarTiposDespesaExercicioAtualResponse',
                                    'retornoTipoDespesaDTO' => 'retornoTipoDespesaDTO',
                                    'tipoDespesaDTO' => 'tipoDespesaDTO',
                                    'inserirMetaEntidade' => 'inserirMetaEntidade',
                                    'metaEntidadeDTO' => 'metaEntidadeDTO',
                                    'despesaEntidadeDTO' => 'despesaEntidadeDTO',
                                    'inserirMetaEntidadeResponse' => 'inserirMetaEntidadeResponse',
                                    'retornoMetaEntidadeDTO' => 'retornoMetaEntidadeDTO',
                                   );

  public function MetaWSImplService($wsdl = "https://hom.pes.planejamento.gov.br/sispes/MetaWSImpl?wsdl", $options = array()) {
    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    parent::__construct($wsdl, $options);
  }

  /**
   *  
   *
   * @param inserirMetaFinanceira $parameters
   * @return inserirMetaFinanceiraResponse
   */
  public function inserirMetaFinanceira(inserirMetaFinanceira $parameters) {
    return $this->__soapCall('inserirMetaFinanceira', array($parameters),       array(
            'uri' => 'http://meta.webservice.sispes.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param inserirMetaFisica $parameters
   * @return inserirMetaFisicaResponse
   */
  public function inserirMetaFisica(inserirMetaFisica $parameters) {
    return $this->__soapCall('inserirMetaFisica', array($parameters),       array(
            'uri' => 'http://meta.webservice.sispes.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param consultarEntidadesDoOrgao $parameters
   * @return consultarEntidadesDoOrgaoResponse
   */
  public function consultarEntidadesDoOrgao(consultarEntidadesDoOrgao $parameters) {
    return $this->__soapCall('consultarEntidadesDoOrgao', array($parameters),       array(
            'uri' => 'http://meta.webservice.sispes.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param consultarTiposDespesaExercicioAtual $parameters
   * @return consultarTiposDespesaExercicioAtualResponse
   */
  public function consultarTiposDespesaExercicioAtual(consultarTiposDespesaExercicioAtual $parameters) {
    return $this->__soapCall('consultarTiposDespesaExercicioAtual', array($parameters),       array(
            'uri' => 'http://meta.webservice.sispes.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param inserirMetaEntidade $parameters
   * @return inserirMetaEntidadeResponse
   */
  public function inserirMetaEntidade(inserirMetaEntidade $parameters) {
    return $this->__soapCall('inserirMetaEntidade', array($parameters),       array(
            'uri' => 'http://meta.webservice.sispes.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

}
?>