<?php

function retornaSelectMontaTabela($nomeTabela) {
	if($nomeTabela == "tipoassunto")
		return pg_query("SELECT tasid as codigo,tasdsc as descricao FROM conjur.tipoassunto  WHERE tasstatus = 'A' ORDER BY tasdsc");
	
	if($nomeTabela == "procedencia")
		return pg_query("SELECT proid as codigo, prodsc as descricao, unpid as idunidade FROM conjur.procedencia ORDER BY proid");
	
}

function podeExcluir($nomeTabela, $codigoRegistro) {
	global $db;

	if($nomeTabela == "tipoassunto")
		$sqlPodeExcluir = "SELECT count(*) FROM conjur.tipoassunto WHERE tasid = ".$codigoRegistro;
	
	if($nomeTabela == "procedencia")
		$sqlPodeExcluir = "SELECT count(*) FROM conjur.procedencia WHERE proid = ".$codigoRegistro;
		
	if($db->pegaUm($sqlPodeExcluir) == 0)
		return true;
	else
		return false;
}

function insereDadosTipoAssunto($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT tasid FROM conjur.tipoassunto WHERE tasstatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tasid = $dados['tasid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tasid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM conjur.tipoassunto WHERE tasid = ".$tasid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							conjur.tipoassunto(tasdsc,tasstatus,tasdtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							conjur.tipoassunto 
						SET 
							tasdsc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							tasid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}	
}

function insereDadosProcedencia($retornoTabelaID, $retornoTabelaDescricao, $retornoTabelaDescricaoServicos) {
global $db;
//	/dump($retornoTabelaID).die;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT proid FROM conjur.procedencia");
	while($dados = pg_fetch_array($sql)) {
		$proid = $dados['proid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($proid== trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM conjur.procedencia WHERE proid = ".$proid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			//intens novos
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							conjur.procedencia(prodsc, unpid, prodtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', ".trim($retornoTabelaDescricaoServicos[$i]).", now());";	
				//dump.die($sql);
				$db->executar($sql);			
				$db->commit();
			// itens editados	
			} else {
				$sql = "UPDATE 
							conjur.procedencia 
						SET 
							prodsc = '".trim($retornoTabelaDescricao[$i])."',	
							unpid = ".trim($retornoTabelaDescricaoServicos[$i]).",	
							prodtinclusao = now()					 
						WHERE 
							proid = ".trim($retornoTabelaID[$i]);
				//dump.die($sql);
				$db->executar($sql);				
				$db->commit();
			}
		}
	}
}
?>