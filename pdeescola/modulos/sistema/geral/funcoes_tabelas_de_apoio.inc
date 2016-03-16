<?php
function retornaSelectMontaTabela($nomeTabela) {
	if($nomeTabela == "tiponivelmodalidadeensino")
		return pg_query("SELECT tmeid as codigo,tmedescricao as descricao FROM pdeescola.tiponivelmodalidadeensino ORDER BY tmedescricao");
	if($nomeTabela == "tipodependencia")
		return pg_query("SELECT tidid as codigo,tiddescricao as descricao FROM pdeescola.tipodependencia ORDER BY tiddescricao");
	if($nomeTabela == "seriecicloescolar")
		return pg_query("SELECT sceid as codigo,scedescricao as descricao FROM pdeescola.seriecicloescolar ORDER BY scedescricao");
	if($nomeTabela == "tipodisciplina")
		return pg_query("SELECT tdiid as codigo,tdidescricao as descricao FROM pdeescola.tipodisciplina ORDER BY tdidescricao");
	if($nomeTabela == "faixaetaria")
		return pg_query("SELECT faeid as codigo,faedescricao as descricao FROM pdeescola.faixaetaria ORDER BY faedescricao");
	if($nomeTabela == "tipoturno")
		return pg_query("SELECT titid as codigo,titdescricao as descricao FROM pdeescola.tipoturno ORDER BY titdescricao");
	if($nomeTabela == "cargofuncao")
		return pg_query("SELECT cafid as codigo,cafdescricao as descricao FROM pdeescola.cargofuncao ORDER BY cafdescricao");
	if($nomeTabela == "tipoesfera")
		return pg_query("SELECT tieid as codigo,tiedescricao as descricao FROM pdeescola.tipoesfera ORDER BY tiedescricao");
	if($nomeTabela == "fonterecurso")
		return pg_query("SELECT forid as codigo,fordescricao as descricao FROM pdeescola.fonterecurso ORDER BY fordescricao");
	if($nomeTabela == "conceitoavaliacao")
		return pg_query("SELECT coaid as codigo,coadescricao as descricao FROM pdeescola.conceitoavaliacao ORDER BY coadescricao");
	if($nomeTabela == "tipolocalizacao")
		return pg_query("SELECT tloid as codigo,tlodescricao as descricao FROM pdeescola.tipolocalizacao ORDER BY tlodescricao");
	if($nomeTabela == "fasepde")
		return pg_query("SELECT fasid as codigo,fasdescricao as descricao FROM pdeescola.fasepde ORDER BY fasdescricao");
	if($nomeTabela == "tipoprograma")
		return pg_query("SELECT tprid as codigo,tprdescricao as descricao FROM pdeescola.tipoprograma ORDER BY tprdescricao");
	if($nomeTabela == "tipocategoria")
		return pg_query("SELECT tcaid as codigo,tcadescricao as descricao FROM pdeescola.tipocategoria ORDER BY tcadescricao");
}

function podeExcluir($nomeTabela, $codigoRegistro) {
	global $db;
	
	if($nomeTabela == "tiponivelmodalidadeensino")
		$sqlPodeExcluir = "	SELECT sum(valores) from (
								(SELECT count(*) as valores FROM pdeescola.nivelmodalidadeensino WHERE tmeid = ".$codigoRegistro."
							union all 
								SELECT count(*) as valores FROM pdeescola.seriecicloescolar WHERE tmeid = ".$codigoRegistro.")) as valoresfinais";
		
	if($nomeTabela == "tipodependencia")
		$sqlPodeExcluir = "SELECT count(*) FROM pdeescola.dependenciascondicaouso WHERE tidid = ".$codigoRegistro;
		
	if($nomeTabela == "seriecicloescolar")
		$sqlPodeExcluir = " SELECT sum(valores) from (
								(SELECT count(*) as valores FROM pdeescola.disciplinacritica  WHERE sceid= ".$codigoRegistro."
								UNION ALL 
								SELECT count(*) as valores FROM pdeescola.distorcaoidadeserie WHERE sceid= ".$codigoRegistro."
								UNION ALL 
								SELECT count(*) as valores FROM pdeescola.aproveitamentoaluno WHERE sceid= ".$codigoRegistro."
								UNION ALL 
								SELECT count(*) as valores FROM pdeescola.matriculainicial WHERE sceid= ".$codigoRegistro."
								--UNION ALL 
								--SELECT count(*) as valores FROM pdeescola.relacaodocentealuno WHERE sceid= ".$codigoRegistro."
							))as valoresfinais";
		
	if($nomeTabela == "tipodisciplina")
		$sqlPodeExcluir = "SELECT count(*) FROM pdeescola.disciplinacritica WHERE tdiid = ".$codigoRegistro;

	if($nomeTabela == "faixaetaria")
		$sqlPodeExcluir = "SELECT count(*) FROM pdeescola.distorcaoidadeserie WHERE faeid = ".$codigoRegistro;
		
	if($nomeTabela == "tipoturno")
		$sqlPodeExcluir = "	SELECT sum(valores) from (
								(SELECT count(*) as valores FROM pdeescola.tipoturno WHERE titid = ".$codigoRegistro."
								UNION ALL 
								SELECT count(*) as valores FROM pdeescola.matriculainicial WHERE titid = ".$codigoRegistro."
								UNION ALL 
								SELECT count(*) as valores FROM pdeescola.relacaoalunodocente WHERE titid = ".$codigoRegistro."
							))as valoresfinais ";

	if($nomeTabela == "cargofuncao")
		$sqlPodeExcluir = "SELECT count(*) FROM pdeescola.pessoaltecnicoformacao WHERE cafid = ".$codigoRegistro;
		
	if($nomeTabela == "tipoesfera")
		$sqlPodeExcluir = "SELECT count(*) FROM pdeescola.fonterecurso WHERE tieid = ".$codigoRegistro;

	if($nomeTabela == "fonterecurso")
		$sqlPodeExcluir = " SELECT sum(valores) from (
								(SELECT count(*) as valores FROM pdeescola.fontedestinacaorecurso WHERE forid = ".$codigoRegistro."
								UNION ALL 
								SELECT count(*) as valores FROM pdeescola.previsaorecursosescola WHERE forid = ".$codigoRegistro."
							))as valoresfinais";
		
	if($nomeTabela == "conceitoavaliacao")
		$sqlPodeExcluir = " SELECT sum(valores) from (
								(SELECT count(*) as valores FROM pdeescola.avaliarelacaosecretaria WHERE coaid = ".$codigoRegistro."
								UNION ALL 
								SELECT count(*) as valores FROM pdeescola.avaliarelacaocomunidade WHERE coaid = ".$codigoRegistro."
							))as valoresfinais";

	if($nomeTabela == "tipolocalizacao")
		$sqlPodeExcluir = "SELECT count(*) FROM pdeescola.pdeescola WHERE tloid = ".$codigoRegistro;

	if($nomeTabela == "fasepde")
		$sqlPodeExcluir = "SELECT count(*) FROM pdeescola.pdeescola WHERE fasid = ".$codigoRegistro;
		if($nomeTabela == "tipoprograma")
		$sqlPodeExcluir = "SELECT count(*) FROM pdeescola.tipoprograma WHERE tprid = ".$codigoRegistro;
		if($nomeTabela == "tipocategoria")
		$sqlPodeExcluir = "	SELECT sum(valores) from (
								(SELECT count(*) as valores FROM pdeescola.tipocategoria WHERE tcaid = ".$codigoRegistro."
								UNION ALL  
								SELECT count(*) as valores FROM pdeescola.tiporecurso WHERE treid = ".$codigoRegistro."
							))as valoresfinais ";

	if($db->pegaUm($sqlPodeExcluir) == 0)
		return true;
	else
		return false;
}

function insereDadosTipoNivelModalidaDeEnsino($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT tmeid FROM pdeescola.tiponivelmodalidadeensino");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['tmeid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.tiponivelmodalidadeensino WHERE tmeid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.tiponivelmodalidadeensino(tmedescricao,tmedataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.tiponivelmodalidadeensino
						SET 
							tmedescricao = '".trim($retornoTabelaDescricao[$i])."',
							tmedataatualizacao = now()							 
						WHERE 
							tmeid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosTipoDependencia($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT tidid FROM pdeescola.tipodependencia");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['tidid'];
		$cont = 0;
	
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}

		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.tipodependencia WHERE tidid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}

	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.tipodependencia(tiddescricao,tiddataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.tipodependencia
						SET 
							tiddescricao = '".trim($retornoTabelaDescricao[$i])."',
							tiddataatualizacao = now()						 
						WHERE 
							tidid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosSerieCicloEscolar($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT sceid FROM pdeescola.seriecicloescolar");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['sceid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.seriecicloescolar WHERE sceid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.seriecicloescolar(scedescricao,scedataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.seriecicloescolar
						SET 
							scedescricao = '".trim($retornoTabelaDescricao[$i])."',
							scedataatualizacao = now()						 
						WHERE 
							sceid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosTipoDiciplina($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT tdiid FROM pdeescola.tipodisciplina");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['tdiid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.tipodisciplina WHERE tdiid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.tipodisciplina(tdidescricao,tdidataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.tipodisciplina
						SET 
							tdidescricao = '".trim($retornoTabelaDescricao[$i])."',
							tdidataatualizacao = now()						 
						WHERE 
							tdiid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosFaixaetaria($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT faeid FROM pdeescola.faixaetaria");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['faeid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.faixaetaria WHERE faeid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.faixaetaria(faedescricao,faedataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.faixaetaria
						SET 
							faedescricao = '".trim($retornoTabelaDescricao[$i])."',
							faedataatualizacao = now()						 
						WHERE 
							faeid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosTipoTurno($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT titid FROM pdeescola.tipoturno");
	while(($dados = pg_fetch_array($sql)) != false) {
		$titid = $dados['titid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($titid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.tipoturno WHERE titid = ".$titid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.tipoturno(titdescricao,titdataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.tipoturno
						SET 
							titdescricao = '".trim($retornoTabelaDescricao[$i])."',
							titdataatualizacao = now()						 
						WHERE 
							titid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosCargoFuncao($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT cafid FROM pdeescola.cargofuncao");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['cafid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.cargofuncao WHERE cafid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.cargofuncao(cafdescricao,cafdataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.cargofuncao
						SET 
							cafdescricao = '".trim($retornoTabelaDescricao[$i])."',
							cafdataatualizacao = now()						 
						WHERE 
							cafid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosTipoEsfera($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT tieid FROM pdeescola.tipoesfera");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['tieid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.tipoesfera WHERE tieid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.tipoesfera(tiedescricao,tiedataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.tipoesfera
						SET 
							tiedescricao = '".trim($retornoTabelaDescricao[$i])."',
							tiedataatualizacao = now()						 
						WHERE 
							tieid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosFonteRecurso($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT forid FROM pdeescola.fonterecurso");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['forid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.fonterecurso WHERE forid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.fonterecurso(fordescricao,fordataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.fonterecurso
						SET 
							fordescricao = '".trim($retornoTabelaDescricao[$i])."',
							fordataatualizacao = now()						 
						WHERE 
							forid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosConceitoAvaliacao($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT coaid FROM pdeescola.conceitoavaliacao");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['coaid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.conceitoavaliacao WHERE coaid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.conceitoavaliacao(coadescricao,coadataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.conceitoavaliacao
						SET 
							coadescricao = '".trim($retornoTabelaDescricao[$i])."',
							coadataatualizacao = now()						 
						WHERE 
							coaid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosTipoLocalizacao($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT tloid FROM pdeescola.tipolocalizacao");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['tloid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.tipolocalizacao WHERE tloid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.tipolocalizacao(tlodescricao,tlodataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.tipolocalizacao
						SET 
							tlodescricao = '".trim($retornoTabelaDescricao[$i])."',
							tlodataatualizacao = now()						 
						WHERE 
							tloid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosFasePDE($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT fasid FROM pdeescola.fasepde");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tmeid = $dados['fasid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tmeid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.fasepde WHERE fasid = ".$tmeid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.fasepde(fasdescricao,fasdataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.fasepde
						SET 
							fasdescricao = '".trim($retornoTabelaDescricao[$i])."',
							fasdataatualizacao = now()						 
						WHERE 
							fasid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosTipoPrograma($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT tprid FROM pdeescola.tipoprograma");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tprid = $dados['tprid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tprid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.tipoprograma WHERE tprid = ".$tprid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.tipoprograma(tprdescricao,tprdataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.tipoprograma
						SET 
							tprdescricao = '".trim($retornoTabelaDescricao[$i])."',
							tprdataatualizacao = now()						 
						WHERE 
							tprid = ".trim($retornoTabelaID[$i]);
				
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosTipoCategoria($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	$cont = 1;
	// Deleta registros se necessário.
	$sql = pg_query("SELECT tcaid FROM pdeescola.tipocategoria");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tcaid = $dados['tcaid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tcaid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM pdeescola.tipocategoria WHERE tcaid = ".$tcaid;
			$db->executar($sql_delete);
			$db->commit();
			
		}
	}
	
	if((count($retornoTabelaID) > 0 ) && ($cont != 0)  ) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							pdeescola.tipocategoria(tcadescricao,tcadataatualizacao ) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."',now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							pdeescola.tipocategoria
						SET 
							tcadescricao = '".trim($retornoTabelaDescricao[$i])."',
							tcadataatualizacao = now()						 
						WHERE 
							tcaid = ".trim($retornoTabelaID[$i]);
				
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

?>