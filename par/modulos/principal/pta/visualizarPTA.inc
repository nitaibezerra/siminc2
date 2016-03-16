<?php 
$inuid = $_SESSION['par']['inuid'];

if( !$inuid ){
	echo "<script> 
			alert('Falta o inuid');
			window.close();
		  </script>";
	exit;
}

$sql = "SELECT 
			prpid, 
			to_char( prpdatainclusao, 'DD/MM/YYYY' ) as data, 
			prpanoconveniofnde, 
			prpnumeroprocesso 
		FROM 
			par.processopar prp
		INNER JOIN par.empenho 		  emp ON emp.empnumeroprocesso = prpnumeroprocesso 
		INNER JOIN par.empenhosubacao ems ON ems.empid = emp.empid and eobstatus = 'A' and empstatus = 'A'
		WHERE 
			prp.prptipoexecucao = 'C' 
			AND prp.inuid = ".$inuid;

$arDados = $db->carregar( $sql );
$prpid   = $arDados[0]["prpid"] ;

/************************* CABEÇALHO E TÍTULO ******************************/
$db->cria_aba( $abacod_tela, $url, '' );
//cte_montaTitulo( $titulo_modulo, '&nbsp;' );

if($prpid != '' ){
$arDadosAgrupados = array();
foreach( $arDados as $arProjeto )
	$arDadosAgrupados[$arProjeto['prpanoconveniofnde']][$arProjeto['prpnumeroprocesso']] = $arProjeto;


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
									<a href="par.php?modulo=principal/pta/gerarPTA&acao=A&prpid=<?php echo $arProjeto['prpid']; ?>" >
										PTA gerado em <?php echo $arProjeto['data']; ?>. (Processo nº <?php echo $arProjeto['prpnumeroprocesso']; ?>)
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