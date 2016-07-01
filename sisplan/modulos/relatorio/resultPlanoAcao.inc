<?php
ini_set("memory_limit", "512M");

if ( $_REQUEST['prtid'] ){
	$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
	$itens = $db->pegaUm( $sql );
	$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	$_REQUEST = $dados;
	unset( $_REQUEST['salvar'] );
}


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = sql_relatio();
$agrupador = agp_relatorio();
$coluna    = coluna_relatorio();

$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados);
$rel->setColuna($coluna);
$rel->setTotNivel(true);
$rel->setBrasao(true);
$rel->setEspandir(true);

if ($_POST['tipo'] == 'exibirXLS'){
    $nomeDoArquivoXls = "Relatório_Plano_Ação_".date("YmdHis");
    echo $rel->getRelatorioXls();
    die;
}
?>
<html>
<head>
	<title>SIG-IPHAN : Sistema Integrado de Gestão do IPHAN</title>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
</head>
<body>
	<!--  Monta o Relatório -->
	<?php echo $rel->getRelatorio(); ?>
</body>
</html>
<?php
function agp_relatorio(){

	$agrupador = $_REQUEST['agrupador'];

	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("atidatainicio",
										  "atidatafim",
										  "atiorcamentocusteio",
										  "atiorcamentocapital",
										  "atiorcamento"),
				);

	foreach ( $agrupador as $val ){
		switch( $val ){
			case "programa":
				array_push($agp['agrupador'], array(
													"campo" => "programa",
											  		"label" => "Programa/PPA")
									   				);
			break;
			case "acao":
				array_push($agp['agrupador'], array(
													"campo" => "acao",
											  		"label" => "Ação/PPA")
									   				);
			break;
			case "departamento":
				array_push($agp['agrupador'], array(
													"campo" => "departamento",
											  		"label" => "Depto")
									   				);
			break;
			case "unidadeexecutora":
				array_push($agp['agrupador'], array(
													"campo" => "unidadeexecutora",
											  		"label" => "Unidade executora")
									   				);
			break;
			case "atidescricao":
				array_push($agp['agrupador'], array(
													"campo" => "atidescricao",
											  		"label" => "Projeto")
									   				);
			break;
			case "situacaocontratacao":
				array_push($agp['agrupador'], array(
													"campo" => "situacaocontratacao",
											  		"label" => "Situação do processo de contratação")
									   				);
			break;
			case "instrumentocontratacao":
				array_push($agp['agrupador'], array(
													"campo" => "instrumentocontratacao",
											  		"label" => "Forma de contratação")
									   				);
			break;
			case "desafio":
				array_push($agp['agrupador'], array(
													"campo" => "desafio",
											  		"label" => "Desafio estratégico")
									   				);
			break;
			case "iniciativa":
				array_push($agp['agrupador'], array(
													"campo" => "iniciativa",
											  		"label" => "Iniciativa estratégica")
									   				);
			break;
			case "estuf":
				array_push($agp['agrupador'], array(
													"campo" => "estuf",
											  		"label" => "UF")
									   				);
			break;
			case "mundescricao":
				array_push($agp['agrupador'], array(
													"campo" => "mundescricao",
											  		"label" => "Município")
									   				);
			break;
			case "esddsc":
				array_push($agp['agrupador'], array(
													"campo" => "esddsc",
											  		"label" => "Estado do Documento")
									   				);
			break;
		}
	}

	return $agp;
}

function coluna_relatorio(){

	$coluna = array();

	foreach ( $_REQUEST['coluna'] as $valor ){

		switch( $valor ){
			case 'dataini':
				array_push( $coluna, array("campo" 	  => "atidatainicio",
							   "label" 	  => "Data de início"));
			break;
			case 'datafim':
				array_push( $coluna, array("campo" 	  => "atidatafim",
							   "label" 	  => "Data de término"));
			break;
			case 'custeio':
				array_push( $coluna, array("campo" 	  => "atiorcamentocusteio",
							   "label" 	  => "Custeio"));
			break;
			case 'capital':
				array_push( $coluna, array("campo" 	  => "atiorcamentocapital",
								   		   "label" 	  => "Capital"));
			break;
			case 'total':
				array_push( $coluna, array("campo" 	  => "atiorcamento",
								   		   "label" 	  => "Total"));
			break;
		}
	}

	return $coluna;
}

function sql_relatio(){

	$where = array();

	extract($_REQUEST);

	$sql = "SELECT * FROM sisplan.v_planoacao_relatorio vpr";

//	ação/PPA
	if( $acacod[0] && $acacod_campo_flag ){
		array_push($where, " acacod " . (!$acacod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "', '", $acacod ) . "') ");
	}
//	programa/PPA
	if( $prgcod[0] && $prgcod_campo_flag ){
		array_push($where, " prgcod " . (!$prgcod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "',' ", $prgcod ) . "') ");
	}
//	Unidade executora
	if( $uexcod[0] && $uexcod_campo_flag ){
		array_push($where, " uexcod " . (!$uexcod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "',' ", $uexcod ) . "') ");
	}
//	Desafio estratégico
	if( $dfecod[0] && $dfecod_campo_flag ){
		array_push($where, " dfeid " . (!$dfecod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "',' ", $dfecod ) . "') ");
	}
//	Estado
	if( $estuf[0] && $estuf_campo_flag ){
		array_push($where, " estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "',' ", $estuf ) . "') ");
	}
//	Município
	if( $muncod[0] && $muncod_campo_flag ){
		array_push($where, " muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "',' ", $muncod ) . "') ");
	}
//	Município
	if( $esdid[0] && $esdid_campo_flag ){
		array_push($where, " esdid " . (!$esdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "',' ", $esdid ) . "') ");
	}

// Adicionando o exercicio atual
	if(!empty($_SESSION['exercicio'])){
		array_push($where, " atianopi = '".$_SESSION['exercicio']."' ");
	}

 	$sql .= count($where) ? " WHERE " . implode(' AND ', $where) : '';
	return $sql;
}
?>
