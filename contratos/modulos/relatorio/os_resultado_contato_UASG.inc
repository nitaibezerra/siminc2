<?php
ini_set("memory_limit", "1024M");
$GLOBALS['idprocesso'] = $_REQUEST['idproc'];
// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

global $db;

$sql   = monta_sql2();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setTotNivel(true);
$r->setBrasao(true);
$r->setEspandir(true);

function monta_agp(){
	//$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											
											"uasg",
											"logradouro",
											"telefone",
											"email"			   		
										  )	  
				);

	array_push($agp['agrupador'], array(
							"campo" => "uasg",
							"label" => "UASG"));
	
	array_push($agp['agrupador'], array(
									"campo" => "logradouro",
									"label" => "Logradouro")										
							   		);	

	
						   		
	return $agp;
}


function monta_coluna(){
		
	$coluna = array();
	
	array_push($coluna, array( "campo" => "telefone",
						       "label" => "Telefone",	
						       "type"  => "string"	
										)										
							   		);	

	array_push($coluna, array( "campo" => "email",
						       "label" => "E-mail",	
						       "type"  => "string"	
										)										
							   		);	

	return $coluna;			  	
}

function monta_sql2(){

		$sql = "select coe.coeid,
					usg.usgcod,
					usg.usgdsc || ' UASG: ' || usg.usgcod as UASG,
					coe.coenddsc || ', ' || coe.coendlog || ', ' || coe.coendnum || ', ' || coe.coendcom || ' ' || coe.coendbai || ', CEP: ' || coe.coendcep || ', ' || mun.mundescricao || '-' || coe.estuf as logradouro,
					ent.entemail as email,
					'('||ent.entnumdddcomercial||') ' || ent.entnumcomercial as telefone
				FROM evento.coenderecoentrega coe
				inner join evento.coadesao coa on coa.coaid = coe.coaid
				inner join workflow.documento doc on coa.docid = doc.docid
				inner join evento.codemandaitem cdi on cdi.coeid = coe.coeid 
				inner join territorios.municipio mun on coe.muncod = mun.muncod
				inner join evento.uasg usg on coa.usgid = usg.usgid
				left join entidade.entidade ent on ent.entunicod = usg.usggestaouasg
				where coa.copid = {$GLOBALS['idprocesso']}
					and doc.esdid = 46
					and cdi.cdiqtde > 0
					group by
					coe.coeid,
					usg.usgcod,
					uasg,
					logradouro,
					email,
					telefone
				order by usgcod";
			
	//ver($sql,d);	 
	//dbg($sql,1);
	return $sql;
}
?>

<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>

	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

		<!--  Monta o Relatório -->
		<? echo $r->getRelatorio(); ?>

	</body>
</html>

<script>
	function chamaproc(prcid)
	{
		//window.opener.location="conjur.php?modulo=principal/editarprocesso&acao=A&prcid="+prcid;
		window.opener.focus();
		
	}
</script>