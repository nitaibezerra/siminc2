<?php

function recuperaTipoModalidade(){
	global $db;
	$sql = "SELECT * from proinfantil.tipomodalidade where timstatus = 'A' and modid = 1 order by timdescricao";
	return $db->carregar($sql);
}

function recuperaVigenciaAtual(){
	global $db;
	$sql = "SELECT 		vigid
			FROM		proinfantil.vigencia
			WHERE		vigstatus = 'A'
			ORDER BY	vigdatainicial ASC";
	return $db->pegaUm($sql);
}

function recuperaProInfantil($obrid){
	global $db;
	$sql = "SELECT		pinid
			FROM		proinfantil.proinfantil
			WHERE		obrid = {$obrid}
			AND			pinststus = 'A'";
	return $db->pegaUm($sql);
}

function recuperaAlunoAtendido($pinid){
	global $db;
	$sql = "SELECT		*
			FROM		proinfantil.mdsalunoatendidopbf
			WHERE		pinid = {$pinid}";
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado){
			$arrAlunos[$dado['titid']][$dado['timid']] = $dado['alaquantidade'];
		}
		return $arrAlunos;
	}else{
		return false;
	}
}

function pegaQrpid( $pinid ){
	global $db;
    
    $sql = "SELECT  	   	que.qrpid
            FROM	       	proinfantil.questionario que
            INNER JOIN     	questionario.questionarioresposta qr ON qr.qrpid = que.qrpid
            WHERE          	que.pinid = {$pinid} 
            AND 			qr.queid = ".QUESTIONARIO;

    $qrpid = $db->pegaUm( $sql );
    
    if(!$qrpid)
    {
    	
        $sql = "SELECT
        			obi.obrnome
        		FROM
        			proinfantil.proinfantil pro
				INNER JOIN obras2.obras obi ON obi.obrid = pro.obrid
				WHERE
					pro.pinid = ".$pinid;

        $titulo = $db->pegaUm( $sql );
        
        $arParam = array ( "queid" => QUESTIONARIO, "titulo" => "Proinfancia (".$titulo.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        
        $sql = "INSERT INTO proinfantil.questionario (qrpid, pinid) VALUES ({$qrpid},{$pinid})";
        $db->executar( $sql );
        $db->commit();
    }
    
    return $qrpid;
}

function filtraMunicipio($estuf){
	global $db;
	$sql = "SELECT
				ter.muncod AS codigo,
				ter.mundescricao AS descricao
			FROM
				territorios.municipio ter
			WHERE
				ter.estuf = '$estuf'
			ORDER BY ter.mundescricao";

	echo $db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '',"","","","N","","");
}

function atualizaAnaliseProinfancia($pinid = null){
	global $db;
	
	$sql_data = "SELECT to_date(r.resdsc, 'DD/MM/YYYY') AS data 
			FROM proinfantil.questionario q 
			INNER JOIN questionario.resposta r ON r.qrpid = q.qrpid 
			WHERE r.perid = 1587 AND q.pinid = {$pinid} AND to_date(r.resdsc, 'DD/MM/YYYY') <= '2011-10-31'";
	
	$rs_data = $db->pegaUm($sql_data);
	
	if(!empty($rs_data)){
		$sql_upd = "UPDATE proinfantil.proinfantil 
					SET pinanoseguinte = 'N'
					WHERE pinid = $pinid";		
		$db->executar($sql_upd);
		$db->commit();
		return true;
	} else {
		return false;
	}
}

function checkPerfil( $pflcods, $testa_superuser = true ){

	global $db;

	if ($db->testa_superuser() && $testa_superuser) {

		return true;

	}else{

		if ( is_array( $pflcods ) )
		{
			$pflcods = array_map( "intval", $pflcods );
			$pflcods = array_unique( $pflcods );
		}
		else
		{
			$pflcods = array( (integer) $pflcods );
		}
		if ( count( $pflcods ) == 0 )
		{
			return false;
		}
		$sql = "
			select
				count(*)
			from seguranca.perfilusuario
			where
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
		return $db->pegaUm( $sql ) > 0;

	}
}

function teste_superUser( ){

	global $db;
	
	return $db->testa_superuser();
}

function recuperaObsParecista( $docid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				coalesce(cmddsc,'Não informado.') as observacao,
				max(hst.hstid)
			FROM 
				workflow.documento doc
			INNER JOIN workflow.historicodocumento 	hst ON hst.docid = doc.docid
			INNER JOIN workflow.acaoestadodoc 		aed ON aed.aedid = hst.aedid
			LEFT  JOIN workflow.comentariodocumento cmd ON cmd.hstid = hst.hstid
			WHERE 
				doc.docid = $docid
			GROUP BY
				cmddsc";
	return $db->pegaUm( $sql );
}

function recuperaDiligencia( $municipio ){
	global $db;
	$sql = "SELECT 
					mdaanalisetecnica
			FROM 	
					proinfantil.mdsanalise
  		   WHERE 
  		   			muncod = '{$municipio}' AND mdastatus = 'A' and mdaano = '{$_SESSION['exercicio']}'
		   ORDER BY 
		   			mdaid DESC";
	return $db->pegaUm( $sql );
}

function verificaQuestao( $perid, $qrpid ){
	global $db;
	
	$sql = "SELECT 	resid 
			FROM 	questionario.resposta 
			WHERE 	perid = 3039 AND qrpid = {$qrpid}";
	
	$resposta = $db->pegaUm($sql);
	if( !$resposta ){
		if( $perid == 3040 || $perid == 3041 ){
			return false;
		} else {
			return true;
		}
	} else {
		return true;
	}
}

function pegaEstadoAtualDocumento( $docid ) {
	global $db; 
	if($docid) {
		$docid = (integer) $docid;
		$sql = "SELECT			ed.esdid
				FROM			workflow.documento d
				INNER JOIN 		workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE			d.docid = {$docid}";
		$estado = $db->pegaUm( $sql );
		return $estado;
	} else {
		return false;
	}
}

function pegaPerfil($usucpf){
       global $db;
       
       $sql = "SELECT          pu.pflcod
                       FROM                 seguranca.perfilusuario pu 
                       INNER JOIN         seguranca.perfil p on p.pflcod = pu.pflcod
                       AND                 pu.usucpf = '{$usucpf}' 
                       AND                 p.sisid = {$_SESSION['sisid']}
                       AND                        pflstatus = 'A'";
                               
       $arrPflcod = $db->carregar($sql);
       !$arrPflcod? $arrPflcod = array() : $arrPflcod = $arrPflcod;
       $arrPerfil = array();
       foreach($arrPflcod as $pflcod){
               $arrPerfil[] = $pflcod['pflcod'];
       }
       
       return $arrPerfil;
}


function verificaResolucao(){
	 global $db;
	 
	 $resolucao = array();
	 
	 $sql = "SELECT 	resnumero, Extract('Year' From resdata) as resdata, modid
	 		 FROM 		proinfantil.resolucao
	 		 WHERE		resstatus = 'A'";
	 $aryRes = $db->carregar($sql);
	 $aryRes = $aryRes ? $aryRes : array();
	 
	 foreach($aryRes as $res){
	 	$resolucao[$res['modid']] = "Resolução FNDE nº ".$res['resnumero']."/".$res['resdata'];
	 }
	 return $resolucao;
}

function calculaDiasVigencia($arrWork, $aedidIni, $aedidFim){
	global $db;

	$dias = 0;
	$arDataIni = array();
	$arDataFim = array();
    foreach ($arrWork as $v) {
    	$dataInicio = $db->pegaUm("select to_char(h.htddata, 'YYYY-MM-DD') from workflow.historicodocumento h where h.aedid = $aedidIni and h.hstid = {$v['hstid']}");
        $dataFinal  = $db->pegaUm("select to_char(h.htddata, 'YYYY-MM-DD') from workflow.historicodocumento h where h.aedid = $aedidFim and h.hstid = {$v['hstid']}");
        		
        if($dataInicio){
        	array_push($arDataIni, $dataInicio);
        }
        if($dataFinal){
        	array_push($arDataFim, $dataFinal);
        }
    }
    
    $dataAtual = date('Y-m-d');
    $dias = 0;
    $totalDias = 0;
    foreach ($arDataIni as $key => $dtini) {
    	$dtfim = $arDataFim[$key];
        if( !empty($dtfim) ){        	 
        	 $diferenca = strtotime($dtfim) - strtotime($dtini);
        	 $dias = floor($diferenca / (60 * 60 * 24));
        	 $dias += ( ($d == 0) ? 1 : $d);
        } else{
        	$diferenca = strtotime($dataAtual) - strtotime($dtini);
        	$dias = floor($diferenca / (60 * 60 * 24));        	
        	$dias += ( ($d == 0) ? 1 : $d); 
        }        
        $totalDias = $totalDias + $dias;
	}
	
	return $totalDias;
}