<?php

	if ($_REQUEST['filtrosession']){
		$filtroSession = $_REQUEST['filtrosession'];
	}
	
	if ($_POST['agrupador']){
		header('Content-Type: text/html; charset=iso-8859-1'); 
	}
	ini_set("memory_limit","500M");
	set_time_limit(0);
?>

<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
<?php
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	$sql   = monta_sql();
	$dados = $db->carregar($sql);
	$agrup = monta_agp();
	$col   = monta_coluna();
	
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
?>

</body>
</html>

<?php 
function monta_sql(){
	global $filtroSession;

	extract($_POST);
	
	if($estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){		
		$where[0] = " and en.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	
	if($municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " and m.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	
	if($memanoreferencia != ''){
		$where[2] = " and me.memanoreferencia = $memanoreferencia ";		
	}
	
	/*
	if($memanoreferencia == 'T'){
		$where[2] = " and me.memanoreferencia = '2012' ";		
	}elseif($memanoreferencia == '2011'){
		$where[2] = " and me.memanoreferencia = '2011' ";
	}elseif($memanoreferencia == '2012'){
		$where[2] = " and me.entcodent Not in (Select entcodent From pdeescola.memaiseducacao Where memstatus = 'A' and memanoreferencia = 2011)";
	}
	*/
	
	if($esfera != ''){
		$where[3] = " and e.tpcid in ($esfera) ";		
	}
	
	if($mempagofnde == 'S'){
		$where[4] = " and me.mempagofnde = 't' ";		
	}elseif( $mempagofnde == 'N'){
		$where[4] = " and me.mempagofnde = 'f' ";
	}else{
		$where[4] = "";
	}	

	if($memadesaopst == 'S'){
		$where[5] = " and me.memadesaopst = 'S' ";		
	}elseif($memadesaopst == 'N'){
		$where[5] = " and me.memadesaopst = 'N' ";
	}else{
		$where[5] = "";
	}

	if($mamescolaaberta == 'S'){
		$where[6] = " and me.mamescolaaberta = 't' ";		
	}elseif($mamescolaaberta == 'N'){
		$where[6] = " and me.mamescolaaberta = 'f' ";
	}else{
		$where[6] = "";
	}	

	if($membrasilsemmiseria == 'S'){
		$where[7] = " and me.membrasilsemmiseria = 't' ";		
	}elseif($membrasilsemmiseria == 'N'){
		$where[7] = " and me.membrasilsemmiseria = 'f' ";
	}else{
		$where[7] = "";
	}

	if($memclassificacaoescola == 'U'){
		$where[8] = " and  me.memclassificacaoescola = 'U' ";		
	}elseif ($memclassificacaoescola == 'R'){
		$where[8] = " and  me.memclassificacaoescola = 'R' ";
	}else{
		$where[8] = "";
	}
		
	$sql = "
			SELECT           DISTINCT me.memid,
                             en.estuf as estuf,
                             m.muncod as muncod,
                             m.mundescricao as mundescricao,
                             tc.tpcdesc as tpcdesc,
                             me.entcodent as entcodent,
                             me.memvlrpago as memvlrpago,
                             me.memdataadesaopst as memdataadesaopst,
                              e.entnumcpfcnpj as cnpj,
                              e.entnome as entnome,
                            (select coalesce(ORGAOFUNCAO.entnome,'Não Informado') 
                               from entidade.entidade ORGAOFUNCAO
                               inner join entidade.funcaoentidade fe
                                       on ORGAOFUNCAO.entid = fe.entid
                               inner join entidade.funentassoc f on fe.fueid = f.fueid 
                               where e.entid = f.entid
                                 and fe.funid = 19 
                                 and fe.fuestatus = 'A'  LIMIT 1 ) as diretor,
                            (select coalesce(ORGAOFUNCAO.entemail,'Não Informado') 
                               from entidade.entidade ORGAOFUNCAO
                               inner join entidade.funcaoentidade fe
                                       on ORGAOFUNCAO.entid = fe.entid
                               inner join entidade.funentassoc f on fe.fueid = f.fueid 
                               where e.entid = f.entid
                                 and fe.funid = 19 
                                 and fe.fuestatus = 'A'  LIMIT 1 ) as email,
                              e.entnumdddcomercial || ' - ' || e.entnumcomercial as telefone,
                             tl.tpldesc as tpldesc,
                             en.endlog || '-' || en.endbai  || '-' || en.endcom as endereco, 
                             en.endcep as endcep, 
                             Case When me.mempagofnde = 't' Then 'SIM' Else 'NÃO' End as mempagofnde,
                             Case When me.memadesaopst = 'S' Then 'SIM' Else 'NÃO' End as memadesaopst,
                             Case When me.mamescolaaberta = 't' Then 'SIM' Else 'NÃO' End as mamescolaaberta,
                             Case When me.membrasilsemmiseria = 't' Then 'SIM' Else 'NÃO' End as membrasilsemmiseria,
                             (SELECT coalesce(sum(mapquantidade),0) FROM pdeescola.mealunoparticipante WHERE memid = me.memid) as alunadoparticipante 
				
            FROM pdeescola.memaiseducacao me
			Left Join entidade.entidade e on me.entcodent = e.entcodent
			Left Join entidade.endereco en on e.entid = en.entid
			Left Join territorios.municipio m on en.muncod = m.muncod
			Left Join entidade.tipoclassificacao tc on tc.tpcid = e.tpcid
			Left Join entidade.tipolocalizacao tl on tl.tplid = e.tplid
                             
			Where me.memstatus = 'A' 
		
		--UTILIZAR UMA DAS TRÊS OPÇÕES ABAIXO Filtrar por Ano de Referência.
		--Estado
		".$where[0]."
		--Município.
		".$where[1]."
		--Escolas de anos de referencia 2011, 2012 e todos.
		".$where[2]."
		--Esfera.
		".$where[3]."
		--Pago FNDE.
		".$where[4]."
		--Adesao 2º tempo.
		".$where[5]."
		--Escola Aberta
		".$where[6]."
		--Brasil sem miseria.
		".$where[7]."
		--Localização da Escola Rural ou Urbana.
		".$where[8]."
				
			GROUP BY me.memid,
					 en.estuf,
					 m.muncod,
					 m.mundescricao,
					 tc.tpcdesc,
					 me.entcodent,
					 me.memvlrpago,
					 me.memdataadesaopst,
					  e.entnumcpfcnpj,
					  e.entnome,
					  e.entid,
					  e.entnumdddcomercial, e.entnumcomercial,
					 tl.tpldesc,
					 en.endlog, en.endbai, en.endcom, 
					 en.endcep,
					 me.mempagofnde,
					 me.memadesaopst,
					 me.mamescolaaberta,
					 me.membrasilsemmiseria

			ORDER BY en.estuf, m.muncod, me.entcodent";
	//ver($sql,d);
	//echo $sql;
	return $sql;
}

function monta_coluna(){
	
	$colunas = $_POST['colunas'];
	$colunas = $colunas ? $colunas : array();
	
	$coluna = array();
	
	foreach ($colunas as $val){
		switch ($val) {
			case 'estuf':
				array_push( $coluna,
								array(	"campo"		=> "estuf",
										"label" 	=> "Unidade Federativa",
									   	"type"	  	=> "string"
								)
				);
				continue;
			break;
			case 'mundescricao':
				array_push( $coluna, 
								array(	"campo" 	=> "mundescricao",
							   		   	"label"		=> "Município",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'tpcdesc':
				array_push( $coluna, 
								array(	"campo" 	=> "tpcdesc",
							   		   	"label"		=> "Esfera",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'entcodent':
				array_push( $coluna, 
								array(	"campo" 	=> "entcodent",
							   		   	"label"		=> "Código INEP",
							   		   	"type"	  	=> "string"
								)
				);
				continue;
			break;
			case 'cnpj':
				array_push( $coluna,
								array(	"campo" 	=> "cnpj",
										"label"		=> "CPF/CNPJ",
										"type"	  	=> "string"
								)
				);
				continue;
			case 'entnome':
				array_push( $coluna, 
								array(	"campo" 	=> "entnome",
							   		   	"label"		=> "Escola",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			case 'diretor':
				array_push( $coluna,
								array(	"campo" 	=> "diretor",
										"label"		=> "Diretor",
										"type"	  	=> "string"
								)
				);
				continue;
			case 'email':
				array_push( $coluna,
								array(	"campo" 	=> "email",
										"label"		=> "E-mail",
										"type"	  	=> "string"
								)
				);
				continue;								
			break;
			case 'tpldesc':
				array_push( $coluna, 
								array(	"campo" 	=> "tpldesc",
							   		   	"label"		=> "Localização",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;			
			case 'endereco':
				array_push( $coluna, 
								array(	"campo" 	=> "endereco",
							   		   	"label"		=> "Endereço",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'telefone':
				array_push( $coluna,
								array(	"campo" 	=> "telefone",
										"label"		=> "Telefones",
										"type"	  	=> "string"
								)
				);
				continue;				
			break;
			case 'mempagofnde':
				array_push( $coluna, 
								array(	"campo" 	=> "mempagofnde",
							   		   	"label"		=> "Recursos FNDE",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'memvlrpago':
				array_push( $coluna, 
								array(	"campo" 	=> "memvlrpago",
							   		   	"label"		=> "Recurso pago",
							   		   	"type"	  	=> "numeric"
								) 
				);
				continue;
			break;
			case 'memadesaopst':
				array_push( $coluna, 
								array(	"campo" 	=> "memadesaopst",
							   		   	"label"		=> "Adesão ao 2º tempo",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'memdataadesaopst':
				array_push( $coluna, 
								array(	"campo" 	=> "memdataadesaopst",
							   		   	"label"		=> "Data da adesão ao 2º tempo",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'mamescolaaberta':
				array_push( $coluna, 
								array(	"campo" 	=> "mamescolaaberta",
							   		   	"label"		=> "Escola Aberta",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'membrasilsemmiseria':
				array_push( $coluna, 
								array(	"campo" 	=> "membrasilsemmiseria",
							   		   	"label"		=> "Brasil sem miséria",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'alunadoparticipante':
				array_push( $coluna, 
								array(	"campo" 	=> "alunadoparticipante",
							   		   	"label"		=> "Alunado Participante",
							   		   	"type"	  	=> "numeric"
								) 
				);
				continue;
			break;			
		}
	}	
	//ver($coluna, d);	
	return $coluna;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		//ver($agrupador, d);
	$agp = 	array(	"agrupador" => array(), 
					"agrupadoColuna" => array("memid","estuf","mundescricao","tpcdesc","projetado","entcodent","telefone","cnpj","entnome","diretor","email","tpldesc","endereco","mempagofnde","memvlrpago","memadesaopst","memdataadesaopst","mamescolaaberta","membrasilsemmiseria",/*"total_aluno_censo",*/ "alunadoparticipante")	  
			);
			
	$count = 1;
	$i = 0;

	/*foreach( $_POST['esfera'] as $esfera => $valor ){
		if( $valor == 'm' ){
			$vari = "Relatório analitico Municipal<br>";
			$i++;
		} elseif( $valor == 'e' ){
			$vari = "Relatório analitico - Estadual<br>";
			$i++;
		}
	}*/
	
	//if( $i > 1 || $i == 0 ){
	//	$vari = "Relatório de Escolas Estadual e Municipal<br>";	
	//}
	
	foreach ($agrupador as $val){
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}	
		
		switch ($val) {
			case 'estuf':
				array_push($agp['agrupador'], array("campo" => "estuf", "label" => "$var Estado") );				
		   		continue;
		    break;
		    case 'mundescricao':
				array_push($agp['agrupador'], array("campo" => "mundescricao", "label" => "$var Município") );					
		    	continue;
		    break;		    	
		    case 'tpcdesc':
				array_push($agp['agrupador'], array("campo" => "tpcdesc", "label" => "$var Esfera"));					
		    	continue;			
		    break;
		    case 'entcodent':
				array_push($agp['agrupador'], array("campo" => "entcodent","label" => "$var INEP - Escola"));					
		   		continue;			
		    break;
		    case 'entnome':
				array_push($agp['agrupador'], array("campo" => "entnome","label" => "$var Escola"));					
		   		continue;			
		    break;		    
		}
		$count++;
	}
	#Agrupador padrão. 
	array_push($agp['agrupador'], array("campo" => "memid","label" => "Cód."));
		
	return $agp;
}
?>