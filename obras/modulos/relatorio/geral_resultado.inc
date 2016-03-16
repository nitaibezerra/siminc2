<?php
if($_REQUEST['status'] != ""){
	$_REQUEST['colunas'][] = "obrdtexclusao";
	$_REQUEST['colunas'][] = "usucpfexclusao";
	$_REQUEST['colunas'][] = "obrobsexclusao";
}

set_time_limit(0);
ini_set("memory_limit", "1024M");

// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
	$existe_rel = 0;
	$sql = sprintf(
		"select prtid from public.parametros_tela where prtdsc = '%s'",
		$_REQUEST['titulo']
	);
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0) 
	{
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			$_SESSION['usucpf'],
			$_SESSION['mnuid'],
			$existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else 
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	?>
	<script type="text/javascript">
		alert('Operação realizada com sucesso!');
		location.href = '?modulo=<?= $modulo ?>&acao=A';
	</script>
	<?
	die;
}

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

if(!$_REQUEST['orgid']) $_REQUEST['orgid'][] = '3'; 

// monta o sql, agrupador e coluna do relatório
$sql       = obras_monta_sql_relatio();
$agrupador = obras_monta_agp_relatorio();
$coluna    = obras_monta_coluna_relatorio();
$dados 	   = $db->carregar($sql);

// Filtro de 'Valor do Contrato divergente do Cronograma'
if( $_REQUEST['valordivergente'] != 'todos' )
{
	$arr = array();
	for($i=0; $i<count($dados); $i++)
	{
		$arrParam  = array("traid" => $dados[$i]['traid'], "traseq" => $dados[$i]['traseq']);
		//Recupera o Valor Total das etapas do Cronograma Físico-Financeiro.
		$valorTotalCronograma = recuperaValorTotalCronograma($dados[$i]['obrid'], $arrParam);
		
		if( !is_null($valorTotalCronograma) && $valorTotalCronograma != '' )
		{
			//Recupera o Valor Total do Contrato da Obra com ou sem Aditivo.
			$valorTotalContrato   = recuperaValorTotalContrato($dados[$i]['obrid'], $arrParam);
			
			if( $_REQUEST['valordivergente'] == 'sim' && $valorTotalCronograma != $valorTotalContrato )
			{
				$arr[] = $dados[$i];
			}
			if( $_REQUEST['valordivergente'] == 'nao' && $valorTotalCronograma == $valorTotalContrato )
			{
				$arr[] = $dados[$i];
			}
		}
	}
	
	$dados = $arr;
}
	
$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTotNivel(true);
$rel->setTotalizador(false);

//dbg($rel->getAgrupar(), 1);

// Gera o XLS do relatório
if ( $_REQUEST['tipoRelatorio'] == 'xls' ){
	ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die;
}

?>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
		
	</body>
</html>