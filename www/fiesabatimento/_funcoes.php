<?php
/* WS de consulta a Plataforma Freire
 * Métodos:
 * 	lerDadosPessoais 
 * 	lerDadosAtuacao
 * Parâmetros: $dados = Array()
 * 	'servico' 	=> passar o metodo desejado
 * 	'cpf'		=> passar o cpf a ser consultado
 * Retorno: $retorno Array() de resposta.
 *  * -> lerDadosPessoais
 * 	Array
	(
	    [NO_PESSOA] => RAUL MAIA DA SILVA
	    [NU_CPF] => 72341602134
	    [DT_NASCIMENTO] => 03/10/1984
	    [NU_RG] => 2233670
	    [DS_CONTATO_ELETRONICO] => raul.silva@capes.gov.br
	    [NU_DDD] => 61
	    [NU_TELEFONE] => 92775466 
	    [DS_LOGRADOURO] => DE CHÁCARAS CÓRREGO DA ONÇA RUA A CHÁCARA 2
	    [DS_COMPLEMENTO] => 
	    [DS_BAIRRO] => SETOR DE CHÁCARAS CÓRREGO DA ONÇA (NÚCLEO BANDEIRANTE)
	    [DS_NUMERO] => 2
	    [NU_CEP] => 71761155
	    [CO_MUNICIPIO] => 5300108
	    [NO_MUNICIPIO_ACENTO] => BRASÍLIA
	    [SG_UF] => DF
	    [NO_UF] => Distrito Federal
	)
 * -> lerDadosAtuacao
 * 	Array
	(
	    [NO_PESSOA] => RAUL MAIA DA SILVA
	    [NU_CPF] => 72341602134
	    [DT_NASCIMENTO] => 03/10/1984
	    [NU_RG] => 2233670
	    [DT_FIM] => 
	    [DT_INICIO] => 01/08/2006
	    [NU_CARGA_HORARIA] => 40
	    [CO_INEP] => 
	    [NO_ENTIDADE] => Ministério da Educação
	    [CO_DEP_ADM] => F
	    [NO_DEP_ADM] => Federal
	    [CO_TIPO_VINCULO] => 6
	    [NO_TIPO_VINCULO] => Contrato Terceirizado
	    [CO_FUNCAO] => 4
	    [NO_FUNCAO] => Auxiliar
	    [CO_MUNICIPIO] => 5300108
	    [NO_MUNICIPIO_ACENTO] => BRASÍLIA
	    [SG_UF] => DF
	    [NO_UF] => Distrito Federal
	)
 * */
function wf_lerDados( $dados ){

	global $db;

	// Setar o header para aceitar array como POST
	$headers = array(
		'Content-Type: multipart/form-data'
	);

	// Array a ser enviado com as informações do serviço
	$data = array(
			'method'=> $dados['servico'],
// 			'co_tipo_pessoa_juridica '=> 4,
// 			'nivel_escolar'=> 1,2,3,18,19,
// 			'co_funcao'=> 2,
// 			'co_dep_adm'=> 'M', ’E', 'F',
// 			'co_tipo_vinculo'=> 'lerDadosPessoais',
// 			'method'=> 1,2,5,
			'pCpf'=> $dados['cpf']
	);

	// Inicio conexão Curl
	$handle = curl_init();

	// Setando parâmetros do Curl
	curl_setopt($handle, CURLOPT_URL, WS_WSDL_FIES);
	curl_setopt($handle, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($handle, CURLOPT_SSL_VERIFYHOST, false);
	curl_setopt($handle, CURLOPT_SSL_VERIFYPEER, false);
	curl_setopt($handle, CURLOPT_TIMEOUT, 60);
	curl_setopt($handle, CURLOPT_POST, true);
	curl_setopt($handle, CURLOPT_POSTFIELDS, $data);

	// Executa a consulta no WS
	$response = curl_exec($handle);

	// inicia XML parser, converte XML em Array e limpa obj XML parser;
	$p = xml_parser_create();
	xml_parse_into_struct($p, $response, $vals, $index);
	xml_parser_free($p);
	
	// Filtra dados do Array
	$retorno = Array();
	foreach( $vals as $val ){
		if( $val['level'] == 4 && substr($val['tag'],0,4) == 'KEY_' ){
			$x = explode('_',$val['tag']);
			$key = $x[1];
		}
		if( $val['level'] == 5 ){
			$retorno[$key][strtolower($val['tag'])] = utf8_decode($val['value']);
		}
	}
	
	curl_close($handle);
	
	return $retorno;
}

function temResp( $param ){
	
	global $db;
	
	if( $param['campo'] != '' && $param['valor'] != '' ){
		$sql = "SELECT
					true
				FROM
					fiesabatimento.usuarioresponsabilidade
				WHERE
					{$param['campo']} = '{$param['valor']}'
					AND pflcod = {$param['perfil']}
					AND usucpf = '{$_SESSWION['usucpf']}'";
					
		$retorno = $db->pegaUm($sql);
		
		return $retorno == 't';
	}
	
	return false;
}

function gravarMesesAtuacoes( Array $atpid ){
	
	global $db;
	if( is_array($atpid) ){
		foreach( $atpid as $id ){
			$sql = "SELECT
						CASE WHEN atp.esferaprofessor = 'M' THEN 'Municipal'
							ELSE 'Estadual'
						END as no_dep_adm,
						atp.atpinep as co_inep,
						atp.atpdescricaoescola as no_entidade,
						atp.atpvinculo as no_tipo_vinculo,
						atp.atpfuncao as no_funcao,
						atp.atpnumcargahoraria as nu_carga_horaria,
						atp.atpdatainicio as dt_inicio,
						atp.atpdatafim as dt_fim,
						COALESCE(atp.atpdatafim - atp.atpdatainicio,0) as difmeses,
						sba.sbaid,
						sba.sbaanoinicio,
						sba.sbaanofim,
						sba.sbarenovacao
					FROM fiesabatimento.solicitacaoabatimento sba
					INNER JOIN fiesabatimento.atuacaoprofissional atp ON atp.sbaid = sba.sbaid
					WHERE
						atpid = $id";
			$arrDados = $db->pegaLinha($sql);
			
			$sbaanoinicio = $arrDados['sbaanoinicio'];
			$sbaanofim = $arrDados['sbaanofim'];

			$DT_INICIO_PROGRAMA = "{$sbaanoinicio}-01-01";
			$DT_FIM_PROGRAMA = "{$sbaanofim}-12-31";
			
			$dtInicio = explode('-', $arrDados['dt_inicio']);
			$dtFim = explode('-',$arrDados['dt_fim']);

			$arrAnos = recupaAnosEscola($arrDados, $sbaanoinicio, $sbaanofim, $sbarenovacao, $sbames);
			$arrMeses = recuperaMesesFIES();

			$arrINI = explode('-',$DT_INICIO_PROGRAMA);
			$arrFIM = explode('-',$DT_FIM_PROGRAMA);

			if( is_array( $arrAnos ) ){
				$sqlM = '';
				foreach( $arrAnos as $ano){
					foreach( $arrMeses as $mes ){
						$mescod = strlen($mes['codigo']) == 1 ? "0".$mes['codigo'] : $mes['codigo'];
						$dt_mescod = $ano.$mescod;
							
						$arrI = explode("-",$escola['dt_inicio']);
						$dt_ini = $arrI[0].$arrI[1];
						$dt_ini = !$dt_ini ? $arrINI[0].$arrINI[1] : $dt_ini;
							
						$arrF = explode("-",$escola['dt_fim']);
						$dt_fim = $arrF[0].$arrF[1];
						$dt_fim = !$dt_fim ? $arrFIM[0].$arrFIM[1] : ($dt_fim > $arrFIM[0].$arrFIM[1] ? $arrFIM[0].$arrFIM[1] : $dt_fim);
							
						if($dt_mescod >= $dt_ini && $dt_mescod <= $dt_fim){
							$rdn_confirmar_mes[$ano][$mes['codigo']] = 'lote';
						}
					}
				}
			}

			if( is_array( $rdn_confirmar_mes ) ){
				foreach($rdn_confirmar_mes as $ano => $arrMeses){
					$anoX = $anoX == 0 ? $ano : $anoX;
					$sql = "SELECT
								ranid
							FROM
								fiesabatimento.responsavelanoatuacao
							WHERE
								atpid = $id
								AND rananotuacao = $ano
								AND ranresponsaveltipo = 'D'
								AND rancpfresponsavel = '{$_SESSION['usucpf']}'
								AND ranstatus = 'A'";
					$ranid = $db->pegaUm($sql);
					if(!$ranid){
						$sql = "INSERT INTO fiesabatimento.responsavelanoatuacao(atpid,co_usuario,rananotuacao,ranresponsaveltipo,rancpfresponsavel,ranstatus)
						VALUES ($id,NULL,$ano,'D','{$_SESSION['usucpf']}','A')
						RETURNING ranid";
						$ranid = $db->pegaUm($sql);
					}
					$sql = "delete from fiesabatimento.mesesatuacao where atpid = $id";
					$db->executar($sql);
					if($arrMeses){
						foreach($arrMeses as $mes => $val){
							if( $ano > $dtInicio[0] || ( $ano == $dtInicio[0] && $mes >= $dtInicio[1] ) ){
								if( $dtFim[0] != '' ){
									if( $ano < $dtFim[0] || ( $ano == $dtFim[0] && $mes <= $dtFim[1] ) ){
										$mesX = $mesX == 0 ? $mes : $mesX;
										$qtd_meses ++;
										$sqlM .= "INSERT INTO fiesabatimento.mesesatuacao(ranid,sbaid,atpid,matano,matmes,matstatus)
													VALUES ($ranid,{$arrDados['sbaid']},$id,'$ano','$mes','A');";
									}
								}else{
									$mesX = $mesX == 0 ? $mes : $mesX;
									$qtd_meses ++;
									$sqlM .= "INSERT INTO fiesabatimento.mesesatuacao(ranid,sbaid,atpid,matano,matmes,matstatus)
												VALUES ($ranid,{$arrDados['sbaid']},$id,'$ano','$mes','A');";
								}
							}
						}
					}
				}
				if($sqlM){
					$db->executar($sqlM);
				}
				$db->commit();
			}
		}
	}	
}


function rejeitarAtuacao( $sba_id ){
	
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	$where = Array( "idostatus='A'",
			"sba.sbastatus = 'A'",
			"atp.atpstatus = 'A'",
			"ido.idostatus = 'A'");
	
	if( in_array(PFL_SECRETARIO_MUNICIPAL,$perfis) || in_array(PFL_SUB_SECRETARIO_MUNICIPAL,$perfis) ){
		$flatFiltro = true;
		$where[] = "atp.esferaprofessor = 'M'";
		$inner_resp = "INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.muncod = atp.muncodprofessor AND urs.usucpf = '".$_SESSION['usucpf']."'";
	}
	
	if( in_array(PFL_SECRETARIO_ESTADUAL,$perfis) || in_array(PFL_SUB_SECRETARIO_ESTADUAL,$perfis) ) {
		$flatFiltro = true;
		$where[] = "atp.esferaprofessor in ('E','F')";
		$inner_resp = "INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.estuf = atp.estufprofessor AND urs.usucpf = '".$_SESSION['usucpf']."'";
	}
	
	$sql = "SELECT DISTINCT
				atp.atpid
			FROM
				fiesabatimento.identificacaodocente ido
			INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
			INNER JOIN fiesabatimento.atuacaoprofissional 	atp	ON atp.idoid = ido.idoid AND atp.sbaid = sba.sbaid
			LEFT  JOIN territorios.municipio 			   	mun ON mun.muncod::integer = atp.muncodprofessor::integer
			$inner_resp
			WHERE
				sba.sbaid = $sba_id
				AND ".implode(' AND ',$where);
	
	$atpid = $db->carregarColuna($sql);
	
	$atpid_parcial = Array();
	
	if( $atpid[0] != '' ){
	
		$sql = "UPDATE fiesabatimento.atuacaoprofissional SET 
					atpidusuconfirmacao = '".$_SESSION['usucpf']."', 
					atpdataconfirmacao = now(),
					atprespsecretario = 'N3'
				WHERE atpid in (".implode(',',$atpid).")";
		
		$db->executar($sql);
		$db->commit();
		
		foreach($atpid as $id){
			
			$sql = "SELECT
						atpdatafim as data_fim
					FROM
						fiesabatimento.atuacaoprofissional
					WHERE
						atpid = $id";
				
			$dataFim = $db->pegaUm( $sql );
				
			if( $dataFim != '' ){
				$booEfetivoExercicio = 'FALSE';
			}else{
				$booEfetivoExercicio = 'TRUE';
			}
				
			$sql = "UPDATE fiesabatimento.atuacaoprofissional SET
						atpcompefetivoexercicio = $booEfetivoExercicio
					WHERE
						atpid = $id";
		
			$db->executar($sql);
			$db->commit();
			
			$justificativa = $_POST['justificativa'] ? $_POST['justificativa'] : 'Rejeitado';
				
			$docid = pegaDocidAtuacao($id);
			wf_alterarEstado( $docid, AEDID_REJEITAR_ATUACAO, $justificativa, array('docid'=>$docid ) );

			enviaEmailRejeicaoAtuacao( $id );
		}
	}
		
}

function executarAnaliseAutomatica( $sba_id ){
	
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	$where = Array( "idostatus='A'",
					"sba.sbastatus = 'A'",
					"atp.atpstatus = 'A'",
					"ido.idostatus = 'A'");
	
	if( in_array(PFL_SECRETARIO_MUNICIPAL,$perfis) || in_array(PFL_SUB_SECRETARIO_MUNICIPAL,$perfis) ){
		$flatFiltro = true;
		$where[] = "atp.esferaprofessor = 'M'";
		$inner_resp = "INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.muncod = atp.muncodprofessor AND urs.usucpf = '".$_SESSION['usucpf']."'";
	}
	
	if( in_array(PFL_SECRETARIO_ESTADUAL,$perfis) || in_array(PFL_SUB_SECRETARIO_ESTADUAL,$perfis) ) {
		$flatFiltro = true;
		$where[] = "( atp.esferaprofessor = 'E' OR atp.esferaprofessor = 'F' )";
		$inner_resp = "INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.estuf = atp.estufprofessor AND urs.usucpf = '".$_SESSION['usucpf']."'";
	}
	
	$sql = "SELECT DISTINCT
				atp.atpid
			FROM
				fiesabatimento.identificacaodocente ido
			INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
			INNER JOIN fiesabatimento.atuacaoprofissional 	atp	ON atp.idoid = ido.idoid AND atp.sbaid = sba.sbaid
			LEFT  JOIN territorios.municipio 			   	mun ON mun.muncod::integer = atp.muncodprofessor::integer
			$inner_resp
			WHERE
				sba.sbaid = $sba_id
				AND ".implode(' AND ',$where);

	$atpid = $db->carregarColuna($sql);

	$atpid_parcial = Array();
		
	if( $atpid[0] != '' ){
		
		$sql = "UPDATE fiesabatimento.atuacaoprofissional SET 
					atpidusuconfirmacao = '".$_SESSION['usucpf']."', 
					atpdataconfirmacao = now(), 
					atprespsecretario = 'S'
				WHERE atpid in (".implode(',',$atpid).")";
		$db->executar($sql);
		$db->commit();
		
		gravarMesesAtuacoes($atpid);
		
		foreach($atpid as $id){
			
			$sql = "UPDATE fiesabatimento.atuacaoprofissional SET
						atpcompefetivoexercicio = atpprofefetivoexercicio
					WHERE
						atpid = $id";

			$db->executar($sql);
			$db->commit();
			
			enviaEmailAprovacaoParcial( $id );
			$docid = pegaDocidAtuacao($id);
			wf_alterarEstado( $docid, AEDID_FINALIZAR_ANALISE, 'Aprovado em lote.', array('docid'=>$docid ) );
		}
	}
}


function tramitarLote( $request ){
	
	global $db;

	extract($request);
	
	if( is_array($sbaid) ){
		foreach( $sbaid as $sba_id){
			executarAnaliseAutomatica( $sba_id );
			
			$sql = "SELECT true FROM fiesabatimento.atuacaoprofissional WHERE atpidusuconfirmacao IS NULL AND atpstatus = 'A' AND sbaid = $sba_id";
			$teste = $db->pegaUm($sql);
			if( $teste == '' && testaMesesHoras($sba_id)
					//&& tesJustificativa($sba_id)
			){
				atualizaMesesSolicitacao( $dados['sbaid'] );
				$sql = "SELECT docid FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = $sba_id";
				$docid = $db->pegaUm($sql);
				enviaEmailAprovacao( $sba_id );
				$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = ".$docid);
				if( $esdid == WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL ){
					$test = wf_alterarEstado( $docid, WF_FIES1_APROVAR_SOLICITACAO, 'Aprovado em lote.', array('docid'=>$docid ) );
				}
			}
		}
	}
	echo "<script>alert('".count($docid)." solicitações de abatimento analisadas!');window.location = window.location;</script>";
}

function testeWSAjax( ){
	
	global $db;
	
	$idoid = pegaIdoid();
	
	$dadosUsu = pegarDadosUsuario( $idoid );
	
	$erro .= testeWS( $dadosUsu );
	if($erro!=''){
		echo $erro;
	}
}

function testeWS( $dados ){
	
	global $db;
	
	//trata data nascimento
	if($dados['dt_nascimento']){
		$tempData = explode('-', $dados['dt_nascimento']);
		$dados['dt_nascimento'] = $tempData[2] .'/'. $tempData[1] .'/'. $tempData[0];
	}

	$cliente = new SoapClient(WS_WSDL, array('trace' => TRUE));
	
// 	$dados['dt_nascimento'] = explode('-',$dados['dt_nascimento']);
// 	$dados['dt_nascimento'] = $dados['dt_nascimento'][2].'/'.$dados['dt_nascimento'][1].'/'.$dados['dt_nascimento'][0];

	$input = new stdClass();
	$input->strUsuario 		= WS_USUARIO;
	$input->strSenha 		= WS_SENHA;
	$input->strNuCpf 		= $dados['nu_cpf'];
	$input->strDtNascimento = $dados['dt_nascimento'];
// 	$input->strNuCpf 		= '';
// 	$input->strDtNascimento = '19/02/1987';
// 	$input->strDtNascimento = '26/03/1911';
	$input->strNoCliente 	= WS_CLIENTE;
// 	ver($dados,$input,d);
// 	ver($cliente->verificarEstudanteFIESAtivo( $input ),d);
	
  	try {
	  	$verificarEstudanteFIESAtivoOut = $cliente->verificarEstudanteFIESAtivo( $input );
		if( $verificarEstudanteFIESAtivoOut->detalhamento->exceptionCode == '01' ){
	  		$erro .= '- CPF não encontrado na base de dados dos contratos de \n financiamento concedidos com recursos do FIES no Agente Financeiro. \n';
		}elseif( $verificarEstudanteFIESAtivoOut->detalhamento->exceptionCode || $verificarEstudanteFIESAtivoOut->detalhamento->exceptionMessage ){
	  		$erro .= '- '.utf8_decode($verificarEstudanteFIESAtivoOut->detalhamento->exceptionMessage).'.\n';
	  	}else{
	  		
		  	if( $verificarEstudanteFIESAtivoOut->detalhamento->dtNascimento != $dados['dt_nascimento'] ){
		  		$erro .= '- Data de nascimento é diferente da constante do financiamento (FIES). Regularize a situação. \n';
		  	}
		  	$arrTpCadastroCurso = Array('L','NS','P');
		  	if( !in_array($verificarEstudanteFIESAtivoOut->detalhamento->tpCadastroCurso,$arrTpCadastroCurso) ){
		  		$erro .= '- Solicitante não possui, no agente financeiro, contrato em curso de Licenciatura, Normal Superior ou Pedagogia pelo FIES.\n';
		  	}
		  	
		  	if( !$verificarEstudanteFIESAtivoOut->detalhamento->stAtivo ){
		  		$erro .= '- Professor solicitante não possui financiamento pelo FIES. \n';
		  	}
		  	
		  	if( strrpos(trim( str_to_upper( $verificarEstudanteFIESAtivoOut->detalhamento->stAdimplente ) ), "A") === false ){
		  		$erro .= '- O contrato de financiamento encontra-se em atraso ou inadimplente. Para prosseguir,\n'. 
		  				 'o solicitante deverá retornar o financiamento à situação de normalidade em relação ao pagamento\n'. 
		  				 'dos juros/prestações do financiamento.\n\n'.
						 'Se necessário, faça uso da renegociação prevista na Resolução n° 3, de 20.10.10, observado o disposto\n'. 
						 'no inciso II do §1° e do §2°, do Fundo Nacional de Desenvolvimento da Educação - FNDE.\n\n'.
						 'Após regularização, prossiga com a solicitação do abatimento.';
		  	}
            
//            if(isset($verificarEstudanteFIESAtivoOut->detalhamento->stDemandaJudicial) && $verificarEstudanteFIESAtivoOut->detalhamento->stAdimplente == 'S'){
//                $erro .= '- O contrato de financiamento encontra-se em situação de demanda judicial, impossibilitando o prosseguimento.';
//            }
            
	  	}
  	} catch (Exception $e) {
  		$erro = "Erro do WS: ".str_replace("'",'"', $e->getMessage());
  	}
  	return $erro;
}

function enviarRequisicao(){
	
	extract($_POST);
	//dbg($_POST,1);
	
	$retornoWS = enviaAbatimentoWS( $sbaid );
// 	ver($retornoWS,d);
	if( $retornoWS['boo'] || $retornoWS['cod'] == '23'  ){
		
		enviaEmailEviadoBanco( $sbaid );
		
		$docid = pegaDocidSolicitacaoSbaid( $sbaid );
		$test = wf_alterarEstado( $docid, WF_FIES1_ENVIAR_PROCESSAMENTO_BANCARIO, '', array('docid'=>$docid ) );
		
		echo "
			<script>
				alert('Requisição enviada para o banco.');
				window.location.href = window.location.href;
			</script>";
	}else{
		
		if( $retornoWS['cod'] != '' ){
			$codErro = "\\nCódigo do erro:{$retornoWS['cod']}";
		}
		
		echo "
			<script>
				alert('Operação abortada devido ao seguinte erro:\\n\\n- {$retornoWS['txt']}$codErro\\nTente novamente mais tarde.\\nCaso o erro persista, entre em contato com 0800-61-6161.');
				window.location.href = window.location.href;
			</script>";
	}
}


function enviaAbatimentoWS( $sbaid ){
	
	global $db;
	
	$cliente = new SoapClient(WS_WSDL, array('trace' => TRUE));
	
	$arrDatas = pegaPeriodosAtuacao( $sbaid );
	
	$dataMin = '';
	foreach( $arrDatas as $data ){
		$ini = explode('-',$data['dt_inicio']);
		$meses_ini = ($ini[0]*12)+$ini[1];
		$fim = explode('-',$data['dt_fim']);
		$meses_fim = ($fim[0]*12)+$fim[1];
		if( $dataMin == '' && ($meses_fim-$meses_ini)+1 > 11 ){
			$dataMin = $ini[0].$ini[1];
		}
	}
	
	$sql = "SELECT DISTINCT
				idocpf as cpf,
				to_char(idodatanascimento, 'DD/MM/YYYY') as dt_nascimento,
				sbaqmtvalido as qtd_meses_trabalhados,
				to_char(now(),'DD/MM/YYYY') as dt_inicio_suspensao
			FROM
				fiesabatimento.solicitacaoabatimento sba
			INNER JOIN fiesabatimento.identificacaodocente 	ido ON ido.idoid = sba.idoid
			INNER JOIN fiesabatimento.atuacaoprofissional 	atp ON atp.sbaid = sba.sbaid
			WHERE
				sba.sbaid = ".$sbaid;
		
	$dadosWS = $db->pegaLinha($sql);
	
	$sql = "SELECT
				CASE WHEN atpcompefetivoexercicio
					THEN 'S'
					ELSE 'N'
				END
			FROM
			(
			SELECT DISTINCT
				atpcompefetivoexercicio,
				sum(atpnumcargahoraria) as hrs
			FROM
				fiesabatimento.solicitacaoabatimento sba
			INNER JOIN fiesabatimento.atuacaoprofissional 	atp ON atp.sbaid = sba.sbaid
			WHERE
				atpstatus = 'A'
				AND sba.sbaid = $sbaid
				
			GROUP BY
				atpcompefetivoexercicio
			) foo
			WHERE
				hrs >= 20
			ORDER BY 1 DESC";

	$dadosWS['ativo_exercicio'] = $db->pegaUm($sql);

	$input = new stdClass();
	$input->strUsuario 				= WS_USUARIO;
	$input->strSenha 				= WS_SENHA;
	$input->strNuCpf 				= $dadosWS['cpf'];
	$input->strDtNascimento 		= $dadosWS['dt_nascimento'];
	$input->strQtMesesTrabalhado 	= $dadosWS['qtd_meses_trabalhados'];
	$input->strStSuspenderCobranca 	= $dadosWS['ativo_exercicio'];
	$input->strDtInicioSuspensao 	= $dadosWS['dt_inicio_suspensao'];
	$input->strNoCliente 			= WS_CLIENTE;
	
	try {
		$enviarSuspensaoCobrancaOut = $cliente->enviarSuspensaoCobranca( get_object_vars( $input ) );
// 		ver($enviarSuspensaoCobrancaOut);
		if( $enviarSuspensaoCobrancaOut->output->exceptionCode != '' ){
			$retorno['cod'] = $enviarSuspensaoCobrancaOut->output->exceptionCode;
			$retorno['txt'] = utf8_decode($enviarSuspensaoCobrancaOut->output->exceptionMessage);
			$retorno['boo'] = false;
			$remetente = array('nome'=>'FIES - Abatimento 1% Erro WS Suspensão', 'email'=>'simec@mec.gov.br');
			enviar_email($remetente, 'alex.pereira@mec.gov.br', 'Erro WS Suspensão', $retorno['txt'], $cc, $cco );
			return $retorno;
		}
		
		if( $enviarSuspensaoCobrancaOut->output->dtPrevEfetivacao ){
			$sql = "UPDATE fiesabatimento.solicitacaoabatimento SET
						dtsuspensao = '".$enviarSuspensaoCobrancaOut->output->dtPrevEfetivacao."'
					WHERE
						sbaid = $sbaid";
			
			$db->executar($sql);
			$db->commit();
		}
		
		if( $enviarSuspensaoCobrancaOut ){
			$retorno['txt'] = $enviarSuspensaoCobrancaOut->output->exceptionMessage;
			$retorno['boo'] = true;
			return $retorno;
		}else{
			$retorno['txt'] = "Serviços bancários temporarioamente indisponíveis.\n\nTente mais tarde.";
			$retorno['boo'] = false;
			return $retorno;			
		}
		
	} catch (Exception $e) {
// 		  		ver(simec_htmlentities($cliente->__getLastRequest()),d);
		$erro = "Erro do WS: ".$e->getMessage();
		$remetente = array('nome'=>'FIES - Abatimento 1% Erro WS Suspensão', 'email'=>'simec@mec.gov.br');
		enviar_email($remetente, 'alex.pereira@mec.gov.br', 'Erro WS Suspensão', $erro, $cc, $cco );
		//   		$erro = "Erro do WS: ".simec_htmlentities($cliente->__getLastRequest());
		$retorno['txt'] = $erro;
		$retorno['boo'] = false;
		return $retorno;
	}
	
}

function pegaMuncodCPF(){
	
	global $db;
	
// 	$sql = "SELECT 
// 				co_municipio
// 			FROM dblink (
// 				'".PARAM_DBLINK_FREIRE."',
// 				'SELECT 
// 					en.co_municipio
// 				FROM	
// 					public.tb_sf_pessoa_fisica pf	
// 				LEFT JOIN public.tb_sf_fisica_juridica  fj ON fj.co_pessoa_fisica = pf.co_pessoa_fisica 
// 				LEFT JOIN public.tb_sf_pessoa_juridica  pj ON pj.co_pessoa_juridica = fj.co_pessoa_juridica
// 				LEFT JOIN public.tb_sf_pessoa            p ON p.co_pessoa = pj.co_pessoa_juridica
// 				LEFT JOIN public.tb_sf_endereco	        en ON en.co_pessoa = p.co_pessoa
// 				WHERE 
// 					pf.nu_cpf=\'".$_SESSION['fiesabatimento_var']['cpfusuario']."\' limit 1;'
// 			) as rs (
// 				co_municipio integer
// 			)";

	$dados['cpf'] = $_SESSION['fiesabatimento_var']['cpfusuario'];
	$dados['servico'] = 'lerDadosPessoais';
	
	$dados = wf_lerDados( $dados );
	
	return $dados[0]['co_municipio'];
}

function pegaEstufCPF(){
	
	global $db;
	
// 	$sql = "SELECT 
// 				sg_uf
// 			FROM dblink (
// 				'".PARAM_DBLINK_FREIRE."',
// 				'SELECT 
// 					uf.sg_uf
// 				FROM	
// 					public.tb_sf_pessoa_fisica pf	
// 				LEFT JOIN public.tb_sf_fisica_juridica  fj ON fj.co_pessoa_fisica = pf.co_pessoa_fisica 
// 				LEFT JOIN public.tb_sf_pessoa_juridica  pj ON pj.co_pessoa_juridica = fj.co_pessoa_juridica
// 				LEFT JOIN public.tb_sf_pessoa            p ON p.co_pessoa = pj.co_pessoa_juridica
// 				LEFT JOIN public.tb_sf_endereco	        en ON en.co_pessoa = p.co_pessoa
// 				LEFT JOIN public.tb_sf_municipio	mu ON mu.co_municipio = en.co_municipio
// 				LEFT JOIN public.tb_sf_uf		uf ON uf.co_uf = mu.co_uf
// 				WHERE 
// 					pf.nu_cpf=\'".$_SESSION['fiesabatimento_var']['cpfusuario']."\' limit 1;'
// 			) as rs (
// 				sg_uf character(2)
// 			)";
	
	$dados['cpf'] = $_SESSION['fiesabatimento_var']['cpfusuario'];
	$dados['servico'] = 'lerDadosPessoais';
	
	$dados = wf_lerDados( $dados );
	
	return $dados[0]['sg_uf'];
}

function pegaInep(){
	
	global $db;
	
// 	$sql = "SELECT 
// 				co_inep
// 			FROM dblink (
// 				'".PARAM_DBLINK_FREIRE."',
// 				'SELECT 
// 					pj.co_inep
// 				FROM	
// 					public.tb_sf_pessoa_fisica pf	
// 				LEFT JOIN public.tb_sf_fisica_juridica fj ON fj.co_pessoa_fisica = pf.co_pessoa_fisica 
// 				LEFT JOIN public.tb_sf_pessoa_juridica pj ON pj.co_pessoa_juridica = fj.co_pessoa_juridica
// 				WHERE 
// 					pf.nu_cpf=\'".$_SESSION['fiesabatimento_var']['cpfusuario']."\' limit 1;'
// 			) as rs (
// 				co_inep integer
// 			)";
	
	$dados['cpf'] = $_SESSION['fiesabatimento_var']['cpfusuario'];
	$dados['servico'] = 'lerDadosAtuacao';
	
	$dados = wf_lerDados( $dados );
	
	return $dados['co_inep'];
}

function verificaEsferaEntidade(){
	
	global $db;
	
// 	$sql = "SELECT 
// 				co_dep_adm
// 			FROM dblink (
// 				'".PARAM_DBLINK_FREIRE."',
// 				'SELECT 
// 					co_dep_adm
// 				FROM	
// 					public.tb_sf_pessoa_fisica pf	
// 				LEFT JOIN public.tb_sf_fisica_juridica fj ON fj.co_pessoa_fisica = pf.co_pessoa_fisica 
// 				LEFT JOIN public.tb_sf_pessoa_juridica pj ON pj.co_pessoa_juridica = fj.co_pessoa_juridica
// 				WHERE 
// 					pf.nu_cpf=\'".$_SESSION['fiesabatimento_var']['cpfusuario']."\' limit 1;'
// 			) as rs (
// 				co_dep_adm character(1)
// 			)";
// 	return $db->pegaUm($sql);

	$dados['cpf'] = $_SESSION['fiesabatimento_var']['cpfusuario'];
	$dados['servico'] = 'lerDadosAtuacao';
	
	$dados = wf_lerDados( $dados );
	
	return $dados['co_dep_adm'];
}

function reabrir( $request ){
	
	global $db;
	
	//pega id renovação
	/*
	if($request['sbaid']){
		$preid = $db->pegaUm("SELECT preid FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = ".$request['sbaid']);
	}
	*/
	$dadossbaid = $db->pegaLinha("SELECT sbaanoinicio, sbaanofim, sbarenovacao FROM fiesabatimento.solicitacaoabatimento
								  WHERE sbaid = {$request['sbaid']}");
	if($dadossbaid) extract($dadossbaid);

		//$docid = pegaDocidSolicitacao($request['idoid'], $preid);
	//$docid = pegaDocidSolicitacao($request['idoid']);
	$docid = pegaDocidSolicitacaoSbaid($request['sbaid']);
	$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = $docid");
	$perfis = pegaPerfilGeral();
	
	$where = Array("idoid = ".$request['idoid'],"sbaid = ".$request['sbaid'],"atpstatus = 'A'");
	if( in_array(PFL_SECRETARIO_MUNICIPAL,$perfis) || in_array(PFL_SUB_SECRETARIO_MUNICIPAL,$perfis) ){
		$where[] = "atp.esferaprofessor = 'M'";
		$inner_resp = "INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.muncod = atp.muncodprofessor AND urs.usucpf = '".$_SESSION['usucpf']."'";
	}
	if( in_array(PFL_SECRETARIO_ESTADUAL,$perfis) || in_array(PFL_SUB_SECRETARIO_ESTADUAL,$perfis) ) {
		$where[] = "atp.esferaprofessor in ('E','F')";
		$inner_resp = "INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.estuf = atp.estufprofessor AND urs.usucpf = '".$_SESSION['usucpf']."'";
	}
	
	$sql = "SELECT DISTINCT
				atpid
			FROM
				fiesabatimento.atuacaoprofissional atp 
			$inner_resp
			WHERE
			".implode(' AND ', $where);
	
	$atpids = $db->carregarColuna($sql);

	$sql = "UPDATE fiesabatimento.atuacaoprofissional SET
				atpdatareabertura = NULL,
				atprespsecretario = 'N1'
			WHERE
				atpid in (".implode(',',$atpids).")";
	$db->executar($sql);
	
	$sql = "UPDATE fiesabatimento.atuacaoprofissional SET
				atpdatareabertura = now()
			WHERE
				atpid in (".implode(',',$atpids).")";
	$db->executar($sql);
	
	
	$sql = 'DELETE FROM fiesabatimento.atuacao_motivocorrecao WHERE atpid in ('.implode(',',$atpids).');';
	if( is_array( $request['mocid'] ) ){
		foreach( $request['mocid'] as $mocid ){
			foreach( $atpids as $atpid ){
				$sql .= "INSERT INTO fiesabatimento.atuacao_motivocorrecao(atpid, mocid) 
						 VALUES($atpid, $mocid);";
			}
		}
		$db->executar($sql);
		
	}
	$db->commit();

	$justificativa = $request['justificativa'] ? $request['justificativa'] : 'Enviado para correção do Professor';
	
	foreach( $atpids as $atpid ){
		$docidAt = pegaDocidAtuacao($atpid);
		wf_alterarEstado( $docidAt, AEDID_ENVIAR_CORRECAO, $justificativa, array('docid'=>$docid ) );
	}
	
	if( $esdid == WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL ){
		$test = wf_alterarEstado( $docid, WF_FIES1_REABRIR_ABATIMENTO, $justificativa, array('docid'=>$docid ) );
	}else{
		$test = wf_alterarEstado( $docid, WF_FIES1_REABRIR_ABATIMENTO2, $justificativa, array('docid'=>$docid ) );
	}
	if( is_array($atpids) ){
		foreach( $atpids as $id){
			enviaEmailReabertura( $id );
		}
	}
	if($sbarenovacao == 't'){
		echo "<script>alert('Renovação de abatimento Reaberta');window.location = 'fiesabatimento.php?modulo=principal/listasolicitacaoabatimento&acao=A';</script>";
	}else{
		echo "<script>alert('Solicitação de abatimento Reaberta');window.location = 'fiesabatimento.php?modulo=principal/listasolicitacaoabatimento&acao=A';</script>";
	}
}

function cancelarAtuacaoAtpid( $atpid ){
	
	global $db;
	
	$docidAt = pegaDocidAtuacao($atpid);
	
	$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = $docidAt");
	
	if( $esdid == ESDID_AGUARDANDO_ANALISE ){
		$acao = AEDID_CANCELAR_ATUACAO;
		wf_alterarEstado( $docidAt, $acao, 'Cancelado pelo professor - Tela de cancelamento de solicitação', array('docid'=>$docidAt ) );
		$sql = "UPDATE fiesabatimento.atuacaoprofissional SET atpstatus = 'I' WHERE atpid = $atpid";
		$db->executar($sql);
		$db->commit();
	}
}

function cancelarAtuacao( ){
	
	global $db;
	
	extract($_POST);
	
	cancelarAtuacaoAtpid( $atpid );
	
	$sbaid = pegaSbaidAtuacao($atpid);
	$docid = pegaDocidSolicitacaoSbaid($sbaid);
	
	//pega id renovação
	//$preid = $db->pegaUm("SELECT preid FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = ".$sbaid);
	
	//$dados = pegaAtuacaoSolicitacao( $sbaid, $preid );
	$dados = pegaAtuacaoSolicitacao( $sbaid );

	$dadossbaid = $db->pegaLinha("SELECT sbaanoinicio, sbaanofim, sbarenovacao FROM fiesabatimento.solicitacaoabatimento
								  WHERE sbaid = {$sbaid}");
	if($dadossbaid) extract($dadossbaid);

	$teste = calculaMeses( $dados, $sbaanoinicio, $sbaanofim, $sbames );
	$meses = $teste['meses'];
	

	//if( $meses < 12 && !$preid ){
	if( $meses < 12 ){
		
		if( !testaTodasAtuacoesConfirmadas( $sbaid ) ){
			
			$atpids = pegaAtpidsSolicitacao($sbaid, ' AND atpidusuconfirmacao IS NULL');
			
			$sql = "UPDATE fiesabatimento.atuacaoprofissional SET
						atpidusuconfirmacao = '".$_SESSION['fiesabatimento_var']['cpfusuario']."',
						atpdataconfirmacao = now(),
						atprespsecretario = 'N'
					WHERE atpid in (".implode(',',$atpids).") ";
			
			$db->executar($sql);
			$db->commit();
			
			
			foreach( $atpids as $atpid ){
				
				$sql = "SELECT
							atpdatafim as data_fim
						FROM
							fiesabatimento.atuacaoprofissional
						WHERE
							atpid = $atpid";
				
				$dataFim = $db->pegaUm( $sql );
				
				if( $dataFim != '' ){
					$booEfetivoExercicio = 'FALSE';
				}else{
					$booEfetivoExercicio = 'TRUE';
				}
				
				$docidAt = pegaDocidAtuacao($atpid);
				wf_alterarEstado( $docidAt, AEDID_REJEITAR_ATUACAO, 'Professor cancelou atuação - Meses ou carga horária insuficiente.', array('docid'=>$docidAt ) );
				
				$sql = "UPDATE fiesabatimento.atuacaoprofissional SET
							atpcompefetivoexercicio = $booEfetivoExercicio
						WHERE
							atpid = $atpid";
	
				$db->executar($sql);
				$db->commit();
			}
		}
	}
	
	if( testaTodasAtuacoesConfirmadas( $sbaid ) ){
		$dados['sbaid'] = $sbaid;
		$dados['atpid'] = $atpid;
		$dados['hstid']	= pegaUltimaTramitacao($docid);
		$dados['htrperfil'] = pegaPerfilGeral();
		$dados['hrtmotivoreabertura'] = "'Atuação profissional candelada pelo professor.'";
		
		atualizaMesesSolicitacao( $sbaid );
		
		if( testaMesesHoras( $sbaid, $sbaanoinicio, $sbaanofim, $sbames ) ){

			$comentario = 'Solicitação confirmada em ' . date('d/m/Y H:i:s') . ' por ' . $_SESSION['usucpf'];

			$test = wf_alterarEstado( $docid, WF_FIES1_APROVAR_SOLICITACAO, $comentario, array('docid'=>$docid ) );

			insereHistoricoTramitacao($dados);

			enviaEmailAprovacao( $sbaid );
			
			echo "<script>
					alert('Atuação profissional cancelada.');
					alert('Solicitação de abatimento aprovada.');
					window.location.href = window.location.href;
				</script>";
			
			die();

		}else{
			enviaEmailRejeicao( $sbaid );
				
			$comentario = 'Solicitação rejeitada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
				
			$test = wf_alterarEstado( $docid, WF_FIES1_REJEITAR_ABATIMENTO, $comentario, array('docid'=>$docid ) );
				
			insereHistoricoTramitacao($dados);
				
			$sql = "UPDATE fiesabatimento.solicitacaoabatimento SET sbastatus = 'I' WHERE sbaid = ".$sbaid.";";
// 					"UPDATE fiesabatimento.atuacaoprofissional   SET atpstatus = 'I' WHERE sbaid = ".$sbaid.";";
			$db->executar($sql);
			$db->commit();
			
			echo "<script>
				alert('Atuação profissional cancelada.');
				alert('Solicitação de abatimento rejeitada.');
				window.location.href = window.location.href;
				</script>";
			
			die();
		}
	}
	
	echo "<script>
			alert('Atuação profissional cancelada.');
			window.location.href = window.location.href;
		</script>";
	
	die();
}

function atualizaMesesSolicitacao( $sbaid ){
	
	global $db;	

	$sbaqmtaprovado = calculaMesesAprovados( $sbaid );
	$sbaqmtvalido 	= calculaMesesValidos( $sbaid );
	
	$sbaqmtaprovado = $sbaqmtaprovado == '' ? '0' : $sbaqmtaprovado;
	$sbaqmtvalido 	= $sbaqmtvalido == '' ? '0' : $sbaqmtvalido;

	$sql = "UPDATE fiesabatimento.solicitacaoabatimento SET
				sbaqmtaprovado = $sbaqmtaprovado,
				sbaqmtvalido = $sbaqmtvalido
			WHERE
				sbaid = $sbaid";
	
	$db->executar($sql);
	$db->commit();
}

function calculaMesesValidos( $sbaid ){
	
	global $db;
	
	$arrDatas = pegaPeriodosAtuacao( $sbaid );

	$dataMin = '';
	foreach( $arrDatas as $data ){
		$ini = explode('-',$data['dt_inicio']);
		$meses_ini = ($ini[0]*12)+$ini[1];
		$fim = explode('-',$data['dt_fim']);
		$meses_fim = ($fim[0]*12)+$fim[1];
		if( $dataMin == '' && ($meses_fim-$meses_ini)+1 > 11 ){
			$dataMin = $ini[0].$ini[1];
		}
	}

	if( $dataMin == '' ){
		return '0';
	}
	
	$sql = "SELECT DISTINCT
				qtd as qtd_meses_trabalhados
			FROM
				fiesabatimento.solicitacaoabatimento sba
			INNER JOIN fiesabatimento.identificacaodocente 	ido ON ido.idoid = sba.idoid
			INNER JOIN fiesabatimento.atuacaoprofissional 	atp ON atp.sbaid = sba.sbaid
			LEFT  JOIN 
				(
				SELECT
					sbaid,
					count(DISTINCT matano||matmes) as qtd
				FROM
					(
					SELECT DISTINCT
						sbaid,
						matano,
						matmes,
						sum(atpnumcargahoraria) as hrs
					FROM
						(
						SELECT DISTINCT
							mat.sbaid,
							atp.atpid,
							matano,
							matmes,
							atpnumcargahoraria
						FROM
							fiesabatimento.mesesatuacao mat
						INNER JOIN fiesabatimento.atuacaoprofissional atp ON atp.atpid = mat.atpid
						WHERE
							(matano||lpad(matmes,2,'0'))::integer >= $dataMin::integer
						ORDER BY matano, matmes
						) sub
					GROUP BY sbaid, matano, matmes
					ORDER BY matano, matmes
				) as sub2
			GROUP BY sbaid) as qtd ON qtd.sbaid = sba.sbaid
			WHERE
			sba.sbaid = ".$sbaid;
	$sbaqmtvalido = $db->pegaUm($sql);
	
	return $sbaqmtvalido;
}

function calculaMesesAprovados( $sbaid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				qtd as qtd_meses_trabalhados
			FROM
				fiesabatimento.solicitacaoabatimento sba
			INNER JOIN fiesabatimento.identificacaodocente 	ido ON ido.idoid = sba.idoid
			INNER JOIN fiesabatimento.atuacaoprofissional 	atp ON atp.sbaid = sba.sbaid
			LEFT  JOIN 
				(
				SELECT
					sbaid,
					count(DISTINCT matano||matmes) as qtd
				FROM
					(
					SELECT DISTINCT
						sbaid,
						matano,
						matmes,
						sum(atpnumcargahoraria) as hrs
					FROM
						(
						SELECT DISTINCT
							mat.sbaid,
							atp.atpid,
							matano,
							matmes,
							atpnumcargahoraria
						FROM
							fiesabatimento.mesesatuacao mat
						INNER JOIN fiesabatimento.atuacaoprofissional atp ON atp.atpid = mat.atpid
						ORDER BY matano, matmes
						) sub
					GROUP BY sbaid, matano, matmes
					ORDER BY matano, matmes
					) as sub2
				GROUP BY sbaid) as qtd ON qtd.sbaid = sba.sbaid
			WHERE
			sba.sbaid = ".$sbaid;
	$sbaqmtaprovado = $db->pegaUm($sql);
	
	return $sbaqmtaprovado;
}

function confirmarSolicitacaoCancela($request){
	
	global $db;
	
	extract($request);	
	
	$sql = "SELECT 
				idoid 
			FROM 
				fiesabatimento.identificacaodocente 
			WHERE idocpf='".$_SESSION['fiesabatimento_var']['cpfusuario']."'";
	
	$idoid = $db->pegaUm($sql);
	 
	
	$sbaid = pegaSbaidSolicitacao($idoid);
	$docid = pegaDocidSolicitacao($idoid);
	$esdidSolicitacao = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = $docid");
	
	//grava CANCELAMENTO
	$atpids = pegaAtpidsSolicitacao( $sbaid );
	
	if( is_array($atpids) ){
		foreach( $atpids as $atpid ){
			$docidAt = pegaDocidAtuacao($atpid);
			$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = $docidAt");
			
			if( $esdid == ESDID_AGUARDANDO_ANALISE ){
				$acao = AEDID_CANCELAR_ATUACAO;
			}elseif( $esdid == ESDID_ANALISADO ){
				$acao = AEDID_CANCELAR_ATUACAO_ANAL;
			}elseif( $esdid == ESDID_REJEITADO ){
				$acao = AEDID_CANCELAR_ATUACAO_REJ;
			}
			wf_alterarEstado( $docidAt, $acao, 'Cancelado pelo professor - Tela de cancelamento de solicitação', array('docid'=>$docid ) );
		}
	}
	
	$sql = "UPDATE fiesabatimento.solicitacaoabatimento SET sbastatus = 'I' WHERE sbaid = $sbaid";
	$db->executar($sql);
	$sql = "DELETE FROM fiesabatimento.solicitacaoabatimentocancela WHERE sbaid = $sbaid";
	$db->executar($sql);
	$comentario = '';
	
	if(is_array($mccid)){
		foreach($mccid as $mc){
			
			if($mc == 7){
				$sql = "INSERT INTO fiesabatimento.solicitacaoabatimentocancela (sbaid, mccid, sacoutrosmotivos, sacdatagravacaoregistro, sacstatus)
		    			VALUES ($sbaid, $mc, '$sacoutrosmotivos', now(), 'A');";
			}
			else{
				$sql = "INSERT INTO fiesabatimento.solicitacaoabatimentocancela (sbaid, mccid, sacdatagravacaoregistro, sacstatus)
		    			VALUES ($sbaid, $mc, now(), 'A');";
			}
			$db->executar($sql);
			if( $mc == 7 ){
				$comentario .= ' - '.$sacoutrosmotivos.';';
			}else{
				$sql = "SELECT mccdesc FROM fiesabatimento.motivocancelamento WHERE mccid = $mc";
				$comentario .= ' - '.$db->pegaUm($sql).';';
			}
		}
	}
	
	//altera docid
	$aedid = WF_FIES1_CANCELAR_ABATIMENTO_PEND;
	
	if( $esdidSolicitacao == WF_FIES1_REENVIO ){ $aedid = WF_FIES1_CANCELAR_ABATIMENTO_REEN; }
	if( $esdidSolicitacao == WF_FIES1_REENVIO_PRAZO ){ $aedid = WF_FIES1_CANCELAR_ABATIMENTO_REEN_PRAZO; }
	if( $esdidSolicitacao == WF_FIES1_APROVADA ){ $aedid = WF_FIES1_CANCELAR_ABATIMENTO_APRV; }
	
	$test = wf_alterarEstado( $docid, $aedid, $comentario, array('docid'=>$docid ) );
	echo "<script>alert('Operação Efetuada com Sucesso.'); window.location = 'fiesabatimento.php?modulo=principal/cancelarabatimento&acao=A';</script>";
	
}

/* Aguardando correção WS
 * 
 * */
function confirmarSolicitacao($dados) {
	
	global $db;
	
	 //$cpfOld = $_SESSION['usucpf'];
	 //$_SESSION['usucpf'] = CPF_PROFESSOR;
	
	if( $dados['sbaid'] == '' ){
		if( $dados['idoid'] == '' ){
			
			$conf['cpf'] = $_SESSION['fiesabatimento_var']['cpfusuario'];
			$conf['servico'] = 'lerDadosPessoais';
			
			$dadosPess = wf_lerDados( $conf );
			
			$values = "VALUES ('{$dadosPess[0]['co_municipio']}', '{$dadosPess[0]['nu_cpf']}', '{$dadosPess[0]['no_pessoa']}', '{$dadosPess[0]['nu_rg']}', '{$dadosPess[0]['nu_cep']}', 
					 '{$dadosPess[0]['ds_logradouro']}', '{$dadosPess[0]['ds_numero']}', '{$dadosPess[0]['ds_complemento']}',
					 '{$dadosPess[0]['ds_bairro']}', '{$dadosPess[0]['nu_ddd']}', '{$dadosPess[0]['nu_telefone']}',
					 '{$dadosPess[0]['ds_contato_eletronico']}', null, '{$dadosPess[0]['dt_nascimento']}', now(), 'A', '{$dadosPess[0]['co_municipio']}', '{$dadosPess[0]['sg_uf']}')";
			
			$sql = "INSERT INTO fiesabatimento.identificacaodocente(
			            co_municipio, idocpf, idonome, idorg, idocep, idoendereco, 
			            idoenumero, idocomplemento, idobairro, idotetelefoneddd, idotelefone, 
			            idoeemail, idogravacao, idodatanascimento, idodatagravacao, idostatus,
			            muncodprofessor, estufprofessor)
					$values
					RETURNING
						idoid";
// 					SELECT 
// 						co_municipio, nu_cpf, no_pessoa, nu_rg, nu_cep, ds_logradouro, 
// 						ds_numero, ds_complemento, ds_bairro, nu_ddd, nu_telefone, 
// 						ds_contato_eletronico, NULL, dt_nascimento, NOW(), 'A',
// 						co_municipio2, sg_uf
// 					FROM dblink (
// 					'".PARAM_DBLINK_FREIRE."',
// 					'select 
// 						pf.nu_cpf, coalesce(pf.nu_rg,\'0\'), pe.no_pessoa, en.nu_cep, en.ds_logradouro, en.ds_numero, en.ds_complemento, 
// 						en.ds_bairro, mu.no_municipio_acento, es.no_uf, ce.ds_contato_eletronico, en.co_municipio,
// 						tl.nu_ddd, tl.nu_telefone, dt_nascimento, mu.co_municipio, es.sg_uf
// 					from public.tb_sf_pessoa pe 
// 					inner join public.tb_sf_pessoa_fisica pf on pe.co_pessoa = pf.co_pessoa_fisica 
// 					left join public.tb_sf_contato_eletronico ce on ce.co_pessoa = pe.co_pessoa 
// 					left join public.tb_sf_telefone tl on tl.co_pessoa = pe.co_pessoa 
// 					left join public.tb_sf_endereco en on en.co_pessoa = pe.co_pessoa 
// 					left join public.tb_sf_municipio mu on mu.co_municipio = en.co_municipio 
// 					left join public.tb_sf_uf es on es.co_uf = mu.co_uf
// 					where pf.nu_cpf=\'".$_SESSION['fiesabatimento_var']['cpfusuario']."\' limit 1;'
// 					) as rs (
// 						nu_cpf character(11),
// 						nu_rg character varying(20),
// 						no_pessoa character varying(100),
// 						nu_cep integer,
// 						ds_logradouro character varying(100),
// 						ds_numero character varying(10),
// 						ds_complemento character varying(150),
// 						ds_bairro character varying(100),
// 						no_municipio_acento character varying(200),
// 						no_uf character varying(100),
// 						ds_contato_eletronico character varying(150),
// 						co_municipio integer,
// 						nu_ddd character(2),
// 						nu_telefone character(8),
// 						dt_nascimento date,
// 						co_municipio2 character(7),
// 						sg_uf character(2)
// 					) RETURNING idoid";
			
			$idoid = $db->pegaUm($sql);
		}else{
			$idoid = $dados['idoid'];
		}


		if($dados['sbarenovacao'] == 't'){
			$docid = wf_cadastrarDocumento( TPDID, 'Renovação Abatimento - '.$_SESSION['fiesabatimento_var']['cpfusuario'] );
		}else{
			$docid = wf_cadastrarDocumento( TPDID, 'Solicitação Abatimento - '.$_SESSION['fiesabatimento_var']['cpfusuario'] );
		}


		//$docid = wf_cadastrarDocumento( TPDID, 'Solicitação Abatimento - '.$_SESSION['fiesabatimento_var']['cpfusuario'] );
		
		/*
		$sql = "INSERT INTO fiesabatimento.solicitacaoabatimento(
	            idoid, sbaqmtsolicitado, sbasolicitacaoatendida, 
				sbastatus, docid, preid)
			    VALUES ('".$idoid."','".(($dados['sbaqmtsolicitado'])?$dados['sbaqmtsolicitado']:"0")."',".(($dados['sbasolicitacaoatendida'])?$dados['sbasolicitacaoatendida']:"false").",'A', $docid, ".(($dados['preid'])?$dados['preid']:"null").")
			   returning sbaid;";
		*/

		$sql = "INSERT INTO fiesabatimento.solicitacaoabatimento(
	            idoid, sbaqmtsolicitado, sbasolicitacaoatendida,
				sbastatus, docid, sbaanoinicio, sbaanofim, sbarenovacao)
			    VALUES ('".$idoid."',
			    		'".(($dados['sbaqmtsolicitado'])?$dados['sbaqmtsolicitado']:"0")."',
			    		".(($dados['sbasolicitacaoatendida'])?$dados['sbasolicitacaoatendida']:"false").",
			    		'A',
			    		$docid,
			    		'".$dados['sbaanoinicio']."',
			    		'".$dados['sbaanofim']."',
			    		'".$dados['sbarenovacao']."')
			   returning sbaid;";
		$sbaid = $db->pegaUm($sql);
		
		
		$conf['cpf'] = $_SESSION['fiesabatimento_var']['cpfusuario'];
		$conf['servico'] = 'lerDadosAtuacao';
			
		//dbg($conf);
		$dadosAtu = wf_lerDados( $conf );
 		//ver(1,1,$conf,$dadosAtu,d);

		foreach( $dadosAtu as $atu ){
			
			//$atu['co_entidade'] = 01;
			if(!$atu['co_entidade']) $atu['co_entidade'] = 1;
		
			$sql = "INSERT INTO fiesabatimento.atuacaoprofissional(
			            co_pessoa_juridica, co_funcao, co_tipo_vinculo,
			            idoid, atpdatainicio, atpdatafim, atpnumcargahoraria, atpstatus, atpinep,
			            atpdescricaoescola,atpvinculo,atpfuncao,sbaid,
			            estufprofessor, muncodprofessor,esferaprofessor)
					VALUES (
						{$atu['co_entidade']}, '{$atu['co_funcao']}', '{$atu['co_tipo_vinculo']}', 
						$idoid, '{$atu['dt_inicio']}', ".($atu['dt_fim'] ? "'{$atu['dt_fim']}'" : "null").", 
					 	{$atu['nu_carga_horaria']}, 'A', '{$atu['co_inep']}', 
					 	'{$atu['no_entidade']}', '{$atu['no_tipo_vinculo']}', '{$atu['no_funcao']}', $sbaid,
					 	'{$atu['sg_uf']}', '{$atu['co_municipio']}', '{$atu['co_dep_adm']}')
					RETURNING
						atpid;";
			$atpid = $db->pegaUm($sql);
		}
		$db->commit();
		
// 		$sql = "INSERT INTO fiesabatimento.atuacaoprofissional(
// 		            co_pessoa_juridica, co_funcao, co_tipo_vinculo,  
// 		            idoid, atpdatainicio, atpdatafim, atpnumcargahoraria, atpstatus, atpinep, 
// 		            atpdescricaoescola,atpvinculo,atpfuncao,sbaid,
// 		            estufprofessor, muncodprofessor,esferaprofessor)
// 				select 
// 					co_pessoa_juridica, co_funcao, co_tipo_vinculo, '".$idoid."' as idoid, dt_inicio, dt_fim, 
// 					nu_carga_horaria, 'A',co_inep,no_fantasia,no_tipo_vinculo, no_funcao,$sbaid,
// 					sg_uf, co_municipio::character(7), co_dep_adm
// 				from dblink (
// 				'".PARAM_DBLINK_FREIRE."',
// 				'select 
// 					pj.co_pessoa_juridica, no_dep_adm, co_inep, no_fantasia, no_tipo_vinculo, 
// 					no_funcao, at.nu_carga_horaria, at.dt_inicio, 
// 					CASE WHEN to_char(at.dt_fim,\'YYYYMM\') >= to_char(now(),\'YYYYMMDD\') THEN NULL ELSE at.dt_fim END as dt_fim, at.co_funcao, at.co_tipo_vinculo,
// 					sg_uf, mu.co_municipio, pj.co_dep_adm
// 				FROM 
// 					public.tb_sf_curriculo cu 
// 				INNER JOIN public.tb_sf_atuacao 					at  ON at.co_curriculo = cu.co_curriculo 
// 				LEFT  JOIN tb_sf_atuacao_eb                         eb  ON eb.co_atuacao_eb = at.co_atuacao
// 				LEFT  JOIN tb_sf_atuacao_sup                        sup ON sup.co_atuacao_sup = at.co_atuacao
// 				INNER JOIN public.tb_sf_pessoa_fisica 				pf  ON pf.co_pessoa_fisica = cu.co_curriculo 
// 				INNER JOIN public.tb_sf_pessoa_juridica 			pj  ON pj.co_pessoa_juridica = at.co_pessoa_juridica
// 				INNER JOIN public.tb_sf_dependencia_administrativa 	da  ON da.co_dep_adm = pj.co_dep_adm 
// 				INNER JOIN public.tb_sf_tipo_vinculo 				vi  ON vi.co_tipo_vinculo = at.co_tipo_vinculo 
// 				INNER JOIN public.tb_sf_funcao 						fu  ON fu.co_funcao = at.co_funcao 
// 				LEFT  JOIN public.tb_sf_endereco 					en  ON en.co_pessoa = pj.co_pessoa_juridica
// 				LEFT  JOIN public.tb_sf_municipio 					mu  ON mu.co_municipio = en.co_municipio 
// 				LEFT  JOIN public.tb_sf_uf 							es  ON es.co_uf = mu.co_uf
// 				WHERE 
// 						pj.co_tipo_pessoa_juridica = 4 --Escola
// 					AND 	coalesce (eb.co_nivel_escolar,sup.co_nivel_escolar) IN (1,2,3,18,19) --(Ensino fundamentla, Ensino Fundamentla, Ensino Médio, Médio - Magistério, Médio - Indigena)
// 					AND 	at.co_funcao = 2 --DOCENTE (PROFESSOR)
// 					AND 	pj.co_dep_adm IN (\'M\', \'E\', \'F\') --Municipal / Estadual / Federal (REDE PUBLICA)
// 					AND 	at.co_tipo_vinculo IN (1,2,5) --Servidor Publicao/ Selestina Formal/ Contrato Temporário
// 					AND 	pf.nu_cpf=\'".$_SESSION['fiesabatimento_var']['cpfusuario']."\';'
// 				) as rs (
// 				co_pessoa_juridica integer,
// 				no_dep_adm character varying(20),
// 				co_inep integer,
// 				no_fantasia character varying(100),
// 				no_tipo_vinculo character varying(50),
// 				no_funcao character varying(50),
// 				nu_carga_horaria smallint,
// 				dt_inicio date,
// 				dt_fim date,
// 				co_funcao integer,
// 				co_tipo_vinculo integer,
// 				sg_uf character(2),
// 				co_municipio integer,
// 				co_dep_adm character(1)
// 				) returning atpid";
// 		$atpid = $db->pegaUm($sql);
	}else{
		//$docid = pegaDocidSolicitacao($dados['idoid'], $dados['preid']);
		//$sbaid = pegaSbaidSolicitacao($dados['idoid'], $dados['preid']);
		//$sbaid = pegaSbaidSolicitacao($dados['idoid'], $dados['sbarenovacao']);
		$sbaid = $dados['sbaid'];

		if(!$sbaid){
			echo '<script>
					alert("Erro na solicitação. Favor tente novamente ou entre em contato com o gestor do sistema!");
					history.back();
			  </script>';
			die;
		}


		$docid = pegaDocidSolicitacaoSbaid($sbaid);
		
// 		$sql = "SELECT DISTINCT *
// 				FROM
// 				dblink (
// 				'".PARAM_DBLINK_FREIRE."', '
// 				SELECT DISTINCT
// 					da.co_dep_adm, co_inep, no_fantasia, no_tipo_vinculo, no_funcao, at.nu_carga_horaria,
// 					at.dt_inicio, at.dt_fim, COALESCE(at.dt_fim - at.dt_inicio,0) as difmeses
// 				FROM
// 					public.tb_sf_pessoa pe
// 				INNER join public.tb_sf_pessoa_fisica 		  		pf ON pe.co_pessoa = pf.co_pessoa_fisica
// 				INNER JOIN public.tb_sf_curriculo 		  			cu ON cu.co_curriculo = pf.co_pessoa_fisica
// 				INNER JOIN public.tb_sf_atuacao 		  			at ON at.co_curriculo = cu.co_curriculo
// 				LEFT  JOIN tb_sf_atuacao_eb                         eb ON eb.co_atuacao_eb = at.co_atuacao
// 				LEFT  JOIN tb_sf_atuacao_sup                    	sup ON sup.co_atuacao_sup = at.co_atuacao
// 				INNER JOIN public.tb_sf_pessoa_juridica 	  		pj ON pj.co_pessoa_juridica = at.co_pessoa_juridica
// 				INNER JOIN public.tb_sf_dependencia_administrativa 	da ON da.co_dep_adm = pj.co_dep_adm
// 				INNER JOIN public.tb_sf_tipo_vinculo 		  		vi ON vi.co_tipo_vinculo = at.co_tipo_vinculo
// 				INNER JOIN public.tb_sf_funcao 			  			fu ON fu.co_funcao = at.co_funcao
// 				WHERE
// 					pj.co_tipo_pessoa_juridica = 4 --Escola
// 					AND 	coalesce (eb.co_nivel_escolar,sup.co_nivel_escolar) IN (1,2,3,18,19) --(Ensino fundamentla, Ensino Fundamentla, Ensino Médio, Médio - Magistério, Médio - Indigena)
// 					AND 	at.co_funcao = 2 --DOCENTE (PROFESSOR)
// 					AND 	pj.co_dep_adm IN (\'M\', \'E\', \'F\') --Municipal / Estadual / Federal (REDE PUBLICA)
// 					AND 	at.co_tipo_vinculo IN (1,2,5) --Servidor Publicao/ Selestina Formal/ Contrato Temporário
// 					AND 	pf.nu_cpf=\'".$_SESSION['fiesabatimento_var']['cpfusuario']."\';'
// 				) as rs (
// 					no_dep_adm character varying(20),
// 					co_inep integer,
// 					no_fantasia character varying(100),
// 					no_tipo_vinculo character varying(50),
// 					no_funcao character varying(50),
// 					nu_carga_horaria smallint,
// 					dt_inicio date,
// 					dt_fim date,
// 					difmeses integer
// 				)
// 				ORDER BY
// 					dt_inicio, dt_fim";
		
// 		$dadosFiltro = $db->carregar($sql);

		$conf['cpf'] = $_SESSION['fiesabatimento_var']['cpfusuario'];
		$conf['servico'] = 'lerDadosAtuacao';
			
		$dadosFiltro = wf_lerDados( $conf );
		
		$where = Array('1=0');
		if( is_array( $dadosFiltro ) ){
			foreach( $dadosFiltro as $dado ){
				if( $dado['dt_fim'] == '' ) $dado['dt_fim'] = 'NULL';
				else $dado['dt_fim'] = "'{$dado['dt_fim']}'";
				$where[] = " ( 	esferaprofessor = '".$dado['co_dep_adm']."'
								AND atpinep = '".$dado['co_inep']."'
								AND atpvinculo = '".$dado['no_tipo_vinculo']."'
								AND atpnumcargahoraria = ".$dado['nu_carga_horaria']."
								AND atpdatainicio = '".$dado['dt_inicio']."'
								AND atpdatafim ".($dado['dt_fim'] ? "= ".$dado['dt_fim'] : " IS NULL ")." ) ";
			}
		}
		
		$sql = "SELECT atpid FROM fiesabatimento.atuacaoprofissional WHERE ".implode(' OR ', $where);
		$atpids = $db->carregarColuna($sql);
		
		$sql = "UPDATE fiesabatimento.atuacaoprofissional SET atpstatus = 'I' 
				WHERE sbaid = $sbaid AND ( atpdatareabertura IS NOT NULL ".($atpids[0]!='' ? 'OR atpid NOT IN ('.implode(',',$atpids).')' : '').")";
		$db->executar($sql);
		$db->commit();
		
		foreach( $atpids as $atpid ){
			$docidAt = pegaDocidAtuacao($atpid);
			wf_alterarEstado( $docidAt, AEDID_ENVIAR_ANALISE, 'Reenvio para análise', array('docid'=>$docid ) );
		}
		
		$sql = "SELECT
					atp.esferaprofessor AS co_dep_adm,
					atp.atpinep AS co_inep,
					atp.atpdescricaoescola AS no_entidade,
					atp.atpvinculo AS no_tipo_vinculo,
					atp.atpfuncao AS no_funcao,
					atp.atpnumcargahoraria AS nu_carga_horaria,
					atp.atpdatainicio AS dt_inicio,
					atp.atpdatafim AS dt_fim,
					COALESCE(atp.atpdatafim - atp.atpdatainicio,0) AS difmeses,
					atp.atpid
				FROM
					fiesabatimento.solicitacaoabatimento sba
				INNER JOIN fiesabatimento.atuacaoprofissional atp ON atp.sbaid = sba.sbaid
				WHERE
					sba.sbaid = $sbaid
					AND sbastatus = 'A'
					AND atpstatus = 'A'
				ORDER BY
					no_entidade";
		
		
		$dadosFiltro = $db->carregar($sql);
		$where = Array('1=1');
		
		$conf['cpf'] = $_SESSION['fiesabatimento_var']['cpfusuario'];
		$conf['servico'] = 'lerDadosAtuacao';
			
		$dadosAtu = wf_lerDados( $conf );
		
		
		//if( is_array( $dadosFiltro ) && is_array($dadosAtu) ){
		if( is_array($dadosAtu) ){
			foreach( $dadosAtu as $atu ){
				$ok = true;
				/*
				foreach( $dadosFiltro as $dado ){
					if( $dado['co_dep_adm'] == $dados['co_dep_adm'] &&
						$dado['co_inep'] == $dados['co_inep'] &&
						$dado['no_tipo_vinculo'] == $dados['no_tipo_vinculo'] &&
						$dado['nu_carga_horaria'] == $dados['nu_carga_horaria'] &&
						$dado['dt_inicio'] == $dados['dt_inicio'] &&
						$dado['dt_fim'] == $dados['dt_fim'] ){
						
						$ok = false;
					}
// 					$where[] = " ( 	pj.co_dep_adm != \'".$dado['co_dep_adm']."\'
// 									OR co_inep != \'".$dado['co_inep']."\'
// 									OR no_tipo_vinculo != \'".$dado['no_tipo_vinculo']."\'
// 									OR nu_carga_horaria != ".$dado['nu_carga_horaria']."
// 									OR dt_inicio != \'".$dado['dt_inicio']."\'
// 									OR dt_fim ".($dado['dt_fim'] ? "!= \'".$dado['dt_fim']."\'" : " IS NOT NULL")." ) ";
				}
				*/
				
				if( $ok ){
					if(!$atu['co_entidade']) $atu['co_entidade'] = 1;
					$sql = "INSERT INTO fiesabatimento.atuacaoprofissional(
								co_pessoa_juridica, co_funcao, co_tipo_vinculo,
								idoid, atpdatainicio, atpdatafim, atpnumcargahoraria, atpstatus, atpinep,
								atpdescricaoescola,atpvinculo,atpfuncao,sbaid,
								estufprofessor, muncodprofessor,esferaprofessor)
							VALUES(
								{$atu['co_entidade']}, {$atu['co_funcao']}, {$atu['co_tipo_vinculo']},
								{$dados['idoid']}, '{$atu['dt_inicio']}', ".($atu['dt_fim'] ? "'{$atu['dt_fim']}'" : "null").", 
								'{$atu['nu_carga_horaria']}', 'A', '{$atu['co_inep']}',
								'{$atu['no_entidade']}', {$atu['co_tipo_vinculo']}, '{$atu['no_funcao']}', $sbaid, 
								'{$atu['sg_uf']}', '{$atu['co_municipio']}', '{$atu['co_dep_adm']}'
							)
							RETURNING atpid";
					
					$atpid = $db->pegaUm($sql);
				}
				
				
				
			}
			$db->commit();
		}
		
// 		$sql = "INSERT INTO fiesabatimento.atuacaoprofissional(
// 					co_pessoa_juridica, co_funcao, co_tipo_vinculo,
// 					idoid, atpdatainicio, atpdatafim, atpnumcargahoraria, atpstatus, atpinep,
// 					atpdescricaoescola,atpvinculo,atpfuncao,sbaid,
// 					estufprofessor, muncodprofessor,esferaprofessor)
// 				SELECT
// 					co_pessoa_juridica, co_funcao, co_tipo_vinculo, '".$dados['idoid']."' as idoid, dt_inicio, dt_fim,
// 					nu_carga_horaria, 'A',co_inep,no_fantasia,no_tipo_vinculo, no_funcao,$sbaid,
// 					sg_uf, co_municipio::character(7), co_dep_adm
// 				FROM dblink (
// 					'".PARAM_DBLINK_FREIRE."',
// 					'SELECT
// 						pj.co_pessoa_juridica, no_dep_adm, co_inep, no_fantasia, no_tipo_vinculo,
// 						no_funcao, at.nu_carga_horaria, at.dt_inicio, 
// 						CASE WHEN to_char(at.dt_fim,\'YYYYMM\') >= to_char(now(),\'YYYYMMDD\') THEN NULL ELSE at.dt_fim END as dt_fim, at.co_funcao, at.co_tipo_vinculo,
// 						sg_uf, mu.co_municipio, pj.co_dep_adm
// 					FROM 
// 						public.tb_sf_curriculo cu
// 					INNER JOIN public.tb_sf_atuacao 					at ON at.co_curriculo = cu.co_curriculo
// 					LEFT  JOIN tb_sf_atuacao_eb 						eb ON eb.co_atuacao_eb = at.co_atuacao
// 					LEFT  JOIN tb_sf_atuacao_sup 						sup ON sup.co_atuacao_sup = at.co_atuacao
// 					INNER JOIN public.tb_sf_pessoa_fisica 				pf ON pf.co_pessoa_fisica = cu.co_curriculo
// 					INNER JOIN public.tb_sf_pessoa_juridica 			pj ON pj.co_pessoa_juridica = at.co_pessoa_juridica
// 					INNER JOIN public.tb_sf_dependencia_administrativa 	da ON da.co_dep_adm = pj.co_dep_adm
// 					INNER JOIN public.tb_sf_tipo_vinculo 				vi ON vi.co_tipo_vinculo = at.co_tipo_vinculo
// 					INNER JOIN public.tb_sf_funcao 						fu ON fu.co_funcao = at.co_funcao
// 					LEFT JOIN public.tb_sf_endereco 					en ON en.co_pessoa = pj.co_pessoa_juridica
// 					LEFT JOIN public.tb_sf_municipio 					mu ON mu.co_municipio = en.co_municipio
// 					LEFT JOIN public.tb_sf_uf 							es ON es.co_uf = mu.co_uf
// 					WHERE
// 						pj.co_tipo_pessoa_juridica = 4 --Escola
// 						AND 	coalesce (eb.co_nivel_escolar,sup.co_nivel_escolar) IN (1,2,3,18,19) --(Ensino fundamentla, Ensino Fundamentla, Ensino Médio, Médio - Magistério, Médio - Indigena)
// 						AND 	at.co_funcao = 2 --DOCENTE (PROFESSOR)
// 						AND 	pj.co_dep_adm IN (\'M\', \'E\', \'F\') --Municipal / Estadual / Federal (REDE PUBLICA)
// 						AND 	at.co_tipo_vinculo IN (1,2,5) --Servidor Publicao/ Selestina Formal/ Contrato Temporário
// 						AND 	pf.nu_cpf=\'".$_SESSION['fiesabatimento_var']['cpfusuario']."\'
// 						AND 	(".implode(' AND ',$where)." )'
// 				) as rs (
// 					co_pessoa_juridica integer,
// 					no_dep_adm character varying(20),
// 					co_inep integer,
// 					no_fantasia character varying(100),
// 					no_tipo_vinculo character varying(50),
// 					no_funcao character varying(50),
// 					nu_carga_horaria smallint,
// 					dt_inicio date,
// 					dt_fim date,
// 					co_funcao integer,
// 					co_tipo_vinculo integer,
// 					sg_uf character(2),
// 					co_municipio integer,
// 					co_dep_adm character(1)
// 				) 
// 				RETURNING atpid";
		
// 		$atpid = $db->pegaUm($sql);
	}
	
	$atpids = pegaAtpidsSolicitacao( $sbaid );
	
	if( is_array($atpids) ){
		$sql = '';
		foreach( $atpids as $atpid ){
			
			$sqlAt = "SELECT
						atpinep as codinep,
						to_char(atpdatainicio,'YYYY-MM-DD') as dt_inicio,
						to_char(atpdatafim,'YYYY-MM') as dt_fim
					FROM
						fiesabatimento.atuacaoprofissional
					WHERE
						atpid = $atpid;";
			//dbg($sqlAt,1);
			$atuacao = $db->pegaLinha($sqlAt);
			
			/*
			if( $atuacao['data_fim'] ){
				$diasMes = Array('',31,28,31,30,31,30,31,31,30,31,30,31);
				$tempData = explode('-', $atuacao['data_fim']);
				$atuacao['data_fim'] = $tempData[0].'-'.$tempData[1].'-'.$diasMes[(integer)$tempData[1]];
			}
			*/
			
			unset($dt_inicio);
			unset($dt_fim);
			
			if( $atuacao['dt_inicio'] ){
				 $atuacao['dt_inicio'] = str_replace("/","-",$atuacao['dt_inicio']);
				 $dt_inicioArray = explode('-', $atuacao['dt_inicio']);
				 if(strlen($dt_inicioArray[2]) == 4) $dt_inicio=$dt_inicioArray[2].'-'.$dt_inicioArray[1].'-'.$dt_inicioArray[0];
	   			 if(strlen($dt_inicioArray[0]) == 4) $dt_inicio=$dt_inicioArray[0].'-'.$dt_inicioArray[1].'-'.$dt_inicioArray[2];
			}
			
			if( $atuacao['dt_fim'] ){
				$diasMes = Array('',31,28,31,30,31,30,31,31,30,31,30,31);
				$atuacao['dt_fim'] = str_replace("/","-",$atuacao['dt_fim']);
				$tempData = explode('-', $atuacao['dt_fim']);
				if(strlen($tempData[2]) == 4) $dt_fim=$tempData[2].'-'.$tempData[1].'-'.$diasMes[(integer)$tempData[0]];
	   			if(strlen($tempData[0]) == 4) $dt_fim=$tempData[0].'-'.$tempData[1].'-'.$diasMes[(integer)$tempData[2]];
			}
			
			if( $dados['efetivo_exercicio'][$atuacao['codinep'].'_'.$dt_inicio.'_'.$dt_fim] ){
				$atualizaEfetivoExercicio = ", atpprofefetivoexercicio = {$dados['efetivo_exercicio'][$atuacao['codinep'].'_'.$dt_inicio.'_'.$dt_fim]}";
			}else{
				$atualizaEfetivoExercicio = "";
			}
			
			$docidAt = pegaDocidAtuacao($atpid);
			
			if( $docidAt == '' ){
				$docidAt = wf_cadastrarDocumento( TPDID_ANALISE_SITUACAO, "Atuação Profissional - $sbaid - ".$_SESSION['fiesabatimento_var']['cpfusuario'] );
				
				$sql .= "UPDATE fiesabatimento.atuacaoprofissional SET
							docid = $docidAt,
							atprespsecretario = NULL
							$atualizaEfetivoExercicio 
						WHERE
							atpid = $atpid;";
			}
			
		}
		if( $sql != '' ){
			$db->executar($sql);
		}
	}
	
	$db->commit();


	if($dados['sbarenovacao'] == 't'){
		$comentario = 'Renovação enviada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
	}else{
		$comentario = 'Solicitação enviada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
	}

	//$comentario = 'Solicitação enviada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
	
	$sql = "SELECT esdid FROM workflow.documento WHERE docid = $docid";
	
	$esdid = $db->pegaUm( $sql );
	
	$aedid = WF_FIES1_ENVIAR_SOLICITACAO;
	if( $esdid == WF_FIES1_REENVIO ) $aedid = WF_FIES1_ENVIAR_SOLICITACAO_REENV;
	if( $esdid == WF_FIES1_REENVIO_PRAZO ) $aedid = WF_FIES1_ENVIAR_SOLICITACAO_REENV_PRAZO;
		
	$test = wf_alterarEstado( $docid, $aedid, $comentario, array('docid'=>$docid ) );
	$db->commit();
	
	$dados['hstid']	= pegaUltimaTramitacao($docid);
	$dados['sbaid'] = $sbaid;
	$dados['atpid'] = 'null';
	$dados['htrperfil'] =  pegaPerfilGeral();
	$dados['hrtmotivoreabertura'] = 'NULL';
	
	insereHistoricoTramitacao($dados);
	
	//$_SESSION['usucpf'] = $cpfOld;


	if($dados['sbarenovacao'] == 't'){
		echo '<script>'.
					'alert(\'Renovação confirmada. A presente renovação deverá ser aprovada pelo(s) Secretário(s)'.
					' de Educação, observado o teor e as condições declaradas do item "Declaração" da sua renovação. '.
					'Prestações não pagas até a efetiva suspensão da cobrança do saldo devedor deverão ser quitadas.\');
					window.location.href = \'fiesabatimento.php?modulo=principal/renovarabatimento&acao=A\';
			  </script>';
	}else{
		echo '<script>'.
					'alert(\'Solicitação confirmada. A presente solicitação deverá ser aprovada pelo(s) Secretário(s)'.
					' de Educação, observado o teor e as condições declaradas do item "Declaração" da sua solicitação. '.
					'Prestações não pagas até a efetiva suspensão da cobrança do saldo devedor deverão ser quitadas.\');
					window.location.href = \'fiesabatimento.php?modulo=principal/identificacaoabatimento&acao=A\';
			  </script>';	
	}

	/*
	echo '<script>'.
				'alert(\'Solicitação confirmada. A presente solicitação deverá ser aprovada pelo(s) Secretário(s)'.
				' de Educação, observado o teor e as condições declaradas do item "Declaração" da sua solicitação. '.
				'Prestações não pagas até a efetiva suspensão da cobrança do saldo devedor deverão ser quitadas.\');
				window.location.href = \'fiesabatimento.php?modulo=principal/identificacaoabatimento&acao=A\';
		  </script>';
	*/
}

function pegaDocidAtuacao( $atpid ){

	global $db;

	$sql = "SELECT
				docid
			FROM
				fiesabatimento.atuacaoprofissional
			WHERE
				atpid = $atpid";
	return $db->pegaUm($sql);
}

function pegaSbaidAtuacao( $atpid ){

	global $db;

	$sql = "SELECT
				sbaid
			FROM
				fiesabatimento.atuacaoprofissional
			WHERE
				atpid = $atpid";
	return $db->pegaUm($sql);
}

function pegaAtpidsSolicitacao( $sbaid, $where = null ){

	global $db;

	$sql = "SELECT
				atpid
			FROM
				fiesabatimento.atuacaoprofissional
			WHERE
				sbaid = $sbaid AND atpstatus = 'A' $where";
	
	return $db->carregarColuna($sql);
}

//Função legado...
function pegaAtpidSolicitacao($sbaid){
	
	global $db;
	
	$sql = "SELECT
				atpid
			FROM
				fiesabatimento.atuacaoprofissional
			WHERE
				sbaid = $sbaid AND atpstatus = 'A'";
	return $db->pegaUm($sql);
}

function pegaSbaidSolicitacao($idoid, $sbarenovacao = 'f'){
	
	global $db;

	/*
	if($preid){
		$preid = " AND preid = $preid ";
	}else{
		$preid = " AND preid is null ";
	}
	
	$sql = "SELECT
				sbaid
			FROM
				fiesabatimento.solicitacaoabatimento 
			WHERE 
				sbastatus = 'A' AND idoid = $idoid 
				$preid
			";
	*/

	$sql = "SELECT
				sbaid
			FROM
				fiesabatimento.solicitacaoabatimento
			WHERE
				sbastatus = 'A'
				AND sbarenovacao = '$sbarenovacao'
				AND idoid = $idoid
			";

	return $db->pegaUm($sql);
}

//Função legado...
function pegaDocidSolicitacao($idoid, $preid = null){
	
	global $db;

	/*
	if($preid){
		$preid = " AND preid = $preid ";
	}else{
		$preid = " AND preid is null ";
	}

	$sql = "SELECT
				doc.docid
			FROM
				fiesabatimento.solicitacaoabatimento sab
			INNER JOIN workflow.documento doc ON doc.docid = sab.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
				sbastatus = 'A' AND idoid = $idoid
				$preid
			";
	*/
	
	$sql = "SELECT
				doc.docid
			FROM
				fiesabatimento.solicitacaoabatimento sab
			INNER JOIN workflow.documento doc ON doc.docid = sab.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
				sbastatus = 'A' AND idoid = $idoid
			";
	return $db->pegaUm($sql);
}



function pegaDocidSolicitacaoSbaid($sbaid){

	global $db;

	$sql = "SELECT
				doc.docid
			FROM
				fiesabatimento.solicitacaoabatimento sab
			INNER JOIN workflow.documento doc ON doc.docid = sab.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
				sbaid = $sbaid";
	
	return $db->pegaUm($sql);
}

function pegaUltimaTramitacao( $docid ){
	
	global $db;
	
	$sql = "SELECT
				max(hstid)
			FROM
				workflow.historicodocumento hst
			WHERE
				docid = $docid";
	return $db->pegaUm($sql);
}


//Função legado...
function insereHistoricoTramitacao($dados)
{
	global $db;
	
	$sql = "INSERT INTO 
				fiesabatimento.historicotramitacao (co_usuario,sbaid,sbatipo,atpid,htrcpf,htrdata,htrperfil,hrtmotivoreabertura,htrstatus,hstid)
			VALUES
				(NULL,{$dados['sbaid']},'S',{$dados['atpid']},'{$_SESSION['fiesabatimento_var']['cpfusuario']}',now(),'{$dados['htrperfil']}',{$dados['hrtmotivoreabertura']},'A',{$dados['hstid']})";
	$db->executar($sql);
	$db->commit();
}

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}

function pegaAtuacaoSolicitacao( $sbaid, $sbaanoinicio=2010, $sbaanofim=2012, $sbarenovacao='f', $sbames=12 ){
	
	global $db;
	
	$perfis = pegaPerfilGeral();

	/*
	$sql = "SELECT sbaanoinicio, sbaanofim, sbarenovacao FROM fiesabatimento.solicitacaoabatimento
 			WHERE sbaid = $sbaid";

	$dadossba = $db->pegaLinha( $sql );
	if($dadossba) extract($dadossba);
	*/

	$where = Array("sba.sbaid = $sbaid");
	
	$where[] = "atp.atpstatus = 'A'";
	if( in_array(PFL_SECRETARIO_MUNICIPAL,$perfis) //|| in_array(PFL_SUB_SECRETARIO_MUNICIPAL,$perfis) 
		){
		$param = Array('campo'=>'estuf','valor'=> 'DF','perfil'=>PFL_SECRETARIO_MUNICIPAL);
		if( !temResp( $param ) ){
			$where[] = "atp.esferaprofessor = 'M'";
		}
		$inner_resp = "INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.muncod = atp.muncodprofessor AND urs.usucpf = '".$_SESSION['usucpf']."'";
	}
	if( in_array(PFL_SECRETARIO_ESTADUAL,$perfis) //|| in_array(PFL_SUB_SECRETARIO_ESTADUAL,$perfis) 
		) {
		$param = Array('campo'=>'estuf','valor'=> 'DF','perfil'=>PFL_SECRETARIO_ESTADUAL);
		if( !temResp( $param ) ){
			$where[] = "atp.esferaprofessor in ('E','F')";
		}
		$inner_resp = "INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.estuf = atp.estufprofessor AND urs.usucpf = '".$_SESSION['usucpf']."'";
	}
	
	//elimina escolas
	/*
	if($preid){
		//$where[] = " ( (atp.atpdatainicio >= '2013-01-01' and atp.atpdatainicio <= '2013-12-31' ) or atp.atpdatafim is null ) ";
		//$where[] = " ( atp.atpdatainicio <= '2013-12-31'  and (atp.atpdatafim >= '2013-01-01' and atp.atpdatafim <= '2013-12-31' or atp.atpdatafim is null) ) ";
		//$where[] = " ( atp.atpdatainicio <= '2013-12-31'  and (atp.atpdatainicio <= '2013-12-31' or atp.atpdatafim is null) ) ";
		//$where[] = " ( (atp.atpdatafim >= '2013-01-01' and atp.atpdatafim <= '2013-12-01' ) or atp.atpdatafim is null ) ";
		//$where[] = " ( (atp.atpdatainicio BETWEEN '2013-01-01' AND '2013-12-31') OR (atp.atpdatafim BETWEEN '2013-01-01' AND '2013-12-31') OR atp.atpdatafim is null ) ";
		//$where[] = " ((atp.atpdatainicio <= '2013-12-31' and atp.atpdatafim is not null ) OR atp.atpdatafim is null) ";
		//$where[] = " ((atp.atpdatainicio <= '2013-12-31' and atp.atpdatafim >= '2013-01-01' ) OR atp.atpdatafim is null) ";
		$where[] = "  ( (atp.atpdatainicio <= '2013-12-31' and atp.atpdatafim >= '2013-01-01' ) OR (atp.atpdatainicio <= '2013-12-31' and atp.atpdatafim is null) ) ";
	}
	else{
		//$where[] = " ( atp.atpdatainicio <= '2012-12-31'  and (atp.atpdatainicio <= '2012-12-31' or atp.atpdatafim is null) ) ";
		//$where[] = " ( (atp.atpdatafim >= '2012-01-01' and atp.atpdatafim <= '2012-12-01' ) or atp.atpdatafim is null ) ";
		//$where[] = " ( (atp.atpdatainicio BETWEEN '2010-01-01' AND '2012-12-31') OR (atp.atpdatafim BETWEEN '2010-01-01' AND '2012-12-31') OR (atp.atpdatainicio <= '2012-12-31' and atp.atpdatafim is null) ) ";
		//$where[] = " ((atp.atpdatainicio <= '2012-12-31' and atp.atpdatafim is not null ) OR atp.atpdatafim is null) ";
		//$where[] = " ((atp.atpdatainicio <= '2012-12-31' and atp.atpdatafim >= '2010-01-01' ) OR atp.atpdatafim is null) ";
		$where[] = "  ( (atp.atpdatainicio <= '2014-12-31' and atp.atpdatafim >= '2010-01-01' ) OR (atp.atpdatainicio <= '2014-12-31' and atp.atpdatafim is null) ) ";
	}
	*/

	$where[] = "  ( (atp.atpdatainicio <= '{$sbaanofim}-12-31' and atp.atpdatafim >= '{$sbaanoinicio}-01-01' ) OR (atp.atpdatainicio <= '{$sbaanofim}-12-31' and atp.atpdatafim is null) ) ";

	$sql = "SELECT DISTINCT
				CASE 
					WHEN atp.esferaprofessor = 'M' THEN 'Municipal'
					WHEN atp.esferaprofessor = 'F' THEN 'Federal'
					ELSE 'Estadual'
				END as no_dep_adm,
				atp.atpinep as co_inep,
				atp.atpdescricaoescola as no_entidade,
				atp.atpvinculo as no_tipo_vinculo,
				atp.atpfuncao as no_funcao,
				atp.atpnumcargahoraria as nu_carga_horaria,
				atp.atpdatainicio::date as dt_inicio,
				CASE 
					WHEN atp.atpdatafim > NOW() THEN NULL
					ELSE atp.atpdatafim
				END as dt_fim,
				atp.atpidusuconfirmacao,
				COALESCE(atp.atpdatafim - atp.atpdatainicio,0) as difmeses,
				atp.atpid,
				atpprofefetivoexercicio,
				atpcompefetivoexercicio,
				atprespsecretario,
				doc.esdid 
			FROM
				fiesabatimento.solicitacaoabatimento sba
			INNER JOIN fiesabatimento.atuacaoprofissional atp ON atp.sbaid = sba.sbaid
			LEFT JOIN workflow.documento doc ON doc.docid = atp.docid
			$inner_resp
			WHERE
				".implode(' AND ', $where)."
			ORDER BY
				no_entidade";
	//dbg($sql,1);
	return $db->carregar($sql);
}

function verificaSolicitacaoAtiva( $idoid ){
	
	global $db;
	if( $idoid ){
		$sql = "SELECT
					true
				FROM
					fiesabatimento.solicitacaoabatimento sba
				INNER JOIN workflow.documento doc ON doc.docid = sba.docid 
				WHERE
					sbastatus = 'A'
					AND idoid = $idoid
					AND doc.esdid != ".WF_FIES1_REENVIO;
		$teste = $db->pegaUm($sql);
		return $teste == 't' ? true : false;
	}else{
		return false;
	}
}

function atualizaInformações( $idoid ){
	
	global $db;
	
	$dadosUsu = pegarInfoUsuario( $_SESSION['fiesabatimento_var']['cpfusuario'] );
	
	extract($dadosUsu);
	
	if($ds_logradouro) $ds_logradouro = str_replace("'","",$ds_logradouro);
	if($ds_complemento) $ds_complemento = str_replace("'","",$ds_complemento);
	if($ds_bairro) $ds_bairro = str_replace("'","",$ds_bairro);
	
	$sql = "UPDATE fiesabatimento.identificacaodocente SET
				idorg = '$nu_rg',
				idocep = '$nu_cep',
				idoendereco = '$ds_logradouro',
				idoenumero = '$ds_numero',
				idocomplemento = '$ds_complemento',
				idobairro = '$ds_bairro',
				idoeemail = '$ds_contato_eletronico',
				estufprofessor = '$sg_uf',
				muncodprofessor = '$co_municipio',
				idotetelefoneddd = '$nu_ddd',
				idotelefone = '$nu_telefone'
			WHERE
				idoid = $idoid";
	 $db->executar($sql);
	 $db->commit();
}

function pegarAtuacaoUsuario($cpf) {
	
	global $db;
	
	$conf['cpf'] = $cpf;
	$conf['servico'] = 'lerDadosAtuacao';
		
	$dadosAtu = wf_lerDados( $conf );
	
// 	$sql = "SELECT DISTINCT *
// 			FROM 
// 				dblink (
// 				'".PARAM_DBLINK_FREIRE."', '
// 					SELECT DISTINCT
// 						no_dep_adm, co_inep, no_fantasia, no_tipo_vinculo, no_funcao, at.nu_carga_horaria, 
// 						at.dt_inicio, 
// 						CASE WHEN to_char(at.dt_fim,\'YYYYMM\') >= to_char(now(),\'YYYYMMDD\') THEN NULL ELSE at.dt_fim END as dt_fim, 
// 						COALESCE(at.dt_fim - at.dt_inicio,0) as difmeses
// 					FROM 
// 						public.tb_sf_pessoa pe 
// 					INNER join public.tb_sf_pessoa_fisica 		  		pf ON pe.co_pessoa = pf.co_pessoa_fisica 
// 					INNER JOIN public.tb_sf_curriculo 		  			cu ON cu.co_curriculo = pf.co_pessoa_fisica 
// 					INNER JOIN public.tb_sf_atuacao 		  			at ON at.co_curriculo = cu.co_curriculo 
// 					LEFT  JOIN tb_sf_atuacao_eb                         eb ON eb.co_atuacao_eb = at.co_atuacao
// 					LEFT  JOIN tb_sf_atuacao_sup                        sup ON sup.co_atuacao_sup = at.co_atuacao 
// 					INNER JOIN public.tb_sf_pessoa_juridica 	  		pj ON pj.co_pessoa_juridica = at.co_pessoa_juridica
// 					INNER JOIN public.tb_sf_dependencia_administrativa 	da ON da.co_dep_adm = pj.co_dep_adm 
// 					INNER JOIN public.tb_sf_tipo_vinculo 		  		vi ON vi.co_tipo_vinculo = at.co_tipo_vinculo 
// 					INNER JOIN public.tb_sf_funcao 			  			fu ON fu.co_funcao = at.co_funcao
// 					WHERE 
// 						pj.co_tipo_pessoa_juridica = 4 --Escola
// 						AND 	coalesce (eb.co_nivel_escolar,sup.co_nivel_escolar) IN (1,2,3,18,19) --(Ensino fundamentla, Ensino Fundamentla, Ensino Médio, Médio - Magistério, Médio - Indigena)
// 						AND 	at.co_funcao = 2 --DOCENTE (PROFESSOR)
// 						AND 	pj.co_dep_adm IN (\'M\', \'E\', \'F\') --Municipal / Estadual / Federal (REDE PUBLICA)
// 						AND 	at.co_tipo_vinculo IN (1,2,5) --Servidor Publicao/ Selestina Formal/ Contrato Temporário
// 						AND 	pf.nu_cpf=\'".$cpf."\';'
// 				) as rs (
// 					no_dep_adm character varying(20),
// 					co_inep integer,
// 					no_fantasia character varying(100),
// 					no_tipo_vinculo character varying(50),
// 					no_funcao character varying(50),
// 					nu_carga_horaria smallint,
// 					dt_inicio date,
// 					dt_fim date,
// 					difmeses integer
// 				)
// 			--WHERE
// 				--(to_char(dt_fim,'YYYY')::integer > 2009 OR dt_fim is null)
// 				--OR (to_char(dt_inicio,'YYYY')::integer > 2009 AND dt_fim is not null)
// 			ORDER BY
// 				dt_inicio, dt_fim";
	
// 	$dados = $db->carregar($sql);
	
	return $dadosAtu;
	
}

function pegarInfoUsuario($cpf) {
	
	global $db;
// 	ver(PARAM_DBLINK_FREIRE,d);

	$conf['cpf'] = $cpf;
	$conf['servico'] = 'lerDadosPessoais';
	
	$dados = wf_lerDados( $conf );
	
// 	$sql = "select *
// 			from dblink (
// 			'".PARAM_DBLINK_FREIRE."',
// 			'select 
// 				pf.nu_cpf, coalesce(pf.nu_rg,\'0\') as nu_rg, coalesce(pf.nu_rg_complemento,\'0\') as nu_rg_complemento, pe.no_pessoa, 
// 				en.nu_cep, en.ds_logradouro, en.ds_numero, en.ds_complemento, en.ds_bairro, mu.no_municipio_acento, 
// 				es.no_uf, ce.ds_contato_eletronico, nu_ddd, nu_telefone, dt_nascimento, mu.co_municipio, es.sg_uf 
// 			from public.tb_sf_pessoa pe 
// 			inner join public.tb_sf_pessoa_fisica pf on pe.co_pessoa = pf.co_pessoa_fisica 
// 			left join public.tb_sf_telefone tl on tl.co_pessoa = pe.co_pessoa AND tl.st_ativo = TRUE
// 			left join public.tb_sf_contato_eletronico ce on ce.co_pessoa = pe.co_pessoa AND ce.st_ativo = TRUE 
// 			left join public.tb_sf_endereco en on en.co_pessoa = pe.co_pessoa AND en.st_ativo = TRUE  
// 			left join public.tb_sf_municipio mu on mu.co_municipio = en.co_municipio 
// 			left join public.tb_sf_uf es on es.co_uf = mu.co_uf
// 			where pf.nu_cpf=\'".$cpf."\'
// 			order by ce.dt_incl
// 			limit 1;'
// 			) as rs (
// 			nu_cpf character(11),
// 			nu_rg character varying(20),
// 			nu_rg_complemento character varying(4),
// 			no_pessoa character varying(100),
// 			nu_cep integer,
// 			ds_logradouro character varying(100),
// 			ds_numero character varying(10),
// 			ds_complemento character varying(150),
// 			ds_bairro character varying(100),
// 			no_municipio_acento character varying(200),
// 			no_uf character varying(100),
// 			ds_contato_eletronico character varying(150),
// 			nu_ddd character(2),
// 			nu_telefone character(8),
// 			dt_nascimento date,
// 			co_municipio character(7),
// 			sg_uf character(2)
// 			)";
// 	ver($sql);
	return $dados[0];
}

function htmlAtuacaoSolicitacao($sbaid, $boDatas = true, $boCancela = false, $sbaanoinicio=2010, $sbaanofim=2012, $sbarenovacao='f', $sbames=12) {

	$dadosAtu = pegaAtuacaoSolicitacao($sbaid, $sbaanoinicio, $sbaanofim, $sbarenovacao, $sbames);
	//dbg($dadosAtu);
	?>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<script>
		function cancelarAtuacao( atpid ){
			if( confirm( 'Deseja cancelar esta atuação profissional? \nCaso esta solicitação não alcance \nos critérios mínimos para aprovação ela será automaticamente rejeitada.' ) ){
				jQuery('#requisicao').val('cancelarAtuacao');
				jQuery('#atpid').val( atpid );
				jQuery('#formulario').submit();
			}
		}
		</script>
		<tr>
			<td class="SubTituloCentro" rowspan="2">Nº</td>
			<td class="SubTituloCentro" rowspan="2">Dependência Administrativa(Empregador)</td>
			<td class="SubTituloCentro" colspan="2">Escola</td>
			<td class="SubTituloCentro" colspan="3">Contrato de trabalho</td>
			<?php if( $boDatas ){?>
			<td class="SubTituloCentro" colspan="3">Periodo de exercício da docência</td>
			<?php }?>
			<?php if( $boCancela ){?>
			<td class="SubTituloCentro" rowspan="2">Cancelar<br>Atuação?</td>
			<?php }?>
		</tr>
		<tr>
			<td class="SubTituloCentro">Código INEP</td>
			<td class="SubTituloCentro">Nome</td>
			<td class="SubTituloCentro">Vínculo</td>
			<td class="SubTituloCentro">Função</td>
			<td class="SubTituloCentro">Carga horária semanal</td>
			<?php if( $boDatas ){?>
			<td class="SubTituloCentro">Data início</td>
			<td class="SubTituloCentro">Data fim</td>
			<td class="SubTituloCentro">Em efetivo exercício<br>(na data da solicitação)</td>
			<?php }?>
		</tr>
	<? if($dadosAtu[0]) : ?>
		<? foreach($dadosAtu as $num => $atuacao) : ?>
		<?php 
		
			if( $atuacao['dt_fim'] ){
				$diasMes = Array('',31,28,31,30,31,30,31,31,30,31,30,31);
				$atuacao['dt_fim'] = str_replace("/","-",$atuacao['dt_fim']);
				$tempData = explode('-', $atuacao['dt_fim']);
				$atuacao['dt_fim'] = $tempData[0].'-'.$tempData[1].'-'.$diasMes[(integer)$tempData[1]];
	// 			$atuacao['dt_fim'] = $tempData[0].'-'.$tempData[1].'-'.$tempData[2];
			}else{
				//$atuacao['atpprofefetivoexercicio'] = 't';
			}
		?>
		<tr>
			<td><?=($num+1) ?></td>
			<td><?=$atuacao['no_dep_adm'] ?></td>
			<td><?=$atuacao['co_inep'] ?></td>
			<td><?=$atuacao['no_entidade'] ?></td>
			<td><?=$atuacao['no_tipo_vinculo'] ?></td>
			<td><?=$atuacao['no_funcao'] ?></td>
			<td><?=$atuacao['nu_carga_horaria'] ?></td>
			<?php if( $boDatas ){?>
			<td><?=formata_data($atuacao['dt_inicio']) ?></td>
			<td><?=formata_data($atuacao['dt_fim']) ?></td>
			<td align="center">
				<?=( $atuacao['atpcompefetivoexercicio'] == 't' ? 'Sim' : ( $atuacao['atpprofefetivoexercicio'] == 't' ? 'Sim' :'Não') ) ?>
				<?php if( $atuacao['esdid'] != ESDID_AGUARDANDO_ANALISE ){?>
					<?if($atuacao['atpprofefetivoexercicio'] == 't' || $atuacao['atpprofefetivoexercicio'] == 'f'){?>
						<strong><?=( $atuacao['atpcompefetivoexercicio'] != $atuacao['atpprofefetivoexercicio'] ? ' *' :'') ?></strong>
					<?php }?>
				<?php }?>
			</td>
			<?php }?>
			<?php if( $boCancela ){?>
			<td align="center">
				<?php if( $atuacao['esdid'] == ESDID_AGUARDANDO_ANALISE ){?>
					<img border="0" align="top" src="../imagens/excluir_2.gif" style="cursor:pointer" 
						title="Cancelar Atuação Profissional" onclick="cancelarAtuacao( <?=$atuacao['atpid'] ?> )">
				<?php }elseif( $atuacao['esdid'] == ESDID_ANALISADO ){?>
					<img border="0" align="top" src="../imagens/check_checklist.png" style="cursor:pointer" 
						title="Aprovado pelo Secretário" >
				<?php }elseif( $atuacao['esdid'] == ESDID_REJEITADO ){?>
					<img border="0" align="top" src="../imagens/alerta_sistema.gif" style="cursor:pointer" width="30px"
						title="Rejeitado pelo Secretário" >
				<?php }?>
			</td>
			<?php }?>
		</tr>
		<? endforeach; ?>
		<?php if( $atuacao['esdid'] != ESDID_AGUARDANDO_ANALISE ){?>
		<tr>
			<td class="SubTituloCentro" colspan="11" style="text-align:right">* Alterado pelo secretário</td>
		</tr>
		<?php }?>
	<? endif; ?>
	</table>	
	<?
	return $dadosAtu;
}

function htmlAtuacaoUsuario($cpf, $sbaanoinicio=2010, $sbaanofim=2012, $sbarenovacao='f', $sbames=12) {

	$dadosAtu = pegarAtuacaoUsuario($cpf);

	//faz filtro no array
	foreach ($dadosAtu as $k => $v) {
		//dbg($v['dt_inicio']);
		unset($anoi);
		unset($anof);
		$v['dt_inicio'] = str_replace("/","-",$v['dt_inicio']);
		$tempDatai = explode('-', $v['dt_inicio']);
		$v['dt_fim'] = str_replace("/","-",$v['dt_fim']);
		$tempDataf = explode('-', $v['dt_fim']);
		//dbg($tempData[2]);
		if(strlen($tempDatai[2]) == 4) $anoi=$tempDatai[2];
		if(strlen($tempDatai[0]) == 4) $anoi=$tempDatai[0];
		if(strlen($tempDataf[2]) == 4) $anof=$tempDataf[2];
		if(strlen($tempDataf[0]) == 4) $anof=$tempDataf[0];

		if((int)$anoi > (int)$sbaanofim || ($anof < $sbaanoinicio && $anof)){
			unset($dadosAtu[$k]);
			$entrou = 1;
		}
	}

	if($entrou==1){
		$dadosAtu  = array_values($dadosAtu);
	}

	
	?>
	
	<script>
		
		function mudadtfim( id, tipo ){
			/*
			if(tipo=='S'){
				//jQuery('#dtfim_'+id).val('');
				jQuery('#divdtfim_'+id).hide();
			}else{
				jQuery('#divdtfim_'+id).show();
			}
			*/
		}
		
	</script>
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
	<tr>
		<td class="SubTituloCentro" rowspan="3">Nº</td>
		<td class="SubTituloCentro" rowspan="3">Dependência Administrativa(Empregador)</td>
		<td class="SubTituloCentro" colspan="2">Escola</td>
		<td class="SubTituloCentro" colspan="3">Contrato de trabalho</td>
		<td class="SubTituloCentro" colspan="4">Efetivo exercício da docência</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" rowspan="2">Código INEP</td>
		<td class="SubTituloCentro" rowspan="2">Nome</td>
		<td class="SubTituloCentro" rowspan="2">Vínculo</td>
		<td class="SubTituloCentro" rowspan="2">Função</td>
		<td class="SubTituloCentro" rowspan="2">Carga horária semanal</td>
		<td class="SubTituloCentro" rowspan="2">Data início</td>
		<td class="SubTituloCentro" rowspan="2">Data fim</td>
		<td class="SubTituloCentro" colspan="2">Em efetivo exercício<BR>(na data da solicitação)</td>
	</tr>
	<tr>
		<td class="SubTituloCentro">Sim</td>
		<td class="SubTituloCentro">Não</td>
	</tr>
	<? if($dadosAtu[0]) : ?>
	<? foreach($dadosAtu as $num => $atuacao) : ?>
	<?php   
			unset($dt_inicio);
			unset($dt_fim);
			unset($dt_fim_format);
			
			if( $atuacao['dt_inicio'] ){
				 $atuacao['dt_inicio'] = str_replace("/","-",$atuacao['dt_inicio']);
				 $dt_inicioArray = explode('-', $atuacao['dt_inicio']);
				 if(strlen($dt_inicioArray[2]) == 4) $dt_inicio=$dt_inicioArray[2].'-'.$dt_inicioArray[1].'-'.$dt_inicioArray[0];
	   			 if(strlen($dt_inicioArray[0]) == 4) $dt_inicio=$dt_inicioArray[0].'-'.$dt_inicioArray[1].'-'.$dt_inicioArray[2];
			}
			//dbg($atuacao['dt_fim']);
			
			if( $atuacao['dt_fim'] ){
				$diasMes = Array('',31,28,31,30,31,30,31,31,30,31,30,31);
				$atuacao['dt_fim'] = str_replace("/","-",$atuacao['dt_fim']);
				$tempData = explode('-', $atuacao['dt_fim']);
				if(strlen($tempData[2]) == 4) $dt_fim=$tempData[2].'-'.$tempData[1].'-'.$diasMes[(integer)$tempData[0]];
	   			if(strlen($tempData[0]) == 4) $dt_fim=$tempData[0].'-'.$tempData[1].'-'.$diasMes[(integer)$tempData[2]];
				//$atuacao['dt_fim'] = $tempData[0].'-'.$tempData[1].'-'.$diasMes[(integer)$tempData[1]];
				
	   			$dt_fim_format = substr($dt_fim,8,2).'/'.substr($dt_fim,5,2).'/'.substr($dt_fim,0,4);
			}
			//$atuacao['dt_fim'] = "2014-10-30";
			//dbg($atuacao['dt_fim']);
			//dbg($dt_fim);
			//dbg($dt_inicio);
	?>
	<tr>
		<td><?=($num+1) ?></td>
		<td><?=$atuacao['no_dep_adm'] ?></td>
		<td><?=$atuacao['co_inep'] ?></td>
		<td><?=$atuacao['no_entidade'] ?></td>
		<td><?=$atuacao['no_tipo_vinculo'] ?></td>
		<td><?=$atuacao['no_funcao'] ?></td>
		<td><?=$atuacao['nu_carga_horaria'] ?></td>
		<td>
			<?=formata_data($dt_inicio)?>
			<!-- 
			<input type="hidden" id="dtini_<?=$atuacao['co_inep']?>" name="dtini_<?=$atuacao['co_inep']?>" value="<?=formata_data($atuacao['dt_inicio'])?>">
 			-->
		</td>
		<td nowrap="nowrap">
			<?=$dt_fim_format?>
			<!--  
			<div id="divdtfim_<?=$atuacao['co_inep']?>" style="display: <?=(!$atuacao['dt_fim']?'none':'') ?>"><?=campo_data( 'dtfim_'.$atuacao['co_inep'], 'S', 'S', '', 'S', '', '', $atuacao['dt_fim'] )?></div>
			-->
		</td>
		<td><center><input type="radio" name="efetivo_exercicio[<?=$atuacao['co_inep'] ?>_<?=$dt_inicio ?>_<?=$dt_fim ?>]" value="true" <?=(!$atuacao['dt_fim']?'checked="checked"':'') ?> <?=($dt_fim_format?'disabled':'');?>  onclick="mudadtfim('<?=$atuacao['co_inep']?>','S');"/></center></td>
		<td><center><input type="radio" name="efetivo_exercicio[<?=$atuacao['co_inep'] ?>_<?=$dt_inicio ?>_<?=$dt_fim ?>]" value="false" <?=(!$atuacao['dt_fim']?'':'checked="checked"') ?> onclick="mudadtfim('<?=$atuacao['co_inep']?>','N');"/></center></center></td>
	</tr>
	<? endforeach; ?>
	<? endif; ?>
	</table>	
	<?php
	return $dadosAtu;
}

function pegarDadosUsuario($idoid) {
	
	global $db;
	
	$sql = "SELECT DISTINCT
				idocpf as nu_cpf,
				idorg as nu_rg,
				idonome as no_pessoa,
				idocep as nu_cep,
				idoendereco as ds_logradouro,
				idoenumero as ds_numero,
				idocomplemento as ds_complemento,
				idobairro as ds_bairro,
				mundescricao as no_municipio_acento,
				estdescricao as no_uf,
				idoeemail as ds_contato_eletronico,
				idotetelefoneddd as nu_ddd,
				idotelefone as nu_telefone,
				idodatanascimento as dt_nascimento
			FROM
				fiesabatimento.identificacaodocente ido
			LEFT JOIN territorios.municipio mun ON mun.muncod = ido.muncodprofessor
			LEFT JOIN territorios.estado est ON est.estuf = ido.estufprofessor
			WHERE
				idoid = $idoid
				AND idostatus = 'A'";
	$dados = $db->pegaLinha($sql);
	return $dados;
}

function pegaIdoid(){
	
	global $db;
	
	if( $_SESSION['fiesabatimento_var']['cpfusuario'] != '' ){
		$sql = "SELECT
					idoid
				FROM
					fiesabatimento.identificacaodocente
				WHERE
					idocpf = '".$_SESSION['fiesabatimento_var']['cpfusuario']."'
					AND idostatus = 'A'";
		
		$idoid = $db->pegaUm($sql);
	}
	return $idoid;
}

function htmlDadosUsuario($cpf) {
	
	$idoid = pegaIdoid();
	
	if( $idoid ){
		$dadosUsu = pegarDadosUsuario( $idoid );
	}else{
		$dadosUsu = pegarInfoUsuario($cpf);
	}
	?>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloDireita" width="25%">CPF :</td>
			<td><?=mascaraglobal($dadosUsu['nu_cpf'],"###.###.###-##") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%">RG :</td>
			<td><?=$dadosUsu['nu_rg'].$dadosUsu['nu_rg_complemento'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome :</td>
			<td><?=$dadosUsu['no_pessoa'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CEP :</td>
			<td><?=mascaraglobal($dadosUsu['nu_cep'],"#####-###") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Endereço :</td>
			<td><?=$dadosUsu['ds_logradouro'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número :</td>
			<td><?=$dadosUsu['ds_numero'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Complemento :</td>
			<td><?=$dadosUsu['ds_complemento'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Bairro :</td>
			<td><?=$dadosUsu['ds_bairro'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF :</td>
			<td><?=$dadosUsu['no_uf'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Munícipio :</td>
			<td><?=$dadosUsu['no_municipio_acento'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone :</td>
			<td>(<?=mascaraglobal($dadosUsu['nu_ddd'],"##") ?>) <?=mask($dadosUsu['nu_telefone'], '####-####') ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Email :</td>
			<td><?=$dadosUsu['ds_contato_eletronico'] ?></td>
		</tr>
	</table>
	<?php
}

function mask($val, $mask)
{
	$maskared = '';
	$k = 0;
	for($i = 0; $i<=strlen($mask)-1; $i++)
	{
		if($mask[$i] == '#')
		{
			if(isset($val[$k]))
				$maskared .= $val[$k++];
		}
		else
		{
			if(isset($mask[$i]))
				$maskared .= $mask[$i];
		}
	}
	return $maskared;
}

function diferencaMeses($dtIni, $dtFim){
	if( $dtIni != '' && $dtFim != '' ){
		$mesIni = explode('-',$dtIni);
		$mesFim = explode('-',$dtFim);
		$dif = (12-$mesIni[1]+1)+((2011-$mesIni[0])*12);
	}else{
		$dif = 0;
	}
	return $dif;
}

//Função legado...
function testaProfessor(){
	/*
	global $db;
	$sql = "SELECT *
			FROM dblink (
				'".PARAM_DBLINK_FREIRE."',
				'SELECT DISTINCT
					pf.co_pessoa_fisica,
					st_publico,
					vi.co_tipo_vinculo,
					at.dt_inicio, 
					at.dt_fim, 
					at.nu_carga_horaria,
					fu.co_funcao,
					COALESCE(at.dt_fim - at.dt_inicio,0) as difmeses,
					dt_nascimento 
				FROM 
					public.tb_sf_pessoa pe 
				LEFT join public.tb_sf_pessoa_fisica 		  pf ON pe.co_pessoa = pf.co_pessoa_fisica 
				LEFT JOIN public.tb_sf_curriculo 		  cu ON cu.co_curriculo = pf.co_pessoa_fisica 
				LEFT JOIN public.tb_sf_atuacao 			  at ON at.co_curriculo = cu.co_curriculo  
				LEFT JOIN public.tb_sf_pessoa_juridica 		  pj ON pj.co_pessoa_juridica = at.co_pessoa_juridica
				LEFT JOIN public.tb_sf_dependencia_administrativa da ON da.co_dep_adm = pj.co_dep_adm 
				LEFT JOIN public.tb_sf_tipo_vinculo 		  vi ON vi.co_tipo_vinculo = at.co_tipo_vinculo 
				LEFT JOIN public.tb_sf_funcao 			  fu ON fu.co_funcao = at.co_funcao
				WHERE 
					pf.nu_cpf=\'".$_SESSION['fiesabatimento_var']['cpfusuario']."\';
				'
			) as rs (
				co_pessoa_fisica integer,
				st_publico boolean,
				co_tipo_vinculo integer,
				dt_inicio date,
				dt_fim date,
				nu_carga_horaria integer,
				co_funcao integer,
				difmeses integer,
				dt_nascimento date
			)
			WHERE
				(to_char(dt_fim,'YYYY')::integer > 2009 OR dt_fim is null)
				OR (to_char(dt_inicio,'YYYY')::integer > 2009 AND dt_fim is not null)";
	$dados = $db->carregar($sql);*/
	$dados = pegarInfoUsuario($_SESSION['fiesabatimento_var']['cpfusuario']);
	$vinc = 0;
	if( is_array($dados) ){
		foreach( $dados as $dado ){
			if (is_array($dado)) {
				if( $dado['dt_fim'] ){
					$mesIni = explode('-',$dado['dt_inicio']);
					$mesFim = explode('-',$dado['dt_fim']);
					$dif = diferencaMeses($dado['dt_inicio'], $dado['dt_fim']);
					if( $vinc<$dif ){
						$vinc = $dif;
					}
				}
			}
		}
	}
	
	$erro = '';
	
	$dados['cpf']  = $_SESSION['fiesabatimento_var']['cpfusuario'];
	$dados['data'] = $dados[0]['dt_nascimento'];
	$_SESSION['fiesabatimento_var']['dt_nascimento'] = $dados[0]['dt_nascimento'];
	
	$teste['erro'] = $erro;
	$teste['meses'] = $carga['dif'];
	
	$teste = '';
	return $teste;
}

function testaRenovacao(){
	
	global $db;
	
	$sql = "SELECT
				to_char(sba.sbadatasolicitacao, 'DD-MM-YYYY')
			FROM
				fiesabatimento.solicitacaoabatimento sba
			INNER JOIN fiesabatimento.identificacaodocente ido ON ido.idoid = sba.idoid
			INNER JOIN workflow.documento doc ON doc.docid = sba.docid
			WHERE
				idocpf = ''
				AND sbastatus = 'A'
				AND to_char(sba.sbadatasolicitacao, 'YYYY')::integer < to_char(now(),'YYYY')::integer 
				AND doc.esdid = ".WF_FIES1_APROVADA;
	
	$teste = $db->pegaUm( $sql );
	
	return $teste;
}

function recuperaMesesFIES()
{
	global $db;
	
	$meses = Array(Array('codigo'=>1 ,'descricao'=>'Janeiro'),
				   Array('codigo'=>2 ,'descricao'=>'Fevereiro'),
				   Array('codigo'=>3 ,'descricao'=>'Março'),
				   Array('codigo'=>4 ,'descricao'=>'Abril'),
				   Array('codigo'=>5 ,'descricao'=>'Maio'),
				   Array('codigo'=>6 ,'descricao'=>'Junho'),
				   Array('codigo'=>7 ,'descricao'=>'Julho'),
				   Array('codigo'=>8 ,'descricao'=>'Agosto'),
				   Array('codigo'=>9 ,'descricao'=>'Setembro'),
				   Array('codigo'=>10,'descricao'=>'Outubro'),
				   Array('codigo'=>11,'descricao'=>'Novembro'),
				   Array('codigo'=>12,'descricao'=>'Dezembro'));
	
	return $meses;
	
}

function recupaAnosEscola($arrDados, $sbaanoinicio=2010, $sbaanofim=2012, $sbarenovacao='f', $sbames=12)
{
	//trata dt_inicio
	$arrDados['dt_inicio'] = str_replace("/","-",$arrDados['dt_inicio']);
	$ini = explode('-',$arrDados['dt_inicio']);
	if(strlen($ini[2]) == 4) $arrDados['dt_inicio']=$ini[2].'-'.$ini[1].'-'.$ini[0];
   	if(strlen($ini[0]) == 4) $arrDados['dt_inicio']=$ini[0].'-'.$ini[1].'-'.$ini[2];
	
   	//trata dt_fim
	$arrDados['dt_fim'] = str_replace("/","-",$arrDados['dt_fim']);
	$fim = explode('-',$arrDados['dt_fim']);
	if(strlen($fim[2]) == 4) $arrDados['dt_fim']=$fim[2].'-'.$fim[1].'-'.$fim[0];
   	if(strlen($fim[0]) == 4) $arrDados['dt_fim']=$fim[0].'-'.$fim[1].'-'.$fim[2];
	

	$arrDados['dt_inicio'] = str_replace("/","-",$arrDados['dt_inicio']);
	$dt = str_replace('-','',$arrDados['dt_inicio']);
	$predtini = (int) $sbaanoinicio.'0101';
	if($sbarenovacao == 'f') {
		if ($dt < $predtini) {
			$arrDados['dt_inicio'] = "{$sbaanoinicio}-01-01";
		}
	}else{
		if ($dt < $predtini) {
			$arrDados['dt_inicio'] = "{$sbaanoinicio}-01-01";
		}
	}

	$arrDados['dt_fim'] = str_replace("/","-",$arrDados['dt_fim']);
	$dt = str_replace('-','',$arrDados['dt_fim']);
	$predtfim = (int) $sbaanofim.'1231';
	if( $dt > $predtfim ){
		$arrDados['dt_fim'] = $sbaanofim.'-12-31';
	}
	if( $arrDados['dt_fim'] == '' ){
		$arrDados['dt_fim'] = $sbaanofim.'-12-31';
	}

	$dt_inicio = $arrDados['dt_inicio'];
	$arrI = explode("-",$dt_inicio);
	$anoIni = $arrI[0];
	$dt_fim = $arrDados['dt_fim'];
	$arrF = explode("-",$dt_fim);
	$anoFim = $arrF[0];
	$anoFim = !$anoFim ? date("Y") : $anoFim;
	for($x=$anoIni;$x<=$anoFim;$x++){
		$arrAnos[] = $x;
	}
	
	return $arrAnos;
}

function pegaPeriodosAtuacao( $sbaid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				atpinep as co_inep,
				atpnumcargahoraria as nu_carga_horaria,
				matano::integer as ano,
				matmes::integer as mes
			FROM
				fiesabatimento.solicitacaoabatimento sba
			INNER JOIN fiesabatimento.atuacaoprofissional atp ON atp.sbaid = sba.sbaid 
			INNER JOIN fiesabatimento.responsavelanoatuacao ano ON ano.atpid = atp.atpid
			INNER JOIN fiesabatimento.mesesatuacao mes ON mes.ranid = ano.ranid
			WHERE
				sba.sbaid = $sbaid
				AND atp.atpstatus = 'A'
			ORDER BY
				co_inep,
				ano,
				mes";
	$dados = $db->carregar($sql);

	$chave = 0;
	$temp = Array();
	$arrDatas = Array();
	if( is_array( $dados ) ){
		foreach( $dados as $k => $dado ){
			if( $arrDatas[$chave]['dt_inicio'] == '' ){
				$arrDatas[$chave]['co_inep'] 			= $dado['co_inep'];
				$arrDatas[$chave]['nu_carga_horaria'] 	= $dado['nu_carga_horaria'];
				$arrDatas[$chave]['dt_inicio'] 			= $dado['ano'].'-'.str_pad($dado['mes'], 2, "0", STR_PAD_LEFT).'-01';
			}
			if( $dado['mes'] != 12 ){
				if( $dado['mes']+1 != $dados[$k+1]['mes'] ){
					$arrDatas[$chave]['dt_fim']	= $dado['ano'].'-'.str_pad($dado['mes'], 2, "0", STR_PAD_LEFT).'-01';
					$chave++;
				}
			}else{
				if( $dados[$k+1]['mes'] != '1' ){
					$arrDatas[$chave]['dt_fim']	= $dado['ano'].'-'.str_pad($dado['mes'], 2, "0", STR_PAD_LEFT).'-01';
					$chave++;
				}else{
					if( $dado['ano']+1 != $dados[$k+1]['ano'] ){
						$arrDatas[$chave]['dt_fim']	= $dado['ano'].'-'.str_pad($dado['mes'], 2, "0", STR_PAD_LEFT).'-01';
						$chave++;
					}
				}
			}
		}
	}
	
	return $arrDatas;
}

function testaMesesHoras( $sbaid, $sbaanoinicio=2010, $sbaanofim=2012, $sbames=12 ){
	
	global $db;
	
	$arrDatas = pegaPeriodosAtuacao( $sbaid );
	
	$teste = calculaMeses( $arrDatas, $sbaanoinicio, $sbaanofim, $sbames );
	$meses = $teste['meses'];

	$sbarenovacao = $db->pegaUm("SELECT sbarenovacao FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = {$sbaid}");


	if($sbarenovacao == 'f'){
		return $meses > 11;
	}else{
		return $meses > 0;
	}

	//return $meses > 11;
}

function testaTodasAtuacoesConfirmadas( $sbaid ){
	
	global $db;

	$sql = "SELECT
				true
			FROM
				fiesabatimento.atuacaoprofissional
			WHERE
				sbaid = $sbaid
				AND atpidusuconfirmacao is null
				AND atpstatus = 'A'";
	$testa = $db->pegaUm($sql);
	
	return $testa == '' ? true : false;
}

function contaEntidades( $sbaid ){
	
	global $db;
	
	$sql = "SELECT
				count(atpinep)
			FROM 	
				fiesabatimento.atuacaoprofissional
			WHERE
				sbaid = $sbaid
				AND atpstatus = 'A'";
	$conta = $db->pegaUm($sql);
	return $conta > 0 ? $conta : 0;
}

function confirmarSolicitacaoDiretor()
{
	global $db;

	extract($_POST);
	$idoid = $_GET['idoid'];
	
	$docid = pegaDocidSolicitacaoSbaid($sbaid);

	$sbames = 12;
	
	$dados['sbaid'] = $sbaid;
	$dados['sbaanoinicio'] = $sbaanoinicio;
	$dados['sbaanofim'] = $sbaanofim;
	$dados['sbarenovacao'] = $sbames;
	$dados['sbames'] = $sbarenovacao;
	$dados['hstid']	= pegaUltimaTramitacao($docid);
	$dados['atpid'] = pegaAtpidSolicitacao($sbaid);
	$dados['htrperfil'] = pegaPerfilGeral();

	//pega periodo referencia
	/*
	$dadossba = $db->pegaLinha("SELECT pr.premes, pr.preano, pr.predescricao FROM fiesabatimento.solicitacaoabatimento sb
  						  INNER JOIN fiesabatimento.periodoreferencia pr ON pr.preid = sb.preid
						  WHERE sb.sbaid = {$sbaid} AND sb.sbastatus = 'A'");

	if($dadossba){
		$premes = 12;
		$preano = $dadossba['preano'];
		$predescricao = $dadossba['predescricao'];
	}
	*/
	//pega id renovação
	/*
	$preid = $db->pegaUm("SELECT preid FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = ".$sbaid);
	
	if(!$preid){
		$textoAlert = 'Solicitação de abatimento analisada';
	}else{
		$textoAlert = 'Renovação de abatimento analisada';
	}
	*/

	//$textoAlert = "Solicitação de abatimento analisada - {$predescricao}";

	if($sbarenovacao == 't'){
		$textoAlert = "Renovação de abatimento analisada - {$sbaanoinicio} a {$sbaanofim}";
	}else{
		$textoAlert = "Solicitação de abatimento analisada - {$sbaanoinicio} a {$sbaanofim}";
	}


	//atualiza para não ter duplicações
	if( is_array($atuacoes_avaliadas) ){
		$sql = "UPDATE fiesabatimento.atuacaoprofissional SET atpstatus = 'I'
					WHERE sbaid = ".$dados['sbaid']."
					AND atpid not in (".implode(',',$atuacoes_avaliadas).")";
		$db->executar($sql);
		$db->commit();
	}


	if( $boAprovacao == 'S' ){
		
		executarAnaliseAutomatica( $dados['sbaid'] );
		
	}elseif( $boAprovacao == 'N2' ){
		
		$qtd_meses = 0;
		$anoX = 0;
		$mesX = 0;
		
		$sql = "SELECT
					ranid
				FROM
					fiesabatimento.responsavelanoatuacao ran
				INNER JOIN fiesabatimento.atuacaoprofissional atp ON atp.atpid= ran.atpid
				WHERE
					atp.sbaid = ".$dados['sbaid']."
					AND ranresponsaveltipo = 'D'
					AND rancpfresponsavel = '{$_SESSION['usucpf']}'
					AND ranstatus = 'A'";
		
		$ranids = $db->carregarColuna($sql);
		
		if( is_array($ranids) ){
			foreach( $ranids as $id ){
				$sql = "DELETE FROM fiesabatimento.mesesatuacao WHERE ranid = $id";
				$db->executar($sql);
			}
			$db->commit();
		}
		
		if($rdn_confirmar_mes){
			foreach($rdn_confirmar_mes as $cod_inep => $arrAnos){
				$sql = "SELECT DISTINCT
							atp.atpid
						FROM
							fiesabatimento.solicitacaoabatimento sba
						INNER JOIN fiesabatimento.atuacaoprofissional atp ON atp.sbaid = sba.sbaid
						WHERE
							sba.idoid = $idoid
							AND sba.sbaid = $sbaid
							AND atp.atpstatus = 'A'
							AND sba.sbastatus = 'A'
							AND atp.atpinep = '$cod_inep'";
				$atpid = $db->pegaUm($sql);
				//dbg($atpid,1);
			
				if($arrAnos){
					foreach($arrAnos as $ano => $arrMeses){
						$anoX = $anoX == 0 ? $ano : $anoX;
						$sql = "SELECT
									ranid
								FROM
									fiesabatimento.responsavelanoatuacao
								WHERE
									atpid = $atpid
									AND rananotuacao = $ano
									AND ranresponsaveltipo = 'D'
									AND rancpfresponsavel = '{$_SESSION['usucpf']}'
									AND ranstatus = 'A'";
						$ranid = $db->pegaUm($sql);
						if(!$ranid){
							$sql = "INSERT INTO fiesabatimento.responsavelanoatuacao  (atpid,co_usuario,rananotuacao,ranresponsaveltipo,rancpfresponsavel,ranstatus)
									VALUES ($atpid,NULL,$ano,'D','{$_SESSION['usucpf']}','A')
									RETURNING ranid";
							$ranid = $db->pegaUm($sql);
						}
						$sql = "DELETE FROM fiesabatimento.mesesatuacao WHERE ranid = $ranid";
						$db->executar($sql);
						if($arrMeses){
							foreach($arrMeses as $mes => $val){
								$mesX = $mesX == 0 ? $mes : $mesX;
								$qtd_meses ++;
								$sqlM .= "INSERT INTO fiesabatimento.mesesatuacao (ranid,sbaid,atpid,matano,matmes,matstatus)
										VALUES ($ranid,{$dados['sbaid']},$atpid,'$ano','$mes','A');";
							}
						}
					}
				}
			}
			if($sqlM){
				$db->executar($sqlM);
				$db->commit();
			}
		}
		
		if( is_array($atuacoes_avaliadas) ){
			foreach( $atuacoes_avaliadas as $atpid ){
				
				$sql = "UPDATE fiesabatimento.atuacaoprofissional SET
							atpcompefetivoexercicio = {$atpcompefetivoexercicio[$atpid]},
							atprespsecretario = 'N2'
						WHERE
							atpid = $atpid";
				
				$db->executar($sql);

				
				enviaEmailAprovacaoParcial( $atpid );
				$docidAt = pegaDocidAtuacao($atpid);
				wf_alterarEstado( $docidAt, AEDID_FINALIZAR_ANALISE, 'Aprovado em lote.', array('docid'=>$docidAt ) );
			}

		}
		
		$sqlAtu = "UPDATE fiesabatimento.atuacaoprofissional 
					SET atpidusuconfirmacao = '".$_SESSION['usucpf']."', atpdataconfirmacao = now() 
					WHERE atpid in (".implode(',',$atuacoes_avaliadas).")";
		
		$db->executar($sqlAtu);

		$db->commit();
		
	}elseif( $boAprovacao == 'N3' ){
		
		rejeitarAtuacao( $dados['sbaid'] );
		
		/*
		if(!$preid){
			$textoAlert = 'Solicitação de abatimento rejeitada';
		}else{
			$textoAlert = 'Renovação de abatimento rejeitada';
		}
		*/
		if($sbarenovacao == 't'){
			$textoAlert = "Renovação de abatimento rejeitada - {$sbaanoinicio} a {$sbaanofim}";
		}else{
			$textoAlert = "Solicitação de abatimento rejeitada - {$sbaanoinicio} a {$sbaanofim}";
		}

	}

	
	$dados['hrtmotivoreabertura'] = $dados['hrtmotivoreabertura'] ? "'".$dados['hrtmotivoreabertura']."'" : "''";

	if( testaTodasAtuacoesConfirmadas( $dados['sbaid'] ) ){

		atualizaMesesSolicitacao( $dados['sbaid'] );

		if( testaMesesHoras($sbaid, $sbaanoinicio, $sbaanofim, $sbames) ){

			/*
			if(!$preid){
				$comentario = 'Solicitação confirmada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
			}else{
				$comentario = 'Renovação confirmada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
			}
			*/
			//$comentario = 'Solicitação confirmada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
			if($sbarenovacao == 't'){
				$comentario = "Renovação {$sbaanoinicio} a {$sbaanofim} confirmada em ".date('d/m/Y H:i:s')." por ".$_SESSION['usucpf'];
			}else{
				$comentario = "Solicitação {$sbaanoinicio} a {$sbaanofim} confirmada em ".date('d/m/Y H:i:s')." por ".$_SESSION['usucpf'];
			}
			
			
			$test = wf_alterarEstado( $docid, WF_FIES1_APROVAR_SOLICITACAO, $comentario, array('docid'=>$docid ) );
			
			insereHistoricoTramitacao($dados);
			
			enviaEmailAprovacao($dados['sbaid']);
			
			echo "<script>
					alert('$textoAlert');
					window.location.href = 'fiesabatimento.php?modulo=principal/listasolicitacaoabatimento&acao=A';
				</script>";
			die();
			
		}else{

			/*
			if(!$preid){
				$comentario = 'Solicitação rejeitada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
			}else{
				$comentario = 'Renovação rejeitada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
			}
			*/
			//$comentario = 'Solicitação rejeitada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
			if($sbarenovacao == 't'){
				$comentario = "Renovação {$sbaanoinicio} a {$sbaanofim} rejeitada em ".date('d/m/Y H:i:s')." por ".$_SESSION['usucpf'];
			}else{
				$comentario = "Solicitação {$sbaanoinicio} a {$sbaanofim} rejeitada em ".date('d/m/Y H:i:s')." por ".$_SESSION['usucpf'];
			}

			
			
			$test = wf_alterarEstado( $docid, WF_FIES1_REJEITAR_ABATIMENTO, $comentario, array('docid'=>$docid ) );
			
			insereHistoricoTramitacao($dados);
			
			//$sql = "UPDATE fiesabatimento.atuacaoprofissional SET atpstatus = 'I' WHERE sbaid = ".$dados['sbaid'].";";
			//$db->executar($sql);
			
			$sql = "UPDATE fiesabatimento.solicitacaoabatimento SET sbastatus = 'I' WHERE sbaid = ".$dados['sbaid'].";";
			$db->executar($sql);
			$db->commit();

			enviaEmailRejeicao( $dados['sbaid'] );
			
			echo "<script>
					alert('$textoAlert');
					window.location.href = 'fiesabatimento.php?modulo=principal/listasolicitacaoabatimento&acao=A';
				  </script>";
			die();
		}
	}
	echo "<script>
			alert('$textoAlert');
			window.location.href = 'fiesabatimento.php?modulo=principal/listasolicitacaoabatimento&acao=A';
		</script>";
	die();
	
}

function listaAtuacoesAtivas( $sbaid ){
	
	global $db;
	
	//pega id renovação
	/*
	$preid = $db->pegaUm("SELECT preid FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = ".$sbaid);
	if($preid){
		//$and = " and ( (atp.atpdatainicio >= '2013-01-01' and atp.atpdatainicio <= '2013-12-31' ) or atp.atpdatafim is null ) ";
		$and = " and ( atp.atpdatainicio <= '2013-12-31'  and (atp.atpdatainicio <= '2013-12-31' or atp.atpdatafim is null) ) ";
		$DT_INICIO_PROGRAMA = '2013-01-01';
		$DT_FIM_PROGRAMA = '2013-12-31';
	}
	else{
		//$and = " and ( atp.atpdatainicio <= '2012-12-31'  and (atp.atpdatainicio <= '2012-12-31' or atp.atpdatafim is null) ) ";
		$and = " and ( atp.atpdatainicio <= '2014-12-31'  and (atp.atpdatainicio <= '2014-12-31' or atp.atpdatafim is null) ) ";
		$DT_INICIO_PROGRAMA = '2010-01-01';
		$DT_FIM_PROGRAMA = '2014-12-31';
	}
	*/

	$dadossbaid = $db->pegaLinha("SELECT sbaanoinicio, sbaanofim, sbarenovacao FROM fiesabatimento.solicitacaoabatimento
								  WHERE sbaid = {$sbaid}");
	if($dadossbaid) extract($dadossbaid);

	$and = " and ( atp.atpdatainicio <= '{$sbaanofim}-12-31'  and (atp.atpdatainicio <= '{$sbaanofim}-12-31' or atp.atpdatafim is null) and (atp.atpdatafim >= '{$sbaanoinicio}-01-01' or atp.atpdatafim is null) ) ";
	$DT_INICIO_PROGRAMA = "{$sbaanoinicio}-01-01";
	$DT_FIM_PROGRAMA = "{$sbaanofim}-12-31";

	$sql = "SELECT DISTINCT
				atp.sbaid,
				atp.atpid, 
				atpdescricaoescola as escola, 
				atpnumcargahoraria as carga_horaria, 
				to_char(atpdatainicio,'DD/MM/YYYY') as data_inicio, 
				to_char(atpdatafim,'DD/MM/YYYY') as data_fim, 
				/*
				(
					(
						CASE WHEN (date_part('year',
													CASE WHEN date_part('year',(to_char(atpdatafim,'YYYY-MM-DD'))::date)::integer < 2013
														THEN (to_char(atpdatafim,'YYYY-MM-DD'))::date
														ELSE '".$DT_FIM_PROGRAMA."'
													END)-
									date_part('year',
													CASE WHEN date_part('year',(to_char(atpdatainicio,'YYYY-MM-DD'))::date)::integer > 2009
														THEN (to_char(atpdatainicio,'YYYY-MM-DD'))::date
														ELSE '".$DT_INICIO_PROGRAMA."'
													END)
									) > 0
							THEN (date_part('year',
									CASE WHEN date_part('year',(to_char(atpdatafim,'YYYY-MM-DD'))::date)::integer < 2013
										THEN (to_char(atpdatafim,'YYYY-MM-DD'))::date
										ELSE '".$DT_FIM_PROGRAMA."'
									END)-
								 date_part('year',
									CASE WHEN date_part('year',(to_char(atpdatainicio,'YYYY-MM-DD'))::date)::integer > 2009
										THEN (to_char(atpdatainicio,'YYYY-MM-DD'))::date
										ELSE '".$DT_INICIO_PROGRAMA."'
									END)
								)-1
							ELSE (date_part('year',
									CASE WHEN date_part('year',(to_char(atpdatafim,'YYYY-MM-DD'))::date)::integer < 2013
										THEN (to_char(atpdatafim,'YYYY-MM-DD'))::date
										ELSE '".$DT_FIM_PROGRAMA."'
									END)-
								 date_part('year',
									CASE WHEN date_part('year',(to_char(atpdatainicio,'YYYY-MM-DD'))::date)::integer > 2009
										THEN (to_char(atpdatainicio,'YYYY-MM-DD'))::date
										ELSE '".$DT_INICIO_PROGRAMA."'
									END)
								)
						END
					)*12
				)
				+
				date_part('month',CASE WHEN date_part('year',(to_char(atpdatafim,'YYYY-MM-DD'))::date)::integer < 2013
							THEN (to_char(atpdatafim,'YYYY-MM')||'-27')::date
							ELSE '".$DT_FIM_PROGRAMA."'
						  END)
				+
				(
					12-
					date_part('month',CASE WHEN date_part('year',(to_char(atpdatainicio,'YYYY-MM-DD'))::date)::integer > 2009
								THEN (to_char(atpdatainicio,'YYYY-MM-DD'))::date
								ELSE '".$DT_INICIO_PROGRAMA."'
							END)
					+1
				) as qtd_solicitado,
				*/
				/*
				case WHEN sba.sbarenovacao = false then
					CASE WHEN atp.atpdatainicio < '".$DT_INICIO_PROGRAMA."' and (atp.atpdatafim > '".$DT_FIM_PROGRAMA."' or atp.atpdatafim is null) THEN COALESCE('".$DT_FIM_PROGRAMA."'::date - '".$DT_INICIO_PROGRAMA."'::date,0)/30
						 WHEN (atp.atpdatainicio >= '".$DT_INICIO_PROGRAMA."' and atp.atpdatainicio <= '".$DT_FIM_PROGRAMA."') and (atp.atpdatafim > '".$DT_FIM_PROGRAMA."' or atp.atpdatafim is null) THEN COALESCE('".$DT_FIM_PROGRAMA."'::date - atp.atpdatainicio::date,0)/30
						 WHEN (atp.atpdatainicio >= '".$DT_INICIO_PROGRAMA."' and atp.atpdatainicio <= '".$DT_FIM_PROGRAMA."') and (atp.atpdatafim <= '".$DT_FIM_PROGRAMA."') THEN COALESCE(atp.atpdatafim::date - atp.atpdatainicio::date,0)/30
						 WHEN atp.atpdatainicio < '".$DT_INICIO_PROGRAMA."' and (atp.atpdatafim >= '".$DT_INICIO_PROGRAMA."' and atp.atpdatafim <= '".$DT_FIM_PROGRAMA."') THEN
						case when mod(atp.atpdatafim::date - '".$DT_INICIO_PROGRAMA."'::date,30) >= 20 then
							(COALESCE(atp.atpdatafim::date - '".$DT_INICIO_PROGRAMA."'::date,0)/30)+1
							 else
							COALESCE(atp.atpdatafim::date - '".$DT_INICIO_PROGRAMA."'::date,0)/30
							 end
						 WHEN atp.atpdatainicio < '".$DT_INICIO_PROGRAMA."' and atp.atpdatafim < '".$DT_INICIO_PROGRAMA."' THEN 0
						 WHEN atp.atpdatainicio > '".$DT_FIM_PROGRAMA."' and (atp.atpdatafim > '".$DT_FIM_PROGRAMA."' or atp.atpdatafim is null) THEN 0
					END
                else
                	sba.sbaqmtsolicitado
			    END as qtd_solicitado,
				*/

				CASE WHEN atp.atpdatainicio < '".$DT_INICIO_PROGRAMA."' and (atp.atpdatafim > '".$DT_FIM_PROGRAMA."' or atp.atpdatafim is null) THEN COALESCE('".$DT_FIM_PROGRAMA."'::date - '".$DT_INICIO_PROGRAMA."'::date,0)/30
						 WHEN (atp.atpdatainicio >= '".$DT_INICIO_PROGRAMA."' and atp.atpdatainicio <= '".$DT_FIM_PROGRAMA."') and (atp.atpdatafim > '".$DT_FIM_PROGRAMA."' or atp.atpdatafim is null) THEN COALESCE('".$DT_FIM_PROGRAMA."'::date - atp.atpdatainicio::date,0)/30
						 WHEN (atp.atpdatainicio >= '".$DT_INICIO_PROGRAMA."' and atp.atpdatainicio <= '".$DT_FIM_PROGRAMA."') and (atp.atpdatafim <= '".$DT_FIM_PROGRAMA."') THEN COALESCE(atp.atpdatafim::date - atp.atpdatainicio::date,0)/30
						 WHEN atp.atpdatainicio < '".$DT_INICIO_PROGRAMA."' and (atp.atpdatafim >= '".$DT_INICIO_PROGRAMA."' and atp.atpdatafim <= '".$DT_FIM_PROGRAMA."') THEN
						case when mod(atp.atpdatafim::date - '".$DT_INICIO_PROGRAMA."'::date,30) >= 20 then
							(COALESCE(atp.atpdatafim::date - '".$DT_INICIO_PROGRAMA."'::date,0)/30)+1
							 else
							COALESCE(atp.atpdatafim::date - '".$DT_INICIO_PROGRAMA."'::date,0)/30
							 end
						 WHEN atp.atpdatainicio < '".$DT_INICIO_PROGRAMA."' and atp.atpdatafim < '".$DT_INICIO_PROGRAMA."' THEN 0
						 WHEN atp.atpdatainicio > '".$DT_FIM_PROGRAMA."' and (atp.atpdatafim > '".$DT_FIM_PROGRAMA."' or atp.atpdatafim is null) THEN 0
				END as qtd_solicitado,

				--date_part('year', age(atpdatafim,atpdatainicio))*12+date_part('month', age(atpdatafim,atpdatainicio)) as qtd_solicitado,
				--sba.sbaqmtsolicitado as qtd_solicitado,

				estufprofessor as uf, 
				mundescricao as municipio, 
				doc.docid,
				count(DISTINCT matano||matmes) as qtd_aprovado,
				esd.esddsc as estado,
				cmd.cmddsc as comentario
			FROM 
				fiesabatimento.atuacaoprofissional atp
			INNER JOIN fiesabatimento.solicitacaoabatimento			sba ON sba.sbaid = atp.sbaid
			INNER JOIN workflow.documento			doc ON doc.docid = atp.docid
			INNER JOIN workflow.estadodocumento 		esd ON esd.esdid = doc.esdid
			LEFT  JOIN (SELECT
							max(h.hstid) as hstid,
							h.docid
						FROM
							workflow.historicodocumento h
						INNER JOIN workflow.documento d ON d.docid = h.docid AND tpdid = ".TPDID_ANALISE_SITUACAO." 
						GROUP BY
							h.docid) as mst ON mst.docid = atp.docid
			LEFT  JOIN workflow.historicodocumento 		hst ON hst.hstid = mst.hstid
			LEFT  JOIN workflow.comentariodocumento 	cmd ON cmd.hstid = hst.hstid
			INNER JOIN territorios.municipio 		mun ON mun.muncod = atp.muncodprofessor
			LEFT  JOIN fiesabatimento.responsavelanoatuacao res ON res.atpid = atp.atpid
			LEFT  JOIN fiesabatimento.mesesatuacao		ran ON ran.ranid = res.ranid AND ran.sbaid = atp.sbaid
			WHERE
				atp.sbaid = $sbaid
				AND atp.atpstatus = 'A'
				$and
			GROUP BY
				atp.atpid, 
				atpdescricaoescola, 
				atpnumcargahoraria, 
				atpdatainicio, 
				atpdatafim, 
				estufprofessor, 
				mundescricao, 
				doc.docid,
				atp.sbaid,
				esd.esddsc,
				cmd.cmddsc,
				sba.sbaqmtsolicitado,
				sba.sbarenovacao
			ORDER BY
				1, 2";
	//dbg($sql);
	$atuacoes = $db->carregar($sql);
	
?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#DCDCDC">
			<td width="5%"><b>Detalhar</b></td>
			<td width="20%"><b>Escola</b></td>
			<td width="10%"><b>Municipio/UF</b></td>
			<td width="15%"><b>Situação Atual</b></td>
			<td width="10%"><b>Carga horária</b></td>
			<td width="10%"><b>Data de início</b></td>
			<td width="10%"><b>Data de Termino</b></td>
			<td width="10%"><b>Quantidade de meses solicitados</b></td>
			<td width="10%"><b>Quantidade de meses aprovados</b></td>
		</tr>
<?php 
	if( is_array($atuacoes) ){
		foreach( $atuacoes as $atuacao ){
?>
		<tr>
			<td align="center">
				<img border="0" title="Indica campo obrigatório." src="../imagens/consultar.gif" 
					 class="historico" id="<?=$atuacao['docid'] ?>" style="cursor:pointer">
			</td>
			<td><?=$atuacao['escola'] ?></td>
			<td><?=$atuacao['municipio'] ?>/<?=$atuacao['uf'] ?></td>
			<td><?=$atuacao['estado'] ?></td>
			<td><?=$atuacao['carga_horaria'] ?></td>
			<td><?=$atuacao['data_inicio'] ?></td>
			<td><?=$atuacao['data_fim'] ?></td>
			<td><?=$atuacao['qtd_solicitado'] ?></td>
			<td><?=$atuacao['qtd_aprovado'] ?></td>
		</tr>
<?php 
		}
?>
	</table>
<?php 
	}
}

function listaAtuacoesCanceladas( $sbaid ){
	
	global $db;
	
	//pega id renovação
	/*
	$preid = $db->pegaUm("SELECT preid FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = ".$sbaid);
	if($preid){
		$and = " and ( atp.atpdatainicio <= '2013-12-31'  and (atp.atpdatainicio <= '2013-12-31' or atp.atpdatafim is null) ) ";
		$DT_INICIO_PROGRAMA = '2013-01-01';
		$DT_FIM_PROGRAMA = '2013-12-31';
	}
	else{
		$and = " and ( atp.atpdatainicio <= '2014-12-31'  and (atp.atpdatainicio <= '2014-12-31' or atp.atpdatafim is null) ) ";
		$DT_INICIO_PROGRAMA = '2010-01-01';
		$DT_FIM_PROGRAMA = '2014-12-31';
	}
	*/

	$dadossbaid = $db->pegaLinha("SELECT sbaanoinicio, sbaanofim, sbarenovacao FROM fiesabatimento.solicitacaoabatimento
								  WHERE sbaid = {$sbaid}");
	if($dadossbaid) extract($dadossbaid);

	$and = " and ( atp.atpdatainicio <= '{$sbaanofim}-12-31'  and (atp.atpdatainicio <= '{$sbaanofim}-12-31' or atp.atpdatafim is null) ) ";
	$DT_INICIO_PROGRAMA = "{$sbaanoinicio}-01-01";
	$DT_FIM_PROGRAMA = "{$sbaanofim}-12-31";


	$sql = "SELECT DISTINCT
				atp.sbaid,
				atp.atpid, 
				atpdescricaoescola as escola, 
				atpnumcargahoraria as carga_horaria, 
				to_char(atpdatainicio,'DD/MM/YYYY') as data_inicio, 
				to_char(atpdatafim,'DD/MM/YYYY') as data_fim, 
				date_part('year', age(atpdatafim,atpdatainicio))*12+date_part('month', age(atpdatafim,atpdatainicio)) as qtd_solicitado,
				estufprofessor as uf, 
				mundescricao as municipio, 
				doc.docid,
				count(DISTINCT matano||matmes) as qtd_aprovado,
				esd.esddsc as estado --, cmd.cmddsc as comentario
			FROM 
				fiesabatimento.atuacaoprofissional atp
			INNER JOIN workflow.documento			doc ON doc.docid = atp.docid
			INNER JOIN workflow.estadodocumento 		esd ON esd.esdid = doc.esdid
			/*
			LEFT  JOIN (SELECT
						max(hstid) as hstid,
						docid
					FROM
						workflow.historicodocumento
					GROUP BY
						docid) as mst ON mst.docid = atp.docid
			LEFT  JOIN workflow.historicodocumento 		hst ON hst.hstid = mst.hstid
			LEFT  JOIN workflow.comentariodocumento 	cmd ON cmd.hstid = hst.hstid
			*/
			INNER JOIN territorios.municipio 		mun ON mun.muncod = atp.muncodprofessor
			LEFT  JOIN fiesabatimento.responsavelanoatuacao res ON res.atpid = atp.atpid
			LEFT  JOIN fiesabatimento.mesesatuacao		ran ON ran.ranid = res.ranid AND ran.sbaid = atp.sbaid
			WHERE
				atp.sbaid = $sbaid
				AND atp.atpstatus = 'I'
				AND doc.esdid = ".ESDID_CANCELADO_PELO_PROFESSOR."
				$and
			GROUP BY
				atp.atpid, 
				atpdescricaoescola, 
				atpnumcargahoraria, 
				atpdatainicio, 
				atpdatafim, 
				estufprofessor, 
				mundescricao, 
				doc.docid,
				atp.sbaid,
				esd.esddsc --, cmd.cmddsc
			ORDER BY
				1, 2";
	//dbg($sql,1);
	$atuacoes = $db->carregar($sql);
	
	if( is_array($atuacoes) ){
?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#DCDCDC">
			<td style="color:red" colspan="9"><b>Atuações Profissionais Canceladas</b></td>
		</tr>
<?php 
		if( is_array($atuacoes) ){
			foreach( $atuacoes as $atuacao ){
?>
		<tr>
			<td align="center" width="5%">
				<img border="0" title="Indica campo obrigatório." src="../imagens/consultar.gif" 
					 class="historico" id="<?=$atuacao['docid'] ?>" style="cursor:pointer">
			</td>
			<td width="20%"><?=$atuacao['escola'] ?></td>
			<td width="10%"><?=$atuacao['municipio'] ?>/<?=$atuacao['uf'] ?></td>
			<td width="15%"><?=$atuacao['estado'] ?></td>
			<td width="10%"><?=$atuacao['carga_horaria'] ?></td>
			<td width="10%"><?=$atuacao['data_inicio'] ?></td>
			<td width="10%"><?=$atuacao['data_fim'] ?></td>
			<td width="10%"><?=$atuacao['qtd_solicitado'] ?></td>
			<td width="10%"><?=$atuacao['qtd_aprovado'] ?></td>
		</tr>
<?php 
			}
?>
	</table>
<?php 
		}
	}
}

function pegaPerfilFreire(){

	global $db;
	
	$conf['cpf'] = $_SESSION['fiesabatimento_var']['cpfusuario'];
	$conf['servico'] = 'lerDadosPessoais';
	
	$dados = wf_lerDados( $conf );
	
// 	$sql = "SELECT *
// 			FROM dblink (
// 				'".PARAM_DBLINK_FREIRE."',
// 				'SELECT DISTINCT
// 					pe.no_pessoa,
// 					pr.co_perfil,
// 					pr.no_perfil,
// 					da.co_dep_adm,
// 					da.no_dep_adm,
// 					dt_nascimento	
// 				FROM 
// 					public.tb_sf_pessoa pe 
// 				INNER JOIN public.tb_sf_pessoa_fisica   	   	   pf ON pe.co_pessoa = pf.co_pessoa_fisica 
// 				INNER JOIN public.tb_sf_usuario	        	   	   us ON us.co_usuario = pe.co_pessoa
// 				INNER JOIN public.tb_sf_perfil_usuario  	   	   pu ON pu.co_usuario = us.co_usuario
// 				INNER JOIN public.tb_sf_perfil          	   	   pr ON pr.co_perfil = pu.co_perfil
// 				LEFT  JOIN public.tb_sf_fisica_juridica 	   	   fj ON fj.co_pessoa_fisica = pf.co_pessoa_fisica
// 				LEFT  JOIN public.tb_sf_pessoa_juridica 	   	   pj ON pj.co_pessoa_juridica = fj.co_pessoa_juridica
// 				LEFT  JOIN public.tb_sf_dependencia_administrativa da ON da.co_dep_adm = pj.co_dep_adm
// 				WHERE 
// 					pf.nu_cpf::numeric = ".$_SESSION['fiesabatimento_var']['cpfusuario']."::numeric;'
// 			) as rs (
// 				no_pessoa character varying(100),
// 				co_perfil integer,
// 				no_perfil character varying(50),
// 				co_dep_adm character(1),
// 				no_dep_adm character varying(20),
// 				dt_nascimento date
// 			)
// 			ORDER BY co_perfil DESC";
//	ver($sql);
	return $dados[0];
}

//Função legado....
function aprovarSolicitacaoDiretor($dados){
	
	if( is_array($dados['chk_aprovar']) ){
		foreach( $dados['chk_aprovar'] as $idoid ){
			$docid = pegaDocidSolicitacao($idoid);
			$dados['hstid']	= pegaUltimaTramitacao($docid);
			$dados['sbaid'] = pegaSbaidSolicitacao($idoid);
			$dados['atpid'] = pegaAtpidSolicitacao($dados['sbaid']);
			$dados['htrperfil'] = pegaPerfilGeral();
			$dados['hrtmotivoreabertura'] = 'NULL';
			
			$comentario = 'Solicitação aprovada em '.date('d/m/Y H:i:s').' por '.$_SESSION['usucpf'];
			
			$test = wf_alterarEstado( $docid, WF_FIES1_APROVAR_SOLICITACAO, $comentario, array('docid'=>$docid ) );
			
			insereHistoricoTramitacao($dados);
		}
	}
}

//Retorna os meses confirmados pelo secretário
function retornaConfirmacaoDiretor($atpid)
{
	global $db;
	$sql = "SELECT 
				* 
			FROM 
				fiesabatimento.responsavelanoatuacao  resp
			INNER JOIN fiesabatimento.mesesatuacao mes ON resp.ranid = mes.ranid
			WHERE
				matstatus = 'A'
				AND ranstatus = 'A'
				AND ranresponsaveltipo = 'D'
				AND rancpfresponsavel = '{$_SESSION['usucpf']}'
				AND resp.atpid = $atpid";
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado){
			$arrRetorno[$dado['matano']][trim($dado['matmes'])] = true; 
		}
	}
	return $arrRetorno ? $arrRetorno : array();
}

// Função legado...
function retornaConfirmacaoSecretario($atpid = null)
{
	global $db;
	if(!$atpid){
		return array();
	}
	$sql = "select 
				* 
			from 
				fiesabatimento.responsavelanoatuacao  resp
			inner join
				fiesabatimento.mesesatuacao mes ON resp.ranid = mes.ranid
			where
				matstatus = 'A'
			and
				ranstatus = 'A'
			and
				ranresponsaveltipo = 'S'
			and
				rancpfresponsavel = '{$_SESSION['usucpf']}'";

	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado){
			$arrRetorno[$dado['rananotuacao']][$dado['matmes']] = true; 
		}
	}
	return $arrRetorno ? $arrRetorno : array();
}

//Função legado....
function retornaConfirmacaoEscola($atpinep)
{
	global $db;
	$sql = "select 
				* 
			from 
				fiesabatimento.responsavelanoatuacao  resp
			inner join
				fiesabatimento.mesesatuacao mes ON resp.ranid = mes.ranid
			where
				matstatus = 'A'
			and
				ranstatus = 'A'
			and
				ranresponsaveltipo = 'D'";

	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado){
			$arrRetorno[$dado['rananotuacao']][$dado['matmes']] = true; 
		}
	}
	return $arrRetorno ? $arrRetorno : array();
}

//Função legado...
function recuperaDadosProfessor($atpid)
{
	global $db;
	
	$sql = "select 
				*
			from
				fiesabatimento.identificacaodocente ido
			inner join
				fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
			inner join
				fiesabatimento.atuacaoprofissional atp ON atp.sbaid = sba.sbaid
			inner join
				fiesabatimento.responsavelanoatuacao resp ON resp.atpid = atp.atpid
			inner join
				territorios.municipio mun ON mun.muncod::integer = ido.co_municipio::integer
			where
				sba.sbastatus = 'A'
			and
				atp.atpstatus = 'A'
			and
				ido.idostatus = 'A'
			and
				atp.atpid = $atpid";
	$arrDados = $db->pegaLinha($sql);
	if($arrDados){
		return $arrDados;
	}else{
		return array();
	}
}

//Função legado..
function confirmarSolicitacaoSecretario()
{
	global $db;
	extract($_POST);
	
	if($rdn_confirmar_mes){
		foreach($rdn_confirmar_mes as $cod_inep => $arrAnos){
			if($arrAnos){
				foreach($arrAnos as $ano => $arrMeses){
					$sql = "select 
								ranid 
							from 
								fiesabatimento.responsavelanoatuacao 
							where
								atpid = $atpid
							and
								rananotuacao = $ano
							and
								ranresponsaveltipo = 'S'
							and
								rancpfresponsavel = '{$_SESSION['usucpf']}'
							and
								ranstatus = 'A'";
					$ranid = $db->pegaUm($sql);
					
					if(!$ranid){
						$sql = "insert into 
									fiesabatimento.responsavelanoatuacao  (atpid,co_usuario,rananotuacao,ranresponsaveltipo,rancpfresponsavel,ranstatus)
								values
									($atpid,NULL,$ano,'S','{$_SESSION['usucpf']}','A')
								returning
									ranid";
						$ranid = $db->pegaUm($sql);
					}
					
					$sql = "delete from fiesabatimento.mesesatuacao where ranid = $ranid";
					$db->executar($sql);
					
					if($arrMeses){
						foreach($arrMeses as $mesid => $valor){
							$sqlM .= "insert into 
										fiesabatimento.mesesatuacao (ranid,atpid,sbaid,matmes,matano,matstatus)
									values
										($ranid,$atpid,$sbaid,$mesid,$ano,'A');";
						}
					}
				}
			}
		}
		if($sqlM){
			$db->executar($sqlM);
		}
	}
	$db->commit();
	$_SESSION['fiesabatimento_var']['alert'] = "Operação realizada com sucesso.";
	header("Location: fiesabatimento.php?modulo=principal/aprovacaoabatimentoconcedido&acao=A&atpid=$atpid");
	exit;
}

function atualizaComboMunicipio( $request ){

	global $db;

	extract($request);

	if( $estuf != '' ) $whereMuncod = " WHERE estuf = '".$estuf."' ";

	$sql = "SELECT
				muncod as codigo,
				mundescricao||' - '||estuf as descricao
			FROM
				territorios.municipio
				$whereMuncod
			ORDER BY
				2";
	echo $db->monta_combo('muncod',$sql,'S','Selecione...','','','',200,'N', 'muncod', '', '', '');
}


function enviaEmailAprovacaoParcial( $atpid ){

	global $db;
	
	$sql = "SELECT DISTINCT
				upper(idonome) as nome,
				idoeemail as email,
				atpdescricaoescola as escola,
				sba.sbaid
			FROM
				fiesabatimento.identificacaodocente ido
			INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
			INNER JOIN fiesabatimento.atuacaoprofissional	atp ON atp.sbaid = sba.sbaid
			WHERE
				atpid = $atpid";
	$docente = $db->pegaLinha($sql);
	$sql = "SELECT
				usunome,
				pfl.pflcod
			FROM
				seguranca.usuario usu
			INNER JOIN seguranca.perfilusuario 	pus ON pus.usucpf = usu.usucpf
			INNER JOIN seguranca.perfil			pfl ON pfl.pflcod = pus.pflcod
			WHERE
				usu.usucpf = '".$_SESSION['usucpf']."'
				AND pfl.sisid = ".$_SESSION['sisid'];
	$secretario = $db->pegaLinha($sql);
	
	if( $secretario['pflcod'] == PFL_SECRETARIO_ESTADUAL || $secretario['pflcod'] == PFL_SUB_SECRETARIO_ESTADUAL ){
		$sql = "SELECT
					estdescricao
				FROM
					fiesabatimento.atuacaoprofissional	atp
				INNER JOIN territorios.estado est ON est.estuf = atp.estufprofessor
				WHERE
					atpid = $atpid";
		$uf = $db->pegaUm($sql);
		
		$html = "Secretaria Estadual de Educação de $uf em ".date('d/m/Y');
	}else{
		$sql = "SELECT
					mundescricao
				FROM
					fiesabatimento.atuacaoprofissional atp
				INNER JOIN territorios.municipio mun ON mun.muncod = atp.muncodprofessor
				WHERE
					atpid = $atpid";
		$mun = $db->pegaUm($sql);
		
		$html = "Secretaria Municipal de Educação de $mun em ".date('d/m/Y');
	}
	
	//pega id renovação
	/*
	$preid = $docente['preid'];
	if($preid){
		$DT_INICIO_PROGRAMA = '2013-01-01';
		$DT_FIM_PROGRAMA = '2013-12-31';
	}else{
		$DT_INICIO_PROGRAMA = '2010-01-01';
		$DT_FIM_PROGRAMA = '2014-12-31';
	}
	*/

	$dadossbaid = $db->pegaLinha("SELECT sbaanoinicio, sbaanofim, sbarenovacao FROM fiesabatimento.solicitacaoabatimento
						  		  WHERE sbaid = ".$docente['sbaid']." AND sbastatus = 'A'");
	if($dadossbaid) extract($dadossbaid);

	$DT_INICIO_PROGRAMA = "{$sbaanoinicio}-01-01";
	$DT_FIM_PROGRAMA = "{$sbaanofim}-12-31";
	
	$sql = "SELECT DISTINCT
				(
					(
						CASE WHEN (date_part('year',
													CASE WHEN date_part('year',(to_char(atpdatafim,'YYYY-MM-DD'))::date)::integer < 2013
														THEN (to_char(atpdatafim,'YYYY-MM-DD'))::date
														ELSE '".$DT_FIM_PROGRAMA."'
													END)-
									date_part('year',
													CASE WHEN date_part('year',(to_char(atpdatainicio,'YYYY-MM-DD'))::date)::integer > 2009
														THEN (to_char(atpdatainicio,'YYYY-MM-DD'))::date
														ELSE '".$DT_INICIO_PROGRAMA."'
													END)
									) > 0
							THEN (date_part('year',
									CASE WHEN date_part('year',(to_char(atpdatafim,'YYYY-MM-DD'))::date)::integer < 2013
										THEN (to_char(atpdatafim,'YYYY-MM-DD'))::date
										ELSE '".$DT_FIM_PROGRAMA."'
									END)-
								 date_part('year',
									CASE WHEN date_part('year',(to_char(atpdatainicio,'YYYY-MM-DD'))::date)::integer > 2009
										THEN (to_char(atpdatainicio,'YYYY-MM-DD'))::date
										ELSE '".$DT_INICIO_PROGRAMA."'
									END)
								)-1
							ELSE (date_part('year',
									CASE WHEN date_part('year',(to_char(atpdatafim,'YYYY-MM-DD'))::date)::integer < 2013
										THEN (to_char(atpdatafim,'YYYY-MM-DD'))::date
										ELSE '".$DT_FIM_PROGRAMA."'
									END)-
								 date_part('year',
									CASE WHEN date_part('year',(to_char(atpdatainicio,'YYYY-MM-DD'))::date)::integer > 2009
										THEN (to_char(atpdatainicio,'YYYY-MM-DD'))::date
										ELSE '".$DT_INICIO_PROGRAMA."'
									END)
								)
						END
					)*12
				)
				+
				date_part('month',CASE WHEN date_part('year',(to_char(atpdatafim,'YYYY-MM-DD'))::date)::integer < 2013
							THEN (to_char(atpdatafim,'YYYY-MM')||'-27')::date
							ELSE '".$DT_FIM_PROGRAMA."'
						  END)
				+
				CASE WHEN 
					date_part('year',
					CASE WHEN date_part('year',(to_char(atpdatainicio,'YYYY-MM-DD'))::date)::integer > 2009
								THEN (to_char(atpdatainicio,'YYYY-MM-DD'))::date
								ELSE '".$DT_INICIO_PROGRAMA."'
							END
					)
					<
					date_part('year',
					CASE WHEN date_part('year',(to_char(atpdatafim,'YYYY-MM-DD'))::date)::integer < 2013
							THEN (to_char(atpdatafim,'YYYY-MM')||'-27')::date
							ELSE '".$DT_FIM_PROGRAMA."'
						  END
					)
					THEN
						(
							12-
							date_part('month',CASE WHEN date_part('year',(to_char(atpdatainicio,'YYYY-MM-DD'))::date)::integer > 2009
										THEN (to_char(atpdatainicio,'YYYY-MM-DD'))::date
										ELSE '".$DT_INICIO_PROGRAMA."'
									END)
							+1
						)
					ELSE 0
				END
			FROM
				fiesabatimento.atuacaoprofissional	
			WHERE
				atpid = $atpid";
	
	$qtdDeclarados = $db->pegaUm($sql);
	
	$sql = "SELECT DISTINCT
				count(matid)
			FROM
				fiesabatimento.mesesatuacao mat
			WHERE
				mat.atpid = $atpid";
	$qtdAprovados = $db->pegaUm($sql);
	
	$remetente = array('nome'=>'FIES - Abatimento 1%', 'email'=>'simec@mec.gov.br');
	
	$assunto  = "Solicitação Aprovada - FIES - Abatimento 1%";
	
	$conteudo = "<p>Prezado(a) Professor(a) ".$docente['nome'].",</p>
	
				<p>A atuação profissional da escola ".$docente['escola']." foi analisada pela $html.</p>
				
				<p>Meses declarados: $qtdDeclarados</p>
				
				<p>Meses aprovados: $qtdAprovados</p>
				
				<p>Este é um e-mail automático. Não é necessário respondê-lo.</p>
				
				<p>Atenciosamente,</p>
				<p>Fundo Nacional de Desenvolvimento da Educação - FNDE</p>
				<p>Agente Operador do FIES</p>";
	
	if( $_SESSION['usucpforigem'] == '' ) $docente['email'] = 'alex.pereira@mec.gov.br';
	
	enviar_email($remetente, $docente['email'], $assunto, $conteudo, $cc, $cco );
}

function enviaEmailReabertura( $atpid ){

	global $db;
	
	$sql = "SELECT
				upper(idonome) as nome,
				idoeemail as email,
				atpdescricaoescola as escola,
				cmd.cmddsc as comentario
			FROM
				fiesabatimento.identificacaodocente ido
			INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
			INNER JOIN workflow.documento 					mst ON mst.docid = sba.docid
			INNER JOIN workflow.historicodocumento 			hst ON hst.hstid = mst.hstid
			INNER JOIN workflow.comentariodocumento 		cmd ON cmd.hstid = hst.hstid
			INNER JOIN fiesabatimento.atuacaoprofissional	atp ON atp.sbaid = sba.sbaid
			WHERE
				atpid = $atpid";
	$docente = $db->pegaLinha($sql);
	
	$sql = "SELECT
				usunome,
				pfl.pflcod
			FROM
				seguranca.usuario usu
			INNER JOIN seguranca.perfilusuario 	pus ON pus.usucpf = usu.usucpf
			INNER JOIN seguranca.perfil			pfl ON pfl.pflcod = pus.pflcod
			WHERE
				usu.usucpf = '".$_SESSION['usucpf']."'
				AND pfl.sisid = ".$_SESSION['sisid'];
	$secretario = $db->pegaLinha($sql);
	
	if( $secretario['pflcod'] == PFL_SECRETARIO_ESTADUAL || $secretario['pflcod'] == PFL_SUB_SECRETARIO_ESTADUAL ){
		$sql = "SELECT
					estdescricao
				FROM
					fiesabatimento.atuacaoprofissional	atp
				INNER JOIN territorios.estado est ON est.estuf = atp.estufprofessor
				WHERE
					atpid = $atpid";
		$uf = $db->pegaUm($sql);
		
		$html = "Secretaria Estadual de Educação de $uf em ".date('d/m/Y');
	}else{
		$sql = "SELECT
					mundescricao
				FROM
					fiesabatimento.atuacaoprofissional atp
				INNER JOIN territorios.municipio mun ON mun.muncod = atp.muncodprofessor
				WHERE
					atpid = $atpid";
		$mun = $db->pegaUm($sql);
		
		$html = "Secretaria Municipal de Educação de $mun na data ".date('d/m/Y');
	}
	
	$dataFim = date('d/m/Y',mktime(0, 0, 0, date("m")  , date("d")+30, date("Y")) );
	
	$dataTmp = explode('/',$dataFim);
	
	if( ($dataTmp[2].$dataTmp[1].$dataTmp[0]) > DT_FIM_APROVACAO ){
		$dataFim = $db->pegaUm("SELECT to_char(pdadatafimaprovacao,'DD/MM/YYYY') FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'");
	}
	
	$sql = "SELECT DISTINCT
				mocdesc
			FROM
				fiesabatimento.motivocorrecao moc
			INNER JOIN fiesabatimento.atuacao_motivocorrecao amc ON amc.mocid = moc.mocid
			WHERE
				atpid = $atpid";
	$motivos = $db->carregarColuna($sql);
	$htmMotivos = '';
	if( is_array($motivos) ){
		foreach( $motivos as $motivo ){
			$htmMotivos .= "<p> - $motivo</p>";
		}
	}
	
	$remetente = array('nome'=>'FIES - Abatimento 1%', 'email'=>'simec@mec.gov.br');
	
	$assunto  = "Solicitação Reaberta - FIES - Abatimento 1%";
	
	$conteudo = "<p>Prezado(a) Professor(a) ".$docente['nome'].",</p>
	
				<p>A atuação profissional da escola {$docente['escola']} foi reaberta pela $html para que 
				promova as correções cabíveis até ".$dataFim.".</p>
				
				<p>Caso a atuação profissional não seja corrigida e reenviada para validação até a data estipulada acima, 
				será rejeitada por decurso de prazo e poderá ser insumo para rejeição da solicitação.</p>
				
				<p>Motivo da reabertura:</p>
				$htmMotivos
				<p>{$docente['comentario']}</p>
				
				<p>Observação: A solicitação somente será aprovada ou rejeitada após a validação de todas as 
				Secretarias que possuem vínculo com as escolas informadas na sua solicitação.</p>
				
				<p>Este é um e-mail automático. Não é necessário respondê-lo.</p>
				
				<p>Atenciosamente,</p>
				<p>Fundo Nacional de Desenvolvimento da Educação - FNDE</p>
				<p>Agente Operador do FIES</p>";
	
	if( $_SESSION['usucpforigem'] == '' ) $docente['email'] = 'alex.pereira@mec.gov.br';
	
	if ( $_SERVER['HTTP_HOST'] != "simec-local" ){
       enviar_email($remetente, $docente['email'], $assunto, $conteudo, $cc, $cco );
	}
	
}

function enviaEmailRejeicaoAtuacao( $atpid ){

	global $db;
	/*
	$sql = "SELECT
				upper(idonome) as nome,
				idoeemail as email,
				atpdescricaoescola as escola,
				cmd.cmddsc as comentario
			FROM
				fiesabatimento.identificacaodocente ido
			INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
			INNER JOIN (SELECT
							max(hstid) as hstid,
							docid
						FROM
							workflow.historicodocumento
						GROUP BY
							docid) as mst ON mst.docid = sba.docid
			INNER JOIN workflow.historicodocumento 			hst ON hst.hstid = mst.hstid
			INNER JOIN workflow.comentariodocumento 		cmd ON cmd.hstid = hst.hstid
			INNER JOIN fiesabatimento.atuacaoprofissional	atp ON atp.sbaid = sba.sbaid
			WHERE
				atpid = $atpid";
	*/
	$sql = "SELECT
				upper(idonome) as nome,
				idoeemail as email,
				atpdescricaoescola as escola,
				cmd.cmddsc as comentario
			FROM
				fiesabatimento.identificacaodocente ido
			INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
			INNER JOIN fiesabatimento.atuacaoprofissional	atp ON atp.sbaid = sba.sbaid
			INNER JOIN workflow.documento				doc on atp.docid = doc.docid
			INNER JOIN workflow.comentariodocumento 		cmd ON cmd.hstid = doc.hstid

			WHERE
				atpid = ".$atpid;
	$docente = $db->pegaLinha($sql);

	$sql = "SELECT DISTINCT
				usunome,
				to_char(atpdataconfirmacao,'DD/MM/YYYY') as data,
				pfl.pflcod,
				atp.atpid
			FROM
				fiesabatimento.atuacaoprofissional atp
			INNER JOIN seguranca.usuario usu ON usu.usucpf = lpad(atp.atpidusuconfirmacao::character varying, 11, '0')
			INNER JOIN seguranca.perfilusuario 	pus ON pus.usucpf = usu.usucpf
			INNER JOIN seguranca.perfil			pfl ON pfl.pflcod = pus.pflcod
			WHERE
				atpid = $atpid
				AND atpstatus = 'A'
				AND sisid = ".$_SESSION['sisid'];
	$secretarios = $db->carregar($sql);

	$html = Array();
	if( is_array( $secretarios ) ){
		foreach( $secretarios as $secretario ){
			if( $secretario['pflcod'] == PFL_SECRETARIO_ESTADUAL || $secretario['pflcod'] == PFL_SUB_SECRETARIO_ESTADUAL ){
				$sql = "SELECT
							estdescricao
						FROM
							fiesabatimento.atuacaoprofissional	atp
						INNER JOIN territorios.estado est ON est.estuf = atp.estufprofessor
						WHERE
							atpid = ".$secretario['atpid'];
				$uf = $db->pegaUm($sql);
					
				$html[] = "Secretaria Estadual de Educação de $uf em ".$secretario['data'];
			}else{
				$sql = "SELECT
							mundescricao
						FROM
							fiesabatimento.atuacaoprofissional atp
						INNER JOIN territorios.municipio mun ON mun.muncod = atp.muncodprofessor
						WHERE
							atpid = ".$secretario['atpid'];
				$mun = $db->pegaUm($sql);
					
				$html[] = "Secretaria Municipal de Educação de $mun em ".$secretario['data'];
			}
		}
	}

	$remetente = array('nome'=>'FIES - Abatimento 1%', 'email'=>'simec@mec.gov.br');
	
	$assunto  = "Atuação profissional rejeitada - FIES - Abatimento 1%";

	$conteudo = "<p>Prezado(a) Professor(a) ".$docente['nome'].",</p>
			
				<p>A atuação profissional foi rejeitada pela ".implode(', ',$html).".</p>
				<p>Motivo(s) da rejeição:</p>
				- {$docente['comentario']}
				
				<p>Este é um e-mail automático. Não é necessário respondê-lo.</p>
	
				<p>Atenciosamente,</p>
				<p>Fundo Nacional de Desenvolvimento da Educação - FNDE</p>
				<p>Agente Operador do FIES</p>";

	if( $_SESSION['usucpforigem'] == '' ) $docente['email'] = 'alex.pereira@mec.gov.br';

	if ( $_SERVER['HTTP_HOST'] != "simec-local" ){
		enviar_email($remetente, $docente['email'], $assunto, $conteudo, $cc, $cco );
	}
}

function enviaEmailRejeicao( $sbaid ){

	global $db;
	
	$sql = "SELECT
				upper(idonome) as nome,
				idoeemail as email
			FROM
				fiesabatimento.identificacaodocente ido
			INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
			WHERE
				sbaid = $sbaid";
	$docente = $db->pegaLinha($sql);
	
	$sql = "SELECT DISTINCT
				usunome,
				to_char(atpdataconfirmacao,'DD/MM/YYYY') as data,
				pfl.pflcod,
				atp.atpid
			FROM
				fiesabatimento.atuacaoprofissional atp
			INNER JOIN seguranca.usuario usu ON usu.usucpf = lpad(atp.atpidusuconfirmacao::character varying, 11, '0')
			INNER JOIN seguranca.perfilusuario 	pus ON pus.usucpf = usu.usucpf
			INNER JOIN seguranca.perfil			pfl ON pfl.pflcod = pus.pflcod
			WHERE
				sbaid = $sbaid
				AND atpstatus = 'A'
				AND sisid = ".$_SESSION['sisid'];
	$secretarios = $db->carregar($sql);
	
	$html = Array();
	if( is_array( $secretarios ) ){
		foreach( $secretarios as $secretario ){
			if( $secretario['pflcod'] == PFL_SECRETARIO_ESTADUAL || $secretario['pflcod'] == PFL_SUB_SECRETARIO_ESTADUAL ){
				$sql = "SELECT
							estdescricao
						FROM
							fiesabatimento.atuacaoprofissional	atp
						INNER JOIN territorios.estado est ON est.estuf = atp.estufprofessor
						WHERE
							atpid = ".$secretario['atpid'];
						$uf = $db->pegaUm($sql);
			
				$html[] = "Secretaria Estadual de Educação de $uf em ".$secretario['data'];
			}else{
				$sql = "SELECT
							mundescricao
						FROM
							fiesabatimento.atuacaoprofissional atp
						INNER JOIN territorios.municipio mun ON mun.muncod = atp.muncodprofessor
						WHERE
							atpid = ".$secretario['atpid'];
						$mun = $db->pegaUm($sql);
			
				$html[] = "Secretaria Municipal de Educação de $mun em ".$secretario['data'];
			}
		}
	}
	
	$htmlRejeicao = Array();
	
	$sql = "SELECT
				mcrdesc
			FROM
				fiesabatimento.motivorejeicao
			WHERE
				mcrid NOT IN (
								SELECT mcrid
								FROM fiesabatimento.atuacaomotivorejeicao
								WHERE sbaid = $sbaid
							 )";
	$justs = $db->carregarColuna($sql);
	
	if( count($justs) == 0 ){
		$htmlRejeicao[] = " - Meses insuficientes.";
	}else{
		foreach( $justs as $just ){
			$htmlRejeicao[] = "- $just <br>";
		}
	}
	
	$remetente = array('nome'=>'FIES - Abatimento 1%', 'email'=>'simec@mec.gov.br');
	
	$assunto  = "Solicitação Rejeitada - FIES - Abatimento 1%";
	
	$conteudo = "<p>Prezado(a) Professor(a) ".$docente['nome'].",</p>
	
				<p>A solicitação de abatimento foi rejeitada, pois os requisitos mínimos de período e/ou carga horária não foram atingidos. </p>
				
				<p>Este é um e-mail automático. Não é necessário respondê-lo.</p>
				
				<p>Atenciosamente,</p>
				<p>Fundo Nacional de Desenvolvimento da Educação - FNDE</p>
				<p>Agente Operador do FIES</p>";
	
	if( $_SESSION['usucpforigem'] == '' ) $docente['email'] = 'alex.pereira@mec.gov.br';
	
	if ( $_SERVER['HTTP_HOST'] != "simec-local" ){
		enviar_email($remetente, $docente['email'], $assunto, $conteudo, $cc, $cco );
	}
}

function enviaEmailAprovacao( $sbaid ){

	global $db;
	
	$sql = "SELECT
				upper(idonome) as nome,
				idoeemail as email
			FROM
				fiesabatimento.identificacaodocente ido
			INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
			WHERE
				sbaid = $sbaid";
	$docente = $db->pegaLinha($sql);
	$sql = "SELECT
				usunome,
				to_char(atpdataconfirmacao,'DD/MM/YYYY') as data,
				pfl.pflcod,
				atp.atpid
			FROM
				fiesabatimento.atuacaoprofissional atp
			INNER JOIN seguranca.usuario 		usu ON usu.usucpf = lpad(atp.atpidusuconfirmacao::character varying, 11, '0')
			INNER JOIN seguranca.perfilusuario 	pus ON pus.usucpf = usu.usucpf
			INNER JOIN seguranca.perfil			pfl ON pfl.pflcod = pus.pflcod
			WHERE
				sbaid = $sbaid
				AND atpstatus = 'A'
				AND sisid = ".$_SESSION['sisid'];
	$secretarios = $db->carregar($sql);
	
	$html = Array();
	if( is_array($secretarios) ){
		foreach( $secretarios as $secretario ){
			if( $secretario['pflcod'] == PFL_SECRETARIO_ESTADUAL || $secretario['pflcod'] == PFL_SUB_SECRETARIO_ESTADUAL ){
				$sql = "SELECT
							estdescricao
						FROM
							fiesabatimento.atuacaoprofissional	atp
						INNER JOIN territorios.estado est ON est.estuf = atp.estufprofessor
						WHERE
							atpid = ".$secretario['atpid'];
						$uf = $db->pegaUm($sql);
			
				$html[] = "Secretaria Estadual de Educação de $uf em ".$secretario['data'];
			}else{
				$sql = "SELECT
							mundescricao
						FROM
							fiesabatimento.atuacaoprofissional atp
						INNER JOIN territorios.municipio mun ON mun.muncod = atp.muncodprofessor
						WHERE
							atpid = ".$secretario['atpid'];
						$mun = $db->pegaUm($sql);
			
				$html[] = "Secretaria Municipal de Educação de $mun em ".$secretario['data'];
			}
		}
		
		$remetente = array('nome'=>'FIES - Abatimento 1%', 'email'=>'simec@mec.gov.br');
		
		$assunto  = "Solicitação Aprovada - FIES - Abatimento 1%";
		
		$conteudo = "<p>Prezado(a) professor(a) ".$docente['nome'].",</p>
		
					<p>Sua solicitação foi aprovada em ".$secretario['data']." nos requisitos mínimos para a concessão do abatimento FIES.</p>
					
					<p>Para que seu pedido de abatimento seja efetivado no Agente Financeiro é necessário que você clique no botão 'Enviar requisição' no sistema de abatimento de 1% FIES.</p>

					<p>Obs.: A efetiva concessão do abatimento de 1% fica condicionada à situação de adimplência do financiamento.</p>
					
					<p>Este é um e-mail automático. Não é necessário respondê-lo.</p>
						
					<p>Atenciosamente,</p>
					<p>Fundo Nacional de Desenvolvimento da Educação - FNDE</p>
					<p>Agente Operador do FIES</p>";
		
		if( $_SESSION['usucpforigem'] == '' ) $docente['email'] = 'alex.pereira@mec.gov.br';
		
		if ( $_SERVER['HTTP_HOST'] != "simec-local" ){
			enviar_email($remetente, $docente['email'], $assunto, $conteudo, $cc, $cco );
		}
	}
}

function enviaEmailEviadoBanco( $sbaid ){

	global $db;

	$sql = "SELECT
				upper(idonome) as nome,
				idoeemail as email,
				to_char(dtsuspensao, 'DD/MM/YYYY') as dtsuspensao
			FROM
				fiesabatimento.identificacaodocente ido
			INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
			WHERE
				sbaid = $sbaid";
	$docente = $db->pegaLinha($sql);
	
	if( $docente != '' ){
		$txSuspensao = "<p>Seu pedido foi efetuado com sucesso. A efetivação do abatimento será realizada em {$docente['dtsuspensao']} juntamente com a suspensão das prestações do financiamento, se for o caso.</p>";
	}		

	$remetente = array('nome'=>'FIES - Abatimento 1%', 'email'=>'simec@mec.gov.br');
	
	$assunto  = "Solicitação Rejeitada - FIES - Abatimento 1%";
	
	$conteudo = "<p>Prezado(a) Professor(a) ".$docente['nome'].",</p>
	
				<p>A solicitação de abatimento foi enviada para o banco. </p>
				
				$txSuspensao
		
				<p>Este é um e-mail automático. Não é necessário respondê-lo.</p>
		
				<p>Atenciosamente,</p>
				<p>Fundo Nacional de Desenvolvimento da Educação - FNDE</p>
				<p>Agente Operador do FIES</p>";

	if( $_SESSION['usucpforigem'] == '' ) $docente['email'] = 'alex.pereira@mec.gov.br';

	enviar_email($remetente, $docente['email'], $assunto, $conteudo, $cc, $cco );
}

function testa_prazo_reenvio_solicitacao( $sbaid ){
	
	global $db;
	
	/*
	$sql = "SELECT
				true
			FROM
				fiesabatimento.solicitacaoabatimento sba
			INNER JOIN ( SELECT
							max(hstid) as hstid,
							docid
						FROM
							workflow.historicodocumento
						GROUP BY
							docid ) hs1 ON hs1.docid = sba.docid
			INNER JOIN workflow.historicodocumento hst ON hst.hstid = hs1.hstid
			WHERE
				now()::date - htddata::date > ".PRAZO_REABERTURA."
				AND sbaid = $sbaid";
	*/
	$sql = "SELECT
				true
			FROM
				fiesabatimento.solicitacaoabatimento sba
			INNER JOIN workflow.documento doc ON doc.docid = sba.docid
			INNER JOIN workflow.historicodocumento hst ON hst.hstid = doc.hstid
			WHERE
				now()::date - htddata::date > ".PRAZO_REABERTURA."
				AND sbaid = $sbaid";
	//dbg($sql,1);
	$teste = $db->pegaUm($sql);
	
	if( $teste == 't' ){
		
		$docid = pegaDocidSolicitacaoSbaid($sbaid);
		
		wf_alterarEstado( $docid, WF_FIES1_REJEITAR_PRAZO_REENVIO, 'Rejeitado por decurso de prazo de reenvio.', array('docid'=>$solicitacao['docid'] ) );
		
		$sql = "SELECT idoid FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = $sbaid";
		$idoid = $db->pegaUm( $sql );
		
		$sql = "UPDATE fiesabatimento.solicitacaoabatimento SET sbastatus = 'I' WHERE idoid = ".$idoid.";
				UPDATE fiesabatimento.atuacaoprofissional   SET atpstatus = 'I' WHERE idoid = ".$idoid.";";
		$db->executar($sql);
		$db->commit();
	}
}

//Função legado...
function verificaSolicitacoesDecursoPrazo(){
	
	global $db;
	
	$ano = date('Y');
	$anoAnterior = date('Y');
	$mes = date('m');
	
	if( DT_FIM_APROVACAO > date('Ymd') ){
		return false;
	}
	
	$sql = "SELECT
				sba.sbaid,
				sba.docid,
				upper(ido.idonome) as nome,
				ido.idoeemail,
				to_char(ido.idocpf::numeric,'###.###.###-##') as cpf
			FROM
				fiesabatimento.solicitacaoabatimento sba
			INNER JOIN fiesabatimento.identificacaodocente 	ido ON ido.idoid = sba.idoid
			INNER JOIN workflow.documento 					doc ON doc.docid = sba.docid
			WHERE
				to_char(sbadatasolicitacao,'YYYY')::integer < ".substr(PRAZO_DECURSO_DE_PRAZO,0,4)."
				AND sba.sbastatus = 'A'
				AND doc.esdid = ".WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL;
	
	$solicitacoes = $db->carregar($sql);
	
	if( $sql != ''){
		
		if( is_array($solicitacoes) ){
			foreach( $solicitacoes as $solicitacao ){
				
				$test = wf_alterarEstado( $solicitacao['docid'], WF_FIES1_REJEITAR_PRAZO_ABATIMENTO, 'ejeitado por decurso de prazo.', array('docid'=>$solicitacao['docid'] ) );
				
				//Email Professor
				$remetente = array('nome'=>'FIES - Abatimento 1%', 'email'=>'simec@mec.gov.br');
				
				$assunto  = "Solicitação Rejeitada por Decurso de Prazo - FIES - Abatimento 1%";
				
				$conteudo = "<p>Prezado(a) Professor(a),</p>
				
							<p>Consta nos registros do FIES que a solicitação ou renovação de 
							abatimento não foi aprovada por perda de prazo regulamentar. 
							Está sendo encaminhada mensagem de igual teor para o Secretário/Diretor de Instituição de Ensino Federal.</p>
							
							<p>Atenciosamente,</p>
							<p>Fundo Nacional de Desenvolvimento da Educação - FNDE</p>
							<p>Agente Operador do FIES.</p>";
				
				if( $_SESSION['usucpforigem'] == '' ) $solicitacao['email'] = 'alex.pereira@mec.gov.br';
				
				enviar_email($remetente, $solicitacao['email'], $assunto, $conteudo, $cc, $cco );
				
				//Secretario Estadual
				$sql = "SELECT DISTINCT
							usuemail
						FROM
							fiesabatimento.solicitacaoabatimento sba
						INNER JOIN fiesabatimento.atuacaoprofissional atp ON atp.sbaid = sba.sbaid
						INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.estuf = atp.estufprofessor
						INNER JOIN seguranca.usuario
						WHERE
							atp.esferaprofessor = 'E'";
				$secretarios = $db->carregar($sql);
				if( is_array($secretarios) ){
					foreach( $secretarios as $secretario ){
						
						if( $_SESSION['usucpforigem'] == '' ) $secretario['email'] = 'alex.pereira@mec.gov.br';
						
						enviarEmailSecretarioRejeitaPrazo( $secretario['usuemail'], $solicitacao['nome'], $solicitacao['cpf'] );
					}
				}
				
				//Secretario Municipal
				$sql = "SELECT DISTINCT
							usuemail
						FROM
							fiesabatimento.solicitacaoabatimento sba
						INNER JOIN fiesabatimento.atuacaoprofissional atp ON atp.sbaid = sba.sbaid
						INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.muncod = atp.muncodprofessor
						WHERE
							atp.esferaprofessor = 'M'";
				$secretarios = $db->carregar($sql);
				if( is_array($secretarios) ){
					foreach( $secretarios as $secretario ){
						
						if( $_SESSION['usucpforigem'] == '' ) $secretario['email'] = 'alex.pereira@mec.gov.br';
						
						enviarEmailSecretarioRejeitaPrazo( $secretario['usuemail'], $solicitacao['nome'], $solicitacao['cpf'] );
					}
				}
			}
		}
	}
}

function enviarEmailSecretarioRejeitaPrazo( $email, $nome, $cpf ){
	
	$remetente = array('nome'=>'FIES - Abatimento 1%', 'email'=>'simec@mec.gov.br');
	
	$assunto  = "Solicitação Rejeitada por Decurso de Prazo - FIES - Abatimento 1%";
	
	$conteudo = "<p>Senhor(a) Secretário(a)/Diretor(a),</p>
	
				<p>Consta nos registros do FIES a existência de solicitação ou renovação de abatimento do(a) Professor(a) Sr(a) $nome, CPF: $cpf, ainda não aprovada no prazo regulamentar. Informamos que mensagem de igual teor está sendo encaminhada para o(a) professor(a) solicitante e para o Diretor de Escola vinculado.</p>
				
				<p>Atenciosamente,</p>
				<p>Fundo Nacional de Desenvolvimento da Educação - FNDE</p>
				<p>Agente Operador do FIES.</p>";
	
	if( $_SESSION['usucpforigem'] == '' ) $email = 'alex.pereira@mec.gov.br';
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}

function testa_prazo_solicitacao(){

	if( date('Ymd') < DT_INICIO_SOLICITACAO || date('Ymd') > DT_FIM_SOLICITACAO ){

		$htm = "<script>
					alert('Fora do prazo para solicitação do abatimento.');
					window.location.href = 'fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A';
				</script>";

		die($htm);
	}
}

function testa_prazo_aprovacao(){
	
	if( date('Ymd') < DT_INICIO_APROVACAO || date('Ymd') > DT_FIM_APROVACAO ){
		$htm = "<script>
					alert('Fora do prazo para aprovação das solicitações do abatimento.');
				</script>";
		
		echo $htm;
		
		return true;
	}
	return false;
}

//Calculo de meses inninterruptos com base nos dados da escola
function calculaMeses( $dados, $sbaanoinicio=2010, $sbaanofim=2012, $sbames=12  ){
	
	
	//$teste['meses'] = $carga['dif'];
		//$anos = array(2010,2011,2012,2013,2014);

		$anos = array();
		for($i=$sbaanoinicio; $i<=$sbaanofim; $i++){
			$anos[] = $i;
		}

		//$meses = array('01','02','03','04','05','06','07','08','09','10','11','12');
		$meses = array();
		for($i=1; $i<=$sbames; $i++){
			if(strlen($i)==1) $meses[] = '0'.$i;
			else $meses[] = ''.$i;

		}

		$matriz = Array();
		$matrizOld = Array();
		$escolas = Array();
		$temp = '';
		foreach($anos as $ano){
			foreach($meses as $mes){
				if( is_array($dados) ){
					foreach($dados as $k => $escola){
						if( $escola['dt_inicio'] ){
							$escola['dt_inicio'] = str_replace("/","-",$escola['dt_inicio']);
							$ini = explode('-',$escola['dt_inicio']);
							if(strlen($ini[2]) == 4) $dt_inicio=$ini[2].'-'.$ini[1].'-'.$ini[0];
				   			if(strlen($ini[0]) == 4) $dt_inicio=$ini[0].'-'.$ini[1].'-'.$ini[2];
							$ini = explode('-',$dt_inicio);
														
							$escola['dt_fim'] = $escola['dt_fim'] ? $escola['dt_fim'] : $sbaanofim.'-12-31';
							$escola['dt_fim'] = str_replace("/","-",$escola['dt_fim']);
							$fim = explode('-',$escola['dt_fim']);
							if(strlen($fim[2]) == 4) $dt_fim=$fim[2].'-'.$fim[1].'-'.$fim[0];
				   			if(strlen($fim[0]) == 4) $dt_fim=$fim[0].'-'.$fim[1].'-'.$fim[2];
							$fim = explode('-',$dt_fim);
							
							if( $fim[0] > $sbaanofim ){
								$fim[0] = $sbaanofim;
								$fim[1] = 12;
							}
						}
						if( ( $ano.$mes >= $ini[0].$ini[1]  ) &&
								( $ano.$mes <= $fim[0].$fim[1]  ) ){
							$temp = ( $temp == '' ) ? $escola["co_inep"] : $temp;
							$matriz[$ano.$mes]['escolas'][$escola["co_inep"]]['nu_carga_horaria'] = $escola["nu_carga_horaria"];
							$matriz[$ano.$mes]['carga_mes'] += $escola["nu_carga_horaria"];
							$escolas[$escola["co_inep"]]['dt_inicio'] = $escola['dt_inicio'];
							$escolas[$escola["co_inep"]]['dt_fim'] = $escola['dt_fim'];
							$escolas[$escola["co_inep"]]['nu_carga_horaria'] = $escola['nu_carga_horaria'];
						}
					}
				}
				if( $temp == '' || $matriz[$ano.$mes]['carga_mes'] < 20 ){
					unset($matriz[$ano.$mes]);
					$matrizOld[] = $matriz;
					$matriz = Array();
				}else{
					$temp = '';
				}
			}
		}
		
		//dbg($matriz);
		//dbg(count($matriz));
		//dbg($matrizOld);
		//dbg(count($matrizOld));
		
		
		if( count($matriz) > 11 ){
			$teste['meses'] = count($matriz);
		}else{
			foreach( $matrizOld as $mat ){
				$teste['meses'] = $teste['meses'] < count($mat) ? count($mat) : $teste['meses'];
			}
		}

	if( !$teste['meses'] ) $teste['meses'] = '0';
	
	return $teste;
}
