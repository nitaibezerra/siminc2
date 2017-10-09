<?php
$conf = array(WK_ESTADO_DOC_EM_VALIDACAO 	=> array('tpvid' => '2', 'descricao' => 'validar', 'mvdsituacao_TRUE' => WK_MON_EST_AEDID_VALIDAR, 'mvdsituacao_FALSE' => WK_MON_EST_AEDID_INVALIDAR, 'mvdsituacao_FINALIZAR' => WK_MON_EST_AEDID_FINALIZAR),
			  WK_ESTADO_DOC_EM_EXECUCAO	=> array('tpvid' => '1', 'descricao' => 'executar','mvdsituacao_TRUE' => WK_MON_EST_AEDID_ENVIAR_VALIDACAO, 'mvdsituacao_FALSE' => false, 'mvdsituacao_FINALIZAR' => WK_MON_EST_AEDID_FINALIZAR),
			  WK_ESTADO_DOC_FINALIZADO	    => array('tpvid' => '3', 'descricao' => 'finalizar','mvdsituacao_TRUE' => WK_MON_EST_AEDID_FINALIZAR, 'mvdsituacao_FALSE' => WK_MON_EST_AEDID_ENVIAR_VALIDACAO));	  

function downloadArquivo($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("anexochecklist", $arrCampos = array(), "pde");
	$file->getDownloadArquivo($dados['arqid']);
}
			  
function salvarItemMonitoramento($dados) {
	global $db, $conf;
	
	include_once APPRAIZ . 'includes/workflow.php';
	
	//Pega a entidade do usuário
	$entidUsuario = $db->pegaUm("SELECT entid FROM pde.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '".$_SESSION['usucpf']."' AND entid is not null");
	
	//Se não tiver entidade do usuário, cria
	if( !$entidUsuario ) {
		$entidUsuario = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj = '".$_SESSION['usucpf']."' AND entstatus = 'A'");
	}
	
	//pega o estado do documento (esdid)
	$arrwf = $db->pegaLinha("SELECT 
								d.esdid,
								d.docid,
								i.metid,
								i.dmiid 
							FROM 
								painel.detalhemetaindicador i 
						     INNER JOIN 
						     	workflow.documento d ON i.docid = d.docid
						     INNER JOIN
						     	pde.monitorameta mnm ON mnm.metid = i.metid 
						     WHERE 
						     	mnm.mnmid='".$dados['mnmid']."' and d.docid = {$dados['docid']}");

	//Verifica se a meta ja teve avaliação, se tiver, pega o id (mvdid)
	$sql = "SELECT mvdid FROM pde.monitoravalidacao 
			WHERE entid='".$entidUsuario."' AND tpvid='".$conf[$arrwf['esdid']]['tpvid']."' AND mnmid='".$dados['mnmid']."'";

	$mvdid = $db->pegaUm($sql);
	
	//Se a opção for para seguir para o proximo passo, pega o proximo tipo e o proximo estado de destino do workflow
	if($dados['mvdsituacao'] == "TRUE") {
		$tpviddestino = ((int)$conf[$arrwf['esdid']]['tpvid'])+1;
		$esdiddestino = $conf[$arrwf['esdid']]['mvdsituacao_TRUE'];
	}else{ //Se não, volta para o tipo e estado do workflow anteriores
		$tpviddestino = ((int)$conf[$arrwf['esdid']]['tpvid'])-1;
		$tpviddestino = $tpviddestino == 0 ? 1 : $tpviddestino;
		$esdiddestino = $conf[$arrwf['esdid']]['mvdsituacao_FALSE'];
	}
	
	//Pega os dados do usuário (nome e e-mail) e id da entidade  de acordo com o tipo
	$sql = "SELECT ent.entid, usu.usuemail, usu.usunome FROM pde.monitorametaentidade cle 
			INNER JOIN entidade.entidade ent ON ent.entid = cle.entid 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = ent.entnumcpfcnpj 
			WHERE cle.mnmid = ".$dados['mnmid']." AND cle.tpvid = ".$tpviddestino;
	$dadosusu = $db->pegaLinha($sql);
	
	//Se não tiver os dados do usuário, pega na tabela de responsabilidade
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
	
	//Coleta e trata os valores inseridos pelo usuário
	$mnmqtd = $_POST['mnmqtd'];
	$mnmqtd = str_replace(array(".",","),array("",""),$mnmqtd);
	$mnmqtd = !$mnmqtd ? "0" : $mnmqtd;
	$mnmvalor = $_POST['mnmvalor'];
	$mnmvalor = str_replace(array(".",","),array("","."),$mnmvalor);
	$mnmvalor = !$mnmvalor ? "0" : $mnmvalor;
	
	//Salva a Série Histórica no Painel de Controle, se a situação for verdadeira
	if($dados['mvdsituacao'] == "TRUE"){
		//Cria o detalhe da periodicidade se não existir
		$data = date("Y-m-d");
		$mes = date("m");
		$ano = date("Y");
		$dpeordem = (date("z")+1);
		$dpedsc = date("d/m/Y");
		$sql = "select dpeid from painel.detalheperiodicidade where dpedatainicio = '$data' and dpedatafim = '$data' and dpemesref = '$mes' and dpeanoref = '$ano'";
		$dpeid = $db->pegaUm($sql);
		if(!$dpeid){
			$sql = "insert into
						painel.detalheperiodicidade
					(perid,dpedsc,dpestatus,dpeordem,dpemesref,dpeanoref,dpedatainicio,dpedatafim)
						values
					(5,'$dpedsc','A',$dpeordem,'$mes','$ano','$data','$data') returning dpeid";
			$dpeid = $db->pegaUm($sql);
		}
		
		//Verifica o id do indicador
		$sql = "select 
					indid
				from
					pde.monitoraitemchecklist mic
				inner join
					pde.monitorameta mnm ON mnm.micid = mic.micid
				where
					mnmid = ".$dados['mnmid'];
		$indid = $db->pegaUm($sql);
		
		//Insere na tabela de série histórica
		$sql = "insert into 
					painel.seriehistorica
				(indid,sehvalor,sehqtde,dpeid,sehdtcoleta,regid,sehbloqueado,dmiid)
					values
				($indid,$mnmvalor,$mnmqtd,$dpeid,now(),1,false,{$arrwf['dmiid']}) returning sehid";
		$sehid = $db->pegaUm($sql);
		
		//Insere na tabela de valores de série histórica
		$sql = "insert into 
					painel.detalheseriehistorica
				(ddiid,sehid,dshvalor,dshcod,dshcodmunicipio,dshuf,dshqtde,tidid1,tidid2,iepid,entid,unicod,polid,iecid)
					values
				(NULL,$sehid,$mnmvalor,NULL,NULL,NULL,$mnmqtd,NULL,NULL,NULL,NULL,NULL,NULL,NULL)";
		$db->executar($sql);
	}else{
		// Insere aqui tramitação casa não tenha execução
		$esdiddestino = 1178;
		
	}
	
	//Se tiver avaliação, atualiza os dados
	if($mvdid) {
		$db->executar("UPDATE pde.monitoravalidacao 
					   SET mvdsituacao = ".$dados['mvdsituacao'].", mnmqtd = $mnmqtd, mnmvalor = $mnmvalor, mvddata = now(), mvdobservacao = '".pg_escape_string($dados['mvdobservacao'])."'
					   WHERE mnmid = ".$dados['mnmid']." AND tpvid = ".$conf[$arrwf['esdid']]['tpvid']." AND entid = ".$entidUsuario);
	}else{ // Se não tiver avaliação, atualiza
		$mvdid = $db->pegaUm("INSERT INTO pde.monitoravalidacao(
					            mnmid, tpvid, entid, mvdsituacao, mvddata, mvdobservacao,mnmqtd,mnmvalor)
    						  VALUES ('".$dados['mnmid']."', '".$conf[$arrwf['esdid']]['tpvid']."', '".$entidUsuario."', ".$dados['mvdsituacao'].", now(), '".pg_escape_string($dados['mvdobservacao'])."',$mnmqtd,$mnmvalor) RETURNING mvdid;");
	}

	//Verifica se tem anexo para inserir
	if( $_FILES['arquivo']['name'] ) {
		if( $mvdid ) {
			$sql = "SELECT macid FROM pde.monitoraanexochecklist WHERE mvdid = ".$mvdid." AND macstatus = 'A'";
			$macid = $db->pegaUm($sql);
			if( $macid ) {
				$sql = "UPDATE pde.monitoraanexochecklist SET macstatus = 'I' WHERE macid = ".$macid;
				$db->executar($sql);
				$db->commit();
			}
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$arrCampos = array("mvdid" => $mvdid);
			$file = new FilesSimec("monitoraanexochecklist", $arrCampos, "pde");
			$arqdescricao = 'arquivo_meta_checklist_monitoramento_estrategico_'.$mvdid.'_'.date('Ymdhis');
			$file->setUpload($arqdescricao, "arquivo");
		}
	}
	
	//Recupera a atividade (pai) da meta
	$atiid = $db->pegaUm("	SELECT 
								atiid 
							FROM 
								pde.monitoraitemchecklist mic
							INNER JOIN
								pde.monitorameta mnm ON mnm.micid = mic.micid 
							WHERE 
								mnm.mnmid='".$dados['mnmid']."'");
	
	
	//verifica se possui validador para a meta, se não tiver, deve finalizá-la
	//Verifica se o estado atual é em execução
	if($dados['mvdsituacao'] == "TRUE" && $arrwf['esdid'] == WK_ESTADO_DOC_EM_EXECUCAO){
		//verifica se existe a validação
		$sql = "select tpvid from pde.monitoraetapascontrole where tpvid = 2 and mnmid={$dados['mnmid']};";
		$existeValidador = $db->pegaUm($sql);
		
		//Se não existir validador, finaliza
		if(!$existeValidador){
			$dados['mvdsituacao'] = "FINALIZAR";
			$esdiddestino = WK_MON_EST_AEDID_FINALIZAR;
		}
		
	}
	
	//Se existir destino para o workflow
	if($esdiddestino){
		//Se for para voltar no workflow, deve atribuir comentário
		if($dados['mvdsituacao'] == "FALSE") {
			$comentario = $dados['mvdobservacao'];
		}
		
		//Faz a alteração do documento
		wf_alterarEstado( $arrwf['docid'], $esdiddestino, $comentario, $dados );
	}
	
	//Se existir dados do usuário ou e-mail
	if( $dadosusu['usuemail'] || is_array($arrEmail) ) {
		
		// Email para o validador ou certificador informando que a fase anterior do checklist foi concluída:
		if(($tpviddestino == '2' || $tpviddestino == '3') && $dados['mvdsituacao'] == 'TRUE') {
			
			$html .= "<p>Prezado (a) ".$dadosusu['usunome'].",</p>";
	 		$html .= "<p>Informamos que a meta abaixo foi concluída e está pendente de sua validação no módulo Monitoramento Estratégico do sistema SIMEC:</p>";
			
			$sql = "SELECT 
						*,
						to_char(dmi.dmidataexecucao,'DD/MM/YYYY') as dmidataexecucao,
						to_char(dmi.dmidatavalidacao,'DD/MM/YYYY') as dmidatavalidacao 
					FROM 
						pde.monitorameta mnm 
					INNER JOIN 
						pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
					INNER JOIN
						painel.detalhemetaindicador dmi ON mnm.metid = dmi.metid 
					WHERE 
						mnm.mnmid=".$dados['mnmid']."
					and
						dmi.docid = {$dados['docid']}";
			
			$itemchecklist = $db->pegaLinha($sql);
			$html .= "<p>".$itemchecklist['mnmdsc']."</p>";
			//$html .= "<p>Período: De ".$itemchecklist['dmddatainiexecucao']." até ".$itemchecklist['dmddatafimexecucao']."</p>";
			$html .= "<p>Para concluí-la, acesse o módulo Monitoramento Estratégico no endereço <a href=http://simec.mec.gov.br>http://simec.mec.gov.br</a>, no menu \"Principal > Minhas Pendências\", e clique no ícone <img align=absmiddle src=../../imagens/valida2.gif></p>";
		}
		
			// Email para o executor, validador informando que o item foi invalidado
		if(($tpviddestino == '1' || $tpviddestino == '2') && $dados['mvdsituacao'] == 'FALSE') {
			
			$html .= "<p>Prezado (a) ".$dadosusu['usunome'].",</p>";
	 		$html .= "<p>Informamos que a meta do item do checklist abaixo foi invalidada.<br/>Favor acessar o módulo ENEM do sistema SIMEC para realizar nova execução:</p>";
			
			$sql = "SELECT * FROM pde.monitorameta mnm INNER JOIN pde.monitoraitemchecklist mic ON mic.micid = mnm.micid WHERE mnmid='".$dados['mnmid']."'";
			$itemchecklist = $db->pegaLinha($sql);
			
			$html .= "<p>".$itemchecklist['mnmdsc']."</p>";
			//$html .= "<p>Período: De ".$itemchecklist['dmddatainiexecucao']." até ".$itemchecklist['dmddatafimexecucao']."</p>";
			$html .= "<p>Para concluí-la, acesse o módulo ENEM no endereço <a href=http://simec.mec.gov.br>http://simec.mec.gov.br</a>, no menu \"Principal > Minhas Pendências\", e clique no ícone <img align=absmiddle src=../../imagens/valida2.gif></p>";
		}
		
		//Adiciona o e-mail do usuário no array de e-mail, se não houver nenhum
		$arrEmail 	= is_array($arrEmail) ? $arrEmail : array($dadosusu['usuemail']);
		$titulo		= SIGLA_SISTEMA. " - Monitoramento Estratégico - Aviso de validação/invalidação de metas do item de checklist";
		require_once(APPRAIZ . 'includes/classes/EmailAgendado.class.inc');
		//verifica se existe texto para enviar
// 		if($html) {
// 			$e = new EmailAgendado();
// 			$e->setTitle($titulo);
// 			$e->setText($html);
// 			$e->setName(SIGLA_SISTEMA);
// 			$e->setEmailOrigem($_SESSION['email_sistema']);
// 			$e->setEmailsDestino($arrEmail);
// 			$e->enviarEmails();
// 		}

	}

	//Confirma todas as alterações
	$db->commit();
	
	//Alerta o usuário e rotorna para a tela de pendências
	die("<script>
			alert('Dados registrados com sucesso');
			window.location='estrategico.php?modulo=principal/atividade_estrategico/minhasPendencias&acao=A';
		 </script>");
	
}


function telaFluxoMonitoramentoEstrategico($dados) {
	global $db, $conf;
	
	$sql = "SELECT 
				atv._atinumero, 
				doc.esdid, 
				atv.atidescricao,
				mic.micid,
				ind.indnome, 
				mnm.mnmdsc,
				mnm.mtmid, 
				to_char(mnm.mnmprazo, 'dd/mm/YYYY') as mnmprazo, 
				mnm.mnmid,
				ind.indqtdevalor,
				to_char(dmi.dmidataexecucao, 'DD/MM/YYYY') as dmidataexecucao,
				to_char(dmi.dmidatavalidacao, 'DD/MM/YYYY') as dmidatavalidacao,
				ind.unmid,
				ind.estid,
				ume.umedesc,
				ume.umeid,
				dmi.metid,
				dmi.dmiqtde
			FROM painel.detalhemetaindicador dmi
			INNER JOIN workflow.documento doc ON doc.docid = dmi.docid
			INNER JOIN pde.monitorameta mnm ON mnm.metid = dmi.metid
			INNER JOIN pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
			INNER JOIN painel.indicador ind ON ind.indid = mic.indid 
			INNER JOIN pde.atividade atv ON mic.atiid = atv.atiid
			inner join painel.unidademeta ume ON ume.umeid = ind.umeid 
			WHERE dmi.docid = '".$dados['docid']."' order by dmi.dmidataexecucao desc";
	
	$arrItem = $db->pegaLinha($sql);
	echo monta_titulo("Detalhes da Pendência","");
	echo "<form method=post name='formulario_{$dados['docid']}' id='formulario_{$dados['docid']}' enctype=multipart/form-data>";
	echo "<input type=hidden name=requisicao value=salvarItemMonitoramento>";
	echo "<input type=hidden name=mnmid value=".$arrItem['mnmid'].">";
	echo "<input type=hidden name=docid value=".$dados['docid'].">";
	echo "<table class=tabela width=100% cellspacing=2 cellpadding=3 align=center>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Atividade:</td>";
	echo "<td>".$arrItem['_atinumero']." - ".$arrItem['atidescricao']."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Item:</td>";
	echo "<td>".$arrItem['micid']." - ".$arrItem['indnome']."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Meta:</td>";
	if($arrItem['mtmid'] != 1 && $arrItem['unmid'] != UNIDADEMEDICAO_BOLEANA){ //Verifica se precisa coletar a quantidade
		
		$valor = formata_valor($arrItem['dmiqtde']);
		$valor = explode(',',$valor);
		$valor = $valor[1]>0 ? $valor[0].','.$valor[1] : $valor[0];
		$valor = in_array($arrItem['unmid'], array(1)) ? $valor.'%' : $valor; 
		
		echo "<td>".$valor." {$arrItem['umedesc']}</td>";
	}else{		
		echo "<td>".$arrItem['mnmid']." - ".$arrItem['mnmdsc']."</td>";
	}
	echo "</tr>";
	/*
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Período:</td>";
	echo "<td>De ".$arrItem['datainicio']." até ".$arrItem['datafim']."</td>";
	echo "</tr>";
	*/
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Deseja ".$conf[$arrItem['esdid']]['descricao']." este item?</td>";
	echo "<td><input type=radio name=mvdsituacao id=mvdsituacao_TRUE value=TRUE checked> Sim <input type=radio name=mvdsituacao id=mvdsituacao_FALSE value=FALSE> Não</td>";	
	echo "</tr>";
	
	$arrEtcopcaoevidencia = $db->pegaLinha("SELECT mecopcaoevidencia, mecevidencia FROM pde.monitoraetapascontrole WHERE mnmid='".$arrItem['mnmid']."' AND tpvid = '".$conf[$arrItem['esdid']]['tpvid']."'");
	//if($unmid != UNIDADEMEDICAO_BOLEANA)
	
	if($arrItem['mtmid'] != 1 && $arrItem['unmid'] != UNIDADEMEDICAO_BOLEANA){ //Verifica se precisa coletar a quantidade
		echo "<tr>";
		echo "<td class=SubTituloDireita >{$arrItem['umedesc']}:</td>";
		echo "<td>".campo_texto("mnmqtd","S","S","",20,18,"[.###]","","","",""," id='mnmqtd' ")."</td>";
		echo "</tr>";
	}
	
	if($arrItem['unmid'] != UNIDADEMEDICAO_BOLEANA && $arrItem['mtmid'] != 1 && $arrItem['indqtdevalor'] == "t"){ //Verifica se precisa coletar o valor
		echo "<tr>";
		echo "<td class=SubTituloDireita >Valor:</td>";
		echo "<td>".campo_texto("mnmvalor","S","S","",20,18,"###.###.###.###,##","","","",""," id='mnmvalor' ")."</td>";
		echo "</tr>";
	}
	
	echo "<td class=SubTituloDireita width=30%>".(($arrEtcopcaoevidencia['mecevidencia'])?$arrEtcopcaoevidencia['mecevidencia']:"Anexo").":</td>";
	echo "<td><input type=file id=arquivo name=arquivo />".(($arrEtcopcaoevidencia['mecopcaoevidencia']=='t')?"<input type=hidden name=mecopcaoevidencia id=mecopcaoevidencia value=sim>":"")."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Observação:</td>";
	echo "<td><textarea name=mvdobservacao id=mvdobservacao cols=50 rows=7></textarea></td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita colspan=2><input type=button value=Salvar id=\"btn_salvar\" onclick=enviarform('{$dados['docid']}');></td>";
	echo "</tr>";
	
	echo "</table>";
	
	
	if($arrItem['mtmid'] != 1 && $arrItem['unmid'] != UNIDADEMEDICAO_BOLEANA){ //Verifica se precisa coletar a quantidade
		$sql = "SELECT 
					to_char(v.mvddata,'dd/mm/YYYY HH24:MI') as mvddata,
					usu.usunome,
					 mnmqtd, 
					v.mvdobservacao,
					--t.tpvdsc,
					CASE WHEN ar.arqid IS NULL
						THEN '<a href=\'#\' onclick=\"window.location=\'enem.php?modulo=principal/atividade_enem/minhasPendencias&acao=A&requisicao=downloadArquivo&arqid=' || ar.arqid || '\'\" />' || ar.arqnome || '.' || ar.arqextensao || '</a>'
						ELSE 'N/A'
					END as anexo
			FROM pde.monitoravalidacao v 
			LEFT JOIN pde.monitoraanexochecklist a ON a.mvdid = v.mvdid AND a.macstatus = 'A' 
			LEFT JOIN public.arquivo ar ON ar.arqid = a.arqid 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = ar.usucpf 
			LEFT JOIN pde.tipovalidacao t ON t.tpvid = v.tpvid 
			WHERE v.mnmid='".$arrItem['mnmid']."'";
		$arrCabecalho = array("Data de Execução","Usuário",$arrItem['umedesc'],"Observação","Anexo");
	}
	
	if($arrItem['unmid'] != UNIDADEMEDICAO_BOLEANA && $arrItem['mtmid'] != 1 && $arrItem['indqtdevalor'] == "t"){ //Verifica se precisa coletar o valor
		$sql = "SELECT 
					to_char(v.mvddata,'dd/mm/YYYY HH24:MI') as mvddata,
					usu.usunome,
					 mnmqtd,
					 mnmvalor, 
					v.mvdobservacao,
					--t.tpvdsc,
					CASE WHEN ar.arqid IS NULL
						THEN '<a href=\'#\' onclick=\"window.location=\'enem.php?modulo=principal/atividade_enem/minhasPendencias&acao=A&requisicao=downloadArquivo&arqid=' || ar.arqid || '\'\" />' || ar.arqnome || '.' || ar.arqextensao || '</a>'
						ELSE 'N/A'
					END as anexo
			FROM pde.monitoravalidacao v 
			LEFT JOIN pde.monitoraanexochecklist a ON a.mvdid = v.mvdid AND a.macstatus = 'A' 
			LEFT JOIN public.arquivo ar ON ar.arqid = a.arqid 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = ar.usucpf 
			LEFT JOIN pde.tipovalidacao t ON t.tpvid = v.tpvid 
			WHERE v.mnmid='".$arrItem['mnmid']."'";
		$arrCabecalho = array("Data de Execução","Usuário",$arrItem['umedesc'],"Valor (R$)","Observação","Anexo");
	}
	
	if($arrItem['unmid'] == UNIDADEMEDICAO_BOLEANA){
		$sql = "SELECT 
					to_char(v.mvddata,'dd/mm/YYYY HH24:MI') as mvddata,
					usu.usunome,
					v.mvdobservacao,
					--t.tpvdsc,
					CASE WHEN ar.arqid IS NULL
						THEN '<a href=\'#\' onclick=\"window.location=\'enem.php?modulo=principal/atividade_enem/minhasPendencias&acao=A&requisicao=downloadArquivo&arqid=' || ar.arqid || '\'\" />' || ar.arqnome || '.' || ar.arqextensao || '</a>'
						ELSE 'N/A'
					END as anexo
			FROM pde.monitoravalidacao v 
			LEFT JOIN pde.monitoraanexochecklist a ON a.mvdid = v.mvdid AND a.macstatus = 'A' 
			LEFT JOIN public.arquivo ar ON ar.arqid = a.arqid 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = ar.usucpf 
			LEFT JOIN pde.tipovalidacao t ON t.tpvid = v.tpvid 
			WHERE v.mnmid='".$arrItem['mnmid']."'";
		$arrCabecalho = array("Data de Execução","Usuário","Observação","Anexo");
	}

	if($arrItem['esdid'] == WK_ESTADO_DOC_EM_VALIDACAO){
		monta_titulo("Fase de Execução","");
		$db->monta_lista_simples($sql,$arrCabecalho,100000,10000,"N","","N");
	}
	
	
/*
	$sql = "select 
				sehqtde,
				sehvalor 
			from
				painel.seriehistorica seh
			inner join
				painel.detalheseriehistorica dsh ON seh.sehid = dsh.sehid
			where
				seh.metid = {$arrItem['metid']}
			order by
				seh.sehid desc;";
	
	$sql = "select * from pde.monitoravalidacao where mnmid = {$arrItem['mnmid']} and tpvid = 1";
	
	$validacao = $db->carregar($sql);
	dbg($sql);
	
	if($validacao[0]) {
	
		echo "<tr>";
		echo "<td colspan=2><table class=listagem width=100%>";
		echo "<thead>";
		echo "<tr><td align=center><b>Usuário</b></td><td align=center><b>Quantidade</b></td><td align=center><b>Valor</b></td><td align=center><b>Observação</b></td><td align=center><b>Fase</b></td><td align=center><b>Download</b></td></tr>";
		echo "</thead>";
					
		foreach($validacao as $val) {
			echo "<tr><td>".$val['usunome']."</td><td>".$val['mnmqtd']."</td><td>".$val['mnmvalor']."</td><td>".$val['mvdobservacao']."</td><td>".$val['tpvdsc']."</td><td align=center><img src=../imagens/salvar.png align=absmiddle border=0 style=cursor:pointer; onclick=\"window.location='enem.php?modulo=principal/atividade_enem/minhasPendencias&acao=A&requisicao=downloadArquivo&arqid=".$val['arqid']."';\"></td></tr>";
		}
			
		echo "</table></td>";
		echo "</tr>";	
	}
	*/
 
}

function pegarResponsavelItemCheckList($docid) {
	global $db, $conf;
	
	$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");
	$sql = "SELECT 
				distinct ent.entnome
			FROM 
				painel.detalhemetaindicador dmi
			INNER JOIN
				pde.monitorameta mnm ON mnm.metid = dmi.metid
			INNER JOIN 
				pde.monitorametaentidade mme ON mme.mnmid = mnm.mnmid AND tpvid = ".$conf[$esdid]['tpvid']."
			INNER JOIN 
				entidade.entidade ent ON ent.entid = mme.entid 
			WHERE 
				dmi.docid='$docid'";
	$responsavel = $db->pegaUm($sql);
	
	return (($responsavel)?$responsavel:'Sem responsável');
}

if($_REQUEST['requisicao']) {
	
	$_REQUEST['requisicao']($_REQUEST);
	exit;
	
}


include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

monta_titulo("Monitoramento Estratégico", "Minhas Pendências");

// ----- Recupera o entid do usuário
$entidUsuario = $db->pegaUm("SELECT entid FROM pde.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '".$_SESSION['usucpf']."' AND entid is not null");
if( !$entidUsuario ) {
	$entidUsuario = $db->pegaUm("SELECT ent.entid FROM entidade.entidade ent INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid WHERE entnumcpfcnpj = '".$_SESSION['usucpf']."' AND entstatus = 'A' and fen.funid in (98,99,100,101) ");
}
if($entidUsuario) {
	
	$parametros = array("capturar_responsavel" => "pegarResponsavelItemCheckList");

	$sql = "SELECT p.pflcod FROM seguranca.perfilusuario p 
			INNER JOIN seguranca.perfil pp ON p.pflcod = pp.pflcod 
			WHERE p.usucpf = '".$_SESSION['usucpf']."' AND pp.sisid = '".$_SESSION['sisid']."'";
	
	$perfis = $db->carregarColuna($sql);
		
	if($_REQUEST['atiidraiz']){
		$filtro_pendencias = " AND ati._atinumero ilike '".$_REQUEST['atiidraiz']."%' ";
	}
	
	$sql = "select distinct
				doc.docid, 
				doc.esdid, 
				mme.tpvid,
				mnm.mnmid,
				dmi.dmidataexecucao,
				to_char(dmi.dmidataexecucao, 'DD-MM-YYYY') as dmidataexecucao,
				to_char(dmi.dmidatavalidacao, 'DD-MM-YYYY') as dmidatavalidacao
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid and dpestatus = 'A'
			inner join
				pde.monitorameta mnm ON mnm.metid = dmi.metid
			inner join
				 pde.monitorametaentidade mme ON mme.mnmid = mnm.mnmid
			INNER JOIN
				workflow.documento doc ON doc.docid = dmi.docid
			INNER JOIN 
				pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
			INNER JOIN 
				pde.atividade ati ON ati.atiid = mic.atiid	
			where
				dmi.dmistatus= 'A'
			and
				mic.micstatus = 'A'
			and
				ati.atistatus = 'A'
			--RETIRADO EM 09/05/2014 - PARA EXIBIR TODOS OS ITENS EM EXECUÇÃO NA LISTA DE PENDÊNCIAS
			--and dmi.dmidataexecucao <= now()::date
			and
				mnm.mnmstatus = 'A'
			and
				doc.docid is not null
			$filtro_pendencias 
			and
				mme.entid = ".$entidUsuario."
			order by
				 dmi.dmidataexecucao asc";


	/*$sql = "SELECT distinct mnm.docid, doc.esdid, cle.tpvid, to_char(mnm.mnmprazo, 'YYYY-MM-DD') as data 
			FROM pde.monitorameta mnm
			INNER JOIN pde.monitorametaentidade cle ON cle.mnmid = mnm.mnmid AND cle.entid = ".$entidUsuario."
			INNER JOIN workflow.documento doc ON doc.docid = mnm.docid
			INNER JOIN pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
			INNER JOIN painel.indicador ind ON ind.indid = mic.indid
			INNER JOIN pde.atividade ati ON ati.atiid = mic.atiid
			WHERE doc.docid is not null AND
			$filtro_pendencias 
			ati.atistatus='A'";*/
	$dados = $db->carregar($sql);
	
	if($dados[0]) {
		foreach($dados as $d) {
			
			if( ( $d['esdid'] == WK_ESTADO_DOC_EM_EXECUCAO && $d['tpvid'] == 1 && in_array(PERFIL_EXECUTOR,$perfis) ) ) {
				$arrExecutor = recuparMonitoraMetaExecutor($d['mnmid']);
				if($entidUsuario == $arrExecutor['entid_executor']){
					$docs['pendencias'][] = $d['docid'];
				}
			}
			if( ( $d['esdid'] == WK_ESTADO_DOC_EM_VALIDACAO && $d['tpvid'] == 2 && in_array(PERFIL_VALIDADOR,$perfis) ) ) {
				$arrValidador = recuparMonitoraMetaValidador($d['mnmid']);
				if($entidUsuario == $arrValidador['entid_validador']){
					$docs['pendencias'][] = $d['docid'];
				}
			}
				
		}
	}

	//$filtro_pendencias = '';
	//if ( count( $docs['pendencias'] ) > 0 ) $filtro_pendencias .= " and doc.docid not in ( '" . implode( "','", $docs['pendencias'] ) . "' ) ";
	

	if(in_array(PERFIL_EXECUTOR,$perfis)) {
		
		$sql = "select
				doc.docid, 
				doc.esdid, 
				mme.tpvid,
				mnm.mnmid, 
				to_char(dmi.dmidataexecucao, 'DD-MM-YYYY') as dmidataexecucao,
				to_char(dmi.dmidatavalidacao, 'DD-MM-YYYY') as dmidatavalidacao
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid and dpestatus = 'A'
			inner join
				pde.monitorameta mnm ON mnm.metid = dmi.metid
			inner join
				 pde.monitorametaentidade mme ON mme.mnmid = mnm.mnmid
			INNER JOIN
				workflow.documento doc ON doc.docid = dmi.docid
			INNER JOIN 
				pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
			INNER JOIN 
				pde.atividade ati ON ati.atiid = mic.atiid	
			where
				dmi.dmistatus= 'A'
			and
				dmi.dmidataexecucao > now()::date
				--now()::date <= dmi.dmddatafimexecucao
			and
				mnm.mnmstatus = 'A'
			and
				mic.micstatus = 'A'
			and
				ati.atistatus = 'A'
			and
				doc.docid is not null
			$filtro_pendencias 
			and
				mme.entid = ".$entidUsuario."
			AND
				doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".WK_ESTADO_DOC_EM_EXECUCAO."' )
			order by
				dpe.dpedatainicio";

		/*$sql = "SELECT distinct mnm.docid, doc.esdid, cle.tpvid 
				FROM pde.monitorameta mnm 
				INNER JOIN pde.monitoraitemchecklist mic on mnm.micid = mic.micid
				INNER JOIN pde.monitorametaentidade cle ON cle.mnmid = mnm.mnmid AND cle.entid = ".$entidUsuario." 
				INNER JOIN workflow.documento doc ON doc.docid = mnm.docid
				INNER JOIN painel.indicador ind ON ind.indid = mic.indid 
				INNER JOIN pde.atividade ati ON ati.atiid = mic.atiid 
				WHERE doc.docid is not null AND 
					  ati.atistatus='A' AND 
					  mnm.mnmprazo >= now()::date AND 
					  $filtro_pendencias
					  doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".WK_ESTADO_DOC_EM_EXECUCAO."' )
			    ";*/
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$arrExecutor = recuparMonitoraMetaExecutor($d['mnmid']);
				if($entidUsuario == $arrExecutor['entid_executor']){
					$docs['futuras'][$d['docid']] = $d['docid'];
				}
			}
		}
		$sql = "select
				doc.docid, 
				doc.esdid, 
				mme.tpvid,
				mnm.mnmid, 
				to_char(dmi.dmidataexecucao, 'DD-MM-YYYY') as dmidataexecucao,
				to_char(dmi.dmidatavalidacao, 'DD-MM-YYYY') as dmidatavalidacao
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid and dpestatus = 'A'
			inner join
				pde.monitorameta mnm ON mnm.metid = dmi.metid
			inner join
				 pde.monitorametaentidade mme ON mme.mnmid = mnm.mnmid
			INNER JOIN
				workflow.documento doc ON doc.docid = dmi.docid
			INNER JOIN 
				pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
			INNER JOIN 
				pde.atividade ati ON ati.atiid = mic.atiid	
			where
				dmi.dmistatus= 'A'
			--and
				--dmi.dmddatafimexecucao < now()::date
				--now()::date > dmi.dmddatafimexecucao
			and
				mnm.mnmstatus = 'A'
			and
				mic.micstatus = 'A'
			and
				ati.atistatus = 'A'
			and
				doc.docid is not null
			$filtro_pendencias 
			and
				mme.entid = ".$entidUsuario."
			AND
				doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".WK_ESTADO_DOC_EM_EXECUCAO."' )
			order by
				dpe.dpedatainicio";
		/*
		$sql = "SELECT distinct mnm.docid, doc.esdid, cle.tpvid 
				FROM pde.monitorameta mnm 
				INNER JOIN pde.monitoraitemchecklist mic on mnm.micid = mic.micid
				INNER JOIN pde.monitorametaentidade cle ON cle.mnmid = mnm.mnmid AND cle.entid = ".$entidUsuario." 
				INNER JOIN workflow.documento doc ON doc.docid = mnm.docid
				INNER JOIN painel.indicador ind ON ind.indid = mic.indid 
				INNER JOIN pde.atividade ati ON ati.atiid = mic.atiid 
				WHERE doc.docid is not null AND 
					  ati.atistatus='A' AND 
					   mnm.mnmprazo < now()::date AND
					  $filtro_pendencias
					  doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".WK_ESTADO_DOC_EM_EXECUCAO."' )
			    ";*/
		//$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$arrExecutor = recuparMonitoraMetaExecutor($d['mnmid']);
				if($entidUsuario == $arrExecutor['entid_executor']){
					$docs['atrazados'][$d['docid']] = $d['docid'];
				}
			}
		}
		
	}
	
	if(in_array(PERFIL_VALIDADOR,$perfis)) {

		$sql = "select
				doc.docid, 
				doc.esdid, 
				mme.tpvid,
				mnm.mnmid,
				to_char(dmi.dmidataexecucao, 'DD-MM-YYYY') as dmidataexecucao,
				to_char(dmi.dmidatavalidacao, 'DD-MM-YYYY') as dmidatavalidacao
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid and dpestatus = 'A'
			inner join
				pde.monitorameta mnm ON mnm.metid = dmi.metid
			inner join
				 pde.monitorametaentidade mme ON mme.mnmid = mnm.mnmid
			INNER JOIN
				workflow.documento doc ON doc.docid = dmi.docid
			INNER JOIN 
				pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
			INNER JOIN 
				pde.atividade ati ON ati.atiid = mic.atiid	
			where
				dmi.dmistatus= 'A'
			and
				mic.micstatus = 'A'
			and
				ati.atistatus = 'A'
			and
				dmi.dmidataexecucao > now()::date
				--now()::date <= dmi.dmddatafimexecucao
			and
				mnm.mnmstatus = 'A'
			and
				doc.docid is not null
			$filtro_pendencias 
			and
				mme.entid = ".$entidUsuario."
			AND
				doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".WK_ESTADO_DOC_EM_VALIDACAO."' )
			order by
				dpe.dpedatainicio";
			
			/*$sql = "SELECT distinct mnm.docid, doc.esdid, cle.tpvid 
					FROM pde.monitorameta mnm 
					INNER JOIN pde.monitoraitemchecklist mic on mnm.micid = mic.micid
					INNER JOIN pde.monitorametaentidade cle ON cle.mnmid = mnm.mnmid AND cle.entid = ".$entidUsuario." 
					INNER JOIN workflow.documento doc ON doc.docid = mnm.docid
					INNER JOIN painel.indicador ind ON ind.indid = mic.indid 
					INNER JOIN pde.atividade ati ON ati.atiid = mic.atiid 
					WHERE doc.docid is not null AND
							ati.atistatus='A' AND 
							mnm.mnmprazo >= now()::date
							$filtro_pendencias
							and doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".WK_ESTADO_DOC_EM_VALIDACAO."' )
					";*/
			$dados = $db->carregar($sql);
		
		if($dados[0]) {
			foreach($dados as $d) {
				$arrValidador = recuparMonitoraMetaValidador($d['mnmid']);
				if($entidUsuario == $arrValidador['entid_validador']){
					$docs['futuras'][$d['docid']] = $d['docid'];
				}
			}
		}
		
		$sql = "select
				doc.docid, 
				doc.esdid, 
				mme.tpvid,
				mnm.mnmid, 
				to_char(dmi.dmidataexecucao, 'DD-MM-YYYY') as dmidataexecucao,
				to_char(dmi.dmidatavalidacao, 'DD-MM-YYYY') as dmidatavalidacao
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid and dpestatus = 'A'
			inner join
				pde.monitorameta mnm ON mnm.metid = dmi.metid
			inner join
				 pde.monitorametaentidade mme ON mme.mnmid = mnm.mnmid
			INNER JOIN
				workflow.documento doc ON doc.docid = dmi.docid
			INNER JOIN 
				pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
			INNER JOIN 
				pde.atividade ati ON ati.atiid = mic.atiid	
			where
				dmi.dmistatus= 'A'
			and
				mic.micstatus = 'A'
			and
				ati.atistatus = 'A'
			--and
				--dmi.dmddatafimexecucao > now()::date
				--now()::date > dmi.dmddatafimexecucao
			and
				mnm.mnmstatus = 'A'
			and
				doc.docid is not null
			$filtro_pendencias 
			and
				mme.entid = ".$entidUsuario."
			AND
				doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".WK_ESTADO_DOC_EM_VALIDACAO."' )
			order by
				dpe.dpedatainicio";
			
			/*$sql = "SELECT distinct mnm.docid, doc.esdid, cle.tpvid 
				FROM pde.monitorameta mnm 
				INNER JOIN pde.monitoraitemchecklist mic on mnm.micid = mic.micid
				INNER JOIN pde.monitorametaentidade cle ON cle.mnmid = mnm.mnmid AND cle.entid = ".$entidUsuario." 
				INNER JOIN workflow.documento doc ON doc.docid = mnm.docid
				INNER JOIN painel.indicador ind ON ind.indid = mic.indid 
				INNER JOIN pde.atividade ati ON ati.atiid = mic.atiid 
				WHERE doc.docid is not null AND 
						ati.atistatus='A' AND 
						mnm.mnmprazo < now()::date
						$filtro_pendencias
						and doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".WK_ESTADO_DOC_EM_VALIDACAO."' )
				";*/
		
		//$dados = $db->carregar($sql);
		
		if($dados[0]) {
			foreach($dados as $d) {
				$arrValidador = recuparMonitoraMetaValidador($d['mnmid']);
				if($entidUsuario == $arrValidador['entid_validador']){
					$docs['atrazados'][$d['docid']] = $d['docid'];
				}
				
			}
		}
		
	}
	
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.min.js"></script>
<script>
			function enviarform(docid) {
			if(document.getElementById('mnmqtd')){
				if(document.getElementById('mnmqtd').value == '') {
					alert('Informe a quantidade');
					return false;
				}
			}
			if(document.getElementById('mvdsituacao_FALSE').checked){
				if(document.getElementById('mvdobservacao').value == '') {
					alert('Observação é obrigatória');
					return false;
				}
			}
			if(document.getElementById('mecopcaoevidencia')){
				if(document.getElementById('arquivo').value == '') {
					alert('Anexe um arquivo');
					return false;
				}
			}
			jQuery("#btn_salvar").attr("disabled","disabled");
			jQuery("#btn_salvar").val("Carregando...");
			document.getElementById('formulario_' + docid).submit();
		  }

			function formulario_filtro_submeter()
			{
				document.formulario_filtro.submit();
			}
</script>
<script type="text/javascript">
		
		function wf_atualizarTela( mensagem, janela )
		{
			janela.close();
			alert( mensagem );
			window.location.reload();
		}
		
		function wf_alterarEstado( aedid, docid, esdid, acao )
		{
			if(acao) acao = acao.toLowerCase();
			if ( !confirm( 'Deseja realmente ' + acao + ' ?' ) )
			{
				return;
			}
			var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/alterar_estado.php' +
				'?aedid=' + aedid +
				'&docid=' + docid +
				'&esdid=' + esdid +
				'&verificacao=a%3A2%3A%7Bs%3A5%3A%22docid%22%3Bs%3A7%3A%221941287%22%3Bs%3A9%3A%22PHPSESSID%22%3Bs%3A32%3A%222cf3a4ce993a7e9aaaf9e94adb9f9af9%22%3B%7D';
			var janela = window.open(
				url,
				'alterarEstado',
				'width=550,height=500,scrollbars=no,scrolling=no,resizebled=no'
			);
			janela.focus();
		}
		
		function wf_exibirHistorico( docid )
		{
			var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
				'?modulo=principal/tramitacao' +
				'&acao=C' +
				'&docid=' + docid;
			window.open(
				url,
				'alterarEstado',
				'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
			);
		}
		
		function abrePainelEstrategico(atiprojeto)
		{
			window.open('/pde/estrategico.php?modulo=principal/painel_estrategico&acao=A&atiprojeto='+atiprojeto,'Indicadores','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
		}
	</script>


<form name="formulario_filtro" action="" method="post">
	<input type="hidden" name="formulario_filtro" value="1"/>
	<table class="tabela" border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<?php if($_SESSION['projeto']): ?>
			<tr>
				<td class="SubTituloDireita">
					Atividade
				</td>
				<td>
					<select id="atiidraiz" name="atiidraiz" class="CampoEstilo" style="width: 250px;">
						<option value="">
							<?php
								if(PROJETO != "PROJETO"){
									$sql = "select atidescricao from pde.atividade where atiid = " . PROJETO;
									echo $db->pegaUm( $sql );
								}
							?>
						</option>
						<?php
						if(PROJETO != "PROJETO"){
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
						}
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
		<?php endif; ?>
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
<tr><td><? wf_gerencimentoFluxoMonitoramentoEstrategico(TIPO_FLUXO_MONITORAMENTO, $docs, (($_REQUEST['cxentrada'])?$_REQUEST['cxentrada']:'pendencias'),$parametros,$filtro_pendencias); ?></td></tr>
</table>