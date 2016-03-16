<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once "_funcoesPar.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";


if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT DISTINCT	pre.preid -- , pto.*, e.* --count(oi.obrid)
		FROM obras.preobra pre
			inner JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
			inner JOIN territorios.municipio mun
			inner JOIN territorios.muntipomunicipio mtpm
			inner JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtpm.tpmid 
					AND tpmstatus = 'A' AND gtmid = 7 ON mtpm.muncod = mun.muncod
				ON pre.muncod  = mun.muncod
			inner join par.empenhoobra eo on eo.preid = pre.preid and eobstatus = 'A'
			inner join par.empenho e on e.empid = eo.empid and empstatus = 'A'
		WHERE pre.prestatus = 'A' 
			AND pre.preidpai IS NULL                
			AND pre.tooid = 1 
			AND presistema = 23 
			and pre.preid in (select preid from par.empenhoobra eo 
								inner join par.empenho em on em.empid = eo.empid  and eobstatus = 'A' and empstatus = 'A') 
			and pre.preid not in (	SELECT DISTINCT pre.preid
									FROM obras.preobra pre                                                                   
									INNER JOIN obras.pretipoobra 		pto ON pre.ptoid = pto.ptoid    
									INNER JOIN territorios.municipio 	mun
										INNER JOIN territorios.muntipomunicipio mtpm 
											INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtpm.tpmid 
																						AND tpmstatus = 'A' 
																						AND gtmid = 7 
										ON mtpm.muncod = mun.muncod
									ON pre.muncod  = mun.muncod
									INNER JOIN obras2.obras				oi ON oi.preid = pre.preid and oi.obrstatus = 'A'
									WHERE 
										pre.prestatus = 'A' AND pre.preidpai IS NULL                
										AND pre.tooid = 1  AND presistema = 23  
										AND pre.preid in (SELECT preid FROM par.empenhoobra eo 
														  INNER JOIN par.empenho em ON  em.empid = eo.empid and eobstatus = 'A' and empstatus = 'A' 
																						)
)";

$arrDados = $db->carregarColuna( $sql );
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $preid) {
	/*** Recupera dados da Pre Obra ***/
	$sql = "SELECT 
				p.predescricao || ' - ' || mun.mundescricao || ' - ' || mun.estuf as nome_obra,
				ent.entid as unidade_implantadora,
				p.precep,
				p.prelogradouro,
				p.precomplemento,
				p.prebairro,
				p.muncod,
				p.estuf,
				p.prenumero,
				p.prelatitude,
				p.prelongitude,
				p.preesfera,
				p.ptoid,
				CASE   WHEN p.ptoid in (1,2,3,4,5,11,12,6,7,13,25,26,31,14,15,16,8,9,23,24,27,28,32,21,10,35,36)   THEN 1 
                                    ELSE   CASE WHEN p.ptoid in (29,17,33) THEN 4
                                                ELSE 
                                                           CASE WHEN p.ptoid in (30,18,34) THEN 3 END
                                                END
                                    END AS tipodeobra,
                                    CASE WHEN (substring(UPPER(p.predescricao), 'INDÍGENA') IS NOT NULL OR substring(UPPER(p.predescricao), 'INDÍGENA') != '' ) OR
                                                  (substring(UPPER(p.predescricao), 'INDIGENA') IS NOT NULL OR substring(UPPER(p.predescricao), 'INDIGENA') != '' )
                                                THEN 4 -- INDÍGENA
                                                ELSE 
                                                           CASE WHEN (substring(UPPER(p.predescricao), 'RURAL') IS NOT NULL OR substring(UPPER(p.predescricao), 'RURAL') != '' )
                                                           THEN 1
                                                           ELSE 2
                                                END
                                    END AS classificacaoobra,
                                    p.prevalorobra as valorobra,
                                    pt.tpoid
			FROM 
				obras.preobra p
			INNER JOIN
				territorios.municipio mun on p.muncod = mun.muncod
			INNER JOIN 
				entidade.endereco ende ON ende.muncod = p.muncod
			INNER JOIN
				entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
			INNER JOIN
				entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid IN (1)
			LEFT JOIN
				obras.pretipoobra pt ON pt.ptoid = p.ptoid
			WHERE 
				p.preid = ".$preid;
	$dadosPreObra = $db->carregar($sql);
	
	/*** Insere novo endereço da obra ***/
	$sql = "INSERT INTO 
				entidade.endereco (endcep,
								   endlog,
								   endcom,
								   endbai,
								   muncod,
								   estuf,
								   endnum,
								   medlatitude,
								   medlongitude,
								   endstatus)
			
			VALUES
				( '".$dadosPreObra[0]['precep']."', 
				  '".$dadosPreObra[0]['prelogradouro']."', 
				  '".$dadosPreObra[0]['precomplemento']."', 
				  '".$dadosPreObra[0]['prebairro']."', 
				  '".$dadosPreObra[0]['muncod']."', 
				  '".$dadosPreObra[0]['estuf']."', 
				  '".$dadosPreObra[0]['prenumero']."',  
				  '".$dadosPreObra[0]['prelatitude']."', 
				  '".$dadosPreObra[0]['prelongitude']."', 
				  'A' ) RETURNING endid";
	
	$endid = $db->pegaUm($sql);
	
	/*** Insere a nova obra ***/
	
	$sql = "INSERT INTO obras2.empreendimento
				( empdsc, entidunidade orgid, tooid, preid, empesfera, tpoid, prfid, tobid, cloid, empvalorprevisto )
			VALUES (
				'".str_ireplace( "'", "", $dadosPreObra[0]['nome_obra'])."',
				{$dadosPreObra[0]['unidade_implantadora']},
				3,
				1,
				$preid,
				'{$dadosPreObra[0]['preesfera']}',
				{$dadosPreObra[0]['tpoid']},
				39,
				{$dadosPreObra[0]['tipodeobra']},
				{$dadosPreObra[0]['classificacaoobra']},
				{$dadosPreObra[0]['valorobra']})
			RETURNING empid";
			
	$empid = $db->pegaUm($sql);
	 
	$sql = "INSERT INTO obras2.obras  
				( empid, obrnome, entid, tooid, preid, endid, tpoid,  tobid, cloid, obrvalorprevisto )
			VALUES
				(
				$empid,
				'".str_ireplace( "'", "", $dadosPreObra[0]['nome_obra'])."',
				{$dadosPreObra[0]['unidade_implantadora']},
				1,
				$preid,
				$endid,
				{$dadosPreObra[0]['tpoid']},
				39,
				{$dadosPreObra[0]['tipodeobra']},
				{$dadosPreObra[0]['classificacaoobra']},
				{$dadosPreObra[0]['valorobra']})
			RETURNING obrid";
			
	$obrid = $db->pegaUm($sql);
	
	// descrição do documento
    $docdsc = "Fluxo de obra do módulo Obras II - obrid " . $obrid;

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento(TPDID_OBJETO, $docdsc);
    
    $sql = "UPDATE obras2.obras SET docid = $docid WHERE obrid = $obrid";
	
//	$sql = "INSERT INTO obr as.ob rainfraestrutura(obrdesc, entidunidade, orgid, tooid, preid, endid, obrtipoesfera, tpoid, prfid, tobraid, cloid, obrvalorprevisto ) 
//			VALUES('".str_ireplace( "'", "", $dadosPreObra[0]['nome_obra'])."',
//					".$dadosPreObra[0]['unidade_implantadora'].",
//					3,
//					1,
//					".$preid.",
//					".$endid.",
//					'".$dadosPreObra[0]['preesfera']."',
//					".$dadosPreObra[0]['tpoid'].",
//					39,
//					'".$dadosPreObra[0]['tipodeobra']."',
//					'".$dadosPreObra[0]['classificacaoobra']."',
//					'".$dadosPreObra[0]['valorobra']."') 
//			RETURNING obrid";
//	$obrid = $db->pegaUm($sql);
	
	/*** Recupera as fotos do terreno no Pré Obra ***/
	$sql = "SELECT DISTINCT
				arq.arqid
			FROM 
				public.arquivo arq
			INNER JOIN 
				obras.preobrafotos pof ON arq.arqid = pof.arqid
			INNER JOIN 
				obras.preobra pre ON pre.preid = pof.preid
			WHERE							
				pre.preid = ".$preid."
			AND							
				(substring(arqtipo,1,5) = 'image')";
	$fotosTerreno = $db->carregar($sql);
	
	if( $fotosTerreno ){
		/*** Insere as fotos para galeria de fotos da obra ***/
		foreach($fotosTerreno as $foto){
			$sql = "INSERT INTO 
					obras.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
					VALUES
					(".$obrid.", 21, ".$foto['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
			$db->executar($sql);
		}
	}
	
	/*** Recupera os documentos anexos no Pré Obra ***/
	$sql = "SELECT DISTINCT
				arq.arqid
			FROM 
				obras.preobraanexo p
			INNER JOIN 
				public.arquivo arq ON arq.arqid = p.arqid
			WHERE							
				p.preid = ".$preid;
	$anexos = $db->carregar($sql);
	
	if( $anexos ){
		/*** Insere os documentos nos arquivos da obra ***/
		foreach($anexos as $anexo){
			$sql = "INSERT INTO 
					obras.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
					VALUES
					(".$obrid.", 21, ".$anexo['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
			$db->executar($sql);
		}
	}
	
	/*** Inclue o ID da nova obra na tabela do pre obra ***/
	$sql = "UPDATE obras.preobra SET obrid = ".$obrid." WHERE preid = ".$preid;
	$db->executar($sql);
}

$db->commit();

?>