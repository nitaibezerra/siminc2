<?php
if ($_REQUEST['filtrosession']){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
		header('Content-Type: text/html; charset=iso-8859-1');
}
?>
<html>
<head>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css' />
</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">

	<?php
	include APPRAIZ. 'includes/classes/relatorio.class.inc';

	//RECUPERA O ÚLTIMO SALDO CARREGADO 
	$sql = "SELECT DISTINCT dfimesanosaldo, dfidatasaldo
			FROM (
				SELECT pre.preid, sum(po.pobvalorpagamento) AS valorpgto, em.empnumeroprocesso, muncod
				FROM par.pagamento p
				INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid
				INNER JOIN obras.preobra pre ON pre.preid = po.preid AND pre.prestatus = 'A'
				INNER JOIN par.empenho em ON em.empid = p.empid and empstatus = 'A'
				WHERE p.pagstatus = 'A'
				GROUP BY pre.preid, em.empnumeroprocesso, muncod
			UNION ALL
				SELECT pre.preid, sum(po.popvalorpagamento) AS valorpgto, em.empnumeroprocesso, muncod
				FROM par.pagamento p
				INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid
				INNER JOIN obras.preobra pre ON pre.preid = po.preid AND pre.prestatus = 'A'
				INNER JOIN par.empenho em ON em.empid = p.empid and empstatus = 'A'
				WHERE p.pagstatus = 'A'
				GROUP BY pre.preid, em.empnumeroprocesso, muncod
			) AS FOO
			INNER JOIN painel.dadosfinanceirosconvenios df ON df.dfiprocesso = FOO.empnumeroprocesso
			ORDER BY dfidatasaldo DESC
			LIMIT 1";
	$_POST['dfimesanosaldo'] = $db->pegaUm($sql);
	
	$sql   = monta_sql();

	//ver($sql,d);
	
	$dados = $db->carregar($sql);
	$agrup = monta_agp();
	$col   = monta_coluna();

	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados);
	$r->setColuna($col);
	$r->setBrasao(true);
	$r->setTotNivel(false);
	$r->setEspandir(false);
	$r->setMonstrarTolizadorNivel(false);
	$r->setTotalizador(true);
	$r->setTolizadorLinha(false);

	if($_POST['tipo_relatorio'] == 'xls'){
		ob_clean();
		$nomeDoArquivoXls="relatorio_analitico_".date('d-m-Y_H_i');
		echo $r->getRelatorioXls();
	}elseif($_POST['tipo_relatorio'] == 'visual'){
		echo $r->getRelatorio();
	}
	?>

	<?php 
	function monta_sql(){
		global $filtroSession;

		//extract($_POST);
		
		$sql = "SELECT foo.preid, obr.obrid, obr.obrdesc, sto.stodesc, valorpgto, dfisaldoconta, dfisaldofundo, dfisaldopoupanca, dfisaldordbcdb, totalaplicacao, coalesce(dfimesanosaldo, 'Saldo Não Informado') as dfimesanosaldo, --obr.obrpercexec, totalconta,
				CASE
					WHEN totalconta >= valorpgto THEN '<font color=blue>' || to_char(totalconta, '999G999G990D99') || '</font>' --AZUL
				ELSE
					'<font color=red>' || to_char(totalconta, '999G999G990D99') || '</font>' --VERMELHO
				END AS totalconta,
				CASE 
					WHEN totalconta < valorpgto THEN
						CASE 
							WHEN obr.obrpercexec = 0 OR obr.obrpercexec IS NULL THEN '<b><font color=purple>' || to_char(coalesce(obr.obrpercexec,0), '999G999G990D99') || '</font></b>' --ROXO
						ELSE
							'<b><font color=blue>' || to_char(obr.obrpercexec, '999G999G990D99') || '</font></b>' --AZUL
						END
					ELSE to_char(coalesce(obr.obrpercexec,0), '999G999G990D99')
				END AS obrpercexec,
				m.estuf, ent.entnome as unidade
			FROM (
				SELECT pre.preid, sum(po.pobvalorpagamento) AS valorpgto, em.empnumeroprocesso, muncod
				FROM par.pagamento p
				INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid
				INNER JOIN obras.preobra pre ON pre.preid = po.preid AND pre.prestatus = 'A'
				INNER JOIN par.empenho em ON em.empid = p.empid and empstatus = 'A'
				WHERE p.pagstatus = 'A'
				GROUP BY pre.preid, em.empnumeroprocesso, muncod
			UNION ALL
				SELECT pre.preid, sum(po.popvalorpagamento) AS valorpgto, em.empnumeroprocesso, muncod
				FROM par.pagamento p
				INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid
				INNER JOIN obras.preobra pre ON pre.preid = po.preid AND pre.prestatus = 'A'
				INNER JOIN par.empenho em ON em.empid = p.empid and empstatus = 'A'
				WHERE p.pagstatus = 'A'
				GROUP BY pre.preid, em.empnumeroprocesso, muncod
			) AS FOO
			LEFT JOIN 
				(SELECT sum(dfisaldoconta) AS dfisaldoconta,
					sum(dfisaldofundo) AS dfisaldofundo, 
					sum(dfisaldopoupanca) AS dfisaldopoupanca, 
					sum(dfisaldordbcdb) AS dfisaldordbcdb, 
					(sum(dfisaldofundo) + sum(dfisaldopoupanca) + sum(dfisaldordbcdb)) AS totalaplicacao,
					(sum(dfisaldoconta) + sum(dfisaldofundo) + sum(dfisaldopoupanca) + sum(dfisaldordbcdb)) AS totalconta,
					dfiprocesso, dfimesanosaldo
				FROM painel.dadosfinanceirosconvenios
				WHERE dfimesanosaldo = '".$_POST['dfimesanosaldo']."'
				GROUP BY dfiprocesso, dfimesanosaldo) AS df ON df.dfiprocesso = FOO.empnumeroprocesso
			LEFT JOIN obras.obrainfraestrutura obr ON obr.preid = FOO.preid AND obr.obsstatus = 'A'
			LEFT JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
			inner join territorios.municipio m on m.muncod = foo.muncod
			left join entidade.entidade ent on ent.entid = obr.entidunidade
			where 1 = 1 ".whereEstados()."".whereMunicipio()." 
			ORDER BY obr.obrid, obr.obrdesc, sto.stodesc";
			
		return $sql;
	}

	
	function whereEstados()
	{
		$valores = $_POST['slEstado'];
	
		$sql = "";
		$ct = 0;
		if ( is_array( $valores) )
		{
			if (sizeof( $valores) > 0 && trim($valores[0]) != "" )
			{
				$sql .= " and  estuf in ( ";
				foreach ($valores as $uf){
					if ( trim($uf) != "")
					{
						if ($ct > 0)
						{
							$sql .= ", ";
						}
	
						$ct .= 1;
						$sql .= "'". $uf."'";
					}
				}
				$sql .= ") ";
			}
		}
	
		return $sql;
	}
	
	function whereMunicipio()
	{
		$valores = $_POST['slMunicipio'];
		$sql = "";
		$ct = 0;
		if ( is_array( $valores) )
		{
	
			if (sizeof( $valores) > 0 && trim($valores[0]) != "" )
			{
				$sql .= " and  m.muncod in ( ";
				foreach ($valores as $mun){
					if ( trim($mun) != "")
					{
						if ($ct > 0)
						{
							$sql .= ", ";
						}
	
						$ct .= 1;
						$sql .= "'". $mun."'";
					}
				}
				$sql .= ") ";
			}
		}
		return $sql;
	}
	
	function monta_coluna(){

		$coluna = array(		array(	"campo"		=> "preid",
		"label" 	=> "ID Pré-Obra",
		"type"	  	=> "string"
				));
	
		
		array_push( $coluna,
		array(	"campo"		=> "obrdesc",
		"label" 	=> "Nome da Obra",
		"type"	  	=> "string",
		"html" => "<div style='text-align:left'>{obrdesc}</div>"
				)
		);
		
		array_push( $coluna,
		array(	"campo"		=> "estuf",
		"label" 	=> "UF",
		"type"	  	=> "string"
				)
		);
		
		array_push( $coluna,
		array(	"campo"		=> "unidade",
		"label" 	=> "Unidade Implantadora",
		"type"	  	=> "string"
				)
		);
		
		array_push( $coluna,
		array(	"campo"		=> "stodesc",
		"label" 	=> "Situação",
		"type"	  	=> "string"
				)
		);
		
		array_push( $coluna,
		array(	"campo"		=> "valorpgto",
		"label" 	=> "Valor Pago",
		"type"	  	=> "",
				)
		);
		
		array_push( $coluna,
		array(	"campo"		=> "dfisaldoconta",
		"label" 	=> "Saldo da Conta Corrente<br>(A)",
		"type"	  	=> ""
				)
		);
		
		//array_push( $coluna,
		//array(	"campo"		=> "dfisaldofundo",
		//"label" 	=> "Saldo do Fundo",
		//"type"	  	=> ""
		//		)
		//);
		
		//array_push( $coluna,
		//array(	"campo"		=> "dfisaldopoupanca",
		//"label" 	=> "Saldo da Poupança",
		//"type"	  	=> ""
		//		)
		//);
		
		//array_push( $coluna,
		//array(	"campo"		=> "dfisaldordbcdb",
		//"label" 	=> "Saldo do RDBCDB",
		//"type"	  	=> ""
		//		)
		//);
		
		array_push( $coluna,
		array(	"campo"		=> "totalaplicacao",
		"label" 	=> "Saldo de Aplicações<br>(B)",
		"type"	  	=> ""
				)
		);
		
		array_push( $coluna,
		array(	"campo"		=> "totalconta",
		"label" 	=> "Total Conta<br>(A+B)",
		"html"	  	=> "<b>{totalconta}</b>",
		"title"		=> "Azul(Total Conta >= Valor Pago) e Vermelho(Total Conta < Valor Pago)"
				)
		);
		
		array_push( $coluna,
		array(	"campo"		=> "dfimesanosaldo",
		"label" 	=> "Data do Saldo",
		"type"	  	=> "string"
				)
		);
		
		array_push( $coluna,
		array(	"campo"		=> "obrpercexec",
		"label" 	=> "% de Execução",
		"type"	  	=> "",
		"title"		=> "Se Total Conta Vermlho, então: Azul(% > 0) e Roxo(% <= 0)"
				)
		);
		
		return $coluna;
	}

	function monta_agp(){

	$agp = 	array(	"agrupador" => array(),
					"agrupadoColuna" => array('preid', 'obrdesc', 'estuf', 'unidade', 'stodesc', 'valorpgto', 'dfisaldoconta', 'dfisaldofundo', 'dfisaldopoupanca', 'dfisaldordbcdb', 'totalaplicacao', 'totalconta', 'dfimesanosaldo', 'obrpercexec','SaldoXPagto','Percentual')
			);

		array_push($agp['agrupador'], array("campo" => "obrid", "label" => "ID") );


	return $agp;
}
?>

</body>
</html>
