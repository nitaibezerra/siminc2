<?php

	if ($_REQUEST['filtrosession']){
		$filtroSession = $_REQUEST['filtrosession'];
	}
	
	if ($_POST['tipo_relatorio']){
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
		$nomeDoArquivoXls="relatorio_pnld_".date('d-m-Y_H_i');
		echo $r->getRelatorioXls();
	}elseif($_POST['tipo_relatorio'] == 'html'){
		echo $r->getRelatorio();	
	}
?>

</body>
</html>

<?php 
function monta_sql(){
	global $filtroSession;
	
	extract($_POST);

	if( $editora[0] && ( $editora_campo_flag || $editora_campo_flag == '1' )){		
		$where[1] = "Where ed.edtid " . (( $editora_campo_excludente == null || $editora_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $editora ) . "') ";		
	}
	
	$sql = "
		Select 	distinct rr.rreid,
				rr.rredescricao as recursodescricao,
				to_char( arq.arqdata, 'DD/MM/YYYY' ) as dataparecer,
				to_char( arq2.arqdata, 'DD/MM/YYYY' ) as datarecurso,
				to_char( arq3.arqdata, 'DD/MM/YYYY' ) as dataresposta,
				substr(ed.edtcnpj, 1, 2)||'.'||substr(ed.edtcnpj, 3, 3)||'.'||substr(ed.edtcnpj, 6, 3)||'/'||substr(ed.edtcnpj, 9, 4)||'-'||substr(ed.edtcnpj, 13, 2) as edtcnpj,
				--ed.edtcnpj,
				ed.edtnome
		From livro.resenharecurso as rr
		Join public.arquivo as arq on arq.arqid = rr.arqid
		Left Join public.arquivo as arq2 on arq2.arqid = rr.arqidrecurso
		Left Join public.arquivo as arq3 on arq3.arqid = rr.arqidresposta
		Left Join livro.livrodidatico as lv on lv.rreid = rr.rreid and lv.prsano = '".$_SESSION['exercicio']."'
		Join livro.editora as ed on ed.edtid = lv.edtid
		
		".$where[1]."
		
		Order by rr.rreid
	";
	return $sql;
}

function monta_coluna(){
	
	$colunas = array();
	
	$coluna = array(
			
				array(	"campo" 	=> "recursodescricao",
			   		   	"label"		=> "Descrição do Recurso",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "dataparecer",
			   		   	"label"		=> "Data do Parecer",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "datarecurso",
			   		   	"label"		=> "Data do Recurso",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "dataresposta",
			   		   	"label"		=> "Data da Resposta",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "edtcnpj",
			   		   	"label"		=> "CNPJ",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "edtnome",
			   		   	"label"		=> "Nome Editora",
			   		   	"type"	  	=> "string"
				) 

		);
	return $coluna;
}

function monta_agp(){

	$agp = 	array(	"agrupador" => array(), 
					"agrupadoColuna" => array("rreid","recursodescricao","dataparecer","datarecurso","dataresposta","edtcnpj","edtnome")	  
			);
		
	array_push($agp['agrupador'], array("campo" => "rreid", "label" => "$var Id") );
		
	return $agp;
}
?>