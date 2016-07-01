<?php 
if( $_REQUEST['req'] == 'baixar'){
	include_once APPRAIZ."includes/classes/fileSimec.class.inc";
	$file 		= new FilesSimec(NULL, NULL, 'evento');
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die();
}

function testaSaecod( $dados ){
	
	global $db;
	
	$dados['cpf'] = str_replace(Array('.','-'),Array('',''),$dados['cpf']);
	
	$sql = "SELECT DISTINCT
				count(nu_demanda) as qtd
			FROM
				evento.sae_ajudadecusto
			WHERE
				nu_cpf = '".$dados['cpf']."'";
	$qtd = $db->pegaUm($sql);

	if( $qtd > 0 ){
		$sql = "SELECT 
					'1' as codigo, 
					'1' as descricao
				FROM 
					public.unidadegestora
				WHERE
					1=0";
		$db->monta_combo('saecod',$sql,'N','Sem demandas para esta rota...','','','','','N', 'saecod');
	}else{
		return false;
	}
}

function buscaSaecod( $dados ){
	
	global $db;
	
	if( $dados['cpf'] != '' && $dados['dt_inicio'] != '' && $dados['dt_fim'] != '' ){
		$dados['cpf'] = str_replace(Array('.','-'),Array('',''),$dados['cpf']);
		$dt_inicio	  = ajusta_data($dados['dt_inicio']);
		$dt_fim	  	  = ajusta_data($dados['dt_fim']);
		
		$sql = "SELECT DISTINCT
					nu_demanda as codigo, 
					ds_demanda as descricao
				FROM
					evento.sae_ajudadecusto
				WHERE
					nu_cpf = '".$dados['cpf']."'
					AND dt_inicio >= '".$dt_inicio."'
					AND dt_fim <= '".$dt_fim."'";
		$testa = $db->carregar($sql);

		if( $testa[0]['codigo'] > 0 ){
			$db->monta_combo('saecod',$sql,'S','Selecione...','','','','','N', 'saecod');
		}else{
			$sql = "SELECT 
						'1' as codigo, 
						'1' as descricao
					FROM 
						public.unidadegestora
					WHERE
						1=0";
			$db->monta_combo('saecod',$sql,'N','Sem demandas para esta rota...','','','','','N', 'saecod');
		}
	}else{
		$sql = "SELECT 
						'1' as codigo, 
						'1' as descricao
					FROM 
						public.unidadegestora
					WHERE
						1=0";
			$db->monta_combo('saecod',$sql,'N','Sem demandas para esta rota...','','','','','N', 'saecod');
	}
}

function monta_cabecalho_ajuda_de_custo( $solid ){
	
	global $db;
	
	if(strlen($_SESSION['evento']['ungcod'])==5){
		$sql = "SELECT 
					unidsc as descricao
				FROM 
					public.unidade
				WHERE
					unicod = '".$_SESSION['evento']['ungcod']."'";
	}else{
		$sql = "SELECT 
					ungdsc as descricao
				FROM 
					public.unidadegestora
				WHERE
					ungcod = '".$_SESSION['evento']['ungcod']."'";
	}
	
	$unidade = $db->pegaUm($sql);
	
	if($solid){
		$docid = $db->pegaUm('SELECT docid FROM evento.solicitacaodiaria WHERE solid = '.$solid);
		$sql = "SELECT
					ed.esdid
				FROM 
					workflow.documento d
				INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE
					d.docid = " . $docid;
		
		$esdid = (integer) $db->pegaUm( $sql );
	}
	
	$empenho = '';
	if( $esdid == WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO || !$solid ){
		
		$sql = "SELECT DISTINCT
					empcod as codigo,
					empcod as descricao
				FROM
					evento.empenho_unidadegestora
				WHERE
					ungcod = '".$_SESSION['evento']['ungcod']."'
					OR unicod = '".$_SESSION['evento']['ungcod']."'";
		
		$empdados = $db->carregar($sql);
		
		$sql = "SELECT DISTINCT
					empcod as descricao
				FROM
					evento.empenho_unidadegestora
				WHERE
					empcod = '".$_SESSION['evento']['empcod']."'";
		$selected = $db->pegaUm($sql);
		
		$empenho = '<select class="obrigatorio" id="empcod" style="width: auto" class="CampoEstilo obrigatorio" name="empcod">
						<option value="">Selecione...</option>';
		foreach($empdados as $emp){
			$empenho .= '<option value="'.$emp['codigo'].'" '.( $emp['codigo'] == $selected ? 'selected' : '' ).'>'.$emp['descricao'].'</option>';
		}
		$empenho .=	'</select>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
		
	}else{
		
		$sql = "SELECT DISTINCT
					empcod as descricao
				FROM
					evento.empenho_unidadegestora
				WHERE
					empcod = '".$_SESSION['evento']['empcod']."'";
		$empenho = $db->pegaUm($sql);
	}
	
	$excluir='';
	if($_SESSION['evento']['solid']){
		
		$sql = "SELECT
					docid
				FROM
					evento.solicitacaodiaria
				WHERE
					solid = ".$_SESSION['evento']['solid'];
		
		$docid = $db->pegaUm($sql);
		$sql = "SELECT
					ed.esdid
				FROM 
					workflow.documento d
				INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE
					d.docid = " . $docid;
		
		$esdid = (integer) $db->pegaUm( $sql );
		
	}

	$html = '<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td class="SubTituloDireita" width="20%">Unidade Gestora</td>
					<td>
						'.$unidade.'
					</td>
					'.$botão.'
				</tr> 
				<tr>
					<td class="SubTituloDireita">Empenho</td>
					<td>
						'.$empenho.'
					</td>
				</tr> 
			</table>';
	echo $html;
}

function descricaoMun($muncod){
	
	global $db;
	
	$sql = "SELECT
				mundescricao||' - '||estuf as mundescricao
			FROM
				territorios.municipio
			WHERE
				muncod = '".$muncod."'";
	return $db->pegaUm($sql);
}

function preparaData($data,$delimitador){
	$data = explode($delimitador,$data);
	$data = $data[2].'/'.$data[1].'/'.$data[0];
	return $data;
}

function retornaClassePagamento($dados){
	
	global $db;
	
	$cpf = str_replace(Array('.','-'),Array('',''),$dados['cpf']);
	
	$sql = "SELECT DISTINCT
				max(clf.clpid) as cplid
			FROM
				siape.tb_siape_cadastro_servidor_ativos scs
			INNER JOIN evento.classe_funcao   clf ON (clf.clffuncao = trim(scs.co_funcao) AND clf.clfnivelfuncao = scs.co_nivel_funcao) OR (clf.clfcargo = scs.co_cargo_emprego)
			WHERE
				nu_cpf = '".$cpf."'";
	$classe = $db->pegaUm($sql);
	$classe = $classe>0 ? $classe : '1';

	if($dados['ajax']==1){
		echo $classe;
	}else{
		return $classe;
	}
}

function retornaInfoCpf($dados){
	
	global $db;
	
	if( $dados['solid'] != '' ){
		$sql = "SELECT DISTINCT
					1 as tem,
					CASE WHEN solorgaodsc is null
						THEN coalesce(ds_orgao,'- - -') END as orgao,
					CASE WHEN solorgaocod is null
						THEN sog.co_orgao::varchar
						ELSE solorgaocod
					END as orgaocod,
					CASE WHEN solempregodsc is null
						THEN coalesce(ds_cargo_emprego,'- - -')
						ELSE solempregodsc
					END as emprego,
					CASE WHEN solempregocod is null
						THEN co_cargo_emprego
						ELSE solempregocod
					END as empregocod,
					CASE WHEN solfuncaodsc is null
						THEN coalesce(co_funcao,'-') || ' - ' || coalesce(co_nivel_funcao::varchar,'-')
						ELSE solfuncaodsc
					END as funcao,
					CASE WHEN solfuncaocod is null
						THEN co_funcao
						ELSE solfuncaocod
					END as funcaocod,
					CASE WHEN solauxtransp is null
						THEN coalesce(svl.vl_transporte,0)::varchar
						ELSE solauxtransp::varchar
					END as vltransporte,
					CASE WHEN solauxalim is null
						THEN '304.00'
						ELSE solauxalim::varchar
					END  as vlalimentacao,
					CASE WHEN trim(foo.solemail) is null
						THEN trim(ds_email)
						ELSE trim(foo.solemail)
					END as email,
					CASE WHEN foo.solbanco is NULL
						THEN coalesce(co_banco,'0')
						ELSE foo.solbanco
					END as banco,
	  				CASE WHEN foo.solagencia is null
	  					THEN coalesce(co_agencia,'0')
	  					ELSE foo.solagencia
	  				END as agencia,
	  				CASE WHEN foo.solconta is null
	  					THEN coalesce(nu_conta_corrente,'0')
	  					ELSE foo.solconta
	  				END as conta
				FROM
					siape.tb_siape_cadastro_servidor_ativos csa
				LEFT  JOIN siape.tb_simec_orgao 				    sog ON sog.co_orgao = csa.co_orgao::numeric
				LEFT  JOIN siape.tb_siape_servidor_valor_transporte svl ON svl.nu_matricula_siape::integer = csa.nu_matricula_siape::integer
				INNER JOIN (SELECT 
								nu_cpf, 
								max(dt_ocorr_ing_orgao_serv) as dt_ocorr_ing_orgao_serv
							FROM 
								siape.tb_siape_cadastro_servidor_ativos
							WHERE 
								nu_cpf = '".$dados['cpf']."'
							GROUP BY 
								nu_cpf ) 						      a ON a.dt_ocorr_ing_orgao_serv = csa.dt_ocorr_ing_orgao_serv
				LEFT JOIN (SELECT
								solorgaodsc,
								solorgaocod,
								solempregodsc,
								solempregocod,
								solfuncaodsc,
								solfuncaocod,
								solauxalim,
								solauxtransp,
								solemail, 
								solbanco, 
								solagencia, 
								solconta,
								usucpf
							FROM 
								evento.solicitacaodiaria
							WHERE
								usucpf = '".$dados['cpf']."' 
								AND solid = ".$dados['solid']." )   foo ON foo.usucpf = csa.nu_cpf
				WHERE
					csa.nu_cpf = '".$dados['cpf']."'";
	}else{
		$sql = "SELECT DISTINCT
					1 as tem,
					coalesce(ds_orgao,'- - -')  as orgao,
					sog.co_orgao::varchar as orgaocod,
					coalesce(ds_cargo_emprego,'- - -') as emprego,
					co_cargo_emprego as empregocod,
					coalesce(co_funcao,'-') || ' - ' || coalesce(co_nivel_funcao::varchar,'-') as funcao,
					co_funcao as funcaocod,
					coalesce(svl.vl_transporte,0) as vltransporte,
					'304.00' as vlalimentacao,
					CASE WHEN trim(ds_email) is null
						THEN trim(foo.solemail)
						ELSE trim(ds_email)
					END as email,
					CASE WHEN co_banco is NULL
						THEN foo.solbanco
						ELSE coalesce(co_banco,'0')
					END as banco,
	  				CASE WHEN co_agencia is null
	  					THEN foo.solagencia
	  					ELSE coalesce(co_agencia,'0')
	  				END as agencia,
	  				CASE WHEN nu_conta_corrente is null
	  					THEN foo.solconta
	  					ELSE coalesce(nu_conta_corrente,'0')
	  				END as conta
				FROM
					siape.tb_siape_cadastro_servidor_ativos csa
				LEFT  JOIN siape.tb_simec_orgao 				    sog ON sog.co_orgao = csa.co_orgao::numeric
				LEFT  JOIN siape.tb_siape_servidor_valor_transporte svl ON svl.nu_matricula_siape::integer = csa.nu_matricula_siape::integer
				INNER JOIN (SELECT 
								nu_cpf, 
								max(dt_ocorr_ing_orgao_serv) as dt_ocorr_ing_orgao_serv
							FROM 
								siape.tb_siape_cadastro_servidor_ativos
							WHERE 
								nu_cpf = '".$dados['cpf']."'
							GROUP BY 
								nu_cpf ) 						      a ON a.dt_ocorr_ing_orgao_serv = csa.dt_ocorr_ing_orgao_serv
				LEFT JOIN (SELECT
								max(solid),
								solorgaodsc,
								solorgaocod,
								solempregodsc,
								solempregocod,
								solfuncaodsc,
								solfuncaocod,
								solauxalim,
								solauxtransp,
								solemail, 
								solbanco, 
								solagencia, 
								solconta,
								usucpf
							FROM 
								evento.solicitacaodiaria
							WHERE
								usucpf = '".$dados['cpf']."' 
							GROUP BY
								solorgaodsc,
								solorgaocod,
								solempregodsc,
								solempregocod,
								solfuncaodsc,
								solfuncaocod,
								solauxtransp,
								solauxalim,
								solemail,
								solbanco,
								solagencia,
								solconta,
								usucpf )   foo ON foo.usucpf = csa.nu_cpf
				WHERE
					csa.nu_cpf = '".$dados['cpf']."'";
	}
	$cpf = $dados['cpf'];
	$dados = $db->pegaLinha($sql);
	
	if(!is_array($dados)){
		
		$sql = "SELECT
					1 as tem,
					max(solid) as id,
					'-' as orgao,
					'-' as orgaocod,
					'-' as emprego,
					'-' as empregocod,
					'-' as funcao,
					'-' as funcaocod,
					'0' as vltransporte,
					'0' as vlalimentacao,
					solemail as email, 
					solbanco as banco, 
					solagencia as agencia, 
					solconta as conta
				FROM 
					evento.solicitacaodiaria
				WHERE
					usucpf = '".$cpf."' 
				GROUP BY 
					solorgaodsc,
					solorgaocod,
					solempregodsc,
					solempregocod,
					solfuncaodsc,
					solfuncaocod,
					solauxtransp,
					solauxalim,
					solemail,
					solbanco,
					solagencia,
					solconta,
					usucpf ";

		$dados = $db->pegaLinha($sql);
	}
	
	echo simec_json_encode($dados);
	die();
}

function salvar($dados){
	global $db;

	$erro = false;
	if(!$dados['cpf']&&!$dados['numdocestrangeiro']){$erro='Cpf/numero de documento inválido';}
	if(!$dados['nome']){$erro='Nome inválido';}
	if(!$dados['banco']){$erro='Banco inválido';}
	if(!$dados['agencia']){$erro='Agência inálida';}
	if(!$dados['conta']){$erro='Conta corrente inválida';}
	if($dados['passagem']=='S'){
		if(!$dados['codSCDP']){$erro='Código SCDP inválido';}
	}
	if($dados['totValor']==0){$erro='Valor total inválido';}
	
	$dados['cpf'] = str_replace(Array('.','-'),Array('',''),$dados['cpf']);
	
	$whereSolid   = $_REQUEST['solid'] 			? "AND sol.solid not in (".$_REQUEST['solid'].")" : '' ;
	$whereSolcomp = $_REQUEST['solcomplemento'] ? "AND sol.solid not in (".$_REQUEST['solcomplemento'].")" : '' ;
	
	$sql = "SELECT 
				sol.solid as solicitacao,
				to_char(min(itidataorigem),'YYYYMMDD') as datamin, 
				to_char(max(itidatadestino),'YYYYMMDD') as datamax
			FROM 
				evento.itinerariosolicitacaodiaria iti
			INNER JOIN evento.solicitacaodiaria sol ON sol.solid = iti.solid
			INNER JOIN workflow.documento 		  d ON d.docid = sol.docid
			INNER JOIN workflow.estadodocumento  ed ON ed.esdid = d.esdid
			WHERE
				usucpf = '".$dados['cpf']."' 
				AND ed.esdid not in (".WF_SOLICITAÇÃO_CANCELADA.")
				AND solnumdocestrangeiro = '".$dados['numdocestrangeiro']."' 
				AND sol.solstatus = 'A' 
				".$whereSolid.$whereSolcomp."
			GROUP BY 
				sol.solid";
//	ver($sql,d);
	$testeData = $db->carregar($sql); 
	
	if( is_array($testeData) ){
		foreach( $testeData as $datas ){
			$dataorigem0 = explode('/', $dados['dataOrigem'][0]);
			$dataorigem0 = $dataorigem0[2].$dataorigem0[1].$dataorigem0[0];
			$datadestino0 = explode('/', $dados['dataDestino'][0]);
			$datadestino0 = $datadestino0[2].$datadestino0[1].$datadestino0[0];
			if( $datas['datamin'] <= $dataorigem0 && $datadestino0 <= $datas['datamax'] ){
				$erro='Já existe solicitação para esta pessoa neste período';
			}
			$dataorigem0 = explode('/', $dados['dataOrigem'][count($dados['dataOrigem'])-1]);
			$dataorigem0 = $dataorigem0[2].$dataorigem0[1].$dataorigem0[0];
			$datadestino0 = explode('/', $dados['dataDestino'][count($dados['dataDestino'])-1]);
			$datadestino0 = $datadestino0[2].$datadestino0[1].$datadestino0[0];
			if( $datas['datamin'] <= $dataorigem0 && $datadestino0 <= $datas['datamax'] ){
				$erro='Já existe solicitação para esta pessoa neste período';
			}
		}
	}
	
	if($erro){
		echo "<script>
				alert('$erro');
				window.location = 'evento.php?modulo=principal/solicitacaoDiarias&acao=A';
			  </script>";
	}else{
		
		if(!$dados['codSCDP']){$dados['codSCDP'] = 'NULL';}
		if(!$dados['obs']){$dados['obs'] = 'NULL';}
		
		$possuipassagem = $dados['passagem'] == 'S' ? 'TRUE' : 'FALSE';
//		ver($dados,d);
		$cpf 						= $dados['cpf'];
		$dados['cpf'] 				= $dados['cpf'] 			  ? '\''.$dados['cpf'].'\'' 					: 'NULL'; 
		$unidade 					= $dados['unicod'] 		  	  ? $dados['unicod'] 		   					: 'NULL';
		$cargo   					= $dados['co_cargo_emprego']  ? $dados['co_cargo_emprego'] 					: 'NULL';
		$funcao  					= $dados['co_funcao'] 		  ? $dados['co_funcao'] 	   					: 'NULL';
		$dados['ordembancaria']  	= $dados['ordembancaria']  	  ? "'".$dados['ordembancaria']."'" 			: 'NULL';
		$solcomplemento		 		= $dados['solcomplemento'] 	  ? "'".$dados['solcomplemento']."'" 			: 'NULL';
		$dados['solgru'] 			= $dados['solgru'] 		  	  ? $dados['solgru'] 							: 'NULL';
		$dados['infvalalimentacao'] = $dados['infvalalimentacao'] ? $dados['infvalalimentacao'] 				: 'NULL';
		$dados['infvaltransporte']  = $dados['infvaltransporte']  ? $dados['infvaltransporte'] 					: 'NULL';
		$dados['solorgaodsc']  		= $dados['solorgaodsc']  	  ? "'".$dados['solorgaodsc']."'" 				: 'NULL';
		$dados['solorgaocod']  		= $dados['solorgaocod']  	  ? "'".$dados['solorgaocod']."'" 				: 'NULL';
		$dados['solempregodsc']  	= $dados['solempregodsc']  	  ? "'".$dados['solempregodsc']."'" 			: 'NULL';
		$dados['solempregocod']  	= $dados['solempregocod']  	  ? "'".$dados['solempregocod']."'" 			: 'NULL';
		$dados['solfuncaodsc']  	= $dados['solfuncaodsc']  	  ? "'".$dados['solfuncaodsc']."'" 				: 'NULL';
		$dados['solfuncaocod']  	= $dados['solfuncaocod']  	  ? "'".$dados['solfuncaocod']."'" 				: 'NULL';
		$dados['codSCDP']  			= $dados['solcompnum'] != ''  ? "'".$dados['codSCDP_bkp']."'" 				: "'".$dados['codSCDP']."'";
		$dados['solcompnum']		= $dados['solcompnum']		  ? $dados['solcompnum']						: 'NULL';
		$dados['auxalim'] 			= $dados['auxalim'] 	  	  ? $dados['auxalim'] 						    : '0';
		$dados['auxtransp'] 		= $dados['auxtransp'] 	  	  ? $dados['auxtransp'] 					    : '0';
		$auxtransp 					= $dados['totDiasSfds'] > 0   ? ($dados['auxtransp']/$dados['totDiasSfds']) : '0';
		
		$empLinha = '';
		if($dados['empcod']){
			$empLinha = $dados['empcod'] ? "empcod = '".$dados['empcod']."'," : '';
		}
		
//		ver($dados['codSCDP'],d);
		if($dados['saecod']){
			
			$sql = "SELECT 
						nu_demanda,
						sg_secretaria as secretaria, 
						ds_demanda as demanda, 
						dt_inicio, 
						dt_fim
					FROM 
						evento.sae_ajudadecusto
					WHERE
						nu_demanda = ".$dados['saecod'];
			$demanda = $db->pegaLinha($sql);
		}
		
		$demanda['nu_demanda'] = $demanda['nu_demanda'] ? $demanda['nu_demanda'] 		 : 'null';
		$demanda['secretaria'] = $demanda['secretaria'] ? "'".$demanda['secretaria']."'" : 'null';
		$demanda['demanda']    = $demanda['demanda']    ? "'".$demanda['demanda']."'"    : 'null';
		$demanda['dt_inicio']  = $demanda['dt_inicio']  ? "'".$demanda['dt_inicio']."'"  : 'null';
		$demanda['dt_fim']     = $demanda['dt_fim']     ? "'".$demanda['dt_fim']."'"     : 'null';

		if( $dados['solcomplemento']!='' ){
			$sql = "SELECT  
						docid
					FROM 
						evento.solicitacaodiaria 
					WHERE
						solid = ".$dados['solcomplemento'];
			$docid = $db->pegaUm($sql);
			$sql = "SELECT
						ed.esdid
					FROM 
						workflow.documento d
					INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
					WHERE
						d.docid = ".$docid;
			$esdid = (integer) $db->pegaUm( $sql );
			
			if( $esdid == WF_SOLICITACAO_DIARIAS_PAGO ){
				echo "<script>
						alert('Solicitação não pode ser complementada, pois não foi paga.');
						window.location = 'evento.php?modulo=principal/solicitacaoDiarias&acao=A';
					  </script>";
			}
			
		}
		
		if($dados['solid']==''){

			$docid = wf_cadastrarDocumento( WF_SOLICITAÇÃO_DIARIAS, "Solicitação de Diárias");
			
			if($_REQUEST['solcomplemento']){
				$sql = "UPDATE evento.solicitacaodiaria SET
							solstatus = 'I'
						WHERE
							solid = ".$_REQUEST['solcomplemento'];
				$db->executar($sql);
			}
			
			if(strlen($_SESSION['evento']['ungcod'])==5){
				$campo = 'unicod';
			}else{
				$campo = 'ungcod';
			}
			
			$sql = "INSERT INTO evento.solicitacaodiaria(
						solusucpf,
	            		usucpf, 
	            		solnumdocestrangeiro, 
	            		solnome, 
	            		solemail,
	            		solbanco, 
	            		solagencia, 
	            		solconta,
	            		solpassagem, 
	            		solcodigoscdb, 
	            		solobservacao,
	            		solobjetivo,
	            		solanoexercicio,
	            		solorgaodsc,
						solorgaocod,
						solempregodsc,
						solempregocod,
						solfuncaodsc,
						solfuncaocod,
	            		solauxalim,
	            		solauxtransp,
	            		soldeducaoalim,
	            		soldeducaotransp,
	            		solunidade,
	            		solcargo,
	            		solfuncao,
	            		soltotdias,
	            		$campo,
	            		empcod,
	            		solordembancaria,
	            		solgru,
	            		solcomplemento,
	            		solcompnum,
	            		soldemcod,
	            		soldemdsc,
	            		soldemsec,
	            		soldemdtini,
	            		soldemdtfim,
	            		docid)
				    VALUES (
				    	'".$_SESSION['usucpf']."',
				    	".$dados['cpf'].",
				    	'".$dados['numdocestrangeiro']."',
				    	'".$dados['nome']."',
				    	'".$dados['email']."',
				    	'".$dados['banco']."',
				    	'".$dados['agencia']."',
				    	'".$dados['conta']."',
				    	".$possuipassagem.",
				    	".$dados['codSCDP'].",
				    	'".$dados['obs']."',
				    	'".$dados['obj']."',
				    	".$_SESSION['exercicio'].",
				    	".$dados['solorgaodsc'].",
				    	".$dados['solorgaocod'].",
				    	".$dados['solempregodsc'].",
				    	".$dados['solempregocod'].",
				    	".$dados['solfuncaodsc'].",
				    	".$dados['solfuncaocod'].",
				    	".$dados['infvalalimentacao'].",
				    	".$dados['infvaltransporte'].",
				    	".$dados['auxalim'].",
				    	".$auxtransp.",
				    	".$unidade.",
				    	".$cargo.",
				    	'".$funcao."',
				    	'".$dados['totDiasSfds']."',
				    	'".$_SESSION['evento']['ungcod']."',
				    	'".$_SESSION['evento']['empcod']."',
				    	".$dados['ordembancaria'].",
				    	".$dados['solgru'].",
				    	".$solcomplemento.",
				    	".$dados['solcompnum'].",
				    	".$demanda['nu_demanda'].",
						".$demanda['demanda'].",
						".$demanda['secretaria'].",
						".$demanda['dt_inicio'].",
						".$demanda['dt_fim'].",
				    	$docid)
				    RETURNING 
				    	solid;";
			$solid = $db->pegaUm($sql);
		}else{
			$solid = $dados['solid'];
			
			$sql = "UPDATE evento.solicitacaodiaria SET
	            		usucpf 					= ".$dados['cpf'].", 
	            		solnumdocestrangeiro 	= '".$dados['numdocestrangeiro']."',
	            		solnome 				= '".$dados['nome']."', 
	            		solemail 				= '".$dados['email']."',
	            		solbanco 				= '".$dados['banco']."', 
	            		solagencia 				= '".$dados['agencia']."', 
	            		--soldigitoagencia 		= '".$dados['digitoagencia']."', 
	            		solconta 		   		= '".$dados['conta']."', 
	            		--soldigitoconta   		= '".$dados['digitoconta']."',
	            		solpassagem 	   		= ".$possuipassagem.",
	            		solcodigoscdb 	   		= ".$dados['codSCDP'].", 
	            		solobservacao 	   		= '".$dados['obs']."',
	            		solobjetivo 	   		= '".$dados['obj']."',
	            		solanoexercicio    		= ".$_SESSION['exercicio'].",
	            		soldeducaoalim     		= ".$dados['auxalim'].",
	            		soldeducaotransp 		= ".$auxtransp.",
	            		solunidade 				= ".$unidade.",
	            		solcargo 				= ".$cargo.", 
	            		solfuncao 				= '".$funcao."',
	            		soltotdias 				= '".$dados['totDiasSfds']."',
	            		solordembancaria 		= ".$dados['ordembancaria'].",
	            		soldemcod 				= ".$demanda['nu_demanda'].",
	            		soldemdsc 				= ".$demanda['demanda'].",
	            		soldemsec		    	= ".$demanda['secretaria'].",
	            		soldemdtini 			= ".$demanda['dt_inicio'].",
	            		soldemdtfim 			= ".$demanda['dt_fim'].",
	            		".$empLinha."
	            		solgru 					= ".$dados['solgru']."--,
	            		--dtsid 				= ".$dados['dtsid']."
				    WHERE
				    	solid = ".$solid.";
				    UPDATE evento.itinerariosolicitacaodiaria SET
				    	itistatus = 'I'
				    WHERE
				    	solid = ".$solid.";";
			if($_FILES['arquivo']['name'] != ''){
				$sql .= "UPDATE evento.anexosolicitacao SET
							axsstatus = 'I'
						WHERE
							solid = ".$solid;
			}
			$db->executar($sql);
		}
//		ver($_FILES['arquivo'],d);
		if( $_FILES['arquivo']['name'] != '' ){
			include_once APPRAIZ."includes/classes/fileSimec.class.inc";
			$campos	= array("solid"	        => $solid,
							"axsdescricao"	=> "'Anexo_solicitacaoDiaria_usucpf_".$cpf."_data_".date('Y-m-d')."'");
		
			$file = new FilesSimec("anexosolicitacao", $campos,"evento");
			
			if(is_file($_FILES["arquivo"]["tmp_name"])){	
				$arquivoSalvo = $file->setUpload("Anexo_solicitacaoDiaria_usucpf_".$cpf."_data_".date('Y-m-d')."","arquivo");
			}
		}
		
		$sql='';
		
		foreach( $dados['origem'] as $k => $origem ){
			$ultimo     = $dados['ultimo'][$k] ? $dados['ultimo'][$k] : ( ($k+1) == count($dados['origem'])   ? 'TRUE'  : 'FALSE' );
			
			$embarque   = $dados['boolEmbarque'][$k] == '1' ? 'TRUE'  : 'FALSE';  
			$escala     = $dados['escala'][$k] == '1'       ? 'TRUE'  : 'FALSE';  
			$rodoviario     = $dados['rodoviario'][$k] == 'R'       ? 'R'  : '';  
			$meiadiaria = $dados['meiadiaria'][$k] == '1'   ? 'FALSE' : 'TRUE';
			
			$sql .= "INSERT INTO evento.itinerariosolicitacaodiaria(
						solid, 
						itiorigem, 
						itidestino, 
						itidataorigem, 
						itidatadestino, 
					    itidias, 
					    itiultimo, 
					    itiadicionalemb, 
					    itiescala,
					    itirodoviario, 
					    itimeiadiaria,
					    itivalordiaria,
					    itivaloradicionalembarque)
					VALUES (
						".$solid.", 
						'".$dados['origem'][$k]."',  
						'".$dados['destino'][$k]."',  
						'".formata_data_sql($dados['dataOrigem'][$k])."', 
						'".formata_data_sql($dados['dataDestino'][$k])."',  
						".$dados['dias'][$k].", 
					    '".$ultimo."', 
					    '".$embarque."',
					    '".$escala."',
					    '".$rodoviario."',
					    '".$meiadiaria."',
					    ".$dados['valorDiaria'][$k].",
					    ".$dados['embarque'][$k].");";
			
		}
		
		if($db->executar($sql)){
			$db->commit();
			$db->sucesso('principal/solicitacaoDiarias','&solid='.$solid);
		}
	}
}

function comparaData($data1, $data2, $op){
	
	$data1 = explode('/',$data1);
	$data1 = $data1[2].$data1[1].$data1[0];
	$data2 = explode('/',$data2);
	$data2 = $data2[2].$data2[1].$data2[0];
	
	switch($op) {
		case">" : return $data1 >  $data2 ? true : false; break;
		case">=": return $data1 >= $data2 ? true : false; break;
		case"<" : return $data1 <  $data2 ? true : false; break;
		case"<=": return $data1 <= $data2 ? true : false; break;
		case"==": return $data1 == $data2 ? true : false; break;
	}
}

function calcularDias($dados){
	require_once( APPRAIZ . 'includes/classes/dateTime.inc');
	if( $dados['anterior'] != 'undefined' && $dados['atual'] != 'undefined' && $dados['atual'] != 'undefined' ){
		$obData = new Data();
		$dias = $obData->quantidadeDeDiasEntreDuasDatas( $dados['anterior'], $dados['atual']);
		$diaOff = 0;
		
		for($i = $dados['data1']; ; ){
			if(comparaData($i, $dados['atual'], '>')){
				break;
			}
			$ano =  substr("$i", 6, 4);
			$mes =  substr("$i", 3, 2);
			$dia =  substr("$i", 0, 2);
			
			$diasemana = date("w", mktime(0,0,0,$mes,$dia,$ano) );
			
			if(in_array($diasemana,Array(1,2,3,4,5))){
				$diaOff++;
			}
			$i = $obData->acrescentaDiasNaData($i,1);
			$i = substr("$i", 6, 2)."/".substr("$i", 4, 2)."/".substr("$i", 0, 4);
		}
	}
	
	echo simec_json_encode(Array('dias' => $dias, 'fds' => $diaOff));
}


function buscarDados($dados){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				solemail as email,
				solbanco as banco,
				solagencia as agencia,
				solconta as conta
			FROM
				evento.solicitacaodiaria
			WHERE
				usucpf = '".$dados['cpf']."'
			LIMIT 1";
	$dados = $db->pegaLinha($sql);
	
	if( is_array($dados) ){
		echo simec_json_encode($dados);
		die();
	}
	
	die();
	
}

function calcularValor($dados){
	
	global $db;

		$clpid = retornaClassePagamento($dados);
		
	$sql = "SELECT 
				vldid,
				vldvalor as valor
			FROM 
				evento.valordiaria
			WHERE
				muncod = '".$dados['origem']."' 
				AND clpid = ".$clpid;
	$valor = $db->pegaLinha($sql);
	
	if( is_array($valor)  ){
		echo simec_json_encode($valor);
		die();
	}
	
	$sql = "SELECT 
				vldid,
				vldvalor as valor
			FROM 
				evento.valordiaria
			WHERE
				muncod is null 
				AND clpid = ".$clpid;
	
	$valor = $db->pegaLinha($sql);
	
	if( is_array($valor) ){
		echo simec_json_encode($valor);
		die();
	}

	$sql = "SELECT 
				vldid,
				vldvalor as valor
			FROM 
				evento.valordiaria
			WHERE
				muncod is null 
				AND clpid is null";
	
	$valor = $db->pegaLinha($sql);
	
	if( is_array($valor) ){
		echo simec_json_encode($valor);
		die();
	}
	
	die();
	
}

function verificaFDS($dados){
	
	require_once( APPRAIZ . 'includes/classes/dateTime.inc');
	$obData = new Data();
	$fds = false;
	
	for($i = $dados['data1']; ; ){
		if(comparaData($i, $dados['data2'], '>')||$fds){
			break;
		}
		$ano =  substr("$i", 6, 4);
		$mes =  substr("$i", 3, 2);
		$dia =  substr("$i", 0, 2);
		
		$diasemana = date("w", mktime(0,0,0,$mes,$dia,$ano) );
		
		if(in_array($diasemana,Array(0,6))){
			$fds = true;
		}
		$i = $obData->acrescentaDiasNaData($i,1);
		$i = substr("$i", 6, 2)."/".substr("$i", 4, 2)."/".substr("$i", 0, 4);
	}
	
	if( $fds ){
		echo 'Devido a existência de fim de semana, faz-se necessário a justificativa das diárias';
		die();
	}
	return '';
	die();
}

function calcularFeriado($dados){
	
	global $db;
	
//	$dataOrigem  = explode('/',$dados['dataOrigem']);
//	$dataOrigem  = $dataOrigem[2]."/".$dataOrigem[1]."/".$dataOrigem[0];
//	$dataDestino = explode('/',$dados['dataDestino']);
//	$dataDestino = $dataDestino[2]."/".$dataDestino[1]."/".$dataDestino[0];
	$dataOrigem  = strftime("%Y/%m/%d",strtotime($dados['dataOrigem']));
	$dataDestino = strftime("%Y/%m/%d",strtotime($dados['dataDestino']));
	 
	$sql = "SELECT
				feddescricao
			FROM
				public.feriados
			WHERE
				feddata >= '".$dataOrigem."'
				AND feddata <= '".$dataDestino."'";
	
	$feriados = $db->carregar($sql);
	
	if( is_array($feriados) ){
		echo 'Devido a existência de feriados, faz-se necessário a justificativa das diárias';
		die();
	}
	return '';
	die();
}

if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
	die();
}

$disabled = 'S';
$block = false;

if($_REQUEST['solcomplemento']){
	$solcomplemento = $_REQUEST['solcomplemento'];
}

if($_REQUEST['solid']||$_REQUEST['solcomplemento']){
	
	$_SESSION['evento']['solid'] = $_REQUEST['solid'];
	$solid = $_REQUEST['solid'];
	$busca = $solid;
	
	if($_REQUEST['solcomplemento']){
		$solcomplemento = $_REQUEST['solcomplemento'];
		$busca = $solcomplemento;
	}
	
	$sql = "SELECT  
				usucpf, 
				solnumdocestrangeiro, 
				tpaid, 
				solnome, 
				solemail, 
				solbanco, 
				solagencia, 
				solconta, 
				solpassagem, 
				solcodigoscdb, 
				solobservacao, 
				solobjetivo,
				soldeducaoalim,
				soldeducaotransp,
				soltotdias,
				unidsc,
				ds_cargo_emprego as cargo,
				CASE WHEN solfuncao = 'NULL'
					THEN ''
					ELSE solfuncao
				END AS solfuncao,
				CASE WHEN solordembancaria = 'NULL'
					THEN ''
					ELSE solordembancaria
				END as solordembancaria,
				solgru,
				docid,
				CASE WHEN sol.ungcod is not null
					THEN sol.ungcod
					ELSE sol.unicod
				END as ungcod,
				empcod,
				solcomplemento,
				solcompnum,
				soldemcod
			FROM 
				evento.solicitacaodiaria sol
			LEFT JOIN public.unidade uni ON uni.unicod = sol.solunidade::varchar
			LEFT JOIN siape.tb_simec_cargo_emprego sce ON sce.co_cargo_emprego = sol.solcargo::varchar
			WHERE
				solstatus = 'A'
				AND solid = ".$busca;
	
	$dados = $db->carregar($sql);
	
	if(!is_array($dados)){
		echo "<script>
				window.location = 'evento.php?modulo=principal/listaSolicitacaoDiarias&acao=A';
			  </script>";
	}
	
	$dados[0]['solcodigoscdb'] = $dados[0]['solcodigoscdb'] != 'NULL' ? $dados[0]['solcodigoscdb'] : '' ;
	$dados[0]['solobservacao'] = $dados[0]['solobservacao'] != 'NULL' ? $dados[0]['solobservacao'] : '' ;
	$solcomplemento = $solcomplemento ? $solcomplemento : $dados[0]['solcomplemento'];
	
	if(!$db->testa_superuser()){
		
		$arPerfisSolicitacaoAjudaCusto = Array(EVENTO_PERFIL_SOLICITADOR, 
						 	 EVENTO_PERFIL_VALIDADOR,
						 	 EVENTO_PERFIL_AGENTE_FINANCEIRO,
						 	 EVENTO_PERFIL_AUTORIZADOR,
					 		 EVENTO_PERFIL_ADMINISTRADOR_DIARIAS
							);
		
		$perfil = pegaPerfilArray($_SESSION['usucpf'],$_SESSION['sisid']);
		
		if( possuiPerfil( $arPerfisSolicitacaoAjudaCusto ) ){
			
			$sql = "SELECT DISTINCT 
						p.ungcod,
						emu.empcod 
					FROM 
						evento.usuarioresponsabilidade ur 
					INNER JOIN public.unidadegestora 		   p ON ur.ungcod  = p.ungcod
					INNER JOIN evento.empenho_unidadegestora emu ON emu.ungcod = ur.ungcod 
																	AND emu.ungcod = '".$dados[0]['ungcod']."'
																	AND emu.empcod = '".$dados[0]['empcod']."'
					WHERE
						ur.rpustatus = 'A' and
						ur.usucpf = '".$_SESSION['usucpf']."' and
						ur.pflcod IN (".implode(',',$perfil).") and
						ur.prsano = '".$_SESSION['exercicio']."'";
			
			$notUngcod = $db->carregarColuna($sql);
			if(!is_array($notUngcod)){
				
				$sql = "SELECT DISTINCT 
						p.ungcod,
						emu.empcod 
					FROM 
						evento.usuarioresponsabilidade ur 
					INNER JOIN public.unidadegestora 		   p ON ur.ungcod  = p.ungcod
					INNER JOIN evento.empenho_unidadegestora emu ON emu.ungcod = ur.ungcod 
																	AND emu.ungcod = ".$dados[0]['ungcod']."
					WHERE
						ur.rpustatus = 'A' and
						ur.usucpf = '".$_SESSION['usucpf']."' and
						ur.pflcod IN (".implode(',',$perfil).") and
						ur.prsano = '".$_SESSION['exercicio']."'";
			
				$notUngcod = $db->carregarColuna($sql);
				
				if(!is_array($notUngcod)){
					echo "<script>
							alert('Acesso negado.');
							window.history.back(-1); 
						  </script>";
				}
			}
		}else{
			echo "<script>
					alert('Acesso negado.');
					window.history.back(-1); 
				  </script>";
		}
	}
	
	$_SESSION['evento']['ungcod'] = $dados[0]['ungcod'];
	$_SESSION['evento']['empcod'] = $dados[0]['empcod'];
	
	if(!$_REQUEST['solcomplemento']&&$dados[0]['docid']){
		$docid = $dados[0]['docid'];
		$sql = "SELECT
					ed.esdid
				FROM 
					workflow.documento d
				INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE
					d.docid = " . $docid;
		
		$esdid = (integer) $db->pegaUm( $sql );
		
		$block = $esdid == WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO ? false : true;
		
		$disabled = $block ? 'N' : 'S';
		if($dados[0]['solcomplemento']){
			$complemento = true;
		}
	}else{
		$dados[0]['solcompnum'] = $dados[0]['solcompnum']+1;
		$complemento = true;
	}
	
	$sql = "SELECT 
				vldid,  
				itiorigem, 
				itidestino, 
				itidataorigem, 
				itidatadestino, 
				itidias, 
				itidiasviajem, 
				itiultimo, 
				itiescala, 
				itirodoviario,
				itimeiadiaria, 
				itiadicionalemb, 
				itidatainclusao, 
				itivalordiaria, 
				itivaloradicionalper, 
				itivaloradicionalembarque
			FROM 
				evento.itinerariosolicitacaodiaria
			WHERE
				itistatus = 'A'
				AND solid = ".$busca;
	
	$dadosItinerario = $db->carregar($sql);
}else{
	unset($_SESSION['evento']['solid']);
}
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

if($_SESSION['evento']['solid']){
	$abas = Array(Array('descricao'=>'Lista de Solicitação de Diarias',
				  		'link'     =>'evento.php?modulo=principal/listaSolicitacaoDiarias&acao=A'),
				  Array('descricao'=>'Solicitações de Diárias - Escolha de Empenho ',
			  			'link'     =>'evento.php?modulo=principal/empenhoSolicitacaoDiarias&acao=A'),
				  Array('descricao'=>'Solicitação de Diarias',
						'link'     =>'evento.php?modulo=principal/solicitacaoDiarias&acao=A&solid='.$_SESSION['evento']['solid']),
					);
	$url = 'evento.php?modulo=principal/solicitacaoDiarias&acao=A&solid='.$_SESSION['evento']['solid'];
}else{
	$abas = Array(Array('descricao'=>'Lista de Solicitação de Diarias',
			  			'link'     =>'evento.php?modulo=principal/listaSolicitacaoDiarias&acao=A'),
				  Array('descricao'=>'Solicitações de Diárias - Escolha de Empenho ',
			  			'link'     =>'evento.php?modulo=principal/empenhoSolicitacaoDiarias&acao=A'),
				  Array('descricao'=>'Solicitação de Diarias',
						'link'     =>'evento.php?modulo=principal/solicitacaoDiarias&acao=A'),
					);
	$url = 'evento.php?modulo=principal/solicitacaoDiarias&acao=A';
}

echo montarAbasArray($abas, $url);

$subtitulo = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Campo obrigatório.';
if($_REQUEST['solid']){
	$titulo = 'Verificar Solicitação de Ajuda de Custo';
}elseif($_REQUEST['solcomplemento']||$solcomplemento!=''){
	$titulo = 'Complementar Solicitação de Ajuda de Custo';
}else{
	$titulo = 'Solicitar Nova Ajuda de Custo';
}
monta_titulo( $titulo, $subtitulo );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<!--<script type='text/javascript' href='../includes/jquery-caret/jquery.caret.1.02.js'></script>-->
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function() {

	var exercicio = <?=substr($_SESSION['exercicio'],2,2); ?>;
	var block = '<?=$disabled ?>';
	var boolFunc = '<?=$dados[0]['funcao'] ?>';
	var solid = '<?=$_REQUEST['solid'] ?>';
	var solcomplemento = '<?=$solcomplemento ?>';
	var complemento = '<?=$complemento ?>';

	if(solid){
		jQuery('#origemI_dsc').attr('disabled',true);
		jQuery('#origemI_img').hide();
		var pri = 0;
		var ult = 0; 
		jQuery('.qtd').each(function(){
			if(jQuery(this).attr('id')>ult){
				ult = jQuery(this).attr('id');
			}
			if(pri==0){
				pri = jQuery(this).attr('id');
			}
		});
		verificaFDS(jQuery('#dataOrigem'+pri).val(),jQuery('#dataDestino'+ult).val());
	}
	
	if(boolFunc==''){
		jQuery('#cpf').blur();
	}

	function validaInfoBancaria(num){

		var info = num.replace('-','');
		info = info.replace('.','');
		info = info.replace('.','');
		info = info.replace('.','');
		if( parseInt(info)>0 ){
			return true;
		}else{
			return false;
		}
	}

	function validaArquivo( arq ){
		var size = parseInt(arq.length);
		var ext  = arq.substring(size-4,size);
		var ext2 = arq.substring(size-5,size);
		if(ext=='.exe'&&ext=='.dll'&&ext=='.bat'&&ext2=='.sys'&&ext=='.cmd'&&ext=='.scr'&&ext=='.vbs'&&ext2=='.ws'){
			return false;
		}else{
			return true;
		}
	}

	function calcularComplemento(){
		var vlrOriginal    = parseFloat(jQuery('#vlrorioginal').val());
		var vlrComplemento = parseFloat(jQuery('#auxtot').val()).toFixed(2);
		jQuery('#vlrcomplemento').val(vlrComplemento);
		jQuery('#lbvlrcomplemento').html(MascaraMonetario(parseFloat(vlrComplemento)+''));
		var vlrcomp = parseFloat(vlrComplemento) - parseFloat(vlrOriginal);
		jQuery('#comptotal').val(vlrcomp);
		jQuery('#lbcomptotal').html(MascaraMonetario((parseFloat(vlrcomp).toFixed(2))+''));
	}
	
//Calcula as deduçõe aqui
	function calculardeducoes(){

		var dias = parseFloat(jQuery('#totDiasSfds').val());
		if(parseFloat(jQuery('#totValor').val())>0){
			dias = parseFloat(dias);
		}else{
			jQuery('#totitinerario').html('0,00');
			jQuery('#totiti').val(0);
			jQuery('#auxalimentacao').html('0,00');
			jQuery('#auxalim').val(0);
			jQuery('#auxtransporte').html('0,00');
			jQuery('#auxtransp').val(0);
			jQuery('#auxtotal').html('0,00');
			jQuery('#auxtot').val(0);
			return false;
		}

		//aqui está o calculo do auxílio alimentação
		var alim  = 0;
		if(jQuery('#boolalim').val()=='1'){
//			alim = parseFloat(dias)*(parseFloat('304.00')/22);
			alim = parseFloat(dias)*(jQuery('#infvalalimentacao').val()/22);
		}else{
			alim = 0;
		}
		jQuery('#auxalimentacao').html(((parseFloat(alim).toFixed(2))+'').replace('.',','));
		jQuery('#auxalim').val(alim);
		
		var trans = 0;
		//Aqui calcula o valor do transporte
		trans = parseFloat(jQuery('#infvaltransporte').val()/22)*parseFloat(dias);
		jQuery('#auxtransporte').html(MascaraMonetario((parseFloat(trans).toFixed(2))+''));
		jQuery('#auxtransp').val(trans);
		
		var iti   = jQuery('#totValor').val();
		iti = parseFloat(iti).toFixed(2);
		jQuery('#totitinerario').html(MascaraMonetario( iti ));
		jQuery('#totiti').val(iti);
		
		var rest  = 0;
		rest = parseFloat(iti)-(parseFloat(alim)+parseFloat(trans));
		jQuery('#auxtot').val(rest);
		jQuery('#auxtotal').html(MascaraMonetario( parseFloat(rest).toFixed(2) ));

		if(solcomplemento!=''){
			calcularComplemento();
		}
	}
	
	function zerardeducoes(){
		jQuery('.deducoes').each(function(){
			jQuery(this).val('');
		});
	}

	function verificaFDS(data1, data2){
		if(data1&&data2){
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "req=verificaFDS&data1="+data1+"&data2="+data2,
				async: false,
				success: function(msg){
					if(msg!=''){
						if(jQuery.trim(jQuery('#obs').val())==''){
							alert(msg);
							jQuery('#obs').addClass('obrigatorio');
							jQuery('#obsObrImg').show();
						}
					}else{
						jQuery('#obs').removeClass('obrigatorio');
						jQuery('#obsObrImg').hide();
					}
				}
			});
		}else{
			jQuery('#obs').removeClass('obrigatorio');
			jQuery('#obsObrImg').hide();
		}
	}
	
	function verificaFeriado(data1, data2){
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "req=calcularFeriado&dataOrigem="+data1+"&dataDestino="+data2,
			async: false,
			success: function(msg){
				if(msg!=''){
					if(jQuery.trim(jQuery('#obs').val())==''){
						alert(msg);
						jQuery('#obs').addClass('obrigatorio');
						jQuery('#obsObrImg').show();
					}
				}else{
					jQuery('#obs').removeClass('obrigatorio');
					jQuery('#obsObrImg').hide();
				}
			}
		});
	}
	
	function calculaTotais(){
		var totDias = 0;
		jQuery('.dias').each(function(){
			totDias = parseInt(totDias)+ parseInt(jQuery(this).val());
		});
		jQuery('#totDias').val(totDias);
		jQuery('#lTotDias').html(totDias);

//		var totDiasSfds = 0;
//		jQuery('.diasSfds').each(function(){
//			totDiasSfds = parseInt(totDiasSfds)+ parseInt(jQuery(this).val());
//		});
//		jQuery('#totDiasSfds').val(totDiasSfds);

		var totAdicionalPer = 0;
		jQuery('.adicionalPer').each(function(){
			totAdicionalPer = parseInt(totAdicionalPer)+ parseInt(jQuery(this).val());
		});
		jQuery('#totAdicionalPer').val(totAdicionalPer);
		jQuery('#lTotAdicionalPer').html(MascaraMonetario(totAdicionalPer+''));

		var totAdicional = 0;
		jQuery('.adicional').each(function(){
			totAdicional = parseInt(totAdicional)+ parseInt(jQuery(this).val());
		});
		jQuery('#totAdicional').val(totAdicional);
		jQuery('#lTotAdicional').html(MascaraMonetario(totAdicional+''));

		var totValor = 0;
		jQuery('.valor').each(function(){
			totValor = parseFloat(totValor)+ parseFloat(jQuery(this).val());
		});
		jQuery('#totValor').val(totValor);
		totValorTela = totValor+'';
		jQuery('#lTotValor').html(MascaraMonetario(totValorTela));

		var totDiarias = 0;
		jQuery('.valorDiarias').each(function(){
			totDiarias = parseFloat(totDiarias) + parseFloat(jQuery(this).val());
		});
		jQuery('#totValorDiarias').html('R$ '+MascaraMonetario(totDiarias+''));
		
	}

	function testaSaecod( cpf ){
//		if( cpf != '' ){
//			jQuery.ajax({
//				type: "POST",
//				url: window.location,
//				data: "req=testaSaecod&cpf="+cpf,
//				async: false,
//				success: function(msg){
//					if(msg){
//						jQuery('#trSaecod').show();
//						jQuery('#divSaecod').html(msg);
//					}else{
//						jQuery('#trSaecod').hide();
//						jQuery('#divSaecod').html('');
//					}
//				}
//			});
//		}else{
//			jQuery('#trSaecod').hide();
//			jQuery('#divSaecod').html('');
//		}
	}

	function buscaSaecod( cpf, dt_inicio, dt_fim ){
//		if( cpf != '' ){
//			jQuery.ajax({
//				type: "POST",
//				url: window.location,
//				data: "req=buscaSaecod&cpf="+cpf+"&dt_inicio="+dt_inicio+"&dt_fim="+dt_fim,
//				async: false,
//				success: function(msg){
//					if(msg){
//						jQuery('#trSaecod').show();
//						jQuery('#divSaecod').html(msg);
//					}else{
//						jQuery('#trSaecod').hide();
//						jQuery('#divSaecod').html('');
//					}
//				}
//			});
//		}
	}

	function cpfBlur(cpf){
		if( cpf ){
			if( !validar_cpf( cpf ) ){
				alert('Cpf inválido!');
				jQuery('#cpf').focus();
				return false;
			}
			var no = cpf.replace('.','');
			no = no.replace('.','');
			no = no.replace('-','');
			var cpf = new dCPF();
			cpf.buscarDados(no);
			if(cpf.dados.no_pessoa_rf){
				jQuery('#nome').val(cpf.dados.no_pessoa_rf);
			}else{
				alert('CPF inexistente.');
				jQuery('#cpf').val('');
				jQuery('#cpf').focus();
				return false;
			}
//			if(jQuery('#dtsid').val()!='3'){
				jQuery.ajax({
					type: "POST",
					url: window.location,
					data: "req=retornaClassePagamento&cpf="+no+"&ajax=1",
					async: false,
					success: function(msg){
						jQuery('#clpid').val(msg);
						var solid = jQuery('#solid').val();
						jQuery.ajax({
							type: "POST",
							url: window.location,
							data: "req=retornaInfoCpf&cpf="+no+"&solid="+solid,
							dataType: 'json',
							async: false,
							success: function(msg){
								if(msg.tem){
									jQuery('#complemento').hide();
									jQuery('#info').show();
									jQuery('#deducao').show();
									jQuery('#inforgao').html(msg.orgao);
									jQuery('#infcargo').html(msg.emprego);
									jQuery('#inffuncao').html(msg.funcao);
									jQuery('#solorgaodsc').val(msg.orgao);
									jQuery('#solorgaocod').val(msg.orgaocod);
									jQuery('#solempregodsc').val(msg.emprego);
									jQuery('#solempregocod').val(msg.empregocod);
									jQuery('#solfuncaodsc').val(msg.funcao);
									jQuery('#solfuncaocod').val(msg.funcaocod);
//									if(jQuery('#email').val()==''){ 
										jQuery('#email').val(msg.email); 
//									}
//									if(jQuery('#banco').val()==''){ 
										jQuery('#banco').val(msg.banco); 
//									}
//									if(jQuery('#agencia').val()==''){ 
										jQuery('#agencia').val(msg.agencia); 
//									}
//									if(jQuery('#conta').val()==''){ 
										jQuery('#conta').val(msg.conta); 
//									}
									if(msg.vlalimentacao){jQuery('#boolalim').val('1');}
									jQuery('#infalimentacao').html( MascaraMonetario(msg.vlalimentacao) );
									jQuery('#infvalalimentacao').val( msg.vlalimentacao );
									jQuery('#inftransporte').html( MascaraMonetario(msg.vltransporte) );
									jQuery('#infvaltransporte').val(msg.vltransporte);
									calculardeducoes();
								}else{
									jQuery('#complemento').hide();
									jQuery('#info').hide();
									jQuery('#deducao').hide();
									jQuery('#inforgao').html('');
									jQuery('#infcargo').html('');
									jQuery('#inffuncao').html('');
									jQuery('#infalimentacao').html('');
									jQuery('#inftransporte').html('');
									jQuery('#infvaltransporte').val(0);
									jQuery('#auxtransporte').html('');
									jQuery('#auxtransp').val(0);
									jQuery('#auxtotal').html('');
									jQuery('#auxtot').val(0);
									jQuery('#email').val('');
									jQuery('#banco').val('');
									jQuery('#agencia').val('');
									jQuery('#conta').val('');
									calculardeducoes();
								}
							}
						});
					}
				});
//			}
		}
	};
	
	jQuery('#cpf').blur(function (){
		if( !validar_cpf( jQuery('#cpf').val() ) && jQuery('#cpf').val() != ''){
			alert('Cpf inválido!');
			jQuery('#cpf').val('');
			jQuery(this).focus();
			return false;
		}
		if(jQuery('#cpfbkp').val()==''||jQuery('#cpfbkp').val()==jQuery('#cpf').val()){
			if(jQuery('#cpfbkp').val()==''){
				jQuery('.itinerario').each(function(){
					jQuery(this).remove();
				});

				jQuery('#origemI_dsc').attr('disabled',false);
				jQuery('#origemI_img').show();
	
				jQuery('#origemI_dsc').attr('disabled',false);
				
				calculaTotais();
	
				calculardeducoes();
			}
			jQuery('#cpfbkp').val(jQuery('#cpf').val());
			cpfBlur(jQuery(this).val());
		}else{
			if(confirm('A troca de CPF resultará na exclusão dos itinerários atuais.\n Deseja continuar?')){
				cpfBlur(jQuery(this).val());
				jQuery('.itinerario').each(function(){
					jQuery(this).remove();
				});

				jQuery('#origemI_dsc').attr('disabled',false);
				jQuery('#origemI_img').show();

				jQuery('#origemI_dsc').attr('disabled',false);
				
				calculaTotais();

				calculardeducoes();
				jQuery('#cpfbkp').val(jQuery('#cpf').val());
			}else{
				jQuery('#cpf').val(jQuery('#cpfbkp').val());
			}
		}
		if(jQuery('#cpfbkp').val()!=''){
			testaSaecod(jQuery('#cpfbkp').val());
		}
	});


	jQuery('#numdocestrangeiro').blur(function (){
		if(jQuery('#numdocestrangeirobkp').val()==''){
			if(jQuery('#numdocestrangeirobkp').val()==''){
				jQuery('.itinerario').each(function(){
					jQuery(this).remove();
				});
	
				jQuery('#origemI_dsc').attr('disabled',false);
				
				calculaTotais();
	
				calculardeducoes();
			}
		}else{
			if(confirm('A troca de CPF resultará na exclusão dos itinerários atuais.\n Deseja continuar?')){
				cpfBlur(jQuery(this).val());
				jQuery('.itinerario').each(function(){
					jQuery(this).remove();
				});

				jQuery('#origemI_dsc').attr('disabled',false);
				
				calculaTotais();

				calculardeducoes();
			}else{
				jQuery('#numdocestrangeiro').val(jQuery('#numdocestrangeirobkp').val());
			}
		}
		jQuery('#numdocestrangeirobkp').val(jQuery('#numdocestrangeiro').val());
	});

	function arredondamento (x, n)
	{
	if (n < 0 || n > 10) return x;
	var pow10 = Math.pow (10, n);
	var y = x * pow10;
	return Math.round (y) / pow10;
	} 
	
	jQuery('#AddMov').click(function(){

		jQuery(this).attr('disabled',true);

		if(jQuery('#cpf').val()==''&&jQuery('#numdocestrangeiro').val()==''){alert('Preencha o campo \'CPF\' ou N° do Documento antes de adicionar rotas ao itinerário.');jQuery('#cpf').focus(); jQuery(this).attr('disabled',false);return false;}
		
//		if(jQuery('#clpid').val()==''&&jQuery('#compvalor').val()==''){alert('É necessário o preenchimento do complemento.');return false;}

		if(jQuery('#escala').val() == ''){alert('Campo \'Escala\' Obrigatório.');jQuery('#escala').focus(); jQuery(this).attr('disabled',false); return false;}
		if(jQuery('#origemI').val() == ''){alert('Campo \'Origem\' Obrigatório.');jQuery('#origemI').focus(); jQuery(this).attr('disabled',false); return false;}
		if(jQuery('#destinoI').val() == ''){alert('Campo \'Destino\' Obrigatório.');jQuery('#destinoI').focus(); jQuery(this).attr('disabled',false); return false;}
		if(jQuery('#origemI').val() == jQuery('#destinoI').val() ){alert('A Origem deve ser diferente do Destino.');jQuery('#destinoI').focus(); jQuery(this).attr('disabled',false); return false; }
		if(jQuery('#dataOrigem').val() == ''){alert('Campo \'Data Origem\' Obrigatório.');jQuery('#dataOrigem').focus(); jQuery(this).attr('disabled',false); return false;}
		if(jQuery('#dataDestino').val() == ''){alert('Campo \'Data Destino\' Obrigatório.');jQuery('#dataDestino').focus(); jQuery(this).attr('disabled',false); return false;}
		if(jQuery('#percdia').val() == ''){alert('Campo \'Valor Ajuda de Custo\' Obrigatório.');jQuery('#percdia').focus(); jQuery(this).attr('disabled',false); return false;}

		var qtd = 0;
		jQuery('.qtd').each(function(){
			if(jQuery(this).attr('id')>qtd){
				qtd = jQuery(this).attr('id');
			}
		});
		qtd = parseInt(qtd)+1;
		var dias = 0;
		var diasSfds = 0;
		var valor = 0;
		var vldid;
		var valorTela = 0;
		var valorDiaria = 0;
		var valorDiariaTela = 0;

		var dtsaida   = jQuery('#dataOrigem').val();
		var dtchegada = jQuery('#dataDestino').val();
		var tempoViajem = 0;
		var Fds = 0;
		var adicionalViajem = 0;

		var pri = 0;
		jQuery('.excluir').each(function(){
			if(pri==0){
				pri = jQuery(this).attr('id');
			}
		});
		if(pri==0){
			var data1 = jQuery('#dataOrigem').val();
		}else{
			var data1 = jQuery('#dataOrigem'+pri).val();
		}
		
		jQuery.ajax({
			type: "POST",
			url: window.location,
			dataType: 'json',
			data: "req=calcularDias&anterior="+dtsaida+"&atual="+dtchegada+"&data1="+data1,
			async: false,
			success: function(msg){
				tempoViajem = msg.dias;
				Fds = msg.fds;
				if(tempoViajem){
					var origem = jQuery('#origemI').val();
					var cpf = jQuery('#cpf').val();
					var clpid = jQuery('#clpid').val();
					jQuery.ajax({
						type: "POST",
						url: window.location,
						dataType: 'json',
						data: "req=calcularValor&origem="+origem+"&clpid="+clpid+"&cpf="+cpf,
						async: false,
						success: function(msg){
							valor = parseFloat(msg.valor);
							vldid = msg.vldid;
						}
					});
				}
				adicionalViajem = parseInt(tempoViajem)*parseFloat(valor);
			}
		});

		var redDisplay = '';
		
		if( qtd == 1 || jQuery('#escala').val() == 1 ){
			dias = 0;
			redDisplay = 'display:none'
		}else{
			valor = 0;
			var ant = parseInt(qtd)-1;
			var anterior = jQuery('#dataDestino'+ant).val();
			var atual	 = jQuery('#dataOrigem').val();
			
			jQuery.ajax({
				type: "POST",
				url: window.location,
				dataType: 'json',
				data: "req=calcularDias&anterior="+anterior+"&atual="+atual+"&data1="+data1,
				async: false,
				success: function(msg){
					dias = msg.dias;
					Fds = msg.fds;
				}
			});

			var origem = jQuery('#origemI').val();

			var clpid = jQuery('#clpid').val();
			var cpf = jQuery('#cpf').val();
			jQuery.ajax({
				type: "POST",
				url: window.location,
				dataType: 'json',
				data: "req=calcularValor&origem="+origem+"&clpid="+clpid+"&cpf="+cpf,
				async: false,
				success: function(msg){
					valor = parseFloat(msg.valor);
					vldid = msg.vldid;
				}
			});

			var anterior = jQuery('#destino'+ant).val();
			var atual	 = jQuery('#origemI').val();
			if( anterior != atual ){alert('A origem da próxima movimentação deve ser igual ao destino da movimentação anterior!');jQuery('#origemI').focus(); return false;}

			var anterior = jQuery('#dataDestino'+ant).val();
			var atual	 = jQuery('#dataOrigem').val();
			var x     = new Data();
			if(!x.comparaData(anterior,atual,'<=')){alert('A data dessa movimentação deve ser MAIOR ou IGUAL que a da movimentação anterior!'+anterior);jQuery('#data').focus(); return false;}
		}
		valorDiaria = parseFloat(valor);
		valorDiariaTela = MascaraMonetario((valorDiaria*parseFloat(jQuery('#percdia').val()))+'');
		dias = parseFloat(dias)+parseInt(tempoViajem);
		valor = valorDiaria*parseFloat(jQuery('#percdia').val());
		valor = valor*( parseFloat(dias) );
		var ultimo = 'FALSE';
		if(complemento){
			var teste = 0; 
			jQuery('.excluir').each(function(){
				teste = parseFloat(teste)+1;
			});
			if(teste==0){
				valor = valor+( parseFloat(valorDiaria)/2 );
				ultimo = 'TRUE';
			}
		}else{
			valor = valor+( parseFloat(valorDiaria)/2 );
			ultimo = 'TRUE';
		}
		valor = valor.toFixed(2)+'';
		adicionalViajemTela = (adicionalViajem+'').replace('.',',');
		
		var adicional = 0;
		var vlAdicional = 0;
		var valor1 = parseFloat(valor);
		var valorTela1 = valor1.toFixed(2)+'';
		valorTela1 = MascaraMonetario(valorTela1);
		valor1     = valorTela1.replace('.','');
		valor1     = valor1.replace('.','');
		valor1     = valor1.replace('.','');
		valor1     = valor1.replace('.','');
		valor1     = valor1.replace(',','.');
		valor     = parseFloat(valor);
		if(jQuery('#adicional').attr('checked')){
			vlAdicional = 95;
			adicional = 1;
			valor = valor+95;
		}
		
		valorTela = valor.toFixed(2)+'';
		valorTela = MascaraMonetario(valorTela);
		valor     = valorTela.replace('.','');
		valor     = valor.replace('.','');
		valor     = valor.replace('.','');
		valor     = valor.replace('.','');
		valor     = valor.replace(',','.');

		var escala = '';
		if(jQuery('#escala').val()=='1'){
			escala = 'Escala';
		}

		var meia='';
		if(jQuery('#percdia').val()=='0.5'&& jQuery('#escala').val()!='1'){
			meia='(meia)';
		}

		var rod = '';
		var rodoviario = '';
		if(jQuery('#BTrodoviario').attr('checked')){
			rod = 'R';
			rodoviario = 'Rodoviário';
		}

		var origem_dsc = jQuery('#origemI_dsc').val();
		var destino_dsc = jQuery('#destinoI_dsc').val();

		var html = '<tr style="background-color: rgb(240,240,240)" id="linha'+qtd+'" name="linha" class="itinerario">'+
						'<td align="center"> '+
							'<label id="lEscala'+qtd+'">'+escala+'</label>'+
							'<input type="hidden" id="escala'+qtd+'"  name="escala[]" value="'+jQuery('#escala').val()+'"/>'+
						'</td>'+
						'<td align="center"> '+
							'<label id="lRodoviario'+qtd+'">'+rodoviario+'</label>'+
							'<input type="hidden" id="rodoviario'+qtd+'"  name="rodoviario[]" value="'+rod+'"/>'+
						'</td>'+
						'<td align="center"> '+
							jQuery('#origemI_dsc').val()+'<br />'+
							'<input type="hidden" id="origem'+qtd+'" name="origem[]" value="'+jQuery('#origemI').val()+'"/>'+
							'<label id="lDataOrigem'+qtd+'">'+jQuery('#dataOrigem').val()+'</label>'+
							'<input type="hidden" id="dataOrigem'+qtd+'"  name="dataOrigem[]" value="'+jQuery('#dataOrigem').val()+'"/>'+
						'</td>'+
						'<td align="center">'+
							'<label id="lDestino'+qtd+'">'+destino_dsc+'</label><br />'+
							'<input type="hidden" id="destino'+qtd+'" name="destino[]" value="'+jQuery('#destinoI').val()+'"/>'+
							'<label id="lDataDestino'+qtd+'">'+jQuery('#dataDestino').val()+'</label>'+
							'<input type="hidden" id="dataDestino'+qtd+'"  name="dataDestino[]" value="'+jQuery('#dataDestino').val()+'"/>'+
						'</td>'+
						'<td align="center">'+
							'<label id="lDias'+qtd+'">'+dias+'</label> Diárias '+
							'<label id="lDiasUlt'+qtd+'" style="color:red;'+redDisplay+'">(+0.5)</label> '+
							'<input type="hidden" class="dias" id="dias'+qtd+'" name="dias[]" value="'+dias+'"/>'+
							'<input type="hidden" class="diasviajem" id="diasviajem'+qtd+'" name="diasviajem[]" value="'+tempoViajem+'"/>'+
						'</td>'+
						'<td align="center"> '+
							'R$ <label id="lValorDiaria'+qtd+'" title="Valor da Diária'+meia+':'+valorDiariaTela+'">'+valorTela1+'</label>'+
							'<input type="hidden" id="valorDiaria'+qtd+'" name="valorDiaria[]" value="'+valorDiaria+'"/>'+
							'<input type="hidden" id="valorDiarias'+qtd+'" class="valorDiarias" value="'+valor1+'"/>'+
							'<input type="hidden" id="vldid'+qtd+'" name="vldid[]" value="'+vldid+'"/>'+
							'<input type="hidden" id="meiadiaria'+qtd+'" name="meiadiaria[]" value="'+jQuery('#percdia').val()+'"/>'+
						'</td>'+
						'<td align="center">'+
							'R$ <label id="lEmbarque'+qtd+'">'+MascaraMonetario(vlAdicional+'')+'</label> '+
							'<input type="hidden" class="adicional" id="embarque'+qtd+'" name="embarque[]" value="'+vlAdicional+'"/>'+
							'<input type="hidden" class="boolAdicional" id="boolEmbarque'+qtd+'" name="boolEmbarque[]" value="'+adicional+'"/>'+
						'</td>'+
						'<td align="center"> '+
							'R$ <label id="lValor'+qtd+'">'+valorTela+'</label> '+
							'<input type="hidden" class="valor" id="valor'+qtd+'" name="valor[]" value="'+valor+'"/>'+
						'</td>'+
						'<td align="center"> '+
							'<img id="'+qtd+'" class="excluir" border="0" title="Remover itinerário." src="../imagens/excluir.gif">'+
							'<input type="hidden" id="'+qtd+'" class="qtd" />'+
							'<input type="hidden" id="ultimo" name="ultimo[]" value="'+ultimo+'" />'
						'</td>'+
					'</tr>';
		
		var ult = 0; 
		jQuery('.qtd').each(function(){
			if(jQuery(this).attr('id')>ult){
				ult = jQuery(this).attr('id');
			}
		});

		jQuery('.excluir').each(function(){
			jQuery(this).hide();
		});

		if(!complemento){
			jQuery('#valorDiarias'+ult).val( parseFloat( jQuery('#valorDiaria'+ult).val() ) * parseFloat( jQuery('#dias'+ult).val() )  );
			jQuery('#lValorDiaria'+ult).html(MascaraMonetario(parseFloat(jQuery('#valorDiarias'+ult).val()).toFixed(2)));
			
			jQuery('#valor'+ult).val( parseFloat(jQuery('#valorDiarias'+ult).val()) + parseFloat( jQuery('#embarque'+ult).val() ) );
			jQuery('#lValor'+ult).html( MascaraMonetario(parseFloat(jQuery('#valor'+ult).val()).toFixed(2)) );
	
			jQuery('#lDiasUlt'+ult).html(' ');

			jQuery('#ultimo').val('FALSE');
		}
							
		jQuery('#bordainferior').before(html);

		if(complemento){
			var teste = 0; 
			jQuery('.excluir').each(function(){
				teste = parseFloat(teste)+1;
			});
			jQuery('.qtd').each(function(){
				if(jQuery(this).attr('id')>ult){
					ult = jQuery(this).attr('id');
				}
			});
			if(teste==1){
				jQuery('#lDiasUlt'+ult).html('(+0.5)');
			}else{
				jQuery('#lDiasUlt'+ult).html('');
			}
		}
		
		calculaTotais();

		var pri = 0;
		var ult = 0; 
		jQuery('.qtd').each(function(){
			if(jQuery(this).attr('id')>ult){
				ult = jQuery(this).attr('id');
			}
			if(pri==0){
				pri = jQuery(this).attr('id');
			}
		});

		jQuery.ajax({
			type: "POST",
			url: window.location,
			dataType: 'json',
			data: "req=calcularDias&anterior="+jQuery('#dataOrigem'+pri).val()+"&atual="+jQuery('#dataDestino'+ult).val()+"&data1="+jQuery('#dataOrigem'+pri).val(),
			async: false,
			success: function(msg){
				jQuery('#totDiasSfds').val(msg.fds)
			}
		});

//		jQuery('#totDiasSfds').val(parseFloat(Fds));
		
		calculardeducoes();
		
		verificaFeriado(jQuery('#dataOrigem'+pri).val(),jQuery('#dataDestino'+ult).val());
		verificaFDS(jQuery('#dataOrigem'+pri).val(),jQuery('#dataDestino'+ult).val());
		buscaSaecod( jQuery('#cpf').val(), jQuery('#dataOrigem'+pri).val(), jQuery('#dataDestino'+ult).val() );

		jQuery('#origemI_dsc').val(destino_dsc);
		jQuery('#origemI').val(jQuery('#destinoI').val());
		jQuery('#origemI_dsc').attr('disabled',true);
		jQuery('#origemI_img').hide();
		jQuery('#dataOrigem').val(jQuery('#dataDestino').val());
		jQuery('#destinoI_dsc').val('Selecione...');
		jQuery('#destinoI').val('');

		jQuery(this).attr('disabled',false); 
	});

	jQuery('.excluir').live('click',function (){
		var row    = jQuery(this).attr('id');
		var tbSize = jQuery('#movimentacao tr').length;
		var first = '';
		jQuery('.qtd').each(function(){
			if(first==''){
				first = jQuery(this).attr('id');
			}
		});
		if( first == row && tbSize > '5' ){
			var prox = parseInt(row)+1;
			jQuery('#dias'+prox).val('0');
			jQuery('#lDias'+prox).html('0');
			jQuery('#lValorDiaria'+prox).val('0');
			jQuery('#valorDiaria'+prox).html('0');
			jQuery('#vldid'+prox).html('0');
			jQuery('#lAdicionalPer'+prox).html('0');
			jQuery('#acidionalPer'+prox).val('0');
			var valor;
			valor = parseFloat(jQuery('#embarque'+prox).val());
			jQuery('#valor'+prox).val(valor);
			valor = valor.toFixed(2)+'';
			jQuery('#lValor'+prox).html(valor.replace('.',','));
		}

		
		jQuery('#linha'+row).remove();
		
		var qtd = 0; 
		jQuery('.qtd').each(function(){
			if(jQuery(this).attr('id')>qtd){
				qtd = jQuery(this).attr('id');
			}
		});
		if( qtd == 0 ){
			jQuery('#origemI_dsc').val('Selecione...');
			jQuery('#origemI').val('');
			jQuery('#destinoI_dsc').val('Selecione...');
			jQuery('#destinoI').val('');
			jQuery('#origemI_dsc').attr('disabled',false);
			jQuery('#origemI_img').show();
		}else{
			jQuery('#origemI_dsc').val(jQuery('#lDestino'+qtd).html());
			jQuery('#origemI').val(jQuery('#destino'+qtd).val());
			jQuery('#dataOrigem').val(jQuery('#dataDestino'+qtd).val());
			jQuery('#dataDestino').val(jQuery('#dataDestino'+qtd).val());
			jQuery('#destinoI_dsc').val('Selecione...');
			jQuery('#destinoI').val('');
			jQuery('#origemI_dsc').attr('disabled',true);
			jQuery('#origemI_img').hide();
		}

		var ult = 0; 
		jQuery('.qtd').each(function(){
			if(jQuery(this).attr('id')>ult){
				ult = jQuery(this).attr('id');
			}
		});

		jQuery('#'+ult).show();
		if( parseInt(jQuery('#dias'+ult).val())>0 && !complemento ){

			jQuery('#valorDiarias'+ult).val(parseFloat(jQuery('#valorDiarias'+ult).val())+(parseFloat(jQuery('#valorDiaria'+ult).val())/2));
			jQuery('#lValorDiaria'+ult).html((parseFloat(jQuery('#valorDiarias'+ult).val()).toFixed(2)).replace('.',','));

			jQuery('#valor'+ult).val(parseFloat(jQuery('#valor'+ult).val())+(parseFloat(jQuery('#valorDiaria'+ult).val())/2)-(parseInt(jQuery('#boolEmbarque'+ult).val())*95));
			jQuery('#lValor'+ult).html((parseFloat(jQuery('#valor'+ult).val()).toFixed(2)).replace('.',','));

			jQuery('#embarque'+ult).val(0);
			jQuery('#lEmbarque'+ult).html((parseFloat(jQuery('#embarque'+ult).val()).toFixed(2)).replace('.',','));

			jQuery('#lDiasUlt'+ult).html('(+0.5)');
		}
		var dt_inicio = '';
		var dt_fim 	  = '';
		if( jQuery('#dataOrigem'+first).val() && jQuery('#dataDestino'+ult).val() ){
			dt_inicio = jQuery('#dataOrigem'+first).val();
			dt_fim	  = jQuery('#dataDestino'+ult).val();
		}

		jQuery.ajax({
			type: "POST",
			url: window.location,
			dataType: 'json',
			data: "req=calcularDias&anterior="+jQuery('#dataOrigem'+first).val()+"&atual="+jQuery('#dataDestino'+ult).val()+"&data1="+jQuery('#dataOrigem'+first).val(),
			async: false,
			success: function(msg){
				jQuery('#totDiasSfds').val(msg.fds)
			}
		});
		
		verificaFDS(jQuery('#dataOrigem'+first).val(),jQuery('#dataDestino'+ult).val());
		buscaSaecod( jQuery('#cpf').val(), dt_inicio, dt_fim );
		calculaTotais();
		calculardeducoes();
		
	});

	jQuery('#salvar').click(function(){
		
		jQuery(this).attr('disabled',true);

		var pendencia = false;
		
		jQuery('.obrigatorio').each(function(){
			var val = jQuery.trim(jQuery(this).val());
			//alert(val.length);
			if( val.length == 0 ){
				jQuery(this).focus();
				pendencia = true;
				jQuery(this).attr('disabled',false);
				return false;
			}
		});
				
		if( pendencia ){alert('Campo obrigatório.');jQuery(this).attr('disabled',false); return false; }

		if( !validaInfoBancaria(jQuery('#banco').val()) ){
			alert('Banco inválido.');
			jQuery(this).attr('disabled',false);
			jQuery('#banco').focus();
			return false;
		}

//		if( !validaInfoBancaria(jQuery('#agencia').val()) ){
//			alert('Agência inválida.');
//			jQuery(this).attr('disabled',false);
//			jQuery('#agencia').focus();
//			return false;
//		}
//
//		if( !validaInfoBancaria(jQuery('#conta').val()) ){
//			alert('Conta Corrente inválida.');
//			jQuery(this).attr('disabled',false);
//			jQuery('#conta').focus();
//			return false;
//		}

		if(!validaEmail(jQuery('#email').val())){
			alert('E-mail inválido');
			jQuery(this).attr('disabled',false);
			jQuery('#email').focus();
			return false;
		}

		if(jQuery('#codSCDP').val()!=''){
			var erro = false;
	        var cod = jQuery.trim(jQuery('#codSCDP').val());
	     	// testa tamanho campo
	        if ( (cod.length!=9)&&(cod.length!=12) )
	        {
		        erro = true;
	        }
			// testa campo zerado
	        if ((cod.indexOf("0")>=6) )
	        {
		        erro = true;
	        }
	        var exerc = cod.substring(7,9);
	        if( exerc != exercicio){
		        erro = true;
		    }
	        if(erro){
		        alert('Código SCPD inválido.');
		        jQuery('#codSCDP').focus();
		        jQuery(this).attr('disabled',false);
		        return false;
		    }
		}

		var arq = '';
		if(jQuery('#arquivo').val()){
			arq = jQuery('#arquivo').val();
		}
		if( arq != '' ){
			if(!validaArquivo( arq )){
				alert('Extenção de arquivo inválida.');
				jQuery(this).attr('disabled',false);
				jQuery('#arquivo').focus();
				return false;
			}
		}

		var tbSize = jQuery('#movimentacao tr').length;
		if(tbSize<5){alert('Insira no mínimo uma movimentação no seu itinerário.');jQuery(this).attr('disabled',false);return false;}

		var ln;
		var anterior;
		var atual;
		var x=0;
		var OK = true;
		jQuery('.qtd').each(function(){
			ln = jQuery(this).attr('id');
			if(x!='0'){
				atual = jQuery('#origem'+ln);	
				if(jQuery(atual).val()!=jQuery(anterior).val()){alert('Itinerário inválido!\nCertifique-se se as origens e destinos estão coesas.');OK=false;}
			}		
			anterior = jQuery('#destino'+ln);	
			x++;
		});
		if(jQuery('#totValor').val()<='0'){alert('Valor total da solicitação deve ser MAIOR QUE R$ 0,00!');jQuery(this).attr('disabled',false);return false;}
		if(OK){
			jQuery('#req').val('salvar');
			jQuery('#formSolDia').submit();
		}
		jQuery(this).attr('disabled',false);
	});

	jQuery('#dataOrigem').change(function(){
		jQuery('#dataDestino').val(jQuery('#dataOrigem').val());
	});

	jQuery('#trocacpf').click(function(){
		if( jQuery('#cpfbkp').val() != ''){
			if(confirm('Esta operação resultará na exclusão dos itinerários atuais.\n Deseja continuar?')){
				jQuery('.itinerario').each(function(){
					jQuery(this).remove();
				});
	
				jQuery('#origemI_dsc').attr('disabled',false);
				
				calculaTotais();
	
				calculardeducoes();

				jQuery('#email').val('');
				jQuery('#banco').val('');
				jQuery('#agencia').val('');
				jQuery('#conta').val('');
				jQuery('#trcpf').hide();
				jQuery('#cpf').removeClass('obrigatorio');
				jQuery('#trnumdoc').show();
				jQuery('#numdocestrangeiro').addClass('obrigatorio');
				if(block!='N'){
					jQuery('#nome').removeClass('disabled');
					jQuery('#nome').addClass('normal');
					jQuery('#nome').attr('readonly',false);
				}
				jQuery('#cpf').val('');
				jQuery('#nome').val('');
				jQuery('#numdocestrangeiro').val('');
			}
		}else{
			jQuery('#email').val('');
			jQuery('#banco').val('');
			jQuery('#agencia').val('');
			jQuery('#conta').val('');
			jQuery('#trcpf').hide();
			jQuery('#cpf').removeClass('obrigatorio');
			jQuery('#trnumdoc').show();
			jQuery('#numdocestrangeiro').addClass('obrigatorio');
			if(block!='N'){
				jQuery('#nome').removeClass('disabled');
				jQuery('#nome').addClass('normal');
				jQuery('#nome').attr('readonly',false);
			}
			jQuery('#cpf').val('');
			jQuery('#nome').val('');
			jQuery('#numdocestrangeiro').val('');
		}
	});

	jQuery('#trocanumdoc').click(function(){
		if( jQuery('#trocanumdoc').val() != ''){
			if(confirm('Esta operação resultará na exclusão dos itinerários atuais.\n Deseja continuar?')){
				jQuery('.itinerario').each(function(){
					jQuery(this).remove();
				});
	
				jQuery('#origemI_dsc').attr('disabled',false);
				
				calculaTotais();
	
				calculardeducoes();

				jQuery('#email').val('');
				jQuery('#banco').val('');
				jQuery('#agencia').val('');
				jQuery('#conta').val('');
				jQuery('#trnumdoc').hide();
				jQuery('#numdocestrangeiro').removeClass('obrigatorio');
				jQuery('#trcpf').show();
				jQuery('#cpf').addClass('obrigatorio');
				if(block!='N'){
					jQuery('#nome').removeClass('normal');
					jQuery('#nome').addClass('disabled');
					jQuery('#nome').attr('readonly',true);
				}
				jQuery('#cpf').val('');
				jQuery('#nome').val('');
				jQuery('#numdocestrangeiro').val('');
			}
		}else{
			jQuery('#email').val('');
			jQuery('#banco').val('');
			jQuery('#agencia').val('');
			jQuery('#conta').val('');
			jQuery('#trnumdoc').hide();
			jQuery('#numdocestrangeiro').removeClass('obrigatorio');
			jQuery('#trcpf').show();
			jQuery('#cpf').addClass('obrigatorio');
			if(block!='N'){
				jQuery('#nome').removeClass('normal');
				jQuery('#nome').addClass('disabled');
				jQuery('#nome').attr('readonly',true);
			}
			jQuery('#cpf').val('');
			jQuery('#nome').val('');
			jQuery('#numdocestrangeiro').val('');
		}
	});

	jQuery('#novo').click(function(){
		window.location = 'evento.php?modulo=principal/solicitacaoDiarias&acao=A';
	});

	jQuery('#voltar').click(function(){
		window.location = 'evento.php?modulo=principal/listaSolicitacaoDiarias&acao=A';
	});

	jQuery('#dataDestino').change(function (){
		var X = new Data();
		
		if(X.comparaData(jQuery('#dataOrigem').val(), jQuery('#dataDestino').val(), '>')||X.comparaData(jQuery('#dataDestino').val(), X.dtAddDia(jQuery('#dataOrigem').val(), 1), '>')){
			alert('A data de destino deve ser igual ou 1(um) dia maior que origem.');
			jQuery('#dataDestino').val('');
			jQuery('#dataDestino').focus();
		}

		var ult = 0; 
		jQuery('.qtd').each(function(){
			if(jQuery(this).attr('id')>ult){
				ult = jQuery(this).attr('id');
			}
		});
		
	});


	jQuery('#dataOrigem').change(function (){
		var X = new Data();

		var ult = 0; 
		jQuery('.qtd').each(function(){
			if(jQuery(this).attr('id')>ult){
				ult = jQuery(this).attr('id');
			}
		});

		if(X.comparaData(jQuery('#dataOrigem').val(), jQuery('#dataDestino'+ult).val(), '<')){
			alert('A data de origem deve ser maior ou igual a data de destino do último trecho.');
			jQuery('#dataOrigem').val('');
			jQuery('#dataOrigem').focus();
		}
		
	});

	//Complemento campo_popup_autocomplete
	
	jQuery('.campo_popup_autocomplete').autocomplete("/geral/campopopup.php?nome=origemI&autocomplete=1", {
	  	cacheLength:50,
	  	max:50,
		width: 440,
		scrollHeight: 220,
		delay: 1000,
		selectFirst: true,
		autoFill: false
	});

	jQuery('.campo_popup_autocomplete').result(function(event, data, formatted) {
	     if (data) {

	     	// Extract the data values
	     	var campoHidden = jQuery(this).attr('id').replace('_dsc','');
			
			var descricao = data[0];
			var id = data[1];

			if( id == '' ){
		    	jQuery(this).val('');
		    	return false;
			}

	     	jQuery('#'+campoHidden).val( id );
		  	jQuery('#'+campoHidden+'_retorno').val( descricao );
		}
	});

	jQuery('.campo_popup_autocomplete').blur(function (){
		jQuery(this).val(jQuery(this).val());
	  	var campoHidden = jQuery(this).attr('id').replace('_dsc','');
	  	if( jQuery(this).val() != '' && jQuery('#'+campoHidden+'_retorno').val() != '' ){
			if( jQuery(this).val() != jQuery('#'+campoHidden+'_retorno').val() ){
				jQuery(this).val('Selecione...')
				jQuery('#'+campoHidden).val('')
				jQuery('#'+campoHidden+'_retorno').val('')
			}
		}else{
			jQuery(this).val('Selecione...')
		}
	});

	jQuery('[name="passagem"]').click(function(){
		if(jQuery(this).val()=='S'){
			jQuery('#codSCDP').addClass('obrigatorio');
			jQuery('#imgcodSCDP').show();
		}
		if(jQuery(this).val()=='N'){
			jQuery('#codSCDP').removeClass('obrigatorio');
			jQuery('#imgcodSCDP').hide();
		}
	});

	jQuery('.arquivo').click(function(){
	    windowOpen('evento.php?modulo=principal/solicitacaoDiarias&acao=A&req=baixar&arqid='+jQuery(this).attr('id'),
	                      '', 'height=800,width=1024,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no');
	});

	jQuery('.excluirAnexo').click(function(){
		jQuery(this).hide();
	    jQuery('.arquivo').each(function(){
		    jQuery(this).css('color','grey');
		});
		jQuery('#arquivoL').show();
	    jQuery('.naoExcluirAnexo').each(function(){
		    jQuery(this).show();
		});
		jQuery('[name="arquivo"]').show();
	});

	jQuery('.naoExcluirAnexo').click(function(){
		jQuery(this).hide();
	    jQuery('.arquivo').each(function(){
		    jQuery(this).css('color','blue');
		});
	    jQuery('#arquivoL').hide();
	    jQuery('.excluirAnexo').each(function(){
		    jQuery(this).show();
		});
		jQuery('[name="arquivo"]').hide();
	});

	jQuery('#comptransporte').blur(function(){
		var val = (parseFloat(jQuery(this).val())+'').replace(',','.');
		jQuery('#comptransp').val(val);
		jQuery('#booltransp').val('1');
		calculardeducoes();
	});

//	jQuery('#dtsid').change(function(){
//		jQuery('#cpf').val('');
//		jQuery('#nome').val('');
//		if(jQuery(this).val()!='3'){
//			jQuery('#deducao').show();
//			jQuery('#complemento').hide();
//			jQuery('#boolalim').val('0');
//			jQuery('#booltransp').val('0');
//			calculardeducoes();
//		}else{
//			jQuery('#boolalim').val('1');
//			jQuery('#booltransp').val('1');
//			jQuery('#deducao').hide();
//			zerardeducoes();
//			jQuery('.camposervidor').each(function(){
//				jQuery(this).hide();
//			});
////			jQuery('#complemento').show();
//			calculardeducoes();
//		}
//	});

	jQuery('#auxalimetacaoS').click(function(){
		jQuery('#boolalim').val('1');
		jQuery('#booltransp').val('1');
		calculardeducoes();
	});

	jQuery('.complementarSol').click(function(){
		if(confirm('Realmente deseja complementar esta solicitação?')){
			var solid = jQuery('#solid').val();
			window.location = 'evento.php?modulo=principal/solicitacaoDiarias&acao=A&solcomplemento='+solid;
		}
	});

	jQuery('#codSCDP').keyup(function(){
		var texto = jQuery(this).val();
		texto = texto.replace('/','');
		texto = texto.replace('-','');
		texto = texto.replace('C','');
		padrao = /\d{1,10}/
		if(padrao.exec(texto)!=texto){
			texto = texto.substring(0,texto.length-2);
		}
		switch(texto.length)
		{
		case 6:
			if(solcomplemento!=''){
				texto = jQuery('#codSCDP_bkp').val();
			}else{
				texto = texto.substring(0,6);
			}
			break;
		case 7:
			if(solcomplemento!=''){
				texto = jQuery('#codSCDP_bkp').val();
			}else{
				texto = texto.substring(0,6)+'/'+texto.substring(6,8);
			}
			break;
		case 8:
			if(solcomplemento!=''){
				texto = jQuery('#codSCDP_bkp').val()+'-'+jQuery('#codSCDP_num').val()+'C';
			}else{
				texto = texto.substring(0,6)+'/'+texto.substring(6,8);
			}
			break;
		case 9:
			if(solcomplemento!=''){
				texto = jQuery('#codSCDP_bkp').val()+'-'+jQuery('#codSCDP_num').val()+'C';
			}else{
				texto = texto.substring(0,6)+'/'+texto.substring(6,8);
			}
			break;
		case 10:
			if(solcomplemento!=''){
				texto = jQuery('#codSCDP_bkp').val()+'-'+jQuery('#codSCDP_num').val()+'C';
			}else{
				texto = texto.substring(0,6)+'/'+texto.substring(6,8);
			}
			break;
		}
		jQuery('#codSCDP').val(texto);
	});

	jQuery('#nome').addClass('obrigatorio');
	jQuery('#codSCDP').keyup();
	cpfBlur(jQuery('#cpf').val());
	calculaTotais();
	
});
</script>
<form method="post" name="formSolDia" id="formSolDia" enctype="multipart/form-data" action="">
<?php 
monta_cabecalho_ajuda_de_custo( $busca );
?>
	<input type="hidden" id="req" 			 name="req" 		   value=""/>
	<input type="hidden" id="solid" 		 name="solid" 		   value="<?=$solid ?>"/>
	<input type="hidden" id="solcomplemento" name="solcomplemento" value="<?=$solcomplemento ?>"/>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr id="trcpf" style="<?=$dados[0]['solnumdocestrangeiro'] != '' ? 'display:none' : '' ?>">
			<td class="SubTituloDireita">CPF</td>
			<td>
				<?php 
					$cpf = formatar_cpf($dados[0]['usucpf']); 
					echo campo_texto( 'cpf', 'S', $complemento ? 'N' : $disabled, '', 16, 15, '###.###.###-##', '','','','','id="cpf"' ); 
				?>
				<?php if($disabled=='S'&&!$complemento){?>
					<a id="trocacpf" style="color: blue">É estrangeiro?</a>
				<?php }?>
				<input type="hidden" id="clpid" nome="clpid" value="" />
				<input type="hidden" id="cpfbkp" nome="cpfbkp" value="<?=$cpf; ?>" />
			</td>
			<td rowspan="9" width="20%" valign="top">
				<?php 
					if($docid){
						echo '<div style="position:relative;margin-bottom:10xp;margin-right:5xp;float:right;">';
						wf_desenhaBarraNavegacao( $docid , array( 'solid' => $_REQUEST['solid'] ));
					    echo '</div><br />';
				 	}
				?>
			</td>
		</tr>
		<tr id="trnumdoc" style="<?=$dados[0]['solnumdocestrangeiro'] != '' ? '' : 'display:none' ?>">
			<td class="SubTituloDireita">N° do Documento:<br /> (Estrangeiro)</td>
			<td>
				<?php 
					$numdocestrangeiro = $dados[0]['solnumdocestrangeiro']; 
					echo campo_texto( 'numdocestrangeiro', 'N',$complemento ? 'N' : $disabled, '', 32, 30, '[#]', '','','','','id="numdocestrangeiro"' ); 
				?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				<?php if($disabled=='S'&&!$complemento){?>
				<a id="trocanumdoc" style="color: blue">Não é estrangeiro?</a>
				<?php }?>
				<input type="hidden" id="numdocestrangeirobkp" nome="numdocestrangeirobkp" value="<?=$numdocestrangeiro; ?>" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome</td>
			<td>
				<?php 
					$nome = $dados[0]['solnome']; 
					echo campo_texto( 'nome', 'S', ($dados[0]['solnumdocestrangeiro'] != '' ? $disabled : 'N'), '', 50, 200, '', '','','','','id="nome"' ); 
				?>
			</td>
		</tr>
		<tr <?=$dados[0]['soldeducaoalim']!='NULL'&&$dados[0]['soldeducaoalim']!=''&&$dados[0]['unidsc']=='' ? '' : 'style="display:none"'; ?> id="info">
			<td class="SubTituloDireita">Complemento</td>
			<td>
				<table align="center" bgcolor="#f5f5f5" border="0" style="border:0px;" class="tabela" width="100%" cellpadding="3" cellspacing="1">
					<tr>
						<td class="SubTituloCentro" colspan="2">Informações do Contemplado</td>
					</tr>
					<?php if($dados[0]['unidsc']!='NULL'){ ?>
					<tr>
						<td class="SubTituloDireita" width="15%">Unidade</td>
						<td>
							<label id="inforgao"><?=$dados[0]['unidsc']; ?></label>
							<input type="hidden" id="solorgaodsc" name="solorgaodsc" value="" />
							<input type="hidden" id="solorgaocod" name="solorgaocod" value="" />
						</td>
					</tr>
					<?php }?>
					<tr>
						<td class="SubTituloDireita" >Cargo/Emprego</td>
						<td>
							<label id="infcargo"><?=$dados[0]['cargo']; ?></label>
							<input type="hidden" id="solempregodsc" name="solempregodsc" value="" />
							<input type="hidden" id="solempregocod" name="solempregocod" value="" />
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Função</td>
						<td>
							<label id="inffuncao"><?=$dados[0]['solfuncao']; ?></label>
							<input type="hidden" id="solfuncaodsc" name="solfuncaodsc" value="" />
							<input type="hidden" id="solfuncaocod" name="solfuncaocod" value="" />
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Auxílio Alimentação (mês)</td>
						<td>
							<label id="infalimentacao">0,00</label>
							<input type="hidden" name="infvalalimentacao" id="infvalalimentacao" value="0"/>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Auxílio Transporte (mês)</td>
						<td>
							<label id="inftransporte">0,00</label>
							<input type="hidden" name="infvaltransporte" id="infvaltransporte" value="0"/>
							<input type="hidden" id="tptransp"  value="<?=$dados[0]['soldeducaotransp'] ? 'D' : 'I'; ?>"/>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">E-mail</td>
			<td>
				<?php 
					$email = $dados[0]['solemail'];
					echo campo_texto( 'email', 'S', $disabled, '', 40, 50, '', '','','','','id="email"' ); 
				?>
			</td>
		</tr>
		<tr style="display:none">
			<td class="SubTituloDireita">Tipo de Auxílio</td>
			<td>
			<?php 
				// Tipo de Auxilio
				$sql = "SELECT 
							tpaid as codigo, 
							tpadescricao as descricao 
						FROM 
							evento.tipoauxiliodiaria
						WHERE
							tpastatus = 'A'
							AND tpatipo = 'A'";
				$db->monta_combo('aux',$sql,$disabled,'Selecione...','','','','','N', 'aux', '', $dados[0]['tpaid'] );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Banco</td>
			<td>
				<?php 
					$banco = $dados[0]['solbanco']; 
					echo campo_texto( 'banco', 'S', $disabled, '', 2, 3, '###', '','','','','id="banco"' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Agência</td>
			<td>
				<?php 
					$agencia = $dados[0]['solagencia']; 
					echo campo_texto( 'agencia', 'S', $disabled, '', 8, 10, '', '','','','','id="agencia"' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Conta Corrente</td>
			<td>
				<?php 
					$conta = $dados[0]['solconta']; 
					echo campo_texto( 'conta', 'S', $disabled, '', 18, 20, '', '','','','','id="conta"' );  
				?>
			</td>
		</tr>
		<tr id="trSaecod" <?=$dados[0]['soldemcod'] ? '' : 'style="display:none"'; ?>>
			<td class="SubTituloDireita">Demanda SAE</td>
			<td>
				<div id="divSaecod" >
				<?php 
					$saecod = $dados[0]['soldemcod'];
					if( $saecod != '' ){
						$sql = "SELECT DISTINCT
									nu_demanda as codigo, 
									ds_demanda as descricao
								FROM
									evento.sae_ajudadecusto
								WHERE
									nu_demanda = ".$saecod;
				
					$db->monta_combo('saecod',$sql,'S','Selecione...','','','','','N', 'saecod');
					}
				?>
				</div>
			</td>
		</tr>
		<?php 
				$arPerfisSolicitador = array( EVENTO_PERFIL_AGENTE_FINANCEIRO );
	
				if( possuiPerfil( $arPerfisSolicitador ) ){ ?>
			<?php if(in_array($esdid,Array(WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO,WF_SOLICITAÇÃO_DIARIAS_PAGO))){?>
		<tr>
			<td class="SubTituloDireita">Ordem bancária</td>
			<td>
				<?php 
					$ordembancaria = $dados[0]['solordembancaria']; 
					echo campo_texto( 'ordembancaria', ($esdid == WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO ? 'S' : 'N'), ($esdid == WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO ? 'S' : $disabled), '', 15, 12, '', '','','','','id="ordembancaria"' ); 
				?>
			</td>
		</tr>
			<?php } ?>
		<?php } ?>
		<?php 
				$arPerfisSolicitador = array( EVENTO_PERFIL_AGENTE_FINANCEIRO );
	
				if( possuiPerfil( $arPerfisSolicitador ) ){ ?>
			<?php if(in_array($esdid,Array(WF_SOLICITAÇÃO_AGUARDANDO_DEVOLUCAO,WF_SOLICITAÇÃO_DEVOLUCAO_EFETUADA))){?>
		<tr>
			<td class="SubTituloDireita">Ordem bancária</td>
			<td>
				<?php 
					$solgru = $dados[0]['solgru']; 
					echo campo_texto( 'solgru', ($esdid == WF_SOLICITAÇÃO_AGUARDANDO_DEVOLUCAO ? 'S' : 'N'), ($esdid == WF_SOLICITAÇÃO_AGUARDANDO_DEVOLUCAO ? 'S' : $disabled), '', 15, 12, '', '','','','','id="solgru"' ); 
				?>
			</td>
		</tr>
			<?php } ?>
		<?php } ?>
		<tr>
			<td class="SubTituloDireita">Possui passagem?</td>
			<td>
				<input type="radio" value="S" name="passagem" <?=$block||$complemento ? 'disabled="disabled"' : '' ?> id="passagemS" <?=$dados[0]['solcodigoscdb'] != '' ? 'checked="checked"' : ''; ?>> Sim &nbsp
				<input type="radio" value="N" name="passagem" <?=$block||$complemento ? 'disabled="disabled"' : '' ?> id="passagemN" <?=$dados[0]['solcodigoscdb'] != '' ? '' : 'checked="checked"'; ?>> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Código SCDP</td>
			<td>
				<?php 
					$codSCDP = $dados[0]['solcodigoscdb']; 
					$obrig = $codSCDP != '' ? '' : 'style="display:none"';
					echo campo_texto( 'codSCDP', 'N', $complemento ? 'N' : $disabled , '', 25, 20, '', '','','','','id="codSCDP"' ); 
				?>
				<input type="hidden" id="codSCDP_bkp" name="codSCDP_bkp" value="<?=$codSCDP ?>" />
				<input type="hidden" id="codSCDP_num" name="solcompnum" value="<?=$dados[0]['solcompnum'] ?>" />
				<img id="imgcodSCDP" <?=$obrig ?> border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Observações</td>
			<td>
				<?php 
					$obs = $dados[0]['solobservacao']; 
					echo  campo_textarea( 'obs', 'N', $disabled, $label, 100, 5, '230', $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $obs); 
				?>
				<img id="obsObrImg" style="display:none" border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Objetivo da Viagem</td>
			<td>
				<?php 
					$obj = $dados[0]['solobjetivo']; 
					echo  campo_textarea( 'obj', 'N', $disabled, $label, 100, 5, '230', $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $obj); 
				?>
				<img id="obsObrImg" style="display:none" border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
			<td rowspan="2" valign="bottom" width="20%">
				<?php 
				if($solcomplemento!=''){
					$sql = "SELECT DISTINCT
								SUM(
								(iti.itidias*(iti.itivalordiaria*(CASE WHEN iti.itimeiadiaria THEN 0.5 ELSE 1 END) ) )
								+
								(iti.itivaloradicionalembarque )
								+
								(CASE WHEN iti.itiultimo THEN (iti.itivalordiaria/2) ELSE 0 END )
								) as valor,
								(soldeducaoalim + ( soldeducaotransp * soltotdias ) ) as deducao
							FROM 
								evento.itinerariosolicitacaodiaria iti
							INNER JOIN evento.solicitacaodiaria sol ON sol.solid = iti.solid
							WHERE
								itistatus = 'A' AND iti.solid = ".$solcomplemento."
							GROUP BY
								soldeducaoalim,
								soldeducaotransp,
								soltotdias";
					 $dadosComp = $db->pegaLinha($sql);
					 $vlrcomplemento = $dadosComp['valor']-$dadosComp['deducao'];
				?>
				<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" width="100%" cellpadding="3" cellspacing="1">
					<tr>
						<td class="SubTituloCentro" colspan="2">Complemento</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Valor do Complemento</td>
						<td>
							R$ <label class="complemento" id="lbvlrcomplemento">0,00</label>
							<input class="complemento" type="hidden" id="vlrcomplemento"/>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="50%">Valor Solicitação Original</td>
						<td style="color:red">
							- R$ <label class="complemento" id="lbvlrorioginal"><?=str_replace('.',',',number_format($vlrcomplemento,2)) ?></label>
							<input class="complemento" type="hidden" id="vlrorioginal" value="<?=$vlrcomplemento ?>"/>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita"><b>Valor a ser complementado</b></td>
						<td>
							R$ <label class="complemento" id="lbcomptotal">0,00</label>
							<input class="complemento" type="hidden" id="comptotal"/>
						</td>
					</tr>
				</table>
				<br />
				<?php }?>
				<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" width="100%" cellpadding="3" cellspacing="1">
					<tr>
						<td class="SubTituloCentro" colspan="2">Deduções</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Valor Itinerário</td>
						<td>
							R$ <label class="deducoes" id="totitinerario">0,00</label>
							<input class="deducoes" type="hidden" id="totiti"/>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="50%">Dedução Alimentação</td>
						<td style="color:red">
							- R$ <label class="deducoes" id="auxalimentacao">0,00</label>
							<input class="deducoes" type="hidden" id="auxalim" name="auxalim" value="0"/>
							<input class="deducoes" type="hidden" id="boolalim" value="<?=$dados[0]['soldeducaoalim'] ? '1' : '0'; ?>"/>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Dedução Transporte</td>
						<td style="color:red;border-bottom: 1px solid black;">
							- R$ <label class="deducoes" id="auxtransporte">0,00</label>
							<input class="deducoes" type="hidden" id="auxtransp" name="auxtransp" value="0"/>
							<input class="deducoes" type="hidden" id="booltransp" value="<?=$dados[0]['soldeducaotransp'] ? '1' : '0'; ?>"/>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita"><b>Valor Total da Ajuda de Custo</b></td>
						<td>
							R$ <label class="deducoes" id="auxtotal">0,00</label>
							<input class="deducoes" type="hidden" id="auxtot"/>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Anexo:</td>
			<td>
				<?php 
				
				
				if($_REQUEST['solid']){
					
					$sql = "SELECT 
								arqid
							FROM 
								evento.anexosolicitacao
							WHERE
								axsstatus = 'A'
								AND solid = ".$_REQUEST['solid'];
					$arqids = $db->carregarColuna($sql);
					
					if(count($arqids)>0){
						foreach($arqids as $arqid){
							$sql = "SELECT 
										arqnome
									FROM 
										public.arquivo
									WHERE
										arqid = ".$arqid;
							$arqnome = $db->pegaUm($sql);
				?>
							<a class="arquivo" id="<?=$arqid; ?>" style="color:blue"><?=$arqnome; ?></a>
							<?php if(!$block){ ?>
							<img border="0" src="../imagens/refresh2.gif" title="Trocar anexo." class="excluirAnexo" id="_2">
							<br />
							<input type="file" class="normal" style="display:none" id="arquivo" name="arquivo"/> 
							<img border="0" src="../imagens/refresh2_01.gif" style="display:none" title="Não trocar anexo." class="naoExcluirAnexo" id="_2"> 
							<?php }?>
				<?php
						}
					}else{
				?>
				<?php if(!$block){ ?>
					<input type="file" class="normal" id="arquivo" name="arquivo"/> 
				<?php }?>
				<?php 
					}
				 }else{?>
				 <?php if(!$block){ ?>
					<input type="file" class="normal" id="arquivo" name="arquivo"/> 
				<?php }?>
				<?php }?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="3">Itinerário</td>
		</tr>
		<tr>
			<td colspan="3">
				<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="movimentacao">
					<tr>
						<td class="SubTituloDireita" valign="bottom" style="text-align: center">Escala/Conexão</td>
						<td class="SubTituloDireita" valign="bottom" style="text-align: center">Rodoviário?</td>
						<td class="SubTituloDireita" valign="bottom" style="text-align: center">Origem/Partida</td>
						<td class="SubTituloDireita" valign="bottom" style="text-align: center">Destino/Chegada</td>
						<td class="SubTituloDireita" valign="bottom" style="text-align: center">Permanência</td>
						<td class="SubTituloDireita" valign="bottom" style="text-align: center">Valor Ajuda de Custo<br />(A)</td>
						<td class="SubTituloDireita" valign="bottom" style="text-align: center">Adicional por Embarque<br />(B)</td>
						<td class="SubTituloDireita" valign="bottom" style="text-align: center">Valor Total(R$)<br />(A+B)</td>
						<td class="SubTituloDireita" valign="bottom" style="text-align: center"> Adicionar/Excluir </td>
					</tr>
					<tr style="background-color: rgb(230,230,230)">
						<td align="center">
							<?php 
								// Escala/Conexão
								$sql = Array(Array('codigo'=>1,'descricao'=>'Sim'),
									 		 Array('codigo'=>0,'descricao'=>'Não'));
								$db->monta_combo('escala',$sql,$disabled,'','','','','','N', 'escala','', '');
							?>
							<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
						</td>
						<td align="center">
							<input type="checkbox" value="R" <?=$block ? 'disabled="disabled"' : '' ?> name="BTrodoviario" id="BTrodoviario">
						</td>
						<td align="center">
							<?php 
								// Origem
								$sql = "SELECT 
											muncod as codigo,
											mundescricao || ' - ' || estuf as descricao
										FROM
											territorios.municipio
										WHERE
											1=1
										ORDER BY 2";
								if($_REQUEST['solid']||$_REQUEST['solcomplemento']){
									$muncodDestino   = $dadosItinerario[count($dadosItinerario)-1]['itidestino'];
									$munDestino  	 = descricaoMun($muncodDestino);
									$origemI['value']	 = $muncodDestino;
									$origemI['descricao'] = $munDestino;
								}
								campo_popup('origemI',$sql,'Selecione','','400x800','20', Array( 0 =>Array('descricao'=>'UF', 'codigo'=>'estuf'), 1 =>Array('descricao'=>'Município', 'codigo'=>'mundescricao')), 1, false, false, '', '', Array('class'=>'campo_popup_autocomplete'));
							?>
							<?= campo_data2( 'dataOrigem', 'N', $disabled, '', 'S', '', '', '', "jQuery('#dataOrigem').change()" ); ?>
							<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
						</td>
						<td align="center">
							<?php 
								// Destino
								$sql = "SELECT 
											muncod as codigo,
											mundescricao || ' - ' || estuf as descricao
										FROM
											territorios.municipio
										WHERE
											1=1
										ORDER BY 2";
								campo_popup('destinoI',$sql,'Selecione','','400x800','20', Array( 0 =>Array('descricao'=>'UF', 'codigo'=>'estuf'), 1 =>Array('descricao'=>'Município', 'codigo'=>'mundescricao')), 1, false, $block, '', '', Array('class'=>'campo_popup_autocomplete','whereAuto'=>'mundescricao'));
							?>
							<?= campo_data2( 'dataDestino', 'N', $disabled, '', 'S', '', '', '', "jQuery('#dataDestino').change()" ); ?>
							<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
						</td>
						<td align="center">
							-
						</td>
						<td align="center">
							<?php 
								// Diaria 50%/100%
								$sql = Array(Array('codigo'=>'0.5','descricao'=>'50%'),
									 		 Array('codigo'=>'1','descricao'=>'100%'));
								$db->monta_combo('percdia',$sql,$disabled,'Selecione...','','','','','N', 'percdia','','');
							?>
							<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
						</td>
						<td align="center">
							<input type="checkbox" value="S" <?=$block ? 'disabled="disabled"' : '' ?> name="adicional" id="adicional">
						</td>
						<td align="center">
							-
						</td>
						<td align="center">
							<?=$block ? '' : '<input type="button" id="AddMov" value="Adicionar" />' ?> 
						</td>
					</tr>
					<?php 
					
					$totDias = 0;
					$totValorDiarias = 0;
					$totAdicionalPernoite = 0;
					$totAdicionalEmb = 0;
					$totValor = 0;
					
					if(is_array($dadosItinerario)){
						$qtd = 0;
						$html = '';
						$final = count($dadosItinerario);
						$temfinal = false;
						
						foreach($dadosItinerario as $itinerario){
							$qtd++;
//							ver($itinerario);
							$escala 	 	 = $itinerario['itiescala'] == 't' ? 'Escala' : ''; 
							$escalaBool  	 = $itinerario['itiescala'] == 't' ? '1' : '0';
							$rod 	 	 	 = $itinerario['itirodoviario'] == 'R' ? 'R' : ''; 
							$rodoviario 	 = $itinerario['itirodoviario'] == 'R' ? 'Rodoviário' : '';
							$munOrigem   	 = descricaoMun($itinerario['itiorigem']);
							$dataOrigem  	 = preparaData($itinerario['itidataorigem'],'-');
							$munDestino  	 = descricaoMun($itinerario['itidestino']);
							$dataDestino 	 = preparaData($itinerario['itidatadestino'],'-');
							$ultimoDisplay   = $itinerario['itiultimo'] == 't' ? '' : 'display:none';
							$percdia	     = $itinerario['itimeiadiaria'] == 't' ? 0.5 : 1;
							$meia		     = $itinerario['itimeiadiaria'] == 't' ? '(meia)' : '';
							$valorMeia	     = $itinerario['itimeiadiaria'] == 't' ? $itinerario['itivalordiaria']/2 : $itinerario['itivalordiaria'];
							$valorUltimo     = $itinerario['itiultimo'] == 't' ? $itinerario['itivalordiaria']/2 : 0;
							$adicionalViajem = $itinerario['itidiasviajem']*$itinerario['itivalordiaria'];
							$valor			 = ($itinerario['itidias']*$valorMeia)+$valorUltimo+$itinerario['itivaloradicionalembarque']+$itinerario['itivaloradicionalper'];
							
							$totDias     		  += $itinerario['itidias'];
							$totValorDiarias 	  += (($valorMeia*$itinerario['itidias'])+($valorUltimo));
							$totAdicionalPernoite += $adicionalViajem;
							$totAdicionalEmb 	  += $itinerario['itivaloradicionalembarque'];
							$totValor 			  += $valor;
							
							$temfinal = $itinerario['itiultimo'] == 't' ? true : $temfinal;
							
							$html .= '<tr style="background-color: rgb(240,240,240)" id="linha'.$qtd.'" name="linha" class="itinerario">'.
										'<td align="center"> '.
											'<label id="lEscala'.$qtd.'">'.$escala.'</label>'.
											'<input type="hidden" id="escala'.$qtd.'"  name="escala[]" value="'.$escalaBool.'"/>'.
										'</td>'.
										'<td align="center"> '.
											'<label id="lRodoviario'.$qtd.'">'.$rodoviario.'</label>'.
											'<input type="hidden" id="rodoviario'.$qtd.'"  name="rodoviario[]" value="'.$rod.'"/>'.
										'</td>'.
										'<td align="center"> '.
											$munOrigem.'<br />'.
											'<input type="hidden" id="origem'.$qtd.'" name="origem[]" value="'.$itinerario['itiorigem'].'"/>'.
											'<label id="lDataOrigem'.$qtd.'">'.$dataOrigem.'</label>'.
											'<input type="hidden" id="dataOrigem'.$qtd.'"  name="dataOrigem[]" value="'.$dataOrigem.'"/>'.
										'</td>'.
										'<td align="center">'.
											'<label id="lDestino'.$qtd.'">'.$munDestino.'</label><br />'.
											'<input type="hidden" id="destino'.$qtd.'" name="destino[]" value="'.$itinerario['itidestino'].'"/>'.
											'<label id="lDataDestino'.$qtd.'">'.$dataDestino.'</label>'.
											'<input type="hidden" id="dataDestino'.$qtd.'"  name="dataDestino[]" value="'.$dataDestino.'"/>'.
										'</td>'.
										'<td align="center">'.
											'<label id="lDias'.$qtd.'">'.$itinerario['itidias'].'</label> Diárias '.
											'<label id="lDiasUlt'.$qtd.'" style="color:red;'.$ultimoDisplay.'">(+0.5)</label> '.
											'<input type="hidden" class="dias" id="dias'.$qtd.'" name="dias[]" value="'.$itinerario['itidias'].'"/>'.
											'<input type="hidden" class="diasviajem" id="diasviajem'.$qtd.'" name="diasviajem[]" value="'.$itinerario['itidiasviajem'].'"/>'.
										'</td>'.
										'<td align="center"> '.
											'R$ <label id="lValorDiaria'.$qtd.'" title="Valor da Diária'.$meia.':'.str_replace('.',',',$valorMeia).'">'.str_replace('.',',',(number_format(($valorMeia*$itinerario['itidias'])+($valorUltimo),2))).'</label>'.
											'<input type="hidden" id="valorDiaria'.$qtd.'" name="valorDiaria[]" value="'.$itinerario['itivalordiaria'].'"/>'.
											'<input type="hidden" id="valorDiarias'.$qtd.'" class="valorDiarias" value="'.(($valorMeia*$itinerario['itidias'])+($valorUltimo)).'"/>'.
											'<input type="hidden" id="vldid'.$qtd.'" name="vldid[]" value="'.$itinerario['vldid'].'"/>'.
											'<input type="hidden" id="meiadiaria'.$qtd.'" name="meiadiaria[]" value="'.$percdia.'"/>'.
										'</td>'.
										'<td align="center">'.
											'R$ <label id="lEmbarque'.$qtd.'">'.str_replace('.',',',number_format($itinerario['itivaloradicionalembarque'],2)).'</label> '.
											'<input type="hidden" class="adicional" id="embarque'.$qtd.'" name="embarque[]" value="'.$itinerario['itivaloradicionalembarque'].'"/>'.
											'<input type="hidden" class="boolAdicional" id="boolEmbarque'.$qtd.'" name="boolEmbarque[]" value="'.($itinerario['itiadicionalemb']=='t' ? 1 : 0).'"/>'.
										'</td>'.
										'<td align="center"> '.
											'R$ <label id="lValor'.$qtd.'">'.str_replace('.',',',number_format($valor,2)).'</label> '.
											'<input type="hidden" class="valor" id="valor'.$qtd.'" name="valor[]" value="'.$valor.'"/>'.
										'</td>'.
										'<td align="center"> '.
											(!$block&&!$complemento 
												? '<img id="'.$qtd.'" class="excluir" '.( $final == $qtd ? '' : 'style="display:none"' ).'border="0" title="Remover itinerário." src="../imagens/excluir.gif">' 
												: ($complemento && $temfinal ) ? '<img id="'.$qtd.'" class="excluir" '.( $final == $qtd ? '' : 'style="display:none"' ).'border="0" title="Remover itinerário." src="../imagens/excluir.gif">' : '' ).
											'<input type="hidden" id="'.$qtd.'" class="qtd" />'.
											'<input type="hidden" name="ultimo[]" value="'.( $itinerario['itiultimo'] == 't' ? 'TRUE' : 'FALSE' ).'"/>'.
										'</td>'.
									'</tr>';
						}
						echo $html;
					}
					
					?>
					<tr id="bordainferior" style="background-color: rgb(215,215,215)">
						<td class="SubTituloDireita" valign="bottom" colspan="9"> </td> 
					</tr>
					<tr style="background-color: rgb(230,230,230)">
						<td class="SubTituloDireita" valign="bottom" colspan="4" style="font-weight: bold;"> Total:</td> 
						<td align="center" style="font-weight: bold;"> <label id="lTotDias"><?=$totDias ?></label> Diárias <input type="hidden" id="totDias" name="totDias" value="<?=$totDias ?>"/><input type="hidden" id="totDiasSfds" name="totDiasSfds" value="<?=$dados[0]['soltotdias'] ?>"/></td>
						<td align="center" style="font-weight: bold;"> <label id="totValorDiarias">R$ <?=str_replace('.',',',number_format($totValorDiarias,2)) ?></label> <!--  <input type="hidden" id="totDias" name="totDias" value="<?=$totValorDiarias ?>"/> --></td>
						<td align="center" style="font-weight: bold;"> R$ <label id="lTotAdicional"><?=str_replace('.',',',number_format($totAdicionalEmb,2)) ?></label> <input type="hidden" id="totAdicional" name="totAdicional" value="<?=$totAdicionalEmb ?>"/></td>
						<td align="center" style="font-weight: bold;"> R$ <label id="lTotValor"><?=str_replace('.',',',number_format($totValor,2)) ?></label> <input type="hidden" id="totValor" name="totValor" value="<?=$totValor ?>"/></td>
						<td align="center" style="font-weight: bold;"> - </td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="3" class="SubTituloDireita" style="text-align:center">
				<div style="float:left">
					<input type="button" id="voltar" value="Voltar para a lista" />	
				</div>
				<?php 
				$arPerfisSolicitador = array( EVENTO_PERFIL_SOLICITADOR );
			
				if( possuiPerfil( $arPerfisSolicitador ) && (in_array($esdid,array(WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO)) || $esdid=='' ) ){ ?>
					<?php if($_REQUEST['solid']){?>
						<?php if(in_array($esdid,array(WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO))){?>
					<input type="button" id="salvar" value="Gravar Alterações" />
						<?php }?>
					<div style="float:right">
						<input type="button" id="novo" value="Inserir novo." />	
					</div>
					<?php }else{?>
					<input type="button" id="salvar" value="Salvar" />
					<?php }?>	
				<?php }elseif($esdid==''){?>
					<input type="button" id="salvar" value="Salvar" />
				<?php }?>	
				<?php 
				$arPerfisSolicitador = array( EVENTO_PERFIL_AGENTE_FINANCEIRO );
	
				if( possuiPerfil( $arPerfisSolicitador ) && in_array($esdid,array(WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO)) ){ ?>
					<?php if($_REQUEST['solid']){?>
						<?php if(in_array($esdid,array(WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO))){?>
					<input type="button" id="salvar" value="Gravar Alterações" />
						<?php }?>
					<div style="float:right">
						<input type="button" id="novo" value="Inserir novo." />	
					</div>
					<?php }else{?>
					<input type="button" id="salvar" value="Salvar" />
					<?php }?>	
				<?php }
				if( possuiPerfil( $arPerfisSolicitador ) && in_array($esdid,array(WF_SOLICITAÇÃO_DIARIAS_PAGO)) ){ ?>
<!--					<div style="float:right">-->
<!--						<input type="button" class="complementarSol" id="complementar" value="Complementar Solicitação" />-->
<!--					</div>-->
				<?php }?>
			</td>
		</tr>
	</table>
</form>