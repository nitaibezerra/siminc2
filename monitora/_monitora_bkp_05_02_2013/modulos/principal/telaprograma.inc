<?
ini_set('memory_limit', '16M');

function excluirValorIndicadores($dados) {
	global $db;
	$sql = "UPDATE monitora.valorindicador SET vlistatus='I' WHERE vliid='".$dados['vliid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Valor excluido com sucesso');
			window.location='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarIndicadores&idpid=".$dados['idpid']."';
	      </script>";
	
}

function excluirValorMetas($dados) {
	global $db;
	$sql = "UPDATE monitora.valormeta SET vlmstatus='I' WHERE vlmid='".$dados['vlmid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Valor excluido com sucesso');
			window.location='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarMetas&metid=".$dados['metid']."';
	      </script>";
	
}

function inserirValorMetas($dados) {
	global $db;
	$sql = "INSERT INTO monitora.valormeta(
            metid, vlmreferencia, vlmvalor, vlmstatus, vlmordem, usucpfcadastro, 
            usucpfalteracao, dpeid)
		    VALUES ('".$dados['metid']."', NULL, '".str_replace(array(".",","),array("","."),$dados['vlmvalor'])."', 'A', NULL, '".$_SESSION['usucpf']."', 
		            NULL, '".$dados['dpeid']."');";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Valor inserido com sucesso');
			window.location='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarMetas&metid=".$dados['metid']."';
	      </script>";
	
}

function inserirValorIndicadores($dados) {
	global $db;
	$sql = "INSERT INTO monitora.valorindicador(
            idpid, vlireferencia, vlivalor, vlistatus, vliordem, usucpfcadastro, 
            usucpfalteracao, dpeid)
    		VALUES ('".$dados['idpid']."', NULL, '".str_replace(array(".",","),array("","."),$dados['vlivalor'])."', 'A', NULL, '".$_SESSION['usucpf']."', 
            NULL, '".$dados['dpeid']."');";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Valor inserido com sucesso');
			window.location='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarIndicadores&prgcod=".$dados['prgcod']."&idpid=".$dados['idpid']."';
	      </script>";

}

function atualizarIndicadores($dados) {
	global $db;
	$sql = "UPDATE monitora.indicadorprograma SET idpdsc='".$dados['idpdsc']."', umiid='".$dados['umiid']."', perid='".$dados['perid']."', usucpfalteracao='".$_SESSION['usucpf']."' WHERE idpid='".$dados['idpid']."'";
	$db->executar($sql);
	$db->commit();
	
	if($dados['atualizartodapaginapai']) {
		
		echo "<script>
				alert('Indicadores atualizados com sucesso');
				window.opener.location = window.opener.location;
				window.close();
			  </script>";
		
	} else {
		
		echo "<script>
				alert('Indicadores atualizados com sucesso');
				if(window.opener.document.getElementById('programa_".$dados['prgcod']."').title=='mais') {
					window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
				} else {
					window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
					window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
				}
				window.close();
			  </script>";
		
	}
}

function atualizarIniciativas($dados) {
	global $db;
	$sql = "UPDATE monitora.iniciativa SET ictdsc='".$dados['ictdsc']."', usucpfalteracao='".$_SESSION['usucpf']."' WHERE ictid='".$dados['ictid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Iniciativa atualizada com sucesso');
			if(window.opener.document.getElementById('objetivo_".$dados['objid']."').title=='mais') {
				window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
			} else {
				window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
				window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
			}
			window.close();
		  </script>";
}

function atualizarMetas($dados) {
	global $db;
	
	$sql = "UPDATE monitora.meta SET metdsc='".$dados['metdsc']."', perid='".$dados['perid']."' WHERE metid='".$dados['metid']."'";
	$db->executar($sql);
	$db->commit();
	
	$sql = "delete from monitora.metaindicador where metid = {$dados['metid']}";
	$db->executar($sql);
	if($dados['indid'][0]) {
		foreach($dados['indid'] as $indid){ 
			$sqlI.="insert into monitora.metaindicador (metid,indid) values ({$dados['metid']},$indid);";
		}
		$db->executar($sqlI);
	}
	$db->commit();
	
	if($dados['atualizartodapaginapai']) {
		echo "<script>
				alert('Metas atualizada com sucesso');
				window.opener.location=window.opener.location;
				window.close();
			  </script>";
	} else {
		echo "<script>
				alert('Metas atualizada com sucesso');
				if(window.opener.document.getElementById('objetivo_".$dados['objid']."').title=='mais') {
					window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
				} else {
					window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
					window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
				}
				window.close();
			  </script>";
	}
}

function atualizarObjetivos($dados) {
	global $db;
	$sql = "UPDATE monitora.objetivo SET 
			objcod='".$dados['objcod']."', 
			objdsc='".$dados['objdsc']."', 
			usucpfalteracao='".$_SESSION['usucpf']."',
			objcaracterizacao=".(($dados['objcaracterizacao'])?"'".$dados['objcaracterizacao']."'":"NULL")." 
			WHERE objid='".$dados['objid']."'";
	$db->executar($sql);
	$db->commit();
	
	if($dados['atualizartodapaginapai']) {
		
		echo "<script>
				alert('Objetivo atualizado com sucesso');
				window.opener.location = window.opener.location; 
				window.close();
			  </script>";
		
	} else {
		
		echo "<script>
				alert('Objetivo atualizado com sucesso');
				if(window.opener.document.getElementById('programa_".$dados['prgcod']."').title=='mais') {
					window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
				} else {
					window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
					window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
				}
				window.close();
			  </script>";
		
	}
	
}

function excluirMetas($dados) {
	global $db;
	$sql = "UPDATE monitora.meta SET metstatus='I' WHERE metid='".$dados['metid']."'";
	$db->executar($sql);
	$db->commit();
	echo "Meta excluída com sucesso";
}

function excluirIndicadores($dados) {
	global $db;
	$sql = "UPDATE monitora.indicadorprograma SET idpstatus='I' WHERE idpid='".$dados['idpid']."'";
	$db->executar($sql);
	$db->commit();
	echo "Indicador excluído com sucesso";
}

function excluirIniciativas($dados) {
	global $db;
	$sql = "UPDATE monitora.iniciativa SET ictstatus='I' WHERE ictid='".$dados['ictid']."'";
	$db->executar($sql);
	$db->commit();
	echo "Iniciativa excluído com sucesso";
}

function excluirObjetivos($dados) {
	global $db;
	$sql = "UPDATE monitora.objetivo SET objstatus='I' WHERE objid='".$dados['objid']."'";
	$db->executar($sql);
	$db->commit();
	echo "Objetivo excluído com sucesso";
}

function excluirAcoes($dados) {
	global $db;
	$sql = "UPDATE monitora.iniciativaacao SET icastatus='I' WHERE icaid='".$dados['icaid']."'";
	$db->executar($sql);
	$db->commit();
	echo "Ação excluída com sucesso";
}

function excluirAnaliseSituacionalIniciativa( $dados ){
	global $db;

	if ( $dados['ianid'] && $dados['ictid'] ){
		$sql = "UPDATE monitora.iniciativaanexo
				SET ianstatus = 'I'
				WHERE
					 ianid = {$dados['ianid']} AND 
					 ictid = {$dados['ictid']}";
		
		$db->executar( $sql );
		$db->commit();
	}
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.location='?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalIniciativa&ictid={$dados['ictid']}&aniid={$dados['aniid']}';
		  </script>";
	
}

function excluirAnaliseSituacionalObjetivo( $dados ){
	global $db;

	if ( $dados['oanid'] && $dados['objid'] ){
		$sql = "UPDATE monitora.objetivoanexo
				SET oanstatus = 'I'
				WHERE
					 oanid = {$dados['oanid']} AND 
					 objid = {$dados['objid']}";
		
		$db->executar( $sql );
		$db->commit();
	}
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.location='?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalObjetivo&anoid={$dados['anoid']}&objid={$dados['objid']}';
		  </script>";
	
}

function excluirAnaliseSituacionalMeta( $dados ){
	global $db;

	if ( $dados['manid'] && $dados['metid'] ){
		$sql = "UPDATE monitora.metaanexo
				SET manstatus = 'I'
				WHERE
					 manid = {$dados['manid']} AND 
					 metid = {$dados['metid']}";
		
		$db->executar( $sql );
		$db->commit();
	}
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.location='?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalMeta&anmid={$dados['anmid']}&metid={$dados['metid']}';
		  </script>";
	
}

function inserirAcoes($dados) {
	global $db;
	
	$sql = "DELETE FROM monitora.iniciativaacao WHERE ictid='".$dados['ictid']."'";
	$db->executar($sql);
	
	if($dados['acoes']) {
		foreach($dados['acoes'] as $acacod => $acadsc) {
			$sql = "INSERT INTO monitora.iniciativaacao(
            		ictid, acacod, acadsc, icastatus)
    				VALUES ('".$dados['ictid']."', '".$acacod."', '".$acadsc."', 'A');";
			$db->executar($sql);
		}
	}
	$db->commit();
	
	echo "<script>
			alert('Ações inseridas com sucesso');
			if(window.opener.document.getElementById('iniciativa_".$dados['ictid']."').title=='mais') {
				window.opener.carregarAcoes('".$dados['ictid']."',window.opener.document.getElementById('iniciativa_".$dados['ictid']."'));
			} else {
				window.opener.carregarAcoes('".$dados['ictid']."',window.opener.document.getElementById('iniciativa_".$dados['ictid']."'));
				window.opener.carregarAcoes('".$dados['ictid']."',window.opener.document.getElementById('iniciativa_".$dados['ictid']."'));
			}
			window.close();
		  </script>";
	
}

function inserirIniciativas($dados) {
	global $db;
	
	$sql = "INSERT INTO monitora.iniciativa(
            ictcod, ictdsc, ictstatus, usucpfcadastro, usucpfalteracao)
    		VALUES ('".$dados['ictcod']."', '".$dados['ictdsc']."', 'A', '".$_SESSION['usucpf']."', NULL) RETURNING ictid;";
	
	$ictid = $db->pegaUm($sql);
	
	$sql = "INSERT INTO monitora.objetivoiniciativa(
            ictid, objid)
    		VALUES ('".$ictid."', '".$dados['objid']."');";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Iniciativa inserida com sucesso');
			if(window.opener.document.getElementById('objetivo_".$dados['objid']."').title=='mais') {
				window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
			} else {
				window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
				window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
			}
			window.close();
		  </script>";
}

function downloadAnaliseSituacionalObjetivo( $dados ){
	global $db;

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("objetivoanexo", $campos ,"monitora");
	$file->getDownloadArquivo($dados['arqid']);
	
}

function inserirAnaliseSituacionalObjetivo( $dados ){
	global $db;
	
	if ( isset($dados['anodsc']) ){
		
		if ($dados['anoid'] == ''){
			if($dados['unmcod'] != null){
			$sql = "INSERT INTO monitora.analiseobjetivo (anodsc, anostatus, usucpfcadastro, objid, refcod, unmcod
					) VALUES ('{$dados['anodsc']}','A','{$_SESSION['usucpf']}','{$dados['objid']}', {$dados['refcod']}, " . ($dados['unmcod'] ? $dados['unmcod'] : 'NULL') . ") returning anoid";
			
            $dados['anoid'] = $db->pegaUm( $sql, 'anoid');
			}else{
			$sql = "INSERT INTO monitora.analiseobjetivo (anodsc, anostatus, usucpfcadastro, objid, refcod
					) VALUES ('{$dados['anodsc']}','A','{$_SESSION['usucpf']}','{$dados['objid']}', {$dados['refcod']}) returning anoid";
			
            $dados['anoid'] = $db->pegaUm( $sql, 'anoid');
				
			}
            
		}else{
			if($dados['unmcod'] != null){
				$sql = "UPDATE 
							monitora.analiseobjetivo
		   				SET anodsc = '{$dados['anodsc']}', refcod = {$dados['refcod']}, unmcod = " . ($dados['unmcod'] ? $dados['unmcod'] : 'NULL') . " 
		   				WHERE 
		 					anoid = {$dados['anoid']};";
				
				$db->executar( $sql );
			}else{
				$sql = "UPDATE 
							monitora.analiseobjetivo
		   				SET anodsc = '{$dados['anodsc']}', refcod = {$dados['refcod']} 
		   				WHERE 
		 					anoid = {$dados['anoid']};";
				
				$db->executar( $sql );
				
			}
		}
		
		$sql = "INSERT INTO monitora.historicoanaliseobjetivo  (
			            haodsc, usucpfcadastro, anoid, haosituacao
				)VALUES('{$dados['anodsc']}', '" . $_SESSION['usucpf'] . "', " . $dados['anoid'] . ",'" . $dados['anosituacao'] . "'  );";
		$db->executar($sql);
		$db->commit();
		$txt = 'Análise situacional inserida com sucesso';
	}else{
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$campos = array('oandescricao' => "'" . $dados['oandescricao'] . "'",
						'objid'		   => $dados['objid']);
		
		$file = new FilesSimec("objetivoanexo", $campos ,"monitora");
		$file->setUpload();
		$txt = 'Anexo da análise situacional inserida com sucesso';
	}
	
	echo "<script>
			alert('" . $txt . "');
			window.location='?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalObjetivo&objid={$dados['objid']}&anoid={$dados['anoid']}';
		  </script>";
	
}

function inserirAnaliseSituacionalMeta( $dados ){
	global $db;
	
	if ( isset($dados['anmdsc']) ){
		if ($dados['anmid'] == '')
		{
			if($dados['unmcod'] != null){
				$sql = "INSERT INTO monitora.analisemeta (
							anmdsc, anmstatus, usucpfcadastro, 
							metid, anmquantidadealcancada, anmdtreferencia, 
							refcod, unmcod 
						) VALUES (
						'{$dados['anmdsc']}','A','{$_SESSION['usucpf']}',
						'{$dados['metid']}', " .  ($dados['anmquantidadealcancada'] ? str_replace(array(".",","),array("","."),$dados['anmquantidadealcancada']) : 'null')   . ", 
						". ($dados['anmdtreferencia'] ? "'".formata_data_sql($dados['anmdtreferencia'])."'" : 'null') .", {$dados['refcod']}, {$dados['unmcod']}) returning anmid";
					
				$dados['anmid'] = $db->pegaUm( $sql, 'anmid');
			}else{
				$sql = "INSERT INTO monitora.analisemeta (
							anmdsc, anmstatus, usucpfcadastro, 
							metid, anmquantidadealcancada, anmdtreferencia, 
							refcod 
						) VALUES (
						'{$dados['anmdsc']}','A','{$_SESSION['usucpf']}',
						'{$dados['metid']}', " .  ($dados['anmquantidadealcancada'] ? str_replace(array(".",","),array("","."),$dados['anmquantidadealcancada']) : 'null')   . ", 
						". ($dados['anmdtreferencia'] ? "'".formata_data_sql($dados['anmdtreferencia'])."'" : 'null') .", {$dados['refcod']}) returning anmid";
					
				$dados['anmid'] = $db->pegaUm( $sql, 'anmid');
				
			}
		}
		else 
		{
			if($dados['unmcod'] != null){
				$sql = "UPDATE 
							monitora.analisemeta
		   				SET 
			   				anmdsc = '{$dados['anmdsc']}',
			   				anmquantidadealcancada = " . ($dados['anmquantidadealcancada'] ? str_replace(array(".",","),array("","."),$dados['anmquantidadealcancada']) : 'null') . ",
			   				anmdtreferencia = ". ($dados['anmdtreferencia'] ? "'" . formata_data_sql($dados['anmdtreferencia']) . "'" : 'null') . ", 
			   				refcod = {$dados['refcod']},
			   				unmcod = {$dados['unmcod']} 
		   				WHERE anmid = {$dados['anmid']};";
				$db->executar( $sql );
			}else{
				$sql = "UPDATE 
							monitora.analisemeta
		   				SET 
			   				anmdsc = '{$dados['anmdsc']}',
			   				anmquantidadealcancada = " . ($dados['anmquantidadealcancada'] ? str_replace(array(".",","),array("","."),$dados['anmquantidadealcancada']) : 'null') . ",
			   				anmdtreferencia = ". ($dados['anmdtreferencia'] ? "'" . formata_data_sql($dados['anmdtreferencia']) . "'" : 'null') . ", 
			   				refcod = {$dados['refcod']}
		   				WHERE anmid = {$dados['anmid']};";
				$db->executar( $sql );
				
			}
		}
		
		$sql = "INSERT INTO monitora.historicoanalisemeta  (
			            hamdsc, usucpfcadastro, anmid,  hamquantidadealcancada, hamdtreferencia, hamsituacao
				)VALUES('{$dados['anmdsc']}', '" . $_SESSION['usucpf'] . "', " 
						. $dados['anmid'] . ", " . ($dados['anmquantidadealcancada'] ? str_replace(array(".",","),array("","."),$dados['anmquantidadealcancada']) : 'null') . ", "  
						. ($dados['anmdtreferencia'] ? "'" . formata_data_sql($dados['anmdtreferencia']) . "'" : 'null')   . " ,'" . $dados['anmsituacao'] . "' );";
		$db->executar($sql);
		$db->commit();
		$txt = 'Análise situacional inserida com sucesso';
	}else{
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$campos = array('mandescricao' => "'" . $dados['mandescricao'] . "'",
						'metid'		   => $dados['metid']);
		
		$file = new FilesSimec("metaanexo", $campos ,"monitora");
		$file->setUpload();
		$txt = 'Anexo da análise situacional inserida com sucesso';
	}
	
	echo "<script>
			alert('" . $txt . "');
			window.location='?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalMeta&metid={$dados['metid']}&anmid={$dados['anmid']}';
		  </script>";
	
}

function inserirAnaliseSituacionalIniciativa( $dados ){
	global $db;
	
	if ( isset($dados['anidsc']) ){
		
		if ($dados['aniid'] == '')
		{
			$sql = "INSERT INTO monitora.analiseiniciativa (
						anidsc, anistatus, usucpfcadastro, ictid, refcod
					) VALUES (
						'{$dados['anidsc']}','A','{$_SESSION['usucpf']}','{$dados['ictid']}', {$dados['refcod']}
					) returning aniid";
			
			$dados['aniid'] = $db->pegaUm( $sql, 'aniid');
		}
		else
		{
		$sql = "UPDATE monitora.analiseiniciativa
   				SET anidsc = '{$dados['anidsc']}',
   				refcod = {$dados['refcod']}
 				WHERE
 				aniid = {$dados['aniid']};";
		
		$db->executar( $sql );
		}
		
		
		$sql = "INSERT INTO monitora.historicoanaliseiniciativa  (
			            haidsc, usucpfcadastro, aniid, haisituacao
				)VALUES('{$dados['anidsc']}', '" . $_SESSION['usucpf'] . "', " . $dados['aniid'] . ",'" . $dados['anisituacao'] . "');";
		
		$db->executar($sql);
		$db->commit();
		$txt = 'Análise situacional inserida com sucesso';
	}else{
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$campos = array('iandescricao' => "'" . $dados['iandescricao'] . "'",
						'ictid'		   => $dados['ictid']);
		
		$file = new FilesSimec("iniciativaanexo", $campos ,"monitora");
		$file->setUpload();
		$txt = 'Anexo da análise situacional inserida com sucesso';
	}
	
	echo "<script>
			alert('" . $txt . "');
			window.location='?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalIniciativa&ictid={$dados['ictid']}&aniid={$dados['aniid']}';
		  </script>";
	
}

function inserirMetas($dados) {
	global $db;
	$sql = "INSERT INTO monitora.meta(
            metdsc, metstatus, perid)
    		VALUES ('".$dados['metdsc']."', 'A', '".$dados['perid']."') RETURNING metid;";
	
	$metid = $db->pegaUm($sql);
	
	$sql = "INSERT INTO monitora.objetivometa(
            metid, objid)
    		VALUES ('".$metid."', '".$dados['objid']."');";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Meta inserido com sucesso');
			if(window.opener.document.getElementById('objetivo_".$dados['objid']."').title=='mais') {
				window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
			} else {
				window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
				window.opener.carregarMetasIniciativas('".$dados['objid']."',window.opener.document.getElementById('objetivo_".$dados['objid']."'));
			}
			window.location='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarMetas&metid=".$metid."';
		  </script>";
	
	
}

function inserirObjetivos($dados) {
	global $db;
	$sql = "INSERT INTO monitora.objetivo(
            prgano, 
            prgcod, 
            prgid, 
            objcod, 
            objdsc, 
            objstatus, 
            objdtinclusao, 
            usucpfcadastro, 
            usucpfalteracao,
            objcaracterizacao)
    VALUES ('".$_SESSION['exercicio']."', 
    		'".$dados['prgcod']."', 
    		(select prgid from monitora.programa where prgcod='".$dados['prgcod']."' and prgano='".$_SESSION['exercicio']."'), 
    		'".$dados['objcod']."', 
    		'".$dados['objdsc']."', 
    		'A', 
    		NOW(), 
            '".$_SESSION['usucpf']."', 
            NULL,
            ".(($dados['objcaracterizacao'])?"'".$dados['objcaracterizacao']."'":"NULL").") RETURNING objid;";
	
	$objid = $db->pegaUm($sql);
	$db->commit();
	
	echo "<script>
			alert('Objetivo inserido com sucesso');
			if(window.opener.document.getElementById('programa_".$dados['prgcod']."').title=='mais') {
				window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
			} else {
				window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
				window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
			}
			window.close();
		  </script>";
	
}

function inserirIndicadores($dados) {
	global $db;
	
	$sql = "INSERT INTO monitora.indicadorprograma(
            umiid, prgano, prgcod, prgid, idpdsc, idpdtinclusao, idpstatus, 
            usucpfcadastro, usucpfalteracao, perid)
    		VALUES ('".$dados['umiid']."', 
    				'".$_SESSION['exercicio']."', 
    				'".$dados['prgcod']."', 
    				(select prgid from monitora.programa where prgcod='".$dados['prgcod']."' and prgano='".$_SESSION['exercicio']."'), 
    				'".$dados['idpdsc']."', 
    				NOW(), 
    				'A', 
    				'".$_SESSION['usucpf']."', 
            		NULL, 
            		'".$dados['perid']."') RETURNING idpid;";
	
	$idpid = $db->pegaUm($sql);
	$db->commit();
	
	echo "<script>
			alert('Indicador inserido com sucesso');
			if(window.opener.document.getElementById('programa_".$dados['prgcod']."').title=='mais') {
				window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
			} else {
				window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
				window.opener.carregarIndicadoresObjetivos('".$dados['prgcod']."',window.opener.document.getElementById('programa_".$dados['prgcod']."'));
			}
			window.location='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarIndicadores&prgcod=".$dados['prgcod']."&idpid=".$idpid."';
		  </script>";

}

function gerenciarAcoes($dados) {
	global $db;
	
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	echo "<script>";
	
	echo "function validarAcoes(){
		  divCarregando();
		  document.getElementById('form_acao').submit();
		  }";
	
	echo "</script>";
	
	echo "<form method=post id=form_acao>";
	echo "<input type=hidden name=ictid value=".$dados['ictid'].">";
	
	echo "<input type=hidden name=requisicao value=inserirAcoes>";
	
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir ações</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td colspan=\"2\">";
	$sql = "SELECT DISTINCT '<input type=checkbox name=\"acoes['||acacod||']\" value=\"'||acadsc||'\">' as chk, acacod||' - '||acadsc FROM monitora.acao WHERE acasnrap=FALSE AND prgcod IN(SELECT prgcod FROM monitora.objetivo o INNER JOIN monitora.objetivoiniciativa i ON i.objid = o.objid WHERE i.ictid=".$dados['ictid'].")";
	$cabecalho = array("&nbsp;", "Ações");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	echo "</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" colspan=\"2\"><input type=button name=salvar value=Salvar onclick=validarAcoes();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
	
}

function gerenciarMetas($dados) {
	global $db;
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	echo "<script>";
	
	echo "function validarMetas(){
		  if(document.getElementById('metdsc').value=='') {alert('Preencha a descrição');return false;}
		  if(document.getElementById('perid').value=='') {alert('Selecione periodicidade');return false;}
		  //if(document.getElementById('unmcod').value=='') {alert('Selecione uma unidade de medida');return false;}
		  divCarregando();
		  selectAllOptions( form_metas.indid );
		  document.getElementById('form_metas').submit();
		  }";
	
	echo "function validarValorMetas(){
		  if(document.getElementById('vlmvalor').value=='') {alert('Preencha a descrição');return false;}
		  if(document.getElementById('dpeid').value=='') {alert('Selecione a unidade de medida');return false;}
		  divCarregando();
		  document.getElementById('requisicao').value='inserirValorMetas';
		  selectAllOptions( form_metas.indid );
		  document.getElementById('form_metas').submit();
		  }";
	
	echo "function excluirValorMeta(vlmid) {
		  var conf = confirm('Deseja confirmar a exclusão deste valor');
		  if(conf) {
		  window.location='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=excluirValorMetas&metid=".$dados['metid']."&vlmid='+vlmid;
		  }
		  }";
	
	echo "function onOffCampo( campo )
		{
			var div_on = document.getElementById( campo + '_campo_on' );
			var div_off = document.getElementById( campo + '_campo_off' );
			var input = document.getElementById( campo + '_campo_flag' );
			if ( div_on.style.display == 'none' )
			{
				div_on.style.display = 'block';
				div_off.style.display = 'none';
				input.value = '1';
			}
			else
			{
				div_on.style.display = 'none';
				div_off.style.display = 'block';
				input.value = '0';
			}
		}";
	
	
	echo "</script>";
	
	echo "<form method=post id=form_metas name=form_metas >";
	echo "<input type=hidden name=objid value=".$dados['objid'].">";
	
	if($dados['metid']) {
		echo "<input type=hidden name=requisicao id=requisicao value=atualizarMetas>";
		echo "<input type=hidden name=metid value=".$dados['metid'].">";
		if($dados['atualizartodapaginapai']) echo "<input type=hidden name=atualizartodapaginapai value=TRUE>";
		
		$dadosmeta = $db->pegaLinha("SELECT * FROM monitora.meta idp WHERE metid='".$dados['metid']."'");
		extract($dadosmeta);
		
	} else {
		echo "<input type=hidden name=requisicao value=inserirMetas>";
	}
	
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir meta</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Descrição da meta:</td>";
	echo "<td>".campo_textarea( 'metdsc', 'S', 'S', '', '50', '4', '2500', '', '', '', '', '', trim($metdsc))."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Periodicidade:</td>";
	
	$sql = "SELECT perid as codigo, perdsc as descricao 
			FROM painel.periodicidade 
			WHERE perstatus='A'";
	
	echo "<td>".$db->monta_combo('perid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'perid', true, $perid)."</td>";
	
	echo "</tr>";
	
	$stSql = "SELECT
				i.indid AS codigo,
				indnome AS descricao
			  FROM
			  	painel.indicador i
			  inner join
			  	painel.agendaindicador a ON a.indid = i.indid
			  WHERE
			  	--agiid = 8
			  --and
			  	indstatus = 'A'
			  ORDER BY
			  	indnome";
	//dbg($stSql);
	if($dados['metid']){
		$stSqlCarregados = "select
								i.indid as codigo,
								i.indnome AS descricao
							from
								monitora.metaindicador m
							inner join
								painel.indicador i ON i.indid = m.indid
							inner join
			  					painel.agendaindicador a ON a.indid = i.indid
							where
								--aggid = 8
							--and
								metid = {$dados['metid']}
							and
								i.indstatus = 'A'";
	}
	
	$where = array(
					array(
							"codigo" 	=> "indnome",
							"descricao" => "Indicador",
							"numeric"	=> false
				   		  )
							   );
	mostrarComboPopup( 'Indicador(es)', 'indid',  $stSql, $stSqlCarregados, 'Selecione o(s) Indicador(es)', $where ); 
	
	if($dados['metid']) {
		$sql = "SELECT dpeid as codigo, dpedsc as descricao FROM painel.detalheperiodicidade WHERE dpestatus='A' AND perid='".$perid."' order by dpedatainicio";
		echo "<tr><td colspan=\"2\" class=SubTituloEsquerda>Valor : <input type=\"text\" class=\"normal\" name=\"vlmvalor\" id=\"vlmvalor\" size=\"15\" onkeyup=\"this.value=mascaraglobal('###.###.###,##',this.value);\"> ".$db->monta_combo('dpeid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'dpeid', true, $perid)." <input type=button name=adicionar value=Adicionar onclick=\"validarValorMetas();\"></td></tr>";
		echo "<tr>";
		echo "<td colspan=\"2\">";
		$sql = "SELECT '<center><img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirValorMeta(\''||vlmid||'\');\"></center>' as acoes, dpedsc, vlmvalor FROM monitora.valormeta vi INNER JOIN painel.detalheperiodicidade dp ON dp.dpeid=vi.dpeid WHERE metid='".$dados['metid']."' AND vlmstatus='A' order by dp.dpeid";
		$cabecalho = array("&nbsp;", "Período", "Valor");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		echo "</td>";
		echo "</tr>";
	}
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" colspan=\"2\"><input type=button name=salvar value=Salvar onclick=validarMetas();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function gerenciarIniciativas($dados) {
	global $db;
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	echo "<script>";
	
	echo "function validarIniciativas(){
		  if(document.getElementById('ictcod').value=='') {alert('Preencha o código');return false;}
		  if(document.getElementById('ictdsc').value=='') {alert('Selecione a descrição');return false;}
		  divCarregando();
		  document.getElementById('form_iniciativas').submit();
		  }";
	
	echo "</script>";
	
	echo "<form method=post id=form_iniciativas>";
	echo "<input type=hidden name=objid value=".$dados['objid'].">";
	
	if($dados['ictid']) {
		echo "<input type=hidden name=requisicao value=atualizarIniciativas>";
		echo "<input type=hidden name=idpid value=".$dados['ictid'].">";
		
		$dadosiniciativa = $db->pegaLinha("SELECT * FROM monitora.iniciativa idp WHERE ictid='".$dados['ictid']."'");
		extract($dadosiniciativa);
		
	} else {
		echo "<input type=hidden name=requisicao value=inserirIniciativas>";
	}
	
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir iniciativa</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Código da iniciativa:</td>";
	echo "<td>".campo_texto("ictcod", "S", "S", "Código da iniciativa", 5, 4, "", "", '', '', 0,'id="ictcod"','',trim($ictcod))."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Descrição da iniciativa:</td>";
	echo "<td>".campo_textarea( 'ictdsc', 'S', 'S', '', '50', '4', '2500', '', '', '', '', '', trim($ictdsc))."</td>";
	echo "</tr>";
	
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" colspan=\"2\"><input type=button name=salvar value=Salvar onclick=validarIniciativas();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
	
}


function gerenciarObjetivos($dados) {
	global $db;
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	echo "<script>";
	
	echo "function validarObjetivos(){
		  if(document.getElementById('objcod').value=='') {alert('Preencha o código');return false;}
		  if(document.getElementById('objdsc').value=='') {alert('Selecione a descrição');return false;}
		  divCarregando();
		  document.getElementById('form_objetivos').submit();
		  }";
	
	echo "</script>";
	
	echo "<form method=post id=form_objetivos>";
	echo "<input type=hidden name=prgcod value=".$dados['prgcod'].">";
	
	if($dados['objid']) {
		
		echo "<input type=hidden name=requisicao value=atualizarObjetivos>";
		echo "<input type=hidden name=idpid value=".$dados['objid'].">";
		if($dados['atualizartodapaginapai']) echo "<input type=hidden name=atualizartodapaginapai value=TRUE>";
		
		$dadosobjetivo = $db->pegaLinha("SELECT * FROM monitora.objetivo WHERE objid='".$dados['objid']."'");
		extract($dadosobjetivo);
		
	} else {
		echo "<input type=hidden name=requisicao value=inserirObjetivos>";
	}
	
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir objetivo</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Código do objetivo:</td>";
	echo "<td>".campo_texto("objcod", "S", "S", "Código do objetivo", 5, 4, "", "", '', '', 0,'id="objcod"','',trim($objcod))."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Descrição do objetivo:</td>";
	echo "<td>".campo_textarea( 'objdsc', 'S', 'S', '', '50', '4', '2500', '', '', '', '', '', trim($objdsc))."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Caracterização do objetivo:</td>";
	echo "<td>".campo_textarea( 'objcaracterizacao', 'S', 'S', '', '50', '4', '2500', '', '', '', '', '', trim($objcaracterizacao))."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" colspan=\"2\"><input type=button name=salvar value=Salvar onclick=validarObjetivos();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
	
}

function gerenciarIndicadores($dados) {
	global $db;
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	echo "<script>";
	
	echo "function validarIndicadores(){
		  if(document.getElementById('idpdsc').value=='') {alert('Preencha a descrição');return false;}
		  if(document.getElementById('umiid').value=='') {alert('Selecione a unidade de medida');return false;}
		  if(document.getElementById('perid').value=='') {alert('Selecione a periodicidade');return false;}
		  divCarregando();
		  document.getElementById('form_indicadores').submit();
		  }";
	
	echo "function validarValorIndicadores(){
		  if(document.getElementById('vlivalor').value=='') {alert('Preencha a descrição');return false;}
		  if(document.getElementById('dpeid').value=='') {alert('Selecione a unidade de medida');return false;}
		  divCarregando();
		  document.getElementById('requisicao').value='inserirValorIndicadores';
		  document.getElementById('form_indicadores').submit();
		  }";
	
	echo "function excluirValorIndicadores(vliid) {
		  var conf = confirm('Deseja confirmar a exclusão deste valor');
		  if(conf) {
		  window.location='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=excluirValorIndicadores&idpid=".$dados['idpid']."&vliid='+vliid;
		  }
		  }";
	
	
	echo "</script>";
	
	$arAba = array(0 => array("id" => 1, "descricao" => "Indicadores", 	"link" => "/monitora/monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarIndicadores&atualizartodapaginapai=true&prgcod={$_REQUEST['prgcod']}&idpid={$_REQUEST['idpid']}"),
				   1 => array("id" => 2, "descricao" => "Anexos",    	"link" => "/monitora/monitora.php?modulo=principal/anexoIndicador&acao=A&requisicao=gerenciarIndicadores&atualizartodapaginapai=true&prgcod={$_REQUEST['prgcod']}&idpid={$_REQUEST['idpid']}") 				   
			  	  );

	echo montarAbasArray($arAba, "/monitora/monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarIndicadores&atualizartodapaginapai=true&prgcod={$_REQUEST['prgcod']}&idpid={$_REQUEST['idpid']}");		
	
	echo "<form method=post id=form_indicadores>";
	echo "<input type=hidden name=prgcod value=".$dados['prgcod'].">";
	
	if($dados['idpid']) {
		echo "<input type=hidden name=requisicao value=atualizarIndicadores id=requisicao>";
		echo "<input type=hidden name=idpid value=".$dados['idpid'].">";
		if($dados['atualizartodapaginapai']) echo "<input type=hidden name=atualizartodapaginapai value=TRUE>";
		
		$dadosindicador = $db->pegaLinha("SELECT * FROM monitora.indicadorprograma idp WHERE idpid='".$dados['idpid']."'");
		extract($dadosindicador);
		
	} else {
		echo "<input type=hidden name=requisicao value=inserirIndicadores id=requisicao>";
	}
	
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir indicador</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Descrição do indicador:</td>";
	echo "<td>".campo_textarea( 'idpdsc', 'S', 'S', '', '50', '4', '2500', '', '', '', '', '', trim($idpdsc))."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Unidade de medida:</td>";
	
	$sql = "SELECT umiid as codigo, umidsc as descricao 
			FROM monitora.unidademedidaindicador 
			WHERE umistatus='A'";
	
	echo "<td>".$db->monta_combo('umiid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'umiid', true, $umiid)."</td>";
	
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Periodicidade:</td>";
	
	$sql = "SELECT perid as codigo, perdsc as descricao 
			FROM painel.periodicidade 
			WHERE perstatus='A'";
	
	echo "<td>".$db->monta_combo('perid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'perid', true, $perid)."</td>";
	
	echo "</tr>";
	
	
	
	
	if($dados['idpid']) {
		$sql = "SELECT dpeid as codigo, dpedsc as descricao FROM painel.detalheperiodicidade WHERE dpestatus='A' AND perid='".$perid."' order by dpedatainicio";
		echo "<tr><td colspan=\"2\" class=SubTituloEsquerda>Valor : <input type=\"text\" class=\"normal\" name=\"vlivalor\" id=\"vlivalor\" size=\"15\" onkeyup=\"this.value=mascaraglobal('###.###.###,##',this.value);\"> ".$db->monta_combo('dpeid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'dpeid', true, $perid)." <input type=button name=adicionar value=Adicionar onclick=\"validarValorIndicadores();\"></td></tr>";
		echo "<tr>";
		echo "<td colspan=\"2\">";
		$sql = "SELECT '<center><img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirValorIndicadores(\''||vliid||'\');\"></center>' as acoes, dpedsc, vlivalor FROM monitora.valorindicador vi INNER JOIN painel.detalheperiodicidade dp ON dp.dpeid=vi.dpeid WHERE idpid='".$dados['idpid']."' AND vlistatus='A' order by dp.dpeid";
		$cabecalho = array("&nbsp;", "Período", "Valor(R$)");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		echo "</td>";
		echo "</tr>";
	}
	
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" colspan=\"2\"><input type=button name=salvar value=Salvar onclick=validarIndicadores();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
	
}

function carregarMetasIniciativas($dados) {
	global $db;
	echo "<b>Lista de Metas</b><br>";
	$sql = "SELECT '<center><img src=\"../imagens/alterar.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"gerenciarMetas(\''||o.objid||'\',\''||m.metid||'\');\"> <img src=\"../imagens/excluir.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirMetas(\''||m.metid||'\',this)\"></center>' as mais, metdsc as descricao from monitora.meta m 
			INNER JOIN monitora.objetivometa o ON o.metid = m.metid 
			WHERE o.objid='".$dados['objid']."' AND m.metstatus='A' ORDER BY metdsc";
	$cabecalho = array("&nbsp;", "Metas");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	echo "<b>Lista de Iniciativas</b><br>";
	$sql = "SELECT '<center><img src=\"../imagens/mais.gif\" id=\"iniciativa_'||i.ictid||'\" title=\"mais\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"carregarAcoes(\''||i.ictid||'\',this);\"></center>' as mais, '<div style=\"white-space:nowrap;\"><img src=\"../imagens/gif_inclui.gif\" align=\"absmiddle\" title=\"Inserir Ações\" style=\"cursor:pointer;\" onclick=\"gerenciarAcoes(\''||i.ictid||'\',\'\');\"> <img src=\"../imagens/alterar.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"gerenciarIniciativas(\''||o.objid||'\',\''||i.ictid||'\');\"> <img src=\"../imagens/excluir.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirIniciativas(\''||i.ictid||'\',this)\"></div>' as acoes, ictdsc as descricao from monitora.iniciativa i 
			INNER JOIN monitora.objetivoiniciativa o ON o.ictid = i.ictid 
			WHERE o.objid='".$dados['objid']."' AND ictstatus='A' ORDER BY ictdsc";
	$cabecalho = array("&nbsp;","&nbsp;", "Iniciativas");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}


function carregarIndicadoresObjetivos($dados) {
	global $db;
	echo "<b>Lista de Indicadores</b><br>";
	$sql = "SELECT '<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"gerenciarIndicadores(\''||prgcod||'\',\''||idpid||'\');\"> <img src=\"../imagens/excluir.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirIndicadores(\''||idpid||'\',this)\"></center>' as mais, idpdsc as descricao from monitora.indicadorprograma WHERE prgano='".$_SESSION['exercicio']."' AND prgcod='".$dados['prgcod']."' AND idpstatus='A' ORDER BY idpdsc";
	$cabecalho = array("&nbsp;", "Indicadores");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	echo "<b>Lista de Objetivos</b><br>";
	$inserirIniciativas = "<img src=\"../imagens/gif_inclui.gif\" align=\"absmiddle\" title=\"Inserir Iniciativas\" style=\"cursor:pointer;\" onclick=\"gerenciarIniciativas(\''||objid||'\',\'\');\">";
	$arrDesabilitaIniciativa = Array(PERFIL_MONIT_AVALIADOR_INICIATIVA,PERFIL_MONIT_AVALIADOR_OBJETIVO,PERFIL_MONIT_AVALIADOR_META);
	if( possui_perfil2($arrDesabilitaIniciativa) ){ $inserirIniciativas = ''; }
	$sql = "SELECT '<center><img src=\"../imagens/mais.gif\" id=\"objetivo_'||objid||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarMetasIniciativas(\''||objid||'\',this);\"></center>' as mais, '<div style=\"white-space:nowrap;\"><img src=\"../imagens/gif_inclui.gif\" title=\"Inserir Metas\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"gerenciarMetas(\''||objid||'\',\'\');\"> $inserirIniciativas <img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"gerenciarObjetivos(\''||prgcod||'\',\''||objid||'\');\"> <img src=\"../imagens/excluir.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirObjetivos(\''||objid||'\',this)\"></div>' as inserir, objdsc as descricao from monitora.objetivo WHERE prgano='".$_SESSION['exercicio']."' AND prgcod='".$dados['prgcod']."' AND objstatus='A' ORDER BY objdsc";
	$cabecalho = array("&nbsp;", "&nbsp;", "Objetivos");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function carregarProgramas($dados) {
	global $db;
	$sql = "SELECT '<center><img src=\"../imagens/mais.gif\" id=\"programa_'||p.prgcod||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarIndicadoresObjetivos(\''||p.prgcod||'\',this);\"></center>' as mais, '<center><img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Inserir Indicadores\" onclick=\"gerenciarIndicadores(\''||p.prgcod||'\',\'\');\"> <img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" title=\"Inserir Objetivos\" align=\"absmiddle\" onclick=\"gerenciarObjetivos(\''||p.prgcod||'\',\'\');\"></center>' as inserir, p.prgcod || ' - ' || p.prgdsc as descricao from monitora.programa p inner join monitora.acao a ON a.prgid = p.prgid WHERE p.prgano='".$_SESSION['exercicio']."' and acasnrap = false group by p.prgcod, p.prgdsc ORDER BY p.prgcod, prgdsc";
	$cabecalho = array("&nbsp;", "&nbsp;", "Programas");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function carregarAcoes($dados) {
	global $db;
	echo "<b>Lista de Ações</b><br>";
	$sql = "SELECT '<center><img src=\"../imagens/excluir.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirAcoes(\''||icaid||'\',this)\"></center>' as mais, acacod||' - '||acadsc as descricao from monitora.iniciativaacao WHERE ictid='".$dados['ictid']."' AND icastatus='A' ORDER BY acadsc";
	$cabecalho = array("&nbsp;", "Ações");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
}

function excluirAnaliseSituacionalObjetivo_II($dados){
	global $db;

	$sql = "UPDATE monitora.analiseobjetivo
			SET anostatus='I'
			WHERE
				usucpfcadastro = '" . $_SESSION['usucpf'] . "' AND 
				anoid = {$dados['anoid']};";
	
	$db->executar( $sql );
	$db->commit();
	
	die("<script>
			alert('Operação realizada com sucesso!');
			location.href = '?modulo=principal/telaprograma&acao=A&requisicao=listarAnaliseSituacionalObjetivo&prgcod={$dados['prgcod']}&objid={$dados['objid']}';
	     </script>");
}

function listarAnaliseSituacionalObjetivo($dados){
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	$objid  = $dados['objid'];
	$prgcod = $dados['prgcod'];
	
	$sql = "SELECT
				COUNT(*)
			FROM
				monitora.analiseobjetivo
			WHERE
				anostatus = 'A' AND
				objid = {$objid} AND
				usucpfcadastro = '" . $_SESSION['usucpf'] . "'";
	
	$existAnaliseUser = $db->pegaUm( $sql );
?>		
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript">
	
	function alterarAnalise(anoid){
		window.location = '?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalObjetivo&prgcod=<?php echo $prgcod ?>&objid=<?php echo $objid ?>&anoid=' + anoid ;
	}
	
	function excluirAnalise(anoid){
		if (confirm('Deseja apagar a sua análise?')){
			// Só vai conseguir excluir se tiver sido o próprio usuário que cadastrou
			window.location = '?modulo=principal/telaprograma&acao=A&requisicao=excluirAnaliseSituacionalObjetivo_II&prgcod=<?php echo $prgcod ?>&objid=<?php echo $objid ?>&anoid=' + anoid;
		}
	}
	
	</script>
<?php 

	if( $db->testa_superuser() ){
		$acao = "
			'<div style=\"white-space:nowrap;\">
				<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: alterarAnalise(\'' || ao.anoid || '\');\" title=\"Alterar Análise Situacional\">
				<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"javascript: excluirAnalise(\'' || ao.anoid || '\');\" title=\"Excluir Análise Situacional\">
			</div>' as acao,
		";
	}elseif( ( in_array(PERFIL_MONIT_AVALIADOR_OBJETIVO, $perfis) || 
			   in_array(PERFIL_MONIT_AVALIADOR_INICIATIVA, $perfis) || 
			   in_array(PERFIL_MONIT_AVALIADOR_META, $perfis) || 
			   in_array(PERFIL_MONIT_CPMO, $perfis) || 
			   in_array(PERFIL_MONIT_CONSULTA, $perfis) ) ){
		$acao = "
			'<div style=\"white-space:nowrap;\">
				<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: alterarAnalise(\'' || ao.anoid || '\');\" title=\"Alterar Análise Situacional\">
				<img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Excluir Análise Situacional\">
			</div>' as acao,
		";
	}else{
		$acao = "
			'<div style=\"white-space:nowrap;\">
				<img align=\"absmiddle\" src=\"/imagens/alterar_01.gif\" title=\"Alterar Análise Situacional\">
				<img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Excluir Análise Situacional\">
			</div>' as acao,
		";
	}

	$sql = "SELECT 	$acao
					ao.anodsc, 
					r.refperiodo,
					TO_CHAR(ao.anodtinclusao, 'DD/MM/YYYY') AS data,
					u.usunome
			FROM monitora.analiseobjetivo ao
			JOIN seguranca.usuario u ON u.usucpf = ao.usucpfcadastro				
			JOIN monitora.referencia r ON r.refcod = ao.refcod				
			WHERE ao.anostatus = 'A' AND ao.objid = {$dados['objid']}";
?>
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Análise Situacional</td>
		</tr>
		<tr>
			<td>
			<?php
				$cabecalho = array("Ação", "Análise", "Período de Referência", "Data de Inclusão", "Cadastrante");
				$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
			?>
			</td>
		</tr>
		<?php if ( $existAnaliseUser < 4 ){ ?>
		<tr bgcolor="#CCCCCC">
			<td>
				<?php
				if( possui_perfil2( Array(PERFIL_MONIT_AVALIADOR_OBJETIVO) ) ){?>
					<input type="button" name="btnovaanalise" value="Nova Análise" onclick="location.href='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalObjetivo&prgcod=<?php echo $prgcod ?>&objid=<?php echo $objid ?>';" class="botao">
				<?php }else{?>
					<input type="button" name="btnovaanalise" value="Nova Análise" class="botao" disabled="disabled">
				<?php }?>
		   	</td>
		</tr>
		<?php 
		}
		?> 	
	</table>
<?php	
}

function testaAvaliadorObjetivo( $objid ){

	global $db;

	if( possui_perfil2( Array(PERFIL_MONIT_AVALIADOR_OBJETIVO) ) ){

		$sql = "SELECT
					true
				FROM
					monitora.usuarioresponsabilidade
				WHERE
					objid = $objid AND
					rpustatus = 'A' AND
					usucpf = '{$_SESSION['usucpf']}'";
		
		$teste = $db->pegaUm($sql);

		return ($teste==''?false:true);
	}else{
		return false;
	}
}

function gerenciarAnaliseSituacionalObjetivo($dados){
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	$habilita = 'N';
	$habilita_Work = 'N';
	
	if ( $dados['anoid'] ){
		$sql = "SELECT 
					anoid, TRIM(anodsc) AS anodsc , anosituacao, refcod, unmcod 
				FROM
					monitora.analiseobjetivo
				WHERE
					anostatus = 'A' AND
					anoid = {$dados['anoid']} AND 
					objid = {$dados['objid']}";
		
		$registros = $db->pegaLinha( $sql );
		$anoid  = $registros['anoid'];
		$anodsc = $registros['anodsc'];
		$anosituacao = $registros['anosituacao'];
		$refcod = $registros['refcod'];
		$unmcod = $registros['unmcod'];
		
		$docid = pegaDocidAnaliseObjetivo( $dados['anoid'] );
	}		
	
	$esdid = pegarEstadoDocumento( $docid );
		
	if( $esdid == '' || $esdid == WK_OBJ_EM_CADASTRAMENTO ){
		if( ( possui_perfil2( Array( PERFIL_MONIT_AVALIADOR_OBJETIVO ) ) && testaAvaliadorObjetivo( $dados['objid'] ) ) ||
			possui_perfil2( Array( PERFIL_MONIT_CPMO ) ) ){
			$habilita = 'S';
			$habilita_Work = 'S';
		}
	}
?>	

	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

	<script type="text/javascript">
	<!--
	function excluirAnexoAnalise( oanid ){
		if ( confirm('Deseja apagar o anexo?') ){
			location.href = '?modulo=principal/telaprograma&acao=A&requisicao=excluirAnaliseSituacionalObjetivo&oanid=' + oanid + '&anoid=<?=$dados['anoid']?>&objid=<?=$dados['objid']?>';
		}
	}
	
	function validaAnalise(){
		if ( $.trim( $('[name=anodsc]').val() ) == '' ){
			alert('O campo análise é obrigatório!');
			return false;
		}

		if ( $.trim( $('[name=refcod]').val() ) == '' ){
			alert('O campo mês de referência é obrigatório!');
			return false;
		}
		
		return true;
	}
	
	function validaAnexoAnalise(){
		if ( $.trim( $('[name=oandescricao]').val() ) == '' ){
			alert('O campo assunto é obrigatório!');
			return false;
		}
		
		if ( $.trim( $('[name=objanexo]').val() ) == '' ){
			alert('O campo anexo é obrigatório!');
			return false;
		}
		return true;
	}
	//-->
	</script>

	<form name="formulario" action="?modulo=principal/telaprograma&acao=A&requisicao=inserirAnaliseSituacionalObjetivo&objid=<?=$dados['objid'] ?>" method="post" onsubmit="return validaAnalise();">
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="3">Análise Situacional</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Análise:</b></td>
			<td>
			<? echo campo_textarea( 'anodsc', 'S', $habilita, '', '70', '4', '5000', '', 0, '', false, null, $anodsc); ?>
			</td>
			<td rowspan="3">
			<?php 
			if ( $docid && $habilita_Work == 'S'){
				wf_desenhaBarraNavegacao( $docid , array('descricao'=>'Objetivo','codigo'=>$anoid),  array('historico'=>$oculta) );
			}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Período de Referência:</b></td>
			<td>
			<? 
			$sql = "SELECT
						refcod
					FROM
						monitora.analiseobjetivo
					WHERE
						anostatus = 'A' AND
						objid = {$dados['objid']} AND
						usucpfcadastro = '" . $_SESSION['usucpf'] . "'" . 
						( $refcod ? " AND refcod NOT IN ($refcod) " : "" );
			$arRefcod = $db->carregarColuna( $sql );
			
			$sql = "SELECT 
						refcod AS CODIGO,
						refperiodo AS DESCRICAO 
					FROM 
						monitora.referencia 
					WHERE 
						refdata_limite_avaliacao_aca IS NOT NULL AND 
						refsnmonitoramento='t' AND 
						refano_ref='{$_SESSION['exercicio']}' 
						" . ( $arRefcod ? " AND refcod NOT IN(" . implode(", ", $arRefcod) . ")" : "" ) . "
					ORDER BY refdsc ";
			$db->monta_combo('refcod', $sql, $habilita, 'Selecione', '', '', '', '200', 'S', 'refcod', false, $refcod);
			?>
			</td>
		</tr>
		
		<!--  
		<tr>
			<td class="SubTituloDireita" width="40%">Unidade de Medida:</td>
				<?php 
//					$sql = "SELECT unmcod as codigo, unmdsc as descricao 
//							FROM public.unidademedida 
//							WHERE unmstatus='A'
//							ORDER BY unmdsc";
				?>	
			<td>
				<?php 
//				echo $db->monta_combo('unmcod', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'unmcod', true, $unmcod);
				?>
			</td>
		</tr>
		-->
		
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloEsquerda">
			<input type="hidden" name = "anoid" value = "<?= $anoid ?>">
			<input type="hidden" name = "anosituacao" value = "<?= $anosituacao ?>">
			<?php if($habilita == 'S'){?>
				<input type="submit" name="btl_salvar_analise" value="Salvar">
			<?php }else{?>
				<input type="button" value="Salvar" disabled="disabled">
			<?php }?>
			</td>
		</tr>
	</table>
	</form>
	
	<form name="anexoobjetivo" action="?modulo=principal/telaprograma&acao=A&requisicao=inserirAnaliseSituacionalObjetivo&objid=<?=$dados['objid'] ?>&anoid=<?=$dados['anoid'] ?>" method="post" enctype="multipart/form-data" onsubmit="return validaAnexoAnalise();">
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Anexo da Análise Situacional</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Assunto:</b></td>
			<td>
			<? echo campo_textarea( 'oandescricao', 'S', $habilita, '', '70', '2', '255'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Anexo:</b></td>
			<td>
			<input name="objanexo" type="file">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloEsquerda">
			<?php if($habilita == 'S'){?>
				<input type="submit" name="btl_salvar_anexo" value="Salvar">
			<?php }else{?>
				<input type="button" value="Salvar" disabled="disabled">
			<?php }?>
			<input type="button" name="btl_voltar" value="Voltar" onclick="location.href='?modulo=principal/telaprograma&acao=A&requisicao=listarAnaliseSituacionalObjetivo&prgcod=<?php echo $dados['prgcod']; ?>&objid=<?php echo $dados['objid']; ?>';">
			</td>
		</tr>
		<?php 
		if ( $dados['anoid'] ){
		?>
		<tr>
			<td colspan="2">
			<fieldset>
				<legend>Lista de Anexos</legend>
				<div style="overflow:auto; height: 140px;">
			<?php
			if($habilita == 'S'){
				$acao = "
					'<div style=\"white-space:nowrap;\">
						<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirAnexoAnalise(' || oa.oanid || ');\">
					</div>' as acao,
				";
			}else{
				$acao = "
					'<div style=\"white-space:nowrap;\">
						<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer\" onclick=\"excluirAnexoAnalise(' || oa.oanid || ');\">
					</div>' as acao,
				";
			}
			
			$sql = "SELECT $acao
						   oa.oandescricao as assunto, 
						   '<a href=\"?modulo=principal/telaprograma&acao=A&requisicao=downloadAnaliseSituacionalObjetivo&arqid=' || a.arqid || '\">' || a.arqnome || '.' || a.arqextensao || '</a>' as arquivo, 
						   u.usunome as nome 
					FROM 
						monitora.objetivoanexo oa
			 		INNER JOIN public.arquivo a on a.arqid = oa.arqid 
			 		INNER JOIN seguranca.usuario u on u.usucpf = a.usucpf
					WHERE 
						oa.oanstatus = 'A' AND
						oa.objid IN ({$dados['objid']})";
			$cabecalho = array("&nbsp;", "<b>Assunto</b>", "<b>Arquivo</b>", "<b>Inserido por</b>");
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%','center');
			?>
				</div>
			</fieldset>
			</td>
		</tr>
		<?php 
		}
		?>
	</table>
	</form>
<?	
}

function gerenciarAnaliseGlobalObjetivo($dados){
	//começa aqui.
	global $db;
	
	$sql = "select 
				objanaliseglobal 
			from
				monitora.objetivo
			where
				objid = {$dados['objid']}";
	$objanaliseglobal = $db->pegaUm($sql);
	
	monta_titulo("Análise Global","&nbsp;");
	
	$perfis = pegaPerfilGeral();
	
	$habilita = 'S';
	if( in_array(PERFIL_MONIT_AVALIADOR_OBJETIVO, $perfis) || in_array(PERFIL_MONIT_AVALIADOR_INICIATIVA, $perfis) || in_array(PERFIL_MONIT_AVALIADOR_META, $perfis) || in_array(PERFIL_MONIT_CONSULTA, $perfis) || in_array(PERFIL_MONIT_COORDENADOR_ACAO, $perfis) || in_array(PERFIL_MONIT_VALIDADOR, $perfis) ){
		$habilita = 'N';
	}
	
?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<script>
		function salvarAnalise()
		{
			if($("[name='objanaliseglobal']").val()){
				$("[name='formulario']").submit();
			}else{
				alert('Informe a análise.');
			}
		}
	</script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<form name="formulario" id="formulario" action="?modulo=principal/telaprograma&acao=A&requisicao=salvarAnaliseGlobalObjetivo&prgcod=<?=$dados['prgcod'] ?>&objid=<?=$dados['objid'] ?>" method="post" >
		<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" >Análise Global</td>
				<td>
					<? echo campo_textarea( 'objanaliseglobal', 'S', $habilita, '', '70', '15', '5000', '', 0, '', false, null, $objanaliseglobal); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="2">
					<?php if($habilita == 'S'){?>
					<input type="button" name="btn_salvar" value="Salvar" onclick="salvarAnalise()" />
					<input type="button" name="btn_cancelar" value="Fechar" onclick="window.close()" />
					<?php }else{?>
					<input type="button" name="btn_salvar" value="Salvar" disabled="disabled"/>
					<input type="button" name="btn_cancelar" value="Fechar" disabled="disabled"/>
					<?php }?>
				</td>
			</tr>
		</table>
	</form>
	<?php
}

function salvarAnaliseGlobalObjetivo($dados)
{
	global $db;
	
	$sql = "update
				monitora.objetivo
			set
				objanaliseglobal = '{$dados['objanaliseglobal']}'
			where
				objid = {$dados['objid']}";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Operação realizada com sucesso.');window.location='?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseGlobalObjetivo&prgcod={$dados['prgcod']}&objid={$dados['objid']}';</script>";
}

function gerenciarAnaliseGlobalMeta($dados)
{
	global $db;
	
	$sql = "select 
				metanaliseglobal  
			from
				monitora.meta
			where
				metid = {$dados['metid']}";
	$metanaliseglobal = $db->pegaUm($sql);
	
	monta_titulo("Análise Global","&nbsp;");
	
	$perfis = pegaPerfilGeral();
	
	$habilita = 'S';
	if( in_array(PERFIL_MONIT_AVALIADOR_OBJETIVO, $perfis) || in_array(PERFIL_MONIT_AVALIADOR_META, $perfis) || in_array(PERFIL_MONIT_CONSULTA, $perfis) || in_array(PERFIL_MONIT_COORDENADOR_ACAO, $perfis) || in_array(PERFIL_MONIT_VALIDADOR, $perfis) || in_array(PERFIL_MONIT_AVALIADOR_INICIATIVA, $perfis) ){
		$habilita = 'N';
	}
	
	
	?>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	
	<script type="text/javascript">
		function salvarAnalise(){
			if($("[name='metanaliseglobal']").val()){
				$("[name='formulario']").submit();
			}else{
				alert('Informe a análise.');
			}
		}
	</script>
	
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<form name="formulario" id="formulario" action="?modulo=principal/telaprograma&acao=A&requisicao=salvarAnaliseGlobalMeta&metid=<?=$dados['metid'] ?>" method="post" >
		<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" >Análise Global</td>
				<td>
					<? echo campo_textarea( 'metanaliseglobal', 'S', $habilita, '', '70', '15', '5000', '', 0, '', false, null, $metanaliseglobal); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="2">
					<?php if( $db->testa_superuser() ){ ?>
						<input type="button" name="btn_salvar" value="Salvar" onclick="salvarAnalise()" />
						<input type="button" name="btn_cancelar" value="Fechar" onclick="window.close()" />
					<?php }elseif( $habilita == 'N' ){?>
						<input type="button" name="btn_salvar" value="Salvar" disabled="disabled"/>
						<input type="button" name="btn_cancelar" value="Fechar" onclick="window.close()" />
					<?php }else{?>
						<input type="button" name="btn_salvar" value="Salvar" onclick="salvarAnalise()" />
						<input type="button" name="btn_cancelar" value="Fechar" onclick="window.close()" />					
					<?php }?>
				</td>
			</tr>
		</table>
	</form>
	<?php
}

function salvarAnaliseGlobalMeta($dados)
{
	global $db;
	
	$sql = "update
				monitora.meta
			set
				metanaliseglobal = '{$dados['metanaliseglobal']}'
			where
				metid = {$dados['metid']}";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Operação realizada com sucesso.');window.location='?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseGlobalMeta&metid={$dados['metid']}';</script>";
}

function gerenciarAnaliseGlobalIniciativa($dados)
{
	global $db;
	
	$habil = 'N';
	
	$arrEdita = Array( PERFIL_MONIT_CPMO );
	
	if( possui_perfil2( $arrEdita ) || $db->testa_superuser() ){
		$habil = 'S';
	}
	
	$sql = "select 
				ictanaliseglobal   
			from
				monitora.iniciativa
			where
				ictid = {$dados['ictid']}";
	$ictanaliseglobal = $db->pegaUm($sql);
	
	monta_titulo("Análise Global","&nbsp;")
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<script>
		function salvarAnalise()
		{
			if($("[name='ictanaliseglobal']").val()){
				$("[name='formulario']").submit();
			}else{
				alert('Informe a análise.');
			}
		}
	</script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<form name="formulario" id="formulario" action="?modulo=principal/telaprograma&acao=A&requisicao=salvarAnaliseGlobalIniciativa&ictid=<?=$dados['ictid'] ?>" method="post" >
		<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" >Análise Global</td>
				<td>
				<?php
				include_once APPRAIZ ."includes/workflow.php";
				$docid = pegaDocidAvaliacao_analiseiniciativa_ictid( $dados['ictid'] );
				$arrEstado = wf_pegarEstadoAtual($docid);
				if( possui_perfil2( $arrEdita ) || $db->testa_superuser() ){
					if(WK_FINALIZADO_ANALISE == $arrEstado['esdid']){
						$habil = "N";
					}else{
						$habil = "S";
					}
				}
				
				echo campo_textarea( 'ictanaliseglobal', 'S', $habil, '', '70', '4', '5000', '', 0, '', false, null, $ictanaliseglobal); 
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="2">
					<?php if( possui_perfil2( $arrEdita ) || $db->testa_superuser() ){?>
						<?php if($habil == "N"){ ?>
							<input type="button" name="btn_salvar" value="Salvar" onclick="salvarAnalise()" disabled="disabled"/>
						<?php }else{ ?>
							<input type="button" name="btn_salvar" value="Salvar" onclick="salvarAnalise()" />
						<?php } ?>
					<input type="button" name="btn_cancelar" value="Fechar" onclick="window.close()" />
					<?php }?>
				</td>
			</tr>
		</table>
	</form>
	<?php
}

function salvarAnaliseGlobalIniciativa($dados)
{
	global $db;
	
	$sql = "update
				monitora.iniciativa
			set
				ictanaliseglobal = '{$dados['ictanaliseglobal']}'
			where
				ictid = {$dados['ictid']}";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Operação realizada com sucesso.');window.location='?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseGlobalIniciativa&ictid={$dados['ictid']}';</script>";
}

function excluirAnaliseSituacionalIniciativa_II($dados){
	global $db;

	$sql = "UPDATE monitora.analiseiniciativa
			SET anistatus='I'
			WHERE
				usucpfcadastro = '" . $_SESSION['usucpf'] . "' AND 
				aniid = {$dados['aniid']};";
	
	$db->executar( $sql );
	$db->commit();
	
	die("<script>
			alert('Operação realizada com sucesso!');
			location.href = '?modulo=principal/telaprograma&acao=A&requisicao=listarAnaliseSituacionalIniciativa&objid={$dados['objid']}&ictid={$dados['ictid']}';
	     </script>");
}


function listarAnaliseSituacionalIniciativa($dados){
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	$objid  = $dados['objid'];
	$ictid = $dados['ictid'];
	
	$sql = "SELECT
				COUNT(*)
			FROM
				monitora.analiseiniciativa ai
			WHERE
				ai.anistatus = 'A' AND
				ai.ictid = {$ictid} AND
				ai.usucpfcadastro = '" . $_SESSION['usucpf'] . "'";
	//dbg($sql);
	$existAnaliseUser = $db->pegaUm( $sql );
?>		
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript">
	
	function alterarAnalise(aniid){
		window.location = '?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalIniciativa&ictid=<?php echo $ictid ?>&objid=<?php echo $objid ?>&aniid=' + aniid ;
	}
	
	function excluirAnalise(aniid){
		if (confirm('Deseja apagar a sua análise?')){
			// Só vai conseguir excluir se tiver sido o próprio usuário que cadastrou
			window.location = '?modulo=principal/telaprograma&acao=A&requisicao=excluirAnaliseSituacionalIniciativa_II&ictid=<?php echo $ictid ?>&objid=<?php echo $objid ?>&aniid=' + aniid;
		}
	}
	
	</script>
<?php
	if( $db->testa_superuser() ){
		$acao = "
			'<div style=\"white-space:nowrap;\">
				<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: alterarAnalise(\'' || ai.aniid || '\');\" title=\"Alterar Análise Situacional\">
				<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"javascript: excluirAnalise(\'' || ai.aniid || '\');\" title=\"Excluir Análise Situacional\">
			</div>' as acao,
		";
	}elseif( ( possuiRespIniciativa( $ictid )  && in_array(PERFIL_MONIT_AVALIADOR_META, $perfis) ) || 
			 in_array(PERFIL_MONIT_CPMO, $perfis) ){
		$acao = "
		'<div style=\"white-space:nowrap;\">
			<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: alterarAnalise(\'' || ai.aniid || '\');\" title=\"Alterar Análise Situacional\">
			<img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Excluir Análise Situacional\">
		</div>' as acao,
		";
	}elseif( in_array(PERFIL_MONIT_CONSULTA, $perfis) ){
		$acao = "
			'<div style=\"white-space:nowrap;\">
				<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: alterarAnalise(\'' || ai.aniid || '\');\" title=\"Alterar Análise Situacional\">
				<img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Excluir Análise Situacional\">
			</div>' as acao,
		";
	}else{
		$acao = "
		'<div style=\"white-space:nowrap;\">
			<img align=\"absmiddle\" src=\"/imagens/alterar_01.gif\" title=\"Alterar Análise Situacional\">
			<img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Excluir Análise Situacional\">
		</div>' as acao,
		";
	}	
	
	$sql = "SELECT 	$acao				
					ai.anidsc, 
					r.refperiodo,
					TO_CHAR(ai.anidtinclusao, 'DD/MM/YYYY') AS data,
					u.usunome
			FROM monitora.analiseiniciativa ai
			JOIN seguranca.usuario u ON u.usucpf = ai.usucpfcadastro				
			JOIN monitora.referencia r ON r.refcod = ai.refcod				
			WHERE ai.anistatus = 'A' AND ai.ictid = {$dados['ictid']}";
	
?>
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Análise Situacional</td>
		</tr>
		<tr>
			<td>
			<?php
				$cabecalho = array("Ação", "Análise", "Período de Referência", "Data de Inclusão", "Cadastrante");
				$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
			?>
			</td>
		</tr>
		<?php 
		if ( $existAnaliseUser < 2 ){
		?>
		<tr bgcolor="#CCCCCC">
			<td>
		   		<?php if( ( possuiRespIniciativa( $ictid ) && possui_perfil2( Array(PERFIL_MONIT_AVALIADOR_INICIATIVA) ) ) || 
		   				  possui_perfil2( Array(PERFIL_MONIT_CPMO) ) ){ ?>
				<input type="button" name="btnovaanalise" value="Nova Análise" onclick="location.href='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalIniciativa&ictid=<?php echo $ictid ?>&objid=<?php echo $objid ?>';" class="botao">
				<?php }else{ ?>		   	
				<input type="button" value="Nova Análise" disabled="disabled">
				<?php }?>
		   </td>
		</tr>
		<?php 
		}
		?> 	
	</table>
<?php	
}


function gerenciarAnaliseSituacionalIniciativa($dados){
	global $db;

	if ( $dados['aniid'] ){
		$sql = "SELECT 
					 aniid, TRIM(anidsc) AS anidsc, ictid, anisituacao, refcod
				FROM
					monitora.analiseiniciativa
				WHERE
					anistatus = 'A' AND
					aniid = {$dados['aniid']}";
		
		$registros =$db->pegaLinha ( $sql );
		$aniid = $registros['aniid'];
		$anidsc = $registros['anidsc'];
		$anisituacao = $registros['anisituacao'];
		$refcod = $registros['refcod'];
		
		$docid = pegaDocidAnaliseIniciativa( $aniid );
	}
	
	$sql =  "select max(acacod) as acacod, count(ictid) as qtd from monitora.iniciativaacao where ictid =" .$dados['ictid'];
	$registros = $db-> pegaLinha($sql);
	
	if ($registros['qtd'] == 1)
	{
		
		$sql = sprintf(
				"select rofmes, max(rofdatainclusao) - interval '1 day' as dataatu, sum(rofdot_ini) as rofdot_ini, sum(rofautorizado) as rofautorizado,  sum(rofempenhado) as empenhado,
				 sum(rofliquidado_favorecido) as rofliquidado_favorecido, sum(rofpago) as rofpago
				 from financeiro.execucao
				 where acacod = '%s' and rofano = '%s'
				 group by rofmes, prgcod,acacod, unicod,loccod",
				 $registros['acacod'], $_SESSION['exercicio']);

		$dadosfinanceiroscompletos = @$db->carregar( $sql );
		
		$dadofinanceiro = array();
		if($dadosfinanceiroscompletos) {
			foreach($dadosfinanceiroscompletos as $linhafin) {
				$dataatualizada = $linhafin['dataatu'];
				$dadofinanceiro['rofdot_ini'] += $linhafin['rofdot_ini'];
				$dadofinanceiro['rofautorizado'] += $linhafin['rofautorizado'];
				$dadofinanceiro['empenhado'] += $linhafin['empenhado'];
				$dadofinanceiro['rofliquidado_favorecido'] += $linhafin['rofliquidado_favorecido'];
				$dadofinanceiro['rofpago'] += $linhafin['rofpago'];
			}
		}
	}
	
	
?>	

	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

	<script type="text/javascript">
	<!--
	<?php 
	//if ( !possuiRespIniciativa( $dados['ictid'] ) && !$db->testa_superuser() ){
	?>
		//alert('Seu perfil no sistema não lhe permite cadastrar a análise situacional para esta iniciativa!');
		//window.close();		
	<?php	
	//}
	?>
	
	
	function excluirAnexoAnalise( ianid ){
		if ( confirm('Deseja apagar o anexo?') ){
			location.href = '?modulo=principal/telaprograma&acao=A&requisicao=excluirAnaliseSituacionalIniciativa&ianid=' + ianid + '&ictid=<?=$dados['ictid']?>&aniid=<?=$dados['aniid']?>';
		}
	}
	
	function validaAnalise(){
		if ( $.trim( $('[name=anidsc]').val() ) == '' ){
			alert('O campo análise é obrigatório!');
			return false;
		}



		if ( $.trim( $('[name=refcod]').val() ) == '' ){
			alert('O campo mês de referência é obrigatório!');
			return false;
		}
		
		return true;
	}
	
	function validaAnexoAnalise(){
		if ( $.trim( $('[name=iandescricao]').val() ) == '' ){
			alert('O campo assunto é obrigatório!');
			return false;
		}
		
		if ( $.trim( $('[name=objanexo]').val() ) == '' ){
			alert('O campo anexo é obrigatório!');
			return false;
		}
		return true;
	}
	//-->
	</script>

	<form name="formulario" action="?modulo=principal/telaprograma&acao=A&requisicao=inserirAnaliseSituacionalIniciativa&ictid=<?=$dados['ictid'] ?>" method="post" onsubmit="return validaAnalise();">
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

		<?php 
		
		if ($registros['qtd'] == 1)
		{
		?>
		<tr>
			<td class="SubTituloCentro" colspan="2">Dados Financeiros</td>
		</tr>
		<tr>
		<td colspan=2 align="center">
		<table class="tabela">
			<tr>
				<td class="SubTituloDireita">Dotação Inicial</td>
				<td class="SubTituloDireita">Dotação ( Lei + Créditos )</td>
				<td class="SubTituloDireita">Empenhado</td>
				<td class="SubTituloDireita">Liquidado</td>
				<td class="SubTituloDireita">Pago</td>
			</tr>
			<tr>
			<?php foreach( $dadofinanceiro as $valor ): ?>
				<td align="right">R$ <?= number_format($valor, 2, ',', '.'); ?></td>
			<?php endforeach; ?>
			</tr>
		</table>
		</td>
		</tr>
		<?
		} 
		?>
		<tr>
			<td class="SubTituloCentro" colspan="3">Análise Situacional</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%" valign="top"><b>Análise:</b></td>
			<td valign="top">
			<?php
			include_once APPRAIZ ."includes/workflow.php";
			$docid = pegaDocidAvaliacao_analiseiniciativa( $aniid );
			$arrEstado = wf_pegarEstadoAtual($docid);
			
			$readonly_tineMCE = "N";
			if( WK_OBJ_EM_CADASTRAMENTO == $arrEstado['esdid'] || $arrEstado['esdid'] == '' ){
				if( ( possuiRespIniciativa( $dados['ictid'] ) && possui_perfil2( Array( PERFIL_MONIT_AVALIADOR_INICIATIVA ) ) ) || 
					possui_perfil2( Array( PERFIL_MONIT_CPMO ) ) ){
					$readonly_tineMCE = "S";
				}
			}
			
			?>
			<? echo campo_textarea( 'anidsc', 'S', $readonly_tineMCE, '', '70', '4', '5000', '', 0, '', false, null, $anidsc); ?>
			</td>
			<td rowspan="3">
			<?php 
			if ($docid){
				wf_desenhaBarraNavegacao( $docid , array('descricao'=>'Iniciativa','codigo'=>$aniid),  array('historico'=>$oculta) );
			}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Período de Referência:</b></td>
			<td>
			<? 
			$sql = "SELECT
						refcod
					FROM
						monitora.analiseiniciativa ai
					WHERE
						ai.anistatus = 'A' AND
						ai.ictid = {$dados['ictid']} AND
						ai.usucpfcadastro = '" . $_SESSION['usucpf'] . "'" . 
						( $refcod ? " AND refcod NOT IN ($refcod) " : "" );
			$arRefcod = $db->carregarColuna( $sql );
			
			$sql = "SELECT 
						refcod AS codigo,
						refperiodo AS descricao 
					FROM 
						monitora.referencia 
					WHERE 
						refdata_limite_avaliacao_aca IS NOT NULL AND 
						refsnmonitoramento='t' AND 
						refano_ref='{$_SESSION['exercicio']}' 
						" . ( $arRefcod ? " AND refcod NOT IN(" . implode(", ", $arRefcod) . ")" : "" ) . "
					ORDER BY refdsc";
			$db->monta_combo('refcod', $sql, $readonly_tineMCE, 'Selecione', '', '', '', '200', 'S', 'refcod', false, $refcod);
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloEsquerda">
				<input type="hidden" name="aniid" value="<?= $aniid?>">
				<input type="hidden" name="anisituacao" value="<?= $anisituacao?>">
				<?php if($readonly_tineMCE == "S"){ ?>
						<input type="submit" name="btl_salvar_analise" value="Salvar">
				<?php }else{ ?>
						<input type="submit" name="btl_salvar_analise" value="Salvar" disabled="disabled">
				<?php } ?>
				<input type="button" name="btl_voltar" value="Voltar" onclick="location.href='?modulo=principal/telaprograma&acao=A&requisicao=listarAnaliseSituacionalIniciativa&objid=<?php echo $dados['objid'] ?>&ictid=<?php echo $dados['ictid'] ?>';">
			</td>
		</tr>
	</table>
	</form>
	
	<form name="anexoobjetivo" action="?modulo=principal/telaprograma&acao=A&requisicao=inserirAnaliseSituacionalIniciativa&ictid=<?=$dados['ictid'] ?>&aniid=<?=$dados['aniid'] ?>" method="post" enctype="multipart/form-data" onsubmit="return validaAnexoAnalise();">
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Anexo da Análise Situacional</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Assunto:</b></td>
			<td>
			<? echo campo_textarea( 'iandescricao', 'S', $readonly_tineMCE, '', '70', '2', '255'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Anexo:</b></td>
			<td>
			<?php if($readonly_tineMCE == "S"){ ?>
					<input name="objanexo" type="file">
		   <?php }else{ ?>
				 	<input name="objanexo" type="file" disabled="disabled">
	       <?php } ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloEsquerda">
			<?php if($readonly_tineMCE == "S"){ ?>
					<input type="submit" name="btl_salvar_anexo" value="Salvar">
			<?php }else{ ?>
					<input type="submit" name="btl_salvar_anexo" value="Salvar" disabled="disabled">
			<?php } ?>
			</td>
		</tr>
		<?php
		if ( $dados['aniid'] ){
		?>
		<tr>
			<td colspan="2">
			<fieldset>
				<legend>Lista de Anexos</legend>
				<div style="overflow:auto; height: 140px;">
			<?php
			$sql = "SELECT 
						  '<center>
								<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirAnexoAnalise(' || ia.ianid || ');\">
						   </center>' as acao, 
						   ia.iandescricao as assunto, 
						   '<a href=\"?modulo=principal/telaprograma&acao=A&requisicao=downloadAnaliseSituacionalObjetivo&arqid=' || a.arqid || '\">' || a.arqnome || '.' || a.arqextensao || '</a>' as arquivo, 
						   u.usunome as nome 
					FROM 
						 monitora.iniciativaanexo ia
			 		INNER JOIN public.arquivo a on a.arqid = ia.arqid 
			 		INNER JOIN seguranca.usuario u on u.usucpf = a.usucpf
					WHERE 
						ia.ianstatus = 'A' AND
						ia.ictid IN ({$dados['ictid']})";
			$cabecalho = array("&nbsp;", "<b>Assunto</b>", "<b>Arquivo</b>", "<b>Inserido por</b>");
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%','center');
			?>
				</div>
			</fieldset>
			</td>
		</tr>
		<?php 
		}
		?>
	</table>
	</form>
<?	
}

function excluirAnaliseSituacionalMeta_II($dados){
	global $db;

	$sql = "UPDATE monitora.analisemeta
			SET anmstatus='I'
			WHERE
				usucpfcadastro = '" . $_SESSION['usucpf'] . "' AND 
				anmid = {$dados['anmid']};";
	
	$db->executar( $sql );
	$db->commit();
	
	die("<script>
			alert('Operação realizada com sucesso!');
			location.href = '?modulo=principal/telaprograma&acao=A&requisicao=listarAnaliseSituacionalMeta&metid={$dados['metid']}&objid={$dados['objid']}';
	     </script>");
}

function listarAnaliseSituacionalMeta($dados){
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	$objid  = $dados['objid'];
	$metid = $dados['metid'];
	
	$sql = "SELECT
				COUNT(*)
			FROM
				monitora.analisemeta am
			WHERE
				am.anmstatus = 'A' AND
				am.metid = {$metid} AND
				am.usucpfcadastro = '" . $_SESSION['usucpf'] . "'";
	
	$existAnaliseUser = $db->pegaUm( $sql );
?>		
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript">
	
	function alterarAnalise(anmid){
		window.location = '?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalMeta&metid=<?php echo $metid ?>&objid=<?php echo $objid ?>&anmid=' + anmid ;
	}
	
	function excluirAnalise(anmid){
		if (confirm('Deseja apagar a sua análise?')){
			// Só vai conseguir excluir se tiver sido o próprio usuário que cadastrou
			window.location = '?modulo=principal/telaprograma&acao=A&requisicao=excluirAnaliseSituacionalMeta_II&metid=<?php echo $metid ?>&objid=<?php echo $objid ?>&anmid=' + anmid;
		}
	}
	
	</script>
<?php
	
	if( $db->testa_superuser() ){
		$acao = "
			'<div style=\"white-space:nowrap;\">
				<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: alterarAnalise(\'' || am.anmid || '\');\" title=\"Alterar Análise Situacional\">
				<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"javascript: excluirAnalise(\'' || am.anmid || '\');\" title=\"Excluir Análise Situacional\">
			</div>' as acao,
		"; 
	}elseif( possuiRespMeta($metid) && ( in_array(PERFIL_MONIT_AVALIADOR_OBJETIVO, $perfis) || in_array(PERFIL_MONIT_AVALIADOR_META, $perfis) || in_array(PERFIL_MONIT_CONSULTA, $perfis) || in_array(PERFIL_MONIT_AVALIADOR_INICIATIVA, $perfis) ) ){
		$acao = "
			'<div style=\"white-space:nowrap;\">
				<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: alterarAnalise(\'' || am.anmid || '\');\" title=\"Alterar Análise Situacional\">
				<img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Excluir Análise Situacional\">
			</div>' as acao,
		";
	}elseif( ( in_array(PERFIL_MONIT_CPMO, $perfis) ) ){
			$acao = "
			'<div style=\"white-space:nowrap;\">
			<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: alterarAnalise(\'' || am.anmid || '\');\" title=\"Alterar Análise Situacional\">
			<img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Excluir Análise Situacional\">
			</div>' as acao,
			";		
	}else{
		$acao = "
			'<div style=\"white-space:nowrap;\">
				<img align=\"absmiddle\" src=\"/imagens/alterar_01.gif\" title=\"Alterar Análise Situacional\">
				<img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Excluir Análise Situacional\">
			</div>' as acao,
		";
	}
		
	$sql = "SELECT $acao
					am.anmdsc, 
					r.refperiodo,
					TO_CHAR(am.anmdtinclusao, 'DD/MM/YYYY') AS data,
					u.usunome
			FROM monitora.analisemeta am
			JOIN seguranca.usuario u ON u.usucpf = am.usucpfcadastro				
			JOIN monitora.referencia r ON r.refcod = am.refcod				
			WHERE am.anmstatus = 'A' AND am.metid = {$dados['metid']}";
?>
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Análise Situacional</td>
		</tr>
		<tr>
			<td>
			<?php
				$cabecalho = array("Ação", "Análise", "Período de Referência", "Data de Inclusão", "Cadastrante");
				$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
			?>
			</td>
		</tr>
		<?php 
		if ( $existAnaliseUser < 4 ){
		?>
		<tr bgcolor="#CCCCCC">
			<td>
		   		<?php if( possuiRespMeta($metid) && ( in_array(PERFIL_MONIT_AVALIADOR_OBJETIVO, $perfis) || in_array(PERFIL_MONIT_AVALIADOR_META, $perfis) ) ){ ?>
				<input type="button" name="btnovaanalise" value="Nova Análise" onclick="location.href='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalMeta&metid=<?php echo $metid ?>&objid=<?php echo $objid ?>';" class="botao">
				<?php }elseif( $db->testa_superuser() || in_array(PERFIL_MONIT_CPMO, $perfis) ){?>
				<input type="button" name="btnovaanalise" value="Nova Análise" onclick="location.href='monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAnaliseSituacionalMeta&metid=<?php echo $metid ?>&objid=<?php echo $objid ?>';" class="botao">
				<?php }else{?>
				<input type="button" name="btnovaanalise" value="Nova Análise" disabled="disabled">
				<?php }?>		   	
			</td>
		</tr>
		<?php 
		}
		?> 	
	</table>
<?php	
}

function gerenciarAnaliseSituacionalMeta($dados){
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	if ( $dados['anmid'] ){
		$sql = "SELECT 
					anmid, TRIM(anmdsc) AS anmdsc, anmquantidadealcancada, anmdtreferencia, anmsituacao, refcod, unmcod
				FROM
					monitora.analisemeta
				WHERE
					anmstatus = 'A' AND
					anmid = {$dados['anmid']} AND
					metid = {$dados['metid']}";
		$registros = $db->pegaLinha($sql);
				
		$anmid 					= $registros["anmid"];
		$anmdsc 				= $registros["anmdsc"];
		$anmquantidadealcancada = $registros['anmquantidadealcancada'];
		$anmdtreferencia 		= $registros['anmdtreferencia'];
		$anmsituacao 			= $registros['anmsituacao'];
		$refcod 				= $registros['refcod'];
		$unmcod 				= $registros['unmcod'];
		
		$docid = pegaDocidAnaliseMeta( $dados['anmid'] );
	}
	
	$esdid = pegarEstadoDocumento( $docid );
	
	$habilita = 'N';
	if( $esdid == '' || $esdid == WK_OBJ_EM_CADASTRAMENTO ){
		if( ( possui_perfil2( Array( PERFIL_MONIT_AVALIADOR_META ) && possuiRespMeta( $dados['metid'] ) ) ) ||
			possui_perfil2( Array( PERFIL_MONIT_CPMO ) ) ){
			$habilita = 'S';
		}
	}
?>	
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

	<script type="text/javascript">
	<!--
	
	function excluirAnexoAnalise( manid ){
		if ( confirm('Deseja apagar o anexo?') ){
			location.href = '?modulo=principal/telaprograma&acao=A&requisicao=excluirAnaliseSituacionalMeta&manid=' + manid + '&anmid=<?=$dados['anmid']?>&metid=<?=$dados['metid']?>';
		}
	}
	
	function validaAnalise(){
		if ( $.trim( $('[name=anmdsc]').val() ) == '' ){
			alert('O campo análise é obrigatório!');
			return false;
		}

		if ( $.trim( $('[name=refcod]').val() ) == '' ){
			alert('O campo mês de referência é obrigatório!');$dados['metid']
			return false;
		}

		if($.trim( $('[name=anmquantidadealcancada]').val() ) != '' && document.getElementById('unmcod').value==''){
			alert('Os campos unidade de medida e Quantidade alcançada são obrigatórios!');
			return false;
		}
		
		if($.trim( $('[name=anmquantidadealcancada]').val() ) == '' && document.getElementById('unmcod').value!='') {
			alert('Os campos unidade de medida e Quantidade alcançada são obrigatórios!');
			return false;
		}
		
		return true;
	}
	
	function validaAnexoAnalise(){
		if ( $.trim( $('[name=mandescricao]').val() ) == '' ){
			alert('O campo assunto é obrigatório!');
			return false;
		}
		
		if ( $.trim( $('[name=objanexo]').val() ) == '' ){
			alert('O campo anexo é obrigatório!');
			return false;
		}
		return true;
	}
	//-->
	</script>

	<form name="formulario" action="?modulo=principal/telaprograma&acao=A&requisicao=inserirAnaliseSituacionalMeta&metid=<?=$dados['metid'] ?>" method="post" onsubmit="return validaAnalise();">
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="3">Análise Situacional</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Análise:</b></td>
			<td>
			<? echo campo_textarea('anmdsc', 'S', $habilita, '', '60', '4', '5000', '', 0, '', false, null, $anmdsc); ?>
			</td>
			<td rowspan="6">
			<?php 
			if ( $docid && $habilita == 'S'){
				wf_desenhaBarraNavegacao( $docid , array('descricao'=>'Meta','codigo'=>false),  array('historico'=>$oculta) );
			}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Quantidade Alcançada:</b></td>
			<td>
			<? echo campo_texto('anmquantidadealcancada', 'N', $habilita, '', '10', '20', '###.###.###,##', '','','','','','',$anmquantidadealcancada); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Data de Referência:</b></td>
			<td>
			
			<?echo campo_data2('anmdtreferencia', 'N', $habilita, '', '','','', formata_data($anmdtreferencia));?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Período de Referência:</b></td>
			<td>
			<? 
			$sql = "SELECT
						refcod
					FROM
						monitora.analisemeta am
					WHERE
						am.anmstatus = 'A' AND
						am.metid = {$dados['metid']} AND
						am.usucpfcadastro = '" . $_SESSION['usucpf'] . "'" . 
						( $refcod ? " AND refcod NOT IN ($refcod) " : "" );
			$arRefcod = $db->carregarColuna( $sql );
			
			$sql = "SELECT 
						refcod AS codigo,
						refperiodo AS descricao 
					FROM 
						monitora.referencia 
					WHERE 
						refdata_limite_avaliacao_aca IS NOT NULL AND 
						refsnmonitoramento='t' AND 
						refano_ref='{$_SESSION['exercicio']}'
						" . ( $arRefcod ? " AND refcod NOT IN(" . implode(", ", $arRefcod) . ")" : "" ) . " 
					ORDER BY refdsc";
			$db->monta_combo('refcod', $sql, $habilita, 'Selecione', '', '', '', '200', 'S', 'refcod', false, $refcod);
			?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" width="40%"><b>Unidade de Medida:</b></td>
				<?php 
					$sql = "SELECT unmcod as codigo, unmdsc as descricao 
							FROM public.unidademedida 
							WHERE unmstatus='A'
							ORDER BY unmdsc";
				?>	
			<td>
				<?php 
				echo $db->monta_combo('unmcod', $sql, $habilita, 'Selecione', '', '', '', '200', 'S', 'unmcod', true, $unmcod);
				?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloEsquerda" colspan="2">
			<input type = "hidden" name = "anmid" value= "<?= $anmid ?>">
			<input type = "hidden" name = "anmsituacao" value= "<?= $anmsituacao ?>">
			
			<?php if( $habilita == 'S' ){ ?>
				<input type="submit" name="btl_salvar_analise" value="Salvar">
			<?php }elseif( in_array(PERFIL_MONIT_AVALIADOR_OBJETIVO, $perfis) ){ ?>
				<input type="button" name="" value="Salvar" disabled="disabled">
			<?php }?>
			
			<input type="button" name="btl_voltar" value="Voltar" onclick="location.href='?modulo=principal/telaprograma&acao=A&requisicao=listarAnaliseSituacionalMeta&objid=<?php echo $dados['objid'] ?>&metid=<?php echo $dados['metid'] ?>';">
			</td>
		</tr>
	</table>
	</form>
	
	<form name="anexoobjetivo" action="?modulo=principal/telaprograma&acao=A&requisicao=inserirAnaliseSituacionalMeta&anmid=<?=$dados['anmid'] ?>&metid=<?=$dados['metid'] ?>" method="post" enctype="multipart/form-data" onsubmit="return validaAnexoAnalise();">
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Anexo da Análise Situacional</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Assunto:</b></td>
			<td>
			<? echo campo_textarea( 'mandescricao', 'S', $habilita, '', '70', '2', '255'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Anexo:</b></td>
			<td>
				<input name="objanexo" type="file">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloEsquerda">
			<?php if( $habilita == 'S' ){ ?>
				<input type="submit" name="btl_salvar_anexo" value="Salvar">
			<?php }else{ ?>
				<input type="button" name="" value="Salvar" disabled="disabled">
			<?php }?>
			</td>
		</tr>
		<?php 
		if ( $dados['anmid'] ){
		?>
		<tr>
			<td colspan="2">
			<fieldset>
				<legend>Lista de Anexos</legend>
				<div style="overflow:auto; height: 140px;">
			<?php
			if( $db->testa_superuser() ){
				$acao = "
					'<div style=\"white-space:nowrap;\">
						<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirAnexoAnalise(' || ma.manid || ');\">
					</div>' as acao, 
				";
			}elseif( in_array(PERFIL_MONIT_AVALIADOR_OBJETIVO, $perfis) ){
				$acao = "
					'<div style=\"white-space:nowrap;\">
						<img src=\"../imagens/excluir_01.gif\">
					</div>' as acao,
				";
			}
			
			$sql = "SELECT $acao					  
						   ma.mandescricao as assunto, 
						   '<a href=\"?modulo=principal/telaprograma&acao=A&requisicao=downloadAnaliseSituacionalObjetivo&arqid=' || a.arqid || '\">' || a.arqnome || '.' || a.arqextensao || '</a>' as arquivo, 
						   u.usunome as nome 
					FROM 
						monitora.metaanexo ma
			 		INNER JOIN public.arquivo a on a.arqid = ma.arqid 
			 		INNER JOIN seguranca.usuario u on u.usucpf = a.usucpf
					WHERE 
						ma.manstatus = 'A' AND
						ma.metid IN ({$dados['metid']})";
			$cabecalho = array("&nbsp;", "<b>Assunto</b>", "<b>Arquivo</b>", "<b>Inserido por</b>");
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%','center');
			?>
				</div>
			</fieldset>
			</td>
		</tr>
		<?php 
		}
		?>
	</table>
	</form>
<?	
}

//dbg($_REQUEST, d);
$_SESSION['monitora']['objid'] = $_REQUEST['objid']; 
$_SESSION['monitora']['metid'] = $_REQUEST['metid'];
$_SESSION['monitora']['ictid'] = $_REQUEST['ictid'];

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
	
monta_titulo('PPA 2012 - 2015', 'PPA 2012 - 2015');

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
jQuery.noConflict();

jQuery(document).ready(function() {
	carregarProgramas();
});

function carregarProgramas() {
	jQuery.ajax({
		type: "POST",
		url: "monitora.php?modulo=principal/telaprograma&acao=A",
		data: "requisicao=carregarProgramas",
		async: false,
		success: function(html){jQuery('#td_programas').html(html);}
	});
}

function carregarIndicadoresObjetivos(prgcod, obj) {
	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.src = '../imagens/menos.gif';
		obj.title = 'menos';
		var linhan = tabela.insertRow(linha.rowIndex);
		var cell0 = linhan.insertCell(0);
		cell0.innerHTML = '&nbsp';
		var cell1 = linhan.insertCell(1);
		cell1.colSpan = 2;
		cell1.id = 'id_io_'+prgcod+'_'+linhan.rowIndex;
		cell1.innerHTML = 'Carregando...';
	
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=carregarIndicadoresObjetivos&prgcod="+prgcod,
			async: false,
			success: function(html){jQuery('#'+cell1.id).html(html);}
		});

	} else {
		obj.src = '../imagens/mais.gif';
		obj.title = 'mais';
		tabela.deleteRow(linha.rowIndex);
	}
}

function carregarMetasIniciativas(objid, obj) {
	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.src = '../imagens/menos.gif';
		obj.title = 'menos';
		var linhan = tabela.insertRow(linha.rowIndex);
		var cell0 = linhan.insertCell(0);
		cell0.innerHTML = '&nbsp';
		var cell1 = linhan.insertCell(1);
		cell1.colSpan = 2;
		cell1.id = 'id_mi_'+objid+'_'+linhan.rowIndex;
		cell1.innerHTML = 'Carregando...';
	
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=carregarMetasIniciativas&objid="+objid,
			async: false,
			success: function(html){jQuery('#'+cell1.id).html(html);}
		});

	} else {
		obj.src = '../imagens/mais.gif';
		obj.title = 'mais';
		tabela.deleteRow(linha.rowIndex);
	}
}

function carregarAcoes(ictid, obj) {
	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.src = '../imagens/menos.gif';
		obj.title = 'menos';
		var linhan = tabela.insertRow(linha.rowIndex);
		var cell0 = linhan.insertCell(0);
		cell0.innerHTML = '&nbsp';
		var cell1 = linhan.insertCell(1);
		cell1.colSpan = 2;
		cell1.id = 'id_aa_'+ictid+'_'+linhan.rowIndex;
		cell1.innerHTML = 'Carregando...';
	
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=carregarAcoes&ictid="+ictid,
			async: false,
			success: function(html){jQuery('#'+cell1.id).html(html);}
		});

	} else {
		obj.src = '../imagens/mais.gif';
		obj.title = 'mais';
		tabela.deleteRow(linha.rowIndex);
	}
}

function gerenciarIndicadores(prgcod,idpid) {
	window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarIndicadores&prgcod='+prgcod+'&idpid='+idpid,'Indicadores','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function gerenciarObjetivos(prgcod,objid) {
	window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarObjetivos&prgcod='+prgcod+'&objid='+objid,'Objetivos','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function gerenciarMetas(objid,metid) {
	window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarMetas&objid='+objid+'&metid='+metid,'Metas','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function gerenciarIniciativas(objid,ictid) {
	window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarIniciativas&objid='+objid+'&ictid='+ictid,'Iniciativas','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function gerenciarAcoes(ictid,icaid) {
	window.open('monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarAcoes&ictid='+ictid+'&icaid='+icaid,'Ações','scrollbars=yes,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function excluirIndicadores(idpid, obj) {
	var conf = confirm('Deseja realmente excluir o indicador?');
	if(conf) {
		var td_id = obj.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.id;
		var td_parts = td_id.split("_");
		
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=excluirIndicadores&idpid="+idpid,
			async: false,
			success: function(html){alert(html);}
		});
		
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=carregarIndicadoresObjetivos&prgcod="+td_parts[2],
			async: false,
			success: function(html){jQuery('#'+td_id).html(html);}
		});


	}
}

function excluirMetas(metid, obj) {
	var conf = confirm('Deseja realmente excluir a meta?');
	if(conf) {
		var td_id = obj.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.id;
		
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=excluirMetas&metid="+metid,
			async: false,
			success: function(html){alert(html);}
		});
		

	}
}

function excluirIniciativas(ictid, obj) {
	var conf = confirm('Deseja realmente excluir a iniciativa?');
	if(conf) {
		var td_id = obj.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.id;
		var td_parts = td_id.split("_");
		
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=excluirIniciativas&ictid="+ictid,
			async: false,
			success: function(html){alert(html);}
		});
		
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=carregarMetasIniciativas&objid="+td_parts[2],
			async: false,
			success: function(html){jQuery('#'+td_id).html(html);}
		});

		

	}
}

function excluirAcoes(icaid, obj) {
	var conf = confirm('Deseja realmente excluir a ação?');
	if(conf) {
		var td_id = obj.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.id;
		var td_parts = td_id.split("_");
		
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=excluirAcoes&icaid="+icaid,
			async: false,
			success: function(html){alert(html);}
		});
		
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=carregarAcoes&ictid="+td_parts[2],
			async: false,
			success: function(html){jQuery('#'+td_id).html(html);}
		});

	}
}

function excluirObjetivos(objid, obj) {
	var conf = confirm('Deseja realmente excluir o objetivo?');
	if(conf) {
		var td_id = obj.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.id;
		var td_parts = td_id.split("_");
		
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=excluirObjetivos&objid="+objid,
			async: false,
			success: function(html){alert(html);}
		});
		
		jQuery.ajax({
			type: "POST",
			url: "monitora.php?modulo=principal/telaprograma&acao=A",
			data: "requisicao=carregarIndicadoresObjetivos&prgcod="+td_parts[1],
			async: false,
			success: function(html){jQuery('#'+td_id).html(html);}
		});

	}
}

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarProfissionais">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td id="td_programas"></td>
	</tr>
</table>
</form>