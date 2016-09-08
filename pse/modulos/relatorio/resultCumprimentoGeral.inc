<?
if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
ini_set("memory_limit","2048M");
set_time_limit(30000);
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">

				
<?php
/*
extract($_POST);

echo '
			<center>
				<table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
					<tr bgcolor="#ffffff">
						<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>			
						<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">				
							SIMEC- Sistema Integrado de Monitoramento Execução e Controle<br/>				
							Ministério da Educação - Ministério da Saúde<br />
						</td>
						<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">					
							Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>					
							Hora da Impressão:' . date( 'd/m/Y - H:i:s' ) . '<br />					
						</td>					
					</tr>				
				</table>
			</center>
		';

echo "<br>";

if($tiporelatorio == '1'){
	echo "<center><b>Relatório de Municípios que atingiu a meta</b></center>";
}else{
	echo "<center><b>Relatório de Municípios que não atingiu a meta</b></center>";
}


$cabecalho = array("Estado","Município","Componente","Ação","Escola","Qtd. Pactuada","Qtd. Realizada","(%) Percentual Atingida");
$dadosMetaAtingida = array();
$dadosMetaNaoAtingida = array();



if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
	$where[0] = " AND ed.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
}
if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
	$where[1] = " AND mu.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
}

	
//lista todos os municipios
$sql = "select mu.estuf, mu.mundescricao as municipio, mu.muncod
		from pse.planoacaoc1 p
		inner join entidade.entidade e on e.entid = p.entid
		inner join entidade.endereco ed on ed.entid = e.entid
		inner join territorios.municipio mu on mu.muncod = ed.muncod
		WHERE e.entstatus = 'A'
		".$where[0]."
		".$where[1]."
		group by 1,2,3
		
		union
		
		select mu.estuf, mu.mundescricao as municipio, mu.muncod
		from pse.planoacaoc2 p
		inner join entidade.entidade e on e.entid = p.entid
		inner join entidade.endereco ed on ed.entid = e.entid
		inner join territorios.municipio mu on mu.muncod = ed.muncod
		WHERE e.entstatus = 'A'
		".$where[0]."
		".$where[1]."
		group by 1,2,3
		
		order by 1,2";
$ufMun = $db->carregar($sql);

if($ufMun){
	foreach($ufMun as $vUfMun){
		
		//lista componentes
		$sql = "select comid, upper(comdescricao) as componente from pse.componenteavalia order by 1";
		$comp = $db->carregar($sql);
		
		foreach($comp as $vComp){
			
			//lista ações
			$sql = "select acaoid, acaodescricao as acao from pse.acao order by 1,2";
			$acao = $db->carregar($sql);
			
			foreach($acao as $vAcao){
				
				//lista escolas
				if($vComp['acaoid'] == 1){
					$sql = "select e.entnome as escola, p.pamquanteduseratend as qtdpactuada
							from pse.planoacaoc1 p
							inner join entidade.entidade e on e.entid = p.entid
							inner join entidade.endereco ed on ed.entid = e.entid
							inner join territorios.municipio mu on mu.muncod = ed.muncod
							WHERE e.entstatus = 'A'
							and mu.muncod = '".$vUfMun['muncod']."'
							order by 1";
				}
				else{
					$sql = "select e.entnome as escola, p.pacquanteduseratend as qtdpactuada
							from pse.planoacaoc2 p
							inner join entidade.entidade e on e.entid = p.entid
							inner join entidade.endereco ed on ed.entid = e.entid
							inner join territorios.municipio mu on mu.muncod = ed.muncod
							WHERE e.entstatus = 'A'
							and mu.muncod = '".$vUfMun['muncod']."'
							order by 1";
				}
				$escola = $db->carregar($sql);
				
				foreach($escola as $vEscola){
					
					array_push($dadosMetaAtingida,
									
									array(	
											'Estado' => $vUfMun['estuf'],
										  	'Municipio' => $vUfMun['municipio'],
										  	'Componente' => $vComp['componente'],
											'Ação' => $vAcao['acao'],
											'Escola' => $vEscola['escola'],
											'Qtd. Pactuada' => $vEscola['qtdpactuada'],
											'Qtd. Realizada' => $qtdRealizada2,
											'(%) Percentual Atingida' => $qtdPercentual2,
											'Qtd. Educandos Alterados' => $qtdPercentualMunicipio
										 )
								   
				  	);
				  
				}
			}
		}
		
		/*
		//Pega Qtd. Pactuada Comp. I
		$sql = "select sum(pamquanteduseratend) as total
				from pse.planoacaoc1 p 
				inner join entidade.entidade e on e.entid = p.entid
				inner join entidade.endereco ed on ed.entid = e.entid 
				where ed.muncod = '".$vUfMun['muncod']."'";
		$qtdPactuada1 = $db->pegaUm($sql);
		if(!$qtdPactuada1) $qtdPactuada1 = 0;
		
		//Pega Qtd. Pactuada Comp. II
		$sql = "select sum(pacquanteduseratend) as total
				from pse.planoacaoc2 p 
				inner join entidade.entidade e on e.entid = p.entid
				inner join entidade.endereco ed on ed.entid = e.entid 
				where ed.muncod = '".$vUfMun['muncod']."'";
		$qtdPactuada2 = $db->pegaUm($sql);
		if(!$qtdPactuada2) $qtdPactuada2 = 0;
		
		
		//Pega Qtd. Realizada Comp. I
		$sql = "select sum(avleducandoavaliado) as total 
				from pse.avaliacao a
				inner join pse.monitoraescola m on m.moeid = a.moeid
				inner join entidade.entidade e on e.entid = m.entid
				inner join entidade.endereco ed on ed.entid = e.entid 
				where a.comid = 1
				and a.avlvalidar = 'S'
				and ed.muncod = '".$vUfMun['muncod']."'";
		$qtdRealizada1 = $db->pegaUm($sql);
		if(!$qtdRealizada1) $qtdRealizada1 = 0;
		
		//Pega Qtd. Realizada Comp. II
		$sql = "select sum(avcquantidade) as total 
				from pse.avaliacomp2 a
				inner join pse.monitoraescola m on m.moeid = a.moeid
				inner join entidade.entidade e on e.entid = m.entid
				inner join entidade.endereco ed on ed.entid = e.entid 
				where a.comid = 2
				and a.avcvalidar = 'S'
				and ed.muncod = '".$vUfMun['muncod']."'";
		$qtdRealizada2 = $db->pegaUm($sql);
		if(!$qtdRealizada2) $qtdRealizada2 = 0;
		
		
		//Pega Percentual Atingida Comp. I
		if($qtdPactuada1 > 0){
			$qtdPercentual1 = ($qtdRealizada1 * 100) / $qtdPactuada1;
		}else{
			$qtdPercentual1 = 0;
		}
		
		//Pega Percentual Atingida Comp. II
		if($qtdPactuada2 > 0){
			$qtdPercentual2 = ($qtdRealizada2 * 100) / $qtdPactuada2;
		}else{
			$qtdPercentual2 = 0;
		}
		
		//Pega Percentual Atingida do Município
		$qtdPercentualMunicipio = ($qtdPercentual1 + $qtdPercentual2) / 2;
		if(!$qtdPercentualMunicipio) $qtdPercentualMunicipio = 0;
		
		if($qtdPercentualMunicipio >= 70){
			array_push($dadosMetaAtingida,
					
									array(	
											'Estado' => $vUfMun['estuf'],
										  	'Municipio' => $vUfMun['mundescricao'],
										  	'Qtd. Pactuada Comp. I' => $qtdPactuada1,
											'Qtd. Realizada Comp. I' => $qtdRealizada1,
											'Percentual Atingida Comp. I' => $qtdPercentual1,
											'Qtd. Pactuada Comp. II' => $qtdPactuada2,
											'Qtd. Realizada Comp. II' => $qtdRealizada2,
											'Percentual Atingida Comp. II' => $qtdPercentual2,
											'Percentual Atingida do Município' => $qtdPercentualMunicipio
										 )
								   
				  );
		}
		else{
			array_push($dadosMetaNaoAtingida,
					
									array(	
											'Estado' => $vUfMun['estuf'],
										  	'Municipio' => $vUfMun['mundescricao'],
										  	'Qtd. Pactuada Comp. I' => $qtdPactuada1,
											'Qtd. Realizada Comp. I' => $qtdRealizada1,
											'Percentual Atingida Comp. I' => $qtdPercentual1,
											'Qtd. Pactuada Comp. II' => $qtdPactuada2,
											'Qtd. Realizada Comp. II' => $qtdRealizada2,
											'Percentual Atingida Comp. II' => $qtdPercentual2,
											'Percentual Atingida do Município' => $qtdPercentualMunicipio
										 )
								   
				  );
		}
	
		 
	}
}

/*
if($tiporelatorio == '1'){
	$db->monta_lista_array($dadosMetaAtingida,$cabecalho,100,10,"N","CENTER");
}else{
	$db->monta_lista_array($dadosMetaNaoAtingida,$cabecalho,100,10,"N","CENTER");
}
*/

//$db->monta_lista_array($dadosMetaAtingida,$cabecalho,1000,10,"N","CENTER");

//die;


include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$dadosOrigem = $dados; 

//verifica quais municipios atingiu a meta de 70% do alunado avaliado.
/*
if( is_array($dados) ){
	
	$limpaMunicipio = array();
	
	foreach($dados as $i => $dado){
		
		if( (int)$dado['percentualatingido'] < 70 && !in_array($dado['municipio'], $limpaMunicipio) ){
			array_push($limpaMunicipio, $dado['municipio']);
		}
		
	}
	
	foreach($dados as $i => $dado){
		
		if( in_array($dado['municipio'], $limpaMunicipio) ){
			unset($dados[$i]);
		}
	}
	
}

if( $_POST['tiporelatorio'] == 2 ){
	$dados = array_diff($dadosOrigem,$dados);
}



//reindexa o array
if ($dados){
	$dados = array_values( $dados );
}
*/
//die;

$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(true);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true);
$r->setTolizadorLinha(false);

//$r->setBrasao(true);
echo $r->getRelatorio();

?>
</body>
</html>
<?php 

function monta_sql(){
	global $db;
	global $filtroSession;
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $componente[0] && ( $componente_campo_flag || $componente_campo_flag == '1' )){
		$where[2] = " AND co.comid " . (( $componente_campo_excludente == null || $componente_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $componente ) . "') ";		
	}
	if( $acao[0] && ( $acao_campo_flag || $acao_campo_flag == '1' )){
		$where[3] = " AND ac.acaoid " . (( $acao_campo_excludente == null || $acao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $acao ) . "') ";		
	}
	
	//monta query monstra
	//lista as ações para montar a query
	$sql2 = "select co.comdescricao as componente, ac.acaodescricao as acao, ac.acaoid, co.comid 
			from pse.acao ac 
			INNER JOIN pse.componenteavalia co ON co.comid = ac.comid
			WHERE co.comid in (1,2)
			and acaoid not in(6,11)
			order by 1,2";
	$dadosAC = $db->carregar($sql2);
	
	foreach($dadosAC as $a){
		
		if($a['comid'] == 1){ // componente 1
			
			if($a['acaoid'] == 6){ // Ação 7
				
				$sql .= "UNION
					
						SELECT  mun.muncod,
								ende.estuf as estado, 
								mun.mundescricao as municipio,
								'".$a['componente']."' as componente,
								'".$a['acao']."' as acao, 
								ent.entnome as escola,
								mp.mpaacao06comp1/totalescola as qtdpactuada,
								(case when mo2.qtdrealizada is not null then
									mo2.qtdrealizada
								      else
									0
								end) as qtdrealizada,
								(case when (mp.mpaacao06comp1/totalescola) > 0 then
										(COALESCE(mo2.qtdrealizada,0) * 100) / (mp.mpaacao06comp1/totalescola)
								      else
										0
								end) as percentualatingido,
								(case when mo2.qtdalterada is not null then
									mo2.qtdalterada
								      else
									0
								end) as qtdalterada
							FROM pse.planoacaoc1 pac
							INNER JOIN
								pse.metapactuada mp ON mp.conid = pac.conid
							INNER JOIN
								entidade.entidade ent ON ent.entid = pac.entid
							INNER JOIN
								entidade.endereco ende ON ende.entid = ent.entid
							INNER JOIN
								territorios.municipio mun ON mun.muncod = ende.muncod
							LEFT JOIN
								(SELECT count(pamid) as totalescola, conid from pse.planoacaoc1 group by conid) te on te.conid = pac.conid
							LEFT JOIN
								(select sum(a.avleducandoavaliado) AS qtdrealizada, sum(a.avlidentifica1) AS qtdalterada, mo.entid, ac.acaodescricao as acao
								from pse.avaliacao a
								INNER JOIN
									pse.monitoraescola mo ON mo.moeid = a.moeid 
								INNER JOIN
									pse.acao ac ON ac.acaoid = mo.acaoid
								INNER JOIN
									pse.componenteavalia co ON co.comid = ac.comid
								WHERE mo.moestatus = 'A' AND mo.moeidentificasse in ('N','S') and a.avlvalidar = 'S' AND ac.acaoid = ".$a['acaoid']."
								".$where[2]."
								".$where[3]."
								GROUP BY mo.entid, ac.acaodescricao
								) mo2 ON mo2.entid = ent.entid 
							WHERE
								ent.entstatus='A'
								".$where[0]."
								".$where[1]."
							GROUP BY 
								mun.muncod,
								ende.estuf,
								mun.mundescricao,
								ent.entcodent, ent.entnome,
								mp.mpaacao06comp1,
								mo2.acao,
								mo2.qtdrealizada,
								mo2.qtdalterada,
								totalescola
							";
				//dbg($sql,1);
					
			}else{
					$sql .= "UNION
					
						SELECT  mun.muncod,
								ende.estuf as estado, 
								mun.mundescricao as municipio,
								'".$a['componente']."' as componente,
								'".$a['acao']."' as acao, 
								ent.entnome as escola,
								pac.pamquanteduseratend as qtdpactuada,
								(case when mo2.qtdrealizada is not null then
									mo2.qtdrealizada
								      else
									0
								end) as qtdrealizada,
								(case when pac.pamquanteduseratend > 0 then
										(COALESCE(mo2.qtdrealizada,0) * 100) / pac.pamquanteduseratend
								      else
										0
								end) as percentualatingido,
								(case when mo2.qtdalterada is not null then
									mo2.qtdalterada
								      else
									0
								end) as qtdalterada,
								(case when validadadiretor = 'S' then
									'Sim'
								      else
									'Não'
								end) as validada,
								dataavaliacao as avaliacao
							FROM pse.planoacaoc1 pac
							INNER JOIN
								entidade.entidade ent ON ent.entid = pac.entid
							INNER JOIN
								entidade.endereco ende ON ende.entid = ent.entid
							INNER JOIN
								territorios.municipio mun ON mun.muncod = ende.muncod
							LEFT JOIN
								(select a.avldataavaliacao::char as dataavaliacao, a.avleducandoavaliado AS qtdrealizada, a.avlidentifica1 AS qtdalterada, a.avlvalidar as validadadiretor, mo.entid, ac.acaodescricao as acao
								from pse.avaliacao a
								INNER JOIN
									pse.monitoraescola mo ON mo.moeid = a.moeid 
								INNER JOIN
									pse.acao ac ON ac.acaoid = mo.acaoid
								INNER JOIN
									pse.componenteavalia co ON co.comid = ac.comid
								WHERE mo.moestatus = 'A' AND mo.moeidentificasse in ('N','S') AND ac.acaoid = ".$a['acaoid']."
								".$where[2]."
								".$where[3]."
								
								) mo2 ON mo2.entid = ent.entid 
							WHERE
								ent.entstatus='A'
								".$where[0]."
								".$where[1]."
							GROUP BY 
								mun.muncod,
								ende.estuf,
								mun.mundescricao,
								ent.entcodent, ent.entnome,
								pac.pamquanteduseratend,
								mo2.acao,
								mo2.qtdrealizada,
								mo2.qtdalterada,
								mo2.validadadiretor,
								mo2.dataavaliacao
								
							";
					//dbg($sql,1);
				}
		}
		else{ // componente 2
			
			
			if($a['acaoid'] == 11){ // Ação 3
				
				$sql .= "UNION
					
						SELECT  mun.muncod,
								ende.estuf as estado, 
								mun.mundescricao as municipio,
								'".$a['componente']."' as componente,
								'".$a['acao']."' as acao, 
								ent.entnome as escola,
								mp.mpaacao04comp3/totalescola as qtdpactuada,
								(case when mo2.qtdrealizada is not null then
									mo2.qtdrealizada
								      else
									0
								end) as qtdrealizada,
								(case when (mp.mpaacao04comp3/totalescola) > 0 then
										(COALESCE(mo2.qtdrealizada,0) * 100) / (mp.mpaacao04comp3/totalescola)
								      else
										0
								end) as percentualatingido,
								0 as qtdalterada
							FROM pse.planoacaoc2 pac
							INNER JOIN
								pse.metapactuada mp ON mp.conid = pac.conid
							INNER JOIN
								entidade.entidade ent ON ent.entid = pac.entid
							INNER JOIN
								entidade.endereco ende ON ende.entid = ent.entid
							INNER JOIN
								territorios.municipio mun ON mun.muncod = ende.muncod
							LEFT JOIN
								(SELECT count(pacid) as totalescola, conid from pse.planoacaoc2 group by conid) te on te.conid = pac.conid
							LEFT JOIN
								(select sum(a.avcquantidade) AS qtdrealizada, mo.entid, ac.acaodescricao as acao
								from pse.avaliacomp2 a
								INNER JOIN
									pse.monitoraescola mo ON mo.moeid = a.moeid 
								INNER JOIN
									pse.acao ac ON ac.acaoid = mo.acaoid
								INNER JOIN
									pse.componenteavalia co ON co.comid = ac.comid
								WHERE mo.moestatus = 'A' AND mo.moeidentificasse in ('N','S') AND a.avcvalidar = 'S' AND ac.acaoid = ".$a['acaoid']."
								".$where[2]."
								".$where[3]."
								GROUP BY mo.entid, ac.acaodescricao
								) mo2 ON mo2.entid = ent.entid 
							WHERE
								ent.entstatus='A'
								".$where[0]."
								".$where[1]."
							GROUP BY 
								mun.muncod,
								ende.estuf,
								mun.mundescricao,
								ent.entcodent, ent.entnome,
								mp.mpaacao04comp3,
								mo2.acao,
								mo2.qtdrealizada,
								totalescola
								
							";
				
				
					
			}else{
				
				$sql .= "UNION
					
						SELECT  mun.muncod,
								ende.estuf as estado, 
								mun.mundescricao as municipio,
								'".$a['componente']."' as componente,
								'".$a['acao']."' as acao, 
								ent.entnome as escola,
								pac.pacquanteduseratend as qtdpactuada,
								(case when mo2.qtdrealizada is not null then
									mo2.qtdrealizada
								      else
									0
								end) as qtdrealizada,
								(case when pac.pacquanteduseratend > 0 then
										(COALESCE(mo2.qtdrealizada,0) * 100) / pac.pacquanteduseratend
								      else
										0
								end) as percentualatingido,
								0 as qtdalterada,
								(case when validadadiretor = 'S' then
									'Sim'
								      else
									'Não'
								end) as validada,
								dataavaliacao as avaliacao
							FROM pse.planoacaoc2 pac
							INNER JOIN
								entidade.entidade ent ON ent.entid = pac.entid
							INNER JOIN
								entidade.endereco ende ON ende.entid = ent.entid
							INNER JOIN
								territorios.municipio mun ON mun.muncod = ende.muncod
							LEFT JOIN
								(select a.avctipo::char as dataavaliacao, a.avcquantidade AS qtdrealizada, a.avcvalidar as validadadiretor, mo.entid, ac.acaodescricao as acao
								from pse.avaliacomp2 a
								INNER JOIN
									pse.monitoraescola mo ON mo.moeid = a.moeid 
								INNER JOIN
									pse.acao ac ON ac.acaoid = mo.acaoid
								INNER JOIN
									pse.componenteavalia co ON co.comid = ac.comid
								WHERE mo.moestatus = 'A' AND mo.moeidentificasse in ('N','S') AND ac.acaoid = ".$a['acaoid']."
								".$where[2]."
								".$where[3]."
								) mo2 ON mo2.entid = ent.entid 
							WHERE
								ent.entstatus='A'
								".$where[0]."
								".$where[1]."
							GROUP BY 
								mun.muncod,
								ende.estuf,
								mun.mundescricao,
								ent.entcodent, ent.entnome,
								pac.pacquanteduseratend,
								mo2.acao,
								mo2.qtdrealizada,
								mo2.validadadiretor,
								mo2.dataavaliacao
								
							";
				//dbg($sql,1);
			}
		}
		
		
	}
	
	$sql = substr($sql, 5);
	$sql = $sql . " ORDER BY 2, 3, 4, 5, 6 ";
	
	//dbg($sql,1);
	
	
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"qtdpactuada",
											"qtdrealizada",
											"percentualatingido",
											"qtdalterada",
											"validada"		
										  )	  
				);
				
	foreach ($agrupador as $val):
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);					
		    	continue;
		    break;
		    case 'componente':
				array_push($agp['agrupador'], array(
													"campo" => "componente",
											  		"label" => "Componente")										
									   				);					
		    	continue;
		    break;
		    case 'acao':
				array_push($agp['agrupador'], array(
													"campo" => "acao",
											  		"label" => "Acao")										
									   				);					
		    	continue;
		    break;
		    case 'escola':
				array_push($agp['agrupador'], array(
													"campo" => "escola",
											  		"label" => "Escola")										
									   				);					
		    	continue;
		    break;
		    case 'avaliacao':
				array_push($agp['agrupador'], array(
													"campo" => "avaliacao",
											  		"label" => "Avaliação")										
									   				);					
		    	continue;
		    break;
		}
		$count++;
	endforeach;
	
	return $agp;
}


function monta_coluna(){

	$coluna    = array(
						array(
							  "campo" => "qtdpactuada",
					   		  "label" => "Quantidade Pactuada",
							  "type"  => "numeric",
							  "blockAgp" => array('estado','municipio','componente') 
						),
						array(
							  "campo" => "qtdrealizada",
					   		  "label" => "Quantidade Realizada",
							  "type"  => "numeric",
							  "blockAgp" => array('estado','municipio','componente')
						),
						array(
							  "campo" => "percentualatingido",
					   		  "label" => "(%) Percentual Atingido",
							  "php"	  => array("expressao" => "{qtdpactuada} > 0",
											   "type"	   => "numeric",
											   "html"	   => "{percentual} %",
											   "var" 	   => "percentual",
											   "true"	   => "number_format(({qtdrealizada} * 100) / {qtdpactuada},2,',','.')",
											   "false"	   => "0"),
							  "type"  => "numeric",
							  "blockAgp" => array('estado','municipio','componente')
						),
						array(
							  "campo" => "qtdalterada",
					   		  "label" => "Quantidade Educandos com Alteração",
							  "type"  => "numeric",
							  "blockAgp" => array('estado','municipio','componente')
						),
						array(
							  "campo" => "avaliacao",
					   		  "label" => "Ação Validada pelo Diretor (Sim/Não)",
							  "type"  => "string",
							  "blockAgp" => array('estado','municipio','componente')
						)
					);
	return $coluna;	
/*	
	$arrPerfil = array(
						PERFIL_SUPERUSUARIO,
						PERFIL_ADMINISTRADOR
					  );
	
	$permissao = academico_possui_perfil($arrPerfil);
	
	$coluna    = array(
						array(
							  "campo" => "projetado",
					   		  "label" => "Projetado <br/>(A)",
							  "type"  => "numeric"
						),
	//					array(
	//						  "campo" => "autorizado",
	//				   		  "label" => "Autorizado",
	//						  "type"  => "numeric"
	//					),
						array(
							  "campo" => "autorizado",
					   		  "label" => "Concursos <br/>Autorizados <br/>(B)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "publicado",
					   		  "label" => "Concursos <br/>Publicados <br/>(C)",
							  "type"  => "numeric"
						)
					);
					
	if ( $permissao ){				
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric",
								  "html"  	 => "<span style='color:{color}'>{homologado}</span>",
								  "php" 	 => array(
									  					"expressao" => "{homologado} > {autorizado}",
														"var" 		=> "color",
														"true"	    => "red",
														"false"     => "#0066CC",
									  				 )	
							)
				  );			
	}else{
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric"
								 )
				  );			

	}
	
	array_push($coluna,array(
							  "campo" => "provimento",
					   		  "label" => "Provimentos <br/>Autorizados <br/>(E)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "lepvlrprovefetivados",
					   		  "label" => "Provimentos <br/>Efetivados <br/>(F)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "provimentosnaoefetivados",
					   		  "label" => "Provimentos <br/>Não Efetivados <br/>(G)=(E)-(F)",
							  "type"  => "numeric"
						)
				);	

				
	if ( $permissao ){				
		array_push($coluna, array(
									  "campo" => "provimentopendencia",
							   		  "label" => "Aut. Provimento <br/>Pendente <br/>(H)=(D)-(E)",
									  "type"  => "numeric",
									  "html"  => "{valorcampo}",
									  "php"   => array(
									  					"expressao" => "({homologado} - {provimento} >= 0) || ({provimentopendencia} > 0)",
														"type"		=> "string",
									  					"var"       => "valorcampo",
														"true"		=> "{provimentopendencia}",
														"false"		=> "<span style='color:red;'>-</span>"					
									  				   )
								)
				  );			
	}else{
		array_push($coluna, array(
									  "campo" => "provimentopendencia",
							   		  "label" => "Aut. Provimento <br/>Pendente <br/>(H)=(D)-(E)",
									  "type"  => "numeric"		
								  )
				  );			

	}			

	return $coluna;			  	
*/	
}
?>


<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
/*
$(function() {

	 //esconde todas as ações 7 do componete 1
	 $('[id*=componente_1_1acao_6_1_img]').hide();

	 //esconde todas as ações 3 do componete 2
	 $('[id*=componente_2_1acao_3_1_img]').hide();
	 
});
*/
</script>
