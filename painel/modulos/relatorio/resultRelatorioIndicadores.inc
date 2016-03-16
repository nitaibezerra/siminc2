<?php

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

//ver ($_REQUEST,d);

if ($_REQUEST['filtrosession']){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ( $_REQUEST['salvar'] == 1 ){
	$existe_rel = 0;
	
	$sql = "select prtid from public.parametros_tela where prtdsc = '".$_POST['titulo_consulta']."' and usucpf ='".$_SESSION['usucpf']."'and mnuid =".$_SESSION['mnuid'].";";
	
	$existe_rel = $db->pegaUm( $sql );

	if ($existe_rel > 0)
	{
		$sql = sprintf(
				"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d, prtobservacao = '%s' WHERE prtid = %d",
				$_POST['titulo'],
				addslashes( addslashes( serialize( $_REQUEST ) ) ),
				$_SESSION['usucpf'],
				$_SESSION['mnuid'],
				$_POST['obs_consulta'],
				$existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else
	{
		$sql = sprintf(
					"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid, prtobservacao) VALUES ( '%s', '%s', %s, '%s', %d, '%s' )",
					$_POST['titulo'],
					addslashes( addslashes( serialize( $_REQUEST ) ) ),
					'FALSE',
					$_SESSION['usucpf'],
					$_SESSION['mnuid'],
					$_POST['obs_consulta']
			);
		$db->executar( $sql );
		$db->commit();
	}
	?>
<script type="text/javascript">
			alert('Operação realizada com sucesso!');
			location.href = '?modulo=relatorio/relatorioIndicadores&acao=A';
		</script>
<?
die;
}



if ($_POST['colunas'] ){
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
	
	
	//verifica se a opcao agruparunidades (unidade meta) nao foi selecionada
	if (! $_POST['agruparunidades'])
	{
		$sql = monta_sqlNaoAgrupado();
		$agrup = monta_agpNaoAgrupado();
		$col   = monta_colunaNaoAgrupado();
		
		$dados = $db->carregar($sql);
		
		$r = new montaRelatorio();
		$r->setAgrupador($agrup, $dados);
		$r->setColuna($col);
		$r->setBrasao(true);
		$r->setTotNivel(true);
		$r->setEspandir(false);
		$r->setMonstrarTolizadorNivel(true);
		$r->setTotalizador(true);
		$r->setTolizadorLinha(true);
		
		
		if($_POST['tipo_relatorio'] == 'xls'){
			ob_clean();
			$nomeDoArquivoXls="relatorio_analitico_".date('d-m-Y_H_i');
			echo $r->getRelatorioXls();
		}elseif($_POST['tipo_relatorio'] == 'visual'){
			echo $r->getRelatorio();
		}
	}
	else 
	{
		$sql   = monta_sql();
		$agrup = monta_agp();
		$col   = monta_coluna();
		
		$dados = $db->carregar($sql);
		
		$r = new montaRelatorio();
		$r->setAgrupador($agrup, $dados);
		$r->setColuna($col);
		$r->setBrasao(true);
		$r->setTotNivel(true);
		$r->setEspandir(false);
		$r->setMonstrarTolizadorNivel(true);
		$r->setTotalizador(true);
		$r->setTolizadorLinha(true);
		
		
		if($_POST['tipo_relatorio'] == 'xls'){
		ob_clean();
		$nomeDoArquivoXls="relatorio_analitico_".date('d-m-Y_H_i');
		echo $r->getRelatorioXls();
		}elseif($_POST['tipo_relatorio'] == 'visual'){
		echo $r->getRelatorio();
		}
		
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
				$sql .= " and  dshuf in ( ";
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
	
	function whereMesoregiao()
	{
		$valores = $_POST['slMesoregiao'];
		$sql = "";
		$ct = 0;
		if ( is_array( $valores) )
		{
			if (sizeof( $valores) > 0 && trim($valores[0]) != "" )
			{
				$sql .= " and  mes.mescod in ( ";
				foreach ($valores as $mes){
					if ( trim($mes) != "")
					{
						if ($ct > 0)
						{
							$sql .= ", ";
						}
		
						$ct .= 1;
						$sql .= "'". $mes."'";
					}
				}
				$sql .= ") ";
			}
		}
		return $sql;
	}
	
	
	function whereMicroregiao()
	{
		$valores = $_POST['slMicroregiao'];
		$sql = "";
		$ct = 0;
		if ( is_array( $valores) )
		{
			if (sizeof( $valores) > 0 && trim($valores[0]) != "" )
			{
				$sql .= " and  mun.miccod in ( ";
				foreach ($valores as $mic){
					if ( trim($mic) != "")
					{
						if ($ct > 0)
						{
							$sql .= ", ";
						}
		
						$ct .= 1;
						$sql .= "'". $mic."'";
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
				$sql .= " and  muncod in ( ";
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
	
	//funcao chamada quando nao e selecionado agrupar unidade em seleciondarIndicadores
	function monta_sqlNaoAgrupado(){
		global $db;
	
		$colunas = array();
		
		$sql = "select ";
		
	    if (in_array("dshuf",$_POST['colunas']) ){
			$sql .= "estuf as dshuf,";
		}
		
		if (in_array("mundescricao",$_POST['colunas']) ){
			$sql .= "mundescricao , substring ( muncod, 1 , 6) as muncod, ";
		}
		
		$ct = 1;
		foreach ($_POST['colunas'] as $col)
		{
			
			if (strpos( $col, "umeid") !== false)
			{
				$valores = explode(":",$col);
					
				$valores[2] = str_replace("(", "", $valores[2]);
				$valores[2] = str_replace(")", "", $valores[2]);
				$valores[2] = str_replace(" ", "", $valores[2]);
					
				$valoresAux = explode("-",$valores[2]);
			
				$sql1 = "select  indcumulativo from painel.indicador where indid = {$valoresAux[1]}";
				$cumulativo = $db->pegaUm($sql1);
				
				switch ($cumulativo)
				{
					case 'S':
						$sql1 =  "(select sum (dshqtde) from   painel.detalheseriehistorica d inner join painel.seriehistorica s
						on d.sehid = s.sehid inner join painel.indicador i on i.indid = s.indid where i.indid = {$valoresAux[1]}   ";
						break;
						case 'A':
						$sql1 = "(select sum (dshqtde) from   painel.detalheseriehistorica d inner join painel.seriehistorica s
						on d.sehid = s.sehid inner join painel.indicador i on i.indid = s.indid
						inner join painel.detalheperiodicidade dpe on s.dpeid = dpe.dpeid
						where i.indid = {$valoresAux[1]} and  dpe.dpeanoref = (select max (dpeanoref) from painel.detalheperiodicidade where  dpeid in
						( select dpeid from painel.seriehistorica where indid = {$valoresAux[1]}) ";
						break;
						case 'N':
						$sql1 = "(select sum (dshqtde) from   painel.detalheseriehistorica d inner join painel.seriehistorica s
						on d.sehid = s.sehid inner join painel.indicador i on i.indid = s.indid
						inner join painel.detalheperiodicidade dpe on s.dpeid = dpe.dpeid
						where i.indid = {$valoresAux[1]} and dpe.dpeid = (select max (dpeid) from painel.seriehistorica where indid = {$valoresAux[1]}) ";
						break;		
				}
				
				
				if (in_array("dshuf",$_POST['colunas']) && ! in_array("mundescricao",$_POST['colunas'] )){
					$sql1 .= " and dshuf = e1.estuf group by dshuf";
				}
				
				if (in_array("mundescricao",$_POST['colunas']) ){
					$sql1 .= " and dshcodmunicipio = m1.muncod ";
				}
				
				$sql1 .= ") as {$valoresAux[0]}{$valoresAux[1]}"; 
				
				$ct.=1;
				$selects[] = $sql1;
			}
		}

	
		$sql .= implode(",", $selects);
		
		if (in_array("dshuf",$_POST['colunas']) && ! in_array("mundescricao",$_POST['colunas'] ) ){
			$sql .= " from territorios.estado e1 where 1=1 ";
		}
		
		if (in_array("mundescricao",$_POST['colunas']) ){
			$sql .= " from territorios.municipio m1 where muncod is not null "; 
					
		}
		
		$sql .= str_replace("dshuf", "estuf", whereEstados()) ;
		
		if (in_array("mundescricao",$_POST['colunas']) ){
			$sql.= wheremunicipio();
			$sql.= " order by mundescricao";
		}
		//ver($sql,d);
		return $sql;
	}
	
	
	function monta_sql(){
		global $filtroSession;

		extract($_POST);
		
		$valores = implode(",",  $_POST['agrupador']);
		
		$sql1 = "select {$valores}  ";
		
		foreach ( $_POST['colunas'] as $col)
		{
			if (strpos( $col, "umeid") !== false)
			{
				$valores = explode(":",$col);
		
				$valores[2] = str_replace("(", "", $valores[2]);
				$valores[2] = str_replace(")", "", $valores[2]);
				$valores[2] = str_replace(" ", "", $valores[2]);
		
				$sql1 .= ", sum({$valores[2]}) as {$valores[2]}";
			}
		}
		
		$dshvalor = false;
		if (in_array("dshvalor",$_POST['colunas']) )
		{
			$dshvalor = true;
			$sql1 .= " , sum( dshvalor) as dshvalor";
		}
				
		$sql1 .= " from ( ";
				
		$colunas = "";
		
		if(in_array("indnome",$_POST['colunas'])  || in_array("indnome",$_POST['agrupador']) ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " ind.indid || ' - ' || ind.indnome as indnome  " ;
		}

		if(in_array("tiddsc",$_POST['colunas'])  || in_array("tiddsc",$_POST['agrupador']) ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " tid.tiddsc " ;
		}

		if(in_array("tiddsc2",$_POST['colunas'])  || in_array("tiddsc2",$_POST['agrupador']) ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " tid2.tiddsc as tiddsc2 " ;
		}

		if(in_array("dshuf",$_POST['colunas'])  || in_array("dshuf",$_POST['agrupador']) ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " dshuf " ;
		}

		if( in_array("paiid",$_POST['agrupador']) ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " reg.paiid " ;
		}

		if(in_array("dshqtde",$_POST['colunas'])  ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " dshqtde " ;
		}

		if( in_array("micdsc",$_POST['colunas']) || in_array("micdsc",$_POST['agrupador']) ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " micdsc " ;
		}

		if( in_array("mesdsc",$_POST['colunas']) || in_array("mesdsc",$_POST['agrupador']) ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " mesdsc " ;
		}


		if( in_array("mundescricao",$_POST['colunas']) || in_array("mundescricao",$_POST['agrupador']) ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " mundescricao " ;
		}
		
		if( in_array("dpeanoref",$_POST['colunas']) || in_array("dpeanoref",$_POST['agrupador'])    ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " dpeanoref " ;
		}
		
		
		if( in_array("umeid",$_POST['colunas'])  ){
			if ($colunas != "")
				$colunas .= ",";
			$colunas .= " dpeanoref " ;
		}
		
		$from .=  " from painel.indicador ind
			inner join painel.seriehistorica sh on sh.indid=ind.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid";

		if(in_array("mesdsc",$_POST['agrupador']) || ($_POST['slMesoregiao'] &&  $_POST['slMesoregiao'][0] != "" )
		|| in_array("mesdsc",$_POST['colunas'])	 || in_array("micdsc",$_POST['colunas'])	|| in_array("micdsc",$_POST['agrupador'])  || ($_POST['slMicroregiao'] &&  $_POST['slMicroregiao'][0] != "" )
		|| in_array("mundescricao",$_POST['colunas']) || in_array("mundescricao",$_POST['agrupador']) || ($_POST['slMunicipio'] &&  $_POST['slMunicipio'][0] != "" ))
		{
			$from .= " inner join territorios.municipio mun on dsh.dshcodmunicipio = mun.muncod
				inner join territorios.mesoregiao mes on mes.mescod = mun.mescod
				inner join territorios.microregiao mic on mic.miccod = mun.miccod ";
		}

		if(in_array("paiid",$_POST['agrupador']) )
		{
			$from .= " inner join territorios.estado est on est.estuf = dshuf ";
			$from .= " inner join territorios.regiao reg on reg.regcod = est.regcod ";
		}

		if(in_array("tiddsc",$_POST['colunas']) ||  in_array("tiddsc",$_POST['agrupador'])  ){
			$from .= " left join painel.detalhetipodadosindicador tid on tid.tidid = tidid1 ";
		}

		if(in_array("tiddsc2",$_POST['colunas']) ||  in_array("tiddsc2",$_POST['agrupador']) ){
			$from .= " left join painel.detalhetipodadosindicador tid2 on tid2.tidid = tidid2 ";
		}
		
		
		$wheresqlaux1 =  " where INDCUMULATIVO = 'S' ";
		
		$wheresqlaux1 .= " and ind.indid in ( ";
		
		$valores = $_POST['slIndicadores'];
		
		$indicadores = "";
		$ct = 0;
		
		if ( is_array( $valores) )
		{
			foreach ($valores as $ind){
				if ($ct > 0)
				{
					$indicadores .= ", ";
				}
		
				$ct .= 1;
				$indicadores .=  $ind;
			}
		}
		
		$wheresqlaux1 .= $indicadores. ")";
		
		$wheresqlaux1 .= whereEstados();
		$wheresqlaux1 .= whereMesoregiao();
		$wheresqlaux1 .= whereMicroregiao();
		$wheresqlaux1 .= whereMunicipio();
		
		
		$sqlaux1 = $wheresqlaux1 ;
		
		if ($dshvalor)
		{
			$sqlaux1 .= " union all select {$colunas} , dshvalor ";
			$sqlaux1 .= monta_subselect(true);
			$sqlaux1 .=$from;
			$sqlaux1 .=  str_replace("INDCUMULATIVO", "INDCUMULATIVOVALOR", $wheresqlaux1) ;
		}
		
		$sqlaux2 =  " union all select {$colunas} , 0 as dshvalor ";
		$sqlaux2 .= monta_subselect(false);
		$wheresqlaux2 .= " where INDCUMULATIVO = 'A'  AND ( ";  

		$ct = 0;
		foreach ($valores as $indicador){
			if ($ct > 0){
				$wheresqlaux2 .= " OR ";
			}
			$ct.=1;
			$wheresqlaux2 .= " (ind.indid = {$indicador}  and  dpe.dpeanoref = (select max (dpeanoref) from painel.detalheperiodicidade where  dpeid in ( select dpeid from painel.seriehistorica where indid = {$indicador}))) ";
		}
		
		$wheresqlaux2 .= ") ";
		
		$wheresqlaux2 .= whereEstados();
		$wheresqlaux2 .= whereMesoregiao();
		$wheresqlaux2 .= whereMicroregiao();
		$wheresqlaux2 .= whereMunicipio();
		
		$sqlaux2 .=$from;
		$sqlaux2 .= $wheresqlaux2;
		
		if ($dshvalor)
		{
			$sqlaux2 .= " union all select {$colunas} , dshvalor ";
			$sqlaux2 .= monta_subselect(true);
			$sqlaux2 .=$from;
			$sqlaux2 .=  str_replace("INDCUMULATIVO", "INDCUMULATIVOVALOR", $wheresqlaux2) ;
		}
		
		
		$sqlaux3 =  " union all select {$colunas} , 0 as dshvalor ";
		$sqlaux3 .= monta_subselect(false);
		$wheresqlaux3 .= " where INDCUMULATIVO = 'N' AND ( ";
		
		$ct = 0;
		foreach ($valores as $indicador){
			if ($ct > 0){
				$wheresqlaux3 .= " OR ";
			}
			$ct.=1;
			$wheresqlaux3 .= " (ind.indid = {$indicador}  and  dpe.dpeid = (select max (dpeid) from painel.seriehistorica where indid = {$indicador})) ";
		}
		
		$wheresqlaux3 .= ") ";
		
		$wheresqlaux3 .= whereEstados();
		$wheresqlaux3 .= whereMesoregiao();
		$wheresqlaux3 .= whereMicroregiao();
		$wheresqlaux3 .= whereMunicipio();
		
		$sqlaux3 .=$from;
		$sqlaux3 .= $wheresqlaux3;	
		
		if ($dshvalor)
		{
			$sqlaux3 .= " union all select {$colunas} , dshvalor ";
			$sqlaux3 .= monta_subselect(true);
			$sqlaux3 .=$from;
			$sqlaux3 .= str_replace("INDCUMULATIVO", "INDCUMULATIVOVALOR", $wheresqlaux3) ;
		}

		
		$sql = $sql1."Select".$colunas.",0 as dshvalor ";
		$sql .= monta_subselect(false);
		$sql .=$from;		
		$sql .=$sqlaux1.$sqlaux2.$sqlaux3;
		$sql .= " ) as foo";
		$sql .= monta_groupby();
		$sql .= monta_orderby();
		
		//ver ($sql);
		return $sql;
	}
	

	
	function monta_groupby()
	{
		$valores = $_POST['agrupador'];
		$sql = "";
		$ct = 0;
		if ( is_array( $valores) )
		{
			
			if (sizeof( $valores) > 0 && trim($valores[0]) != "" )
			{
				$sql .= " group by ";
				foreach ($valores as $group){
					if ( trim($group) != "")
					{
						if ($ct > 0)
						{
							$sql .= ", ";
						}
		
						$ct .= 1;
						$sql .=  $group;
					}
				}
			}
		}
		return $sql;
	}
	
	function monta_orderby()
	{
		$valores = $_POST['agrupador'];
		$sql = "";
		$ct = 0;
		if ( is_array( $valores) )
		{
			$sql = "";
			if (sizeof( $valores) > 0 && trim($valores[0]) != "" )
			{
				$sql .= " order by ";
				foreach ($valores as $sort){
					if ( trim($sort) != "")
					{
						if ($ct > 0)
						{
							$sql .= ", ";
						}
		
						$ct .= 1;
						$sql .=  $sort;
					}
				}
			}
		}
		return $sql;
	}
	
	
	function monta_subselect($comdshvalor)
	{
		
			$resposta = ""; 
			foreach ( $_POST['colunas'] as $col)
			{
				if (strpos( $col, "umeid") !== false)
				{
					$valores = explode(":",$col);
			
					$valores[2] = str_replace("(", "", $valores[2]);
					$valores[2] = str_replace(")", "", $valores[2]);
					$valores[2] = str_replace(" ", "", $valores[2]);
			
					
					if (!$comdshvalor)
					{
						$resposta .= " , (select dshqtde from   painel.detalheseriehistorica d inner join painel.seriehistorica s
						on d.sehid = s.sehid inner join painel.indicador i on i.indid = s.indid where dshid = dsh.dshid and i.indid =
						ind.indid and i.umeid = {$valores[1]} ) as {$valores[2]}";
					}
					else
					{
					   $resposta .= " ,0 as {$valores[2]}";
					}
				}
			}
			
			return $resposta;
	}
	
	//funcao chamada quando nao e selecionado agrupar unidade em seleciondarIndicadores
	function monta_colunaNaoAgrupado(){
	
		$colunas = $_POST['colunas'];
		$colunas = $colunas ? $colunas : array();
	
		$coluna = array();

		if (in_array("mundescricao",$_POST['colunas']) ){
			array_push( $coluna,
			array(	"campo"		=> strtolower( "mundescricao"),
			"label" 	=>  "Município",
			"type"	  	=> "string")
			);
		}
		
		if (in_array("dshuf",$_POST['colunas']) ){
			array_push( $coluna,
			array(	"campo"		=> strtolower( "dshuf"),
			"label" 	=>  "Estado",
			"type"	  	=> "string")
			);
		}
		
		
		
		foreach ($colunas as $val){
			
			if (strpos( $val, "umeid") !== false)
			{
	
				$valores = explode(":",$val);
					
				$valores[2] = str_replace("(", "", $valores[2]);
				$valores[2] = str_replace(")", "", $valores[2]);
				$valores[2] = str_replace(" ", "", $valores[2]);
					
				$valoresAux = explode("-",$valores[2]);

				array_push( $coluna,
				array(	"campo"		=> strtolower( $valoresAux[0].$valoresAux[1]),
				"label" 	=>  $valoresAux[0].$valoresAux[1],
				"type"	  	=> "numeric")
				);
				
			}
			
		}
		

		
		//ver($coluna,d);
		return $coluna;
	}
	
	function monta_coluna(){

		$colunas = $_POST['colunas'];
		$colunas = $colunas ? $colunas : array();

		$coluna = array();

		//ver($colunas);
		foreach ($colunas as $val){
			switch ($val) {
				case 'indnome':
					array_push( $coluna,
					array(	"campo"		=> "indnome",
					"label" 	=> "Indicador",
					"type"	  	=> "string"
						)
					);
					continue;
					break;
				case 'tiddsc':
					array_push( $coluna,
					array(	"campo"		=> "tiddsc",
					"label" 	=> "Detalhe 01",
					"type"	  	=> "string")
					);
					continue;
					break;
				case 'tiddsc2':
					array_push( $coluna,
					array(	"campo"		=> "tiddsc2",
					"label" 	=> "Detalhe 02",
					"type"	  	=> "string")
					);
					continue;
					break;
				case 'dshqtde':
					array_push( $coluna,
					array(	"campo"		=> "dshqtde",
					"label" 	=> "Quantidade",
					"type"	  	=> "numeric")
					);
					continue;
					break;
				case 'dshvalor':
					array_push( $coluna,
					array(	"campo"		=> "dshvalor",
					"label" 	=> "Valor",
					"type"	  	=> "")
					);
					continue;
					break;
				case 'dshuf':
					array_push( $coluna,
					array(	"campo"		=> "dshuf",
					"label" 	=> "Estado",
					"type"	  	=> "string")
					);
					continue;
					break;
				case 'micdsc':
					array_push( $coluna,
					array(	"campo"		=> "micdsc",
					"label" 	=> "Microrregião",
					"type"	  	=> "string")
					);
					continue;
					break;
				case 'mesdsc':
					array_push( $coluna,
					array(	"campo"		=> "mesdsc",
					"label" 	=> "Mesorregião",
					"type"	  	=> "string")
					);
					continue;
					break;
				case 'mundescricao':
					array_push( $coluna,
					array(	"campo"		=> "mundescricao",
					"label" 	=> "Município",
					"type"	  	=> "string")
					);
					continue;
					break;
				case 'dpeanoref':
					array_push( $coluna,
					array(	"campo"		=> "dpeanoref",
					"label" 	=> "Ano",
					"type"	  	=> "string")
					);
					continue;
					break;
			}
			
			
			//as colunas geradas dinamicamente vem com a unidade meta, umeid na definicao.
			if (strpos( $val, "umeid") !== false)
			{
				
				//unmid=5
				$valores = explode(":",$val);
					
				$valores[2] = str_replace("(", "", $valores[2]);
				$valores[2] = str_replace(")", "", $valores[2]);
				$valores[2] = str_replace(" ", "", $valores[2]);
				
				if ($valores[1] == 52)
				{
					array_push( $coluna,
					array(	"campo"		=> strtolower( $valores[2]),
					"label" 	=> $valores[2],
					"type"	  	=> "")
					);
				}
				else
				{
					array_push( $coluna,
					array(	"campo"		=> strtolower( $valores[2]),
					"label" 	=> $valores[2],
					"type"	  	=> "numeric")
					);
				}
			}
			
		}
		
		return $coluna;
	}
	
	//funcao chamada quando nao e selecionado agrupar unidade em seleciondarIndicadores
	function monta_agpNaoAgrupado(){

		$agp = 	array(	"agrupador" => array(),
				"agrupadoColuna" => array( "dshuf","muncod", "estuf", "mundescricao")
		);
	
		foreach ( $_POST['colunas'] as $col)
		{
			if (strpos( $col, "umeid") !== false)
			{
				$valores = explode(":",$col);
					
				$valores[2] = str_replace("(", "", $valores[2]);
				$valores[2] = str_replace(")", "", $valores[2]);
				$valores[2] = str_replace(" ", "", $valores[2]);
					
				$valoresAux = explode("-",$valores[2]);
					
				array_push($agp['agrupadoColuna'], strtolower( $valoresAux[0].$valoresAux[1]) );
			}
		}
				
		if (in_array('dshuf', $_POST['colunas']) && in_array('mundescricao', $_POST['colunas']))
		{
			array_push($agp['agrupador'], array("campo" => "muncod", "label" => "Cód IBGE") );
		}
		elseif (in_array('dshuf', $_POST['colunas']))
		{
			array_push($agp['agrupador'], array("campo" => "dshuf", "label" => "$var Estado") );
		}
		else
		{
			array_push($agp['agrupador'], array("campo" => "muncod", "label" => "Cód IBGE") );
		}
		
		//else 
		//{
			//array_push($agp['agrupador'], array("campo" => "indnome", "label" => "Indicador") );
		//}
		
		return $agp;
	}
	
	
	
	function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = 	array(	"agrupador" => array(),
					"agrupadoColuna" => array( "indnome" ,"tiddsc", "tiddsc2", "paiid" ,"dshqtde" , "dshvalor"  ,"dshuf", "mesdsc", "micdsc", "mundescricao", "dpeanoref")
			);

	foreach ( $_POST['colunas'] as $col)
	{
		if (strpos( $col, "umeid") !== false)
		{
			$valores = explode(":",$col);
	
			$valores[2] = str_replace("(", "", $valores[2]);
			$valores[2] = str_replace(")", "", $valores[2]);
			$valores[2] = str_replace(" ", "", $valores[2]);
			
			
			array_push($agp['agrupadoColuna'], strtolower( $valores[2]) );
		}
	}
	


	$count = 1;
	$i = 0;


	if ($agrupador)
	{
		foreach ($agrupador as $val){
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";
		}

		switch ($val) {
			case 'indnome':
				array_push($agp['agrupador'], array("campo" => "indnome", "label" => "$var Indicador") );
				continue;
				break;
			case 'dshuf':
				array_push($agp['agrupador'], array("campo" => "dshuf", "label" => "$var Estado") );
				continue;
				break;
			case 'mesdsc':
				array_push($agp['agrupador'], array("campo" => "mesdsc", "label" => "$var Mesorregião") );
				continue;
				break;
			case 'paiid':
				array_push($agp['agrupador'], array("campo" => "paiid", "label" => "$var Brasil") );
				continue;
				break;
			case 'micdsc':
				array_push($agp['agrupador'], array("campo" => "micdsc", "label" => "$var Microrregião") );
				continue;
				break;
			case 'tiddsc':
				array_push($agp['agrupador'], array("campo" => "tiddsc", "label" => "$var Detalhe 01") );
				continue;
				break;
			case 'tiddsc2':
				array_push($agp['agrupador'], array("campo" => "tiddsc2", "label" => "$var Detalhe 02") );
				continue;
				break;
			case 'mundescricao':
				array_push($agp['agrupador'], array("campo" => "mundescricao", "label" => "$var Município") );
				continue;
				break;
			case 'dpeanoref':
				array_push($agp['agrupador'], array("campo" => "mundescricao", "label" => "$var Ano") );
				continue;
				break;
		}
		$count++;
	}
	}

	return $agp;
}
?>

</body>
</html>
