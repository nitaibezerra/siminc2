<?php

$downloadfile = "justificativas_sidor.txt";

header("Content-Type: plain/text");
header("Content-disposition: attachment; filename=$downloadfile");
header("Content-Transfer-Encoding: binary");
header("Pragma: no-cache");
header("Expires: 0");

$sql = pg_query("
SELECT
	Case length(ipc.ipcnumerodecontrole)
		when 1 then '0000000'||ipc.ipcnumerodecontrole 
		when 2 then '000000'||ipc.ipcnumerodecontrole 
		when 3 then '00000'||ipc.ipcnumerodecontrole 
		when 4 then '0000'||ipc.ipcnumerodecontrole 
		when 5 then '000'||ipc.ipcnumerodecontrole 
		when 6 then '00'||ipc.ipcnumerodecontrole 
		when 7 then '0'||ipc.ipcnumerodecontrole 
		when 8 then ipc.ipcnumerodecontrole 
		end AS ipcnumerodecontrole,
	trim(regexp_replace(replace(replace(replace(regexp_replace(jc.jsccaracterizacao,'<\s*?[^>]+\s*?>',' ','g'),chr(9),' '),chr(10),''),chr(13),' '),'&[^; ]*;','','g')) AS jsccaracterizacao,
	trim(regexp_replace(replace(replace(replace(regexp_replace(jc.jscreflexo,'<\s*?[^>]+\s*?>',' ','g'),chr(9),' '),chr(10),''),chr(13),' '),'&[^; ]*;','','g')) AS jscreflexo,
	trim(regexp_replace(replace(replace(replace(regexp_replace(jc.jscconsequencia,'<\s*?[^>]+\s*?>',' ','g'),chr(9),' '),chr(10),''),chr(13),' '),'&[^; ]*;','','g')) AS jscconsequencia,	
	trim(regexp_replace(replace(replace(replace(regexp_replace(jc.jscrepercussao,'<\s*?[^>]+\s*?>',' ','g'),chr(9),' '),chr(10),''),chr(13),' '),'&[^; ]*;','','g')) AS jscrepercussao,
	trim(regexp_replace(replace(replace(replace(regexp_replace(jc.jscmemoria,'<\s*?[^>]+\s*?>',' ','g'),chr(9),' '),chr(10),''),chr(13),' '),'&[^; ]*;','','g')) AS jscmemoria
--	trim(regexp_replace(replace(replace(replace(regexp_replace(jc.jscresultado,'<\s*?[^>]+\s*?>',' ','g'),chr(9),' '),chr(10),''),chr(13),' '),'&[^; ]*;','','g')) AS jscresultado,
--	trim(regexp_replace(replace(replace(replace(regexp_replace(jc.jscaplicacao,'<\s*?[^>]+\s*?>',' ','g'),chr(9),' '),chr(10),''),chr(13),' '),'&[^; ]*;','','g')) AS jscaplicacao
FROM 
	elabrev.itempedidocredito ipc 
INNER JOIN 
	elabrev.tipocredito tc ON tc.tcrcod = substr(ipc.ipctipocredito,1,3) AND tc.tcrstatus = 'A' and tc.tcrano= '2011' 
INNER JOIN 
	elabrev.justificativacredito jc ON jc.tcrid = tc.tcrid AND jc.unicod = trim(ipc.ipcorgao||ipc.ipcunidade) 
									   AND jc.unitpocod = 'U' 
INNER JOIN 
	elabrev.pedidocredito pc ON pc.pcrid = ipc.pcrid 
INNER JOIN 
	carga.saidacreditos sc ON pc.pcrnumeropedidocredito::integer = sc.numpedido::integer 
WHERE
--	pc.pcrano = '2010' and ipc.ipcnumerodecontrole is not null and ipc.ipcnumerodecontrole between '00001403' and '00001447' AND jc.mcrid = 20
--	pc.pcrano = '2010' and ipc.ipcnumerodecontrole is not null and ipc.ipcnumerodecontrole between '00001393' and '00001402' AND jc.mcrid = 19
--	pc.pcrano = '2010' and ipc.ipcnumerodecontrole is not null and ipc.ipcnumerodecontrole between '00001448' and '00001489' AND jc.mcrid = 18
	pc.pcrano = '2011' and ipc.ipcnumerodecontrole is not null and ipc.ipcnumerodecontrole between '15096' and '15140' AND jc.mcrid = 27
--	pc.pcrano = '2009' and ipc.ipcnumerodecontrole is not null and ipc.ipcnumerodecontrole between '00004998' and '00005037' AND jc.mcrid = 16
--	pc.pcrano = '2009' and ipc.ipcnumerodecontrole is not null and ipc.ipcnumerodecontrole between '00004091' and '00004151' AND jc.mcrid = 15
--	pc.pcrano = '2009' and ipc.ipcnumerodecontrole is not null and ipc.ipcnumerodecontrole between '00001715' and '00001792' AND jc.mcrid = 12
--	pc.pcrano = '2009' and ipc.ipcnumerodecontrole is not null and ipc.ipcnumerodecontrole between '00001793' and '00001801' AND jc.mcrid = 13
--	pc.pcrano = '2009' and ipc.ipcnumerodecontrole is not null and ipc.ipcnumerodecontrole between '00001802' and '00001805' AND jc.mcrid = 14
GROUP BY 
	ipcnumerodecontrole,
	jsccaracterizacao,
	jscresultado,
	jscconsequencia,
	jscreflexo,
	jscrepercussao,
	jscaplicacao,
	jscmemoria
");


$numSequencial = "000001";
while(($dados = pg_fetch_array($sql)) != false) {
	$controle = 0;
	$strLinha = "";
	
	/*** NÚMERO DE CONTROLE ***/
	$numControle = $dados['ipcnumerodecontrole'];
	
	/*** USUÁRIO ***/
	$usuario = "IQF";
	
	/*** ANO DO EXERCÍCIO ***/
	if($_SESSION["exercicio"] != "") {
		$anoExercicio = $_SESSION["exercicio"];		
	}
	else {
		$anoExercicio = "    ";		 
	}
	
	/*** MOMENTO ***/	
	$momento = "20";
	
	//*************************************************************************************//
	
	/*** Pergunta - Caracterização ***/
	$pergunta = "01";
	
	/*** Nº Linha Justificativa ***/
	$linhaJustificativa = "000001";
	
	/*** Justificativa - Caracterização ***/
	$justCaracterizacao = "";
	if(($dados['jsccaracterizacao'] != "") && ($dados['jsccaracterizacao'] != null)) {
		$justCaracterizacao .= (string) $dados['jsccaracterizacao'];		
	} else {
		/*for($k=0;$k<60;$k++)
			$justCaracterizacao .= " ";*/
		$controle = 1;
	}
	 
  if($controle == 0) {
	$tamanho = strlen($justCaracterizacao);	
	$qtdLinhas = ($tamanho / 60);
	
	if(is_float($qtdLinhas)) {
		$qtdLinhas = (intval($qtdLinhas) + 1);
	}
	
	$justificativa = array();
	for($i=0; $i < $qtdLinhas; $i++) {
		$posIncial = ($i * 60);
		$justificativa[$i] = substr($justCaracterizacao, $posIncial, 60);
		
		if(strlen($justificativa[$i]) < 60) {
			$aux = (60 - strlen($justificativa[$i]));
			for($k=0; $k < $aux; $k++) {
				$justificativa[$i] .= " ";
			}
		}
		
		if($numSequencial != "000001")
			$strLinha = "\r\n".$strLinha;
		
		$strLinha .= $numControle . $usuario . $anoExercicio . $momento;
		$strLinha .= $pergunta . $linhaJustificativa . $justificativa[$i] . $numSequencial;
		
		echo $strLinha;
		
		$sql_inclusao = "INSERT INTO elabrev.justificativapedidocredito (jpcnumerodecontrole,jpcusuario,
						 jpcanoexercicio,jpcmomento,jpcpergunta,jpcnumlinhajustificativa,jpcjustificativa,
						 jpcnumsequencial) VALUES ('".$numControle."','".$usuario."','".$anoExercicio."',
						 '".$momento."','".$pergunta."','".$linhaJustificativa."','".str_replace("'","''",str_replace("\'","''",$justificativa[$i]))."',
						 '".$numSequencial."')";
		$db->executar($sql_inclusao);
		$db->commit();
		
		$linhaJustificativa = sprintf("%06s", ++$linhaJustificativa);
		$numSequencial = sprintf("%06s", ++$numSequencial);
		$strLinha = "";
	}
  }
	
$controle = 0;
	//*************************************************************************************//
	
	/*** Pergunta - Reflexo ***/
	$pergunta = "02";
	
	/*** Nº Linha Justificativa ***/
	$linhaJustificativa = "000001";
	
	/*** Justificativa - Reflexo ***/
	$justReflexo = "";
	if(($dados['jscreflexo'] != "") && ($dados['jscreflexo'] != null)) {
		$justReflexo .= (string) $dados['jscreflexo'];		
	} else {
		/*for($k=0;$k<60;$k++)
			$justReflexo .= " ";*/
		$controle = 1;
	}
if($controle == 0) {
	$tamanho = strlen($justReflexo);	
	$qtdLinhas = ($tamanho / 60);
	
	if(is_float($qtdLinhas)) {
		$qtdLinhas = (intval($qtdLinhas) + 1);
	}
	
	$justificativa = array();
	for($i=0; $i < $qtdLinhas; $i++) {
		$posIncial = ($i * 60);
		$justificativa[$i] = substr($justReflexo, $posIncial, 60);
		
		if(strlen($justificativa[$i]) < 60) {
			$aux = (60 - strlen($justificativa[$i]));
			for($k=0; $k < $aux; $k++) {
				$justificativa[$i] .= " ";
			}
		}
		
		if($numSequencial != "000001")
			$strLinha = "\r\n".$strLinha;
		
		$strLinha .= $numControle . $usuario . $anoExercicio . $momento;
		$strLinha .= $pergunta . $linhaJustificativa . $justificativa[$i] . $numSequencial;
		
		echo $strLinha;
		
		$sql_inclusao = "INSERT INTO elabrev.justificativapedidocredito (jpcnumerodecontrole,jpcusuario,
						 jpcanoexercicio,jpcmomento,jpcpergunta,jpcnumlinhajustificativa,jpcjustificativa,
						 jpcnumsequencial) VALUES ('".$numControle."','".$usuario."','".$anoExercicio."',
						 '".$momento."','".$pergunta."','".$linhaJustificativa."','".str_replace("'","''",str_replace("\'","''",$justificativa[$i]))."',
						 '".$numSequencial."')";
		$db->executar($sql_inclusao);
		$db->commit();
		
		$linhaJustificativa = sprintf("%06s", ++$linhaJustificativa);
		$numSequencial = sprintf("%06s", ++$numSequencial);
		$strLinha = "";
	}
}

	$controle = 0;
	//*************************************************************************************//
	
	/*** Pergunta - Consequência ***/
	$pergunta = "03";
	
	/*** Nº Linha Justificativa ***/
	$linhaJustificativa = "000001";
	
	/*** Justificativa - Consequência ***/
	$justConsequencia = "";
	if(($dados['jscconsequencia'] != "") && ($dados['jscconsequencia'] != null)) {
		$justConsequencia .= (string) $dados['jscconsequencia'];		
	} else {
		/*for($k=0;$k<60;$k++)
			$justConsequencia .= " ";*/
		$controle = 1;
	}
	
if($controle == 0) {
	$tamanho = strlen($justConsequencia);	
	$qtdLinhas = ($tamanho / 60);
	
	if(is_float($qtdLinhas)) {
		$qtdLinhas = (intval($qtdLinhas) + 1);
	}
	
	$justificativa = array();
	for($i=0; $i < $qtdLinhas; $i++) {
		$posIncial = ($i * 60);
		$justificativa[$i] = substr($justConsequencia, $posIncial, 60);
		
		if(strlen($justificativa[$i]) < 60) {
			$aux = (60 - strlen($justificativa[$i]));
			for($k=0; $k < $aux; $k++) {
				$justificativa[$i] .= " ";
			}
		}
		
		if($numSequencial != "000001")
			$strLinha = "\r\n".$strLinha;
		
		$strLinha .= $numControle . $usuario . $anoExercicio . $momento;
		$strLinha .= $pergunta . $linhaJustificativa . $justificativa[$i] . $numSequencial;
		
		echo $strLinha;
		
		$sql_inclusao = "INSERT INTO elabrev.justificativapedidocredito (jpcnumerodecontrole,jpcusuario,
						 jpcanoexercicio,jpcmomento,jpcpergunta,jpcnumlinhajustificativa,jpcjustificativa,
						 jpcnumsequencial) VALUES ('".$numControle."','".$usuario."','".$anoExercicio."',
						 '".$momento."','".$pergunta."','".$linhaJustificativa."','".str_replace("'","''",str_replace("\'","''",$justificativa[$i]))."',
						 '".$numSequencial."')";
		$db->executar($sql_inclusao);
		$db->commit();
		
		$linhaJustificativa = sprintf("%06s", ++$linhaJustificativa);
		$numSequencial = sprintf("%06s", ++$numSequencial);
		$strLinha = "";
	}
}

$controle = 0;
	//*************************************************************************************//
	
	/*** Pergunta - Repercussão ***/
	$pergunta = "04";
	
	/*** Nº Linha Justificativa ***/
	$linhaJustificativa = "000001";
	
	/*** Justificativa - Repercussão ***/
	$justRepercussao = "";
	if(($dados['jscrepercussao'] != "") && ($dados['jscrepercussao'] != null)) {
		$justRepercussao .= (string) $dados['jscrepercussao'];		
	} else {
		/*for($k=0;$k<60;$k++)
			$justRepercussao .= " ";*/
		$controle = 1;
	}
if($controle == 0) {
	$tamanho = strlen($justRepercussao);	
	$qtdLinhas = ($tamanho / 60);
	
	if(is_float($qtdLinhas)) {
		$qtdLinhas = (intval($qtdLinhas) + 1);
	}
	
	$justificativa = array();
	for($i=0; $i < $qtdLinhas; $i++) {
		$posIncial = ($i * 60);
		$justificativa[$i] = substr($justRepercussao, $posIncial, 60);
		
		if(strlen($justificativa[$i]) < 60) {
			$aux = (60 - strlen($justificativa[$i]));
			for($k=0; $k < $aux; $k++) {
				$justificativa[$i] .= " ";
			}
		}
		
		if($numSequencial != "000001")
			$strLinha = "\r\n".$strLinha;
		
		$strLinha .= $numControle . $usuario . $anoExercicio . $momento;
		$strLinha .= $pergunta . $linhaJustificativa . $justificativa[$i] . $numSequencial;
		
		echo $strLinha;
		
		$sql_inclusao = "INSERT INTO elabrev.justificativapedidocredito (jpcnumerodecontrole,jpcusuario,
						 jpcanoexercicio,jpcmomento,jpcpergunta,jpcnumlinhajustificativa,jpcjustificativa,
						 jpcnumsequencial) VALUES ('".$numControle."','".$usuario."','".$anoExercicio."',
						 '".$momento."','".$pergunta."','".$linhaJustificativa."','".str_replace("'","''",str_replace("\'","''",$justificativa[$i]))."',
						 '".$numSequencial."')";
		$db->executar($sql_inclusao);
		$db->commit();
		
		$linhaJustificativa = sprintf("%06s", ++$linhaJustificativa);
		$numSequencial = sprintf("%06s", ++$numSequencial);
		$strLinha = "";
	}
	
}

$controle = 0;
	//*************************************************************************************//
	
	/*** Pergunta - Memória ***/
	$pergunta = "05";
	
	/*** Nº Linha Justificativa ***/
	$linhaJustificativa = "000001";
	
	/*** Justificativa - Memória ***/
	$justMemoria = "";
	if(($dados['jscmemoria'] != "") && ($dados['jscmemoria'] != null)) {
		$justMemoria .= (string) $dados['jscmemoria'];		
	} else {
		/*for($k=0;$k<60;$k++)
			$justMemoria .= " ";*/
		$controle = 1;
	}
if($controle == 0) {
	$tamanho = strlen($justMemoria);	
	$qtdLinhas = ($tamanho / 60);
	
	if(is_float($qtdLinhas)) {
		$qtdLinhas = (intval($qtdLinhas) + 1);
	}
	
	$justificativa = array();
	for($i=0; $i < $qtdLinhas; $i++) {
		$posIncial = ($i * 60);
		$justificativa[$i] = substr($justMemoria, $posIncial, 60);
		
		if(strlen($justificativa[$i]) < 60) {
			$aux = (60 - strlen($justificativa[$i]));
			for($k=0; $k < $aux; $k++) {
				$justificativa[$i] .= " ";
			}
		}
		
		if($numSequencial != "000001")
			$strLinha = "\r\n".$strLinha;
		
		$strLinha .= $numControle . $usuario . $anoExercicio . $momento;
		$strLinha .= $pergunta . $linhaJustificativa . $justificativa[$i] . $numSequencial;
		
		echo $strLinha;
		
		$sql_inclusao = "INSERT INTO elabrev.justificativapedidocredito (jpcnumerodecontrole,jpcusuario,
						 jpcanoexercicio,jpcmomento,jpcpergunta,jpcnumlinhajustificativa,jpcjustificativa,
						 jpcnumsequencial) VALUES ('".$numControle."','".$usuario."','".$anoExercicio."',
						 '".$momento."','".$pergunta."','".$linhaJustificativa."','".str_replace("'","''",str_replace("\'","''",$justificativa[$i]))."',
						 '".$numSequencial."')";
		$db->executar($sql_inclusao);
		$db->commit();
		
		$linhaJustificativa = sprintf("%06s", ++$linhaJustificativa);
		$numSequencial = sprintf("%06s", ++$numSequencial);
		$strLinha = "";
	}
}

  $controle = 0;
	//*************************************************************************************//
	
	/*** Pergunta - Resultado ***/
	$pergunta = "06";
	
	/*** Nº Linha Justificativa ***/
	$linhaJustificativa = "000001";
	
	/*** Justificativa - Resultado ***/
	$justResultado = "";
	if(($dados['jscresultado'] != "") && ($dados['jscresultado'] != null)) {
		$justResultado .= (string) $dados['jscresultado'];		
	} else {
		/*for($k=0;$k<60;$k++)
			$justResultado .= " ";*/
		$controle = 1;
	}
  if($controle == 0) {	
	$tamanho = strlen($justResultado);	
	$qtdLinhas = ($tamanho / 60);
	
	if(is_float($qtdLinhas)) {
		$qtdLinhas = (intval($qtdLinhas) + 1);
	}
	
	$justificativa = array();
	for($i=0; $i < $qtdLinhas; $i++) {
		$posIncial = ($i * 60);
		$justificativa[$i] = substr($justResultado, $posIncial, 60);
		
		if(strlen($justificativa[$i]) < 60) {
			$aux = (60 - strlen($justificativa[$i]));
			for($k=0; $k < $aux; $k++) {
				$justificativa[$i] .= " ";
			}
		}
		
		if($numSequencial != "000001")
			$strLinha = "\r\n".$strLinha;
		
		$strLinha .= $numControle . $usuario . $anoExercicio . $momento;
		$strLinha .= $pergunta . $linhaJustificativa . $justificativa[$i] . $numSequencial;
		
		echo $strLinha;
		
		$sql_inclusao = "INSERT INTO elabrev.justificativapedidocredito (jpcnumerodecontrole,jpcusuario,
						 jpcanoexercicio,jpcmomento,jpcpergunta,jpcnumlinhajustificativa,jpcjustificativa,
						 jpcnumsequencial) VALUES ('".$numControle."','".$usuario."','".$anoExercicio."',
						 '".$momento."','".$pergunta."','".$linhaJustificativa."','".str_replace("'","''",str_replace("\'","''",$justificativa[$i]))."',
						 '".$numSequencial."')";
		$db->executar($sql_inclusao);
		$db->commit();
		
		$linhaJustificativa = sprintf("%06s", ++$linhaJustificativa);
		$numSequencial = sprintf("%06s", ++$numSequencial);
		$strLinha = "";
	}
  }

$controle = 0;
	//*************************************************************************************//

	/*** Pergunta - Aplicação ***/
	$pergunta = "07";
	
	/*** Nº Linha Justificativa ***/
	$linhaJustificativa = "000001";
	
	/*** Justificativa - Aplicação ***/
	$justAplicacao = "";
	if(($dados['jscaplicacao'] != "") && ($dados['jscaplicacao'] != null)) {
		$justAplicacao .= (string) $dados['jscaplicacao'];		
	} else {
		/*for($k=0;$k<60;$k++)
			$justAplicacao .= " ";*/
		$controle = 1;
	}
if($controle == 0) {
	$tamanho = strlen($justAplicacao);	
	$qtdLinhas = ($tamanho / 60);
	
	if(is_float($qtdLinhas)) {
		$qtdLinhas = (intval($qtdLinhas) + 1);
	}
	
	$justificativa = array();
	for($i=0; $i < $qtdLinhas; $i++) {
		$posIncial = ($i * 60);
		$justificativa[$i] = substr($justAplicacao, $posIncial, 60);
		
		if(strlen($justificativa[$i]) < 60) {
			$aux = (60 - strlen($justificativa[$i]));
			for($k=0; $k < $aux; $k++) {
				$justificativa[$i] .= " ";
			}
		}
		
		if($numSequencial != "000001")
			$strLinha = "\r\n".$strLinha;
		
		$strLinha .= $numControle . $usuario . $anoExercicio . $momento;
		$strLinha .= $pergunta . $linhaJustificativa . $justificativa[$i] . $numSequencial;
		
		echo $strLinha;
		
		$sql_inclusao = "INSERT INTO elabrev.justificativapedidocredito (jpcnumerodecontrole,jpcusuario,
						 jpcanoexercicio,jpcmomento,jpcpergunta,jpcnumlinhajustificativa,jpcjustificativa,
						 jpcnumsequencial) VALUES ('".$numControle."','".$usuario."','".$anoExercicio."',
						 '".$momento."','".$pergunta."','".$linhaJustificativa."','".str_replace("'","''",str_replace("\'","''",$justificativa[$i]))."',
						 '".$numSequencial."')";
		$db->executar($sql_inclusao);
		$db->commit();
		
		$linhaJustificativa = sprintf("%06s", ++$linhaJustificativa);
		$numSequencial = sprintf("%06s", ++$numSequencial);
		$strLinha = "";
	}
}

}
exit;

?>