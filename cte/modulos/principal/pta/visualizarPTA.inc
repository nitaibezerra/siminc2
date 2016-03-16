<?php 

if( !( $inuid = $_SESSION['inuid'] ) ){
	echo "<script> 
			window.close();
		  </script>";
	exit;
}
$sql = "select prsid, to_char( prsdata, 'DD/MM/YYYY à\s HH24:MI:SS' ) as data, prsano, prsnumeroprocesso
		from  cte.projetosape p
		where inuid = $inuid	
		order by prsdata";

$arDados = $db->carregar( $sql );
$prsid   = $arDados[0]["prsid"] ;
/************************* CABEÇALHO E TÍTULO ******************************/
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );

if($prsid != '' ){
$arDadosAgrupados = array();
foreach( $arDados as $arProjeto )
	$arDadosAgrupados[$arProjeto['prsano']][$arProjeto['prsnumeroprocesso']] = $arProjeto;


?>

<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
</head>
	<body>
		<form name="formulario" id="formulario" action="" method="post">
			<input type=hidden name="salvar" value="false" />
			<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%";>
				<?php if( count( $arDadosAgrupados ) ){ ?>
					<?php foreach( $arDadosAgrupados as $ano => $arProjetos ){ ?>
						<tr>
							<th><?php echo $ano ?></th>
						</tr>
						<?php foreach( $arProjetos as $ano => $arProjeto ){ ?>
							<tr>
								<td>
									<a href="cte.php?modulo=principal/pta/gerarPTA&acao=A&prsid=<?php echo $arProjeto['prsid']; ?>" >
										PTA gerado em <?php echo $arProjeto['data']; ?>. (Processo nº <?php echo $arProjeto['prsnumeroprocesso']; ?>)
									</a>
								</td>
							</tr>
			
					<?php } ?>	
					<?php } ?>	
		</table></form>		<?php }
}else{ ?>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
</head>
	<body>
	 		<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%";>
					<tr>
						<th>Não há PTA gerado</th>
					</tr>
<?php } ?>
			</table>
		
	</body>
</html>