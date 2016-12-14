<?php
/**
 * PLANACOMORC
 * base: Programação Orçamentária
 */

/**
 * Classe de mapeamento de retorno do WSQualitativo do SIOP.
 * @see WSQualitativo
 */
include_once APPRAIZ . "planacomorc/classes/WSQualitativo.class.inc";

class SiopQualitativo{
  private $_urlWsdl;
  private $_callOptions = array();
  private $wsusuario;
  private $wssenha;
  private $post;
  private $codigomomento;

  private $acaid;
  private $acacod;
  private $unicod;
  private $valorfisico;
  private $db;

  private $_overloadedMethods = array(
      'obterTabelasApoio', 'obterAcoes', 'obterOrgao', 'obterProgramas',
      'obterObjetivos', 'obterMetas', 'obterIniciativas', 'obterLocalizadores',
      'obterRegionalizacoes', 'obterUnidadesOrcamentarias', 'obterIndicadores',
      'obterMomentos', 'obterPlanosOrcamentarios');

  public function __construct($arrParam = array()) {
    global $db;

    if (isset($_POST['webservice'])) {
        $arrParam['post']['webservice'] = $_POST['webservice'];
    }
    
    $this->_urlWsd = WEB_SERVICE_SIOP_URL. 'WSQualitativo?wsdl';
    $this->_callOptions['local_cert'] = WEB_SERVICE_SIOP_CERTIFICADO;

    $this->_callOptions['passphrase'] = 'simec';
    $this->_callOptions['exceptions'] = true;
    $this->_callOptions['trace'] = true;
    $this->_callOptions['encoding'] = 'ISO-8859-1';
//    $this->_callOptions['encoding'] = 'UTF-8';

    if ($db) { $this->db = $db;
    } else { $this->db = new cls_banco(); }

    $this->wsusuario = $arrParam['usuario'];
    $this->wssenha = $arrParam['senha'];
    $this->post = $arrParam['post'];
    $this->codigomomento = $arrParam['post']['codigomomento'];
    $this->acaid = '';
    $this->unicod = '';
    $this->acacod = '';
    $this->valorfisico = '';
  }

  public function setPost($arrParam) {
      $this->post = $arrParam;
      if (isset($_POST['webservice'])) {
          $arrParam['post']['webservice'] = $_POST['webservice'];
      }
  }

  /**
   * Executa uma chamada ao método obterProgramacaoCompleta do webservice Qualitativo.
   * @return retornoProgramacaoQualitativoDTO
   */
  public function obterProgramacaoCompleta() {
    $obEnvio = $this->getObterProgramacaoCompleta();
    $wsQualitativo = new WSQualitativo($this->_urlWsdl, $this->_callOptions);
    $retorno = $wsQualitativo->obterProgramacaoCompleta($obEnvio);
    return $retorno;
  }

  /**
   * Sobrecarga dos métodos de consulta ao webservice. Os métodos executam um
   * procedimento padrão e comum.
   * @param string $nome Nome do método invocado;
   * @param array $parametros Parâmetros enviados na chamada.
   * @return mixed
   */
  public function __call($nome, $parametros) {
    if (!in_array($nome, $this->_overloadedMethods)) {
      trigger_error("Função '{$nome}' não está definida em SiopQualitativo nem em SiopQualitativo::_overloadedMethods.", E_USER_ERROR);
    }

    $nome = ucfirst($nome);
    $get = "get{$nome}";
    $oEnvio = $this->$get();
    if (!is_object($oEnvio)) {
      trigger_error("O método 'get{$nome}' deve retornar um objeto.");
    }
    $wsQualitativo = new WSQualitativo($this->_urlWsdl, $this->_callOptions);
    $classObter = get_class($oEnvio);

    return $wsQualitativo->$classObter($oEnvio);
  }

  /**
   * Cria um objeto credencialDTO com as credenciais de acesso ao ws.
   * @return \credencialDTO
   */
  private function getCredencial() {
    $ObjetoWS = new credencialDTO();
    $ObjetoWS->perfil = 32;
    $ObjetoWS->senha = md5($this->wssenha);
    $ObjetoWS->usuario = $this->wsusuario;

    return $ObjetoWS;
  }

  /**
   * Seleciona quais métodos serão executados ao invocar o webservice.
   * @return obterProgramacaoCompleta
   */
  private function getObterProgramacaoCompleta() {
    $ObjetoWS = new obterProgramacaoCompleta();
    $ObjetoWS->credencial = $this->getCredencial(); // credencialDTO
    $ObjetoWS->exercicio = $_SESSION['exercicio']; // int
    $ObjetoWS->codigoMomento = $this->codigomomento; // int
    $ObjetoWS->retornarOrgaos = (boolean) $this->post['retornarOrgaos'];
    $ObjetoWS->retornarProgramas = (boolean) $this->post['retornarProgramas'];
    $ObjetoWS->retornarIndicadores = (boolean) $this->post['retornarIndicadores'];
    $ObjetoWS->retornarObjetivos = (boolean) $this->post['retornarObjetivos'];
    $ObjetoWS->retornarIniciativas = (boolean) $this->post['retornarIniciativas'];
    $ObjetoWS->retornarAcoes = (boolean) $this->post['retornarAcoes'];
    $ObjetoWS->retornarLocalizadores = (boolean) $this->post['retornarLocalizadores'];
    $ObjetoWS->retornarMetas = (boolean) $this->post['retornarMetas'];
    $ObjetoWS->retornarRegionalizacoes = (boolean) $this->post['retornarRegionalizacoes'];
    $ObjetoWS->retornarPlanosOrcamentarios = (boolean) $this->post['retornarPlanosOrcamentarios'];
    $ObjetoWS->retornarAgendaSam = (boolean) $this->post['retornarAgendaSam'];
    $ObjetoWS->retornarMedidasInstitucionaisNormativas = (boolean) $this->post['retornarMedidasInstitucionaisNormativas'];
    $ObjetoWS->dataHoraReferencia = ''; // dateTime
    return $ObjetoWS;
  }

  /**
   * Configura a requisição de forma a obter apenas as tabelas de apoios selecionadas
   * na interface web.
   * @return \obterTabelasApoio
   */
  private function getObterTabelasApoio() {
    $ObjetoWS = new obterTabelasApoio();
    $ObjetoWS->credencial = $this->getCredencial(); // credencialDTO
    $ObjetoWS->exercicio = $_SESSION['exercicio']; // int
    $ObjetoWS->retornarBasesGeograficas = (boolean)$this->post['retornarBasesGeograficas']; // boolean
    $ObjetoWS->retornarEsferas = (boolean)$this->post['retornarEsferas']; // boolean
    $ObjetoWS->retonarFuncoes = (boolean)$this->post['retonarFuncoes']; // boolean
    $ObjetoWS->retornarMacroDesafios = (boolean)$this->post['retornarMacroDesafios']; // boolean
    $ObjetoWS->retornarMomentos = (boolean)$this->post['retornarMomentos']; // boolean
    $ObjetoWS->retornarPerfis = (boolean)$this->post['retornarPerfis']; // boolean
    $ObjetoWS->retornarPeriodicidades = (boolean)$this->post['retornarUnidadesMedidaIndicador']; // boolean
    $ObjetoWS->retornarProdutos = (boolean)$this->post['retornarProdutos']; // boolean
    $ObjetoWS->retornarRegioes = (boolean)$this->post['retornarRegioes']; // boolean
    $ObjetoWS->retornarSubFuncoes = (boolean)$this->post['retornarSubFuncoes']; // boolean
    $ObjetoWS->retornarTiposAcao = (boolean)$this->post['retornarTiposAcao']; // boolean
    $ObjetoWS->retornarTiposInclusao = (boolean)$this->post['retornarTiposInclusao']; // boolean
    $ObjetoWS->retornarTiposPrograma = (boolean)$this->post['retornarTiposPrograma']; // boolean
    $ObjetoWS->retornarUnidadesMedida = (boolean) $this->post['retornarUnidadesMedida']; // boolean
    $ObjetoWS->retornarUnidadesMedidaIndicador = (boolean)$this->post['retornarUnidadesMedidaIndicador']; // boolean
    $ObjetoWS->dataHoraReferencia = false; // dateTime
    return $ObjetoWS;
  }

  /**
   * Prepara parâmetros de entrada para as chamadas a obterAcoesPorIniciativa, obterAcoesProPrograma
   * e obterAcoesPoIdentificadorUnico.
   * @return \obterAcoesPorIniciativa|\obterAcoesProPrograma|\obterAcoesPoIdentificadorUnico
   * @see SiopQualitativo::__call
   */
  private function getObterAcoes() {
    $filtros = $this->post['filtro'];
    switch ($this->post['buscarAcaoPor']) {
      case 'identunico':
        $oWs = new obterAcaoPorIdentificadorUnico();
        $oWs->identificadorUnico = $filtros['identificadorUnico'];
        break;
      case 'programa':
        $oWs = new obterAcoesPorPrograma();
        $oWs->codigoPrograma = $filtros['codigoPrograma'];
        break;
      case 'iniciativa':
        $oWs = new obterAcoesPorIniciativa();
        if (!empty($filtros['codigoPrograma'])) {
          $oWs->codigoPrograma = $filtros['codigoPrograma'];
        }
        if (!empty($filtros['codigoObjetivo'])) {
          $oWs->codigoObjetivo = $filtros['codigoObjetivo'];
        }
        if (!empty($filtros['codigoIniciativa'])) {
          $oWs->codigoIniciativa = $filtros['codigoIniciativa'];
        }
        break;
    }
    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;

    return $oWs;
  }

  /**
   * Prepara parâmetros de entrada para as chamadas a obterOrgao, obterOrgaoPorCodigoSiorg
   * e obterAcoesPoIdentificadorUnico.
   * @return \obterOrgao|\obterOrgaoPorCodigoSiorg
   * @see SiopQualitativo::__call
   */
  private function getObterOrgao() {
    $filtros = $this->post['filtro'];
    switch ($this->post['buscarOrgaoPor']) {
      case 'codtipo':
        $oWs = new obterOrgao();
        if (!empty($filtros['codigoOrgao'])) {
          $oWs->codigoOrgao = $filtros['codigoOrgao'];
        }
        if (!empty($filtros['tipoOrgao'])) {
          $oWs->tipoOrgao = $filtros['tipoOrgao'];
        }
        break;
      case 'siorg':
        $oWs = new obterOrgaoPorCodigoSiorg();
        if (!empty($filtros['codigoSiorg'])) {
          $oWs->codigoSiorg = $filtros['codigoSiorg'];
        }
        break;
    }
    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;

    return $oWs;
  }

  public function getObterProgramas() {
    $filtros = $this->post['filtro'];
    $oWs = new obterProgramasPorOrgao();
    if (!empty($filtros['codigoOrgao'])) {
      $oWs->codigoOrgao = $filtros['codigoOrgao'];
    }
    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;

    return $oWs;
  }

  public function getObterObjetivos() {
    $filtros = $this->post['filtro'];
    $oWs = new obterObjetivosPorPrograma();
    if (!empty($filtros['codigoPrograma'])) {
      $oWs->codigoPrograma = $filtros['codigoPrograma'];
    }
    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;

    return $oWs;
  }

  public function getObterMetas() {
    $filtros = $this->post['filtro'];
    $oWs = new obterMetasPorObjetivo();
    if (!empty($filtros['codigoPrograma'])) {
      $oWs->codigoPrograma = $filtros['codigoPrograma'];
    }
    if (!empty($filtros['codigoObjetivo'])) {
      $oWs->codigoObjetivo = $filtros['codigoObjetivo'];
    }
    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;

    return $oWs;
  }

  public function getObterIniciativas() {
    $filtros = $this->post['filtro'];
    $oWs = new obterIniciativasPorObjetivo();
    if (!empty($filtros['codigoPrograma'])) {
      $oWs->codigoPrograma = $filtros['codigoPrograma'];
    }
    if (!empty($filtros['codigoObjetivo'])) {
      $oWs->codigoObjetivo = $filtros['codigoObjetivo'];
    }
    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;

    return $oWs;
  }

  public function getObterLocalizadores() {
    $filtros = $this->post['filtro'];
    $oWs = new obterLocalizadoresPorAcao();
    if (!empty($filtros['identificadorUnicoAcao'])) {
      $oWs->identificadorUnicoAcao = $filtros['identificadorUnicoAcao'];
    }

    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;
    return $oWs;
  }

  public function getObterRegionalizacoes() {
    $filtros = $this->post['filtro'];
    $oWs = new obterRegionalizacoesPorMeta();
    if (!empty($filtros['codigoPrograma'])) {
      $oWs->codigoPrograma = $filtros['codigoPrograma'];
    }
    if (!empty($filtros['codigoObjetivo'])) {
      $oWs->codigoObjetivo = $filtros['codigoObjetivo'];
    }
    if (!empty($filtros['codigoMeta'])) {
      $oWs->codigoMeta = $filtros['codigoMeta'];
    }

    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;
    return $oWs;
  }

  public function getObterUnidadesOrcamentarias() {
    $filtros = $this->post['filtro'];
    $oWs = new obterUnidadesOrcamentariasOrgao();
    if (!empty($filtros['codigoOrgao'])) {
      $oWs->codigoOrgao = $filtros['codigoOrgao'];
    }
    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;

    return $oWs;
  }

  public function getObterIndicadores() {
    $filtros = $this->post['filtro'];
    $oWs = new obterIndicadoresPorPrograma();
    if (!empty($filtros['codigoPrograma'])) {
      $oWs->codigoPrograma = $filtros['codigoPrograma'];
    }
    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;

    return $oWs;
  }

  public function getObterMomentos() {
    $oWs = new obterMomentoCarga();
    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    //$oWs->codigoMomento = $this->codigomomento;

    return $oWs;
  }

  public function getObterPlanosOrcamentarios() {
    $filtros = $this->post['filtro'];
    $oWs = new obterPlanosOrcamentariosPorAcao();
    if (!empty($filtros['identificadorUnicoAcao'])) {
      $oWs->identificadorUnicoAcao = $filtros['identificadorUnicoAcao'];
    }

    $oWs->credencial = $this->getCredencial();
    $oWs->exercicio = $_SESSION['exercicio'];
    $oWs->codigoMomento = $this->codigomomento;
    return $oWs;
  }

}
