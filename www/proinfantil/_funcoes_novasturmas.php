<?php
function pegaQrpidNovasTurmas( $turid )
{
	global $db;
    
    $sql = "SELECT
            	que.qrpid
            FROM
            	proinfantil.questionario que
            INNER JOIN 
            	questionario.questionarioresposta qr ON qr.qrpid = que.qrpid
            WHERE
            	que.turid = {$turid} 
            	AND qr.queid = ".QUESTIONARIONOVASTURMAS;

    $qrpid = $db->pegaUm( $sql );
    
    if(!$qrpid)
    {
    	
        $sql = "SELECT
        			tur.turdsc
        		FROM
        			proinfantil.turma tur
				WHERE
					tur.turstatus = 'A' and tur.turid = ".$turid;

        $titulo = $db->pegaUm( $sql );
        
        $arParam = array ( "queid" => QUESTIONARIONOVASTURMAS, "titulo" => "Proinfância - Novas Turmas (".$titulo.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        
        $sql = "INSERT INTO proinfantil.questionario (qrpid, turid) VALUES ({$qrpid},{$turid})";
        $db->executar( $sql );
        $db->commit();
    }
    
    return $qrpid;
}

function criaDocumentoNovasTurmas( $muncod ) {
	
	global $db;
	
	if(empty($muncod)) return false;
	
	$docid = pegaDocidNovasTurmas( $muncod );
	
	if( !$docid ){
				
		$tpdid = WF_TPDID_PROINFANTIL_NOVASTURMAS;
		
		$docdsc = "Cadastramento sistema de informação ao cidadão";
		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );		
		
		$sql = "UPDATE proinfantil.novasturmasmunicipios SET 
				 docid = ".$docid." 
				WHERE
				 muncod = '".$muncod."'";

		$db->executar( $sql );		
		$db->commit();
		return $docid;
		
	}
	else {
		return $docid;
	}
}

function pegaDocidNovasTurmas( $muncod ) {
	
	global $db;
	
	$slcid = (integer) $turid;	
	
	$sql = "SELECT
			 docid
			FROM
			 proinfantil.novasturmasmunicipios
			WHERE
			 muncod  = '" . $muncod . "'";
	
	return (integer) $db->pegaUm( $sql );
}

function pegaEstadoAtualNovasTurmas( $docid ) {
	
	global $db; 
	
	if($docid) {
		$docid = (integer) $docid;
		 
		$sql = "
			select
				ed.esdid
			from 
				workflow.documento d
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
		$estado = $db->pegaUm( $sql );
		 
		return $estado;
	} else {
		return false;
	}
}

function cabecalhoTurma($turid = '')
{
	global $db;
	
	$html = '<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >';
	
	if($_SESSION['proinfantil']['turid'] && $_SESSION['exercicio'] < '2013'){
		
		$sql = "select 
					tur.turdsc,
					mun.estuf,
					mun.mundescricao 
				from 
					proinfantil.turma tur
				left join 
					territorios.municipio mun on mun.muncod = tur.muncod 
				where 
					turstatus = 'A' and turid = {$_SESSION['proinfantil']['turid']}";
		
		$rs = $db->pegaLinha($sql);
		
		if($rs){
			
			$boCabecalho[] = 'turma';
			
			$html .= '<tr>
						<td class="subtituloDireita">Turma</td>
						<td>'.$rs['turdsc'].'</td>
					  </tr>';
		}	
	} else {
		if($_SESSION['proinfantil']['muncod']){
			
			$sql = "SELECT 
					    SUM(CAST(
						COALESCE(
							(SELECT h2.htddata 
							FROM workflow.historicodocumento h2 
							INNER JOIN workflow.documento	doc2 ON doc2.docid = h2.docid AND doc2.tpdid = ".WF_TPDID_PROINFANTIL_NOVASTURMAS."
							WHERE 
								h2.hstid = (SELECT min(h3.hstid) 
									    FROM workflow.historicodocumento h3 
									    INNER JOIN workflow.documento	doc3 ON doc3.docid = h3.docid AND doc3.tpdid = ".WF_TPDID_PROINFANTIL_NOVASTURMAS."
									    WHERE h3.docid = doc.docid AND h3.hstid > h.hstid )
								AND h2.aedid = ".NOVASTURMAS_RETORNAR_ANALISE."
							),now()) as date) - cast(h.htddata as date)) as dias,
					    nd.diltexto,
					    doc.esdid
					FROM
					    proinfantil.turma t
					INNER JOIN proinfantil.analisenovasturmasaprovacao 	na  ON na.turid = t.turid
					INNER JOIN proinfantil.novasturmasworkflowturma 	ntw ON ntw.turid = t.turid
					INNER JOIN workflow.documento 						doc ON doc.docid = ntw.docid 	AND doc.tpdid = ".WF_TPDID_PROINFANTIL_NOVASTURMAS."
					INNER JOIN workflow.historicodocumento 				h   ON h.docid = ntw.docid 		AND h.aedid = ".NOVASTURMAS_ENVIAR_DILIGENCIA."
					LEFT  JOIN proinfantil.novasturmasdiligencia 		nd  ON nd.turid = t.turid 		AND nd.anaid = na.anaid AND nd.dilstatus = 'A'
					WHERE
					    t.turid = $turid
					    AND na.anatipo = 'D' 
					    AND na.anastatus = 'A'
					GROUP BY 
						nd.diltexto, doc.esdid";
			
			if( $turid ) {
				$arrDiligencia = $db->pegaLinha($sql);
				
				if( $arrDiligencia['esdid'] ==  WF_NOVASTURMAS_EM_DILIGENCIA ){
					$diasDiligencia = 90 - (int)$arrDiligencia['dias'];
				} else {
					$diasDiligencia = 0;
				}
			}
			
			$sql = "select muncod, mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['proinfantil']['muncod']}'";
			$rs = $db->pegaLinha($sql);
					
			if($rs){
				
				$boCabecalho[] = 'municipio';
						
				$html .= '<tr>
							<td class="subtituloDireita" width="40%">UF:</td>
							<td width="60%">'.$rs['estuf'].'</td>
						  </tr>
						  <tr>
							<td class="subtituloDireita">Município:</td>
							<td>'.$rs['mundescricao'].'</td>
						  </tr>';
				if( $turid && $arrDiligencia['esdid'] == WF_NOVASTURMAS_EM_DILIGENCIA ) {
					$html .= ' <tr>
							<td class="subtituloCentro" colspan="2" style="color: red; ">Dias restantes para responder a diligência: '.(int)$diasDiligencia.'</td>
						  </tr>';
				}
			}
			
		}else if(empty($_SESSION['proinfantil']['muncod']) && isset($_SESSION['proinfantil']['estuf'])){
			
			$sql = "select estdescricao, estuf from territorios.estado where estuf = '{$_SESSION['proinfantil']['estuf']}'";
			$rs = $db->pegaLinha($sql);
			
			if($rs){
			
				$boCabecalho[] = 'estado';
							
				$html .= '<tr>
							<td class="subtituloDireita" width="40%">Estado:</td>
							<td width="60%">'.$rs['estdescricao'].'</td>
						  </tr>
						  <tr>
							<td class="subtituloDireita">UF:</td>
							<td>'.$rs['estuf'].'</td>
						  </tr>';
			}
		}
	}
	
	$html .= '</table>';
	
	if(isset($boCabecalho)){
		echo $html;
	}
}

function salvarFotosSalaNovasTurmas()
{
	global $db;
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	extract($_POST);	
	
	$arArquivosPermitidos  = array('image/png', 'image/jpeg', 'image/gif', 'image/bmp');	
	
	if(!in_array($_FILES['arquivo']['type'], $arArquivosPermitidos)){
		echo "<script>
				alert('O tipo do arquivo não é permitido!');
				document.location.href = 'proinfantil.php?modulo=novasturmas/formFotosTurma&acao=A&turid={$turid}';
			  </script>";
		die;
	}
	
	if( $_FILES['arquivo']['tmp_name'] ){
		$arrCampos = array(
						"salid" => $salid,
						"turid" => $turid,
						"usucpf" => "'{$_SESSION['usucpf']}'",
						"fotstatus" => "'A'",
						"fotdatainclusao" => "now()"
					      );
		$file = new FilesSimec("fotos", $arrCampos, "proinfantil");
		$file->setUpload($arqdescricao, "arquivo");
		if($db->commit()){
			if($foto == 'turma'){
				echo "<script>document.location.href = 'proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&escolhaTurma={$escolhaTurma}';</script>";
			} else {
				echo "<script>document.location.href = 'proinfantil.php?modulo=novasturmas/formFotosTurma&acao=A&turid={$turid}';</script>";
			}
		}
		die();
	} else {
		$_SESSION['proinfantil']['mgs'] = "Não foi possível realizar a operação!";
		if($foto == 'turma'){
			echo "<script>document.location.href = 'proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&escolhaTurma={$escolhaTurma}';</script>";
		} else {
			echo "<script>document.location.href = 'proinfantil.php?modulo=novasturmas/formFotosTurma&acao=A&turid={$turid}';</script>";
		}
	}
}

function removerFotoSalaNovasTurmas()
{
	global $db;
	$arqid = $_GET['arqid'];
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$file = new FilesSimec("fotos",array(),"proinfantil");
	$file->excluiArquivoFisico($arqid);
	$sql = "delete from proinfantil.fotos where arqid = $arqid;";
	$sql.= "delete from public.arquivo where arqid = $arqid;";
	$db->executar($sql);
	$db->commit();
}

function verificaEnvioAnaliseNovasTurmas()
{
	global $db;
	
	$sqlturmasexistentes = "SELECT 	COUNT(turid) 
							FROM 	proinfantil.turma 
							WHERE 	muncod = '{$_SESSION['proinfantil']['muncod']}' AND turstatus = 'A' and turdtinicio is not null;";	
	$rsTurmas = $db->pegaUm($sqlturmasexistentes);

	if($rsTurmas <= 0){
		return 'É Obrigatório cadastrar pelos uma Turma.';
	}
	
	$sqlFotos = "SELECT entcodent, turid, turdsc, COUNT(tisid) AS total FROM (
					 SELECT 	distinct tur.turid,
					 			tur.turdsc,
								sal.tisid,
								tur.entcodent
					 FROM 		proinfantil.turma tur
					 LEFT JOIN 	proinfantil.fotos fot on fot.turid = tur.turid
					 LEFT JOIN 	proinfantil.sala sal on sal.salid = fot.salid and modid = 2
					 WHERE		tur.muncod = '{$_SESSION['proinfantil']['muncod']}'	and tur.turstatus = 'A'	and tur.turdtinicio is not null			 
				 ) AS foo
				 GROUP BY turid, turdsc, entcodent";
	
	$rsFotos = $db->carregar($sqlFotos);
	
	if(!$rsFotos){
		return 'Cadastrar pelo menos uma foto em cada item!';
	}else{
		foreach($rsFotos as $fotos){
			if($fotos['total'] != 4){
				return "A turma {$fotos['turdsc']} - {$fotos['entcodent']} deve conter pelo menos uma foto em cada item!";
				break;
			}
		}
	}
	
	$sqlAtendimento = "SELECT count(tur.turid) as total 
					   FROM 		proinfantil.turma tur
					   INNER JOIN 	proinfantil.mdsalunoatendidopbf pbf ON pbf.turid = tur.turid
					   WHERE 		tur.turstatus = 'A' AND tur.muncod = '{$_SESSION['proinfantil']['muncod']}' and tur.turdtinicio is not null";
	$rsAtendimento = $db->carregar($sqlAtendimento);
	
	if(!$rsAtendimento){
		return "Preencha a quantidade de alunos para atendimento!";
	}
	return true;
}

function getQrpidNovasTurmas( $arDados = null )
{
	global $db;
	
	if($arDados){
		
		if($arDados['queid']) $arWhere1[] = 'qr.queid ='.$arDados['queid'];
		if($arDados['muncod']) $arWhere1[] = "que.muncod = '{$arDados['muncod']}'";

		if($arDados['queid']){
			
			$sql = "SELECT
						que.qrpid
					FROM
						proinfantil.novasturmasmunicipios que
					INNER JOIN
						questionario.questionarioresposta qr ON qr.qrpid = que.qrpid
					".(isset($arWhere1) ? ' WHERE '.implode(' AND ', $arWhere1) : '');
		
			$qrpid = $db->pegaUm( $sql );
		}
	
		if(!$qrpid)
		{
		 
			$sql = "SELECT
						mun.mundescricao
					FROM
						proinfantil.novasturmasmunicipios ntu
					INNER JOIN
						territorios.municipio mun ON mun.muncod = ntu.muncod
					WHERE
						ntu.muncod = '{$arDados['muncod']}'";
		
			$titulo = $db->pegaUm( $sql );
		
			$arParam = array ( "queid" => $arDados['queid'], "titulo" => "Proinfância - Novas Turmas (".$titulo.")" );
			$qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
		
			$sql = "update proinfantil.novasturmasmunicipios set qrpid = {$qrpid} where muncod = '{$arDados['muncod']}'";
			$db->executar( $sql );
			
			$db->commit();
		}
	
		return $qrpid;
	}
	return false;
}

function recuperarDadosMatriculasCadastradas($turid)
{	
	global $db;
	
	$sql = "SELECT t.turtipoestabelecimento,  t.ttuid, t.tirid, maa.titid, COALESCE(SUM(maa.alaquantidade),0) as qtdalunos
			FROM proinfantil.turma t
			INNER JOIN proinfantil.mdsalunoatendidopbf maa ON maa.turid = t.turid
			WHERE t.muncod = '{$_SESSION['proinfantil']['muncod']}'
			AND t.turid NOT IN ({$turid})
			AND t.turstatus = 'A'
			-- AND t.turid = 51
			GROUP BY t.turtipoestabelecimento,  t.ttuid, t.tirid, maa.titid";

	return $db->carregar($sql);	
}


function recuperarDadosMatriculasTurmas()
{
	global $db;
	
	$sql = "SELECT
	
				ntmid,
					
				-- Matriculas
				COALESCE(SUM(ntcqtdalunocrecheparcialpublica),0) as ntcqtdalunocrecheparcialpublica,
				COALESCE(SUM(ntcqtdalunocrecheintegralpublica),0) as ntcqtdalunocrecheintegralpublica,
				COALESCE(SUM(ntcqtdalunopreescolaparcialpublica),0) as ntcqtdalunopreescolaparcialpublica,
				COALESCE(SUM(ntcqtdalunopreescolaintegralpublica),0) as ntcqtdalunopreescolaintegralpublica,
				COALESCE(SUM(ntcqtdalunocrecheparcialconveniada),0) as ntcqtdalunocrecheparcialconveniada,
				COALESCE(SUM(ntcqtdalunocrecheintegralconveniada),0) as ntcqtdalunocrecheintegralconveniada,
				COALESCE(SUM(ntcqtdalunopreescolaparcialconveniada),0) as ntcqtdalunopreescolaparcialconveniada,
				COALESCE(SUM(ntcqtdalunopreescolaintegralconveniada),0) as ntcqtdalunopreescolaintegralconveniada,
				COALESCE(ntmqtdmatriculacrecheparcialpublica,0) as ntmqtdmatriculacrecheparcialpublica,
				COALESCE(ntmqtdmatriculacrecheintegralpublica,0) as ntmqtdmatriculacrecheintegralpublica,
				COALESCE(ntmqtdmatriculapreescolaparcialpublica,0) as ntmqtdmatriculapreescolaparcialpublica,
				COALESCE(ntmqtdmatriculapreescolaintegralpublica,0) as ntmqtdmatriculapreescolaintegralpublica,
				COALESCE(ntmqtdmatriculacrecheparcialconveniada,0) as ntmqtdmatriculacrecheparcialconveniada,
				COALESCE(ntmqtdmatriculacrecheintegralconveniada,0) as ntmqtdmatriculacrecheintegralconveniada,
				COALESCE(ntmqtdmatriculapreescolaparcialconveniada,0) as ntmqtdmatriculapreescolaparcialconveniada,
				COALESCE(ntmqtdmatriculapreescolaintegralconveniada,0) as ntmqtdmatriculapreescolaintegralconveniada,
					
				-- Turmas
				COALESCE(SUM(ntcqtdturmacrechepublica),0) as ntcqtdturmacrechepublica,
				COALESCE(SUM(ntcqtdturmapreescolapublica),0) as ntcqtdturmapreescolapublica,
				COALESCE(SUM(ntcqtdturmaunificadapublica),0) as ntcqtdturmaunificadapublica,
				COALESCE(SUM(ntcqtdturmacrecheconveniada),0) as ntcqtdturmacrecheconveniada,
				COALESCE(SUM(ntcqtdturmapreescolaconveniada),0) as ntcqtdturmapreescolaconveniada,
				COALESCE(SUM(ntcqtdturmaunificadaconveniada),0) as ntcqtdturmaunificadaconveniada,
				COALESCE(ntmqtdturmacrechepublica,0) as ntmqtdturmacrechepublica,
				COALESCE(ntmqtdturmapreescolapublica,0) as ntmqtdturmapreescolapublica,
				COALESCE(ntmqtdturmaunificadapublica,0) as ntmqtdturmaunificadapublica,
				COALESCE(ntmqtdturmacrecheconveniada,0) as ntmqtdturmacrecheconveniada,
				COALESCE(ntmqtdturmapreescolaconveniada,0) as ntmqtdturmapreescolaconveniada,
				COALESCE(ntmqtdturmaunificadaconveniada,0) as ntmqtdturmaunificadaconveniada
					
			FROM proinfantil.novasturmasdadoscenso dc
			LEFT JOIN proinfantil.novasturmasdadosmunicipios dm on dm.muncod = dc.muncod and ntmano = '".($_SESSION['exercicio'])."'
			WHERE dc.muncod = '{$_SESSION['proinfantil']['muncod']}'
			AND dc.ntcanocenso = '".($_SESSION['exercicio']-1)."'
			and dc.ntcstatus = 'A'
			GROUP BY
				ntmid,
					
				-- Matriculas
				ntmqtdmatriculacrecheparcialpublica,
				ntmqtdmatriculacrecheintegralpublica,
				ntmqtdmatriculapreescolaparcialpublica,
				ntmqtdmatriculapreescolaintegralpublica,
				ntmqtdmatriculacrecheparcialconveniada,
				ntmqtdmatriculacrecheintegralconveniada,
				ntmqtdmatriculapreescolaparcialconveniada,
				ntmqtdmatriculapreescolaintegralconveniada,
					
				-- Turmas
				ntmqtdturmacrechepublica,
				ntmqtdturmapreescolapublica,
				ntmqtdturmaunificadapublica,
				ntmqtdturmacrecheconveniada,
				ntmqtdturmapreescolaconveniada,
				ntmqtdturmaunificadaconveniada";
	return $db->pegaLinha($sql);
}


function salvarPerecerNovasTurmas()
{
	global $db;
	
	extract($_POST);
	$usucpf = $_SESSION['usucpf'];
	
	//Salvar os radios
	if($turid){
		$sql_i = "delete from proinfantil.analisenovasturmasaprovacao where turid in (".implode(",",$turid).");";
		foreach($turid as $t){
			$anatipo = $rdn_turid[$t] ? "'".$rdn_turid[$t]."'" : "null";
			$sql_i.= "insert 
						into 
					proinfantil.analisenovasturmasaprovacao
						(turid,anatipo)
					values
						($t,$anatipo);";
		}
	}
	$db->executar($sql_i);
	
	if($ntaid){
		$sql = "update 
					proinfantil.novasturmasanalise 
				set 
					ntastatus = 'H' 
				where 
					ntaid = $ntaid 
				and 
					muncod = '$muncod';";
	}
	$sql.= "insert 
				into  
			proinfantil.novasturmasanalise 
				(muncod,ntamesesrepasse,ntapareceraprovacao,ntastatus,ntadata,usucpf) 
			values 
				('$muncod',1,'$ntapareceraprovacao','A',now(),'$usucpf');";
	
	$db->executar($sql);
	$db->commit();
	$db->sucesso("novasturmas/analise","");
}

function verificaDiligencia($muncod)
{
	//Pelo menos uma diligência parecer.
	global $db;
	
	$sql = "select ntaid from proinfantil.novasturmasanalise where muncod = '$muncod' and ntastatus = 'A'";
	$parecer = $db->pegaUm($sql);
	
	if(!$parecer){
		return "Favor informar o parecer.";
	}
	
	$sql = "select 
			count(turid) as qtde
		from 
			proinfantil.turma
		where
			muncod = '$muncod'
		and
			turstatus = 'A'";
	$total_turmas = $db->pegaUm($sql);
	$sql = "select 
			count(tur.turid) as qtde
		from 
			proinfantil.analisenovasturmasaprovacao ana
		inner join
			proinfantil.turma tur ON tur.turid = ana.turid
		where
			muncod = '$muncod'
			and ana.anastatus = 'A'
		and
			turstatus = 'A'
		and
			anatipo is not null";
	$total_turmas_analisadas = $db->pegaUm($sql);
	$sql = "select 
			count(tur.turid) as qtde
		from 
			proinfantil.analisenovasturmasaprovacao ana
		inner join
			proinfantil.turma tur ON tur.turid = ana.turid
		where
			muncod = '$muncod'
			and ana.anastatus = 'A'
		and
			turstatus = 'A'
		and
			anatipo = 'D' ";
	$total_diligencia = $db->pegaUm($sql);
	if($total_turmas != $total_turmas_analisadas){
		return "Favor selecionar a(s) ".($total_turmas-$total_turmas_analisadas)."restante(s).";
	}
	if($total_diligencia > 0){
		return true;
	}else{
		return "Não existe(m) turma(s) diligenciadas.";	
	}
}
function verificaAguardandoPagamento($muncod){
	//Todas as turmas aprovadas ou indeferidas e parecer.
	global $db;
	
	$sql = "SELECT 	ntaid 
			FROM 	proinfantil.novasturmasanalise 
			WHERE 	muncod = '{$muncod}' and ntastatus = 'A'";
	$parecer = $db->pegaUm($sql);
	
	if(!$parecer){
		return "Favor informar o parecer.";
	}	
	
	$sql = "SELECT	count(turid) as qtde
			FROM 	proinfantil.turma
			WHERE	muncod = '{$muncod}' and turstatus = 'A'";
	$total_turmas = $db->pegaUm($sql);
	
	$sql = "SELECT		count(tur.turid) as qtde
			from 		proinfantil.analisenovasturmasaprovacao ana
			inner join	proinfantil.turma tur ON tur.turid = ana.turid
			where		muncod = '$muncod' AND turstatus = 'A' and ana.anastatus = 'A' AND anatipo != 'D'";
	$total_turmas_analisadas = $db->pegaUm($sql);
	if($total_turmas != $total_turmas_analisadas){
		return "Existe(m) turma(s) diligenciada(s).";
	}else{
		return true;
	}
}

function mostrarTurmas(){
	global $db;
	
	$sql = "SELECT 		turid, turdsc, ttuid, turtipoestabelecimento
			FROM		proinfantil.turma
			WHERE 		muncod = '{$_SESSION['proinfantil']['muncod']}' 
			AND 		turstatus = 'A'
			AND 		turano <= '2012'
			ORDER BY	turid";
	return $db->carregar($sql);
}

function verificaAcessoPerfil($usuario,$estworkflow){
	$perfil = pegaPerfil($_SESSION['usucpf']);
	
	$acesso = array("cadastro" => "N", "analise" => "N");
	
	if( in_array(PERFIL_SUPER_USUARIO,$perfil) || ( in_array(PERFIL_ADMINISTRADOR,$perfil) && $estworkflow != WF_NOVASTURMAS_PAGAMENTO_EFETUADO ) ){
		$acesso = array("cadastro" => "S", "analise" => "S");
	}
	
	if(in_array(EQUIPE_MUNICIPAL,$perfil) || in_array(SECRETARIO_ESTADUAL,$perfil) && ($estworkflow == WF_NOVASTURMAS_EM_CADASTRAMENTO || $estworkflow == WF_NOVASTURMAS_EM_DILIGENCIA)){
		$acesso = array("cadastro" => "S", "analise" => "N");
	}
	
	if( (in_array(PERFIL_ANALISTA,$perfil) && $estworkflow == WF_NOVASTURMAS_EM_ANALISE) || in_array(PERFIL_COORDENADOR,$perfil) && $estworkflow == WF_NOVASTURMAS_EM_ANALISE ){
		$acesso = array("cadastro" => "N", "analise" => "S");
	}
	
	if(in_array(PERFIL_ANALISTA_PAGAMENTO,$perfil) || in_array(CONSULTA_GERAL,$perfil)){
		$acesso = array("cadastro" => "N", "analise" => "N");
	}
	
	return $acesso;
}

function listaMunicipiosSemLote( $post ){
	
	global $db;
	
	extract($post);
	
	$arrWhere = Array("doc.esdid = ". WF_NOVASTURMAS_EM_APROVADA);
	
	if($estuf){
		$arrWhere[] = "mun.estuf = '{$estuf}'";
	}
	
	if($mundescricao){
		$arrWhere[] = "mun.mundescricao ilike '%{$mundescricao}%'";
	}	
	
	$sql = "select muncod from proinfantil.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A'";
	$arrMunicipios = $db->carregarColuna($sql);
	
	if($arrMunicipios){
		$arrWhere[] = "mun.muncod in ('".implode("', '",$arrMunicipios)."')";
	}
	
	$sql = "SELECT distinct '<center><input type=\'checkbox\' class=\'check\' name=\"turid[]\" value=\"'|| tur.turid ||'\" /></center>' as acao,
		            mun.estuf,
		            mun.muncod,
		            mun.mundescricao,
					tur.turdsc,
					esd.esddsc
			FROM territorios.municipio mun
				inner join proinfantil.turma tur on tur.muncod = mun.muncod
			    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
			    inner join workflow.documento doc ON doc.docid = ntw.docid
			    inner join workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
				tur.turano = '{$_SESSION['exercicio']}'
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			ORDER BY 	mun.estuf, mun.mundescricao";
	//ver(simec_htmlentities($sql) );
	$cabecalho = array('Ação &nbsp;<input type=\'checkbox\' class="marcar-todos" marcar="check">', 'UF', 'IBGE', 'Município', 'Turma', 'Situação');
	$db->monta_lista_simples($sql, $cabecalho, 6000, 1, '', '100%', '', '', '', '', true);
}

function listaMunicipiosComLote( $post ){
	
	global $db;
	
	extract($post);
	
	$perfil = pegaPerfil($_SESSION['usucpf']);
	
	$acaoSQLDisabled = " '<input type=\"checkbox\" disabled checked name=\"docid[]\" value=\"'|| doc.docid ||'\" />' ";
	if( in_array(PERFIL_ANALISTA_PAGAMENTO, $perfil) || $db->testa_superuser() ){
		$acaoSQL = " '<input type=\"checkbox\" name=\"turid[]\" value=\"'|| tur.turid ||'\" />' ";
	} else {
		$acaoSQL = " '<input type=\"checkbox\" disabled checked name=\"docid[]\" value=\"'|| doc.docid ||'\" />' ";
	}
	
	if($estuf){
		$arWhere[] = "mun.estuf = '{$estuf}'";
	}
	
	if($mundescricao){
		$arWhere[] = "mun.mundescricao ilike '%{$mundescricao}%'";
	}
	
	if($lotid){
		$arWhere[] = "tur.lotid = $lotid";
	}
	
	$sql = "SELECT distinct 
					'<center>'||CASE WHEN doc.esdid = ".WF_NOVASTURMAS_AGUARDANDO_PAGAMENTO." THEN $acaoSQL ELSE $acaoSQLDisabled END||'</center>' as acao,
		            mun.estuf,
		            iue.iuecnpj,
		            mun.muncod,
		            mun.mundescricao,
		            tur.turdsc,
		            tur.lotid,
		            tur.turid,
		            tur.turano,
		            doc.esdid,
		            esd.esddsc
			FROM territorios.municipio mun
				inner join proinfantil.turma tur on tur.muncod = mun.muncod
			    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
			    inner join workflow.documento doc ON doc.docid = ntw.docid
			    inner join workflow.estadodocumento esd ON esd.esdid = doc.esdid
			    left join par.instrumentounidade iu
	                inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
                on iu.muncod = mun.muncod
			WHERE
				tur.turano= '{$_SESSION['exercicio']}'
				".($arWhere ? " and ".implode(" and ",$arWhere) : "")."
			ORDER BY 	mun.estuf, mun.mundescricao";
	
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	$arrRegistro = array();
	$arrEstado = array();
	$arrMuncod = array();
	foreach ($arrDados as $key => $v) {
				
		array_push($arrEstado, $v['esdid']);
		
		$arPost = array('turid' => $v['turid'], 'muncod' => $v['muncod'], 'exercicio' => $v['turano'], 'anoCenso' => ((int)$v['turano'] - 1));
		$obNovasTurmas = new NovasTurmas( $arPost );
		$aryRepasse = $obNovasTurmas->carregaRepassePorTurma();
		
		$valor_geral = 0;
		foreach($aryRepasse as $repasse){			
			if( $repasse['anatipo'] == 'A' ){
				$totalGeral = str_replace(".","", $repasse['valor_total']);
				$totalGeral = str_replace(",",".", $totalGeral);
			    
			    $valor_geral += $totalGeral; 
			}
		}
		
		//$arrMuncod[$v['muncod']][] = $v['turid'];
		
		//select valorrepasse from proinfantil.loteminutanovasturmas where muncod = '1200427' and lotid = 3
		$arrRegistro[$key] = array(
								'acao' 			=> $v['acao'],								
								'estuf' 		=> $v['estuf'],								
								'cnpj' 			=> $v['iuecnpj'],								
								'muncod' 		=> $v['muncod'],								
								'mundescricao' 	=> $v['mundescricao'],								
								'turdsc' 		=> $v['turdsc'],
								'turano' 		=> $v['turano'],
								'vlrTurma' 		=> $valor_geral,
								'esddsc' 		=> $v['esddsc'],								
								'lotid' 		=> $v['lotid'],								
								'rowspan' 		=> $db->pegaUm("select count(turid) from proinfantil.turma where lotid = $lotid and muncod = '{$v['muncod']}' and turano = '{$v['turano']}'")								
								);
	}
	//$cabecalho = array('Ação', "UF", 'CNPJ', "IBGE", 'Município', 'Turma', 'Valor Turma', 'Situação');
	//$db->monta_lista(array(), $cabecalho, 10000, 50, '', '', '', '');
	
	?>
	<table class="listagem" width="95%" border="1" align="center" cellspacing="0" cellpadding="2">
    <thead>
        <tr>
            <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
            	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ação</td>
            <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
            	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">UF</td>
            <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
            	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">CNPJ</td>
            <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
            	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">IBGE</td>
            <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
            	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Município</td>
            <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
            	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Turma</td>
            <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
            	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ano</td>
            <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
            	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Turma</td>
            <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
            	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Municípios</td>
            <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
            	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Situação</td>
        </tr>
    </thead>
    <tbody>
    <?
    if( $arrRegistro ){
    $muncod = '';
    $corAnterior = '';
    foreach ($arrRegistro as $key => $v) {
    	$key % 2 ? $cor = "#dedfde" : $cor = "";
    	
	    if( $muncod != $v['muncod'] ){
	    	if( empty($corAnterior) && $key > 0 ){
	    		$corAnterior = "#dedfde";
	    	} else{
	    		$corAnterior = "";
	    	}
	    }    	
    	?>
    	<tr bgcolor="<?=$corAnterior ?>" id="tr_<?=$key; ?>" onmouseout="this.bgColor='<?=$corAnterior?>';" onmouseover="this.bgColor='#ffffcc';">
		    <td align="left" title="Ação"><?=$v['acao']; ?></td>
		    <td align="left" title="UF"><?=$v['estuf']; ?></td>
		    <td align="right" title="CNPJ" style="color:#0066cc;"><?=$v['cnpj']; ?></td>
		    <td align="right" title="IBGE" style="color:#0066cc;"><?=$v['muncod']; ?></td>
		    <td align="left" title="Município"><?=$v['mundescricao']; ?></td>
		    <td align="left" title="Turma"><?=$v['turdsc']; ?></td>
		    <td align="left" title="Turma"><?=$v['turano']; ?></td>
		    <td align="right" title="Valor Turma" style="color:#0066cc;"><?=number_format($v['vlrTurma'], 2, ',', '.'); ?></td>
		    <?if( $muncod != $v['muncod'] ){
		    	/*if( empty($corAnterior) && $key > 0 ){
		    		$corAnterior = "#dedfde";
		    	} else{
		    		$corAnterior = "";
		    	}*/
		    	$vrlMunicipio = $db->pegaUm("select valorrepasse from proinfantil.loteminutanovasturmas where muncod = '{$v['muncod']}' and lotid = {$v['lotid']}");		    	
		    	?>
		    	<td align="right" title="Valor Municípios" style="color:#0066cc;" bgcolor="<?=(empty($corAnterior) ? '#F5F5F5' : $corAnterior); ?>" rowspan="<?=$v['rowspan']; ?>"><?=number_format($vrlMunicipio, 2, ',', '.'); ?></td>
		    <?
			} ?>
		    <td align="left" title="Situação"><?=$v['esddsc']; ?></td>
		</tr>
	<?
		$muncod = $v['muncod'];
	} ?>
    </tbody>
    </table>
    
    <table class="listagem" width="95%" border="0" align="center" cellspacing="0" cellpadding="2">
    <tbody>
        <tr bgcolor="#ffffff">
            <td><b>Total de Registros: <?=sizeof($arrRegistro); ?></b></td>
        </tr>
    </tbody>
	</table>
	<?} else { ?>
	<table class="listagem" width="95%" border="0" align="center" cellspacing="0" cellpadding="2">
    <tbody>
        <tr>
            <td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
        </tr>
    </tbody>
	</table>
	<?
	}
	
	if( ( in_array(PERFIL_ANALISTA_PAGAMENTO, $perfil) || $db->testa_superuser()) && in_array( WF_NOVASTURMAS_AGUARDANDO_PAGAMENTO , $arrEstado) ){
?>	
	<center>
		<input type="button" value="Confirmar Pagamento por Turma" class="tramitaDocid"/>
	</center>
<?php 
	}
}

function geraLoteNovasTurmas( $request ){	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	extract($request);
	
	if( $turid[0] != '' ){
		$dataPortaria = explode('/', $lotdataportaria);
		$dia = $dataPortaria[0];
		$mes = $dataPortaria[1];
		$ano = $dataPortaria[2];
		$mes = mes_extenso($mes);
	
		$texto = '<p style="text-align: justify;"><strong>PORTARIA N&ordm;&nbsp;&nbsp;'.$lotnumportaria.', &nbsp;&nbsp;&nbsp;&nbsp;DE&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dia.'&nbsp;&nbsp;&nbsp;&nbsp; DE &nbsp;&nbsp;&nbsp;&nbsp;'.$mes.'&nbsp;&nbsp;&nbsp;&nbsp;DE '.$ano.'.</strong></p>
<p style="text-align: justify;">&nbsp;</p>
<p style="padding-left: 300px; text-align: justify;">Autoriza o Fundo Nacional de Desenvolvimento da Educa&ccedil;&atilde;o - FNDE a realizar a transfer&ecirc;ncia de recurso financeiro para a manuten&ccedil;&atilde;o de novas matr&iacute;culas em novas turmas de educa&ccedil;&atilde;o infantil aos munic&iacute;pios e ao Distrito Federal que pleitearam e est&atilde;o aptos para pagamento, conforme Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 16, de 16 de maio de 2013.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;"><strong>O SECRET&Aacute;RIO DE EDUCA&Ccedil;&Atilde;O B&Aacute;SICA</strong>, no uso das atribui&ccedil;&otilde;es, resolve:</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 1&ordm; Divulgar os munic&iacute;pios e o Distrito Federal que est&atilde;o aptos a receber o pagamento do recurso financeiro para a manuten&ccedil;&atilde;o de novas matr&iacute;culas em novas turmas de educa&ccedil;&atilde;o infantil oferecidas em estabelecimentos educacionais p&uacute;blicos ou em institui&ccedil;&otilde;es comunit&aacute;rias, confessionais ou filantr&oacute;picas sem fins lucrativos conveniadas com o poder p&uacute;blico que tenham cadastradas novas matr&iacute;culas em novas turmas e que ainda n&atilde;o foram contempladas com recursos do Fundo de Manuten&ccedil;&atilde;o e Desenvolvimento da Educa&ccedil;&atilde;o B&aacute;sica e de Valoriza&ccedil;&atilde;o dos Profissionais da Educa&ccedil;&atilde;o (Fundeb), de que trata a Lei n&ordm; 12.722 de 3 de outubro de 2012, e conforme informa&ccedil;&otilde;es declaradas pelos munic&iacute;pios e Distrito Federal no SIMEC &ndash; M&oacute;dulo E.I. Manuten&ccedil;&atilde;o &ndash; Novas Turmas de Educa&ccedil;&atilde;o Infantil.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 2&ordm; Autorizar o FNDE/MEC a realizar a transfer&ecirc;ncia de recursos financeiros aos munic&iacute;pios e Distrito Federal para a manuten&ccedil;&atilde;o de novas matr&iacute;culas em novas turmas de educa&ccedil;&atilde;o infantil, conforme destinat&aacute;rios e valores constantes da listagem anexa.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 3&ordm; Esta Portaria entra em vigor na data de sua publica&ccedil;&atilde;o.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: center;"><strong>MANUEL FERNANDO PALÁCIOS DA CUNHA E MELO</strong></p>
<p style="text-align: center;">Secret&aacute;rio da Educa&ccedil;&atilde;o B&aacute;sica</p>';
		
		$textoSQL = simec_htmlspecialchars($texto, ENT_QUOTES);
		
		$sql = "INSERT INTO proinfantil.lotenovasturmas(usucpf, lotnumportaria, lotdataportaria, lotminutaportaria) 
				VALUES ('".$_SESSION['usucpf']."', {$lotnumportaria}, '".formata_data_sql($lotdataportaria)."', '{$textoSQL}') RETURNING lotid;";
		$lotid = $db->pegaUm($sql);
		
		$html = $texto.'
		<p style="page-break-before:always"><!-- pagebreak --></p>
		<table align="center" class="listagem" border="1" width="100%" cellSpacing="1" cellPadding=3 >
			<tr>
				<th colspan="8" style="text-align: center;">ANEXO</th>
			</tr>
			<tr>
				<th rowspan="2" width="05%"><b>UF</b></th>
				<th rowspan="2" width="25%" style="text-align: center;"><b>Municípo</b></th>
				<th rowspan="2" width="05%" style="text-align: center;"><b>Código IBGE</b></th>
				<th colspan="4" width="60%" style="text-align: justify;"><b>Quantidade de novas matrículas em novas turmas de educação infantil, declaradas pelos Municípios e o Distrito Federal, em   estabelecimentos públicos e /ou conveniados com o poder público</b></th>
				<th rowspan="2" width="05%" style="text-align: center;"><b>Valor do Repasse</b></th>
			</tr>
			<tr>
				<th style="text-align: center;"><b>Creche Púb/Conv Parcial</b></th>
				<th style="text-align: center;"><b>Creche Púb/Conv Integral</b></th>
				
				<th style="text-align: center;"><b>Pré-Escola Púb/Conv Parcial</b></th>
				<th style="text-align: center;"><b>Pré-Escola Púb/Conv Integral</b></th>
			</tr>';
			
		$sql = "SELECT
				    mun.estuf,
				    mun.muncod,
				    mun.mundescricao,
				    count(tur.turid) as turma
				FROM proinfantil.turma tur
				    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
				    inner join workflow.documento doc on doc.docid = ntw.docid
					inner join territorios.municipio mun on mun.muncod = tur.muncod
				 WHERE  
				 	doc.esdid = ".WF_NOVASTURMAS_EM_APROVADA."
				 	AND tur.turstatus = 'A'
				 	and tur.turid in (".implode(', ', $request['turid']).")
				 	and tur.turano = '{$_SESSION['exercicio']}'
				 group by
				 	mun.estuf,
				    mun.muncod,
				    mun.mundescricao
				 order by
				 	mun.estuf, mun.mundescricao";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		foreach ($arrDados as $v) {
			$sql = "SELECT ntmmid, muncod, ntmmstatus, ntmmano, ntmmmes, ntmmqtdmatriculacrecheparcialpublica, ntmmqtdmatriculacrecheintegralpublica,
			  			ntmmqtdmatriculapreescolaparcialpublica, ntmmqtdmatriculapreescolaintegralpublica, ntmmqtdmatriculacrecheparcialconveniada,
			  			ntmmqtdmatriculacrecheintegralconveniada, ntmmqtdmatriculapreescolaparcialconveniada, ntmmqtdmatriculapreescolaintegralconveniada,
			  			ntmmqtdturmacrechepublica, ntmmqtdturmapreescolapublica, ntmmqtdturmaunificadapublica, ntmmqtdturmacrecheconveniada,
			  			ntmmqtdturmapreescolaconveniada, ntmmqtdturmaunificadaconveniada
					FROM proinfantil.novasturmasdadosmunicipiospormes WHERE ntmmstatus = 'A' and muncod = '{$v['muncod']}' and ntmmano = '{$_SESSION['exercicio']}'
					order by ntmmmes desc";
					
			$arrMatricula = $db->carregar($sql);
			$arrMatricula = $arrMatricula ? $arrMatricula : array();
			
			$arrMat = matriculaCalculoDadosMunicipio( $arrMatricula );
			
			$sql = "select tur.turid, ntw.docid, tur.muncod from proinfantil.turma tur
												inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
												inner join workflow.documento doc on doc.docid = ntw.docid 
											where 
												tur.turid in (".implode(', ', $request['turid']).")
												and tur.muncod = '{$v['muncod']}'
												and doc.esdid = ".WF_NOVASTURMAS_EM_APROVADA;
			
			$arTurid = $db->carregar($sql);
			
			$valor_geral 						= 0;
			$qtdCrechepublicaparcial 			= 0;
			$qtdCrechepublicaintegral 			= 0;
			$qtdCrecheconveniadaparcial 		= 0;
			$qtdCrecheconveniadaintegral 		= 0;			
			$qtdPreescolapublicaparcial 		= 0;
			$qtdPreescolapublicaintegral 		= 0;
			$qtdPreescolaconveniadaparcial		= 0;
			$qtdPreescolaconveniadaintegral 	= 0;
			
			$arTurmas = array();
			foreach ($arTurid as $arTur) {
				$aguardando_pagamento = '2002';
				
				$arPost = array('turid' => $arTur['turid'], 'muncod' => $v['muncod']);
				$obNovasTurmas = new NovasTurmas( $arPost );
				$aryRepasse = $obNovasTurmas->carregaRepassePorTurma();
				
				/*timid => 6 - Creche, 		7 - Pré-escola 	*/
				/*tatid => 1 - Integral, 	2 - Parcial 	*/
				/*tirid => 1 - Pública, 	2 - Privada 	*/
				#Creche
				$arParam = array('muncod' => $v['muncod'], 'ano' => $_SESSION['exercicio'], 'timid' => 6, 'tatid' => 2, 'tirid' => 1, 'turma' => $arTur['turid']);
				$qtdCrechepublicaparcial += calculaQtdMatriculaAprovada( $arParam );
				
				$arParam = array('muncod' => $v['muncod'], 'ano' => $_SESSION['exercicio'], 'timid' => 6, 'tatid' => 1, 'tirid' => 1, 'turma' => $arTur['turid']);
				$qtdCrechepublicaintegral += calculaQtdMatriculaAprovada( $arParam );
				
				$arParam = array('muncod' => $v['muncod'], 'ano' => $_SESSION['exercicio'], 'timid' => 6, 'tatid' => 2, 'tirid' => 2, 'turma' => $arTur['turid']);
				$qtdCrecheconveniadaparcial += calculaQtdMatriculaAprovada( $arParam );
				
				$arParam = array('muncod' => $v['muncod'], 'ano' => $_SESSION['exercicio'], 'timid' => 6, 'tatid' => 1, 'tirid' => 2, 'turma' => $arTur['turid']);
				$qtdCrecheconveniadaintegral += calculaQtdMatriculaAprovada( $arParam );
				
				#Pre-Escola
				$arParam = array('muncod' => $v['muncod'], 'ano' => $_SESSION['exercicio'], 'timid' => 7, 'tatid' => 2, 'tirid' => 1, 'turma' => $arTur['turid']);
				$qtdPreescolapublicaparcial += calculaQtdMatriculaAprovada( $arParam );
				
				$arParam = array('muncod' => $v['muncod'], 'ano' => $_SESSION['exercicio'], 'timid' => 7, 'tatid' => 1, 'tirid' => 1, 'turma' => $arTur['turid']);
				$qtdPreescolapublicaintegral += calculaQtdMatriculaAprovada( $arParam );
				
				$arParam = array('muncod' => $v['muncod'], 'ano' => $_SESSION['exercicio'], 'timid' => 7, 'tatid' => 2, 'tirid' => 2, 'turma' => $arTur['turid']);
				$qtdPreescolaconveniadaparcial += calculaQtdMatriculaAprovada( $arParam );
				
				$arParam = array('muncod' => $v['muncod'], 'ano' => $_SESSION['exercicio'], 'timid' => 7, 'tatid' => 1, 'tirid' => 2, 'turma' => $arTur['turid']);
				$qtdPreescolaconveniadaintegral += calculaQtdMatriculaAprovada( $arParam );
				
				foreach($aryRepasse as $repasse){
					
					if( $repasse['anatipo'] == 'A' ){
						$totalGeral = str_replace(".","", $repasse['valor_total']);
						$totalGeral = str_replace(",",".", $totalGeral);
					    
					    $valor_geral += $totalGeral; 
					}
				}
				
				wf_alterarEstado( $arTur['docid'], $aguardando_pagamento, 'Tramitação em Lote', array( 'turid' => $arTur['turid'], 'muncod' => $v['muncod']) );
				
				$sql = "UPDATE proinfantil.turma SET lotid = $lotid WHERE turid = '{$arTur['turid']}'";
				$db->executar($sql);
				
				array_push($arTurmas, $arTur['turid']);
			}
			
			$arrLote[]= array(
							'municipio' 					=> $v['mundescricao'],
							'ibge' 							=> $v['muncod'],
							'estuf' 						=> $v['estuf'],
							'crechepublicaparcial'			=> (int)$qtdCrechepublicaparcial,
							'crechepublicaintegral' 		=> (int)$qtdCrechepublicaintegral,
							'crecheconveniadaparcial' 		=> (int)$qtdCrecheconveniadaparcial,
							'crecheconveniadaintegral' 		=> (int)$qtdCrecheconveniadaintegral,
			
							'preescolapublicaparcial'		=> (int)$qtdPreescolapublicaparcial,
							'preescolapublicaintegral' 		=> (int)$qtdPreescolapublicaintegral,
							'preescolaconveniadaparcial'	=> (int)$qtdPreescolaconveniadaparcial,
							'preescolaconveniadaintegral' 	=> (int)$qtdPreescolaconveniadaintegral,
							'valorRepasse' 					=> (float)$valor_geral,
							'turmas'	 					=> $arTurmas
						);
			
			/*$arrLote[]= array(
							'municipio' 					=> $v['mundescricao'],
							'ibge' 							=> $v['muncod'],
							'estuf' 						=> $v['estuf'],
							'crechepublicaparcial'			=> (int)$arrMat['crechepublicaparcial'],
							'crechepublicaintegral' 		=> (int)$arrMat['crechepublicaintegral'],
							'crecheconveniadaparcial' 		=> (int)$arrMat['crecheconveniadaparcial'],
							'crecheconveniadaintegral' 		=> (int)$arrMat['crecheconveniadaintegral'],
			
							'preescolapublicaparcial'		=> (int)$arrMat['preescolapublicaparcial'],
							'preescolapublicaintegral' 		=> (int)$arrMat['preescolapublicaintegral'],
							'preescolaconveniadaparcial'	=> (int)$arrMat['preescolaconveniadaparcial'],
							'preescolaconveniadaintegral' 	=> (int)$arrMat['preescolaconveniadaintegral'],
							'valorRepasse' 					=> (float)$valor_geral
						);*/
		}
		//ver($arrLote,d);
		foreach ($arrLote as $v) {
			extract($v);
			$valorTotal = ($valorRepasse ? number_format($valorRepasse,2,",",".") : '0,00');
				
			$html.='
			<tr>
				<td>'.$estuf.'</td>
				<td style="text-align: left;">'.$municipio.'</td>
				<td style="text-align: center;">'.$ibge.'</td>
				<td style="text-align: center;">'.((int)$crechepublicaparcial + (int)$crecheconveniadaparcial).'</td>
				<td style="text-align: center;">'.((int)$crechepublicaintegral + (int)$crecheconveniadaintegral).'</td>
				
				<td style="text-align: center;">'.((int)$preescolapublicaparcial + (int)$preescolaconveniadaparcial).'</td>
				<td style="text-align: center;">'.((int)$preescolapublicaintegral + (int)$preescolaconveniadaintegral).'</td>
				<td style="text-align: right;">'.$valorTotal.'</td>
			</tr>';
			
			$valorTotal = str_replace(".","", $valorTotal);
			$valorTotal = str_replace(",",".", $valorTotal);
			
			$sql = "INSERT INTO proinfantil.loteminutanovasturmas(lotid, estuf, muncod, crechepublicaparcial, crechepublicaintegral, crecheconveniadaparcial, crecheconveniadaintegral,
  						preescolapublicaparcial, preescolapublicaintegral, preescolaconveniadaparcial, preescolaconveniadaintegral, valorrepasse)  
					VALUES ({$lotid}, '{$estuf}', '{$ibge}', ".(int)$crechepublicaparcial.", ".(int)$crechepublicaintegral.", ".(int)$crecheconveniadaparcial.", ".(int)$crecheconveniadaintegral.", 
							".(int)$preescolapublicaparcial.", ".(int)$preescolapublicaintegral.", ".(int)$preescolaconveniadaparcial.", ".(int)$preescolaconveniadaintegral.", ".$valorTotal.")";
			$db->executar($sql);
		}
		
		$html.= '</table>';

		include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
		ob_clean();
			
		$nomeArquivo 		= 'minuta_repasse_'.date('Y-m-d').'_lote_'.$lotid;
		$diretorio		 	= APPRAIZ . 'arquivos/proinfantil/minutanovasturmas';
		$diretorioArquivo 	= APPRAIZ . 'arquivos/proinfantil/minutanovasturmas/'.$nomeArquivo.'.pdf';
		
		if( !is_dir($diretorio) ){
			mkdir($diretorio, 0777);
		}
		
		$http = new RequestHttp();
		$html = utf8_encode($html);
		$response = $http->toPdf( $html );
	
		$fp = fopen($diretorioArquivo, "w");
		if ($fp) {
		  stream_set_write_buffer($fp, 0);
		  fwrite($fp, $response);
		  fclose($fp);
		}
		
		$sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
				VALUES( '".$nomeArquivo."',
						'pdf',
						'".$nomeArquivo."',
						'application/pdf',
						'".filesize($diretorioArquivo)."',
						'".date('Y-m-d')."',
						'".date('H:i:s')."',
						'".$_SESSION["usucpf"]."',
						{$_SESSION['sisid']},
						'A') RETURNING arqid";
		
		$arqid = $db->pegaUm($sql);

		$sql = "UPDATE proinfantil.lotenovasturmas SET arqid = $arqid, lotdsc = 'Lote: '||lotid WHERE lotid = $lotid";
		$db->executar($sql);
		$db->commit();
	}
	echo "<script>
			alert('Lote criado com sucesso.');
			window.location = 'proinfantil.php?modulo=novasturmas/pagamentoLote&acao=A';
		  </script>";
}

function confirmaPagamentoLote( $request ){
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	if( $request['lotid'] ){
		if( $request['turid'][0] ) $filtroTurma = " and tur.turid in (".implode(', ', $request['turid']).")";
		
		$sql = "select tur.turid, ntw.docid, tur.muncod from proinfantil.turma tur
										inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
										inner join workflow.documento doc on doc.docid = ntw.docid 
									where tur.lotid = ".$request['lotid']."
										and doc.esdid = ".WF_NOVASTURMAS_AGUARDANDO_PAGAMENTO." ".$filtroTurma;
		$arrTurma = $db->carregar($sql);
		
		$arrTurma = $arrTurma ? $arrTurma : array();
		
		foreach( $arrTurma as $turma ){
			wf_alterarEstado( $turma['docid'], NOVASTURMAS_ENCAMINHAR_PAGAMENTO_EFETUADO, 'Tramitação em Lote - Confirmar Pagamento', array( ) );
		}
		
		echo "<script>
				alert('Pagamento confirmado.');
				window.location = 'proinfantil.php?modulo=novasturmas/pagamentoLote&acao=A';
			  </script>";
	} else {
		echo "<script>
				alert('lote não encontrado.');
				window.location = 'proinfantil.php?modulo=novasturmas/pagamentoLote&acao=A';
			  </script>";
	}
}

function confirmaPagamentoTurma( $request ){
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';	

	if( $request['turid'] ){
		$sql = "select tur.turid, ntw.docid, tur.muncod from proinfantil.turma tur
										inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
										inner join workflow.documento doc on doc.docid = ntw.docid 
									where tur.turid in (".implode(', ', $request['turid']).")
										and doc.esdid = ".WF_NOVASTURMAS_AGUARDANDO_PAGAMENTO;
		$arrTurma = $db->carregar($sql);
		$arrTurma = $arrTurma ? $arrTurma : array();
		$retorno == false;
		foreach ($arrTurma as $turma) {			
			$retorno = wf_alterarEstado( $turma['docid'], NOVASTURMAS_ENCAMINHAR_PAGAMENTO_EFETUADO, 'Tramitação em Lote', array( 'muncod' => $turma['muncod']) );
		}
		
		if( $retorno == true ){
			echo "<script>
					alert('Pagamento confirmado.');
					window.location = 'proinfantil.php?modulo=novasturmas/pagamentoLote&acao=A';
				  </script>";
		} else {
			echo "<script>
				alert('Não foi possivel confirma o Pagamento.');
				window.location = 'proinfantil.php?modulo=novasturmas/pagamentoLote&acao=A';
			  </script>";
		}
	} else {
		echo "<script>
				alert('Turma não encontrada.');
				window.location = 'proinfantil.php?modulo=novasturmas/pagamentoLote&acao=A';
			  </script>";
	}
}

function dataCenso( $datainicio ){
	global $db;
		
	switch ( substr($datainicio, 0, 4) ){
		case '2011': $datacenso = '2011-05-25';
			break;
		case '2012': $datacenso = '2012-05-30';
		  	break;
		case '2013': $datacenso = '2013-05-29';
		  	break;
		case '2014': $datacenso = '2014-05-28';
		 	break;
		case '2015': $datacenso = '2015-05-27';
		  	break;
		case '2016': $datacenso = '2016-05-25';
		 	break;
		case '2017': $datacenso = '2017-05-31';
		  	break;
		case '2018': $datacenso = '2018-05-30';
			break;
		case '2019': $datacenso = '2019-05-29';
			  break;
		case '2020': $datacenso = '2020-05-27';
			  break;
	}
	return $datacenso;
}

function calculaDadosRepasse( $arrDados = array() ){
	global $db;
	
	$datacenso = dataCenso( $arrDados['dtinicio'] );
			
	$anoatend = substr($arrDados['dtinicio'], 0, 4);
	$anoenvio = substr($arrDados['dtanalise'], 0, 4);
	
	$mesatend = substr( $arrDados['dtinicio'] ,5,2);
	$mesenvio = substr( $arrDados['dtanalise'],5,2);
	
	$dataini = str_replace("-","", $arrDados['dtinicio'] );
	$datacenso = str_replace("-","", $datacenso );
				
	$qtdlimite = 0;
	
	if(strtotime( $dataini ) > strtotime( $datacenso )){
		$qtdlimite = '18';
	} elseif(strtotime( $dataini ) <= strtotime( $datacenso )){
		$qtdlimite = '12';
	}
				
	if($qtdlimite == '12'){
		if( $anoenvio > $anoatend){
			$saldolimite = 0;
		} else {
			$saldolimite = (12 - ($mesenvio - 1));
		}
		
		$valor_repasse = (($arrDados['pvevalor'] / 12) * (12 - ($mesenvio - 1) ) * $arrDados['total_alunos']);
	}

	if($qtdlimite == '18'){
		if( $anoenvio > ((int)$anoatend+1) ){
			$saldolimite = 0;
		} else if( $anoenvio > ((int)$anoatend) ){
			$saldolimite = (12 - ($mesenvio - 1));
		} else {
			$saldolimite = (12 - ($mesenvio - 1))+ 12;
		}
		
		$valor_repasse = (($arrDados['pvevalor'] / 12) * ((12 - ($mesenvio - 1) ) + 12) * $arrDados['total_alunos']);
	}
	
	#De acordo com a resolução nº 28/2012, quem iniciar turmas em nov e dez só receberá no próximo ano
	if( ($mesatend == 11 || $mesatend == 12) && ((int)$anoenvio <= (int)$anoatend) ){		
		if( $mesenvio == 11 || $mesenvio == 12 ){
			$saldolimite = 12;
		} else {
			$saldolimite = ( (12 - $mesenvio) + 1);
		}
	}

	if( (int)$qtdlimite < (int)$saldolimite ){
		$saldolimite = (int)$qtdlimite;
	}
		    
	if( empty($arrDados['anatipo']) ){
		$valor_total = '0.00';
	} else {
		$valor_total = $valor_repasse;
	}
	
	$arrRepasse = array(
					'qtdlimite' => $qtdlimite,
					'saldolimite' => $saldolimite,
					'valor_repasse' => $valor_repasse,
					'valor_total' => $valor_total,
					'datacenso' => $datacenso,
					);
	return $arrRepasse;
}

function matriculaCalculoDadosMunicipio( $arrDados ){
	global $db;
	
	$arrMatric = array();
	$totCrecheIntegralPublica = 0;
	$totCrecheParcialPublica = 0;
	$totCrecheParcialConveniada = 0;
	$totCrecheIntegralConveniada = 0;
	$totPreEscolaParcialPublica = 0;
	$totPreEscolaIntegralPublica = 0;
	$totPreEscolaParcialConveniada = 0;
	$totPreEscolaIntegralConveniada = 0;
	
	foreach ($arrDados as $v) {
		
		$arrPost = array(
				'muncod' => $v['muncod'],
				'ntmmmes' => $v['ntmmmes'],
				); 
	
		$obMatricula = new NovasTurmas($arrPost);
		$arrMatricula = $obMatricula->carregaDadosMatriculaMaiorPorMes();
			
		$crecheIntegralPublica 			= ((int)$v['ntmmqtdmatriculacrecheintegralpublica'] - (int)$arrMatricula['ntmmqtdmatriculacrecheintegralpublica']);
		$crecheIntegralConveniada 		= ((int)$v['ntmmqtdmatriculacrecheintegralconveniada'] - (int)$arrMatricula['ntmmqtdmatriculacrecheintegralconveniada']);
		$crecheParcialPublica 			= ((int)$v['ntmmqtdmatriculacrecheparcialpublica'] - (int)$arrMatricula['ntmmqtdmatriculacrecheparcialpublica']);	
		$crecheParcialConveniada 		= ((int)$v['ntmmqtdmatriculacrecheparcialconveniada'] - (int)$arrMatricula['ntmmqtdmatriculacrecheparcialconveniada']);	
		$PreEscolaIntegralPublica 		= ((int)$v['ntmmqtdmatriculapreescolaintegralpublica'] - (int)$arrMatricula['ntmmqtdmatriculapreescolaintegralpublica']);	
		$PreEscolaIntegralConveniada 	= ((int)$v['ntmmqtdmatriculapreescolaintegralconveniada'] - (int)$arrMatricula['ntmmqtdmatriculapreescolaintegralconveniada']);	
		$PreEscolaParcialPublica 		= ((int)$v['ntmmqtdmatriculapreescolaparcialpublica'] - (int)$arrMatricula['ntmmqtdmatriculapreescolaparcialpublica']);
		$PreEscolaParcialConveniada 	= ((int)$v['ntmmqtdmatriculapreescolaparcialconveniada'] - (int)$arrMatricula['ntmmqtdmatriculapreescolaparcialconveniada']);
		
		$totCrecheIntegralPublica 		+= ($crecheIntegralPublica > 0 ? $crecheIntegralPublica : 0);
		$totCrecheParcialPublica 		+= ($crecheParcialPublica > 0 ? $crecheParcialPublica : 0);
		$totCrecheParcialConveniada 	+= ($crecheParcialConveniada > 0 ? $crecheParcialConveniada : 0);
		$totCrecheIntegralConveniada 	+= ($crecheIntegralConveniada > 0 ? $crecheIntegralConveniada : 0);
		$totPreEscolaParcialPublica 	+= ($PreEscolaParcialPublica > 0 ? $PreEscolaParcialPublica : 0);
		$totPreEscolaIntegralPublica 	+= ($PreEscolaIntegralPublica > 0 ? $PreEscolaIntegralPublica : 0);
		$totPreEscolaParcialConveniada 	+= ($PreEscolaParcialConveniada > 0 ? $PreEscolaParcialConveniada : 0);
		$totPreEscolaIntegralConveniada += ($PreEscolaIntegralConveniada > 0 ? $PreEscolaIntegralConveniada : 0);
		
	}
	$arrMatric = array(
							'crechepublicaparcial' 			=> $totCrecheParcialPublica,
							'crechepublicaintegral' 		=> $totCrecheIntegralPublica,
							'crecheconveniadaparcial' 		=> $totCrecheParcialConveniada,
							'crecheconveniadaintegral' 		=> $totCrecheIntegralConveniada,
							'preescolapublicaparcial' 		=> $totPreEscolaParcialPublica,
							'preescolapublicaintegral' 		=> $totPreEscolaIntegralPublica,
							'preescolaconveniadaparcial' 	=> $totPreEscolaParcialConveniada,
							'preescolaconveniadaintegral'	=> $totPreEscolaIntegralConveniada,
							);
	return $arrMatric;
}

function totalAmpliacaoRedeMes( $mes, $muncod ){
	global $db;
	
	$arrRetorno = array();
	$arrPost = array(
			'muncod' => $muncod,
			'ntmmmes' => ((int)$mes)
	);
	$obNovasTurmas = new NovasTurmas( $arrPost );

	$arrMatriculaMaior 		= $obNovasTurmas->carregaDadosMatriculaMaiorPorMes();
	$arrTurmaMaior 			= $obNovasTurmas->carregaDadosTurmaMaiorPorMes();
	$arrMatTurmaCadastrada 	= $obNovasTurmas->carregaDadosMatriculaTurmaMes();

	#Quantidade de Matricula
	$totCrecPublicaC = (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheintegralpublica'] + (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheparcialpublica'];
	$totCrecConveniC = (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheintegralconveniada'] + (int)$arrMatriculaMaior['ntmmqtdmatriculacrecheparcialconveniada'];

	$totPreEPublicaC = (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaintegralpublica'] + (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaparcialpublica'];
	$totPreEConveniC = (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaintegralconveniada'] + (int)$arrMatriculaMaior['ntmmqtdmatriculapreescolaparcialconveniada'];

	$totCrecPublicaM = (int)$arrMatTurmaCadastrada['ntmmqtdmatriculacrecheintegralpublica'] + (int)$arrMatTurmaCadastrada['ntmmqtdmatriculacrecheparcialpublica'];
	$totCrecConveniM = (int)$arrMatTurmaCadastrada['ntmmqtdmatriculacrecheintegralconveniada'] + (int)$arrMatTurmaCadastrada['ntmmqtdmatriculacrecheparcialconveniada'];

	$totPreEPublicaM = (int)$arrMatTurmaCadastrada['ntmmqtdmatriculapreescolaintegralpublica'] + (int)$arrMatTurmaCadastrada['ntmmqtdmatriculapreescolaparcialpublica'];
	$totPreEConveniM = (int)$arrMatTurmaCadastrada['ntmmqtdmatriculapreescolaintegralconveniada'] + (int)$arrMatTurmaCadastrada['ntmmqtdmatriculapreescolaparcialconveniada'];

	#quantidade de Turmas
	$qtdTurmaCenso = (int)$arrTurmaMaior['ntmmqtdturmacrechepublica'] + (int)$arrTurmaMaior['ntmmqtdturmacrecheconveniada'] + (int)$arrTurmaMaior['ntmmqtdturmapreescolapublica'] +
						(int)$arrTurmaMaior['ntmmqtdturmapreescolaconveniada'] + (int)$arrTurmaMaior['ntmmqtdturmaunificadapublica'] + (int)$arrTurmaMaior['ntmmqtdturmaunificadaconveniada'];

	$qtdTurmaInformada = (int)$arrMatTurmaCadastrada['ntmmqtdturmacrecheconveniada'] + (int)$arrMatTurmaCadastrada['ntmmqtdturmapreescolapublica'] + (int)$arrMatTurmaCadastrada['ntmmqtdturmapreescolaconveniada'] +
						(int)$arrMatTurmaCadastrada['ntmmqtdturmaunificadapublica'] + (int)$arrMatTurmaCadastrada['ntmmqtdturmaunificadaconveniada'] + (int)$arrMatTurmaCadastrada['ntmmqtdturmacrechepublica'];
	
	$totGeralCenso 		= (int)$totCrecPublicaC + (int)$totCrecConveniC + (int)$totPreEPublicaC + (int)$totPreEConveniC;
	$totGeralMatricula 	= (int)$totCrecPublicaM + (int)$totCrecConveniM + (int)$totPreEPublicaM + (int)$totPreEConveniM;

	$totTurmaCadastro = ((int)$qtdTurmaInformada - (int)$qtdTurmaCenso);
	//$totalTurmaCadastrada = $db->pegaUm("select count(turid) from proinfantil.turma where muncod = '{$_REQUEST['muncod']}'");
	
	/*array_push($arrRetorno, array(
								'mes' => $mes, 
								'qtdenviada' => ($QtdTurmaEnviada ? $QtdTurmaEnviada : 0), 
								'qtdselecionada' => $arMes['total'], 
								'qtdrestante' => ((int)$totTurmaCadastro - (int)$QtdTurmaEnviada)));*/
	
	return $totTurmaCadastro;
}

function verificaPermissaoAcesso(){
	global $db;
	
	$perfis = pegaPerfil($_SESSION['usucpf']);
	
	if( in_array(PERFIL_SUPER_USUARIO, $perfis) ){
		$arrAcesso['proinfantil.php?modulo=novasturmas/informarMatTurmaMunicipio'] = array('text' => 'S', 'button' => 'S');
	} elseif( in_array(PERFIL_ADMINISTRADOR, $perfis) ){
		$arrAcesso['proinfantil.php?modulo=novasturmas/informarMatTurmaMunicipio'] = array('text' => 'S', 'button' => 'S');
	} elseif( in_array(EQUIPE_MUNICIPAL, $perfis) ){
		$arrAcesso['proinfantil.php?modulo=novasturmas/termoNovasTurmas'] = array('text' => 'N', 'button' => 'S', 'radio' => 'N', 'select' => 'N', 'textarea' => 'N');
		$arrAcesso['proinfantil.php?modulo=novasturmas/informarMatTurmaMunicipio'] = array('text' => 'S', 'button' => 'S', 'radio' => 'S');
		$arrAcesso['proinfantil.php?modulo=novasturmas/popupCadastrarTurmas'] = array('text' => 'S', 'button' => 'S', 'radio' => 'S', 'select' => 'S', 'textarea' => 'S');
	} elseif( in_array(PERFIL_ANALISTA, $perfis) ){
		$arrAcesso['proinfantil.php?modulo=novasturmas/informarMatTurmaMunicipio'] = array('text' => 'N', 'button' => 'N', 'radio' => 'N', 'select' => 'N');
		$arrAcesso['proinfantil.php?modulo=novasturmas/popupCadastrarTurmas'] = array('text' => 'N', 'button' => 'N', 'radio' => 'N', 'select' => 'N', 'textarea' => 'N');
		$arrAcesso['proinfantil.php?modulo=novasturmas/termoNovasTurmas'] = array('text' => 'N', 'button' => 'N', 'radio' => 'N', 'select' => 'N', 'textarea' => 'N');
	} else {
		$arrAcesso['proinfantil.php?modulo=novasturmas/informarMatTurmaMunicipio'] = array('text' => 'N', 'button' => 'N', 'radio' => 'N', 'select' => 'N');
		$arrAcesso['proinfantil.php?modulo=novasturmas/popupCadastrarTurmas'] = array('text' => 'N', 'button' => 'N', 'radio' => 'N', 'select' => 'N', 'textarea' => 'N');
		$arrAcesso['proinfantil.php?modulo=novasturmas/termoNovasTurmas'] = array('text' => 'N', 'button' => 'N', 'radio' => 'N', 'select' => 'N', 'textarea' => 'N');
	}
	#Trava o sistema para todos perfis, caso o ano de exercicio seja menor que o ano atual.
	if( $_SESSION['exercicio'] < date('Y') ){
		$arrAcesso['proinfantil.php?modulo=novasturmas/informarMatTurmaMunicipio'] = array('text' => 'N', 'button' => 'N', 'radio' => 'N', 'select' => 'N');
		$arrAcesso['proinfantil.php?modulo=novasturmas/popupCadastrarTurmas'] = array('text' => 'N', 'button' => 'N', 'radio' => 'N', 'select' => 'N', 'textarea' => 'N');
		$arrAcesso['proinfantil.php?modulo=novasturmas/termoNovasTurmas'] = array('text' => 'N', 'button' => 'N', 'radio' => 'N', 'select' => 'N', 'textarea' => 'N');
	}
	
	$_SESSION['proinfantil']['acesso'] = $arrAcesso;
}

function verificaAnaliseLote(){
	global $db;
	
	$sql = "SELECT
				formata_cpf_cnpj(iue.iuecnpj) as cnpj,
				lot.estuf, 
			    mun.mundescricao,
			    lot.muncod,
                lot.valorrepasse,
                lot.lotid
			FROM 
			  	proinfantil.loteminutanovasturmas lot
				inner join territorios.municipio mun on mun.muncod = lot.muncod
				left join par.instrumentounidade iu
	                inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
                on iu.muncod = mun.muncod 
			--WHERE lot.lotid = 6
			order by
				lot.lotid,
				lot.estuf,
			    mun.mundescricao";
	$arrLote = $db->carregar($sql);
	$arrLote = $arrLote ? $arrLote : array();
	
	$arrRegistro = array();
	foreach ($arrLote as $key => $lote) {
		$arrPost = array('lotid' => $lote['lotid'], 'muncod' => $lote['muncod']);
		$obNovas = new NovasTurmas($arrPost);
		
		$arrTurmas = $obNovas->carregaTurmaPorMunicipio();
		$arrTurmas = $arrTurmas ? $arrTurmas : array();
		
		
		$valor_total = 0;		
		foreach ($arrTurmas as $v) {
			
			if( $v['anatipo'] == 'A' ){
				$valortotal = str_replace(".","", $v['valor_total']);
				$valortotal = str_replace(",",".", $valortotal);
				
				$valor_total += $valortotal;
			}
		}
		if( trim((float)$lote['valorrepasse']) != trim((float)$valor_total) ){
			array_push($arrRegistro, array(
									'cnpj' => $lote['cnpj'],
									'estuf' => $lote['estuf'],
									'mundescricao' => $lote['mundescricao'],
									'muncod' => $lote['muncod'],
									'valorminuta' => number_format($lote['valorrepasse'], 2, '.', ','),
									'valoranalise' => number_format($valor_total, 2, '.', ','),
									'lote' => $lote['lotid']
								));
		}
	}
	ob_clean();
	
	$nomeArquivo = 'minuta_repasse_novasturmas_'.date("Ymd");
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=".$nomeArquivo.".xls");
	header ( "Content-Disposition: attachment; filename=".$nomeArquivo.".xls");
	header ( "Content-Description: MID Gera excel" );
	
	$cabecalho = array('CNPJ', 'UF', 'Municípo', 'Código IBGE', 'Valor Repasse Minuta', 'Valor Repasse Analise', 'Lote');
	$db->monta_lista_tabulado($arrRegistro,$cabecalho,1000000,5,'N','100%', 'S');
	
	
	exit();
}

function enviarPagamentoEfetuado($muncod = ''){
	
	if( $_SESSION['exercicio'] == '2012' ){
		return false;
	} else {
		return true;
	}
}

function calculaQtdMatriculaAprovada( $arParam = array() ){
	global $db;
	
	$muncod = $arParam['muncod'];
	$ano 	= $arParam['ano'];
	$timid 	= $arParam['timid'];
	$tatid 	= $arParam['tatid'];
	$tirid 	= $arParam['tirid'];
	$turma 	= $arParam['turma'];
	
	$filtro = '';
	if( $tirid ){
		$filtro = " and t.tirid = $tirid ";
	} else {
		$filtro = " and t.tirid in (1, 2) ";
	}
	
	if( $turma ) $filtro .= " and t.turid = $turma";
	
	$sql = "select coalesce(sum(nta.ntaquantidade),0) from proinfantil.turma t
				inner join proinfantil.novasturmasalunoatendido nta on nta.turid = t.turid
			        and nta.timid = $timid /* 6 - Creche, 7 - Pré-escola */
			        and nta.tatid = $tatid  /* 1 - Integral, 2 - Parcial */
			    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = t.turid
			    inner join workflow.documento doc on doc.docid = ntw.docid
			where
			    t.muncod = '$muncod'
			    and t.turano = '$ano'
			    $filtro /* 1 - Pública, 2 - Privada */
			    and doc.esdid = ".WF_NOVASTURMAS_EM_APROVADA;
	$total = $db->pegaUm($sql);
	
	return (int)$total;
}
?>