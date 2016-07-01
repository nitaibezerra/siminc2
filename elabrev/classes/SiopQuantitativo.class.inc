<?php
/**
 * Consulta ao método quantitativo do SIOP.
 * $Id: SiopQuantitativo.class.inc 83029 2014-07-17 18:34:07Z werteralmeida $
 */

/**
 * Classes utilizadas no mapeamento do serviço.
 */
include_once APPRAIZ."elabrev/classes/WSQuantitativo.class.inc";

class SiopQuantitativo
{
	private $urlWsdl;
	private $wsusuario;
	private $wssenha;
	private $certificado;
	private $senha_certificado;
	private $db;
	private $post;
	private $acaid;
	private $acacod;
	private $unicod;
	private $valorfisico;
    private $expansaoFisicaSolicitada;
	private $codigomomento;

	public function __construct($arrParam = array())
    {
		global $db;
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWsdl = "https://testews.siop.gov.br/services/WSQuantitativo?wsdl";
			$this->certificado = APPRAIZ. "elabrev/modulos/sistema/comunica/mec.pem";
			$this->senha_certificado = "mec2011";
		} else {
			$urlWsdl = "https://www.siop.gov.br/services/WSQuantitativo?wsdl";
			$this->certificado =  APPRAIZ. "planacomorc/modulos/sistema/comunica/simec.pem";
			$this->senha_certificado = "siMEC2013";
		}

		if($db){
			$this->db = $db;
		} else {
			$this->db = new cls_banco();
		}
		
		$this->wsusuario 	= $arrParam['usuario'];
		$this->wssenha 		= $arrParam['senha'];
		$this->urlWsdl 		= $urlWsdl;
		$this->post			= $arrParam['post'];
		$this->codigomomento= $arrParam['post']['codigomomento'];
		$this->acaid		= '';
		$this->unicod		= '';
		$this->acacod		= '';
		$this->valorfisico	= '';
	}

	public function cadastrarProposta() {
        global $db;
		$arrProposta = $this->carregarDadosProposta($this->post['acaid_cadastra']);
		if ($arrProposta) {
			$acaid = $arrRetorno = array();
			foreach ($arrProposta as $valor) {
				if( !in_array( $valor['acaid'], $acaid ) ){
					$this->acaid = $valor['acaid'];
					$obEnvio = $this->getCadastrarProposta($valor, $arrProposta);
					$this->valorfisico = 0;

                    $sql = "INSERT INTO elabrev.ws_log(url, metodo)
                              VALUES ('" . $this->urlWsdl . "', '')
                              RETURNING wslid";
                    $idLog = $db->pegaUm($sql);
                    $db->commit();

                    $wsQuantitativo = new WSQuantitativo(
                        $this->urlWsdl,
                        array(
                            'local_cert' => $this->certificado,
                            'passphrase ' => $this->senha_certificado,
                            'exceptions' => 1,
                            'trace' => 1,
                            'encoding' => 'ISO-8859-1'
                        )
                    );
                    // -- Enviando a requisição
                    try {
                        $retorno = $wsQuantitativo->cadastrarProposta( $obEnvio, $valor );
                        $sql = "UPDATE elabrev.ws_log
                                  SET wslretorno = 'SUCESSO',
                                      wslrequisicaoxml = '" . str_replace("'", "''", $wsQuantitativo->__getLastRequest()) . "',
                                      wslrequisicaoheader = '" . str_replace("'", "''", $wsQuantitativo->__getLastRequestHeaders()) . "',
                                      wslrespostaxml = '" . str_replace("'", "''", $wsQuantitativo->__getLastResponse()) . "',
                                      wslrespostaheader = '" . str_replace("'", "''", $wsQuantitativo->__getLastResponseHeaders()) . "'
                                  WHERE wslid = {$idLog}";
                    } catch (Exception $e) {
                        $sql = "UPDATE elabrev.ws_log
                                  SET wslretorno = 'ERROR',
                                      metodo = '" . str_replace("'", "''", $e->getMessage()) . "',
                                      wslrequisicaoxml = '" . str_replace("'", "''", $wsQuantitativo->__getLastRequest()) . "',
                                      wslrequisicaoheader = '" . str_replace("'", "''", $wsQuantitativo->__getLastRequestHeaders()) . "',
                                      wslrespostaxml = '" . str_replace("'", "''", $wsQuantitativo->__getLastResponse()) . "',
                                      wslrespostaheader = '" . str_replace("'", "''", $wsQuantitativo->__getLastResponseHeaders()) . "'
                                  WHERE wslid = {$idLog}";
                    }
                    $db->executar($sql);
                    $db->commit();

					array_push(
                        $arrRetorno,
                        array(
                            'acaid' => $valor['acaid'],
                            'retorno' => $retorno,
                            'requisicao' => $obEnvio
                        )
                    );
					$obEnvio = '';
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
		//ver($this->post['acaid_consulta'], $arrProposta,d);
		if ($arrProposta) {
			$acaid = array();
			$arrRetorno = array();
			foreach ($arrProposta as $valor) {
				if (!in_array( $valor['acaid'], $acaid)) {
					$this->acaid = $valor['acaid'];
					$obEnvio = $this->getConsultarProposta($valor, $arrProposta);
					$this->valorfisico = 0;
//					ver($obEnvio,d);
					$wsQuantitativo = new WSQuantitativo(
                        $this->urlWsdl,
                        array(
                            'local_cert' => $this->certificado,
                            'passphrase ' => $this->senha_certificado,
                            'exceptions' => true,
                            'trace'	=> true,
                            'encoding' => 'ISO-8859-1'
                        )
                    );
					$retorno = $wsQuantitativo->consultarProposta( $obEnvio, $valor );
					array_push($arrRetorno, array('acaid' => $valor['acaid'], 'retorno' => $retorno));
					//return $retorno;
					array_push($acaid, $valor['acaid']);
				}
			}
			return $arrRetorno;
		} else {
			return 'erro';
		}
	}
	
	public function excluirProposta(){
		
		$arrProposta = $this->carregarDadosProposta( $this->post['acaid_exclui'] );
		//ver($this->post['acaid_exclui'], $arrProposta,d);
		if( $arrProposta ){
			$acaid = array();
			$arrRetorno = array();
			foreach ($arrProposta as $valor) {
				if( !in_array( $valor['acaid'], $acaid ) ){
					$this->acaid = $valor['acaid'];
					$obEnvio = $this->getExcluirProposta( $valor, $arrProposta );
					$this->valorfisico = 0;
					//ver($obEnvio,d);
					$wsQuantitativo = new WSQuantitativo($this->urlWsdl, array(
																		'local_cert'	=> $this->certificado, 
																		'passphrase ' 	=> $this->senha_certificado,
																		'exceptions'	=> true,
																        'trace'			=> true,
																		'encoding'		=> 'ISO-8859-1' )
														);
					$retorno = $wsQuantitativo->excluirProposta( $obEnvio, $valor );
					array_push($arrRetorno, array( 'acaid' => $valor['acaid'], 'retorno' => $retorno));
					//return $retorno;
					array_push( $acaid, $valor['acaid'] );
				}
			}
			return $arrRetorno;
		} else {
			return 'erro';
		}
	}
	
	public function obterExecucaoOrcamentariaSam(){
		$obEnvio = $this->getObterExecucaoOrcamentariaSam();
		//ver($obEnvio,d);
		$wsQuantitativo = new WSQuantitativo($this->urlWsdl, array(
															'local_cert'	=> $this->certificado, 
															'passphrase ' 	=> $this->senha_certificado,
															'exceptions'	=> true,
													        'trace'			=> true,
															'encoding'		=> 'ISO-8859-1' )
											);
		$retorno = $wsQuantitativo->obterExecucaoOrcamentariaSam( $obEnvio );
		return $retorno;
	}
	
	public function obterProgramacaoCompletaQuantitativo(){
		$obEnvio = $this->getObterProgramacaoCompletaQuantitativo();
		//ver($obEnvio,d);		
		$wsQuantitativo = new WSQuantitativo($this->urlWsdl, array(
															'local_cert'	=> $this->certificado, 
															'passphrase ' 	=> $this->senha_certificado,
															'exceptions'	=> true,
													        'trace'			=> true,
															'encoding'		=> 'ISO-8859-1' )
											);		
		$retorno = $wsQuantitativo->obterProgramacaoCompletaQuantitativo( $obEnvio );		
		return $retorno;
	}
	
	public function obterTabelasApoioQuantitativo(){
		$obEnvio = $this->getObterTabelasApoioQuantitativo();
		//ver($obEnvio,d);
		$wsQuantitativo = new WSQuantitativo($this->urlWsdl, array(
															'local_cert'	=> $this->certificado, 
															'passphrase ' 	=> $this->senha_certificado,
															'exceptions'	=> true,
													        'trace'			=> true,
															'encoding'		=> 'ISO-8859-1' )
											);
		$retorno = $wsQuantitativo->obterTabelasApoioQuantitativo( $obEnvio );
		return $retorno;
	}
	
    public function cadastrarAcompanhamento()
    {
        global $db;

        $arrAcompanhamento = $this->carregaDadosAcompanhamento($this->post['acaid_cadastraAcompanhamento']);

        if ($arrAcompanhamento) {
            $unicod = array();
            $arrRetorno = array();
            $count = 0;

            foreach ($arrAcompanhamento as $valor) {
                $this->acacod = $valor['acacod'];
                $this->unicod = $valor['unicod'];
                $obEnvio = $this->getCadastrarAcompanhamentoOrcamentario($valor);
                $sql = "INSERT INTO elabrev.ws_log(url, metodo, id_acao_programatica, wslusucpf)
                            VALUES ('" . $this->urlWsdl . "', 'wsquantitativo.cadastrarAcompanhamentoOrcamentario', {$valor['acaid']}, '{$_SESSION['usucpf']}')
                            RETURNING wslid";
                $idLog = $db->pegaUm($sql);
                $db->commit();
                $this->valorfisico = 0;

                $wsQuantitativo = new WSQuantitativo(
                        $this->urlWsdl,
                        array(
                            'local_cert' => $this->certificado,
                            'passphrase ' => $this->senha_certificado,
                            'exceptions' => true,
                            'trace' => true,
                            'encoding' => 'ISO-8859-1'
                        )
                );
                try {
                    $retorno = $wsQuantitativo->cadastrarAcompanhamentoOrcamentario($obEnvio, $valor );
                    $sql = "UPDATE elabrev.ws_log
                              SET wslretorno = 'SUCESSO',
                                  wslrequisicaoxml = '" . str_replace("'", "''", $wsQuantitativo->__getLastRequest()) . "',
                                  wslrequisicaoheader = '" . str_replace("'", "''", $wsQuantitativo->__getLastRequestHeaders()) . "',
                                  wslrespostaxml = '" . str_replace("'", "''", $wsQuantitativo->__getLastResponse()) . "',
                                  wslrespostaheader = '" . str_replace("'", "''", $wsQuantitativo->__getLastResponseHeaders()) . "'
                              WHERE wslid = {$idLog}";
                    $db->executar($sql);
                    $db->commit();
                } catch (Exception $e) {
                    $sql = "UPDATE elabrev.ws_log
                              SET wslretorno = 'ERROR',
                                  metodo = '" . str_replace("'", "''", $e->getMessage()) . "',
                                  wslrequisicaoxml = '" . str_replace("'", "''", $wsQuantitativo->__getLastRequest()) . "',
                                  wslrequisicaoheader = '" . str_replace("'", "''", $wsQuantitativo->__getLastRequestHeaders()) . "',
                                  wslrespostaxml = '" . str_replace("'", "''", $wsQuantitativo->__getLastResponse()) . "',
                                  wslrespostaheader = '" . str_replace("'", "''", $wsQuantitativo->__getLastResponseHeaders()) . "'
                              WHERE wslid = {$idLog}";
                    $db->executar($sql);
                    $db->commit();
                }
                $wslmsgretorno = '';
                if (!$retorno->return->sucesso) {
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
                //return $retorno;
                array_push( $unicod, $valor['unicod'] );
            //}
            $count++;
            }
//            ver($arrRetorno,d);
            return $arrRetorno;
        } else {
            return 'erro';
        }
    }
	
	public function consultarAcompanhamento()
    {
		$arrAcompanhamento = $this->carregaDadosAcompanhamento( $this->post['acaid_concultaAcompanhamento'] );
		if ($arrAcompanhamento) {
			$acacod = array();
			$arrRetorno = array();
			foreach ($arrAcompanhamento as $valor) {
                $this->acacod = $valor['acacod'];
                $this->unicod = $valor['unicod'];
                $obEnvio = $this->getConsultarAcompanhamentoOrcamentario( $valor );
                $this->valorfisico = 0;
                $wsQuantitativo = new WSQuantitativo(
                    $this->urlWsdl,
                    array(
                        'local_cert' => $this->certificado,
                        'passphrase ' => $this->senha_certificado,
                        'exceptions' => true,
                        'trace' => true,
                        'encoding' => 'ISO-8859-1'
                    )
                );
                $retorno = $wsQuantitativo->consultarAcompanhamentoOrcamentario( $obEnvio, $valor );
                array_push($arrRetorno, array( 'acacod' => $valor['acacod'], 'unicod' => $valor['unicod'], 'retorno' => $retorno));
                array_push($acacod, $valor['acacod']);
			}
			return $arrRetorno;
		} else {
			return 'erro';
		}
	}
	
	private function getConsultarAcompanhamentoOrcamentario( $arrAcompanhamento = array() ){
		
		$ObjetoWS = new consultarAcompanhamentoOrcamentario();
		$ObjetoWS->codigoMomento 		= $this->codigomomento;
		$ObjetoWS->credencial		 	= $this->getCredencial();
		$ObjetoWS->dataHoraReferencia	= '';
		$ObjetoWS->exercicio			= $_SESSION['exercicio'];
		$ObjetoWS->filtro				= $this->getfiltroFuncionalProgramaticaDTO( $arrAcompanhamento );
		$ObjetoWS->periodoOrdem			= '2';
		
		return $ObjetoWS;
	}
	
	private function getfiltroFuncionalProgramaticaDTO( $arrAcompanhamento ){
		
		$ObjetoWS = new filtroFuncionalProgramaticaDTO();
		$ObjetoWS->codigoAcao 						= $arrAcompanhamento['acacod'];
		$ObjetoWS->codigoEsfera 					= $arrAcompanhamento['esfera'];
		$ObjetoWS->codigoFuncao						= $arrAcompanhamento['funcao'];
		$ObjetoWS->codigoLocalizador				= $arrAcompanhamento['loccod'];
		$ObjetoWS->codigoPrograma					= $arrAcompanhamento['prgcod'];
		$ObjetoWS->codigoSubFuncao					= $arrAcompanhamento['subfuncao'];
		$ObjetoWS->codigoTipoInclusaoAcao			= ($arrAcompanhamento['tipoinclusaoacao'] ? $arrAcompanhamento['tipoinclusaoacao'] : '1');
		$ObjetoWS->codigoTipoInclusaoLocalizador	= ($arrAcompanhamento['tipoinclusaolocalizador'] ? $arrAcompanhamento['tipoinclusaolocalizador'] : '1');
		$ObjetoWS->codigoUO							= $arrAcompanhamento['unicod'];
		
		return $ObjetoWS;
	}
	
	private function getCredencial(){
		
		$ObjetoWS = new credencialDTO();
		$ObjetoWS->perfil 	= 32;
		$ObjetoWS->senha  	= md5($this->wssenha);
		$ObjetoWS->usuario	= $this->wsusuario;
		
		return $ObjetoWS;
	}
	
	private function getCadastrarAcompanhamentoOrcamentario($arrAcompanhamento = array())
    {
		$ObjetoWS = new cadastrarAcompanhamentoOrcamentario();
		$ObjetoWS->acompanhamentoAcao = $this->getAcompanhamentoOrcamentarioAcaoDTO( $arrAcompanhamento );
		$ObjetoWS->credencial = $this->getCredencial();

		return $ObjetoWS;
	}
	
	private function getAcompanhamentoOrcamentarioAcaoDTO( $arrAcompanhamento = array() ){
		
		$ObjetoWS = new acompanhamentoOrcamentarioAcaoDTO();
		$ObjetoWS->periodoOrdem = '1';
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
                    $ObjetoWS->periodoOrdem
                );
		return $ObjetoWS;
	}
	
    private function getAcompanhamentoOrcamentarioLocalizadorDTO($acacod, $unicod, $periodoOrdem)
    {
        $sql = <<<DML
SELECT prf.titulo AS periodo,
       acc.executado_fisico AS realizado,
       TO_CHAR(acc.data_apuracao, 'YYYY-MM-DD') AS dataapuracao,
       acc.executado_rap_fisico AS realizado_rap,
       prg.codigo AS prgcod,
       lpr.loccod,
       apr.id_acao_programatica AS acaid,
       lpr.loccodigotipoinclusao as tipoinclusaolocalizador,
       qsf.quantidade_fisico AS meta,
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
DML;
//                ver($sql, d);
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
		//ver($ObjetoWS,d);
		return $ObjetoWS;
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
  WHERE apo.id_localizador_programatica = %d
DML;
        $stmt = sprintf($sqlPO, $idLocalizadorProgramatica);

        $arAcompPO = array();
        if ($data = $db->carregar($stmt)) {
            foreach ($data as $acompPOData) {
                $acompPO = new acompanhamentoPlanoOrcamentarioDTO();
                $acompPO->dataApuracaoLOA = $dataApuracao;
                $acompPO->planoOrcamentario = $acompPOData['planoorcamentario'];
                $acompPO->realizadoLOA = $acompPOData['realizadoloa'];
                $arAcompPO[] = $acompPO;
            }
        } else {
            $arAcompPO[] = new acompanhamentoPlanoOrcamentarioDTO();
        }
        return $arAcompPO;
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
    INNER JOIN planacomorc.acompanhamento_acao acc USING(id_acao_programatica, id_localizador)
    LEFT JOIN planacomorc.monquestionarioresposta mqr USING(mqpid, id_acompanhamento_acao)
  WHERE mqp.id_acao_programatica = {$idAcaoProgramatica}
    AND lpr.loccod = '{$codLocalizador}'
    AND mnq.id_periodo_referencia = {$periodoOrdem}
    AND mnq.qststatus = 'A'
DML;

        // -- Concatenando as respostas dadas ao questionário
        $questionario = array();
        foreach ($this->db->carregar($sql) as $resposta) {
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
     * 
     * @deprecated Foi susbstituído por getAnaliseLocalizadorDTO
     * @param type $acaid
     * @return \comentarioAcompanhamentoOrcamentarioDTO
     */	
    private function getComentarioAcompanhamentoOrcamentarioDTO( $acaid ){
		
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
			$$key->comentario 			= $v['comentario'];
			$$key->comentarioId 		= $v['codigo'];
			$$key->nomeUsuario 			= $v['nome'];
			$$key->periodoOrdem 		= '2';
			$$key->ultimaModificacao	= $v['data'];
			
			$ObjetoWS[$key] = $$key;
		}
		
		return $ObjetoWS;
	}
	
	private function getObterProgramacaoCompletaQuantitativo(){
		$ObjetoWS = new obterProgramacaoCompletaQuantitativo();
		$ObjetoWS->codigoMomento 		= $this->codigomomento;
		$ObjetoWS->credencial			= $this->getCredencial();
		$ObjetoWS->dataHoraReferencia	= '';
		$ObjetoWS->exercicio			= $_SESSION['exercicio'];
		
		return $ObjetoWS;
	}
	
	private function getCadastrarProposta( $proposta, $arrProposta ){
		
		$ObjetoWS = new cadastrarProposta();
		$ObjetoWS->credencial = $this->getCredencial();
		$ObjetoWS->proposta = $this->getPropostaDTO( $proposta, $arrProposta );
//                ver($ObjetoWS->proposta, d);
		
		return $ObjetoWS;
	}
	
	private function getPropostaDTO($proposta, $arrProposta) { //ver($arrProposta, d);

        // -- Verificar se existe algum PO para a proposta em questão
        $sql = <<<SQL
SELECT COUNT(ploid)
  FROM elabrev.planoorcamentario
  WHERE ploproduto IS NOT NULL
    AND acaid = {$this->acaid}
SQL;

		$total = $this->db->pegaUm($sql);
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
			$ObjetoWS->exercicio = $_SESSION['exercicio']+1; // int
			$ObjetoWS->financeiros = $this->getFinanceiroDTO( $arrProposta ); // financeiroDTO
			$ObjetoWS->justificativa = $proposta['justificativa'];
			$ObjetoWS->metaPlanoOrcamentario = $this->getMetaPlanoOrcamentarioDTO($proposta['acaid']); // metaPlanoOrcamentarioDTO
			$ObjetoWS->quantidadeFisico = $proposta['quantidadefisico']; // long
            $ObjetoWS->expansaoFisicaSolicitada = $proposta['acaexpansaoqtdefisico']; //long
			//$ObjetoWS->receitas = $this->getReceitaDTO(); // receitaDTO
			$ObjetoWS->snAtual = '1'; // boolean
			$ObjetoWS->valorFisico = $this->valorfisico; // long
//            var_dump($ObjetoWS->expansaoFisicaSolicitada);
            if (0 !== (int)$ObjetoWS->expansaoFisicaSolicitada) {
//            if (0 !== (int)$this->expansaoSolicitada) {
//            if (0 != $proposta['valor_expansao']) {
//                $ObjetoWS->expansaoFisicaSolicitada = $this->expansaoFisicaSolicitada; // long
                $proposta['justificativa_expansao'] = trim($proposta['justificativa_expansao']);
                if (!empty($proposta['justificativa_expansao'])) {
                    $ObjetoWS->justificativaExpansaoSolicitada = $proposta['justificativa_expansao'];
                }
            }
		} else {
			$ObjetoWS = new propostaDTO();
			$ObjetoWS->codigoAcao = $proposta['acacod']; // string
			$ObjetoWS->codigoEsfera	= $proposta['esfcod']; // string
			$ObjetoWS->codigoFuncao	= $proposta['funcod']; // string
			$ObjetoWS->codigoLocalizador = $proposta['loccod']; // string
			$ObjetoWS->codigoMomento = $this->codigomomento; // int
			$ObjetoWS->codigoOrgao = $proposta['unicod']; // string
			$ObjetoWS->codigoPrograma = $proposta['prgcod']; // string
			$ObjetoWS->codigoSubFuncao = $proposta['sfucod']; // string
			$ObjetoWS->codigoTipoDetalhamento = $proposta['tipodetalhamento']; // string
			$ObjetoWS->codigoTipoInclusaoAcao = '1'; // int
			$ObjetoWS->codigoTipoInclusaoLocalizador = '1'; // int
			$ObjetoWS->exercicio = $_SESSION['exercicio']+1; // int
            $ObjetoWS->justificativa = $proposta['justificativa'];
			$ObjetoWS->financeiros = $this->getFinanceiroDTO( $arrProposta ); // financeiroDTO
			//$ObjetoWS->metaPlanoOrcamentario = $this->getMetaPlanoOrcamentarioDTO( $proposta['acaid'] ); // metaPlanoOrcamentarioDTO
			$ObjetoWS->quantidadeFisico = $proposta['quantidadefisico']; // long
            $ObjetoWS->expansaoFisicaSolicitada = $proposta['acaexpansaoqtdefisico']; //long
			//$ObjetoWS->receitas = $this->getReceitaDTO(); // receitaDTO
			$ObjetoWS->snAtual = '1'; // boolean
			$ObjetoWS->valorFisico = $this->valorfisico; // long
//            var_dump($ObjetoWS->expansaoFisicaSolicitada);
            if (0 !== (int)$ObjetoWS->expansaoFisicaSolicitada) {
//            if (0 !== (int)$this->expansaoSolicitada) {
//            if (0 != $proposta['valor_expansao']) {
//                $ObjetoWS->expansaoFisicaSolicitada = $this->expansaoFisicaSolicitada; // long
                $proposta['justificativa_expansao'] = trim($proposta['justificativa_expansao']);
                if (!empty($proposta['justificativa_expansao'])) {
                    $ObjetoWS->justificativaExpansaoSolicitada = $proposta['justificativa_expansao'];
                }
            }
		}
//        ver($ObjetoWS, d);
		return $ObjetoWS;
	}
	
	private function getMetaPlanoOrcamentarioDTO($acaid)
    {
        $sql = <<<SQL
SELECT mp.mpoid,
       mp.ploid,
       po.ploidentificadorunicosiop,
       mp.mpovalor,
       mp.mpovalorexpansao
  FROM elabrev.metaplanoorcamentario mp
    INNER JOIN elabrev.planoorcamentario po USING(ploid)
  WHERE po.acaid = {$acaid}
    AND po.ploproduto <> ''
    AND mpostatus = 'A'
    AND po.plostatus = 'A'
SQL;

        $arrDados = $this->db->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();

        if ($arrDados) {
            foreach ($arrDados as $key => $v) {
                $$key = new metaPlanoOrcamentarioDTO();
                $$key->identificadorUnicoPlanoOrcamentario = $v['ploidentificadorunicosiop'];
                $$key->quantidadeFisico = $v['mpovalor'];
                $$key->expansaoFisicaSolicitada = $v['mpovalorexpansao'];

                $ObjetoWS[$key] = $$key;
            }
        }
        if (!is_null($ObjetoWS)) {
            return $ObjetoWS;
        }
        return array(new metaPlanoOrcamentarioDTO);
	}

	private function getFinanceiroDTO( $arrProposta ){
		$chave = 0;
		foreach ($arrProposta as $key => $v) {
			if( $v['acaid'] == $this->acaid ){
				if( $v['ploid'] ){
					$sql = "select ploidentificadorunicosiop from elabrev.planoorcamentario pl where ploid = {$v['ploid']}";
					$ploidentificadorunicosiop = $this->db->pegaUm($sql);
				}
				//ver($ploidentificadorunicosiop,$sql,d);
				
				$$chave = new financeiroDTO();
				$$chave->fonte					= $v['foncod'];
				$$chave->identificadorPlanoOrcamentario 	= $ploidentificadorunicosiop; //$v['ploidentificadorunicosiop'];
				$$chave->idOC					= $v['idocod'];
				$$chave->idUso					= $v['iducod'];
				$$chave->naturezaDespesa			= ($v['ndpcod'] ? $v['ndpcod'] : $v['nrcod']);
				$$chave->resultadoPrimarioAtual			= $v['iducod'];
				$$chave->resultadoPrimarioLei			= $v['iducod'];
				$$chave->valor					= $v['valor'];
                                if (0 !== (int)$v['valor_expansao']) {
                                    $$chave->expansaoSolicitada			= $v['valor_expansao'];
                                }
				
				$this->valorfisico				+= $v['valor'];
//				$this->expansaoFisicaSolicitada			+= $v['valor_expansao'];
				       
				$ObjetoWS[$chave] = $$chave;
				$chave++;
			}
		}
		
		return $ObjetoWS;		
	}
	
	private function getReceitaDTO(){
		
		$ObjetoWS = new receitaDTO();
		$ObjetoWS->naturezaReceita	= '';
		$ObjetoWS->valor			= '';
		
		return $ObjetoWS;		
	}
	
	private function getConsultarProposta($proposta, $arrProposta){
		
		$ObjetoWS = new consultarProposta();
		$ObjetoWS->credencial = $this->getCredencial();
		$ObjetoWS->proposta = $this->getPropostaDTO($proposta, $arrProposta);
		
		return $ObjetoWS;
	}
	
	private function getExcluirProposta( $valor, $arrProposta ){
		
		$ObjetoWS = new excluirProposta();
		$ObjetoWS->credencial = $this->getCredencial();
		$ObjetoWS->proposta = $this->getPropostaDTO( $valor, $arrProposta );
		
		return $ObjetoWS;
	}
	
	private function getObterTabelasApoioQuantitativo(){
		
		$ObjetoWS = new obterTabelasApoioQuantitativo();
		$ObjetoWS->credencial 			= $this->getCredencial();
		$ObjetoWS->dataHoraReferencia	= '';
		$ObjetoWS->exercicio			= $_SESSION['exercicio'];
		$ObjetoWS->retornarFontes		= true;
		$ObjetoWS->retornarIdOcs		= true;
		$ObjetoWS->retornarIdUsos		= true;
		$ObjetoWS->retornarNaturezas	= true;
		$ObjetoWS->retornarRPs			= true;
		
		return $ObjetoWS;
	}
	
	private function getObterExecucaoOrcamentariaSam(){
		
		$ObjetoWS = new obterExecucaoOrcamentariaSam();
		$ObjetoWS->anoExercicio = $_SESSION['exercicio']+1;
		$ObjetoWS->credencial	= $this->getCredencial();
		$ObjetoWS->planoInterno	= '';
		
		return $ObjetoWS;
	}
	
	private function carregarDadosProposta( $arrAcao = array() ){
		
		$sql = "SELECT *
                          FROM (SELECT --1,
                                       'EYX' AS login,
                                       MD5('COISUCA') AS senha,
                                       '26000' AS orgao,
                                       acao.acaid,
                                       CAST(acao.acaid AS varchar(5))
                                         || CASE WHEN acao.acacod in('09HB', '2000', '2272', '2991', '2992', '4001',
                                                                     '4006', '4009', '4086', '6318', '6321', '00C5',
                                                                     '0110', '00H1')
                                                   AND SUBSTR(nd.ndpcod, 1,2) <> '31' THEN '1' ELSE CAST(tda.tpdid AS varchar(2))
                                              END AS codigo,
                                       acao.esfcod,
                                       acao.unicod,
                                       acao.funcod,
                                       acao.sfucod,
                                       acao.prgcod,
                                       acao.acacod,
                                       acao.loccod,
                                       acao.acapropostaenviada,
                                       CASE WHEN acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                '4009', '4086', '6318', '6321','00C5','0110','00H1')
                                              AND SUBSTR(nd.ndpcod, 1,2) <> '31' THEN 1 ELSE tda.tpdid
                                         END AS tipodetalhamento,
                                       CASE WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(coalesce(acadscunmsof, '')) <> '' THEN COALESCE(acaqtdefisico, 1)
                                            WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(coalesce(acadscunmsof, '')) = '' THEN NULL
                                            WHEN tda.tpdid IN(5)
                                              AND acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                 '4009', '4086', '6318', '6321', '00C5', '0110', '00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) <> '31' and trim(coalesce(acadscunmsof,'')) <> '' THEN COALESCE(acaqtdefisico, 1)
					    WHEN tda.tpdid IN(5)
                                              AND acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                 '4009', '4086', '6318', '6321', '00C5', '0110', '0181',
                                                                 '00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) <> '31'
                                              AND TRIM(COALESCE(acadscunmsof, '')) = '' THEN NULL
                                            ELSE COALESCE(acaqtdefisico, 1)
                                         END AS quantidadefisico,
                                       acaexpansaoqtdefisico,
                                       z.valor AS valorfisico,
                                       justificativa AS justificativa,
                                       acao.justificativaexpansao AS justificativa_expansao,
                                       '" . (((int)$_SESSION['exercicio']) + 1) . "' AS ano,
                                       da.iducod,
                                       idoc.idocod,
                                       SUBSTR(nd.ndpcod, 1, 6) || '00' AS ndpcod,
                                       da.foncod,
                                       da.ploid,
                                       COALESCE(SUM(da.dpavalor), 0) AS valor,
                                       COALESCE(SUM(da.dpavalorexpansao), 0) AS valor_expansao, -- // ??
                                       '' AS nrcod,
                                       0 AS valor_receita
                                FROM elabrev.despesaacao da
                                    INNER JOIN elabrev.ppaacao_orcamento acao ON acao.acaid = da.acaid 
                                    INNER JOIN (SELECT a.acaid,
                                                       COALESCE(SUM(a.dpavalor), 0) AS valor
                                                  FROM elabrev.despesaacao a 
                                                    INNER JOIN elabrev.ppaacao_orcamento acao ON acao.acaid = a.acaid 
                                                    INNER JOIN naturezadespesa nd ON nd.ndpid = a.ndpid 
                                                  WHERE prgano = '{$_SESSION['exercicio']}' -- // ?? Definido via variável
                                                    AND SUBSTR(nd.ndpcod, 1, 2) <> '31'
                                                  GROUP by a.acaid ) z ON z.acaid = da.acaid
                                    INNER JOIN naturezadespesa nd ON nd.ndpid = da.ndpid
                                    INNER JOIN idoc ON idoc.idoid = da.idoid
                                    INNER JOIN (SELECT ao.acaid,
                                                       MAX(tpdid) AS tpdid
                                                  FROM elabrev.tipodetalhamentoacao a 
                                                    INNER JOIN elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid 
                                                  WHERE ao.prgano = '{$_SESSION['exercicio']}' -- // ?? Definido via variável
                                                  GROUP BY ao.acaid) tda ON tda.acaid = acao.acaid
                                    --LEFT JOIN naturezareceita nr ON nr.nrcid = da.nrcid -- // ?? Originalmente comentado
                                WHERE 1 = 1
                                  AND da.ppoid = 167
                                  AND acao.acacod NOT IN('2004', '2010', '2011', '2012', '20CW', '20CE', '0284')
                                  AND SUBSTR(nd.ndpcod, 1, 2) <> '31'
                                  AND nd.ndpcod NOT IN('33901514', '33901516', '33903017', '33903504', '33903603',
                                                       '33903615', '33903618', '33903622', '33903654', '33903655',
                                                       '33903657', '33903709', '33903727', '33903728', '33903908',
                                                       '33903911', '33903926', '33903927', '33903928', '33903930',
                                                       '33903931', '33903957', '33903995', '33903997', '33913017',
                                                       '33913504', '33913618', '33913654', '33913655', '33913657',
                                                       '33913709', '33913727', '33913728', '33913908', '33913911',
                                                       '33913926', '33913927', '33913928', '33913930', '33913931',
                                                       '33913957', '33913995', '33913997', '44903017', '44903504',
                                                       '44903615', '44903622', '44903645', '44903646', '44903654',
                                                       '44903655', '44903657', '44903792', '44903793', '44903910',
                                                       '44903912', '44903957', '44903992', '44903993', '44903994',
                                                       '44903995', '44905100', '44905196', '44905199', '44905235',
                                                       '44905250', '44905253', '44913017', '44913504', '44913645',
                                                       '44913646', '44913654', '44913655', '44913657', '44913792',
                                                       '44913793', '44913957', '44913992', '44913993', '44913994',
                                                       '44913995', '44915235', '45906100', '45906102', '45906103',
                                                       '45906104', '45906105', '45906106', '45906107', '45906108',
                                                       '45906191', '45906192', '45906199')
				GROUP BY acao.acaid,
                                         acao.esfcod,
                                         acao.unicod,
                                         acao.funcod,
                                         acao.sfucod,
                                         acao.prgcod,
                                         acao.acacod,
                                         acao.loccod,
                                         acao.acapropostaenviada,
                                         tda.tpdid,
                                         acaqtdefisico,
                                         acaqtdefinanceiro,
                                         justificativa,
                                         SUBSTR(nd.ndpcod, 1, 2),
                                         SUBSTR(nd.ndpcod, 1, 6) || '00',
                                         da.iducod,
                                         da.foncod,
                                         da.ploid,
                                         idoc.idocod,
                                         acao.acaqtdefisico,
                                         acao.acaexpansaoqtdefisico,
                                         acao.tdecod,
                                         acao.justificativa,
                                         acao.justificativaexpansao,
                                         acadscunmsof,
                                         z.valor
                                UNION ALL
                                SELECT --2,
                                       'EYX' AS login,
                                       MD5('COISUCA') AS senha,
                                       '26000' AS orgao,
                                       acao.acaid,
                                       CAST(acao.acaid AS varchar(5))
                                           || CASE WHEN acao.acacod IN ('09HB', '2000', '2272', '2991', '2992', '4001',
                                                                        '4006', '4009', '4086', '6318', '6321', '00C5',
                                                                        '0110', '00H1')
                                                     AND SUBSTR(nd.ndpcod, 1, 2) <> '31' THEN '1'
                                                ELSE CAST(tda.tpdid AS varchar(2)) end as codigo,
                                       acao.esfcod,
                                       acao.unicod,
                                       acao.funcod,
                                       acao.sfucod,
                                       acao.prgcod,
                                       acao.acacod,
                                       acao.loccod,
                                       acao.acapropostaenviada,
                                       CASE WHEN acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                '4009', '4086', '6318', '6321', '00C5', '0110', '00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) <> '31' THEN 1
                                         ELSE tda.tpdid end as tipodetalhamento,
                                       CASE WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(COALESCE(acadscunmsof,'')) <> '' THEN COALESCE(acaqtdefisico, 1)
                                            WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(COALESCE(acadscunmsof,'')) = '' THEN NULL
					    WHEN tda.tpdid IN(5)
                                              AND acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                 '4009', '4086', '6318', '6321', '00C5', '0110', '0181',
                                                                 '00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) = '31' THEN NULL
					    WHEN tda.tpdid IN(5)
                                              AND acao.acacod NOT IN('09HB', '2000', '2272', '2991', '2992', '4001',
                                                                     '4006', '4009', '4086', '6318', '6321', '00C5',
                                                                     '0110', '00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) = '31'
                                              AND TRIM(COALESCE(acadscunmsof,'')) <> '' THEN COALESCE(acaqtdefisico, 1)
					    ELSE COALESCE(acaqtdefisico, 1)
                                         END AS quantidadefisico,
                                       acaexpansaoqtdefisico,
                                       z.valor AS valorfisico,
                                       justificativa AS justificativa,
                                       acao.justificativaexpansao AS justificativa_expansao,
                                       '" . (((int)$_SESSION['exercicio']) + 1) . "' AS ano,
                                       da.iducod,
                                       idoc.idocod,
                                       SUBSTR(nd.ndpcod, 1, 6) || '00' AS ndpcod,
                                       da.foncod,
                                       da.ploid,
                                       COALESCE(SUM(da.dpavalor), 0) AS valor,
                                       COALESCE(SUM(da.dpavalorexpansao), 0) AS valor_expansao, -- // ??
                                       '' AS nrcod,
                                       0 AS valor_receita
                                  FROM elabrev.despesaacao da
                                    INNER JOIN elabrev.ppaacao_orcamento acao ON acao.acaid = da.acaid
                                    INNER JOIN (SELECT a.acaid,
                                                       COALESCE(SUM(a.dpavalor), 0) AS valor
                                                  FROM elabrev.despesaacao a
                                                    INNER JOIN elabrev.ppaacao_orcamento acao ON acao.acaid = a.acaid
                                                    INNER JOIN naturezadespesa nd ON nd.ndpid = a.ndpid
                                                  WHERE prgano = '{$_SESSION['exercicio']}' -- // ?? Definido via variável
                                                    AND SUBSTR(nd.ndpcod, 1, 2) = '31'
                                                  GROUP BY a.acaid ) z ON z.acaid = da.acaid
                                    INNER JOIN naturezadespesa nd ON nd.ndpid = da.ndpid 
                                    INNER JOIN idoc ON idoc.idoid = da.idoid 
                                    INNER JOIN (SELECT ao.acaid,
                                                       MAX(tpdid) AS tpdid
                                                  FROM elabrev.tipodetalhamentoacao a
                                                    INNER JOIN elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
                                                  WHERE ao.prgano = '{$_SESSION['exercicio']}' -- // ?? Definido via variável
                                                  GROUP BY ao.acaid) tda ON tda.acaid = acao.acaid
                                    --LEFT JOIN naturezareceita nr ON nr.nrcid = da.nrcid -- // ?? Originalmente comentado
                                  WHERE 1 = 1
                                    AND da.ppoid = 167
                                    AND acao.acacod NOT IN('2004', '2010', '2011', '2012', '20CW', '20CE', '0284')
                                    AND SUBSTR(nd.ndpcod, 1, 2) = '31'
                                    AND nd.ndpcod NOT IN('33901514', '33901516', '33903017', '33903504', '33903603',
                                                         '33903615', '33903618', '33903622', '33903654', '33903655',
                                                         '33903657', '33903709', '33903727', '33903728', '33903908',
                                                         '33903911', '33903926', '33903927', '33903928', '33903930',
                                                         '33903931', '33903957', '33903995', '33903997', '33913017',
                                                         '33913504', '33913618', '33913654', '33913655', '33913657',
                                                         '33913709', '33913727', '33913728', '33913908', '33913911',
                                                         '33913926', '33913927', '33913928', '33913930', '33913931',
                                                         '33913957', '33913995', '33913997', '44903017', '44903504',
                                                         '44903615', '44903622', '44903645', '44903646', '44903654',
                                                         '44903655', '44903657', '44903792', '44903793', '44903910',
                                                         '44903912', '44903957', '44903992', '44903993', '44903994',
                                                         '44903995', '44905100', '44905196', '44905199', '44905235',
                                                         '44905250', '44905253', '44913017', '44913504', '44913645',
                                                         '44913646', '44913654', '44913655', '44913657', '44913792',
                                                         '44913793', '44913957', '44913992', '44913993', '44913994',
                                                         '44913995', '44915235', '45906100', '45906102', '45906103',
                                                         '45906104', '45906105', '45906106', '45906107', '45906108',
                                                         '45906191', '45906192', '45906199')
                                  GROUP BY acao.acaid,
                                           acao.esfcod,
                                           acao.unicod,
                                           acao.funcod,
                                           acao.sfucod,
                                           acao.prgcod,
                                           acao.acacod,
                                           acao.loccod,
                                           acao.acapropostaenviada,
                                           tda.tpdid,
                                           acaqtdefisico,
                                           acaqtdefinanceiro,
                                           justificativa,
                                           SUBSTR(nd.ndpcod, 1, 2),
                                           SUBSTR(nd.ndpcod, 1, 6) || '00',
                                           da.iducod,
                                           da.foncod,
                                           da.ploid,
                                           idoc.idocod,
                                           acao.acaqtdefisico,
                                           acao.acaexpansaoqtdefisico,
                                           acao.tdecod,
                                           acao.justificativa,
                                           acao.justificativaexpansao,
                                           acadscunmsof,
                                           z.valor
                                UNION ALL
                                SELECT --3,
                                       'EYX' AS login,
                                       MD5('COISUCA') AS senha,
                                       '26000' AS orgao,
                                       acao.acaid,
                                       CAST(acao.acaid AS varchar(5))
                                           || CASE WHEN acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001',
                                                                       '4006', '4009', '4086', '6318', '6321', '00C5',
                                                                       '0110', '00H1')
                                                AND SUBSTR(nr.nrccod, 1, 2) <> '31' THEN '1' ELSE CAST(tda.tpdid as varchar(2))
                                         END AS codigo,
                                       acao.esfcod, 
                                       acao.unicod,
                                       acao.funcod,
                                       acao.sfucod,
                                       acao.prgcod,
                                       acao.acacod,
                                       acao.loccod,
                                       acao.acapropostaenviada,
                                       CASE WHEN acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                '4009', '4086', '6318', '6321', '00C5', '0110', '00H1')
                                              AND SUBSTR(nr.nrccod, 1, 2) <> '31' THEN 1 ELSE tda.tpdid
                                         END AS tipodetalhamento,
                                       CASE WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(COALESCE(acadscunmsof, '')) <> '' THEN COALESCE(acaqtdefisico, 1)
                                            WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(COALESCE(acadscunmsof, '')) = '' THEN NULL
                                            WHEN tda.tpdid in(5)
                                              AND acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                 '4009', '4086', '6318', '6321', '00C5', '0110', '00H1')
                                              AND SUBSTR(nr.nrccod, 1, 2) <> '31'
                                              AND TRIM(COALESCE(acadscunmsof, '')) <> '' THEN COALESCE(acaqtdefisico, 1)
                                            WHEN tda.tpdid IN(5)
                                              AND acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                 '4009', '4086', '6318', '6321', '00C5', '0110', '0181',
                                                                 '00H1')
                                              AND SUBSTR(nr.nrccod, 1, 2) <> '31'
                                              AND TRIM(COALESCE(acadscunmsof, '')) = '' THEN NULL
					    ELSE COALESCE(acaqtdefisico, 1)
                                         END AS quantidadefisico,
                                       acaexpansaoqtdefisico,
                                       CASE WHEN acaqtdefinanceiro IS NULL THEN COALESCE(SUM(da.dpavalor), 0)
                                              ELSE acaqtdefinanceiro
                                         END AS valorfisico, 
				       justificativa AS justificativa,
                                       acao.justificativaexpansao AS justificativa_expansao,
				       '" . (((int)$_SESSION['exercicio']) + 1) . "' AS ano,
                                       da.iducod,
                                       idoc.idocod,
                                       '' AS ndpcod,
                                       da.foncod,
                                       da.ploid, 
				       0 AS valor,
                                       COALESCE(SUM(da.dpavalorexpansao), 0) AS valor_expansao, -- // ??
                                       SUBSTR(nr.nrccod, 1, 6) || '00' AS nrccod,
				       COALESCE(SUM(da.dpavalor), 0) AS valor_receita
                                  FROM elabrev.despesaacao da
                                    INNER JOIN elabrev.ppaacao_orcamento acao ON acao.acaid = da.acaid
                                    INNER JOIN naturezareceita nr ON nr.nrcid = da.nrcid
                                    INNER JOIN idoc ON idoc.idoid = da.idoid
                                    INNER JOIN (SELECT ao.acaid,
                                                       MAX(tpdid) AS tpdid
                                                  FROM elabrev.tipodetalhamentoacao a
                                                    INNER JOIN elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
                                                  WHERE ao.prgano = '{$_SESSION['exercicio']}' -- // ?? Definido via variável
                                                  GROUP BY ao.acaid) tda ON tda.acaid = acao.acaid
                                  WHERE 1 = 1
                                    AND da.ppoid = 167
                                    AND acao.acacod NOT IN('2004', '2010', '2011', '2012', '20CW', '20CE', '0284')
                                    AND nr.nrccod NOT IN('33901514', '33901516', '33903017', '33903504', '33903603',
                                                         '33903615', '33903618', '33903622', '33903654', '33903655',
                                                         '33903657', '33903709', '33903727', '33903728', '33903908',
                                                         '33903911', '33903926', '33903927', '33903928', '33903930',
                                                         '33903931', '33903957', '33903995', '33903997', '33913017',
                                                         '33913504', '33913618', '33913654', '33913655', '33913657',
                                                         '33913709', '33913727', '33913728', '33913908', '33913911',
                                                         '33913926', '33913927', '33913928', '33913930', '33913931',
                                                         '33913957', '33913995', '33913997', '44903017', '44903504',
                                                         '44903615', '44903622', '44903645', '44903646', '44903654',
                                                         '44903655', '44903657', '44903792', '44903793', '44903910',
                                                         '44903912', '44903957', '44903992', '44903993', '44903994',
                                                         '44903995', '44905100', '44905196', '44905199', '44905235',
                                                         '44905250', '44905253', '44913017', '44913504', '44913645',
                                                         '44913646', '44913654', '44913655', '44913657', '44913792',
                                                         '44913793', '44913957', '44913992', '44913993', '44913994',
                                                         '44913995', '44915235', '45906100', '45906102', '45906103',
                                                         '45906104', '45906105', '45906106', '45906107', '45906108',
                                                         '45906191', '45906192', '45906199')
                                  GROUP BY acao.acaid,
                                           acao.esfcod,
                                           acao.unicod,
                                           acao.funcod,
                                           acao.sfucod,
                                           acao.prgcod,
                                           acao.acacod,
                                           acao.loccod,
                                           acao.acapropostaenviada,
                                           tda.tpdid,
                                           acaqtdefisico,
                                           acaqtdefinanceiro,
                                           justificativa,
                                           da.iducod,
                                           da.foncod,
                                           da.ploid,
                                           substr(nr.nrccod, 1, 2),
                                           substr(nr.nrccod, 1, 6) || '00',
                                           idoc.idocod,
                                           acao.acaqtdefisico,
                                           acao.acaexpansaoqtdefisico,
                                           acao.tdecod,
                                           acao.justificativa,
                                           acao.justificativaexpansao,
                                           acadscunmsof
                                UNION ALL
                                /* Detalhamento nas Naturezas de Inform?tica que devem ser enviadas por Subelemento */
                                SELECT --4,
                                       'EYX' AS login,
                                       MD5('COISUCA') AS senha,
                                       '26000' AS orgao,
                                       acao.acaid,
                                       CAST(acao.acaid AS varchar(5))
                                           || CASE WHEN acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001',
                                                                       '4006', '4009', '4086', '6318', '6321', '00C5',
                                                                       '0110','00H1')
                                                     AND SUBSTR(nd.ndpcod, 1, 2) <> '31' THEN '1' ELSE CAST(tda.tpdid AS varchar(2))
                                         END AS codigo,
                                       acao.esfcod,
                                       acao.unicod,
                                       acao.funcod,
                                       acao.sfucod,
                                       acao.prgcod,
                                       acao.acacod,
                                       acao.loccod,
                                       acao.acapropostaenviada,
                                       CASE WHEN acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                '4009', '4086', '6318', '6321','00C5','0110','00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) <> '31' THEN 1
                                            ELSE tda.tpdid
                                         END AS tipodetalhamento,
                                       CASE WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(COALESCE(acadscunmsof, '')) <> '' THEN COALESCE(acaqtdefisico, 1)
                                            WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(COALESCE(acadscunmsof, '')) = '' THEN NULL
                                            WHEN tda.tpdid IN(5)
                                              AND acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                 '4009', '4086', '6318', '6321', '00C5', '0110', '00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) <> '31'
                                              AND TRIM(COALESCE(acadscunmsof, '')) <> '' THEN COALESCE(acaqtdefisico, 1)
                                            WHEN tda.tpdid IN(5)
                                              AND acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                 '4009', '4086', '6318', '6321', '00C5', '0110', '0181',
                                                                 '00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) <> '31'
                                              AND TRIM(COALESCE(acadscunmsof, '')) = '' THEN NULL
					    ELSE COALESCE(acaqtdefisico, 1)
                                         END AS quantidadefisico,
                                       acaexpansaoqtdefisico,
                                       z.valor AS valorfisico,
                                       justificativa AS justificativa,
                                       acao.justificativaexpansao AS justificativa_expansao,
                                       '" . (((int)$_SESSION['exercicio']) + 1) . "' AS ano,
                                       da.iducod,
                                       idoc.idocod,
                                       nd.ndpcod AS ndpcod,
                                       da.foncod,
                                       da.ploid,
                                       COALESCE(SUM(da.dpavalor), 0) AS valor,
                                       COALESCE(SUM(da.dpavalorexpansao), 0) AS valor_expansao, -- // ??
                                       '' AS nrcod,
                                       0 AS valor_receita
                                  FROM elabrev.despesaacao da
                                    INNER JOIN elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid
                                    INNER JOIN (SELECT a.acaid,
                                                       COALESCE(SUM(a.dpavalor), 0) AS valor
                                                  FROM elabrev.despesaacao a
                                                    INNER JOIN elabrev.ppaacao_orcamento acao ON acao.acaid = a.acaid
                                                    INNER JOIN naturezadespesa nd ON nd.ndpid = a.ndpid
                                                  WHERE prgano = '{$_SESSION['exercicio']}' -- // ?? Definido via variável
                                                    AND SUBSTR(nd.ndpcod, 1, 2) <>'31'
                                                  GROUP BY a.acaid ) z ON z.acaid = da.acaid
                                    INNER JOIN naturezadespesa nd ON nd.ndpid = da.ndpid
                                    INNER JOIN idoc on idoc.idoid = da.idoid
                                    INNER JOIN (SELECT ao.acaid,
                                                       MAX(tpdid) AS tpdid
                                                  FROM elabrev.tipodetalhamentoacao a
                                                    INNER JOIN elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
                                                  WHERE ao.prgano = '{$_SESSION['exercicio']}' -- // ?? Definido via variável
                                                  GROUP BY ao.acaid) tda ON tda.acaid = acao.acaid
                                    --LEFT JOIN naturezareceita nr ON nr.nrcid = da.nrcid -- // ?? Originalmente comentado
                                  WHERE 1 = 1
                                    AND da.ppoid = 167
                                    AND acao.acacod NOT IN('2004','2010','2011','2012','20CW','20CE', '0284')
                                    AND SUBSTR(nd.ndpcod, 1, 2) <> '31'
                                    AND nd.ndpcod IN('33901514', '33901516', '33903017', '33903504', '33903603',
                                                     '33903615', '33903618', '33903622', '33903654', '33903655',
                                                     '33903657', '33903709', '33903727', '33903728', '33903908',
                                                     '33903911', '33903926', '33903927', '33903928', '33903930',
                                                     '33903931', '33903957', '33903995', '33903997', '33913017',
                                                     '33913504', '33913618', '33913654', '33913655', '33913657',
                                                     '33913709', '33913727', '33913728', '33913908', '33913911',
                                                     '33913926', '33913927', '33913928', '33913930', '33913931',
                                                     '33913957', '33913995', '33913997', '44903017', '44903504',
                                                     '44903615', '44903622', '44903645', '44903646', '44903654',
                                                     '44903655', '44903657', '44903792', '44903793', '44903910',
                                                     '44903912', '44903957', '44903992', '44903993', '44903994',
                                                     '44903995', '44905100', '44905196', '44905199', '44905235',
                                                     '44905250', '44905253', '44913017', '44913504', '44913645',
                                                     '44913646', '44913654', '44913655', '44913657', '44913792',
                                                     '44913793', '44913957', '44913992', '44913993', '44913994',
                                                     '44913995', '44915235', '45906100', '45906102', '45906103',
                                                     '45906104', '45906105', '45906106', '45906107', '45906108',
                                                     '45906191', '45906192', '45906199')
                                  GROUP BY acao.acaid,
                                           acao.esfcod,
                                           acao.unicod,
                                           acao.funcod,
                                           acao.sfucod,
                                           acao.prgcod,
                                           acao.acacod,
                                           acao.loccod,
                                           acao.acapropostaenviada,
                                           tda.tpdid,
                                           acaqtdefisico,
                                           acaqtdefinanceiro,
                                           justificativa,
                                           nd.ndpcod,
                                           da.iducod,
                                           da.foncod,
                                           da.ploid,
                                           idoc.idocod,
                                           acao.acaqtdefisico,
                                           acao.acaexpansaoqtdefisico,
                                           acao.tdecod,
                                           acao.justificativa,
                                           acao.justificativaexpansao,
                                           acadscunmsof,
                                           z.valor
                                UNION ALL
                                SELECT --5,
                                       'EYX' AS login, 
                                       MD5('COISUCA') AS senha,
                                       '26000' AS orgao,
                                       acao.acaid,
                                       CAST(acao.acaid AS varchar(5))
                                           || CASE WHEN acao.acacod IN ('09HB', '2000', '2272', '2991', '2992', '4001',
                                                                        '4006', '4009', '4086', '6318', '6321','00C5',
                                                                        '0110','00H1')
                                                     AND SUBSTR(nd.ndpcod, 1, 2) <> '31' THEN '1'
                                                   ELSE CAST(tda.tpdid AS varchar(2))
                                                END AS codigo,
                                       acao.esfcod,
                                       acao.unicod,
                                       acao.funcod,
                                       acao.sfucod,
                                       acao.prgcod,
                                       acao.acacod,
                                       acao.loccod,
                                       acao.acapropostaenviada,
                                       CASE WHEN acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                '4009', '4086', '6318', '6321', '00C5', '0110', '00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) <> '31' THEN 1
                                            ELSE tda.tpdid
                                         END AS tipodetalhamento,
                                       CASE WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(COALESCE(acadscunmsof, '')) <> '' THEN COALESCE(acaqtdefisico, 1)
                                            WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(COALESCE(acadscunmsof, '')) = '' THEN NULL
                                            WHEN tda.tpdid IN(5)
                                              AND acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                 '4009', '4086', '6318', '6321', '00C5', '0110', '0181',
                                                                 '00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) = '31' THEN NULL
                                            WHEN tda.tpdid IN(5)
                                              AND acao.acacod NOT IN('09HB', '2000', '2272', '2991', '2992', '4001',
                                                                     '4006', '4009', '4086', '6318', '6321', '00C5',
                                                                     '0110', '00H1')
                                              AND SUBSTR(nd.ndpcod, 1, 2) = '31'
                                              AND TRIM(COALESCE(acadscunmsof, '')) <> '' THEN COALESCE(acaqtdefisico, 1)
                                            ELSE COALESCE(acaqtdefisico, 1)
                                         END AS quantidadefisico,
                                       acaexpansaoqtdefisico,
                                       z.valor AS valorfisico,
                                       justificativa AS justificativa,
                                       acao.justificativaexpansao AS justificativa_expansao,
                                       '" . (((int)$_SESSION['exercicio']) + 1) . "' AS ano,
                                       da.iducod,
                                       idoc.idocod,
                                       nd.ndpcod AS ndpcod,
                                       da.foncod,
                                       da.ploid,
                                       COALESCE(SUM(da.dpavalor), 0) AS valor,
                                       COALESCE(SUM(da.dpavalorexpansao), 0) AS valor_expansao, -- // ??
                                       '' AS nrcod,
                                       0 AS valor_receita
                                  FROM elabrev.despesaacao da
                                    INNER JOIN elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid
                                    INNER JOIN (SELECT a.acaid,
                                                       COALESCE(SUM(a.dpavalor), 0) AS valor
                                                  FROM elabrev.despesaacao a
                                                    INNER JOIN elabrev.ppaacao_orcamento acao ON acao.acaid = a.acaid
                                                    INNER JOIN naturezadespesa nd ON nd.ndpid = a.ndpid
                                                  WHERE prgano = '{$_SESSION['exercicio']}' -- // ?? Definido via variável
                                                    AND SUBSTR(nd.ndpcod, 1, 2) = '31'
                                                  GROUP BY a.acaid) z ON z.acaid = da.acaid
                                    INNER JOIN naturezadespesa nd ON nd.ndpid = da.ndpid
                                    INNER JOIN idoc ON idoc.idoid = da.idoid
                                    INNER JOIN (SELECT ao.acaid,
                                                       MAX(tpdid) AS tpdid
                                                  FROM elabrev.tipodetalhamentoacao a
                                                    INNER JOIN elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
                                                  WHERE ao.prgano = '{$_SESSION['exercicio']}' -- // ?? Definido via variável
                                                  GROUP BY ao.acaid) tda ON tda.acaid = acao.acaid
                                    --LEFT JOIN naturezareceita nr ON nr.nrcid = da.nrcid -- // ?? Originalmente comentado
                                  WHERE 1 = 1
                                    AND da.ppoid = 167
                                    AND acao.acacod NOT IN('2004', '2010', '2011', '2012', '20CW', '20CE','0284')
                                    AND SUBSTR(nd.ndpcod, 1, 2) = '31'
                                    AND nd.ndpcod IN('33901514', '33901516', '33903017', '33903504', '33903603', '33903615',
                                                     '33903618', '33903622', '33903654', '33903655', '33903657', '33903709',
                                                     '33903727', '33903728', '33903908', '33903911', '33903926', '33903927',
                                                     '33903928', '33903930', '33903931', '33903957', '33903995', '33903997',
                                                     '33913017', '33913504', '33913618', '33913654', '33913655', '33913657',
                                                     '33913709', '33913727', '33913728', '33913908', '33913911', '33913926',
                                                     '33913927', '33913928', '33913930', '33913931', '33913957', '33913995',
                                                     '33913997', '44903017', '44903504', '44903615', '44903622', '44903645',
                                                     '44903646', '44903654', '44903655', '44903657', '44903792', '44903793',
                                                     '44903910', '44903912', '44903957', '44903992', '44903993', '44903994',
                                                     '44903995', '44905100', '44905196', '44905199', '44905235', '44905250',
                                                     '44905253', '44913017', '44913504', '44913645', '44913646', '44913654',
                                                     '44913655', '44913657', '44913792', '44913793', '44913957', '44913992',
                                                     '44913993', '44913994', '44913995', '44915235', '45906100', '45906102',
                                                     '45906103', '45906104', '45906105', '45906106', '45906107', '45906108',
                                                     '45906191', '45906192', '45906199')
                                  GROUP BY acao.acaid,
                                           acao.esfcod,
                                           acao.unicod,
                                           acao.funcod,
                                           acao.sfucod,
                                           acao.prgcod,
                                           acao.acacod,
                                           acao.loccod,
                                           acao.acapropostaenviada,
                                           tda.tpdid,
                                           acaqtdefisico,
                                           acaqtdefinanceiro,
                                           justificativa,
                                           nd.ndpcod,
                                           da.iducod,
                                           da.foncod,
                                           da.ploid,
                                           idoc.idocod,
                                           acao.acaqtdefisico,
                                           acao.acaexpansaoqtdefisico,
                                           acao.tdecod,
                                           acao.justificativa,
                                           acao.justificativaexpansao,
                                           acadscunmsof,
                                           z.valor
                                UNION ALL
                                SELECT --6,
                                       'EYX' AS login,
                                       MD5('COISUCA') AS senha,
                                       '26000' AS orgao,
                                       acao.acaid,
                                       CAST(acao.acaid AS varchar(5))
                                           || CASE WHEN acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001',
                                                                       '4006', '4009', '4086', '6318', '6321', '00C5',
                                                                       '0110', '00H1')
                                                     AND SUBSTR(nr.nrccod, 1, 2) <> '31' THEN '1'
                                                   ELSE CAST(tda.tpdid as varchar(2))
                                                  END AS codigo,
                                       acao.esfcod,
                                       acao.unicod,
                                       acao.funcod,
                                       acao.sfucod,
                                       acao.prgcod,
                                       acao.acacod,
                                       acao.loccod,
                                       acao.acapropostaenviada,
                                       CASE WHEN acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                '4009', '4086', '6318', '6321', '00C5', '0110', '00H1')
                                              AND SUBSTR(nr.nrccod, 1, 2) <> '31' THEN 1
                                            ELSE tda.tpdid
                                         END AS tipodetalhamento,
                                       CASE WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(COALESCE(acadscunmsof, '')) <> '' THEN COALESCE(acaqtdefisico, 1)
                                            WHEN tda.tpdid IN(1, 2, 3, 4, 6, 7, 8, 10, 12)
                                              AND TRIM(COALESCE(acadscunmsof, '')) = '' THEN NULL
                                            WHEN tda.tpdid IN(5)
                                              AND acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                 '4009', '4086', '6318', '6321', '00C5', '0110', '00H1')
                                              AND SUBSTR(nr.nrccod, 1, 2) <> '31'
                                              AND TRIM(COALESCE(acadscunmsof, '')) <> '' THEN COALESCE(acaqtdefisico, 1)
                                           WHEN tda.tpdid IN(5)
                                              AND acao.acacod IN('09HB', '2000', '2272', '2991', '2992', '4001', '4006',
                                                                 '4009', '4086', '6318', '6321', '00C5', '0110', '0181',
                                                                 '00H1')
                                              AND SUBSTR(nr.nrccod, 1, 2) <> '31'
                                              AND TRIM(COALESCE(acadscunmsof, '')) = '' THEN NULL
                                           ELSE COALESCE(acaqtdefisico, 1)
                                         END AS quantidadefisico,
                                       acaexpansaoqtdefisico,
                                       CASE WHEN acaqtdefinanceiro IS NULL THEN COALESCE(SUM(da.dpavalor), 0)
                                            ELSE acaqtdefinanceiro
                                         END AS valorfisico,
                                       justificativa as justificativa,
                                       acao.justificativaexpansao AS justificativa_expansao,
                                       '" . (((int)$_SESSION['exercicio']) + 1) . "' AS ano,
                                       da.iducod,
                                       idoc.idocod,
                                       '' AS ndpcod,
                                       da.foncod,
                                       da.ploid,
                                       0 AS valor,
                                       COALESCE(SUM(da.dpavalorexpansao), 0) AS valor_expansao, -- // ??
                                       nr.nrccod AS nrccod,
                                       COALESCE(SUM(da.dpavalor), 0) AS valor_receita
                                  FROM elabrev.despesaacao da
                                    INNER JOIN elabrev.ppaacao_orcamento acao ON acao.acaid = da.acaid
                                    INNER JOIN naturezareceita nr ON nr.nrcid = da.nrcid
                                    INNER JOIN idoc ON idoc.idoid = da.idoid
                                    INNER JOIN (SELECT ao.acaid,
                                                       MAX(tpdid) AS tpdid
                                                  FROM elabrev.tipodetalhamentoacao a
                                                    INNER JOIN elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid
                                                  WHERE ao.prgano = '{$_SESSION['exercicio']}' -- // ?? Definido via variável
                                                  GROUP BY ao.acaid ) tda ON tda.acaid = acao.acaid
                                  WHERE 1 = 1
                                    AND da.ppoid = 167
                                    AND acao.acacod NOT IN ('2004', '2010', '2011', '2012', '20CW', '20CE', '0284')
                                    AND nr.nrccod IN('33901514', '33901516', '33903017', '33903504', '33903603',
                                                     '33903615', '33903618', '33903622', '33903654', '33903655',
                                                     '33903657', '33903709', '33903727', '33903728', '33903908',
                                                     '33903911', '33903926', '33903927', '33903928', '33903930',
                                                     '33903931', '33903957', '33903995', '33903997', '33913017',
                                                     '33913504', '33913618', '33913654', '33913655', '33913657',
                                                     '33913709', '33913727', '33913728', '33913908', '33913911',
                                                     '33913926', '33913927', '33913928', '33913930', '33913931',
                                                     '33913957', '33913995', '33913997', '44903017', '44903504',
                                                     '44903615', '44903622', '44903645', '44903646', '44903654',
                                                     '44903655', '44903657', '44903792', '44903793', '44903910',
                                                     '44903912', '44903957', '44903992', '44903993', '44903994',
                                                     '44903995', '44905100', '44905196', '44905199', '44905235',
                                                     '44905250', '44905253', '44913017', '44913504', '44913645',
                                                     '44913646', '44913654', '44913655', '44913657', '44913792',
                                                     '44913793', '44913957', '44913992', '44913993', '44913994',
                                                     '44913995', '44915235', '45906100', '45906102', '45906103',
                                                     '45906104', '45906105', '45906106', '45906107', '45906108',
                                                     '45906191', '45906192', '45906199')
                                  GROUP BY acao.acaid,
                                           acao.esfcod,
                                           acao.unicod,
                                           acao.funcod,
                                           acao.sfucod,
                                           acao.prgcod,
                                           acao.acacod,
                                           acao.loccod,
                                           acao.acapropostaenviada,
                                           tda.tpdid,
                                           acaqtdefisico,
                                           acaqtdefinanceiro,
                                           justificativa,
                                           da.iducod,
                                           da.foncod,
                                           da.ploid,
                                           nr.nrccod,
                                           idoc.idocod,
                                           acao.acaqtdefisico,
                                           acao.acaexpansaoqtdefisico,
                                           acao.tdecod,
                                           acao.justificativa,
                                           acao.justificativaexpansao,
                                           acadscunmsof) AS foo
                          WHERE acaid IN(" . implode(', ', $arrAcao) . ") -- // ?? Definido via variável
                            AND acapropostaenviada = false
                          ORDER BY esfcod,
                                   unicod,
                                   funcod,
                                   sfucod,
                                   prgcod,
                                   acacod,
                                   loccod,
                                   tipodetalhamento";
//                ver($sql, d);
		$arrDados = $this->db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
//		ver($arrDados, d);
		return $arrDados;
	}

    private function carregaDadosAcompanhamento($arrAcao = array())
    {

/*		$sql = "SELECT distinct
	                a.esfcod as esfera,
                    a.unicod,
	                a.funcod as funcao,
	                a.sfucod as subfuncao,
	                a.prgcod,
	                a.acacod,
	                avp.acaid,
	                a.acatipoinclusao as tipoinclusaoacao --,
	                --a.acatipoinclusaolocalizador as tipoinclusaolocalizador
				FROM monitora.acao a
					inner join monitora.avaliacaoparecer avp on avp.acaid = a.acaid
					inner join monitora.referencia ref on ref.refcod = avp.refcod
					inner join workflow.documento doc on doc.docid = avp.docid
                    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
				WHERE 
	            	a.acacod in ('".implode("', '", $arrAcao)."')
                    and a.prgano = '{$_SESSION['exercicio']}' 
                    and acasnrap = false
                    --and a.unicod in ('26271', '26241')
                    and ref.refcod = 263
                    and esd.esdid in (620, 648)
                    and a.prgcod not in ('1073','1449')
	            	and (trim(a.prodsc) <> '' or trim(a.prodsc) is not null)
				ORDER BY a.prgcod, a.acacod, a.unicod";
		ver($sql, d);
 * 
 */
            $arrAcao = implode(', ', $arrAcao);
            $sql = <<<DML
SELECT DISTINCT esf.codigo AS esfera,
                org.codigo as unicod,
                apr.siop_funcao AS funcao,
                apr.siop_subfuncao AS subfuncao,
                prg.codigo AS prgcod,
                a.codigo AS acacod,
                apr.id_acao_programatica AS acaid,
                apr.siop_codigotipoinclusaoacao AS tipoinclusaoacao
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
	
	private function carregaDadosFinanceiro($unicod, $prgcod, $acacod, $loccod){
		global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $servidor_bd_siafi, $porta_bd_siafi, $nome_bd_siafi, $usuario_db_siafi, $senha_bd_siafi;
		// Parâmetros para a nova conexão com o banco do SIAFI
			
		$servidor_bd = $servidor_bd_siafi;
		$porta_bd    = $porta_bd_siafi;
		$nome_bd     = $nome_bd_siafi;
		$usuario_db  = $usuario_db_siafi;
		$senha_bd    = $senha_bd_siafi;
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
		
		$dadosfinanceiroscompletos = $db2->pegaLinha( $sql );
		return $dadosfinanceiroscompletos;
	}
}
?>
