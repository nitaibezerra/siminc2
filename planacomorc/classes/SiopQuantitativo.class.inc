<?php
/**
 * PLANACOMORC
 * base: Programação Orçamentária
 * $Id: SiopQuantitativo.class.inc 93158 2015-01-27 12:04:31Z maykelbraz $
 */

include_once APPRAIZ . "planacomorc/classes/WSQuantitativo.class.inc";

class SiopQuantitativo {
  private $_urlWsdl;
  private $_callOptions = array();
  private $wsusuario;
  private $wssenha;
  private $db;
  private $post;
  private $acaid;
  private $acacod;
  private $unicod;
  private $valorfisico;
  private $codigomomento;

  private $_overloadedMethods = array(
      'consultarAcompanhamentoOrcamentario',
      'consultarExecucaoOrcamentaria',
      'obterTabelasApoioQuantitativo',
      'obterAcoesDisponiveisAcompanhamentoOrcamentario');

  public function __construct($arrParam = array()) {
    global $db;
    if (('simec_desenvolvimento' == $_SESSION['baselogin']) || ('simec_espelho_producao' == $_SESSION['baselogin'])) { // -- dev
        if (isset($arrParam['post']['webservice']) && (2 == $arrParam['post']['webservice'])) { // -- antigo
            $this->_urlWsdl = 'https://homologacao.siop.planejamento.gov.br/services/WSQuantitativo?wsdl';
            $this->_callOptions['local_cert'] = APPRAIZ . "planacomorc/modulos/sistema/comunica/simec_old.pem";
        } else { // -- novo webservice 10/06/2013
            $this->_urlWsdl = 'https://testews.siop.gov.br/services/WSQuantitativo?wsdl';
            $this->_callOptions['local_cert'] = APPRAIZ . "planacomorc/modulos/sistema/comunica/simec.pem";
        }
    } else { // -- producao
        $this->_urlWsdl = 'https://webservice.siop.gov.br/services/WSQuantitativo?wsdl';
        if (isset($arrParam['post']['webservice']) && (2 == $arrParam['post']['webservice'])) { // -- antigo
            $this->_callOptions['local_cert'] = APPRAIZ . "planacomorc/modulos/sistema/comunica/simec.pem";
        } else { // -- novo webservice 10/06/2013
            $this->_callOptions['local_cert'] = APPRAIZ . "planacomorc/modulos/sistema/comunica/simec.pem";
        }
    }

    $this->_callOptions['passphrase'] = "simec";
    $this->_callOptions['exceptions'] = true;
    $this->_callOptions['trace'] = true;
    $this->_callOptions['encoding'] = 'ISO-8859-1';

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

  /**
   * Sobrecarga dos métodos de consulta ao webservice. Os métodos executam um
   * procedimento padrão e comum.
   * @param string $nome Nome do método invocado;
   * @param array $parametros Parâmetros enviados na chamada.
   * @return mixed
   */
  public function __call($nome, $parametros) {
    if (!in_array($nome, $this->_overloadedMethods)) {
      trigger_error("Função '{$nome}' não está definida em SiopQuantitativo nem em SiopQuantitativo::_overloadedMethods.", E_USER_ERROR);
    }
    $nome = ucfirst($nome);
    $get = "get{$nome}";
    $oEnvio = $this->$get($parametros[0]);
    if (!is_object($oEnvio)) {
      trigger_error("O método 'get{$nome}' deve retornar um objeto.");
    }
    $wsQualitativo = new WSQuantitativo($this->_urlWsdl, $this->_callOptions);
    $classObter = get_class($oEnvio);


    return $wsQualitativo->$classObter($oEnvio);
  }

    public function cadastrarAcompanhamento()
    {
        global $db;

        $arrAcompanhamento = $this->carregaDadosAcompanhamento($this->post['acaid_cadastrarAcompanhamento']);
        if ($arrAcompanhamento) {
            $unicod = array();
            $arrRetorno = array();
            $count = 0;

            foreach ($arrAcompanhamento as $valor) {
                $this->acacod = $valor['acacod'];
                $this->unicod = $valor['unicod'];
                $obEnvio = $this->getCadastrarAcompanhamentoOrcamentario($valor);
                $sql = "INSERT INTO elabrev.ws_log(url, metodo, id_acao_programatica, wslusucpf)
                            VALUES ('" . $this->_urlWsdl . "', 'wsquantitativo.cadastrarAcompanhamentoOrcamentario', {$valor['acaid']}, '{$_SESSION['usucpf']}')
                            RETURNING wslid";

                $idLog = $db->pegaUm($sql);
                $db->commit();
                $this->valorfisico = 0;

                $wsQuantitativo = new WSQuantitativo(
                    $this->_urlWsdl,
                    $this->_callOptions
                );
                try {
                    $retorno = $wsQuantitativo->cadastrarAcompanhamentoOrcamentario($obEnvio, $valor);
                    $resultado = 'SUCESSO';
                    $mensagemAdicional = '';
                } catch (Exception $e) {
                    $resultado = 'ERROR';
                    $mensagemAdicional = str_replace("'", "''", $e->getMessage());
                }

                // -- Gravando log local da app
                $lwsrequestcontent = str_replace("'", "''", $wsQuantitativo->__getLastRequest());
                $lwsrequestheader = str_replace("'", "''", $wsQuantitativo->__getLastRequestHeaders());
                $lwsresponsecontent = str_replace("'", "''", $wsQuantitativo->__getLastResponse());
                $lwsresponseheader = str_replace("'", "''", $wsQuantitativo->__getLastResponseHeaders());

                $sql = "UPDATE elabrev.ws_log
                          SET wslretorno = '{$resultado}',
                              wslrequisicaoxml = '{$lwsrequestcontent}',
                              wslrequisicaoheader = '{$lwsrequestheader}',
                              wslrespostaxml = '{$lwsresponsecontent}',
                              wslrespostaheader = '{$lwsresponseheader}'
                          WHERE wslid = {$idLog}";
                $db->executar($sql);

                // -- Gravando o log completo em spo.logws
                $erro = 'SUCESSO' == $resultado? 't': 'f';
                $mensagemAdicional = "{$valor['acaid']}||{$mensagemAdicional}";
                $sql2 = <<<DML
INSERT INTO spo.logws(lwsrequesttimestamp, lwsurl, lwsmetodo, lwserro, lwsrequestcontent, lwsrequestheader, lwsresponsecontent, lwsresponseheader, lwsresponsetimestamp, lwsobservacao)
  VALUES(
    (SELECT datacriacao FROM elabrev.ws_log WHERE wslid = {$idLog}),
    '{$this->_urlWsdl}',
    'cadastrarAcompanhamentoOrcamentario',
    '{$erro}',
    '{$lwsrequestcontent}',
    '{$lwsrequestheader}',
    '{$lwsresponsecontent}',
    '{$lwsresponseheader}',
    now(),
    '{$mensagemAdicional}'
  )
DML;
                $db->executar($sql2);
                $db->commit();

                $wslmsgretorno = '';
                $sucesso = true;
                if (!$retorno->return->sucesso) {
                    $sucesso = false;

                    if (is_array($retorno->return->mensagensErro)) {
                        $wslmsgretorno .= 'Falha no envio. Motivo(s):<br /> ' . implode('<br />', $retorno->return->mensagensErro) . '<br />';
                    } else {
                        $wslmsgretorno .= 'Falha no envio. Motivo(s):<br /> ' . $retorno->return->mensagensErro . '<br />';
                    }
                } else {
                    if (is_object($retorno->return->alertas)) {
                        if (is_array($retorno->return->alertas->alerta)) {
                            $wslmsgretorno .= implode('<br />', $retorno->return->alertas->alerta) . '<br />';
                        } else {
                            $wslmsgretorno .= $retorno->return->alertas->alerta . '<br />';
                        }
                    }
                    if (is_object($retorno->return->pendencias)) {
                        if (is_array($retorno->return->pendencias->pendencia)) {
                            $wslmsgretorno .= implode('<br />', $retorno->return->pendencias->pendencia) . '<br />';
                        } else {
                            $wslmsgretorno .= $retorno->return->pendencias->pendencia . '<br />';
                        }
                    }
                }
                if ($retorno->return->sucesso && empty($wslmsgretorno)) {
                    $wslmsgretorno .= 'Enviado com sucesso';
                }
                if ($wslmsgretorno) {
                    $wslmsgretorno = str_replace("'", "''", $wslmsgretorno);
                    $sql = "UPDATE elabrev.ws_log
                              SET wslmsgretorno = '{$wslmsgretorno}'
                              WHERE wslid = {$idLog}";
                    $db->executar($sql);
                    $db->commit();
                }
                // -- Atualizando o status dos acompanhamentos de acordo com o retorno da execução
                    $sucesso = $sucesso?'S':'E';
                    $sql2 = <<<DML
UPDATE planacomorc.acompanhamento_acao
  SET ultimoretornosiop = '{$sucesso}'
  WHERE id_acao_programatica = {$valor['acaid']}
    AND id_periodo_referencia = {$valor['id_periodo_referencia']}
DML;
                    $db->executar($sql2);
                    $db->commit();

                $obEnvio = '';
                array_push($arrRetorno, array(
                    'acacod' => $valor['acacod'],
                    'unicod' => $valor['unicod'],
                    'acaid' => $valor['acaid'],
                    'funcao' => $valor['funcao'],
                    'subfuncao' => $valor['subfuncao'],
                    'esfera' => $valor['esfera'],
                    'prgcod' => $valor['prgcod'],
                    'tipoinclusao' => $valor['tipoinclusaoacao'],
                    'retorno' => $retorno)
                );
                array_push( $unicod, $valor['unicod'] );
                $count++;
            }
            return $arrRetorno;
        } else {
            return 'erro';
        }
    }

  /**
   *
   * @param string $posfixo O posfixo serve para diferenciar a chave 'saida' e 'filtro' do $_POST.
   * @return \consultarExecucaoOrcamentaria
   */
  public function getConsultarExecucaoOrcamentaria($posfixo) {
    // -- Indicando se um retorno foi selecionado ou não pelo usuário
    $oSelRetorno = new selecaoRetornoExecucaoOrcamentariaDTO();
    foreach (get_object_vars($oSelRetorno) as $prop => $valor) {
      $oSelRetorno->$prop = isset($_POST["saida{$posfixo}"][$prop]);
    }

    // -- Coletando os filtros selecionados pelo usuário em forma de lista
    $oFiltro = new filtroExecucaoOrcamentariaDTO();
    foreach (get_object_vars($oFiltro) as $prop => $valor) {
      if ('anoReferencia' != $prop && 'mes' != $prop && isset($this->post["filtro{$posfixo}"][$prop])
            && !empty($this->post["filtro{$posfixo}"][$prop])) {
        $oFiltro->$prop = array_map("trim", explode(PHP_EOL, $this->post["filtro{$posfixo}"][$prop]));
      }
    }

    // -- Filtros escalares
    $oFiltro->anoReferencia = $this->post["filtro"]['anoReferencia'];

    // -- credenciais
    if (1 == $this->post['execucao_mensal']) {
        $oWsParam = new consultarExecucaoOrcamentariaMensal();
    } else {
        $oWsParam = new consultarExecucaoOrcamentaria();
    }

    if (isset($this->post["filtro{$posfixo}"]['mes']) && !empty($this->post["filtro{$posfixo}"]['mes'])) {
      $oWsParam->mes = $this->post["filtro{$posfixo}"]['mes'];
    }
    $oWsParam->filtro = $oFiltro;
    $oWsParam->credencial = $this->getCredencial();
    $oWsParam->selecaoRetorno = $oSelRetorno;

    $oPaginacao = new paginacaoDTO();
    $oPaginacao->registrosPorPagina = 2000;
    $oPaginacao->pagina = $this->post['pagina']?(int)$this->post['pagina']:1;
    $oWsParam->paginacao = $oPaginacao;

    if ($_POST['send-email']) {
        ob_start();
        echo '<pre>';
        var_dump($oWsParam);
        enviar_email(array('nome' => "Gustavo 'Raj' Ávila", 'email' => 'gustavo.avila@mec.gov.br'),
                array('maykel.braz@mec.gov.br', 'gustavo.avila@mec.gov.br'), 'Requisição à SOF', ob_get_contents());
        ob_end_clean();
    }

    return $oWsParam;
  }

  private function getConsultarAcompanhamentoOrcamentario() {
/*
    $ObjetoWS->codigoTipoInclusaoAcao = ($arrAcompanhamento['tipoinclusaoacao'] ? $arrAcompanhamento['tipoinclusaoacao'] : '1');
    $ObjetoWS->codigoTipoInclusaoLocalizador = ($arrAcompanhamento['tipoinclusaolocalizador'] ? $arrAcompanhamento['tipoinclusaolocalizador'] : '1');
*/
    $oFiltro = new filtroFuncionalProgramaticaDTO();
    foreach (get_object_vars($oFiltro) as $prop => $valor) {
      if (isset($this->post['filtro'][$prop]) && !empty($this->post['filtro'][$prop])) {
        $oFiltro->$prop = $this->post['filtro'][$prop];
      }
    }

    // -- credenciais
    $oWsParam = new consultarAcompanhamentoOrcamentario();
    $oWsParam->filtro = $oFiltro;
    $oWsParam->exercicio = $_SESSION['exercicio'];
    $oWsParam->credencial = $this->getCredencial();

    return $oWsParam;
  }

    private function getObterAcoesDisponiveisAcompanhamentoOrcamentario()
    {
        $oWsParam = new obterAcoesDisponiveisAcompanhamentoOrcamentario();
        $oWsParam->credencial = $this->getCredencial();
        $oWsParam->exercicio = $_SESSION['exercicio'];
        $oWsParam->periodo = 1;

        return $oWsParam;
    }

  public function cadastrarProposta() {
    $arrProposta = $this->carregarDadosProposta($this->post['acaid_cadastra']);
    if ($arrProposta) {
      $acaid = array();
      $arrRetorno = array();
      foreach ($arrProposta as $valor) {
        if (!in_array($valor['acaid'], $acaid)) {
          $this->acaid = $valor['acaid'];
          $obEnvio = $this->getCadastrarProposta($valor, $arrProposta);
          $this->valorfisico = 0;
          $wsQuantitativo = new WSQuantitativo($this->_urlWsdl, $this->_callOptions);
          $retorno = $wsQuantitativo->cadastrarProposta($obEnvio, $valor);
          $obEnvio = '';
          array_push($arrRetorno, array('acaid' => $valor['acaid'], 'retorno' => $retorno));
          array_push($acaid, $valor['acaid']);
        }
      }
      return $arrRetorno;
    } else {
      return 'erro';
    }
  }

  public function consultarProposta() {
    $arrProposta = $this->carregarDadosProposta($this->post['acaid_consulta']);
    if ($arrProposta) {
      $acaid = array();
      $arrRetorno = array();
      foreach ($arrProposta as $valor) {
        var_dump($valor['acaid']);
        if (!in_array($valor['acaid'], $acaid)) {
          $this->acaid = $valor['acaid'];
          $obEnvio = $this->getConsultarProposta($valor, $arrProposta);
          $this->valorfisico = 0;
          $wsQuantitativo = new WSQuantitativo($this->_urlWsdl, $this->_callOptions);
          $retorno = $wsQuantitativo->consultarProposta($obEnvio, $valor);
          array_push($arrRetorno, array('acaid' => $valor['acaid'], 'retorno' => $retorno));
          array_push($acaid, $valor['acaid']);
        }
      }
      return $arrRetorno;
    } else {
      return 'erro';
    }
  }

  public function excluirProposta() {
    $arrProposta = $this->carregarDadosProposta($this->post['acaid_exclui']);
    if ($arrProposta) {
      $acaid = array();
      $arrRetorno = array();
      foreach ($arrProposta as $valor) {
        if (!in_array($valor['acaid'], $acaid)) {
          $this->acaid = $valor['acaid'];
          $obEnvio = $this->getExcluirProposta($valor, $arrProposta);
          $this->valorfisico = 0;
          $wsQuantitativo = new WSQuantitativo($this->_urlWsdl, $this->_callOptions);
          $retorno = $wsQuantitativo->excluirProposta($obEnvio, $valor);
          array_push($arrRetorno, array('acaid' => $valor['acaid'], 'retorno' => $retorno));
          array_push($acaid, $valor['acaid']);
        }
      }
      return $arrRetorno;
    } else {
      return 'erro';
    }
  }

  public function obterExecucaoOrcamentariaSam() {
    $obEnvio = $this->getObterExecucaoOrcamentariaSam();
    $wsQuantitativo = new WSQuantitativo($this->_urlWsdl, $this->_callOptions);
    $retorno = $wsQuantitativo->obterExecucaoOrcamentariaSam($obEnvio);
    return $retorno;
  }

  public function obterProgramacaoCompletaQuantitativo() {
    $obEnvio = $this->getObterProgramacaoCompletaQuantitativo();
    $wsQuantitativo = new WSQuantitativo($this->_urlWsdl, $this->_callOptions);
    $retorno = $wsQuantitativo->obterProgramacaoCompletaQuantitativo($obEnvio);
    return $retorno;
  }


  private function getfiltroFuncionalProgramaticaDTO($arrAcompanhamento) {

    $ObjetoWS = new filtroFuncionalProgramaticaDTO();
    $ObjetoWS->codigoAcao = $arrAcompanhamento['acacod'];
    $ObjetoWS->codigoEsfera = $arrAcompanhamento['esfera'];
    $ObjetoWS->codigoFuncao = $arrAcompanhamento['funcao'];
    $ObjetoWS->codigoLocalizador = $arrAcompanhamento['loccod'];
    $ObjetoWS->codigoPrograma = $arrAcompanhamento['prgcod'];
    $ObjetoWS->codigoSubFuncao = $arrAcompanhamento['subfuncao'];
    $ObjetoWS->codigoTipoInclusaoAcao = ($arrAcompanhamento['tipoinclusaoacao'] ? $arrAcompanhamento['tipoinclusaoacao'] : '1');
    $ObjetoWS->codigoTipoInclusaoLocalizador = ($arrAcompanhamento['tipoinclusaolocalizador'] ? $arrAcompanhamento['tipoinclusaolocalizador'] : '1');
    $ObjetoWS->codigoUO = $arrAcompanhamento['unicod'];

    return $ObjetoWS;
  }

  private function getCredencial() {

    $ObjetoWS = new credencialDTO();
    $ObjetoWS->perfil = 32;
    $ObjetoWS->senha = md5($this->wssenha);
    $ObjetoWS->usuario = $this->wsusuario;

    return $ObjetoWS;
  }

	private function getCadastrarAcompanhamentoOrcamentario($arrAcompanhamento = array())
    {
		$ObjetoWS = new cadastrarAcompanhamentoOrcamentario();
		$ObjetoWS->acompanhamentoAcao = $this->getAcompanhamentoOrcamentarioAcaoDTO( $arrAcompanhamento );
		$ObjetoWS->credencial = $this->getCredencial();

		return $ObjetoWS;
	}

    private function getAcompanhamentoOrcamentarioAcaoDTO($arrAcompanhamento = array())
    {
        global $db;

        // -- Encontrando o período atual para envio dos dados do acompanhamento
        $query = <<<DML
SELECT MAX(prf.id_periodo_referencia) AS periodo_atual
  FROM planacomorc.periodo_referencia prf
  WHERE prf.id_exercicio = '{$_SESSION['exercicio']}'
DML;
        $periodoAtual = $db->pegaUm($query);
        $mesAtual = (int)date('n');

		$ObjetoWS = new acompanhamentoOrcamentarioAcaoDTO();
		$ObjetoWS->periodoOrdem = $mesAtual >= 4 && $mesAtual <= 9?1:2;
		$ObjetoWS->exercicio = $_SESSION['exercicio'];
		$ObjetoWS->codigoMomento = $this->codigomomento;
		$ObjetoWS->esfera = $arrAcompanhamento['esfera'];
		$ObjetoWS->unidadeOrcamentaria = $arrAcompanhamento['unicod'];
		$ObjetoWS->funcao = $arrAcompanhamento['funcao'];
		$ObjetoWS->subFuncao = $arrAcompanhamento['subfuncao'];
		$ObjetoWS->programa = $arrAcompanhamento['prgcod'];
		$ObjetoWS->acao = $arrAcompanhamento['acacod'];
		$ObjetoWS->codigoTipoInclusaoAcao = ($arrAcompanhamento['tipoinclusaoacao']
            ?$arrAcompanhamento['tipoinclusaoacao']
            :'1'
        );
		$ObjetoWS->snPendencia = '';
		$ObjetoWS->dataHoraAlteracao = '';
		$ObjetoWS->acompanhamentosLocalizadores	= $this->getAcompanhamentoOrcamentarioLocalizadorDTO(
            $arrAcompanhamento['acacod'],
            $arrAcompanhamento['unicod'],
            $periodoAtual
        );
		return $ObjetoWS;
	}

    private function getAcompanhamentoOrcamentarioLocalizadorDTO($acacod, $unicod, $periodoOrdem)
    {
        $sql = <<<DML
SELECT DISTINCT prf.titulo AS periodo,
                acc.executado_fisico AS realizado,
                TO_CHAR(acc.data_apuracao, 'YYYY-MM-DD') AS dataapuracao,
                acc.executado_rap_fisico AS realizado_rap,
                prg.codigo AS prgcod,
                lpr.loccod,
                apr.id_acao_programatica AS acaid,
                (SELECT aca.codtipoinclusaolocalizador
                   FROM monitora.acao aca
                   WHERE aca.acacod = a.codigo
                     AND aca.unicod = org.codigo
                     AND aca.loccod = lpr.loccod
                     AND aca.prgcod = prg.codigo
                     AND aca.prgano::integer = apr.id_exercicio
                   LIMIT 1) as tipoinclusaolocalizador,
                MAX(qsf.quantidade_fisico) AS meta,
                acc.reprogramado_fisico AS previsto,
                acc.reprogramado_financeiro AS limite,
                prf.id_periodo_referencia,
                lpr.id_localizador_programatica
  FROM planacomorc.acao a
    INNER JOIN planacomorc.acao_programatica apr ON apr.id_acao = a.id_acao
    INNER JOIN planacomorc.orgao org USING(id_orgao)
    INNER JOIN planacomorc.localizador_programatica lpr USING(id_acao_programatica)
    INNER JOIN planacomorc.quantitativo_sof qsf USING(id_acao_programatica, loccod)
    INNER JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = a.id_acao
      AND dae.id_exercicio = apr.id_exercicio
    INNER JOIN planacomorc.acompanhamento_acao acc USING(id_acao_programatica, id_localizador)
    INNER JOIN workflow.documento wfd USING(docid)
    INNER JOIN planacomorc.periodo_referencia prf USING(id_periodo_referencia)
    LEFT JOIN planacomorc.programa prg USING(id_programa)
    LEFT JOIN planacomorc.produto prd USING(id_produto)
    LEFT JOIN planacomorc.unidade_medida u ON u.id_unidade_medida = dae.id_unidade_medida
  WHERE apr.id_exercicio = {$_SESSION['exercicio']}
    AND esdid = 753
    AND org.codigo = '{$unicod}'
    AND a.codigo = '{$acacod}'
    AND qsf.quantidade_fisico != 0
  GROUP BY prf.titulo,
           acc.executado_fisico,
           acc.data_apuracao,
           acc.executado_rap_fisico,
           prg.codigo,
           lpr.loccod,
           apr.id_acao_programatica,
           lpr.loccodigotipoinclusao,
           acc.reprogramado_fisico,
           acc.reprogramado_financeiro,
           prf.id_periodo_referencia,
           lpr.id_localizador_programatica,
           a.codigo,
           org.codigo,
           apr.id_exercicio
DML;

        $arrAvaliacao = $this->db->carregar($sql);
		$arrAvaliacao = $arrAvaliacao ? $arrAvaliacao : array();

		foreach ($arrAvaliacao as $key => $v) {
			$avaliacao = strip_tags(htmlspecialchars_decode($v['texto'], ENT_QUOTES));
	      	$avaliacao = html_entity_decode($avaliacao);
	      	$arrTrans = array("&ndash;" => "-", "&nbsp;" => "");
	      	$avaliacao = strtr($avaliacao, $arrTrans);
			$$key = new acompanhamentoOrcamentarioLocalizadorDTO();
			$$key->localizador = $v['loccod'];
			$$key->codigoTipoInclusaoLocalizador = ($v['tipoinclusaolocalizador']?$v['tipoinclusaolocalizador']:'1');
			$$key->meta = $v['meta'];
			$$key->reprogramado = $v['previsto'];
			$$key->realizadoLOA = $v['realizado'];
			$$key->realizadoRAP = $v['realizado_rap'];
			$$key->dataApuracaoLOA = $$key->dataApuracaoRAP = $v['dataapuracao'];
			$$key->limite = $v['limite'];
            $$key->analisesLocalizador = $this->getAnaliseLocalizadorDTO(
                $v['acaid'],
                $v['loccod'],
                $periodoOrdem
            );
            $$key->acompanhamentosPlanoOrcamentario = $this->getAcompanhamentosPlanoOrcamentario(
                $v['id_localizador_programatica'],
                $v['dataapuracao']
            );
			$ObjetoWS[$key] = $$key;
		}
		return $ObjetoWS;
	}

    private function getAnaliseLocalizadorDTO($idAcaoProgramatica, $codLocalizador, $periodoOrdem)
    {
        $sql = <<<DML
SELECT mnq.qstid,
       mqr.mqrresposta AS resposta,
       mqg.mqpdescricao AS pergunta
  FROM planacomorc.monquestionario mnq
    INNER JOIN planacomorc.monqstacaperiodo mqp USING(qstid)
    INNER JOIN planacomorc.monqstperguntas mqg USING(qstid)
    INNER JOIN planacomorc.localizador_programatica lpr USING(id_acao_programatica)
    INNER JOIN planacomorc.acompanhamento_acao acc USING(id_acao_programatica, id_localizador, id_periodo_referencia)
    LEFT JOIN planacomorc.monquestionarioresposta mqr USING(mqpid, id_acompanhamento_acao)
  WHERE mqp.id_acao_programatica = {$idAcaoProgramatica}
    AND lpr.loccod = '{$codLocalizador}'
    AND mnq.id_periodo_referencia = {$periodoOrdem}
    AND mnq.qststatus = 'A'
DML;

        // -- Concatenando as respostas dadas ao questionário
        $questionario = array();
        $dadosdb = $this->db->carregar($sql);
        foreach (($dadosdb?$dadosdb:array()) as $resposta) {
            $questionario[$resposta['qstid']][] = trim($resposta['pergunta']) . ' ' . trim($resposta['resposta']);
        }
        $listaAnalises = array();
        foreach ($questionario as $respostas) {
            $analise = new AnaliseAcompanhamentoOrcamentarioDTO();
            $analise->periodoOrdem = $periodoOrdem;
            $analise->analise = substr(implode(PHP_EOL, $respostas), 0, 5000);
            $listaAnalises[] = $analise;
        }
        return $listaAnalises;
    }

    /**
     * Cria a lista de \acompanhamentoPlanoOrcamentarioDTO de um localizador.
     *
     * @global cls_banco $db Conexão com o banco de dados.
     * @param int $idLocalizadorProgramatica Identificador do localizador programático.
     * @param string $dataApuracao Data em que aconteceu a apuração do dado de meta
     * @return array Lista de \acompanhamentoPlanoOrcamentarioDTO
     */
    public function getAcompanhamentosPlanoOrcamentario($idLocalizadorProgramatica, $dataApuracao)
    {
        global $db;

        $sqlPO = <<<DML
SELECT plo.codigo AS planoOrcamentario,
       apo.realizado_loa AS realizadoLOA
  FROM planacomorc.acompanhamento_po apo
    INNER JOIN planacomorc.plano_orcamentario plo USING(id_plano_orcamentario)
    INNER JOIN planacomorc.snapshot_dotacao_po_programatica sdpp USING(id_localizador_programatica, plocod)
  WHERE apo.id_localizador_programatica = %d
    AND dotacao > 0
DML;
        $stmt = sprintf($sqlPO, $idLocalizadorProgramatica);

        $arAcompPO = array();
        if ($data = $db->carregar($stmt)) {
            foreach ($data as $acompPOData) {
//                if ($acompPOData['realizadoloa'] > 0) {
                $acompPO = new acompanhamentoPlanoOrcamentarioDTO();
                $acompPO->dataApuracaoLOA = $dataApuracao;
                $acompPO->planoOrcamentario = $acompPOData['planoorcamentario'];
                $acompPO->realizadoLOA = $acompPOData['realizadoloa'];
                $arAcompPO[] = $acompPO;
//                }
            }
        }
        return $arAcompPO;
    }

  private function getComentarioAcompanhamentoOrcamentarioDTO($acaid) {

    $sql = "select '' as codigo, avp.avptexto as comentario, us.usunome as nome, to_char(avp.avpdata, 'YYYY-MM-DD') as data
				from monitora.avaliacaoparecer avp
				    inner join monitora.referencia ref on ref.refcod = avp.refcod
				    left join seguranca.usuario us on us.usucpf = avp.usucpf
				where
				    avp.acaid = $acaid
				    and ref.refano_ref = '{$_SESSION['exercicio']}'
				    and ref.refcod = 263
				union
				select '' as codigo, ca.cmadescricao as comentario, us.usunome as nome, to_char(ca.cmadata, 'YYYY-MM-DD') as data
				from monitora.comentarioacao ca
				    inner join seguranca.usuario us on us.usucpf = ca.usucpf
				where ca.acaid = $acaid
					and ca.cmastatus = 'A'";

    $arrComentarios = $this->db->carregar($sql);
    $arrComentarios = $arrComentarios ? $arrComentarios : array();

    foreach ($arrComentarios as $key => $v) {
      $$key = new comentarioAcompanhamentoOrcamentarioDTO();
      $$key->comentario = $v['comentario'];
      $$key->comentarioId = $v['codigo'];
      $$key->nomeUsuario = $v['nome'];
      $$key->periodoOrdem = '2';
      $$key->ultimaModificacao = $v['data'];

      $ObjetoWS[$key] = $$key;
    }

    return $ObjetoWS;
  }

  private function getObterProgramacaoCompletaQuantitativo() {
    $ObjetoWS = new obterProgramacaoCompletaQuantitativo();
    $ObjetoWS->codigoMomento = $this->codigomomento;
    $ObjetoWS->credencial = $this->getCredencial();
    $ObjetoWS->dataHoraReferencia = '';
    $ObjetoWS->exercicio = $_SESSION['exercicio'];
    $ObjetoWS->paginacao = new paginacaoDTO();
    $ObjetoWS->paginacao->pagina = $_REQUEST['pagina']?$_REQUEST['pagina']:1;

    return $ObjetoWS;
  }

  private function getCadastrarProposta($proposta, $arrProposta) {

    $ObjetoWS = new cadastrarProposta();
    $ObjetoWS->credencial = $this->getCredencial();
    $ObjetoWS->proposta = $this->getPropostaDTO($proposta, $arrProposta);

    return $ObjetoWS;
  }

  private function getPropostaDTO($proposta, $arrProposta) {

    $total = $this->db->pegaUm("select count(ploid) from elabrev.planoorcamentario where ploproduto is not null and acaid = {$this->acaid}");
    $boMetas = ($total > 0 ? true : false);

    if ($boMetas) {
      $ObjetoWS = new propostaDTOMeta();
      $ObjetoWS->codigoAcao = $proposta['acacod']; // string
      $ObjetoWS->codigoEsfera = $proposta['esfcod']; // string
      $ObjetoWS->codigoFuncao = $proposta['funcod']; // string
      $ObjetoWS->codigoLocalizador = $proposta['loccod']; // string
      $ObjetoWS->codigoMomento = $this->codigomomento; // int
      $ObjetoWS->codigoOrgao = $proposta['unicod']; // string
      $ObjetoWS->codigoPrograma = $proposta['prgcod']; // string
      $ObjetoWS->codigoSubFuncao = $proposta['sfucod']; // string
      $ObjetoWS->codigoTipoDetalhamento = $proposta['tipodetalhamento']; // string
      $ObjetoWS->codigoTipoInclusaoAcao = '1'; // int
      $ObjetoWS->codigoTipoInclusaoLocalizador = '1'; // int
      $ObjetoWS->exercicio = $_SESSION['exercicio'] + 1; // int
      $ObjetoWS->financeiros = $this->getFinanceiroDTO($arrProposta); // financeiroDTO
      $ObjetoWS->justificativa = $proposta['justificativa'];
      $ObjetoWS->metaPlanoOrcamentario = $this->getMetaPlanoOrcamentarioDTO($proposta['acaid']); // metaPlanoOrcamentarioDTO
      $ObjetoWS->quantidadeFisico = $proposta['quantidadefisico']; // long
      //$ObjetoWS->receitas 						= $this->getReceitaDTO(); // receitaDTO
      $ObjetoWS->snAtual = '1'; // boolean
      $ObjetoWS->valorFisico = $this->valorfisico; // long
    } else {
      $ObjetoWS = new propostaDTO();
      $ObjetoWS->codigoAcao = $proposta['acacod']; // string
      $ObjetoWS->codigoEsfera = $proposta['esfcod']; // string
      $ObjetoWS->codigoFuncao = $proposta['funcod']; // string
      $ObjetoWS->codigoLocalizador = $proposta['loccod']; // string
      $ObjetoWS->codigoMomento = $this->codigomomento; // int
      $ObjetoWS->codigoOrgao = $proposta['unicod']; // string
      $ObjetoWS->codigoPrograma = $proposta['prgcod']; // string
      $ObjetoWS->codigoSubFuncao = $proposta['sfucod']; // string
      $ObjetoWS->codigoTipoDetalhamento = $proposta['tipodetalhamento']; // string
      $ObjetoWS->codigoTipoInclusaoAcao = '1'; // int
      $ObjetoWS->codigoTipoInclusaoLocalizador = '1'; // int
      $ObjetoWS->exercicio = $_SESSION['exercicio'] + 1; // int
      $ObjetoWS->financeiros = $this->getFinanceiroDTO($arrProposta); // financeiroDTO
      $ObjetoWS->justificativa = $proposta['justificativa'];
      //$ObjetoWS->metaPlanoOrcamentario 			= $this->getMetaPlanoOrcamentarioDTO( $proposta['acaid'] ); // metaPlanoOrcamentarioDTO
      $ObjetoWS->quantidadeFisico = $proposta['quantidadefisico']; // long
      //$ObjetoWS->receitas 						= $this->getReceitaDTO(); // receitaDTO
      $ObjetoWS->snAtual = '1'; // boolean
      $ObjetoWS->valorFisico = $this->valorfisico; // long
    }

    return $ObjetoWS;
  }

  private function getMetaPlanoOrcamentarioDTO($acaid) {


    $sql = "SELECT mp.mpoid, mp.ploid, po.ploidentificadorunicosiop, mp.mpovalor
					FROM elabrev.metaplanoorcamentario mp
					    inner join elabrev.planoorcamentario po on po.ploid = mp.ploid
					WHERE po.acaid = $acaid and mpostatus = 'A'
					    and po.plostatus = 'A'";

    $arrDados = $this->db->carregar($sql);
    $arrDados = $arrDados ? $arrDados : array();

    if ($arrDados) {
      foreach ($arrDados as $key => $v) {
        $$key = new metaPlanoOrcamentarioDTO();
        $$key->identificadorUnicoPlanoOrcamentario = $v['ploidentificadorunicosiop'];
        $$key->quantidadeFisico = $v['mpovalor'];

        $ObjetoWS[$key] = $$key;
      }
    }

    return $ObjetoWS;
  }

  private function getFinanceiroDTO($arrProposta) {
    $chave = 0;
    foreach ($arrProposta as $key => $v) {
      if ($v['acaid'] == $this->acaid) {
        if ($v['ploid']) {
          $sql = "select ploidentificadorunicosiop from elabrev.planoorcamentario pl where ploid = {$v['ploid']}";
          $ploidentificadorunicosiop = $this->db->pegaUm($sql);
        }
        //ver($ploidentificadorunicosiop,$sql,d);

        $$chave = new financeiroDTO();
        $$chave->fonte = $v['foncod'];
        $$chave->identificadorPlanoOrcamentario = $ploidentificadorunicosiop; //$v['ploidentificadorunicosiop'];
        $$chave->idOC = $v['idocod'];
        $$chave->idUso = $v['iducod'];
        $$chave->naturezaDespesa = ($v['ndpcod'] ? $v['ndpcod'] : $v['nrcod']);
        $$chave->resultadoPrimarioAtual = $v['iducod'];
        $$chave->resultadoPrimarioLei = $v['iducod'];
        $$chave->valor = $v['valor'];

        $this->valorfisico += $v['valor'];

        $ObjetoWS[$chave] = $$chave;
        $chave++;
      }
    }

    return $ObjetoWS;
  }

  private function getReceitaDTO() {

    $ObjetoWS = new receitaDTO();
    $ObjetoWS->naturezaReceita = '';
    $ObjetoWS->valor = '';

    return $ObjetoWS;
  }

  private function getConsultarProposta($proposta, $arrProposta) {

    $ObjetoWS = new consultarProposta();
    $ObjetoWS->credencial = $this->getCredencial();
    $ObjetoWS->proposta = $this->getPropostaDTO($proposta, $arrProposta);

    return $ObjetoWS;
  }

  private function getExcluirProposta($valor, $arrProposta) {

    $ObjetoWS = new excluirProposta();
    $ObjetoWS->credencial = $this->getCredencial();
    $ObjetoWS->proposta = $this->getPropostaDTO($valor, $arrProposta);

    return $ObjetoWS;
  }

  private function getObterTabelasApoioQuantitativo() {
    $ObjetoWS = new obterTabelasApoioQuantitativo();
    $ObjetoWS->credencial = $this->getCredencial();
    $ObjetoWS->dataHoraReferencia = '';
    $ObjetoWS->exercicio = $_SESSION['exercicio'];
    $ObjetoWS->retornarFontes = (boolean)$this->post['retornarFontes'];
    $ObjetoWS->retornarIdOcs = (boolean)$this->post['retornarIdOcs'];
    $ObjetoWS->retornarIdUsos = (boolean)$this->post['retornarIdUsos'];
    $ObjetoWS->retornarNaturezas = (boolean)$this->post['retornarNaturezas'];
    $ObjetoWS->retornarRPs = (boolean)$this->post['retornarRPs'];

    return $ObjetoWS;
  }

  private function getObterExecucaoOrcamentariaSam() {

    $ObjetoWS = new obterExecucaoOrcamentariaSam();
    $ObjetoWS->anoExercicio = $_SESSION['exercicio'] + 1;
    $ObjetoWS->credencial = $this->getCredencial();
    $ObjetoWS->planoInterno = '';

    return $ObjetoWS;
  }

  private function carregarDadosProposta($arrAcao = array()) {

    $sql = "select * from
					      (
					select  'EYX' as login,
					       MD5('COISUCA') as senha,
					       '26000' as orgao,
					       acao.acaid,
					       cast(acao.acaid as varchar(5))||case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' then '1' else cast(tda.tpdid as varchar(2)) end as codigo,
					       acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' then 1 else tda.tpdid end as tipodetalhamento,
					       Case
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) = '' then null
					        when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','0181','00H1') and substr(nd.ndpcod, 1,2)<>'31' and trim(coalesce(acadscunmsof,'')) = '' then null
					        else coalesce(acaqtdefisico,1) end as quantidadefisico,
					       z.valor as valorfisico,
					       justificativa as justificativa,
					       '2013' as ano,
					       da.iducod,
					       idoc.idocod,
					       substr(nd.ndpcod,1,6)||'00' as ndpcod,
					       da.foncod, da.ploid,
					       coalesce ( SUM(da.dpavalor), 0 ) as valor,

					       '' as nrcod,
					       0 as valor_receita
					       from elabrev.despesaacao da
					        inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid
					        inner join ( select a.acaid, coalesce ( SUM(a.dpavalor), 0 ) as valor from elabrev.despesaacao a
					                inner join elabrev.ppaacao_orcamento acao on acao.acaid = a.acaid
					               inner join naturezadespesa nd ON nd.ndpid = a.ndpid
					                  where prgano ='{$_SESSION['exercicio']}'  and substr(nd.ndpcod, 1,2)<>'31'
					                 group by a.acaid ) z ON z.acaid = da.acaid
					       inner join naturezadespesa nd ON nd.ndpid = da.ndpid
					       inner join idoc on idoc.idoid = da.idoid
					       inner join ( select ao.acaid, max(tpdid) as tpdid from elabrev.tipodetalhamentoacao a
					         inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
					         where ao.prgano ='{$_SESSION['exercicio']}'
					         group by ao.acaid ) tda ON tda.acaid = acao.acaid
					       --left join naturezareceita nr ON nr.nrcid = da.nrcid
					       where 1=1  and da.ppoid = 161 and acao.acacod not in ( '2004','2010','2011','2012','20CW','20CE', '0284' ) and substr(nd.ndpcod, 1,2)<>'31' and nd.ndpcod not in  ( '33901514', '33901516', '33903017',   '33903504',  '33903603', '33903615', '33903618', '33903622', '33903654', '33903655', '33903657', '33903709', '33903727', '33903728', '33903908', '33903911',  '33903926', '33903927', '33903928', '33903930', '33903931', '33903957', '33903995', '33903997', '33913017', '33913504', '33913618', '33913654', '33913655', '33913657', '33913709', '33913727', '33913728', '33913908', '33913911', '33913926', '33913927', '33913928', '33913930', '33913931', '33913957', '33913995', '33913997', '44903017', '44903504', '44903615', '44903622', '44903645', '44903646', '44903654', '44903655', '44903657', '44903792', '44903793', '44903910', '44903912',  '44903957', '44903992', '44903993', '44903994', '44903995', '44905100',   '44905196', '44905199', '44905235',  '44905250', '44905253', '44913017', '44913504', '44913645', '44913646', '44913654', '44913655', '44913657', '44913792', '44913793', '44913957', '44913992', '44913993', '44913994', '44913995', '44915235', '45906100', '45906102', '45906103', '45906104', '45906105', '45906106', '45906107', '45906108', '45906191', '45906192', '45906199')
					       group by
					       acao.acaid,
					        acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       tda.tpdid,
					       acaqtdefisico ,
					       acaqtdefinanceiro ,
					       justificativa ,
					       substr(nd.ndpcod,1,2),
					       substr(nd.ndpcod,1,6)||'00',
					       da.iducod,
					       da.foncod, da.ploid,
					       idoc.idocod,
					       acao.acaqtdefisico,
					       acao.tdecod,
					       acao.justificativa,
					       acadscunmsof,
					       z.valor

					      union all

					      select  'EYX' as login,
					       MD5('COISUCA') as senha,
					       '26000' as orgao,
					       acao.acaid,
					       cast(acao.acaid as varchar(5))||case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' then '1' else cast(tda.tpdid as varchar(2)) end as codigo,
					       acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' then 1 else tda.tpdid end as tipodetalhamento,
					       Case
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) = '' then null
					        when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','0181','00H1') and substr(nd.ndpcod, 1,2)='31' then null
					        when tda.tpdid in (5) and acao.acacod not in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)='31' and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        else coalesce(acaqtdefisico,1) end as quantidadefisico,
					       z.valor as valorfisico,
					       justificativa as justificativa,
					       '2013' as ano,
					       da.iducod,
					       idoc.idocod,
					       substr(nd.ndpcod,1,6)||'00' as ndpcod,
					       da.foncod, da.ploid,
					       coalesce ( SUM(da.dpavalor), 0 ) as valor,

					       '' as nrcod,
					       0 as valor_receita
					       from elabrev.despesaacao da
					        inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid
					        inner join ( select a.acaid, coalesce ( SUM(a.dpavalor), 0 ) as valor from elabrev.despesaacao a
					                inner join elabrev.ppaacao_orcamento acao on acao.acaid = a.acaid
					               inner join naturezadespesa nd ON nd.ndpid = a.ndpid
					                  where prgano ='{$_SESSION['exercicio']}'  and substr(nd.ndpcod, 1,2)='31'
					                 group by a.acaid ) z ON z.acaid = da.acaid
					       inner join naturezadespesa nd ON nd.ndpid = da.ndpid
					       inner join idoc on idoc.idoid = da.idoid
					       inner join ( select ao.acaid, max(tpdid) as tpdid from elabrev.tipodetalhamentoacao a
					         inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
					         where ao.prgano ='{$_SESSION['exercicio']}'
					         group by ao.acaid ) tda ON tda.acaid = acao.acaid
					       --left join naturezareceita nr ON nr.nrcid = da.nrcid
					       where 1=1  and da.ppoid = 161 and acao.acacod not in ( '2004','2010','2011','2012','20CW','20CE', '0284' ) and substr(nd.ndpcod, 1,2)='31' and nd.ndpcod not in  ( '33901514', '33901516', '33903017',   '33903504',  '33903603', '33903615', '33903618', '33903622', '33903654', '33903655', '33903657', '33903709', '33903727', '33903728', '33903908', '33903911',  '33903926', '33903927', '33903928', '33903930', '33903931', '33903957', '33903995', '33903997', '33913017', '33913504', '33913618', '33913654', '33913655', '33913657', '33913709', '33913727', '33913728', '33913908', '33913911', '33913926', '33913927', '33913928', '33913930', '33913931', '33913957', '33913995', '33913997', '44903017', '44903504', '44903615', '44903622', '44903645', '44903646', '44903654', '44903655', '44903657', '44903792', '44903793', '44903910', '44903912',  '44903957', '44903992', '44903993', '44903994', '44903995', '44905100',   '44905196', '44905199', '44905235',  '44905250', '44905253', '44913017', '44913504', '44913645', '44913646', '44913654', '44913655', '44913657', '44913792', '44913793', '44913957', '44913992', '44913993', '44913994', '44913995', '44915235', '45906100', '45906102', '45906103', '45906104', '45906105', '45906106', '45906107', '45906108', '45906191', '45906192', '45906199')
					       group by
					       acao.acaid,
					        acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       tda.tpdid,
					       acaqtdefisico ,
					       acaqtdefinanceiro ,
					       justificativa ,
					       substr(nd.ndpcod,1,2),
					       substr(nd.ndpcod,1,6)||'00',
					       da.iducod,
					       da.foncod, da.ploid,
					       idoc.idocod,
					       acao.acaqtdefisico,
					       acao.tdecod,
					       acao.justificativa,
					       acadscunmsof,
					       z.valor

					      union all

					      select  'EYX' as login,
					       MD5('COISUCA') as senha,
					       '26000' as orgao,
					       acao.acaid,
					       cast(acao.acaid as varchar(5))||case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nr.nrccod, 1,2)<>'31' then '1' else cast(tda.tpdid as varchar(2)) end as codigo,
					       acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nr.nrccod, 1,2)<>'31' then 1 else tda.tpdid end as tipodetalhamento,
					       Case
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) = '' then null
					        when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nr.nrccod, 1,2)<>'31' and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','0181','00H1') and substr(nr.nrccod, 1,2)<>'31' and trim(coalesce(acadscunmsof,'')) = '' then null
					        else coalesce(acaqtdefisico,1) end as quantidadefisico,
					       Case when acaqtdefinanceiro is null then coalesce ( SUM(da.dpavalor), 0 ) else acaqtdefinanceiro end as valorfisico,
					       justificativa as justificativa,
					       '2013' as ano,

					       da.iducod,
					       idoc.idocod,
					       '' as ndpcod,
					       da.foncod, da.ploid,
					       0 as valor,

					       substr(nr.nrccod,1,6)||'00' as nrccod,
					       coalesce ( SUM(da.dpavalor), 0 ) as valor_receita
					       from elabrev.despesaacao da
					        inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid
					       inner join naturezareceita nr ON nr.nrcid = da.nrcid
					       inner join idoc on idoc.idoid = da.idoid
					       inner join ( select ao.acaid, max(tpdid) as tpdid from elabrev.tipodetalhamentoacao a
					         inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
					         where ao.prgano ='{$_SESSION['exercicio']}'
					         group by ao.acaid ) tda ON tda.acaid = acao.acaid
					       where 1=1  and da.ppoid = 161 and acao.acacod not in ( '2004','2010','2011','2012','20CW','20CE', '0284' ) and nr.nrccod not in  ( '33901514', '33901516', '33903017',   '33903504',  '33903603', '33903615', '33903618', '33903622', '33903654', '33903655', '33903657', '33903709', '33903727', '33903728', '33903908', '33903911',  '33903926', '33903927', '33903928', '33903930', '33903931', '33903957', '33903995', '33903997', '33913017', '33913504', '33913618', '33913654', '33913655', '33913657', '33913709', '33913727', '33913728', '33913908', '33913911', '33913926', '33913927', '33913928', '33913930', '33913931', '33913957', '33913995', '33913997', '44903017', '44903504', '44903615', '44903622', '44903645', '44903646', '44903654', '44903655', '44903657', '44903792', '44903793', '44903910', '44903912',  '44903957', '44903992', '44903993', '44903994', '44903995', '44905100',   '44905196', '44905199', '44905235',  '44905250', '44905253', '44913017', '44913504', '44913645', '44913646', '44913654', '44913655', '44913657', '44913792', '44913793', '44913957', '44913992', '44913993', '44913994', '44913995', '44915235', '45906100', '45906102', '45906103', '45906104', '45906105', '45906106', '45906107', '45906108', '45906191', '45906192', '45906199')
					       group by
					       acao.acaid,
					        acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       tda.tpdid,
					       acaqtdefisico ,
					       acaqtdefinanceiro ,
					       justificativa ,
					       da.iducod,
					       da.foncod, da.ploid,
					       substr(nr.nrccod,1,2),
					       substr(nr.nrccod,1,6)||'00',
					       idoc.idocod,
					       acao.acaqtdefisico,
					       acao.tdecod,
					       acao.justificativa,
					       acadscunmsof


					      union all

					 /* Detalhamento nas Naturezas de Inform?tica que devem ser enviadas por Subelemento */
					      select  'EYX' as login,
					       MD5('COISUCA') as senha,
					       '26000' as orgao,
					       acao.acaid,
					       cast(acao.acaid as varchar(5))||case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' then '1' else cast(tda.tpdid as varchar(2)) end as codigo,
					       acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' then 1 else tda.tpdid end as tipodetalhamento,
					       Case
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) = '' then null
					        when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','0181','00H1') and substr(nd.ndpcod, 1,2)<>'31' and trim(coalesce(acadscunmsof,'')) = '' then null
					        else coalesce(acaqtdefisico,1) end as quantidadefisico,
					       z.valor as valorfisico,
					       justificativa as justificativa,
					       '2013' as ano,
					       da.iducod,
					       idoc.idocod,
					       nd.ndpcod as ndpcod,
					       da.foncod, da.ploid,
					       coalesce ( SUM(da.dpavalor), 0 ) as valor,

					       '' as nrcod,
					       0 as valor_receita
					       from elabrev.despesaacao da
					        inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid
					        inner join ( select a.acaid, coalesce ( SUM(a.dpavalor), 0 ) as valor from elabrev.despesaacao a
					                inner join elabrev.ppaacao_orcamento acao on acao.acaid = a.acaid
					               inner join naturezadespesa nd ON nd.ndpid = a.ndpid
					                  where prgano ='{$_SESSION['exercicio']}'  and substr(nd.ndpcod, 1,2)<>'31'
					                 group by a.acaid ) z ON z.acaid = da.acaid
					       inner join naturezadespesa nd ON nd.ndpid = da.ndpid
					       inner join idoc on idoc.idoid = da.idoid
					       inner join ( select ao.acaid, max(tpdid) as tpdid from elabrev.tipodetalhamentoacao a
					         inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
					         where ao.prgano ='{$_SESSION['exercicio']}'
					         group by ao.acaid ) tda ON tda.acaid = acao.acaid
					       --left join naturezareceita nr ON nr.nrcid = da.nrcid
					       where 1=1  and da.ppoid = 161 and acao.acacod not in ( '2004','2010','2011','2012','20CW','20CE', '0284' ) and substr(nd.ndpcod, 1,2)<>'31' and nd.ndpcod in  ( '33901514', '33901516', '33903017',   '33903504',  '33903603', '33903615', '33903618', '33903622', '33903654', '33903655', '33903657', '33903709', '33903727', '33903728', '33903908', '33903911',  '33903926', '33903927', '33903928', '33903930', '33903931', '33903957', '33903995', '33903997', '33913017', '33913504', '33913618', '33913654', '33913655', '33913657', '33913709', '33913727', '33913728', '33913908', '33913911', '33913926', '33913927', '33913928', '33913930', '33913931', '33913957', '33913995', '33913997', '44903017', '44903504', '44903615', '44903622', '44903645', '44903646', '44903654', '44903655', '44903657', '44903792', '44903793', '44903910', '44903912',  '44903957', '44903992', '44903993', '44903994', '44903995', '44905100',   '44905196', '44905199', '44905235',  '44905250', '44905253', '44913017', '44913504', '44913645', '44913646', '44913654', '44913655', '44913657', '44913792', '44913793', '44913957', '44913992', '44913993', '44913994', '44913995', '44915235', '45906100', '45906102', '45906103', '45906104', '45906105', '45906106', '45906107', '45906108', '45906191', '45906192', '45906199')
					       group by
					       acao.acaid,
					        acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       tda.tpdid,
					       acaqtdefisico ,
					       acaqtdefinanceiro ,
					       justificativa ,
					       nd.ndpcod,
					       da.iducod,
					       da.foncod, da.ploid,
					       idoc.idocod,
					       acao.acaqtdefisico,
					       acao.tdecod,
					       acao.justificativa,
					       acadscunmsof,
					       z.valor

					      union all

					      select  'EYX' as login,
					       MD5('COISUCA') as senha,
					       '26000' as orgao,
					       acao.acaid,
					       cast(acao.acaid as varchar(5))||case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' then '1' else cast(tda.tpdid as varchar(2)) end as codigo,
					       acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' then 1 else tda.tpdid end as tipodetalhamento,
					       Case
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) = '' then null
					        when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','0181','00H1') and substr(nd.ndpcod, 1,2)='31' then null
					        when tda.tpdid in (5) and acao.acacod not in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)='31' and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        else coalesce(acaqtdefisico,1) end as quantidadefisico,
					       z.valor as valorfisico,
					       justificativa as justificativa,
					       '2013' as ano,
					       da.iducod,
					       idoc.idocod,
					       nd.ndpcod as ndpcod,
					       da.foncod, da.ploid,
					       coalesce ( SUM(da.dpavalor), 0 ) as valor,

					       '' as nrcod,
					       0 as valor_receita
					       from elabrev.despesaacao da
					        inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid
					        inner join ( select a.acaid, coalesce ( SUM(a.dpavalor), 0 ) as valor from elabrev.despesaacao a
					                inner join elabrev.ppaacao_orcamento acao on acao.acaid = a.acaid
					               inner join naturezadespesa nd ON nd.ndpid = a.ndpid
					                  where prgano ='{$_SESSION['exercicio']}'  and substr(nd.ndpcod, 1,2)='31'
					                 group by a.acaid ) z ON z.acaid = da.acaid
					       inner join naturezadespesa nd ON nd.ndpid = da.ndpid
					       inner join idoc on idoc.idoid = da.idoid
					       inner join ( select ao.acaid, max(tpdid) as tpdid from elabrev.tipodetalhamentoacao a
					         inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
					         where ao.prgano ='{$_SESSION['exercicio']}'
					         group by ao.acaid ) tda ON tda.acaid = acao.acaid
					       --left join naturezareceita nr ON nr.nrcid = da.nrcid
					       where 1=1  and da.ppoid = 161 and acao.acacod not in ( '2004','2010','2011','2012','20CW','20CE', '0284' ) and substr(nd.ndpcod, 1,2)='31' and nd.ndpcod in ( '33901514', '33901516', '33903017',   '33903504',  '33903603', '33903615', '33903618', '33903622', '33903654', '33903655', '33903657', '33903709', '33903727', '33903728', '33903908', '33903911',  '33903926', '33903927', '33903928', '33903930', '33903931', '33903957', '33903995', '33903997', '33913017', '33913504', '33913618', '33913654', '33913655', '33913657', '33913709', '33913727', '33913728', '33913908', '33913911', '33913926', '33913927', '33913928', '33913930', '33913931', '33913957', '33913995', '33913997', '44903017', '44903504', '44903615', '44903622', '44903645', '44903646', '44903654', '44903655', '44903657', '44903792', '44903793', '44903910', '44903912',  '44903957', '44903992', '44903993', '44903994', '44903995', '44905100',   '44905196', '44905199', '44905235',  '44905250', '44905253', '44913017', '44913504', '44913645', '44913646', '44913654', '44913655', '44913657', '44913792', '44913793', '44913957', '44913992', '44913993', '44913994', '44913995', '44915235', '45906100', '45906102', '45906103', '45906104', '45906105', '45906106', '45906107', '45906108', '45906191', '45906192', '45906199')
					       group by
					       acao.acaid,
					        acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       tda.tpdid,
					       acaqtdefisico ,
					       acaqtdefinanceiro ,
					       justificativa ,
					       nd.ndpcod,
					       da.iducod,
					       da.foncod, da.ploid,
					       idoc.idocod,
					       acao.acaqtdefisico,
					       acao.tdecod,
					       acao.justificativa,
					       acadscunmsof,
					       z.valor

					      union all

					      select  'EYX' as login,
					       MD5('COISUCA') as senha,
					       '26000' as orgao,
					       acao.acaid,
					       cast(acao.acaid as varchar(5))||case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nr.nrccod, 1,2)<>'31' then '1' else cast(tda.tpdid as varchar(2)) end as codigo,
					       acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nr.nrccod, 1,2)<>'31' then 1 else tda.tpdid end as tipodetalhamento,
					       Case
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) = '' then null
					        when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nr.nrccod, 1,2)<>'31' and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
					        when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','0181','00H1') and substr(nr.nrccod, 1,2)<>'31' and trim(coalesce(acadscunmsof,'')) = '' then null
					        else coalesce(acaqtdefisico,1) end as quantidadefisico,
					       Case when acaqtdefinanceiro is null then coalesce ( SUM(da.dpavalor), 0 ) else acaqtdefinanceiro end as valorfisico,
					       justificativa as justificativa,
					       '2013' as ano,

					       da.iducod,
					       idoc.idocod,
					       '' as ndpcod,
					       da.foncod, da.ploid,
					       0 as valor,

					       nr.nrccod as nrccod,
					       coalesce ( SUM(da.dpavalor), 0 ) as valor_receita
					       from elabrev.despesaacao da
					        inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid
					       inner join naturezareceita nr ON nr.nrcid = da.nrcid
					       inner join idoc on idoc.idoid = da.idoid
					       inner join ( select ao.acaid, max(tpdid) as tpdid from elabrev.tipodetalhamentoacao a
					         inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
					         where ao.prgano ='{$_SESSION['exercicio']}'
					         group by ao.acaid ) tda ON tda.acaid = acao.acaid
					       where 1=1  and da.ppoid = 161 and acao.acacod not in ( '2004','2010','2011','2012','20CW','20CE', '0284' ) and nr.nrccod in  ( '33901514', '33901516', '33903017',   '33903504',  '33903603', '33903615', '33903618', '33903622', '33903654', '33903655', '33903657', '33903709', '33903727', '33903728', '33903908', '33903911',  '33903926', '33903927', '33903928', '33903930', '33903931', '33903957', '33903995', '33903997', '33913017', '33913504', '33913618', '33913654', '33913655', '33913657', '33913709', '33913727', '33913728', '33913908', '33913911', '33913926', '33913927', '33913928', '33913930', '33913931', '33913957', '33913995', '33913997', '44903017', '44903504', '44903615', '44903622', '44903645', '44903646', '44903654', '44903655', '44903657', '44903792', '44903793', '44903910', '44903912',  '44903957', '44903992', '44903993', '44903994', '44903995', '44905100',   '44905196', '44905199', '44905235',  '44905250', '44905253', '44913017', '44913504', '44913645', '44913646', '44913654', '44913655', '44913657', '44913792', '44913793', '44913957', '44913992', '44913993', '44913994', '44913995', '44915235', '45906100', '45906102', '45906103', '45906104', '45906105', '45906106', '45906107', '45906108', '45906191', '45906192', '45906199')
					       group by
					       acao.acaid,
					        acao.esfcod,
					       acao.unicod,
					       acao.funcod,
					       acao.sfucod,
					       acao.prgcod,
					       acao.acacod,
					       acao.loccod, acao.acapropostaenviada,
					       tda.tpdid,
					       acaqtdefisico ,
					       acaqtdefinanceiro ,
					       justificativa ,
					       da.iducod,
					       da.foncod, da.ploid,
					       nr.nrccod,
					       idoc.idocod,
					       acao.acaqtdefisico,
					       acao.tdecod,
					       acao.justificativa,
					       acadscunmsof

					      ) as foo
					      where
							acaid in (" . implode(', ', $arrAcao) . ") and acapropostaenviada = false
					      order by  esfcod,
					       unicod,
					       funcod,
					       sfucod,
					       prgcod,
					       acacod,
					       loccod,
					       tipodetalhamento";

    $arrDados = $this->db->carregar($sql);
    $arrDados = $arrDados ? $arrDados : array();

    return $arrDados;
  }

    private function carregaDadosAcompanhamento($arrAcao = array())
    {
            $arrAcao = implode(', ', $arrAcao);
            $sql = <<<DML
SELECT DISTINCT esf.codigo AS esfera,
                org.codigo as unicod,
                apr.siop_funcao AS funcao,
                apr.siop_subfuncao AS subfuncao,
                prg.codigo AS prgcod,
                a.codigo AS acacod,
                apr.id_acao_programatica AS acaid,
                (SELECT aca.codtipoinclusaoacao
                   FROM monitora.acao aca
                   WHERE aca.acacod = a.codigo
                     AND aca.unicod = org.codigo
                     AND aca.prgcod = prg.codigo
                     AND aca.prgano::integer = apr.id_exercicio
	               LIMIT 1) AS tipoinclusaoacao,
                id_periodo_referencia
  FROM planacomorc.acao a
    INNER JOIN planacomorc.acao_programatica apr ON apr.id_acao = a.id_acao
    INNER JOIN planacomorc.orgao org USING(id_orgao)
    INNER JOIN planacomorc.localizador_programatica lpr USING(id_acao_programatica)
    INNER JOIN planacomorc.quantitativo_sof qsf USING(id_acao_programatica, loccod)
    INNER JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = a.id_acao
      AND dae.id_exercicio = apr.id_exercicio
    INNER JOIN planacomorc.acompanhamento_acao acc USING(id_acao_programatica, id_localizador)
    INNER JOIN workflow.documento wfd USING(docid)
    LEFT JOIN planacomorc.programa prg USING(id_programa)
    LEFT JOIN planacomorc.produto prd USING(id_produto)
    LEFT JOIN planacomorc.unidade_medida u ON u.id_unidade_medida = dae.id_unidade_medida
    LEFT JOIN planacomorc.esfera esf USING(id_esfera)
    LEFT JOIN planacomorc.subfuncao sfu USING(id_subfuncao)
  WHERE id_acao_programatica in ({$arrAcao})
    AND qsf.quantidade_fisico != 0
    AND esdid = 753
DML;
        $arrDados = $this->db->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }

  private function carregaDadosFinanceiro($unicod, $prgcod, $acacod, $loccod) {
    global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $servidor_bd_siafi, $porta_bd_siafi, $nome_bd_siafi, $usuario_db_siafi, $senha_bd_siafi;
    // Parâmetros para a nova conexão com o banco do SIAFI

    $servidor_bd = $servidor_bd_siafi;
    $porta_bd = $porta_bd_siafi;
    $nome_bd = $nome_bd_siafi;
    $usuario_db = $usuario_db_siafi;
    $senha_bd = $senha_bd_siafi;
    // Cria o novo objeto de conexão
    $db2 = new cls_banco();

    $sql = "SELECT
	                unicod AS cod_agrupador1,
	                sum(valor1) AS rofdot_ini,
	                sum(valor2) AS rofautorizado,
	                sum(valor3) AS empenhado,
	                sum(valor4) AS rofliquidado_favorecido,
	                sum(valor5) AS rofpago
	            FROM
	                (
	                SELECT
	                   	sld.unicod,
	                   	CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192190101','192190109') THEN
			            sld.sldvalor END
			            ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130201','192140100','192140200','192190101','192190109','192190201','192190209','192190301','192190302') THEN
			            sld.sldvalor END
			            ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
			            sld.sldvalor END
			            ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292130201','292130202','292130209','292130301') THEN
			            sld.sldvalor END
			            ELSE 0 END AS valor4,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
			            sld.sldvalor END
			            ELSE 0 END AS valor5
	               	FROM
	                  	dw.saldo2012 sld
	                WHERE
	                	sld.unicod = '$unicod' AND sld.prgcod = '$prgcod' AND sld.acacod = '$acacod' AND sld.loccod = '$loccod'
	                	AND sld.sldcontacontabil in ('192110101','192110201','192110209','192190101','192190109','192110101','192110201',
	                								 '192110209','192110301','192110303','192130101','192130102','192130201','192140100',
	                								 '192140200','192190101','192190109','192190201','192190209','192190301','192190302',
	                								 '292130100','292130201','292130202','292130203','292130301','292130201','292130202',
	                								 '292130209','292130301','292130301','292410403')
	                ) as foo
	            GROUP BY
	                unicod
	                ORDER BY
	                unicod";

    $dadosfinanceiroscompletos = $db2->pegaLinha($sql);
    return $dadosfinanceiroscompletos;
  }

}
