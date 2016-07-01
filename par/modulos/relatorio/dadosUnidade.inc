<?php
set_time_limit(0);
ini_set("memory_limit", "1000M");

global $db;
$i = 0;

$sql = "SELECT
			iu.inuid,
			iu.itrid,
			iu.muncod,
			CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as estuf,
			CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE m.mundescricao END as descricao
		FROM
			par.instrumentounidade iu
		LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
		ORDER BY
			iu.itrid, estuf, descricao";

$dados = $db->carregar($sql);

foreach( $dados as $dado ){
	
	$itrid = $dado['itrid'];
	$inuid = $dado['inuid'];
	$estuf = $dado['estuf'];
	
	if($itrid == 2){
		$muncod = $dado['muncod'];
		$local = $dado['descricao']; 
		$prefeitura = '1';
		$prefeito 	= '2';
		$stWhere = "AND en.muncod = '{$muncod}'";
	}else{
		$local = $dado['estuf']; 
		$prefeitura = '6';
		$prefeito 	= '25';
	}
	
	$arrPrefeitura = array();
	$arrPrefeito   = array();
	$arrSecretaria = array();
	$arrSecretario = array();
	
		//Prefeitura
		$sql = "SELECT
					e.entid,
					e.entnome as entidade,
					e.entdatainclusao AS dthistresponsavel,
					e.entnumcpfcnpj,
					e.entnuninsest,
					e.entrazaosocial,
					e.entemail,
					e.entsig,
					e.entnumdddcomercial,
					e.entnumcomercial,
					e.endcep,
					e.endlog,
					e.endnum,
					e.endbai,
					e.estuf,
					e.muncod,
					e.medlatitude,
					e.medlongitude
				FROM
					par.entidade e 
				WHERE
					e.dutid = ".DUTID_PREFEITURA." AND
					e.entstatus = 'A' AND
					e.muncod = '{$muncod}'";
		/*
		$sql = "SELECT
					e.entid,
					e.entnome as entidade,
					max(his.hstdata) AS dthistresponsavel,
					e.entnumcpfcnpj,
					e.entnuninsest,
					e.entrazaosocial,
					e.entemail,
					e.entsig,
					e.entnumdddcomercial,
					e.entnumcomercial,
					en.endcep,
					en.endlog,
					en.endnum,
					en.endbai,
					en.estuf,
					en.muncod,
					en.medlatitude,
					en.medlongitude
				FROM
					entidade.entidade e 
				INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid 
				INNER JOIN entidade.endereco en 	 ON en.entid = e.entid 
				LEFT  JOIN entidade.historico his 	 ON his.entid = e.entid
				WHERE
					funid='{$prefeitura}' {$stWhere} AND
					en.estuf = '{$estuf}'
				GROUP BY e.entid, e.entnome,en.endcep, en.endlog, en.endnum, en.endbai, en.estuf,
						en.muncod, en.medlatitude, en.medlongitude, e.entnumcpfcnpj, e.entnuninsest,
						e.entrazaosocial, e.entemail, e.entsig, e.entnumdddcomercial, e.entnumcomercial,
						en.endcep, en.endlog, en.endnum, en.endbai, en.estuf, en.muncod, en.medlatitude, 
						en.medlongitude";
		*/
		$arrPrefeitura = $db->pegaLinha( $sql );

		if( is_array($arrPrefeitura) && $arrPrefeitura['entid'] ){
			
			//Prefeito
			$sql = "SELECT
						e.entid,
						e.entnome as responsavel,
						e.entdatainclusao AS dthistresponsavel,
						e.entemail,
						e.entnumrg,
						e.entorgaoexpedidor,
						e.entsexo,
						e.entdatanasc,
						e.entnumdddcomercial,
						e.entnumcomercial
					FROM
						par.entidade e 
					INNER JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
					WHERE
						e.dutid = ".DUTID_PREFEITO." AND
						e.entstatus = 'A' AND
						iu.muncod = '{$muncod}'";
			/*
			$sql = "SELECT
						e.entid,
						e.entnome as responsavel,
						max(his.hstdata) AS dthistresponsavel,
						e.entemail,
						e.entnumrg,
						e.entorgaoexpedidor,
						e.entsexo,
						e.entdatanasc,
						e.entnumdddcomercial,
						e.entnumcomercial
					FROM
						entidade.entidade e 
					INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid 
					INNER JOIN entidade.funentassoc fe 	 ON fe.fueid = f.fueid 
					LEFT  JOIN entidade.historico his 	 ON his.entid = e.entid
					WHERE
						funid='{$prefeito}' AND
						fe.entid = '{$arrPrefeitura['entid']}'
					GROUP BY 
						e.entid, e.entnome, e.entemail, e.entnumrg, e.entorgaoexpedidor,
						e.entsexo, e.entdatanasc, e.entnumdddcomercial, e.entnumcomercial";
			*/
			$arrPrefeito = $db->pegaLinha( $sql );
		} 

		//Secretaria
		$sql = "SELECT
					e.entid,
					e.entnome as entidade,
					e.entdatainclusao AS dthistresponsavel,
					e.entemail,
					e.entsig,
					e.entnumdddcomercial,
					e.entnumcomercial,
					e.endcep,
					e.endlog,
					e.endnum,
					e.endbai,
					e.estuf,
					e.muncod,
					e.medlatitude,
					e.medlongitude
				FROM
					par.entidade e 
				WHERE
					e.dutid = ".DUTID_SECRETARIA_MUNICIPAL." AND
					e.entstatus = 'A' AND
					e.muncod = '{$muncod}'";
		/*
		$sql = "SELECT
					e.entid,
					e.entnome as entidade,
					max(his.hstdata) AS dthistresponsavel,
					e.entemail,
					e.entsig,
					e.entnumdddcomercial,
					e.entnumcomercial,
					en.endcep,
					en.endlog,
					en.endnum,
					en.endbai,
					en.estuf,
					en.muncod,
					en.medlatitude,
					en.medlongitude
				FROM
					entidade.entidade e 
				INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid 
				INNER JOIN entidade.endereco en 	 ON en.entid = e.entid 
				LEFT  JOIN entidade.historico his 	 ON his.entid = e.entid
				WHERE
					funid='7' {$stWhere} AND
					en.estuf = '{$estuf}'
				GROUP BY 
					e.entid, e.entnome,en.endcep, en.endlog, en.endnum, en.endbai, en.estuf,
					en.muncod, en.medlatitude, en.medlongitude, e.entemail, e.entsig,
					e.entnumdddcomercial, e.entnumcomercial";
			*/
		$arrSecretaria = $db->pegaLinha( $sql );
		
		
		if( is_array($arrSecretaria) && $arrSecretaria['entid'] ){
			//Secretario
			$sql = "SELECT
						e.entid,
						e.entnome as responsavel,
						e.entdatainclusao AS dthistresponsavel,
						e.entemail,
						e.entnumrg,
						e.entorgaoexpedidor,
						e.entsexo,
						e.entdatanasc,
						e.entnumdddcomercial,
						e.entnumcomercial
					FROM
						par.entidade e 
					INNER JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
					WHERE
						e.dutid = ".DUTID_DIRIGENTE." AND
						e.entstatus = 'A' AND
						iu.muncod = '{$muncod}'";
			/*
			$sql = "SELECT
						e.entid,
						e.entnome as responsavel,
						max(his.hstdata) AS dthistresponsavel,
						e.entemail,
						e.entnumrg,
						e.entorgaoexpedidor,
						e.entsexo,
						e.entdatanasc,
						e.entnumdddcomercial,
						e.entnumcomercial
					FROM
						entidade.entidade e 
					INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid 
					INNER JOIN entidade.funentassoc fe 	 ON fe.fueid = f.fueid 
					LEFT  JOIN entidade.historico his 	 ON his.entid = e.entid
					WHERE
						funid='15' AND
						fe.entid = '{$arrSecretaria['entid']}'
					GROUP BY
						e.entid, e.entnome, e.entemail, e.entnumrg, e.entorgaoexpedidor,
						e.entsexo, e.entdatanasc, e.entnumdddcomercial, e.entnumcomercial";
			*/
			$arrSecretario = $db->pegaLinha( $sql );
		} 
		
		//$arDados[$i]['inuid'] = $inuid;
		$arDados[$i]['local'] = $local;
		$arDados[$i]['estuf'] = $estuf;
		//$arDados[$i]['itrid'] = $itrid;
		$pendencia = "";
		
		if($itrid == 2){
			if( !$arrPrefeitura['entidade'] || !$arrPrefeitura['dthistresponsavel'] || !$arrPrefeitura['entnumcpfcnpj'] ||
			!$arrPrefeitura['entnuninsest'] || !$arrPrefeitura['entrazaosocial'] || !$arrPrefeitura['entemail'] || !$arrPrefeitura['entsig'] ||
			!$arrPrefeitura['entnumdddcomercial'] || !$arrPrefeitura['entnumcomercial'] || !$arrPrefeitura['endcep'] || !$arrPrefeitura['endlog'] ||
			!$arrPrefeitura['endnum'] || !$arrPrefeitura['endbai'] || !$arrPrefeitura['estuf'] || !$arrPrefeitura['muncod'] || !$arrPrefeitura['medlatitude'] ||
			!$arrPrefeitura['medlongitude']){
				$pendencia = 'Atualize os dados cadastrais da Prefeitura.<br>';
			}

			if( !$arrPrefeito['responsavel'] || !$arrPrefeito['entemail'] || !$arrPrefeito['entnumrg'] || 
			!$arrPrefeito['entorgaoexpedidor'] || !$arrPrefeito['entsexo'] || !$arrPrefeito['entdatanasc'] || !$arrPrefeito['entnumdddcomercial'] || 
			!$arrPrefeito['entnumcomercial'] ){
				$pendencia .= 'Atualize os dados cadastrais do(a) Prefeito(a).<br>';
			}

			if( !$arrSecretaria['entidade'] || !$arrSecretaria['entemail'] ||
			!$arrSecretaria['entsig'] || !$arrSecretaria['entnumdddcomercial'] || !$arrSecretaria['entnumcomercial'] || !$arrSecretaria['endcep'] ||
			!$arrSecretaria['endlog'] || !$arrSecretaria['endnum'] || !$arrSecretaria['endbai'] || !$arrSecretaria['estuf'] ||
			!$arrSecretaria['muncod'] || !$arrSecretaria['medlatitude'] || !$arrSecretaria['medlongitude'] ){
				$pendencia .= 'Atualize os dados cadastrais da Secretaria Municipal de Educação.<br>';
			}

			if(!$arrSecretario['responsavel'] || !$arrSecretario['entemail'] ||
			!$arrSecretario['entnumrg'] || !$arrSecretario['entorgaoexpedidor'] || !$arrSecretario['entsexo'] ||
			!$arrSecretario['entdatanasc'] || !$arrSecretario['entnumdddcomercial'] || !$arrSecretario['entnumcomercial'] ){
				$pendencia .= 'Atualize os dados cadastrais do(a) Dirigente Municipal de Educação.<br>';
			}
			
		} else {

			if( !$arrSecretaria['entidade'] || !$arrSecretaria['dthistresponsavel'] || !$arrSecretaria['entemail'] ||
			!$arrSecretaria['entsig'] || !$arrSecretaria['entnumdddcomercial'] || !$arrSecretaria['entnumcomercial'] || !$arrSecretaria['endcep'] ||
			!$arrSecretaria['endlog'] || !$arrSecretaria['endnum'] || !$arrSecretaria['endbai'] || !$arrSecretaria['estuf'] ||
			!$arrSecretaria['muncod'] || !$arrSecretaria['medlatitude'] || !$arrSecretaria['medlongitude'] ){
				//ver($arDados[$i], $arrSecretaria);
				$pendencia = 'Atualize os dados cadastrais da Secretaria Estadual de Educação.<br>';
			}
		
			if(!$arrSecretario['responsavel'] || !$arrSecretario['dthistresponsavel'] || !$arrSecretario['entemail'] ||
			!$arrSecretario['entnumrg'] || !$arrSecretario['entorgaoexpedidor'] || !$arrSecretario['entsexo'] ||
			!$arrSecretario['entdatanasc'] || !$arrSecretario['entnumdddcomercial'] || !$arrSecretario['entnumcomercial'] ){
				$pendencia .= 'Atualize os dados cadastrais do(a) Secretário Estadual de Educação.<br>';
			}
		}
		
		$arDados[$i]['pendencias'] = $pendencia;
		$i++;
}
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$cabecalho = array("Descrição", "Estado", "Pendencia");
$db->monta_lista($arDados,$cabecalho,6000,5,$soma,"100%","S");

?>