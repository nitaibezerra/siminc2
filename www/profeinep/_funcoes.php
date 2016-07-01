<?
function removerexpediente($dados) {
	global $db;
	$sql = "UPDATE profeinep.anexos SET anxstatus='I' WHERE expid='".$dados["expid"]."'";
	$db->executar($sql);
	
	$sql = "UPDATE profeinep.expediente SET expstatus='I' WHERE expid='".$dados['expid']."'";
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/expediente&acao=A','Remoção efetuada com sucesso');
}

function removerandamento($dados) {
	global $db;
	$sql = "DELETE FROM profeinep.andamentoprocesso WHERE anpid='".$dados['anpid']."'";
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/andamento&acao=A','Remoção efetuada com sucesso');
}

function atualizarandamento($dados) {
	global $db;
	$sql = "UPDATE profeinep.andamentoprocesso
   	 		SET anpdata='".formata_data_sql($dados['anpdata'])."', 
   	 	 	anpdscsituacao='".$dados['anpdscsituacao']."' 
 	 		WHERE anpid='".$dados['anpid']."'";
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/andamento&acao=A','Gravação efetuada com sucesso');
}

function removerdocumento($dados) {
	
	global $db;
	
	$sql = "select nudid from profeinep.anexos WHERE anxstatus='A' and anxid='".$dados["anxid"]."'";
	$nudid = $db->pegaUm($sql);
	
	if($nudid){
		$sql = "UPDATE profeinep.numeracaodocumento SET nudstatus='I' WHERE nudid='".$nudid."'";
		$db->executar($sql);		
	}
	
	$sql = "UPDATE profeinep.anexos SET anxstatus='I' WHERE anxid='".$dados["anxid"]."'";
	$db->executar($sql);
	
	$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$dados["arqid"];
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/documento&acao=A','Arquivo excluído com sucesso.');
}

function desvinculardocumento($dados) {
	
	global $db;
	
	$pflcods = Array(PRF_SUPERUSUARIO,PRF_ADMINISTRADOR);
	if( possuiPerfil( $pflcods ) ){
		$sql = "SELECT nudid FROM profeinep.anexos WHERE anxstatus='A' and anxid='".$dados["anxid"]."'";
		$nudid = $db->pegaUm($sql);
		
		if( $nudid ){
			$sql = "UPDATE profeinep.numeracaodocumento SET nudstatus='A' WHERE nudid='".$nudid."'";
			$db->executar($sql);
		}
			
		$sql = "UPDATE profeinep.anexos SET anxstatus = 'I', nudid = NULL WHERE anxid='".$dados["anxid"]."'";
		$db->executar($sql);
		
		$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$dados["arqid"];
		$db->executar($sql);
		$db->commit();
		
		direcionar('?modulo=principal/documento&acao=A','Arquivo desvinculado com sucesso.');
	}
}

function direcionar($url, $msg) {
	echo "<script>
			alert('".$msg."');
			window.location='".$url."';
		  </script>";
	exit;
}

function inserirexpediente($dados) {
	global $db;
	$sql = "INSERT INTO profeinep.expediente(
            tpeid, prcid, expdscadvogado, expdtinclusaoadvogado, expdscprofeinep, 
            expdtinclusaoprofeinep, expstatus, expdtinclusao, usucpf)
    		VALUES ('".$dados['tpeid']."', 
    				'".$_SESSION['profeinep_var']['prcid']."', 
    				'".substr($dados['expdscadvogado'],0,500)."', 
    				'".formata_data_sql($dados['expdtinclusaoadvogado'])."', 
    				'".substr($dados['expdscprofeinep'],0,500)."', 
            		".(($dados['expdtinclusaoprofeinep'])?"'".formata_data_sql($dados['expdtinclusaoprofeinep'])."'":"NULL").", 
            		'A', 
            		NOW(),
            		'".$_SESSION['usucpf']."') RETURNING expid;";
	$expid = $db->pegaUm($sql);
	$db->commit();
	
	direcionar('?modulo=principal/expediente_lancamento&acao=A&expid='.$expid,'Expediente cadastrado com sucesso.');
}

function atualizarexpediente($dados) {
	global $db;
	$sql = "UPDATE profeinep.expediente
   			SET tpeid='".$dados['tpeid']."', expdscadvogado='".substr($dados['expdscadvogado'],0,500)."', 
   			expdtinclusaoadvogado='".formata_data_sql($dados['expdtinclusaoadvogado'])."', 
       		expdscprofeinep='".substr($dados['expdscprofeinep'],0,500)."', 
       		expdtinclusaoprofeinep=".(($dados['expdtinclusaoprofeinep'])?"'".formata_data_sql($dados['expdtinclusaoprofeinep'])."'":"NULL")."  
 			WHERE expid='".$dados['expid']."'";
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/expediente_lancamento&acao=A&expid='.$dados['expid'],'Expediente atualizado com sucesso.');
}

function inserirprocessoprofeinep($dados) {
	global $db;
	
	$proid = $dados["proid"];
	
	if(!($proid = $db->pegaUm("SELECT proid FROM profeinep.procedencia WHERE prodsc = '".$dados['prodsc']."'")))
	{
		echo "<script>alert('Procedência Inválida');</script>";
		return false;
	}

	$sql= "INSERT INTO profeinep.processoprofeinep(
							--tasid,
							tprid,
							proid,
							prcnumsidoc,
							prcnumeroprocjudicial,
							prcnumeroprocjudantigo,
							prcdtentrada,
							prcdesc,
							prcstatus,
							prcdtinclusao,
							usucpf,
							prcnomeinteressado,
							tipid,
							prcprioritario)
				   VALUES(
				   		  ".$dados["tprid"].",
				   		  '".$proid."',
				          '".$dados["prcnumsidoc"]."',
				           ". (isset($dados["prcnumeroprocjudicial"]) ? "'".$dados["prcnumeroprocjudicial"]."'" : "NULL") .",
				           ". (isset($dados["prcnumeroprocjudantigo"]) ? "'".$dados["prcnumeroprocjudantigo"]."'" : "NULL") .",
				          '".formata_data_sql($dados["prcdtentrada"])."',
				          '".substr($dados["prcdesc"],0,500)."',
				          'A',
				          now(),
				          '".$_SESSION["usucpf"]."',
				          '".$dados['prcnomeinteressado']."',
				          '".$dados['tipid']."',".
				          (($dados['prcprioritario']=='sim')?"TRUE":"FALSE")."
				          ) 
				   RETURNING prcid";
		          
	$prcid = $db->pegaUm($sql);
	
	atualiza_coordenacaoAPOIO( $prcid, 1 );
	
	for($i=0; $i<count($dados["entid"]); $i++) {
		$db->executar("INSERT INTO profeinep.interessadosprocesso(entid,prcid) VALUES(".$dados["entid"][$i].",".$prcid.")");
	}
	
	for($i=0; $i<count($dados["expressaochave"]); $i++) {		
		$db->executar("INSERT INTO profeinep.expressaochave(prcid,excdsc,excstatus,excdtinclusao) VALUES(".$prcid.",'".$dados["expressaochave"][$i]."','A',now())");
	}
	
	if(count($dados['pro_prcid']) > 0) {
		foreach(array_keys($dados['pro_prcid']) as $pro_prcid) {
			$db->executar("INSERT INTO profeinep.processosvinculados(
            	  		   prcid, pro_prcid, usucpf, prvdtvinculacao)
		    	  		   VALUES ('".$prcid."', '".$pro_prcid."', '".$_SESSION['usucpf']."', NOW());");
		}
	}
	
	$db->commit();
	direcionar('?modulo=principal/editarprocesso&acao=A&prcid='.$prcid,'Operação realizada com sucesso!');
}

function atualizarprocessoprofeinep($dados) {
	
	global $db;
	
	if( $dados['tasdsc'] && !is_null($dados['tasdsc']) && $dados['tasdsc'] != '' )
	{
		if(!($dados['tasid'] = $db->pegaUm("SELECT tasid FROM profeinep.tipoassunto WHERE tasdsc = '".$dados['tasdsc']."'")))
		{
			echo "<script>alert('Tema Inválido');</script>";
			return false;
		}
	}
	
	if(!($dados['proid'] = $db->pegaUm("SELECT proid FROM profeinep.procedencia WHERE prodsc = '".$dados['prodsc']."'")))
	{
		echo "<script>alert('Procedência Inválida');</script>";
		return false;
	}
	
	$dados['tacid'] = ($dados['tacid']) ? $dados['tacid'] : 'null';
	
	$sql = "UPDATE profeinep.processoprofeinep 
			SET tacid=".$dados['tacid'].",
				".(($dados['tasid']) ? "tasid='".$dados['tasid']."'," : "")." 
	 	 		tprid='".$dados['tprid']."',
	 	 		proid='".$dados['proid']."',
	 	 		cooid=".(($dados['cooid'])?"'".$dados['cooid']."'":"NULL").",
	 	 		advid=NULL,
       	 		prcnumsidoc='".$dados['prcnumsidoc']."', "
       	 		. (isset($dados['prcnumeroprocjudicial']) ? "prcnumeroprocjudicial='".$dados['prcnumeroprocjudicial']."'," : "")  
       	 		. (isset($dados['prcnumeroprocjudantigo']) ? "prcnumeroprocjudantigo='".$dados['prcnumeroprocjudantigo']."'," : "") . "
       	 		prcdtentrada=".(($dados['prcdtentrada'])?"'".formata_data_sql($dados['prcdtentrada'])."'":"NULL").", 
       	 		prcdesc='".substr($dados['prcdesc'],0,500)."',
       	 		prcnomeinteressado='".$dados['prcnomeinteressado']."',
       	 		tipid=".(($dados['tipid'])?"'".$dados['tipid']."'":"NULL").",
       	 		prcprioritario=".(($dados['prcprioritario']=='sim')?"TRUE":"FALSE")."
       	 	WHERE prcid='".$_SESSION['profeinep_var']['prcid']."';";
       	 		
	$db->executar($sql);
	
	atualizaHistoricoAdovogado( $_SESSION['profeinep_var']['prcid'], $dados['advid'] );
	
	$sql = "DELETE FROM profeinep.expressaochave WHERE prcid='".$_SESSION['profeinep_var']['prcid']."'";
	$db->executar($sql);
	$sql = "DELETE FROM profeinep.interessadosprocesso WHERE prcid='".$_SESSION['profeinep_var']['prcid']."'";
	$db->executar($sql);
	
	//Início -  Pegar os docis que terão seu estado retornado ao anterior
	$sql = "SELECT 
				prcid 
			FROM 
				profeinep.estruturaprocesso
			WHERE
				prcid in (
							SELECT 
								pro_prcid 
							FROM 
								profeinep.processosvinculados 
							WHERE 
								".(count($dados['pro_prcid']) > 0 ? "pro_prcid NOT IN (".implode(",",array_keys($dados['pro_prcid'])).") AND" : " ")." prcid='".$_SESSION['profeinep_var']['prcid']."'
						)";
	
	$arrPrcid = $db->carregarColuna($sql);
	if(is_array($arrPrcid)){
		foreach($arrPrcid as $prcid_desv){
			atualizaHistoricoAdovogado( $prcid_desv, $dados['advid'] );
		}		
	}
	$sql = "SELECT 
				docid 
			FROM 
				profeinep.estruturaprocesso
			WHERE
				prcid in (
							SELECT 
								pro_prcid 
							FROM 
								profeinep.processosvinculados 
							WHERE 
								".(count($dados['pro_prcid']) > 0 ? "pro_prcid NOT IN (".implode(",",array_keys($dados['pro_prcid'])).") AND" : " ")." prcid='".$_SESSION['profeinep_var']['prcid']."'
						)";
	
	$arrDocis = $db->carregar($sql);
	$sql = "SELECT 
				 d.esdid
			FROM 
				workflow.documento d
			INNER JOIN profeinep.estruturaprocesso e ON e.docid = d.docid
			WHERE
				prcid = ".$_SESSION['profeinep_var']['prcid'];
	$esdidPai = $db->pegaUm($sql);
	if($arrDocis){
		foreach($arrDocis as $docid):
			$estadoAntigo = WF_ARQUIVADO;
			$estadoNovo = $esdidPai;
			$comentario = "O Processo foi desvinculado do Número do Processo SIDOC {$dados['prcnumsidoc']}";
			$arrInfo = pegaAcaoEstado($estadoNovo);
			$acaoRealizada = $arrInfo['aeddscrealizada'];
			$acaoARealizadar = $arrInfo['aeddscrealizar'];
			defineEstadoWorkFLow($docid['docid'],$estadoAntigo,$estadoNovo,$comentario,$acaoRealizada,$acaoARealizadar);
		endforeach;
	}
	//Fim -  Pegar os docis que terão seu estado retornado ao anterior
	
	$sql = "DELETE FROM profeinep.processosvinculados WHERE ".(count($dados['pro_prcid']) > 0 ? "pro_prcid NOT IN (".implode(",",array_keys($dados['pro_prcid'])).") AND" : " ")." prcid='".$_SESSION['profeinep_var']['prcid']."'";
	$db->executar($sql);
	
	if($_POST['prazo']){
		$sql = "UPDATE profeinep.estruturaprocesso
				SET espnumdiasrespexterna = {$_POST['prazo']}
				WHERE prcid = ".$_SESSION['profeinep_var']['prcid'];
		$db->executar($sql);
	}
	
	for($i=0; $i<count($dados['entid']); $i++) {
		$db->executar("INSERT INTO profeinep.interessadosprocesso(entid,prcid) VALUES(".$dados['entid'][$i].",".$_SESSION['profeinep_var']['prcid'].")");
	}
	for($i=0; $i<count($dados['expressaochave']); $i++) {		
		$db->executar("INSERT INTO profeinep.expressaochave(prcid,excdsc,excstatus,excdtinclusao) VALUES(".$_SESSION['profeinep_var']['prcid'].",'".$dados['expressaochave'][$i]."','A',now())");
	}
	if(count($dados['pro_prcid']) > 0) {
		
		if($esdidPai==WF_EM_ANALISE_ADVOGADO){
			$advPai = recuperaAdvogado( $_SESSION['profeinep_var']['prcid'] );
		}
		
		foreach(array_keys($dados['pro_prcid']) as $pro_prcid) {
			
			$sql = "select prvid FROM profeinep.processosvinculados where pro_prcid = $pro_prcid AND prcid = {$_SESSION['profeinep_var']['prcid']}";
			$prvid = $db->pegaUm($sql);

			if(!$prvid){
				$db->executar("INSERT INTO profeinep.processosvinculados(
            	  		   prcid, pro_prcid, usucpf, prvdtvinculacao)
		    	  		   VALUES ('".$_SESSION['profeinep_var']['prcid']."', '".$pro_prcid."', '".$_SESSION['usucpf']."', NOW());");
				
				$comentario = "Anexado ao Número do Processo SIDOC {$dados['prcnumsidoc']}";
				$docid = pegaDocidProcesso($pro_prcid);
				$estadoAntigo = (int)pegaEstadoWorkFlow($docid);
				$estadoNovo = "Anexado";
				$acaoRealizada = "Processo Anexado";
				$acaoARealizadar = "Anexar Processo";
				$acaoARealizadar = "";
				if($docid){
					defineEstadoWorkFLow($docid,$estadoAntigo,$estadoNovo,$comentario,$acaoRealizada,$acaoARealizadar);
				}
			}
			atualizaHistoricoCoordenacao( $pro_prcid );
			if($advPai){
				atualizaHistoricoAdovogado( $pro_prcid, $advPai );
			}
		}
		$sql = "UPDATE profeinep.processoprofeinep SET 
					cooid = (SELECT cooid FROM profeinep.processoprofeinep WHERE prcid = ".$_SESSION['profeinep_var']['prcid']." ) 
				WHERE 
					prcid in (".implode(",",array_keys($dados['pro_prcid'])).")";
		$db->executar($sql);
		
	}
	
	$db->commit();
	direcionar('?modulo=principal/editarprocesso&acao=A&prcid='.$_SESSION['profeinep_var']['prcid'],'Operação realizada com sucesso!');
}

function atualizaHistoricoAdovogado( $prcid, $advid ){
	
	global $db;
	
	$adv = recuperaAdvogado( $prcid );
	
	if( $advid != '' && $advid != $adv ){
		$sql = "INSERT INTO profeinep.historicoadvogados(prcid,advid) VALUES (".$prcid.",".$advid.")";
		$db->executar($sql);
	}
	
}

function recuperaAdvogado( $prcid ){
	
	global $db;
	
	return $db->pegaUm('SELECT 
					advid 
				FROM 
					profeinep.historicoadvogados
				WHERE 
					hadid = (SELECT max(hadid) FROM profeinep.historicoadvogados WHERE prcid = '.$prcid.') ');
}

function pegaAcaoEstado($esdid){
	global $db;
	
	$sql = "select
				aeddscrealizar,
				aeddscrealizada
			from
				 workflow.acaoestadodoc
			where
				esdiddestino = $esdid";
	return $db->pegaLinha($sql);
	
}

function monta_abas_processo($tipoprocesso) {
	$abas_editarprocesso[] = array("id" => 1, "descricao" => "Lista de Processos", "link" => "/profeinep/profeinep.php?modulo=inicio&acao=C");
	$abas_editarprocesso[] = array("id" => 2, "descricao" => "Dados do Processo", "link" => "/profeinep/profeinep.php?modulo=principal/editarprocesso&acao=A&prcid=".$_SESSION['profeinep_var']['prcid']);
	$abas_editarprocesso[] = array("id" => 3, "descricao" => "Histórico", "link" => "/profeinep/profeinep.php?modulo=principal/movimentacao&acao=A");
	$abas_editarprocesso[] = array("id" => 4, "descricao" => "Documentos", "link" => "/profeinep/profeinep.php?modulo=principal/documento&acao=A");
	$abas_editarprocesso[] = array("id" => 5, "descricao" => "Gerar Numeração", "link" => "/profeinep/profeinep.php?modulo=principal/geraNumeracao&acao=A");
	$abas_editarprocesso[] = array("id" => 6, "descricao" => "Tramitação em Lote", "link" => "/profeinep/profeinep.php?modulo=principal/tramitaProcessos&acao=A");
	return $abas_editarprocesso;
}

function monta_cabecalho_profeinep($prcid) {
	global $db;
	
	$processoprofeinep = $db->pegaLinha("SELECT prcnumsidoc, prcnomeinteressado, esddsc FROM profeinep.processoprofeinep prc 
									  LEFT JOIN profeinep.estruturaprocesso esp ON prc.prcid = esp.prcid 
    								  LEFT JOIN workflow.documento doc ON doc.docid = esp.docid 
    								  LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid  
									  WHERE prc.prcid='".$prcid."'");
	
	// efetuar select e retornar cabecalho
	$titulo_modulo = "PROFE/INEP";
	monta_titulo( $titulo_modulo,'Consultória Jurídica');
	
	echo "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
	echo "<tr>";
	echo "<td class='SubTituloDireita' width='25%'>Nº do Processo :</td><td>".$processoprofeinep['prcnumsidoc']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class='SubTituloDireita' width='25%'>Interessado :</td><td>".$processoprofeinep['prcnomeinteressado']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class='SubTituloDireita' width='25%'>Localização em andamento :</td><td>".$processoprofeinep['esddsc']."</td>";
	echo "</tr>";
	echo "</table>";
}

function inserirandamento($dados) {
	global $db;
	$sql = "INSERT INTO profeinep.andamentoprocesso(
            prcid, usucpf, anpdata, anpdscsituacao, anpstatus, anpdtinclusao)
     		VALUES ('".$_SESSION['profeinep_var']['prcid']."', 
     				'".$_SESSION['usucpf']."', 
     				'".formata_data_sql($dados['anpdata'])."', 
     				'".$dados['anpdscsituacao']."', 
     				'A', 
     				NOW());";
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/andamento&acao=A','Gravação efetuada com sucesso');
}

function inserirarquivoprofeinep($dados) {
	global $db;
	// obtém o arquivo
	$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		echo "<script>
				alert('O arquivo não foi enviado com sucesso.');
				window.location = '?modulo=principal/inicio&acao=C';
			  </script>";
		exit;
	}
	
	/*if(!$dados['tpaid']) {
		echo "<script>
				alert('Selecione o tipo de arquivo.');
				window.location = '?modulo=principal/documento&acao=A';
			  </script>";
		exit;
	}*/
	
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	
//	$sql = "SELECT DISTINCT
//					tpdgeranumeracao
//				FROM 
//					profeinep.tipodocumento
//				WHERE
//					tpdid = ".$dados['tpdid'];
//	$num = $db->pegaUm($sql);
//	if( $num == 'S' ){
//		//Insere numeração do documento
//		$sql = "INSERT INTO profeinep.numeracaodocumento(
//	            				tpdid, nudnumero, nudusucpf, nuddatageracao, nudano)
//	    		VALUES ({$dados['tpdid']}, {$dados['nudnumero']}, {$_SESSION['usucpf']}, now(), ".Date('Y').")
//	    		RETURNING nudid";
//		$nudid = $db->pegaUm($sql);
//	}else{
//		$nudid = 'null';
//	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
	values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".substr($dados["anxdesc"],0,255)."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	if(!$dados['nudid']){
		$dados['nudid'] = 'null';
	}
	
	//Insere o registro na tabela obras.arquivosobra
	$sql = "INSERT INTO profeinep.anexos(
	            expid, arqid, prcid, anxdesc, anxtipo, anxstatus, anxdtinclusao, tpdid, nudid)
    		VALUES (
    		".(($dados['expid'])?"'".$dados['expid']."'":"NULL").", 
    		".$arqid.", 
    		".$_SESSION['profeinep_var']['prcid'].", 
    		'".substr($dados['anxdesc'],0,255)."', 
    		'".$dados['anxtipo']."', 
    		'A',
    		NOW(),
    		".$dados['tpdid'].",
    		".$dados['nudid'].");";
	$db->executar($sql);

	// se não existir a pasta, cria no servidor
	if(!is_dir('../../arquivos/profeinep/')) {
		mkdir(APPRAIZ.'/arquivos/profeinep/', 0777);
	}
	
	// se não existir o arquivo, cria no servidor
	if(!is_dir('../../arquivos/profeinep/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/profeinep/'.floor($arqid/1000), 0777);
	}
	
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			if( $original_x == 0 ){
				echo "<script>
						alert('O arquivo não foi enviado com sucesso.');
						window.location = '?modulo=principal/documento&acao=A';
					  </script>";
				exit;
			}
			$original_y = $height;
			if( $original_y == 0 ){
				echo "<script>
						alert('O arquivo não foi enviado com sucesso.');
						window.location = '?modulo=principal/documento&acao=A';
					  </script>";
				exit;
			}
			// se a largura for maior que altura
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$this->simec->rollback();
				direcionar('?modulo=inicio&acao=C','Problemas no envio do arquivo.');
			}
	}
	if ( file_exists( $caminho ) ) {
		$db->commit();
	}else{
		$db->rollback();
		direcionar('?modulo=inicio&acao=C','Problemas no envio localizar o arquivo no servidor.');
	}
	switch($dados['anxtipo']) {
		case 'P':
			direcionar('?modulo=principal/documento&acao=A','Gravação efetuada com sucesso');
			break;
		case 'E':
			direcionar('?modulo=principal/expediente_lancamento&acao=A&expid='.$dados['expid'],'Gravação efetuada com sucesso');
			break;
	}
}

function pegarEstadoDocumento( $docid )
{
	global $db;
	$docid = (integer) $docid;
	$sql = "
		select esdid 
		from workflow.documento 
		where
			docid = " . $docid . "
	";
	return (integer) $db->pegaUm( $sql );
}

function profeinep_download_arquivo( $param ){
	global $db;
	$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$param['arqid'];
    $arquivo = current($db->carregar($sql));
    $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
    if ( !is_file( $caminho ) ) {
        $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
    }
    $filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
    header( 'Content-type: '. $arquivo['arqtipo'] );
    header( 'Content-Disposition: attachment; filename='.$filename);
    readfile( $caminho );
    exit();
}


function pegarDocid( $prcid )
{
	global $db;
	$entid = (integer) $entid;
	$sql = "SELECT docid
			FROM profeinep.estruturaprocesso
			WHERE prcid = '" . $prcid . "'";
	return (integer) $db->pegaUm( $sql );
}

function criarDocumento( $prcid )
{
	global $db;
	if(!pegarDocid($prcid)) {
		$tpdid = TIPODOC;
		$sqlDescricao = "SELECT	prcnumsidoc
						 FROM profeinep.processoprofeinep
						 WHERE prcid = '" . $prcid . "'";
		$descricao = $db->pegaUm( $sqlDescricao );
		$docdsc = "Número do processo na PROFE/INEP :" . $descricao;
		// cria documento
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );	
		$sql = "INSERT 
				INTO profeinep.estruturaprocesso 
				(prcid, docid, usucpf) 
				VALUES ('".$prcid."', '".$docid."', '".$_SESSION['usucpf']."')";	
		$db->executar( $sql );		
		$db->commit();
		return $docid;		
	}
}

function testaRespCordPerfil(){
	
	global $db;
	
	$sql = "SELECT 
				p.pflcod 
			FROM seguranca.perfil p 
			INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
			INNER JOIN profeinep.tprperfil tpr ON tpr.pflcod = p.pflcod
			WHERE 
				pu.usucpf = '". $_SESSION['usucpf'] ."' 
				AND p.pflstatus = 'A' 
				AND p.sisid =  '". $_SESSION['sisid'] ."'";
	$teste = $db->carregarColuna($sql);
	return is_array($teste)&&count($teste)>0;
}

function verificaPerfilProfeinep($estid = false, $cooid = false) {
	global $db;
	$sql = "SELECT p.pflcod FROM seguranca.perfil p 
			LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
			WHERE pu.usucpf = '". $_SESSION['usucpf'] ."' and p.pflstatus = 'A' and p.sisid =  '". $_SESSION['sisid'] ."'";
	$perfilid = $db->pegaUm($sql);
	$_SESSION['profeinep']['perfilid'] = $perfilid;
	if($db->testa_superuser() || $perfilid == PRF_ADMPROFEINEP) {
		// permissao para remover e gravar
		$permissoes['remover'] = true;
		$permissoes['gravar'] = true;
		$permissoes['selecionaradvogado'] = true;
	} else {
		// Analisando permissão de acesso de acordo com o estado do documento
		switch($perfilid) {
			
			case PRF_ADMINISTRADOR:
				$permissoes['remover'] = true;
				$permissoes['gravar'] = true;
				$permissoes['selecionaradvogado'] = true;
				break;
			
			case PRF_PROCURADOR:
				$permissoes['remover'] = true;
				$permissoes['gravar'] = true;
				$permissoes['selecionaradvogado'] = true;
				break;
				
			case PRF_APOIO:
				$permissoes['remover'] = true;
				$permissoes['gravar'] = true;
				$permissoes['selecionaradvogado'] = true;
				break;
				
			case PRF_GABINETE:
				$permissoes['remover'] = true;
				$permissoes['gravar'] = true;
				$permissoes['selecionaradvogado'] = true;
				break;
			
			default:
				$permissoes['remover'] = false;
				$permissoes['gravar'] = false;
		}
	}
	return $permissoes;
}

function verificaPerfilCoordenacaoProfeinep( $cooid = false ) {
	global $db;
	
	if(!$cooid) return true;
		
	$sql = "SELECT coonid FROM profeinep.usuarioresponsabilidade
			WHERE rpustatus = 'A' AND coonid = $cooid AND usucpf = '{$_SESSION['usucpf']}'";
	
	$cooid = $db->pegaUm($sql);
	
	return $cooid != '' ? true : false;
}

/* Funções das ações do WORKFLOW */

function profeinep_verificarcoordenacao($prcid) {
	global $db;

	if($db->pegaUm("SELECT cooid FROM profeinep.processoprofeinep WHERE prcid='".$prcid."'")){
		$verificaAnexo = profeinep_verificarprocessoanexo($prcid);
		if($verificaAnexo){
			return true;
		} else {
			return "É necessário anexar um arquivo.";
		}
	}
	return false;
}

function profeinep_verificaCGAC($prcid, $cooid){
	if(1==$cooid){
		return profeinep_verificarprocessoanexo($prcid);
	}else{
		return false;
	}
}

function profeinep_verificaCGNLJ($prcid, $cooid){
	
	if(2==$cooid){
		return profeinep_verificarprocessoanexo($prcid);
	}else{
		return false;
	}
}

function profeinep_verificaCGEPD($prcid, $cooid){
	
	if(3==$cooid){
		return profeinep_verificarprocessoanexo($prcid);
	}else{
		return false;
	}
}

function profeinep_verificarprocessoanexo($prcid) {
	global $db;
	
	$docid = pegaDocidProcesso($prcid);
	$esdid = pegaEstadoWorkFlow($docid);
	
	if(possuiPerfil(array(PRF_ADMINISTRADOR,PRF_SUPERUSUARIO)) || $esdid = WF_EM_ANALISE_GABINETE){
		return true;	
	}
	
	$sql = "select
				coalesce(a.nudid,0) as nudidanexo, 
				nd.nudnumero
			from profeinep.processoprofeinep p
			inner join profeinep.numeracaodocumento nd on p.prcid = nd.prcid
			left join profeinep.anexos a on a.nudid = nd.nudid  and a.anxstatus = 'A'
			where nudstatus = 'A' and p.prcid='".$prcid."' ";
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	
	$boTramitar = false;	
	foreach ($arDados as $dados) {
		if($dados['nudnumero']){
			if($dados['nudidanexo'] > 0){
				$boTramitar = true;
			} else {
				$boTramitar = false;
			}
		}
	}
	
	if( $boTramitar && profeinep_verificarprocessodocumento($prcid) ){
		$boTramitar = true;
	}else{
		$boTramitar = false;
	}
	
	return $boTramitar;
}

function verificarProcessoAnexoNovo($prcid) {
	
	global $db;
	
	$sql = "select
				coalesce(a.nudid,0) as nudidanexo, 
				nd.nudnumero
			from profeinep.processoprofeinep p
			inner join profeinep.numeracaodocumento nd on p.prcid = nd.prcid
			left join profeinep.anexos a on a.nudid = nd.nudid  and a.anxstatus = 'A'
			where nudstatus = 'A' and p.prcid='".$prcid."' ";
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	
	$boTramitar = true;	
	foreach ($arDados as $dados) {
		if($dados['nudnumero']){
			if($dados['nudidanexo'] > 0){
				$boTramitar = true;
			} else {
				$boTramitar = false;
			}
		}
	}
	
	return $boTramitar;
}

function profeinep_verificarprocessodocumento($prcid) {
	global $db;
	
	$docid = pegaDocidProcesso($prcid);
	$esdid = pegaEstadoWorkFlow($docid);
	
	if( possuiPerfil(array(PRF_ADMINISTRADOR,PRF_SUPERUSUARIO)) ){
		return true;	
	}
	
	$sql = "SELECT
				count(a.nudid) as nudidanexo
			FROM profeinep.processoprofeinep p
			INNER JOIN profeinep.numeracaodocumento nd ON p.prcid = nd.prcid
			INNER JOIN profeinep.anexos 	          a ON a.nudid = nd.nudid  AND a.anxstatus = 'A'
			WHERE 
				nudstatus   = 'A' 
				AND p.prcid = '".$prcid."' ";
	$qtdAnexos = $db->pegaUm($sql);
	
	$boTramitar = true;	
	if($qtdAnexos > 0){
		$boTramitar = true;
	} else {
		$boTramitar = false;
	}
	
	return $boTramitar;
}

function profeinep_verificaradvogado($prcid) {
	global $db;
	
	if($db->pegaUm("SELECT max(advid) FROM profeinep.historicoadvogados WHERE prcid='".$prcid."'")){
		$verificaAnexo = profeinep_verificarprocessoanexo($prcid);
		if($verificaAnexo){
			return true;
		} else {
			return "É necessário anexar um arquivo.";
		}
	}
	return false;
}

function verificarAdvogadoNovo($prcid) {
	
	global $db;
	
	$advid = pegaAdvogado( $prcid );
	
	return $advid!='' ? true : false;
}

function pegaAdvogado( $prcid ){
	
	global $db;
	
	return $db->pegaUm("SELECT max(advid) FROM profeinep.historicoadvogados WHERE prcid='".$prcid."'");
}

function carregarAdvogados($dados) {
	global $db;
	if($dados['cooid']){
		$sqlAdvogado = "SELECT adv.advid as codigo, ent.entnome as descricao 
						FROM profeinep.advogados adv 
						INNER JOIN entidade.entidade ent ON ent.entid = adv.entid 
						INNER JOIN profeinep.advogadosxcoordenacao adc ON adc.advid = adv.advid
						WHERE adc.coonid='".$dados['cooid']."' AND ent.entstatus = 'A' AND adv.advstatus = 'A'
					   	ORDER BY ent.entnome";
		
	}else{
		$sqlAdvogado = array( array( "codigo" => "", "descricao" => "Favor selecioanr a Coordenação.") );
	}
	$db->monta_combo('advid', $sqlAdvogado, 'S', 'Selecione...', '', '', '', 300, 'N','advid');
}

function defineEstadoWorkFLow($docid,$estadoAntigo,$estadoNovo,$comentario = null,$acaoRealizada = null, $acaoARealizadar = null, $arrTabelaCampo = null, $sisid = null){
	global $db;
	
	if(!is_array($docid)){
		$arrDocid[] = $docid;
	}else{
		$arrDocid = $docid;
	}
	
	if(!$arrDocid){
		return false;
	}
	
	$sisid = !$sisid ? $_SESSION['sisid'] : $sisid;
	
	if(!$sisid){
		return false;
	}
	
	if(!is_numeric($estadoAntigo)){
		$sql = "select 
					esdid 
				from 
					workflow.estadodocumento esd 
				INNER JOIN 
					workflow.tipodocumento tpd ON esd.tpdid = esd.tpdid 
				WHERE 
					sisid = $sisid
				AND
					esddsc = '$estadoAntigo'
				AND
					tpdstatus = 'A'";
		$estadoAntigo = $db->pegaUm($sql);
	}
	
	if(!$estadoAntigo){
		return false;
	}
	
	if(!is_numeric($estadoNovo)){
		$sql = "select 
					esdid 
				from 
					workflow.estadodocumento esd 
				INNER JOIN 
					workflow.tipodocumento tpd ON esd.tpdid = esd.tpdid 
				WHERE 
					sisid = $sisid
				AND
					esddsc = '$estadoNovo'
				AND
					tpdstatus = 'A'";
		$estadoNovo = $db->pegaUm($sql);
	}
	
	if(!$estadoNovo){
		return false;
	}
	
	foreach($arrDocid as $docid):
		
		$sqlAcao = "select 
						aedid
					from
						workflow.acaoestadodoc
					where
						esdidorigem = $estadoAntigo
					and
						esdiddestino = $estadoNovo
					and
						aedstatus = 'A'
					and
						aeddscrealizada = '$acaoRealizada'";
		$aedid = $db->pegaUm($sqlAcao);
		if(!$aedid){
			$sqlAcao = "insert into workflow.acaoestadodoc
						(esdidorigem,esdiddestino,aeddscrealizar,aedstatus,aeddscrealizada,esdsncomentario,aedcondicao,aedobs,aedposacao,aedvisivel)
					VALUES
						($estadoAntigo,$estadoNovo,'$acaoARealizadar','A','$acaoRealizada',true,NULL,NULL,NULL,false)
					RETURNING
						aedid";
			$aedid = (integer) $db->pegaUm( $sqlAcao );
		}
		
		$sqlHistorico = "insert into workflow.historicodocumento
							( aedid, docid, usucpf, htddata )
						values 
							( $aedid , " . $docid . ", '" . (!$arrTabelaCampo['workflow']['historicodocumento']['usucpf'] ?  $_SESSION['usucpf'] : $arrTabelaCampo['workflow']['historicodocumento']['usucpf']) . "', ".(!$arrTabelaCampo['workflow']['historicodocumento']['htddata'] ? "now()" : "'".$arrTabelaCampo['workflow']['historicodocumento']['htddata']."'" )." )
						returning 
							hstid";
		$hstid = (integer) $db->pegaUm( $sqlHistorico );
		if($comentario && $hstid){
			$sqlCommentario = "insert into workflow.comentariodocumento
								( docid, cmddsc, cmdstatus, cmddata, hstid )
							values 
								( $docid , '" . $comentario . "', 'A', now(), $hstid )";
			$db->executar( $sqlCommentario );
		}
		$sqlUpdate = "	update 
							workflow.documento
						set
							esdid = $estadoNovo
						where
							docid = $docid";
		$db->executar($sqlUpdate);
	endforeach;
	if($db->commit()){
		return true;
	}
}

function atualizaDocidProcessoAnexado()
{
	global $db;
	
	$sql = "select 
				pro_prcid, 
				prvdtvinculacao,
				doc.docid,
				doc.esdid,
				prcnumsidoc,
				prv.usucpf,
				esddsc
			from 
				profeinep.processosvinculados prv
			inner join
				profeinep.processoprofeinep prc ON prc.prcid = prv.prcid
			inner join
				profeinep.estruturaprocesso epr ON epr.prcid = pro_prcid
			inner join
				workflow.documento doc ON doc.docid = epr.docid
			inner join
				workflow.estadodocumento est ON est.esdid = doc.esdid
			where 
				esddsc != 'Anexado'";
	
	$arrDados = $db->carregar($sql);
	
	if(!$arrDados){
		return "Não é necessário atualizar Processo Anexados!";
	}
	
	$sql = "select 
				esdid
			from
				workflow.estadodocumento esd
			inner join
				workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
			where
				sisid = {$_SESSION['sisid']}
			and
				esddsc = 'Anexado'";
	$esdidAnexado = $db->pegaUm($sql);
	
	if(!$esdidAnexado){
		return "Não existe estado Anexado disponível para o documento!";
	}
		
	foreach($arrDados as $dado){		
		
		if($dado['esdid'] == $esdidAnexado){
			$estadoAntigo = pegaEstadoAnteriorWorkFlow($dado['docid']);
		}else{
			$estadoAntigo = $dado['esdid'];
		}
		$estadoNovo = $esdidAnexado;
		
		$arrTabelaCampo['workflow']['historicodocumento']['usucpf'] = $dado['usucpf'];
		$arrTabelaCampo['workflow']['historicodocumento']['htddata'] = date("Y-m-d H:i:s",strtotime($dado['prvdtvinculacao']));
		
		$acaoARealizadar = "";
		
		if(!defineEstadoWorkFLow($dado['docid'],$estadoAntigo,$estadoNovo,"Processo Anexado ao Número do Processo SIDOC ".$dado['prcnumsidoc'],"Processo Anexado",$acaoARealizadar,$arrTabelaCampo)){
			return "Não foi possível atualizar o(s) Processo(s)!";
		}
	}
	return count($arrDados)." Processo(s) Atualizado(s) com Sucesso!";

}

function pegaLocalFisicoAnterior( $prcid ){
	
	global $db;
	
	$sql = "SELECT 
				coonid
			FROM 
				profeinep.historicocoordenacoes
			WHERE
				hcoid = ( SELECT max(hcoid) FROM profeinep.historicocoordenacoes WHERE prcid = $prcid )";
	return $db->pegaUm($sql);
}

function pegaHistoricoLocalFisico( $prcid ){
	
	global $db;
	
	$sql = "SELECT 
				coonid
			FROM 
				profeinep.historicocoordenacoes
			WHERE
				prcid = $prcid ";
	return $db->carregarColuna($sql);
}


function pegaCoordUsu(){
	
	global $db;
	
	$sql = "SELECT 
				coonid
			FROM
				profeinep.usuarioresponsabilidade
			WHERE
				usucpf = '".$_SESSION['usucpf']."' AND rpustatus='A'";
	
	$arr = $db->carregarColuna($sql);
	
	return is_array($arr) ? $arr : Array(); 
}

function pegaCoordProc( $prcid ){
	
	global$db;
	
	$sql = "SELECT
				cooid
			FROM
				profeinep.processoprofeinep
			WHERE
				prcid = $prcid";
	
	return $db->pegaUm($sql);
}

function pegaEstadoAnteriorWorkFlow($docid){
	global $db;

	if(!$docid){
		return false;
	}
	
	$estadoAtual = pegaEstadoWorkFlow($docid);
	
	if(!$estadoAtual){
		return false;
	}
	
	$sql = "SELECT 
				DISTINCT ed.*, hstid
			FROM
				workflow.historicodocumento htd
			INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = htd.aedid
			INNER JOIN workflow.estadodocumento ed ON ed.esdid = aed.esdidorigem
			WHERE
				docid = $docid
				AND esdid != $estadoAtual
			ORDER BY
				hstid desc
			LIMIT 1";
	$estado = $db->pegaUm($sql);
	
	if($estado){
		return $estado;
	}else{
		$sql = "SELECT 
					esdid
				FROM
					workflow.estadodocumento esd
				INNER JOIN workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
				WHERE
					sisid = {$_SESSION['sisid']}
				ORDER BY
					esdordem
				LIMIT 1";
		return $db->pegaUm($sql);
	}
	
	return $db->pegaUm($sql);
}

function pegaHistoricoEstadoWorkFlow($docid){
	global $db;

	if(!$docid){
		return array();
	}
	
	$estadoAtual = pegaEstadoWorkFlow($docid);
	
	if(!$estadoAtual){
		return array();
	}
	
	$sql = "SELECT 
				DISTINCT ed.*, hstid
			FROM
				workflow.historicodocumento htd
			INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = htd.aedid
			INNER JOIN workflow.estadodocumento ed ON ed.esdid = aed.esdidorigem
			WHERE
				docid = $docid
				AND esdid != $estadoAtual
			ORDER BY
				hstid desc";
	
	$estado = $db->carregarColuna($sql);
	
	if(!$estado){
		return array();
	}else{
		return $estado;
	}
}

function pegaDocidProcesso($prcid){
	global $db;
	
	$sql = "select docid from profeinep.estruturaprocesso where prcid = $prcid";
	return $db->pegaUm($sql);
	
}

function pegaEstadoWorkFlow($docid){
	global $db;
	
	$sql = "select esdid from workflow.documento where docid = $docid";
	
	return $db->pegaUm($sql);
	
}

function recuperaCoordenacaoResponssavel(){
	global $db;
	
	if( !$db->testa_superuser() ){
		$sql = "SELECT 
					coordid 
				FROM 
					catalogocurso.usuarioresponsabilidade
				WHERE 
					usucpf = ".$_SESSION['usucpf'];
		
		return $db->carregarColuna($sql);
	}
	
	return $db->testa_superuser();
	
}

function encaminhaParaAdvogado()
{
	global $db;
	
	$docid = pegaDocidProcesso($_SESSION['profeinep_var']['prcid']);
	$advid = $_REQUEST['advid'] ? $_REQUEST['advid'] : 'null';
	
	$sql = "INSERT INTO profeinep.historicoadvogados(prcid,advid) VALUES (".$_SESSION['profeinep_var']['prcid'].",".$advid.") RETURNING hadid";
	$hadid = $db->pegaUm($sql);
	$db->commit();
	
	include APPRAIZ . 'includes/workflow.php';
	
	$cooid = $db->pegaUm("SELECT cooid FROM profeinep.processoprofeinep WHERE prcid='".$_SESSION['profeinep_var']['prcid']."'");
	
	wf_alterarEstado( $docid, ACAO_ENCAMINHAR_PARA_ADVOGADO, $cmddsc = 'Encaminhado para advogado', Array( "prcid" => $_SESSION['profeinep_var']['prcid'], "cooid" => $cooid, "advid" => $advid) );
	echo "<script>alert('função')</script>";
	$sql = "UPDATE profeinep.historicoadvogados SET hstid = (SELECT max(hstid) FROM workflow.historicodocumento WHERE docid = $docid ) WHERE hadid = $hadid";
	$db->executar($sql);
	$db->commit();
	
	return true;
}

function alteraAdvogado( $prcid, $advid, $emlote = 0 ){
	
	global $db;
	echo "<script>alert('pos ação')</script>";
//	$sql = "INSERT INTO profeinep.historicoadvogados(prcid,advid) VALUES (".$_SESSION['profeinep_var']['prcid'].",".$advid.")";
//	$db->executar($sql);
//	$db->commit();
	return true;
}

function carregarWorkflow()
{
	global $db;
	
	$docid = $_REQUEST['docid'];
	$advid = $_REQUEST['advid'] ? $_REQUEST['advid'] : 'null';
	
	$db->executar("UPDATE profeinep.processoprofeinep SET advid = $advid WHERE prcid='".$_SESSION['profeinep_var']['prcid']."'");
	$db->commit();
	
	include APPRAIZ . 'includes/workflow.php';
	
	wf_desenhaBarraNavegacao( $docid, array( 'prcid' => $_SESSION['profeinep_var']['prcid'] ) );
}

function carregarWorkflow2()
{
	global $db;
	
	$docid = $_REQUEST['docid'];
	$cooid = $_REQUEST['cooid'] ? $_REQUEST['cooid'] : 'null';
	
	$db->executar("UPDATE profeinep.processoprofeinep SET cooid = $cooid WHERE prcid='".$_SESSION['profeinep_var']['prcid']."'");
	$db->commit();
	
	include APPRAIZ . 'includes/workflow.php';
	
	wf_desenhaBarraNavegacao( $docid, array( 'prcid' => $_SESSION['profeinep_var']['prcid'], 'cooid' => $cooid, 'usar_acaoPossivel2' => true  ) );
}

function possuiPerfil( $pflcods )
{
	global $db;
	
	if( $db->testa_superuser() ){
		return true;
	}

	if ( is_array( $pflcods ) ){
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	} else {
		$pflcods = array( (integer) $pflcods );
	} if ( count( $pflcods ) == 0 ) {
		return false;
	}
	$sql = "select
				count(*)
			from seguranca.perfilusuario
			where
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
}

function verificaSeMudaCor($prcid){
	global $db;
	
	if($prcid){
		$sql = "select
					coalesce(a.nudid,0) as nudidanexo, 
					nd.nudnumero
					from profeinep.processoprofeinep p
				inner join profeinep.numeracaodocumento nd on p.prcid = nd.prcid
				left join profeinep.anexos a on a.nudid = nd.nudid  and a.anxstatus = 'A'
				where nudstatus = 'A' and p.prcid='".$prcid."' ";
		$arDados = $db->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		$boMudaCorAba = true;	
		foreach ($arDados as $dados) {
			if($dados['nudnumero']){
				if($dados['nudidanexo'] > 0){
					$boMudaCorAba = true;
				} else {
					$boMudaCorAba = false;
				}
			}
		}
		
		return $boMudaCorAba;
		
	}
	
}

function reiniciaTramitacao( $prcid, $emlote = '0' ){
	
	global $db;
	
	$sql = "UPDATE profeinep.estruturaprocesso SET
				espdtrespexterna = null,
				espnumdiasrespexterna = null
			WHERE
				prcid = $prcid";
	
	$db->executar($sql);
	
	atualiza_coordenacaoAPOIO( $prcid, $emlote );
	
	return true;
}

function verificaPrazo( $prcid ){
	
	global $db;
	
	$sql = "SELECT
				espnumdiasrespexterna 
			FROM
				profeinep.estruturaprocesso esp
			WHERE 
				prcid = $prcid
			GROUP BY
				esp.espnumdiasrespexterna";
	$prazo = $db->pegaUm($sql);
	
	if( $prazo != '' ){
		return true;
	}else{
		return 'Defina o prazo para resposta externa.';
	}
}

function atualiza_coordenacaoAPOIO( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COONID_COORDENAÇÃO_DE_APOIO_A_PROJUR,$emlote);
	
	return true;
}

function atualiza_coordenacaoPROCURADOR( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COONID_PROCURADORES_PROJUR,$emlote);
	
	return true;
}

function atualiza_coordenacaoGABINETE( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COONID_GABINETE_PROJUR,$emlote);
	
	return true;
}

function retornaEstadoAnterior( $prcid ) {
	global $db;
	
	$sql = "SELECT ep.docid FROM profeinep.processoprofeinep prc 
			INNER JOIN profeinep.estruturaprocesso ep ON ep.prcid = prc.prcid 
			WHERE prc.prcid='".$prcid."'";
	
	$docid = $db->pegaUm($sql);
	
	$sql = "SELECT a.esdidorigem FROM workflow.historicodocumento h 
			INNER JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid 
			WHERE h.docid='".$docid."' ORDER BY htddata DESC LIMIT 2";
	
	$esdDes = $db->carregarColuna($sql);
	
	if($esdDes[1]) {
		
		$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".ESDID_DESARQUIVADO."' AND esdiddestino='".$esdDes[1]."'";
		$aedidAtual = $db->carregarColuna($sql);
		
		if(count($aedidAtual)>1) {
			
			$sql = "SELECT co.aedid, es.docid, prc.cooid FROM profeinep.processoprofeinep prc 
			INNER JOIN profeinep.estruturaprocesso es ON prc.prcid = es.prcid 
			INNER JOIN profeinep.coordenacao co ON co.coonid = prc.cooid 
			WHERE prc.prcid='".$prcid."'";
			
			$arrProcesso = $db->pegaLinha($sql);
			
			if($arrProcesso) {
				wf_alterarEstado( $arrProcesso['docid'], $arrProcesso['aedid'], '', array('prcid' => $prcid, 'cooid' => $arrProcesso['cooid'], 'emlote' => '1') );
			}
			
		} elseif(count($aedidAtual)==1) {
			wf_alterarEstado( $docid, current($aedidAtual), '', array('prcid' => $prcid, 'emlote' => '1') );
		}
		
	}
	
	return true;
	
}


function retornaLocalFisicoAnterior( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,pegaLocalFisicoAnterior( $prcid ),$emlote);
	
	return true;
}

function atualizaHistoricoCoordenacao( $prcid ){
	
	global $db;
	
	$sql = "SELECT 
				cooid 
			FROM 
				profeinep.processoprofeinep 
			WHERE 
				prcid = $prcid ";
	if( $db->pegaUm($sql) != '' ){
		$sql = "INSERT INTO profeinep.historicocoordenacoes(prcid, coonid, hstid) 
				SELECT 
					prc.prcid, 
					cooid,
					hstid 
				FROM 
					profeinep.processoprofeinep prc
				INNER JOIN profeinep.estruturaprocesso esp ON esp.prcid = prc.prcid
				LEFT JOIN (SELECT
								max(hstid) as hstid,
								docid
							FROM
								workflow.historicodocumento
							GROUP BY docid ) hdc ON hdc.docid = esp.docid
				WHERE 
					prc.prcid = $prcid ";
		$db->executar($sql);
		$db->commit();
	}
}

function atualizaCoordenacao($prcid,$cooid,$emlote = '0'){

	global $db;
	
	$sql = "UPDATE profeinep.processoprofeinep SET
				cooid = $cooid
			WHERE
				prcid = $prcid";
	$db->executar($sql);
	$db->commit();
	
	atualizaHistoricoCoordenacao( $prcid );
	
	geraGuia( $prcid, $emlote );
	
	if( $emlote == '0' ){
		echo "<script>
				alert('Estado alterado com sucesso!');
				window.opener.location = '/profeinep/profeinep.php?modulo=principal/editarprocesso&acao=A&prcid=$prcid';
				window.close();
			  </script>";
	}
	
	return true;
}

function montaAbaInicio($now){
	$menu[0] = array("descricao" => "Listar Processos", "link"=> "profeinep.php?modulo=inicio&acao=C");
	$menu[1] = array("descricao" => "Situação dos Processos", "link"=> "profeinep.php?modulo=principal/statusProcesso&acao=A");
	$menu[2] = array("descricao" => "Gerar Numeração", "link"=> "profeinep.php?modulo=principal/geraNumeracao&acao=A");
	$menu[3] = array("descricao" => "Guias de Tramitação", "link"=> "profeinep.php?modulo=principal/recuperaGuiaDistribuicao&acao=A");
	$menu[4] = array("descricao" => "Tramitação em Lote", "link"=> "profeinep.php?modulo=principal/tramitaProcessos&acao=A");
	echo montarAbasArray($menu, $now);
}

function recuperaCoordenacaoUsuario( $usucpf ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				coo.coodsc||' - '||coo.coosigla as coordenacao
			FROM
				profeinep.usuarioresponsabilidade urp
			INNER JOIN profeinep.coordenacao coo ON coo.coonid = urp.coonid
			WHERE
				urp.usucpf = '".$usucpf."'
			LIMIT 1";
	return $db->pegaUm($sql);
}

function geraGuia( $prcid, $emlote = '0' ){

	if( $emlote == '0' ){
		
		global $db;
		
		$sql = "SELECT DISTINCT
					hst.hstid
				FROM
					profeinep.processoprofeinep prc
				INNER JOIN profeinep.estruturaprocesso    esp ON esp.prcid = prc.prcid
				INNER JOIN workflow.documento 	       doc ON doc.docid = esp.docid
				INNER JOIN workflow.estadodocumento    esd ON esd.esdid = doc.esdid
				INNER JOIN workflow.historicodocumento hst ON hst.docid = doc.docid 
										AND hst.htddata = (SELECT max(h.htddata) 
												FROM workflow.historicodocumento h 
												WHERE h.docid = doc.docid )
				WHERE
					prc.prcstatus = 'A'
					AND prc.prcid = $prcid";
		
		$hstid = $db->pegaUm($sql);
		$html = "<body>";
		$html .= "<form method=\"post\" id=\"formGuia\" action=\"/profeinep/profeinep.php?modulo=principal/guiaDistribuicao&acao=A\">";
		$html .= "<input type=\"hidden\" name=\"guia[$hstid]\" value=\"".$prcid."\" />";		
		$html .= "</form>";
		
		$html .= "</form>";
		$html .= "</body>";
		
		$html .= "<script>
				var form = document.getElementById('formGuia');
				form.target = 'guia';
				var janela 	= window.open( '', 'guia', 'width=1300,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				form.submit();
				janela.focus();
		      </script>";
		echo $html;
	}
	
	return true;
}

?>