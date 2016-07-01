<?php
ini_set("memory_limit", "512M");

if ( $_REQUEST['prtid'] ){
	$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
	$itens = $db->pegaUm( $sql );
	$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
	unset( $_REQUEST['salvar'] );
}


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();
// monta o sql, agrupador e coluna do relatório
$sql       = sql_relatorio();
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
				"agrupadoColuna" => array("dpgvalor",
										  "dpevalor",
	                                      "dpenudocumento",
	                                      "dpeobservacoes",
										  "saldo")
				);


	foreach ( $agrupador as $val ){
		switch( $val ){
			case 'exercicio':
                array_push($agp['agrupador'], array("campo"    => "exercicio",
                                                    "label"    => "Ano"));
            break;
			case "rfdnome":
				array_push($agp['agrupador'], array(
													"campo" => "rfdnome",
											  		"label" => "Referência")
									   				);
			break;
			case "descricao":
				array_push($agp['agrupador'], array(
													"campo" => "descricao",
											  		"label" => "Unidade")
									   				);
			break;
			case "ptres":
				array_push($agp['agrupador'], array(
													"campo" => "ptres",
											  		"label" => "PTRES")
									   				);
			break;
			case "ndpdsc":
				array_push($agp['agrupador'], array(
													"campo" => "ndpdsc",
											  		"label" => "Natureza Despesa")
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
			case 'dpgvalor':
				array_push( $coluna, array("campo" 	  => "dpgvalor",
							               "label" 	  => "Valor Programado"));
			break;
			case 'dpevalor':
				array_push( $coluna, array("campo" 	  => "dpevalor",
							               "label" 	  => "Valor Executado"));
			break;
			case 'dpenudocumento':
				array_push( $coluna, array("campo" 	  => "dpenudocumento",
								   		   "label" 	  => "Número do Documento",
										   "type"     => "string"));
			break;
			case 'dpeobservacoes':
				array_push( $coluna, array("campo" 	  => "dpeobservacoes",
								   		   "label" 	  => "Observações",
										   "type"     => "string"));
			break;
			case 'saldo':
				array_push( $coluna, array("campo" 	  => "saldo",
								   		   "label" 	  => "Saldo"));
			break;
		}
	}

	return $coluna;
}

function sql_relatorio(){

    $where = array();

    extract($_REQUEST);

    // Referencias
    if( $rfdid[0]  && $rfdid_campo_flag ){
        array_push($where, " rfd.rfdid " . (!$rfdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $rfdid ) . "') ");
    }

    // Unidades
    if( $uexid[0]  && $uexid_campo_flag ){
        array_push($where, " uex.codigo " . (!$uexid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $uexid ) . "') ");
    }

    // PTRES
    if( $ptrid[0]  && $ptrid_campo_flag ){
        array_push($where, " ptr.ptrid " . (!$ptrid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ptrid ) . "') ");
    }

    // NATUREZA DESPESA
    if( $ndpid[0]  && $ndpid_campo_flag ){
        array_push($where, " ndp.ndpid " . (!$ndpid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ndpid ) . "') ");
    }

    $sql = "   SELECT
		   		   ptd.*,
                   ptr.*,
                   rfd.*,
                   uex.*,
                   ptn.*,
                   ndp.*,
                   ptr.ptres || '-' || ptr.prgcod || '.' || ptr.acacod || '.' || ptr.unicod || '.' || ptr.loccod as ptres,
                   ndp.ndpcod || ' - ' || ndp.ndpdsc as ndpdsc,
                   COALESCE ( ptr.exercicio , '{$_SESSION['exercicio']}') as exercicio,
                   COALESCE(univalores.dpgvalor,0) AS dpgvalor,
                   COALESCE(univalores.dpevalor,0) AS dpevalor,
                   ( COALESCE(univalores.dpgvalor,0) - COALESCE(univalores.dpevalor, 0)) as saldo,
                   univalores.dpenudocumento,
                   univalores.dpeobservacoes
               FROM
               		planointerno.unidadeexecutora_uo uex
               LEFT JOIN
                    sisplan.despesaprogramacao dpg ON dpg.uexid = uex.codigo AND
                    								  dpg.dpgstatus = 'A'
               LEFT JOIN
                    sisplan.despesaexecucao dpe ON dpe.ptrid = dpg.ptrid AND
                    							   dpe.uexid = dpg.uexid AND
                    							   dpe.ptnid = dpg.ptnid AND
                    							   dpe.rfdid = dpg.rfdid AND
                    							   dpe.dpestatus = 'A'
               LEFT JOIN
                    sisplan.ptresdespesa ptd ON ( CASE
													WHEN ptd.ptrid = dpg.ptrid OR ptd.ptrid = dpe.ptrid THEN
														ptd.ptrid = dpg.ptrid OR ptd.ptrid = dpe.ptrid
													ELSE
														true
													END
												) AND ptd.ptdstatus = 'A'
               LEFT JOIN
                    sisplan.ptres ptr ON ptr.ptrid = ptd.ptrid AND
                    					 ptr.ptrstatus = 'A' AND
                    					 ptr.exercicio = '{$_SESSION['exercicio']}'
               LEFT JOIN
                    sisplan.referenciadespesa rfd ON ( CASE
															WHEN rfd.rfdid = dpe.rfdid OR rfd.rfdid = dpg.rfdid THEN
																rfd.rfdid = dpe.rfdid OR rfd.rfdid = dpg.rfdid
															ELSE
																true
															END
													 ) AND rfd.rfdstatus = 'A' AND rfd.rfdano = '{$_SESSION['exercicio']}'
               LEFT JOIN
                    sisplan.ptresnatureza ptn ON ( CASE
													WHEN ptn.ptnid = dpe.ptnid OR ptn.ptnid = dpg.ptnid THEN
														ptn.ptnid = dpe.ptnid OR ptn.ptnid = dpg.ptnid
													ELSE
														true
													END
												 ) AND ptn.ptnstatus = 'A' AND ptn.ptrid = ptr.ptrid

               LEFT JOIN
                    naturezadespesa ndp ON ndp.ndpid = ptn.ndpid AND
                    					   ndp.ndpsubelementostatus = 'A'
               LEFT JOIN
               		(
						SELECT
						   uex2.uexid,
						   dpg2.dpgid,
						   dpe2.dpeid,
						   COALESCE ( dpg2.ptrid, dpe2.ptrid ) as ptrid,
						   COALESCE ( dpg2.ptnid, dpe2.ptnid ) as ptnid,
						   COALESCE ( dpg2.rfdid, dpe2.rfdid ) as rfdid,
						   dpe2.dpenudocumento,
						   dpe2.dpeobservacoes,
						   dpg2.dpgvalor,
						   dpe2.dpevalor
						FROM
							planointerno.unidadeexecutora_uo uex2
						LEFT JOIN
						    sisplan.despesaprogramacao dpg2 ON dpg2.uexid = uex2.codigo AND
										      dpg2.dpgstatus = 'A'
						LEFT JOIN
						    sisplan.despesaexecucao dpe2 ON dpe2.ptrid = dpg2.ptrid AND
										   dpe2.uexid = dpg2.uexid AND
										   dpe2.ptnid = dpg2.ptnid AND
										   dpe2.rfdid = dpg2.rfdid AND
										   dpe2.dpestatus = 'A'
						WHERE
						    uex2.uexstatus = 'A' AND
						    ( NOT dpe2.dpeid IS NULL OR NOT dpg2.dpgid IS NULL )
               		) univalores ON 	( (univalores.dpeid = dpe.dpeid)OR
	               						  (univalores.dpgid = dpg.dpgid)) AND
	               						univalores.uexid = uex.codigo AND
	               						univalores.ptrid = ptr.ptrid AND
	               						univalores.ptnid = ptn.ptnid AND
	               						univalores.rfdid = rfd.rfdid
               WHERE
		    		uex.uexstatus = 'A' AND  ptr.ptres != '' ".
                    (count($where) ? " AND ". implode(' AND ', $where) : "" )
               ." ORDER BY
					ptr.exercicio, ptr.ptres, uex.descricao,ndp.ndpcod,rfd.rfdordem, univalores.dpenudocumento DESC, univalores.dpeobservacoes DESC";

               // foi adicionado o order by para que o registro com o documento e a observação apareça
               // no topo e possa aparecer.
	return $sql;
}
?>