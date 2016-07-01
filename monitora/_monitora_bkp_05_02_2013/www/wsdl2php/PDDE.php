<?php
class ValorEstimadoEscola {
  public $anoExercicio; // string
  public $coEscola; // string
  public $coProgramaFnde; // string
}

class IsEscolaPaga {
  public $anoExercicio; // string
  public $coEscola; // string
  public $coProgramaFnde; // string
}

class AtualizaAnaliseEscola {
  public $anoExercicio; // string
  public $coEscola; // string
  public $coProgramaFnde; // string
}

class AtualizaValoresEstimativa {
  public $anoExercicio; // string
  public $coProgramaFnde; // string
  public $coEscola; // string
  public $coDestinacao; // string
  public $vlCusteio; // string
  public $vlCapital; // string
  public $vlTotal; // string
}

class escola_pag_dest {
  public $p_an_exercicio; // string
  public $p_co_programa_fnde; // string
  public $p_co_destinacao; // string
  public $p_co_escola; // string
}

class escola_pag_prog {
  public $p_an_exercicio; // string
  public $p_co_programa_fnde; // string
  public $p_co_escola; // string
}

class escola_dados_prog {
  public $p_co_escola; // string
}

class lista_destinacao {
  public $p_an_exercicio; // string
  public $p_co_programa_fnde; // string
}


/**
 * PDDE class
 * 
 *  
 * 
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class PDDE extends SoapClient {

  private static $classmap = array(
                                    'ValorEstimadoEscola' => 'ValorEstimadoEscola',
                                    'IsEscolaPaga' => 'IsEscolaPaga',
                                    'AtualizaAnaliseEscola' => 'AtualizaAnaliseEscola',
                                    'AtualizaValoresEstimativa' => 'AtualizaValoresEstimativa',
                                    'escola_pag_dest' => 'escola_pag_dest',
                                    'escola_pag_prog' => 'escola_pag_prog',
                                    'escola_dados_prog' => 'escola_dados_prog',
                                    'lista_destinacao' => 'lista_destinacao',
                                   );

  public function PDDE($wsdl = "http://www.fnde.gov.br/pddewebservice/server.php?wsdl", $options = array()) {
    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    parent::__construct($wsdl, $options);
  }

  /**
   * Como utilizar:<br />
    	//String de comunica&ccedil;&atilde;o<br />
		&#36;wsdl='http://www.fnde.gov.br/pddewebservice/server.php?wsdl';<br 
   * />
		<br />
		//Com biblioteca NuSoap<br />
		//&#36;client = new nusoap_client(&#36;wsdl, 
   * 'wsdl');<br />
		//Com Soap nativo do php<br />
		&#36;client = new SoapClient(&#36;wsdl);<br 
   * />
		<br />
		&#36;param = array('valorEstimadoEscola'=>array(<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'anoExercicio'=>'2008',<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'coEscola'=>'33081050',<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'coProgramaFnde'=>'02'<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;)<br />
		);<br />
		<br />
		//Com biblioteca NuSoap<br 
   * />
		//echo &#36;retorno = &#36;client->call('valorEstimadoEscola', &#36;param);<br />
		//Com 
   * Soap nativo do php<br />
		echo &#36;retorno = &#36;client->__SoapCall('valorEstimadoEscola', 
   * &#36;param);<br /> 
   *
   * @param ValorEstimadoEscola $valorEstimadoEscola
   * @return string
   */
  public function valorEstimadoEscola(ValorEstimadoEscola $valorEstimadoEscola) {
    return $this->__soapCall('valorEstimadoEscola', array($valorEstimadoEscola),       array(
            'uri' => 'http://www.fnde.gov.br/pddewebservice/',
            'soapaction' => ''
           )
      );
  }

  /**
   * Como utilizar:<br />
    	//String de comunica&ccedil;&atilde;o<br />
		&#36;wsdl='http://www.fnde.gov.br/pddewebservice/server.php?wsdl';<br 
   * />
		<br />
		//Com biblioteca NuSoap<br />
		//&#36;client = new nusoap_client(&#36;wsdl, 
   * 'wsdl');<br />
		//Com Soap nativo do php<br />
		&#36;client = new SoapClient(&#36;wsdl);<br 
   * />
		<br />
		&#36;param = array('isEscolaPaga'=>array(<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'anoExercicio'=>'2008',<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'coEscola'=>'12023590',<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'coProgramaFnde'=>'02'<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;)<br />
		);<br />
		<br />
		//Com biblioteca NuSoap<br 
   * />
		//echo &#36;retorno = &#36;client->call('isEscolaPaga', &#36;param);<br />
		//Com 
   * Soap nativo do php<br />
		echo &#36;retorno = &#36;client->__SoapCall('isEscolaPaga', 
   * &#36;param);<br /> 
   *
   * @param IsEscolaPaga $isEscolaPaga
   * @return string
   */
  public function isEscolaPaga(IsEscolaPaga $isEscolaPaga) {
    return $this->__soapCall('isEscolaPaga', array($isEscolaPaga),       array(
            'uri' => 'http://www.fnde.gov.br/pddewebservice/',
            'soapaction' => ''
           )
      );
  }

  /**
   * Como utilizar:<br />
    	//String de comunica&ccedil;&atilde;o<br />
		&#36;wsdl='http://www.fnde.gov.br/pddewebservice/server.php?wsdl';<br 
   * />
		<br />
		//Com biblioteca NuSoap<br />
		//&#36;client = new nusoap_client(&#36;wsdl, 
   * 'wsdl');<br />
		//Com Soap nativo do php<br />
		&#36;client = new SoapClient(&#36;wsdl);<br 
   * />
		<br />
		&#36;param = array('atualizaAnaliseEscola'=>array(<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'anoExercicio'=>'2008',<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'coEscola'=>'12006831',<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'coProgramaFnde'=>'96'<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;)<br />
		);<br />
		<br />
		//Com biblioteca NuSoap<br 
   * />
		//echo &#36;retorno = &#36;client->call('atualizaAnaliseEscola', &#36;param);<br 
   * />
		//Com Soap nativo do php<br />
		echo &#36;retorno = &#36;client->__SoapCall('atualizaAnaliseEscola', 
   * &#36;param);<br /> 
   *
   * @param AtualizaAnaliseEscola $atualizaAnaliseEscola
   * @return string
   */
  public function atualizaAnaliseEscola(AtualizaAnaliseEscola $atualizaAnaliseEscola) {
    return $this->__soapCall('atualizaAnaliseEscola', array($atualizaAnaliseEscola),       array(
            'uri' => 'http://www.fnde.gov.br/pddewebservice/',
            'soapaction' => ''
           )
      );
  }

  /**
   * Como utilizar:<br />
    	//String de comunica&ccedil;&atilde;o<br />
		&#36;wsdl='http://www.fnde.gov.br/pddewebservice/server.php?wsdl';<br 
   * />
		<br />
		//Com biblioteca NuSoap<br />
		//&#36;client = new nusoap_client(&#36;wsdl, 
   * 'wsdl');<br />
		//Com Soap nativo do php<br />
		&#36;client = new SoapClient(&#36;wsdl);<br 
   * />
		<br />
		&#36;param = array('atualizaValoresEstimativa'=>array(<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'anoExercicio'=>'2008',<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'coEscola'=>'33081050',<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'coProgramaFnde'=>'02'<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'coDestinacao'=>'01'<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'vlCusteio'=>'1000'<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'vlCapital'=>'1000'<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'vlTotal'=>'2000'<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;)<br />
		);<br />
		<br />
		//Com biblioteca NuSoap<br 
   * />
		//echo &#36;retorno = &#36;client->call('atualizaValoresEstimativa', &#36;param);<br 
   * />
		//Com Soap nativo do php<br />
		echo &#36;retorno = &#36;client->__SoapCall('atualizaValoresEstimativa', 
   * &#36;param);<br /> 
   *
   * @param AtualizaValoresEstimativa $atualizaValoresEstimativa
   * @return string
   */
  public function atualizaValoresEstimativa(AtualizaValoresEstimativa $atualizaValoresEstimativa) {
    return $this->__soapCall('atualizaValoresEstimativa', array($atualizaValoresEstimativa),       array(
            'uri' => 'http://www.fnde.gov.br/pddewebservice/',
            'soapaction' => ''
           )
      );
  }

  /**
   * Como utilizar:<br />
    	//String de comunica&ccedil;&atilde;o<br />
		&#36;wsdl='http://www.fnde.gov.br/pddewebservice/server.php?wsdl';<br 
   * />
		<br />
		//Com biblioteca NuSoap<br />
		//&#36;client = new nusoap_client(&#36;wsdl, 
   * 'wsdl');<br />
		//Com Soap nativo do php<br />
		&#36;client = new SoapClient(&#36;wsdl);<br 
   * />
		<br />
		&#36;param = array('escola_pag_dest'=>array(<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_an_exercicio'=>'2010',<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_co_programa_fnde'=>'02',<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_co_destinacao'=>'01'<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_co_escola'=>'12022080'<br />
		&nbsp;&nbsp;&nbsp;&nbsp;)<br 
   * />
		);<br />
		<br />
		//Com biblioteca NuSoap<br />
		//echo &#36;retorno = &#36;client->call('escola_pag_dest', 
   * &#36;param);<br />
		//Com Soap nativo do php<br />
		echo &#36;retorno = &#36;client->__SoapCall('escola_pag_dest', 
   * &#36;param);<br /> 
   *
   * @param escola_pag_dest $escola_pag_dest
   * @return string
   */
  public function escola_pag_dest(escola_pag_dest $escola_pag_dest) {
    return $this->__soapCall('escola_pag_dest', array($escola_pag_dest),       array(
            'uri' => 'http://www.fnde.gov.br/pddewebservice/',
            'soapaction' => ''
           )
      );
  }

  /**
   * Como utilizar:<br />
    	//String de comunica&ccedil;&atilde;o<br />
		&#36;wsdl='http://www.fnde.gov.br/pddewebservice/server.php?wsdl';<br 
   * />
		<br />
		//Com biblioteca NuSoap<br />
		//&#36;client = new nusoap_client(&#36;wsdl, 
   * 'wsdl');<br />
		//Com Soap nativo do php<br />
		&#36;client = new SoapClient(&#36;wsdl);<br 
   * />
		<br />
		&#36;param = array('escola_pag_prog'=>array(<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_an_exercicio'=>'2010',<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_co_programa_fnde'=>'02',<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_co_destinacao'=>'01'<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_co_escola'=>'12022080'<br />
		&nbsp;&nbsp;&nbsp;&nbsp;)<br 
   * />
		);<br />
		<br />
		//Com biblioteca NuSoap<br />
		//echo &#36;retorno = &#36;client->call('escola_pag_prog', 
   * &#36;param);<br />
		//Com Soap nativo do php<br />
		echo &#36;retorno = &#36;client->__SoapCall('escola_pag_prog', 
   * &#36;param);<br /> 
   *
   * @param escola_pag_prog $escola_pag_prog
   * @return string
   */
  public function escola_pag_prog(escola_pag_prog $escola_pag_prog) {
    return $this->__soapCall('escola_pag_prog', array($escola_pag_prog),       array(
            'uri' => 'http://www.fnde.gov.br/pddewebservice/',
            'soapaction' => ''
           )
      );
  }

  /**
   * Como utilizar:<br />
    	//String de comunica&ccedil;&atilde;o<br />
		&#36;wsdl='http://www.fnde.gov.br/pddewebservice/server.php?wsdl';<br 
   * />
		<br />
		//Com biblioteca NuSoap<br />
		//&#36;client = new nusoap_client(&#36;wsdl, 
   * 'wsdl');<br />
		//Com Soap nativo do php<br />
		&#36;client = new SoapClient(&#36;wsdl);<br 
   * />
		<br />
		&#36;param = array('escola_dados_prog'=>array(<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_co_escola'=>'12022080',<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;)<br />
		);<br />
		<br />
		//Com biblioteca NuSoap<br 
   * />
		//echo &#36;retorno = &#36;client->call('escola_dados_prog', &#36;param);<br />
		//Com 
   * Soap nativo do php<br />
		echo &#36;retorno = &#36;client->__SoapCall('escola_dados_prog', 
   * &#36;param);<br /> 
   *
   * @param escola_dados_prog $escola_dados_prog
   * @return string
   */
  public function escola_dados_prog(escola_dados_prog $escola_dados_prog) {
    return $this->__soapCall('escola_dados_prog', array($escola_dados_prog),       array(
            'uri' => 'http://www.fnde.gov.br/pddewebservice/',
            'soapaction' => ''
           )
      );
  }

  /**
   * Como utilizar:<br />
    	//String de comunica&ccedil;&atilde;o<br />
		&#36;wsdl='http://www.fnde.gov.br/pddewebservice/server.php?wsdl';<br 
   * />
		<br />
		//Com biblioteca NuSoap<br />
		//&#36;client = new nusoap_client(&#36;wsdl, 
   * 'wsdl');<br />
		//Com Soap nativo do php<br />
		&#36;client = new SoapClient(&#36;wsdl);<br 
   * />
		<br />
		&#36;param = array('lista_destinacao'=>array(<br />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_an_exercicio'=>'2010',<br 
   * />
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'p_co_programa_fnde'=>'02',<br />
		&nbsp;&nbsp;&nbsp;&nbsp;)<br 
   * />
		);<br />
		<br />
		//Com biblioteca NuSoap<br />
		//echo &#36;retorno = &#36;client->call('escola_pag_prog', 
   * &#36;param);<br />
		//Com Soap nativo do php<br />
		echo &#36;retorno = &#36;client->__SoapCall('escola_pag_prog', 
   * &#36;param);<br /> 
   *
   * @param lista_destinacao $lista_destinacao
   * @return string
   */
  public function lista_destinacao(lista_destinacao $lista_destinacao) {
    return $this->__soapCall('lista_destinacao', array($lista_destinacao),       array(
            'uri' => 'http://www.fnde.gov.br/pddewebservice/',
            'soapaction' => ''
           )
      );
  }

}

?>
