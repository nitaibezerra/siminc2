<?php

$conf = array(ENEM_EST_EM_VALIDACAO 	=> array('tpvid' => '2', 'descricao' => 'validar', 'vldsituacao_TRUE' => ENEM_AEDID_VALIDAR, 'vldsituacao_FALSE' => ENEM_AEDID_INVALIDAR, 'vldsituacao_FINALIZAR' => ENEM_AEDID_VLFINALIZAR),
			  ENEM_EST_EM_EXECUCAO  	=> array('tpvid' => '1', 'descricao' => 'executar','vldsituacao_TRUE' => ENEM_AEDID_EXECUTAR, 'vldsituacao_FALSE' => false, 'vldsituacao_FINALIZAR' => ENEM_AEDID_EXFINALIZAR),
			  ENEM_EST_EM_CERTIFICACAO  => array('tpvid' => '3', 'descricao' => 'certificar','vldsituacao_TRUE' => ENEM_AEDID_CERTIFICAR, 'vldsituacao_FALSE' => ENEM_AEDID_NAOCERTIFICAR));


			  
			  

function downloadArquivo($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("anexochecklist", $arrCampos = array(), "pde");
	$file->getDownloadArquivo($dados['arqid']);
}
			  
function salvarItem($dados) {
	global $db, $conf;
	
	include_once APPRAIZ . 'includes/workflow.php';
	
	$entidUsuario = $db->pegaUm("SELECT entid FROM pde.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '".$_SESSION['usucpf']."' AND entid is not null");
	if( !$entidUsuario ) {
		$entidUsuario = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj = '".$_SESSION['usucpf']."' AND entstatus = 'A'");
	}else{
//		$sql = "SELECT DISTINCT
//					usu.usuemail
//				FROM 
//					pde.usuarioresponsabilidade rpu
//				INNER JOIN seguranca.usuario usu ON usu.usucpf = rpu.usucpf
//				WHERE 
//					rpustatus = 'A' 
//					AND rpu.entid in (".$entidUsuario.")";
//		$arrEmail = $db->carregarColuna($sql);
	}
	
	$arrwf = $db->pegaLinha("SELECT d.esdid, d.docid FROM pde.itemchecklist i 
						     INNER JOIN workflow.documento d ON i.docid = d.docid 
						     WHERE iclid='".$dados['iclid']."'");
				  
	$sql = "SELECT vldid FROM pde.validacao 
			WHERE entid='".$entidUsuario."' AND tpvid='".$conf[$arrwf['esdid']]['tpvid']."' AND iclid='".$dados['iclid']."'";
	
	$vldid = $db->pegaUm($sql);
	
	if($dados['vldsituacao'] == "TRUE") {
		$tpviddestino = $conf[$arrwf['esdid']]['tpvid']+1;
	} else {
		$tpviddestino = $conf[$arrwf['esdid']]['tpvid']-1;
	}
	
	$sql = "SELECT ent.entid, usu.usuemail, usu.usunome FROM pde.checklistentidade cle 
			INNER JOIN entidade.entidade ent ON ent.entid = cle.entid 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = ent.entnumcpfcnpj 
			WHERE cle.iclid = ".$dados['iclid']." AND cle.tpvid = ".$tpviddestino;
	
	$dadosusu = $db->pegaLinha($sql);
	if( !$dadosusu['usuemail'] ) {
		$sql = "SELECT 
					usu.usuemail
				FROM 
					pde.usuarioresponsabilidade rpu
				INNER JOIN seguranca.usuario usu ON usu.usucpf = rpu.usucpf
				WHERE 
					rpustatus = 'A' 
					AND rpu.entid in (".$entidUsuario.")";
		$arrEmail = $db->carregarColuna($sql);
	}
	
	if($vldid) {
		
		$db->executar("UPDATE pde.validacao 
					   SET vldsituacao = ".$dados['vldsituacao'].", vlddata = now(), vldobservacao = '".pg_escape_string($dados['vldobservacao'])."'
					   WHERE iclid = ".$dados['iclid']." AND tpvid = ".$conf[$arrwf['esdid']]['tpvid']." AND entid = ".$entidUsuario);
		
	} else {
		
		$vldid = $db->pegaUm("INSERT INTO pde.validacao(
					            iclid, tpvid, entid, vldsituacao, vlddata, vldobservacao)
    						  VALUES ('".$dados['iclid']."', '".$conf[$arrwf['esdid']]['tpvid']."', '".$entidUsuario."', ".$dados['vldsituacao'].", now(), '".pg_escape_string($dados['vldobservacao'])."') RETURNING vldid;");
		
	}
	
	$db->commit();
	
	if( $_FILES['arquivo']['name'] ) {
		
		if( $vldid ) {
			
			$sql = "SELECT ancid FROM pde.anexochecklist WHERE vldid = ".$vldid." AND ancstatus = 'A'";
			$ancid = $db->pegaUm($sql);
			
			if( $ancid ) {
				$sql = "UPDATE pde.anexochecklist SET ancstatus = 'I' WHERE ancid = ".$ancid;
				$db->executar($sql);
				$db->commit();
			}
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$arrCampos = array("vldid" => $vldid);
			$file = new FilesSimec("anexochecklist", $arrCampos, "pde");
				
			$arqdescricao = 'arquivo_checklist_enem_'.$vldid.'_'.date('Ymdhis');
			$file->setUpload($arqdescricao, "arquivo");
				
		}
	}
	
	$atiid = $db->pegaUm("SELECT atiid FROM pde.itemchecklist WHERE iclid='".$dados['iclid']."'");
	
	// codigo especifico para atender a regra de finalização
	if($dados['vldsituacao'] == "TRUE" && ($conf[$arrwf['esdid']]['tpvid']+1) < 4) {
		$sql = "SELECT etcid FROM pde.etapascontrole WHERE iclid='".$dados['iclid']."' AND tpvid='".($conf[$arrwf['esdid']]['tpvid']+1)."'";
		$etcid = $db->pegaUm($sql);
		if(!$etcid) $dados['vldsituacao'] = "FINALIZAR";
		
	}
	
	
	$aedid = $conf[$arrwf['esdid']]['vldsituacao_'.$dados['vldsituacao']];
			
	if( $aedid ) {
		if($dados['vldsituacao'] == "FALSE") {
			$comentario = $dados['vldobservacao'];
		}
		wf_alterarEstado( $arrwf['docid'], $aedid, $comentario, $dados );
	}
	
	// verificando se todos os item estão finalizados
	// verificando se existir algum item que tenha sido tramitado do estado inicial, atualizar andamento da atividade para 10%
	if($atiid) {
		
		$sql = "SELECT d.esdid FROM pde.itemchecklist i
				INNER JOIN workflow.documento d ON d.docid = i.docid 
				WHERE atiid = '".$atiid."'";
		
		$estados = $db->carregarColuna($sql);
		
		$todositensfinalizados = true;
		$itensemandamento = 0;
		if($estados) {
			foreach($estados as $estado) {
				if($estado != ENEM_EST_EM_EXECUCAO) $itensemandamento++;
				if($estado != ENEM_EST_EM_FINALIZADO) $todositensfinalizados = false;
			}
			if($todositensfinalizados) $db->executar("UPDATE pde.atividade SET esaid=5, atiporcentoexec='100',atidataconclusao=NOW() WHERE atiid='".$atiid."'");
			elseif($itensemandamento > 0) $db->executar("UPDATE pde.atividade SET esaid=2, atiporcentoexec='10' WHERE atiid='".$atiid."'");
		}
	}
	
	$db->commit();
	
	
	if( $dadosusu['usuemail'] || is_array($arrEmail) ) {
		
		// Email para o validador ou certificador informando que a fase anterior do checklist foi concluída:
		if(($tpviddestino == '2' || $tpviddestino == '3') && $dados['vldsituacao'] == 'TRUE') {
			
			$html .= "<p>Prezado (a) ".$dadosusu['usunome'].",</p>";
	 		$html .= "<p>Informamos que o item do checklist abaixo foi concluído e está pendente de sua validação no módulo ENEM do sistema SIMEC:</p>";
			$html .= "<p>";
			$html .= "<table>";
			$html .= "<tr><td>Item</td><td>Descrição</td><td>Prazo</td></tr>";
			
			$sql = "SELECT * FROM pde.itemchecklist WHERE iclid='".$dados['iclid']."'";
			$itemchecklist = $db->pegaLinha($sql);
			
			$html .= "<tr><td>".$itemchecklist['iclid']."</td><td>".$itemchecklist['icldsc']."</td><td>".(($itemchecklist['iclprazo']>=date("Y-m-d"))?$itemchecklist['iclprazo']:"<font color=red>".$itemchecklist['iclprazo']."</font>")."</td></tr>";
			$html .= "</table>"; 
			$html .= "</p>";
			$html .= "<p>* Os itens cujo o prazo está em vermelho estão em atraso, necessitando de ação urgente.</p>";
			$html .= "<p>Para concluí-los, acesse o módulo ENEM no endereço <a href=http://simec.mec.gov.br>http://simec.mec.gov.br</a>, no menu \"Principal > Minhas Pendências\", e clique no ícone <img align=absmiddle src=../../imagens/valida2.gif></p>";
		}
		
			// Email para o executor, validador informando que o item foi invalidado
		if(($tpviddestino == '1' || $tpviddestino == '2') && $dados['vldsituacao'] == 'FALSE') {
			
			$html .= "<p>Prezado (a) ".$dadosusu['usunome'].",</p>";
	 		$html .= "<p>Informamos que o item do checklist abaixo foi invalidado.<br/>Favor acessar o módulo ENEM do sistema SIMEC para realizar nova execução:</p>";
			$html .= "<p>";
			$html .= "<table>";
			$html .= "<tr><td>Item</td><td>Descrição</td><td>Prazo</td><td>Justificativa</td></tr>";
			
			$sql = "SELECT * FROM pde.itemchecklist WHERE iclid='".$dados['iclid']."'";
			$itemchecklist = $db->pegaLinha($sql);
			
			$html .= "<tr><td>".$itemchecklist['iclid']."</td><td>".$itemchecklist['icldsc']."</td><td>".(($itemchecklist['iclprazo']>=date("Y-m-d"))?$itemchecklist['iclprazo']:"<font color=red>".$itemchecklist['iclprazo']."</font>")."</td><td>".pg_escape_string($dados['vldobservacao'])."</td></tr>";
			$html .= "</table>"; 
			$html .= "</p>";
			$html .= "<p>* Os itens cujo o prazo está em vermelho estão em atraso, necessitando de ação urgente.</p>";
			$html .= "<p>Para concluí-los, acesse o módulo ENEM no endereço <a href=http://simec.mec.gov.br>http://simec.mec.gov.br</a>, no menu \"Principal > Minhas Pendências\", e clique no ícone <img align=absmiddle src=../../imagens/valida2.gif></p>";
		}

		$arrEmail 	= is_array($arrEmail) ? $arrEmail : array($dadosusu['usuemail']);
		$titulo		= "SIMEC - ENEM - Aviso de validação/invalidação de item de checklist";
		require_once(APPRAIZ . 'includes/classes/EmailAgendado.class.inc');
		
		if($html) {
			$e = new EmailAgendado();
			$e->setTitle($titulo);
			$e->setText($html);
			$e->setName("SIMEC");
			$e->setEmailOrigem("simec@mec.gov.br");
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();
		}

	}

	die("<script>
			alert('Dados registrados com sucesso');
			window.location='enem.php?modulo=principal/atividade_enem/minhasPendencias&acao=A';
		 </script>");
	
}


function telaFluxoEnem($dados) {
	global $db, $conf;
	
	$sql = "SELECT atv._atinumero, doc.esdid, atv.atidescricao, icl.icldsc, to_char(icl.iclprazo, 'dd/mm/YYYY') as iclprazo, icl.iclid FROM pde.itemchecklist icl
			INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
			INNER JOIN pde.atividade atv ON icl.atiid = atv.atiid 
			WHERE icl.docid = '".$dados['docid']."'";
	
	
	$arrItem = $db->pegaLinha($sql);
	
	echo "<form method=post id=formulario enctype=multipart/form-data>";
	echo "<input type=hidden name=requisicao value=salvarItem>";
	echo "<input type=hidden name=iclid value=".$arrItem['iclid'].">";
	echo "<table class=tabela width=100% cellspacing=2 cellpadding=3 align=center>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Atividade:</td>";
	echo "<td>".$arrItem['_atinumero']." - ".$arrItem['atidescricao']."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Item:</td>";
	echo "<td>".$arrItem['iclid']." - ".$arrItem['icldsc']."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Prazo:</td>";
	echo "<td>".$arrItem['iclprazo']."</td>";
	echo "</tr>";
	
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Deseja ".$conf[$arrItem['esdid']]['descricao']." este item?</td>";
	echo "<td><input type=radio name=vldsituacao id=vldsituacao_TRUE value=TRUE checked> Sim <input type=radio name=vldsituacao id=vldsituacao_FALSE value=FALSE> Não</td>";	
	echo "</tr>";
	
	$arrEtcopcaoevidencia = $db->pegaLinha("SELECT etcopcaoevidencia, etcevidencia FROM pde.etapascontrole WHERE iclid='".$arrItem['iclid']."' AND tpvid = '".$conf[$arrItem['esdid']]['tpvid']."'");
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>".(($arrEtcopcaoevidencia['etcevidencia'])?$arrEtcopcaoevidencia['etcevidencia']:"Anexo").":</td>";
	echo "<td><input type=file id=arquivo name=arquivo />".(($arrEtcopcaoevidencia['etcopcaoevidencia']=='t')?"<input type=hidden name=etcopcaoevidencia id=etcopcaoevidencia value=sim>":"")."</td>";
	echo "</tr>";
	
	$sql = "SELECT to_char(v.vlddata,'dd/mm/YYYY HH24:MI') as vlddata, v.vldobservacao, t.tpvdsc, ar.arqid, usu.usunome FROM pde.validacao v 
			LEFT JOIN pde.anexochecklist a ON a.vldid = v.vldid 
			LEFT JOIN public.arquivo ar ON ar.arqid = a.arqid 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = ar.usucpf 
			LEFT JOIN pde.tipovalidacao t ON t.tpvid = v.tpvid 
			WHERE v.iclid='".$arrItem['iclid']."' AND a.ancstatus = 'A'";
	
	$validacao = $db->carregar($sql);
	
	if($validacao[0]) {
	
		echo "<tr>";
		echo "<td colspan=2><table class=listagem width=100%>";
		echo "<thead>";
		echo "<tr><td align=center><b>Data</b></td><td align=center><b>Usuário</b></td><td align=center><b>Observação</b></td><td align=center><b>Fase</b></td><td align=center><b>Download</b></td></tr>";
		echo "</thead>";
					
		foreach($validacao as $val) {
			echo "<tr><td>".$val['vlddata']."</td><td>".$val['usunome']."</td><td>".$val['vldobservacao']."</td><td>".$val['tpvdsc']."</td><td align=center><img src=../imagens/salvar.png align=absmiddle border=0 style=cursor:pointer; onclick=\"window.location='enem.php?modulo=principal/atividade_enem/minhasPendencias&acao=A&requisicao=downloadArquivo&arqid=".$val['arqid']."';\"></td></tr>";
		}
			
		echo "</table></td>";
		echo "</tr>";
		
		
	}
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Observação:</td>";
	echo "<td><textarea name=vldobservacao id=vldobservacao cols=50 rows=7></textarea></td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita colspan=2><input type=button value=Salvar onclick=enviarform();></td>";
	echo "</tr>";
	
	echo "</table>";
 
}

function pegarResponsavelItemCheckList($docid) {
	global $db, $conf;
	
	$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");
	
	$responsavel = $db->pegaUm("SELECT ent.entnome FROM pde.itemchecklist icl 
				 				INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.tpvid = ".$conf[$esdid]['tpvid']."
				 				INNER JOIN entidade.entidade ent ON ent.entid = cle.entid 
				 				WHERE icl.docid='".$docid."'");
	
	return (($responsavel)?$responsavel:'Sem responsável');
}

if($_REQUEST['requisicao']) {
	
	$_REQUEST['requisicao']($_REQUEST);
	exit;
	
}


include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

monta_titulo("ENEM", "Minhas Pendências");

// ----- Recupera o entid do usuário
$entidUsuario = $db->pegaUm("SELECT entid FROM pde.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '".$_SESSION['usucpf']."' AND entid is not null");
if( !$entidUsuario ) {
	$entidUsuario = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj = '".$_SESSION['usucpf']."' AND entstatus = 'A'");
}

if($entidUsuario) {
	
	$parametros = array("capturar_responsavel" => "pegarResponsavelItemCheckList");

	$sql = "SELECT p.pflcod FROM seguranca.perfilusuario p 
			INNER JOIN seguranca.perfil pp ON p.pflcod = pp.pflcod 
			WHERE p.usucpf = '".$_SESSION['usucpf']."' AND pp.sisid = '".$_SESSION['sisid']."'";
	
	$perfis = $db->carregarColuna($sql);
	
	if( in_array(PERFIL_EXECUTOR,$perfis) ) {
		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid, to_char(icl.iclprazoexec, 'YYYY-MM-DD') as data 
				FROM pde.itemchecklist icl
				INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
				WHERE 
					doc.docid is not null AND 
					ati.atistatus='A' AND
					cle.tpvid = 1 AND
					doc.esdid = ".ENEM_EST_EM_EXECUCAO." 
				ORDER BY 
					icl.iclprazoexec";
	}
	if( in_array(PERFIL_VALIDADOR,$perfis) ) {
		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid, to_char(icl.iclprazoval, 'YYYY-MM-DD') as data 
				FROM pde.itemchecklist icl
				INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
				WHERE 
					doc.docid is not null AND 
					ati.atistatus='A' AND
					cle.tpvid = 2 AND
					doc.esdid = ".ENEM_EST_EM_VALIDACAO." 
				ORDER BY 
					icl.iclprazoval";
	}
	if( in_array(PERFIL_CERTIFICADOR,$perfis) ) {
		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid, to_char(icl.iclprazocert, 'YYYY-MM-DD') as data 
				FROM pde.itemchecklist icl
				INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
				WHERE 
					doc.docid is not null AND 
					ati.atistatus='A' AND
					cle.tpvid = 3 AND
					doc.esdid = ".ENEM_EST_EM_CERTIFICACAO." 
				ORDER BY 
					icl.iclprazocert";
	}
	
	$docs['pendencias'] = $db->carregarColuna($sql,1);
	
//	$sql = "SELECT icl.docid, doc.esdid, cle.tpvid, to_char(icl.iclprazo, 'YYYY-MM-DD') as data 
//			FROM pde.itemchecklist icl
//			INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
//			INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
//			INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
//			WHERE doc.docid is not null AND ati.atistatus='A' ORDER BY icl.iclprazo";
//	
//	$dados = $db->carregar($sql);
//	
//	if($dados[0]) {
//		foreach($dados as $d) {
//			
//			if( ( $d['esdid'] == ENEM_EST_EM_EXECUCAO && $d['tpvid'] == 1 && in_array(PERFIL_EXECUTOR,$perfis) ) ) {
//				$docs['pendencias'][] = $d['docid'];
//			}
//			if( ( $d['esdid'] == ENEM_EST_EM_VALIDACAO && $d['tpvid'] == 2 && in_array(PERFIL_VALIDADOR,$perfis) ) ) {
//				$docs['pendencias'][] = $d['docid'];
//			}
//			if($d['esdid'] == ENEM_EST_EM_CERTIFICACAO && $d['tpvid'] == 3 && in_array(PERFIL_CERTIFICADOR,$perfis)) {
//				$docs['pendencias'][] = $d['docid'];
//			}
//				
//		}
//	}
	
//	$sql = "SELECT icl.docid, doc.esdid, cle.tpvid, to_char(icl.iclprazo, 'YYYY-MM-DD') as data FROM pde.itemchecklist icl
//			INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
//			INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
//			INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
//			WHERE doc.docid is not null AND ati.atistatus='A' AND icl.iclprazo < now() ORDER BY icl.iclprazo";
//	
//	$dados = $db->carregar($sql);
//	
//	if($dados[0]) {
//		foreach($dados as $d) {
//			
//			if( ( $d['esdid'] == ENEM_EST_EM_EXECUCAO     && $d['tpvid'] == 1 && in_array(PERFIL_EXECUTOR,$perfis) ) || ( $d['esdid'] != ENEM_EST_EM_FINALIZADO && $d['data'] < date('Ymd') ) ) {
//				$docs['atrazados'][] = $d['docid']; 
//			}
//			if( ( $d['esdid'] == ENEM_EST_EM_VALIDACAO    && $d['tpvid'] == 2 && in_array(PERFIL_VALIDADOR,$perfis) ) || ( $d['esdid'] != ENEM_EST_EM_FINALIZADO && $d['data'] < date('Ymd') ) ) {
//				$docs['atrazados'][] = $d['docid'];
//			}
//			if($d['esdid'] == ENEM_EST_EM_CERTIFICACAO && $d['tpvid'] == 3 && in_array(PERFIL_CERTIFICADOR,$perfis)) {
//				$docs['atrazados'][] = $d['docid'];
//			}
//				
//		}
//	}

	$filtro_pendencias = '';
	if ( count( $docs['pendencias'] ) > 0 ) $filtro_pendencias = "doc.docid not in ( '" . implode( "','", $docs['pendencias'] ) . "' ) AND";
	
	if($_REQUEST['atiidraiz']){
		$filtro_pendencias .= "ati._atinumero ilike '".$_REQUEST['atiidraiz']."%' AND";
	}

	if(in_array(PERFIL_EXECUTOR,$perfis)) {

		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid FROM pde.itemchecklist icl
				INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario." 
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid 
				WHERE doc.docid is not null AND 
					  ati.atistatus='A' AND 
					  icl.iclprazo >= now()::date AND 
					  $filtro_pendencias
					  doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".ENEM_EST_EM_EXECUCAO."' )
			    ORDER BY icl.iclprazo";
					  //dbg($sql,1);
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$docs['futuras'][$d['docid']] = $d['docid'];
			}
		}
		
		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid FROM pde.itemchecklist icl
				INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario." 
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid 
				WHERE doc.docid is not null AND 
					  ati.atistatus='A' AND 
					  --icl.iclprazoexec < now() AND
					  icl.iclprazo < now()::date AND
					  $filtro_pendencias
					  doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".ENEM_EST_EM_EXECUCAO."' )
			    ORDER BY icl.iclprazoexec";
					 
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$docs['atrazados'][$d['docid']] = $d['docid'];
			}
		}
		
	}
	
	if(in_array(PERFIL_VALIDADOR,$perfis)) {

		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid FROM pde.itemchecklist icl
				INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND tpvid = 2 AND cle.entid = ".$entidUsuario."
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
				WHERE doc.docid is not null AND 
						ati.atistatus='A' AND 
						icl.iclprazo >= now()::date AND
						$filtro_pendencias
						doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".ENEM_EST_EM_VALIDACAO."' )
				ORDER BY icl.iclprazo";
		
		$dados = $db->carregar($sql);
		
		if($dados[0]) {
			foreach($dados as $d) {
				$docs['futuras'][$d['docid']] = $d['docid'];
			}
		}
		
		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid FROM pde.itemchecklist icl
				INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
				WHERE doc.docid is not null AND 
						ati.atistatus='A' AND 
						--icl.iclprazoval < now() AND
						icl.iclprazo < now()::date AND
						$filtro_pendencias
						doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".ENEM_EST_EM_VALIDACAO."' )
				ORDER BY icl.iclprazoval";
		
		$dados = $db->carregar($sql);
		
		if($dados[0]) {
			foreach($dados as $d) {
				$docs['atrazados'][$d['docid']] = $d['docid'];
			}
		}
		
	}
	
	if(in_array(PERFIL_CERTIFICADOR,$perfis)) {

		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid FROM pde.itemchecklist icl
				INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
				WHERE doc.docid is not null AND 
						ati.atistatus='A' AND 
						icl.iclprazo >= now()::date AND
						$filtro_pendencias
						doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".ENEM_EST_EM_CERTIFICACAO."' ) 
				ORDER BY icl.iclprazo";
		
		$dados = $db->carregar($sql);
		
		if($dados[0]) {
			foreach($dados as $d) {
				$docs['futuras'][$d['docid']] = $d['docid'];
			}
		}
	
		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid FROM pde.itemchecklist icl
				INNER JOIN pde.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
				WHERE doc.docid is not null AND 
						ati.atistatus='A' AND 
						--icl.iclprazocert < now() AND
						icl.iclprazo < now()::date AND
						$filtro_pendencias
						doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".ENEM_EST_EM_CERTIFICACAO."' ) 
				ORDER BY icl.iclprazocert";
		
		$dados = $db->carregar($sql);
		
		if($dados[0]) {
			foreach($dados as $d) {
				$docs['atrazados'][$d['docid']] = $d['docid'];
			}
		}
		
	}

}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.min.js"></script>
<script>
			function enviarform() {
			if(document.getElementById('vldsituacao_FALSE').checked){
				if(document.getElementById('vldobservacao').value == '') {
					alert('Observação é obrigatória');
					return false;
				}
			}
			if(document.getElementById('etcopcaoevidencia')){
				if(document.getElementById('arquivo').value == '') {
					alert('Anexe um arquivo');
					return false;
				}
			}
			document.getElementById('formulario').submit();
		  }

			function formulario_filtro_submeter()
			{
				document.formulario_filtro.submit();
			}
</script>


<form name="formulario_filtro" action="" method="post">
	<input type="hidden" name="formulario_filtro" value="1"/>
	<table class="tabela" border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">
				Atividade
			</td>
			<td>
				<select id="atiidraiz" name="atiidraiz" class="CampoEstilo" style="width: 250px;">
					<option value="">
						<?php
						$sql = "select atidescricao from pde.atividade where atiid = " . PROJETO;
						echo $db->pegaUm( $sql );
						?>
					</option>
					<?php
					
					$sql = "
						select
							a.atiid,
							a.atidescricao,
							a._atiprofundidade as profundidade,
							a._atinumero as numero
						from pde.atividade a
						where
							a.atistatus = 'A'
							and a._atiprofundidade < 3
							and a._atiprojeto = " . PROJETO . " 
						order by
							a._atiordem
					";

					$lista = $db->carregar( $sql );
					$lista = $lista ? $lista : array();
					
					?>
					<?php foreach ( $lista as $item ) : ?>
						<option value="<?=$item['numero']?>." <?= $item['numero'].'.' == $_REQUEST["atiidraiz"] ? 'selected="selected"' : '' ?>>
							<?= str_repeat( '&nbsp;', $item['profundidade'] * 5 ) ?>
							<?= $item['numero'] ?>
							<?= $item['atidescricao'] ?>
						</option>
					<?php endforeach; ?>
				</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloEsquerda">
				<input
					type="button"
					name="filtrar"
					value="Filtrar"
					onclick="formulario_filtro_submeter();"
				/>
			</td>
		</tr>
	</table>
</form>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr><td><? wf_gerencimentoFluxo(TPDID_ENEM, $docs, (($_REQUEST['cxentrada'])?$_REQUEST['cxentrada']:'pendencias'),$parametros,$filtro_pendencias); ?></td></tr>
</table>