<?
include APPRAIZ . 'conjur/classes/WorkflowConjur.php';
function removerexpediente($dados) {
	global $db;
	$sql = "UPDATE conjur.anexos SET anxstatus='I' WHERE expid='".$dados["expid"]."'";
	$db->executar($sql);
	
	$sql = "UPDATE conjur.expediente SET expstatus='I' WHERE expid='".$dados['expid']."'";
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/expediente&acao=A','Remoção efetuada com sucesso');
}

function removerandamento($dados) {
	global $db;
	$sql = "DELETE FROM conjur.andamentoprocesso WHERE anpid='".$dados['anpid']."'";
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/andamento&acao=A','Remoção efetuada com sucesso');
}

function atualizarandamento($dados) {
	global $db;
	$sql = "UPDATE conjur.andamentoprocesso
   	 		SET anpdata='".formata_data_sql($dados['anpdata'])."', 
   	 	 	anpdscsituacao='".$dados['anpdscsituacao']."' 
 	 		WHERE anpid='".$dados['anpid']."'";
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/andamento&acao=A','Gravação efetuada com sucesso');
}

function removerdocumento($dados) {
	
	global $db;
	
	$sql = "select nudid from conjur.anexos WHERE anxstatus='A' and anxid='".$dados["anxid"]."'";
	$nudid = $db->pegaUm($sql);
	
	if($nudid){
		$sql = "UPDATE conjur.numeracaodocumento SET nudstatus='I' WHERE nudid='".$nudid."'";
		$db->executar($sql);		
	}
	
	$sql = "UPDATE conjur.anexos SET anxstatus='I' WHERE anxid='".$dados["anxid"]."'";
	$db->executar($sql);
	
	$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$dados["arqid"];
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/documento&acao=A','Arquivo excluído com sucesso.');
}

function removedocumentosapiens($dados){
    global $db;

    $sql = "select spdid from conjur.sapiensanexo WHERE spastatus='A' and spaid='".$dados["spaid"]."'";
    $nudid = $db->pegaUm($sql);

    if($nudid){
        $sql = "UPDATE conjur.sapiensdocumento SET spdstatus='I' WHERE spdid='".$nudid."'";
        $db->executar($sql);
    }

    $sql = "UPDATE conjur.sapiensanexo SET spastatus='I' WHERE spaid='".$dados["spaid"]."'";
    $db->executar($sql);

    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$dados["arqid"];
    $db->executar($sql);
    $db->commit();
    direcionar('?modulo=principal/documentosSapiensAnexo&acao=A','Arquivo excluído com sucesso.');
}

function desvinculardocumento($dados) {
	
	global $db;
	
	$pflcods = Array(PRF_SUPERUSUARIO,PRF_ADMINISTRADOR,PRF_TECNICO_ADM);
	if( possuiPerfil( $pflcods ) ){
		$sql = "SELECT nudid FROM conjur.anexos WHERE anxstatus='A' and anxid='".$dados["anxid"]."'";
		$nudid = $db->pegaUm($sql);
		
		if( $nudid ){
			$sql = "UPDATE conjur.numeracaodocumento SET nudstatus='A' WHERE nudid='".$nudid."'";
			$db->executar($sql);
		}
			
		$sql = "UPDATE conjur.anexos SET anxstatus = 'I', nudid = NULL WHERE anxid='".$dados["anxid"]."'";
		$db->executar($sql);
		
		$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$dados["arqid"];
		$db->executar($sql);
		$db->commit();
		
		direcionar('?modulo=principal/documento&acao=A','Arquivo desvinculado com sucesso.');
	}
}

function desvinvulardocumentosapiens($dados){
    global $db;

    $pflcods = Array(PRF_SUPERUSUARIO,PRF_ADMINISTRADOR,PRF_TECNICO_ADM);
    if( possuiPerfil( $pflcods ) ){
        $sql = "SELECT spdid FROM conjur.sapiensanexo WHERE spastatus='A' and spaid='".$dados["spaid"]."'";
        $nudid = $db->pegaUm($sql);

        if( $nudid ){
            $sql = "UPDATE conjur.sapiensdocumento SET spdstatus='A' WHERE spdid='".$nudid."'";
            $db->executar($sql);
        }

        $sql = "UPDATE conjur.sapiensanexo SET spastatus = 'I', spdid = NULL WHERE spaid='".$dados["spaid"]."'";
        $db->executar($sql);

        $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$dados["arqid"];
        $db->executar($sql);
        $db->commit();

        direcionar('?modulo=principal/documentosSapiensAnexo&acao=A','Arquivo desvinculado com sucesso.');
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
	$sql = "INSERT INTO conjur.expediente(
            tpeid, prcid, expdscadvogado, expdtinclusaoadvogado, expdscconjur, 
            expdtinclusaoconjur, expstatus, expdtinclusao, usucpf)
    		VALUES ('".$dados['tpeid']."', 
    				'".$_SESSION['conjur_var']['prcid']."', 
    				'".substr($dados['expdscadvogado'],0,500)."', 
    				'".formata_data_sql($dados['expdtinclusaoadvogado'])."', 
    				'".substr($dados['expdscconjur'],0,500)."', 
            		".(($dados['expdtinclusaoconjur'])?"'".formata_data_sql($dados['expdtinclusaoconjur'])."'":"NULL").", 
            		'A', 
            		NOW(),
            		'".$_SESSION['usucpf']."') RETURNING expid;";
	$expid = $db->pegaUm($sql);
	$db->commit();
	
	direcionar('?modulo=principal/expediente_lancamento&acao=A&expid='.$expid,'Expediente cadastrado com sucesso.');
}

function atualizarexpediente($dados) {
	global $db;
	$sql = "UPDATE conjur.expediente
   			SET tpeid='".$dados['tpeid']."', expdscadvogado='".substr($dados['expdscadvogado'],0,500)."', 
   			expdtinclusaoadvogado='".formata_data_sql($dados['expdtinclusaoadvogado'])."', 
       		expdscconjur='".substr($dados['expdscconjur'],0,500)."', 
       		expdtinclusaoconjur=".(($dados['expdtinclusaoconjur'])?"'".formata_data_sql($dados['expdtinclusaoconjur'])."'":"NULL")."  
 			WHERE expid='".$dados['expid']."'";
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/expediente_lancamento&acao=A&expid='.$dados['expid'],'Expediente atualizado com sucesso.');
}

function inserirprocessoconjur($dados) {
	global $db;
	
	$proid = $dados["proid"];
	
	if(!($proid = $db->pegaUm("SELECT proid FROM conjur.procedencia WHERE prodsc = '".$dados['prodsc']."'")))
	{
		echo "<script>alert('Procedência Inválida');</script>";
		return false;
	}
	
	$sql = "select count(*) from conjur.processoconjur where prcnumsidoc = '".$dados["prcnumsidoc"]."'"; 
	$ct = $db->pegaUm($sql);
	
	if ($ct > 0)
	{
		echo "<script>alert('Número SIDOC/EMEC já utilizado');</script>";
		return false;
	}

	$sql= "INSERT INTO conjur.processoconjur(
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
							prcprioritario,
							prctiposidemec )
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
				          (($dados['prcprioritario']=='sim')?"TRUE":"FALSE").",
				          '".substr($dados['tipoNumeracao'],0,1)."') 
				   RETURNING prcid";
		          
	$prcid = $db->pegaUm($sql);
	
	atualiza_coordenacaoSEATA( $prcid, 1 );
	
	for($i=0; $i<count($dados["entid"]); $i++) {
		$db->executar("INSERT INTO conjur.interessadosprocesso(entid,prcid) VALUES(".$dados["entid"][$i].",".$prcid.")");
	}
	
	for($i=0; $i<count($dados["expressaochave"]); $i++) {		
		$db->executar("INSERT INTO conjur.expressaochave(prcid,excdsc,excstatus,excdtinclusao) VALUES(".$prcid.",'".$dados["expressaochave"][$i]."','A',now())");
	}
	
	if(count($dados['pro_prcid']) > 0) {
		foreach(array_keys($dados['pro_prcid']) as $pro_prcid) {
			$db->executar("INSERT INTO conjur.processosvinculados(
            	  		   prcid, pro_prcid, usucpf, prvdtvinculacao)
		    	  		   VALUES ('".$prcid."', '".$pro_prcid."', '".$_SESSION['usucpf']."', NOW());");
		}
	}
	
	
	include_once APPRAIZ . "includes/workflow.php";
	
	$docid = pegaDocidProcesso( $prcid );
	
	if(!$docid){		
		$docid = criarDocumento( $prcid );
	}
	
	if($docid){		
		$sql = "UPDATE workflow.documento SET esdid = 374 where docid = {$docid};";
		$db->executar($sql);
	}
	
	$db->commit();
	direcionar('?modulo=principal/editarprocesso&acao=A&prcid='.$prcid,'Operação realizada com sucesso!');
}

function atualizarprocessoconjur($dados) {
	
	global $db;
	
	if( $dados['tasdsc'] && !is_null($dados['tasdsc']) && $dados['tasdsc'] != '' )
	{
		if(!($dados['tasid'] = $db->pegaUm("SELECT tasid FROM conjur.tipoassunto WHERE tasdsc = '".$dados['tasdsc']."'")))
		{
			echo "<script>alert('Tema Inválido');</script>";
			return false;
		}
	}
	
	if(!($dados['proid'] = $db->pegaUm("SELECT proid FROM conjur.procedencia WHERE prodsc = '".$dados['prodsc']."'")))
	{
		echo "<script>alert('Procedência Inválida');</script>";
		return false;
	}
	
	$dados['tacid'] = ($dados['tacid']) ? $dados['tacid'] : 'null';
	
	$sql = "UPDATE conjur.processoconjur 
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
       	 	WHERE prcid='".$_SESSION['conjur_var']['prcid']."';";
       	 		
	$db->executar($sql);
	
	atualizaHistoricoAdovogado( $_SESSION['conjur_var']['prcid'], $dados['advid'] );
	
	$sql = "DELETE FROM conjur.expressaochave WHERE prcid='".$_SESSION['conjur_var']['prcid']."'";
	$db->executar($sql);
	$sql = "DELETE FROM conjur.interessadosprocesso WHERE prcid='".$_SESSION['conjur_var']['prcid']."'";
	$db->executar($sql);
	
	//Início -  Pegar os docis que terão seu estado retornado ao anterior
	$sql = "SELECT 
				prcid 
			FROM 
				conjur.estruturaprocesso
			WHERE
				prcid in (
							SELECT 
								pro_prcid 
							FROM 
								conjur.processosvinculados 
							WHERE 
								".(count($dados['pro_prcid']) > 0 ? "pro_prcid NOT IN (".implode(",",array_keys($dados['pro_prcid'])).") AND" : " ")." prcid='".$_SESSION['conjur_var']['prcid']."'
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
				conjur.estruturaprocesso
			WHERE
				prcid in (
							SELECT 
								pro_prcid 
							FROM 
								conjur.processosvinculados 
							WHERE 
								".(count($dados['pro_prcid']) > 0 ? "pro_prcid NOT IN (".implode(",",array_keys($dados['pro_prcid'])).") AND" : " ")." prcid='".$_SESSION['conjur_var']['prcid']."'
						)";
	
	$arrDocis = $db->carregar($sql);
	$sql = "SELECT 
				 d.esdid
			FROM 
				workflow.documento d
			INNER JOIN conjur.estruturaprocesso e ON e.docid = d.docid
			WHERE
				prcid = ".$_SESSION['conjur_var']['prcid'];
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
	
	$sql = "DELETE FROM conjur.processosvinculados WHERE ".(count($dados['pro_prcid']) > 0 ? "pro_prcid NOT IN (".implode(",",array_keys($dados['pro_prcid'])).") AND" : " ")." prcid='".$_SESSION['conjur_var']['prcid']."'";
	$db->executar($sql);
	
	if($_POST['prazo']){
		$sql = "UPDATE conjur.estruturaprocesso
				SET espnumdiasrespexterna = {$_POST['prazo']}
				WHERE prcid = ".$_SESSION['conjur_var']['prcid'];
		echo "chegou aqui";
	}
	
	for($i=0; $i<count($dados['entid']); $i++) {
		$db->executar("INSERT INTO conjur.interessadosprocesso(entid,prcid) VALUES(".$dados['entid'][$i].",".$_SESSION['conjur_var']['prcid'].")");
	}
	for($i=0; $i<count($dados['expressaochave']); $i++) {		
		$db->executar("INSERT INTO conjur.expressaochave(prcid,excdsc,excstatus,excdtinclusao) VALUES(".$_SESSION['conjur_var']['prcid'].",'".$dados['expressaochave'][$i]."','A',now())");
	}
	if(count($dados['pro_prcid']) > 0) {
		
		if( $esdidPai == WF_EM_ANALISE_ADVOGADO || $esdidPai == WF_EM_ANALISE_SUBCOORDENACAO ){
			$advPai = recuperaAdvogado( $_SESSION['conjur_var']['prcid'] );
		}
		
		foreach(array_keys($dados['pro_prcid']) as $pro_prcid) {
			
			$sql = "select prvid FROM conjur.processosvinculados where pro_prcid = $pro_prcid AND prcid = {$_SESSION['conjur_var']['prcid']}";
			$prvid = $db->pegaUm($sql);

			if(!$prvid){
				$db->executar("INSERT INTO conjur.processosvinculados(
            	  		   prcid, pro_prcid, usucpf, prvdtvinculacao)
		    	  		   VALUES ('".$_SESSION['conjur_var']['prcid']."', '".$pro_prcid."', '".$_SESSION['usucpf']."', NOW());");
				
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
		$sql = "UPDATE conjur.processoconjur SET 
					cooid = (SELECT cooid FROM conjur.processoconjur WHERE prcid = ".$_SESSION['conjur_var']['prcid']." ) 
				WHERE 
					prcid in (".implode(",",array_keys($dados['pro_prcid'])).")";
		$db->executar($sql);
		
	}
	
	$db->commit();
	direcionar('?modulo=principal/editarprocesso&acao=A&prcid='.$_SESSION['conjur_var']['prcid'],'Operação realizada com sucesso!');
}

function atualizaHistoricoAdovogado( $prcid, $advid ){
	
	global $db;
	
	$adv = recuperaAdvogado( $prcid );
	
	if( $advid != '' && $advid != $adv ){
		$sql = "INSERT INTO conjur.historicoadvogados(prcid,advid) VALUES (".$prcid.",".$advid.")";
		$db->executar($sql);
	}
	
}

function recuperaAdvogado( $prcid ){
	
	global $db;
	
	return $db->pegaUm('SELECT 
					advid 
				FROM 
					conjur.historicoadvogados
				WHERE 
					hadid = (SELECT max(hadid) FROM conjur.historicoadvogados WHERE prcid = '.$prcid.') ');
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
	$abas_editarprocesso[] = array("id" => 1, "descricao" => "Lista de Processos", "link" => "/conjur/conjur.php?modulo=inicio&acao=C");
	$abas_editarprocesso[] = array("id" => 2, "descricao" => "Dados do Processo", "link" => "/conjur/conjur.php?modulo=principal/editarprocesso&acao=A&prcid=".$_SESSION['conjur_var']['prcid']);
	$abas_editarprocesso[] = array("id" => 3, "descricao" => "Histórico", "link" => "/conjur/conjur.php?modulo=principal/movimentacao&acao=A");
	$abas_editarprocesso[] = array("id" => 4, "descricao" => "Documentos", "link" => "/conjur/conjur.php?modulo=principal/documento&acao=A");
	$abas_editarprocesso[] = array("id" => 5, "descricao" => "Gerar Numeração", "link" => "/conjur/conjur.php?modulo=principal/geraNumeracao&acao=A");
    $abas_editarprocesso[] = array("id" => 6, "descricao" => "Documentos Sapiens", "link" => "/conjur/conjur.php?modulo=principal/documentosSapiens&acao=A");
    $abas_editarprocesso[] = array("id" => 7, "descricao" => "Documentos Sapiens Anexo", "link" => "/conjur/conjur.php?modulo=principal/documentosSapiensAnexo&acao=A");
	$abas_editarprocesso[] = array("id" => 8, "descricao" => "Tramitação em Lote", "link" => "/conjur/conjur.php?modulo=principal/tramitaProcessos&acao=A");
	return $abas_editarprocesso;
}

function monta_cabecalho_conjur($prcid) {
	global $db;
	
	$processoconjur = $db->pegaLinha("SELECT prcnumsidoc, prcnomeinteressado, esddsc FROM conjur.processoconjur prc 
									  LEFT JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
    								  LEFT JOIN workflow.documento doc ON doc.docid = esp.docid 
    								  LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid  
									  WHERE prc.prcid='".$prcid."'");
	
	// efetuar select e retornar cabecalho
	$titulo_modulo = "CONJUR";
	monta_titulo( $titulo_modulo,'Consultória Jurídica');
	
	echo "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
	echo "<tr>";
	echo "<td class='SubTituloDireita' width='25%'>Nº do Processo :</td><td>".$processoconjur['prcnumsidoc']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class='SubTituloDireita' width='25%'>Interessado :</td><td>".$processoconjur['prcnomeinteressado']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class='SubTituloDireita' width='25%'>Localização em andamento :</td><td>".$processoconjur['esddsc']."</td>";
	echo "</tr>";
	echo "</table>";
}

function inserirandamento($dados) {
	global $db;
	$sql = "INSERT INTO conjur.andamentoprocesso(
            prcid, usucpf, anpdata, anpdscsituacao, anpstatus, anpdtinclusao)
     		VALUES ('".$_SESSION['conjur_var']['prcid']."', 
     				'".$_SESSION['usucpf']."', 
     				'".formata_data_sql($dados['anpdata'])."', 
     				'".$dados['anpdscsituacao']."', 
     				'A', 
     				NOW());";
	$db->executar($sql);
	$db->commit();
	direcionar('?modulo=principal/andamento&acao=A','Gravação efetuada com sucesso');
}

function inserirarquivoconjur($dados) {
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
	
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = " INSERT INTO public.arquivo 
				(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
			VALUES
				('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".substr($dados["anxdesc"],0,255)."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .")
			RETURNING arqid; ";
	$arqid = $db->pegaUm($sql);
	
	if(!$dados['nudid']){
		$dados['nudid'] = 'null';
	}
	
	//Insere o registro na tabela obras.arquivosobra
	$sql = "INSERT INTO conjur.anexos(
	            expid, arqid, prcid, anxdesc, anxtipo, anxstatus, anxdtinclusao, tpdid, nudid)
    		VALUES (
    		".(($dados['expid'])?"'".$dados['expid']."'":"NULL").", 
    		".$arqid.", 
    		".$_SESSION['conjur_var']['prcid'].", 
    		'".substr($dados['anxdesc'],0,255)."', 
    		'".$dados['anxtipo']."', 
    		'A',
    		NOW(),
    		".$dados['tpdid'].",
    		".$dados['nudid'].");";
	$db->executar($sql);

	// se não existir a pasta, cria no servidor
	if(!is_dir('../../arquivos/conjur/')) {
		mkdir(APPRAIZ.'/arquivos/conjur/', 0777);
	}
	
	// se não existir o arquivo, cria no servidor
	if(!is_dir('../../arquivos/conjur/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/conjur/'.floor($arqid/1000), 0777);
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

function conjur_download_arquivo( $param ){
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
			FROM conjur.estruturaprocesso
			WHERE prcid = '" . $prcid . "'";
	return (integer) $db->pegaUm( $sql );
}

function criarDocumento( $prcid )
{
	global $db;
	if(!pegarDocid($prcid)) {
//		$sqlTpdid = "SELECT t.tpdid 
//					FROM seguranca.sistema s					
//					INNER JOIN workflow.tipodocumento t on s.sisid = t.sisid					
//					WHERE s.sisabrev = 'CONJUR'";
		$tpdid = NOVO_WORKFLOW;
		$sqlDescricao = "SELECT	prcnumsidoc
						 FROM conjur.processoconjur
						 WHERE prcid = '" . $prcid . "'";
		$descricao = $db->pegaUm( $sqlDescricao );
		$docdsc = "Número do processo na CONJUR :" . $descricao;
		// cria documento
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );	
		$sql = "INSERT 
				INTO conjur.estruturaprocesso 
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
			INNER JOIN conjur.tprperfil tpr ON tpr.pflcod = p.pflcod
			WHERE 
				pu.usucpf = '". $_SESSION['usucpf'] ."' 
				AND p.pflstatus = 'A' 
				AND p.sisid =  '". $_SESSION['sisid'] ."'";
	$teste = $db->carregarColuna($sql);
	return is_array($teste)&&count($teste)>0;
}

function verificaPerfilConjur($estid = false, $cooid = false) {
	global $db;
	$sql = "SELECT p.pflcod FROM seguranca.perfil p 
			LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
			WHERE pu.usucpf = '". $_SESSION['usucpf'] ."' and p.pflstatus = 'A' and p.sisid =  '". $_SESSION['sisid'] ."'";
	$perfilid = $db->pegaUm($sql);
	$_SESSION['conjur']['perfilid'] = $perfilid;
	if($db->testa_superuser() || $perfilid == PRF_ADMCONJUR) {
		// permissao para remover e gravar
		$permissoes['remover'] = true;
		$permissoes['gravar'] = true;
		$permissoes['selecionaradvogado'] = true;
	} else {
		// Analisando permissão de acesso de acordo com o estado do documento
		switch($perfilid) {
			
			case PRF_SUPERUSUARIO:
				$permissoes['remover'] = true;
				$permissoes['gravar'] = true;
				$permissoes['selecionaradvogado'] = true;
				break;
			
			case PRF_ADMINISTRADOR:
				$permissoes['remover'] = true;
				$permissoes['gravar'] = true;
				$permissoes['selecionaradvogado'] = true;
				break;

            case PRF_APOIO_DGAA:
                $permissoes['remover'] = false;
                $permissoes['gravar'] = true;
                $permissoes['selecionaradvogado'] = true;
                break;

			case PRF_TECNICO_ADM:
				$permissoes['remover'] = true;
				$permissoes['gravar'] = true;
				$permissoes['selecionaradvogado'] = true;
				break;
			
			// analisando estado do documento 
			case PRF_TECNICO:
				//verificanco a coordenação cadastrada
				//if(is_numeric(trim($cooid))){
					$sql = "SELECT coonid FROM conjur.usuarioresponsabilidade
						WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}'";
					$coordenacao_id = $db->pegaUm($sql);
					switch($coordenacao_id){
						//se for igual a coordenação do Processo
						case $cooid:
							$permissoes['remover'] = true;
							$permissoes['gravar'] = true;
							$permissoes['selecionaradvogado'] = true;
							break;
						//se for diferente
						default:
							$permissoes['remover'] = false;
							$permissoes['gravar'] = false;
							$permissoes['selecionaradvogado'] = false;
							break;
					}
					break;
			//	}
				// fim da analise do documento
				
			case PRF_ADVOGADO:
				$permissoes['remover'] = false;
				$permissoes['gravar'] = false;
				$permissoes['selecionaradvogado'] = false;
				break;

			case PRF_APOIO_PROTOCOLO:
				$permissoes['remover'] = true;
				$permissoes['gravar'] = true;
				$permissoes['selecionaradvogado'] = true;
				break;
				
			case PRF_CONSULTAGERAL:
				$permissoes['remover'] = false;
				$permissoes['gravar'] = false;
				$permissoes['selecionaradvogado'] = false;
				break;
			
			case PRF_EQTECMEC:
				// analisando estado do documento 
				switch($estid) {
					default:
						$permissoes['remover'] = true;
						$permissoes['gravar'] = true;
				}
				// fim da analise do documento
				break;
			case PRF_GESTORHU:
				// analisando estado do documento 
				switch($estid) {
					case DOC_APROVACAOHU:
					case DOC_CADHU:
						
						$permissoes['remover'] = true;
						$permissoes['gravar'] = true;
						break;
					default:
						$permissoes['remover'] = false;
						$permissoes['gravar'] = false;
				}
				// fim da analise do documento
				break;
			case PRF_EQAPOIOHU:
				// analisando estado do documento
				switch($estid) {
					case DOC_CADHU:
						$permissoes['remover'] = true;
						$permissoes['gravar'] = true;
						break;
					default:
						$permissoes['remover'] = false;
						$permissoes['gravar'] = false;
				}
				// fim da analise do documento
				break;
			case PRF_CONSULTAMEC:
			case PRF_CONSULTAHU:
				$permissoes['remover'] = false;
				$permissoes['gravar'] = false;
				break;
			case PRF_APOIO_GABINETE:
				$permissoes['gravar'] = true;
				break;
			default:
				$permissoes['remover'] = false;
				$permissoes['gravar'] = false;
		}
	}
	return $permissoes;
}

function verificaPerfilCoordenacaoConjur( $cooid = false ) {
	global $db;
	
	if(!$cooid) return true;
		
	$sql = "SELECT coonid FROM conjur.usuarioresponsabilidade
			WHERE rpustatus = 'A' AND coonid = $cooid AND usucpf = '{$_SESSION['usucpf']}'";
	
	$cooid = $db->pegaUm($sql);
	
	return $cooid != '' ? true : false;
}

/* Funções das ações do WORKFLOW */

function conjur_verificarcoordenacao($prcid) {
	global $db;

	if($db->pegaUm("SELECT cooid FROM conjur.processoconjur WHERE prcid='".$prcid."'")){
		$verificaAnexo = conjur_verificarprocessoanexo($prcid);
		if($verificaAnexo){
			return true;
		} else {
			return "É necessário anexar um arquivo.";
		}
	}
	return false;
}

function conjur_verificaCGAC($prcid, $cooid){
	if(1==$cooid){
		return conjur_verificarprocessoanexo($prcid);
	}else{
		return false;
	}
}

function conjur_verificaCGNLJ($prcid, $cooid){
	
	if(2==$cooid){
		return conjur_verificarprocessoanexo($prcid);
	}else{
		return false;
	}
}

function conjur_verificaCGEPD($prcid, $cooid){
	
	if(3==$cooid){
		return conjur_verificarprocessoanexo($prcid);
	}else{
		return false;
	}
}

function conjur_verificarprocessoanexo($prcid) {
	global $db;
	
	$docid = pegaDocidProcesso($prcid);
	$esdid = pegaEstadoWorkFlow($docid);
	
	if(possuiPerfil(array(PRF_ADMINISTRADOR,PRF_TECNICO_ADM,PRF_SUPERUSUARIO)) || $esdid = WF_EM_ANALISE_GABINETE){
		return true;	
	}
	
	$sql = "select
				coalesce(a.nudid,0) as nudidanexo, 
				nd.nudnumero
			from conjur.processoconjur p
			inner join conjur.numeracaodocumento nd on p.prcid = nd.prcid
			left join conjur.anexos a on a.nudid = nd.nudid  and a.anxstatus = 'A'
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
	
	if( $boTramitar && conjur_verificarprocessodocumento($prcid) ){
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
			from conjur.processoconjur p
			inner join conjur.numeracaodocumento nd on p.prcid = nd.prcid
			left join conjur.anexos a on a.nudid = nd.nudid  and a.anxstatus = 'A'
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

function conjur_regraA( $prcid ){
	return verificarProcessoAnexoNovo( $prcid );
}

function conjur_regraB( $cooid = null ){
	global $db;
	$sudo = $db->testa_superuser() || possuiPerfil(array(PRF_ADMINISTRADOR,PRF_TECNICO_ADM));
	return verificaPerfilCoordenacaoConjur( $cooid )||$sudo;
}

function conjur_regraC( $docid ){
	//$ant = pegaEstadoAnteriorWorkFlow( $docid );
	//return $ant == WF_EM_ANALISE_COORDENADOR;
	return in_array(WF_EM_ANALISE_COORDENADOR_GERAL,pegaHistoricoEstadoWorkFlow($docid));
}

function conjur_regraD( $prcid ){
	return verificarAdvogadoNovo( $prcid );
}

function conjur_regraE( $prcid, $cooid = null ){
	
	$coords = Array(COORD_PROTOCOLO_APOIO_SEATA,COORD_GABINETE_CONJUR);
	
	if($cooid == COORD_CGAC) {
		return true;
	} elseif(in_array($cooid,$coords)) {
		//return (pegaLocalFisicoAnterior( $prcid ) == COORD_CGAC || pegaLocalFisicoAnterior( $prcid ) == '');
		return (in_array(COORD_CGAC,pegaHistoricoLocalFisico($prcid)) || pegaLocalFisicoAnterior( $prcid ) == '');
	} else {
		return false;
	}

}

function conjur_regraF( $prcid, $cooid = null ){
	
	$coords = Array(COORD_PROTOCOLO_APOIO_SEATA,COORD_GABINETE_CONJUR);
		
	if($cooid == COORD_CGEPD) {
		return true;
	} elseif(in_array($cooid,$coords)) {
		$ant = pegaLocalFisicoAnterior( $prcid );
		//return ($ant == COORD_CGEPD || $ant == '');
		return (in_array(COORD_CGEPD,pegaHistoricoLocalFisico($prcid)) || $ant == '');
	} else {
		return false;
	}

}

function conjur_regraG( $prcid, $cooid = null ){
	
	$coords = Array(COORD_PROTOCOLO_APOIO_SEATA,COORD_GABINETE_CONJUR);
	
	if($cooid == COORD_CGNLJ) {
		return true;
	} elseif(in_array($cooid,$coords)) {
		//return (pegaLocalFisicoAnterior( $prcid ) == COORD_CGNLJ || pegaLocalFisicoAnterior( $prcid ) == '');
		return (in_array(COORD_CGNLJ,pegaHistoricoLocalFisico($prcid)) || pegaLocalFisicoAnterior( $prcid ) == '');
	} else {
		return false;
	}

}

function conjur_regraAB( $prcid, $cooid = null ){
	return conjur_regraA( $prcid ) && conjur_regraB( $cooid );
}

function conjur_regraBD( $prcid, $cooid = null ){
	return conjur_regraB( $cooid ) && conjur_regraD( $prcid );
}

function conjur_regraCE( $docid, $prcid, $cooid = null ){
	return conjur_regraC( $docid ) && conjur_regraE( $prcid, $cooid );
}

function conjur_regraCF( $docid, $prcid, $cooid = null ){
	return conjur_regraC( $docid ) && conjur_regraF( $prcid, $cooid );
}

function conjur_regraCG( $docid, $prcid, $cooid ){
	return conjur_regraC( $docid ) && conjur_regraG( $prcid, $cooid );
}

function conjur_verificarprocessodocumento($prcid) {
	global $db;
	
	$docid = pegaDocidProcesso($prcid);
	$esdid = pegaEstadoWorkFlow($docid);
	
	if(possuiPerfil(array(PRF_ADMINISTRADOR,PRF_TECNICO_ADM,PRF_SUPERUSUARIO)) || $esdid = WF_EM_ANALISE_GABINETE){
		return true;	
	}
	
	$sql = "SELECT
				count(a.nudid) as nudidanexo
			FROM conjur.processoconjur p
			INNER JOIN conjur.numeracaodocumento nd ON p.prcid = nd.prcid
			INNER JOIN conjur.anexos 	          a ON a.nudid = nd.nudid  AND a.anxstatus = 'A'
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

function conjur_verificaradvogado($prcid) {
	global $db;
	
	if($db->pegaUm("SELECT max(advid) FROM conjur.historicoadvogados WHERE prcid='".$prcid."'")){
		$verificaAnexo = conjur_verificarprocessoanexo($prcid);
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
	
	return $db->pegaUm("SELECT max(advid) FROM conjur.historicoadvogados WHERE prcid='".$prcid."'");
}

function carregarAdvogados($dados) {
	global $db;
	if($dados['cooid']){
		$sqlAdvogado = "SELECT adv.advid as codigo, ent.entnome as descricao 
						FROM conjur.advogados adv 
						INNER JOIN entidade.entidade ent ON ent.entid = adv.entid 
						INNER JOIN conjur.advogadosxcoordenacao adc ON adc.advid = adv.advid
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
					esd.tpdid = ".NOVO_WORKFLOW."
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
					esd.tpdid = ".NOVO_WORKFLOW."
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
				conjur.processosvinculados prv
			inner join
				conjur.processoconjur prc ON prc.prcid = prv.prcid
			inner join
				conjur.estruturaprocesso epr ON epr.prcid = pro_prcid
			inner join
				workflow.documento doc ON doc.docid = epr.docid
			inner join
				workflow.estadodocumento est ON est.esdid = doc.esdid
			where 
				esddsc != 'Anexado'
			AND 
				est.tpdid = ".NOVO_WORKFLOW."
			";
	
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
			AND 
				esd.tpdid = ".NOVO_WORKFLOW."
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
				conjur.historicocoordenacoes
			WHERE
				hcoid = ( SELECT max(hcoid) FROM conjur.historicocoordenacoes WHERE prcid = $prcid )";
	return $db->pegaUm($sql);
}

function pegaHistoricoLocalFisico( $prcid ){
	
	global $db;
	
	$sql = "SELECT 
				coonid
			FROM 
				conjur.historicocoordenacoes
			WHERE
				prcid = $prcid ";
	return $db->carregarColuna($sql);
}


function pegaCoordUsu(){
	
	global $db;
	
	$sql = "SELECT 
				coonid
			FROM
				conjur.usuarioresponsabilidade
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
				conjur.processoconjur
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
			AND 
				esdid != $estadoAtual
			AND 
				ed.tpdid = ".NOVO_WORKFLOW."
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
				AND 
					esd.tpdid = ".NOVO_WORKFLOW."
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
			AND 
				esdid != $estadoAtual
			AND 
				ed.tpdid = ".NOVO_WORKFLOW."
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
	
	$sql = "select docid from conjur.estruturaprocesso where prcid = $prcid";
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
	
	$docid = pegaDocidProcesso($_SESSION['conjur_var']['prcid']);
	$advid = $_REQUEST['advid'] ? $_REQUEST['advid'] : 'null';
    $haddtprazoadv = $_REQUEST['haddtprazoadv'] ? "'".formata_data_sql($_REQUEST['haddtprazoadv'])."'" : 'null';

	$sql = "INSERT INTO conjur.historicoadvogados(prcid,advid,haddtprazoadv) VALUES (".$_SESSION['conjur_var']['prcid'].",".$advid.",".$haddtprazoadv.") RETURNING hadid";
	$hadid = $db->pegaUm($sql);
	$db->commit();

	include APPRAIZ . 'includes/workflow.php';
	
	$cooid = $db->pegaUm("SELECT cooid FROM conjur.processoconjur WHERE prcid='".$_SESSION['conjur_var']['prcid']."'");
	
	wf_alterarEstado( $docid, ACAO_ENCAMINHAR_PARA_ADVOGADO, $cmddsc = 'Encaminhado para advogado', Array( "prcid" => $_SESSION['conjur_var']['prcid'], "cooid" => $cooid, "advid" => $advid) );
	echo "<script>alert('função')</script>";
	$sql = "UPDATE conjur.historicoadvogados SET hstid = (SELECT max(hstid) FROM workflow.historicodocumento WHERE docid = $docid ) WHERE hadid = $hadid";
	$db->executar($sql);
	$db->commit();
	
	return true;
}

function alteraAdvogado( $prcid, $advid, $emlote = 0 ){
	
	global $db;
	echo "<script>alert('pos ação')</script>";
//	$sql = "INSERT INTO conjur.historicoadvogados(prcid,advid) VALUES (".$_SESSION['conjur_var']['prcid'].",".$advid.")";
//	$db->executar($sql);
//	$db->commit();
	return true;
}

function carregarWorkflow()
{
	global $db;
	
	$docid = $_REQUEST['docid'];
	$advid = $_REQUEST['advid'] ? $_REQUEST['advid'] : 'null';
	
	$db->executar("UPDATE conjur.processoconjur SET advid = $advid WHERE prcid='".$_SESSION['conjur_var']['prcid']."'");
	$db->commit();
	
	include APPRAIZ . 'includes/workflow.php';
	
	wf_desenhaBarraNavegacao( $docid, array( 'prcid' => $_SESSION['conjur_var']['prcid'] ) );
}

function carregarWorkflow2()
{
	global $db;
	
	$docid = $_REQUEST['docid'];
	$cooid = $_REQUEST['cooid'] ? $_REQUEST['cooid'] : 'null';
	
	$db->executar("UPDATE conjur.processoconjur SET cooid = $cooid WHERE prcid='".$_SESSION['conjur_var']['prcid']."'");
	$db->commit();
	
	include APPRAIZ . 'includes/workflow.php';
	
	wf_desenhaBarraNavegacao( $docid, array( 'prcid' => $_SESSION['conjur_var']['prcid'], 'cooid' => $cooid, 'usar_acaoPossivel2' => true  ) );
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
					from conjur.processoconjur p
				inner join conjur.numeracaodocumento nd on p.prcid = nd.prcid
				left join conjur.anexos a on a.nudid = nd.nudid  and a.anxstatus = 'A'
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
	
	$sql = "UPDATE conjur.estruturaprocesso SET
				espdtrespexterna = null,
				espnumdiasrespexterna = null
			WHERE
				prcid = $prcid";
	
	$db->executar($sql);
	
	atualiza_coordenacaoSEATA( $prcid, $emlote );
	
	return true;
}

function testa_CGAE( $prcid ){
	
	global $db;
	
	$sql = "SELECT
				1
			FROM
				conjur.processoconjur
			WHERE
				prcid = $prcid
				AND prcstatus = 'A'
				AND cooid = ".COORD_CGEPD;
	
	return $db->pegaUm($sql) > 0 && conjur_regraB( COORD_CGEPD );
}

function atualiza_coordenacaoChefeSubCoordCGAE( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COORD_CGAE_CHEFE_SUB_COORD,$emlote);
	
	return true;
}

function atualiza_coordenacaoChefeDivisaoCGAE( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COORD_CGAE_CHEFE_DIVISAO,$emlote);
	
	return true;
}

function atualiza_coordenacaoCGAC( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COORD_CGAC,$emlote);
	
	return true;
}

function atualiza_coordenacaoCGLNJ( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COORD_CGNLJ,$emlote);
	
	return true;
}

function atualiza_coordenacaoCGEPD( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COORD_CGEPD,$emlote);
	
	return true;
}

function atualiza_coordenacaoSEATA( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COORD_PROTOCOLO_APOIO_SEATA,$emlote);
	
	return true;
}

function atualiza_coordenacaoEXT( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COORD_EXT,$emlote);
	
	echo "<script>
			window.openner.location = 'conjur.php?modulo=principal/prazoRespostaExterna&acao=A&prcid=".$_SESSION['conjur_var']['prcid']."';
		  </script>";
	
	return true;
}

function atualiza_coordenacaoGABINETE( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COORD_GABINETE_CONJUR,$emlote);
	
	return true;
}

function atualiza_coordenacaoGABAUX( $prcid, $emlote = '0' ){
	
	atualizaCoordenacao($prcid,COORD_GABAUX,$emlote);
	
	return true;
}

function retornaEstadoAnterior( $prcid ) {
	global $db;
	
	$sql = "SELECT ep.docid FROM conjur.processoconjur prc 
			INNER JOIN conjur.estruturaprocesso ep ON ep.prcid = prc.prcid 
			WHERE prc.prcid='".$prcid."'";
	
	$docid = $db->pegaUm($sql);
	
	$sql = "SELECT a.esdidorigem FROM workflow.historicodocumento h 
			INNER JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid 
			WHERE h.docid='".$docid."' ORDER BY htddata DESC LIMIT 2";
	
	$esdDes = $db->carregarColuna($sql);
	
	if($esdDes[1]) {
		
		$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".WF_DESARQUIVADO."' AND esdiddestino='".$esdDes[1]."'";
		$aedidAtual = $db->carregarColuna($sql);
		
		if(count($aedidAtual)>1) {
			
			$sql = "SELECT co.aedid, es.docid, prc.cooid FROM conjur.processoconjur prc 
			INNER JOIN conjur.estruturaprocesso es ON prc.prcid = es.prcid 
			INNER JOIN conjur.coordenacao co ON co.coonid = prc.cooid 
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
    
    $sql = "SELECT cooid FROM conjur.processoconjur WHERE prcid = $prcid ";
    
    if( $db->pegaUm($sql) != '' ){
        $sql = "
            INSERT INTO conjur.historicocoordenacoes(prcid, coonid, hstid) 
                SELECT  prc.prcid, 
                        cooid,
                        hstid 
                FROM conjur.processoconjur prc
                INNER JOIN conjur.estruturaprocesso esp ON esp.prcid = prc.prcid
                
                LEFT JOIN (
                    SELECT  max(h.hstid) as hstid,
                            h.docid
                    FROM workflow.historicodocumento h
                    JOIN workflow.documento d on d.docid = h.docid and d.tpdid = 49
                    GROUP BY h.docid
                ) hdc ON hdc.docid = esp.docid
                
                WHERE prc.prcid = $prcid
        ";
        $db->executar($sql);
        $db->commit();
    }
}

function atualizaCoordenacao($prcid,$cooid,$emlote = '0'){

	global $db;
	
	$sql = "UPDATE conjur.processoconjur SET
				cooid = $cooid
			WHERE
				prcid = $prcid";
	$db->executar($sql);
	$db->commit();
	
	atualizaHistoricoCoordenacao( $prcid );
	
	//geraGuia( $prcid, $emlote );
	
	if( $emlote == '0' ){
		echo "<script>
				alert('Estado alterado com sucesso!');
				window.opener.location = '/conjur/conjur.php?modulo=principal/editarprocesso&acao=A&prcid=$prcid';
				window.close();
			  </script>";
	}
	
	return true;
}

function montaAbaInicio($now){
	$menu[0] = array("descricao" => "Listar Processos", "link"=> "conjur.php?modulo=inicio&acao=C");
	$menu[1] = array("descricao" => "Situação dos Processos", "link"=> "conjur.php?modulo=principal/statusProcesso&acao=A");
	$menu[2] = array("descricao" => "Gerar Numeração", "link"=> "conjur.php?modulo=principal/geraNumeracao&acao=A");
    $menu[3] = array("descricao" => "Documentos Sapiens", "link"=> "conjur.php?modulo=principal/documentosSapiens&acao=A");
	$menu[4] = array("descricao" => "Guias de Tramitação", "link"=> "conjur.php?modulo=principal/recuperaGuiaDistribuicao&acao=A");
	$menu[5] = array("descricao" => "Tramitação em Lote", "link"=> "conjur.php?modulo=principal/tramitaProcessos&acao=A");
	$menu[6] = array("descricao" => "Biblioteca", "link"=> "conjur.php?modulo=principal/listaBiblioteca&acao=A");
	echo montarAbasArray($menu, $now);
}

function recuperaCoordenacaoUsuario( $usucpf ){
	
	global $db;
	
	$sql = "
            SELECT  DISTINCT
                    coo.coodsc||' - '||coo.coosigla as coordenacao
            FROM conjur.usuarioresponsabilidade urp
            INNER JOIN conjur.coordenacao coo ON coo.coonid = urp.coonid
            WHERE urp.usucpf = '{$usucpf}' AND rpustatus = 'A'
        ";
	return $db->pegaUm($sql);
}

function geraGuia( $prcid, $emlote = '0' ){
    
    #FUNÇÃO COMENTADA A PEDIDO DO ANALISTA SIQUEIRA. 
    #OBJETIVO:
    #PARA QUE NÃO SEJA GERADO A GUIA DE TRAMITAÇÃO. 
    #OBS: PEDIDO FEITO PELA USUÁRIA "AMANDA/KATIA".
    #DATA: 17/09/2013.
/*
	if( $emlote == '0' ){
		
		global $db;
		
		$sql = "SELECT DISTINCT
					hst.hstid
				FROM
					conjur.processoconjur prc
				INNER JOIN conjur.estruturaprocesso    esp ON esp.prcid = prc.prcid
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
		$html .= "<form method=\"post\" id=\"formGuia\" action=\"/conjur/conjur.php?modulo=principal/guiaDistribuicao&acao=A\">";
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
	*/
	return true;
}

function verificaAcaoRetornarAdvogado($docid, $cooid = null)
{
	global $db;
	
	if(!conjur_regraB($cooid)){
		return false;
	}
	
	/*
	$sql = "select
				count(*)
			from workflow.historicodocumento hd
				inner join workflow.acaoestadodoc ac on
					ac.aedid = hd.aedid
				inner join workflow.estadodocumento ed on
					ed.esdid = ac.esdidorigem
				inner join seguranca.usuario us on
					us.usucpf = hd.usucpf
				left join workflow.comentariodocumento cd on
					cd.hstid = hd.hstid
			where
				hd.docid = {$docid}
			and 
				ed.esdid = ".EST_AGUARDATRIBUICAO."
			and
				ac.aedid = 157";	
	*/
	
	$sql = "select
				count(*)
			from workflow.historicodocumento hd
				inner join workflow.acaoestadodoc ac on
					ac.aedid = hd.aedid
				inner join workflow.estadodocumento ed on
					ed.esdid = ac.esdidorigem
				inner join seguranca.usuario us on
					us.usucpf = hd.usucpf
				left join workflow.comentariodocumento cd on
					cd.hstid = hd.hstid
			where
				hd.docid = {$docid}
			and 
				ed.esdid = ".WF_EM_ANALISE_COORDENADOR_GERAL."
			and
				ac.aedid = ".ACAO_ENCAMINHAR_PARA_ADVOGADO;
	
	$rs = $db->pegaUm($sql);
	
	if($rs){
		return true;
	}else{
		return false;
	}
}

function verificaNumeroProcessoGerado( $prcid )
{
	global $db; 
		
	if(!conjur_regraA( $prcid )){
		return false;
	}
	
	$sql = "SELECT	
				count(*) as total
			FROM 
				conjur.anexos anx 
			LEFT JOIN 
				conjur.tipodocumento td ON anx.tpdid = td.tpdid
			LEFT JOIN 
				conjur.numeracaodocumento nd ON td.tpdid = nd.tpdid 
					AND nd.nudstatus = 'A' 
					and nd.prcid = anx.prcid 
					and nd.nudid = anx.nudid
			LEFT JOIN 
				public.arquivo arq ON arq.arqid = anx.arqid 
			LEFT JOIN 
				seguranca.usuario usu ON usu.usucpf = arq.usucpf  
			WHERE 
				anx.prcid='{$prcid}' 
			AND 
				anx.anxtipo='P' 
			AND 
				anx.anxstatus='A'";
	
	$rs = $db->pegaUm($sql);
	
	if($rs){
		return false;
	}else{
		return true;
	}
}
function conjur_regraArqCentral2011( $prcid )
{

	global $db;
	
	$sql = "SELECT 
				TRUE
			FROM	
				conjur.processoconjur prc
			INNER JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
			INNER JOIN conjur.coordenacao coo ON coo.coonid = prc.cooid
			INNER JOIN workflow.documento doc ON doc.docid = esp.docid AND doc.tpdid = 49
			INNER JOIN workflow.historicodocumento wd ON wd.hstid = doc.hstid
			WHERE
				prc.prcid = $prcid
				AND to_char(htddata,'YYYY') = '2011'
			";	
//	ver($sql,d);
	$rs = $db->pegaUm($sql);

	if($rs == 't'){
		return true;
	}else{
		return false;
	}
} 
function conjur_regraArqCentral2012( $prcid )
{

    global $db;
    
    $sql = "SELECT 
                TRUE
            FROM    
                conjur.processoconjur prc
            INNER JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
            INNER JOIN conjur.coordenacao coo ON coo.coonid = prc.cooid
            INNER JOIN workflow.documento doc ON doc.docid = esp.docid AND doc.tpdid = 49
            INNER JOIN workflow.historicodocumento wd ON wd.hstid = doc.hstid
            WHERE
                prc.prcid = $prcid
                AND to_char(htddata,'YYYY') = '2012'
            ";  
//  ver($sql,d);
    $rs = $db->pegaUm($sql);

    if($rs == 't'){
        return true;
    }else{
        return false;
    }
} 

function pegarDocidB( $lvaid )
{
	global $db;
	
	$sql = "SELECT docid
			FROM conjur.livroacervo
			WHERE lvaid = '" . $lvaid . "'";
	
	return (integer) $db->pegaUm( $sql );
}

function criarDocumentoB( $lvaid )
{
	global $db;
	
	include_once APPRAIZ . "includes/workflow.php";
	
	if(!pegarDocidB($lvaid)) {

		$tpdid = WORKFLOW_BIBLIOTECA;

		$docdsc = "Número do Acervo:" . $lvaid;
		
		// cria documento
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );	
		
		$sql = "UPDATE conjur.livroacervo SET docid = ".$docid."
				WHERE lvaid = ".$lvaid; 
		$db->executar( $sql );		
		$db->commit();
		
		return $docid;		
	}
}

function posAcaoReservar(){
	global $db;
	
	if(!$_SESSION['lvaid']) return false;
	/*	
	$sql = "SELECT	adv.advid
			 FROM entidade.entidade ent
			 inner join conjur.advogados adv on adv.entid = ent.entid
			 WHERE adv.advstatus = 'A'
			 and ent.entnumcpfcnpj = '" . $_SESSION['usucpf'] . "'";
	$advid = $db->pegaUm($sql);
	
	//atualiza emprestimo
	$sql = "SELECT	max(lveid) as lveid
				 FROM conjur.livroemprestimo
				 WHERE lvaid = ".$_SESSION['lvaid']."
				 and lvedtrealdevolucao is null
				 and lvedtliberacao is null";
	$lveid = $db->pegaUm($sql);
	if($lveid && $advid){
		
		$sql = "UPDATE conjur.livroemprestimo
			   	SET advid=".$advid.",
			   		lvedtreserva=now()
			 	WHERE lveid = ".$lveid;	
		$db->executar( $sql );		
		$db->commit();

		return true;
	}
	else{
		return false;
	}
	*/
	//fim
	
	//atualiza emprestimo
	$sql = "SELECT	max(lveid) as lveid
				 FROM conjur.livroemprestimo
				 WHERE lvaid = ".$_SESSION['lvaid']."
				 and lvedtrealdevolucao is null
				 and lvedtliberacao is null";
	$lveid = $db->pegaUm($sql);
	if($lveid){
		
		$sql = "UPDATE conjur.livroemprestimo
			   	SET lvedtreserva=now()
			 	WHERE lveid = ".$lveid;	
		$db->executar( $sql );		
		$db->commit();

		return true;
	}
	else{
		return false;
	}
	
	
	
	
}

function posAcaoEmprestar(){
	global $db;
	
	if(!$_SESSION['lvaid']) return false;

	//atualiza emprestimo
	$sql = "SELECT	max(lveid) as lveid
				 FROM conjur.livroemprestimo
				 WHERE lvaid = ".$_SESSION['lvaid']."
				 and lvedtrealdevolucao is null
				 and lvedtliberacao is null";
	$lveid = $db->pegaUm($sql);
	
	if($lveid){
		
		$sql = "UPDATE conjur.livroemprestimo
			   	SET lvedtemprestimo=now()
			 	WHERE lveid = ".$lveid;	
		$db->executar( $sql );		
		$db->commit();

		return true;
	}
	else{
		return false;
	}
	//fim
	
	
}

function posAcaoLiberar($lvaid = null){
	global $db;
	
	if(!$lvaid) return false;
	
	//atualiza emprestimo
	$sql = "SELECT	max(lveid) as lveid
				 FROM conjur.livroemprestimo
				 WHERE lvaid = ".$lvaid."
				 and lvedtrealdevolucao is null
				 and lvedtliberacao is null";
	$lveid = $db->pegaUm($sql);
	if($lveid){
		
		$sql = "UPDATE conjur.livroemprestimo
			   	SET lvedtliberacao=now()
			 	WHERE lveid = ".$lveid;	
		$db->executar( $sql );		
		$db->commit();

		return true;
	}
	else{
		return false;
	}
	//fim
	
}


function posAcaoDevolver($lvaid = null){
	global $db;
	
	if(!$lvaid) return false;
	
	$sql = "SELECT	max(lveid) as lveid
				 FROM conjur.livroemprestimo
				 WHERE lvaid = ".$lvaid."
				 and lvedtrealdevolucao is null
				 and lvedtliberacao is null";
	$lveid = $db->pegaUm($sql);
	
	if($lveid){
		
		$sql = "UPDATE conjur.livroemprestimo
			   	SET lvedtrealdevolucao=now()
			 	WHERE lveid = ".$lveid;	
		$db->executar( $sql );		
		$db->commit();

		return true;
	}
	else{
		return false;
	}
	
}




function verificaAcaoEmprestar(){
	global $db;
	
	if(!$_SESSION['lvaid']) return false;
	
	$sql = "SELECT	max(lveid) as lveid
				 FROM conjur.livroemprestimo
				 WHERE lvaid = ".$_SESSION['lvaid']."
				 and lvedtrealdevolucao is null
				 and lvedtliberacao is null
				 and advid is not null
				 and lvedtprevdevolucao is not null";
	$lveid = $db->pegaUm($sql);
	
	if($lveid){
		return true;
	}
	else{
		return false;
	}
}


/**
 * Informa emails registrados para acompanhar erros conjur por email de
 * erro presente no sistema de incompatibilidade entre a presenca da informação
 * de arquivo anexo nos conjur.anexos e os dados realmente presentes na base
 * de arquivos
 *
 * @param Array $dadosDoInforme - dados: anxid, prcid, arqid
 */
function infromaTiErroArquivoAnexoConjur( $dadosDoInforme ){
	global $db;

	$assunto = "Informe de Erro Conjur - ausência de arquivo";
	$mensagem = " 
		Prezados,<br/>
		<br/>
		Existe uma incompatibilidade de dados nos arquivos anexos a um processo no Conjur.<br/>
		Por essa razão, esse email é enviado para que o pessoal da TI saiba do que está acontecendo, e ao Anexo foi aplicado o Status 'I' (inativo) para que não apareça na lista para o usuário baixar, uma vez que o arquivo não está presente na pasta.
		<br/>
		Dados:<br/>
			conjur.anexos.anxid => {$dadosDoInforme['anxid']}<br/>
			conjur.processoconjur.prcid => {$dadosDoInforme['prcid']}<br/>
			public.arquivo.arqid => {$dadosDoInforme['arqid']}<br/>
		<br/>
		Atenciosamente,<br/>
		Equipe SIMEC. ";

	$sql = " select u.usunome, u.usuemail
			 from seguranca.usuario u
			 inner join seguranca.envioerrosusuarios eeu on eeu.usucpf = u.usucpf
			 inner join seguranca.envioerrosususistema eeus on eeus.eeuid = eeu.eeuid
			 where eeus.sisid = ".CONJUR_SISID." ";
	$listaEmail = $db->carregar( $sql );

	if( !empty($listaEmail) )
	foreach ($listaEmail as $key => $value) {

		$remetente = '';
		$destinatario = array('usunome'=>$value['usunome'],'usuemail'=>$value['usuemail']);

		enviar_email( $remetente, $destinatario, $assunto, $mensagem );
	}
}


    function relatorioMaior_15( $dados ){
        global $db;

        if( $_REQUEST['maior_15'] == 'S' ){
            extract($_POST);

            $data_ini = formata_data_sql( $_POST['htddatainicial'] );
            $data_fim = formata_data_sql( $_POST['htddatfinal'] );

            if( $agrupamentoCoordenacao != '' ){
                $colunas = "aca.aeddscrealizada as coordenacao";

                $join = "
                    LEFT JOIN conjur.historicocoordenacoes hco ON hco.hstid = his.hstid
                    LEFT JOIN conjur.coordenacao co1 ON co1.coonid = hco.coonid
                ";

                $group = "coordenacao";

                if( $coonid[0] ){
                    $where[] = " aca.aedid IN ('" . implode( "','", $coonid ) . "') ";
                }
            }

            if( $agrupamentoAdvogados != '' ){
                $colunas = "ent.entnome AS nome_advogado";

                $join = "
                    LEFT JOIN conjur.historicoadvogados had ON had.hstid = his.hstid
                    LEFT JOIN conjur.advogados adv ON adv.advid = had.advid
                    LEFT JOIN entidade.entidade ent ON ent.entid = adv.entid

                    LEFT JOIN conjur.coordenacao co2 ON co2.coonid = adv.coonid
                ";

                $group = "nome_advogado";

                $where[] = "aca.aedid in ( ".ACAO_ENCAMINHAR_PARA_ADVOGADO." )";

                if( $advid[0] ){
                    $where[] = " had.advid IN ('" . implode( "','", $advid ) . "') ";
                }
            }

            if( count($where) > 0 ){
                $and = ' AND ' . implode(' AND ', $where);
            }

            $prcnumsidoc = "'<a style=\"cursor:pointer; color:black;\" onclick=\"chamaproc(' || prc.prcid || ');\">' || prc.prcnumsidoc || '</a>' as prcnumsidoc";
            $prcdesc = "'<a style=\"cursor:pointer; color:black;\" onclick=\"chamaproc(' || prc.prcid || ');\">' || prc.prcdesc || '</a>' as prcdesc";
            
            $sql = "
                SELECT  {$colunas},
                        prc.prcid,
                        {$prcnumsidoc},
                        {$prcdesc},
                        to_char(prc.prcdtentrada, 'DD/MM/YYYY') as prcdtentrada

                FROM workflow.historicodocumento his

                JOIN workflow.documento doc ON doc.docid = his.docid
                JOIN workflow.acaoestadodoc aca ON aca.aedid = his.aedid
                JOIN workflow.estadodocumento eca ON eca.esdid = aca.esdidorigem

                JOIN conjur.estruturaprocesso est ON est.docid = his.docid
                JOIN conjur.processoconjur prc ON prc.prcid = est.prcid

                {$join}

                WHERE his.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' {$and}

                AND (SELECT max(htddata)::date FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) > (SELECT max(htddata)::date FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' )

                GROUP BY {$group}, prc.prcid, prc.prcnumsidoc, prc.prcdesc, prc.prcdtentrada, his.hstid

                HAVING SUM(
                        CASE WHEN (
                            extract( year from age(
                                    ( COALESCE( ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1479) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1469) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), '{$data_fim}' ) )::DATE,
                                    ( (SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) )::DATE
                            ) ) * 360 +
                            extract( month from age(
                                    ( COALESCE( ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1479) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1469) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), '{$data_fim}' ) )::DATE,
                                    ( (SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) )::DATE
                            ) ) * 30 +
                            extract( day from age(
                                    ( COALESCE( ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1479) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1469) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), '{$data_fim}' ) )::DATE,
                                    ( (SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) )::DATE
                            ) ) ) > 15
                                    THEN 1
                                    ELSE 0
                        END
                    ) = 1

                ORDER BY 1, 2
            ";
            $cabecalho = array("Advogado", "Cód. Processo", "Número Sidoc", "Desc. Processo", "Data de Entrada");
            $alinhamento = Array('', '', '', '', '');
            $tamanho = Array('25%', '5%', '10%', '60%', '5%');
            echo '<table align="center" border="0" class="tabela" cellpadding="5" cellspacing="1">';
            echo '<tr> <td class="subTituloCentro"> LISTAGEM DOS PROCESSOS QUE ESTÃO EM PRAZO MAIOR QUE 15 DIAS </td> </tr>';
            echo '<tr> <td>';
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
            echo '</td> </tr>';
            echo '</table>';
        }
    }

    function salvaPrazoAdvogado($prcid, $advid, $haddtprazoadv){
        global $db;

        $sql   = "select max(hadid) as hadid from conjur.historicoadvogados where prcid = {$prcid} and advid = {$advid}";
        $hadid = $db->pegaUm($sql);

        if (!empty($hadid)){
            $sql = "update conjur.historicoadvogados set
                        haddtprazoadv=".(($haddtprazoadv)?"'".formata_data_sql($haddtprazoadv)."'":"NULL")."
                    where hadid = {$hadid}";

            $res = $db->executar($sql);
        }

        return true;
    }

    function form_definePrazoAdvogado(){
        global $db;

        $prcid = $_POST['prcid'];

        $sql = "select
                    prcnumsidoc,
                    (
                        select
                            ent.entnome
                        from conjur.advogados adv
                        inner join entidade.entidade ent on ent.entid = adv.entid
                        inner join conjur.historicoadvogados had on had.advid = adv.advid
                        where had.hadid = (select max(hadid) from conjur.historicoadvogados where prcid = {$prcid})
                    ) as entnome,
                    (select max(hadid) from conjur.historicoadvogados where prcid = {$prcid}) as hadid
                from conjur.processoconjur
                where prcid = {$prcid}";
        $res = $db->pegaLinha($sql);

        ?>
        <div>
            <h3>Definição do Prazo Advogado</h3>
            <table>
                <tr>
                    <td class="subtituloDireita" style="width:20%;">N° do Processo:</td>
                    <td>
                        <input type="hidden" id="hadid" name="hadid" value="<?= $res['hadid'] ?>"/>
                        <label for="" id="lblTeste1"><?= $res['prcnumsidoc'] ?></label>
                    </td>
                </tr>
                <tr>
                    <td class="subtituloDireita" style="width:20%;">Advogado:</td>
                    <td><?= $res['entnome'] ?></td>
                </tr>
                <tr>
                    <td class="subtituloDireita" style="width:20%;">Prazo Advogado:</td>
                    <td>
                        <?php
                            echo campo_data('haddtprazoadv', 'S', 'S', '', 'S', '', '', null, 'formdefinePrazoAdvogado');
                        ?>
                    </td>
                </tr>
            </table>
        </div>
        <?php

    }

function definePrazoAdvogado(){
    return WorkflowConjur::preAcaoDefinePrazoAdvogado();
}

function inserirarquivosapiensconjur($dados) {
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

    // BUG DO IE
    // O type do arquivo vem como image/pjpeg
    if($arquivo["type"] == 'image/pjpeg') {
        $arquivo["type"] = 'image/jpeg';
    }

    //Insere o registro do arquivo na tabela public.arquivo
    $sql = " INSERT INTO public.arquivo
				(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
			VALUES
				('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".substr($dados["anxdesc"],0,255)."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .")
			RETURNING arqid; ";
    $arqid = $db->pegaUm($sql);

//    if(!$dados['nudid']){
//        $dados['nudid'] = 'null';
//    }

    //Insere o registro na tabela obras.arquivosobra
    $sql = "INSERT INTO conjur.sapiensanexo(
	            spdid, arqid, spadsc, spastatus,usucpf,spadtanexo)
    		VALUES (
    		".(($dados['nudid'])?"'".$dados['nudid']."'":"NULL").",
    		".$arqid.",
    		'".substr($dados['anxdesc'],0,255)."',
    		'A','{$_SESSION['usucpf']}','" . date('Y-m-d') . "');";
    $db->executar($sql);

    // se não existir a pasta, cria no servidor
    if(!is_dir('../../arquivos/conjur/')) {
        mkdir(APPRAIZ.'/arquivos/conjur/', 0777);
    }

    // se não existir o arquivo, cria no servidor
    if(!is_dir('../../arquivos/conjur/'.floor($arqid/1000))) {
        mkdir(APPRAIZ.'/arquivos/conjur/'.floor($arqid/1000), 0777);
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
        direcionar('?modulo=principal/documentosSapiensAnexo&acao=A','Gravação efetuada com sucesso');
    }else{
        $db->rollback();
        direcionar('?modulo=inicio&acao=C','Problemas no envio localizar o arquivo no servidor.');
    }



//    switch($dados['anxtipo']) {
//        case 'P':
//            direcionar('?modulo=principal/documento&acao=A','Gravação efetuada com sucesso');
//            break;
//        case 'E':
//          //  direcionar('?modulo=principal/expediente_lancamento&acao=A&expid='.$dados['expid'],'Gravação efetuada com sucesso');
//            break;
//    }
}


?>