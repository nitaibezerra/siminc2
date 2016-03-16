<?php

$boDimensao 	= 'N';
$boArea 		= 'N';
$boIndicador 	= 'N';
$boCriterio 	= 'N';
$boAcao		 	= 'S';


if($_POST['tipo'] == 'acao'){
	//ver($_POST,d);
	$ppaid = $_POST['ppaid'] ? $_POST['ppaid'] : null;
	
	$oAcao 			= new Acao();
	
	//$oAcao->ppaid 				= $ppaid;
	//$oAcao->ppadsc				= $_POST['ppadsc'];
	$oAcao->ptoid					= $_POST['ptoid']; 
	$oAcao->acidsc					= $_POST['acidsc'];
    $oAcao->acinomeresponsavel		= $_POST['acinomeresponsavel'];
    $oAcao->acicargoresponsavel		= $_POST['acicargoresponsavel'];
    $oAcao->aciresultadoesperado	= $_POST['aciresultadoesperado'];
    $oAcao->acistatus				= 'A';
    $oAcao->acidata					= 'NOW()';
    $oAcao->usucpf					= $_SESSION['usucpf'];
	$oAcao->salvar();
    //ver($oAcao, d);
	$oAcao->commit();
	
	echo "<script>window.close();window.opener.location.reload();</script>";
	exit();
}

if( empty($_REQUEST['ptoid']) ){
	echo "<script>
				alert('Faltam dados!');
				history.back(-1);
	 		</script>";
	exit();
}

$sql = "select
			c.crtdsc as criterio,
			i.inddsc as indicador,
			a.aredsc as area,
			d.dimdsc as dimensao
		FROM
			par.pontuacao p
		INNER JOIN par.criterio c ON p.crtid = c.crtid AND c.crtstatus = 'A'
		INNER JOIN par.indicador i ON i.indid = c.indid 
		INNER JOIN par.area a ON a.areid = i.areid
		INNER JOIN par.dimensao d ON d.dimid = a.dimid
		WHERE
			p.ptostatus = 'A' AND p.ptoid = ".$_REQUEST['ptoid'];

$dados = $db->pegaLinha( $sql );
?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
		<script src="../includes/funcoes.js" type="text/javascript"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script src="../includes/jquery-validate/jquery.validate.js" type="text/javascript"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
		<script type="text/javascript">
		<!--
				
			$(document).ready(function(){
				/*$(window).bind("beforeunload", function(){
					window.opener.location.reload(); 
				});*/
				
				//validação
				$(".formulario").validate({
					//ignoreTitle: true,
					rules: {
						dimdsc: "required",
						aredsc: "required",
						inddsc: "required",
						ordcod: "required",
						crtpontuacao: "required",
						crtdsc: "required",
						crtpeso: "required",
						ppadsc: "required"					   
					}				
														
				});

				$('#btFechar').click(function(){
					window.opener.location.reload();
					window.close();
				});
				
			});
			
		//-->
		</script>
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" unload="alert(44)">
		<form action="" method="post" name="formulario" id="formulario" class="formulario">
			<?php
			print '<br />';
			monta_titulo( 'Cadastro de Ação '.$stTitulo, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
				align="center">
				<tr>
					<td class="SubTituloDireita" align="right">Dimensão:</td>
					<td>
						<? // campo_texto( 'dimdsc', 'S', $boDimensao, '', 65, 500, '', '', '', '', '', 'id="dimdsc" title="O campo dimensão é obrigatório."', '', $dados['dimensao'] ); ?>
						<?= campo_textarea( 'dimdsc', 'S', $boDimensao, '', 65, 2, 500, '', '', '', '', '', $dados['dimensao']); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" align="right">Área:</td>
					<td>
						<? //= campo_texto( 'aredsc', 'S', $boArea, '', 65, 500, '', '','','','','title="O campo área é obrigatório."', '', $dados['area'] ); ?>
						<?= campo_textarea( 'aredsc', 'S', $boArea, '', 65, 2, 500, '', '', '', '', '', $dados['area']); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" align="right">Indicador:</td>
					<td>
						<? //= campo_texto( 'inddsc', 'S', $boIndicador, '', 65, 500, '', '','','','','title="O campo indicador é obrigatório."', '', $dados['indicador'] ); ?>
						<?= campo_textarea( 'inddsc', 'S', $boIndicador, '', 65, 3, 500, '', '', '', '', '', $dados['indicador']); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" align="right">Critério:</td>
					<td>
						<? //= campo_texto( 'crtdsc', 'S', $boCriterio, '', 65, 2000, '', '','','','','title="O critério área é obrigatório."', '', $dados['criterio'] ); ?>
						<?= campo_textarea( 'crtdsc', 'S', $boCriterio, '', 65, 6, 2000, '', '', '', '', '', $dados['criterio']); ?>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Descrição da Ação:</td>
				    	<td><?php $acidsc = $oAcao->acidsc; echo campo_textarea( 'acidsc', 'S', $boAcao, '', 100, 5, 500, '', '', '', '', 'O campo Descrição da Ação é obrigatório.' ); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" align="right">Nome do Responsável:</td>
					<td><?php $acinomeresponsavel = $oAcao->acinomeresponsavel; echo campo_texto( 'acinomeresponsavel', 'S', "S", '', 95, 95, '', '','','','','title="O campo Nome do Responsável é obrigatório."' );?></td>
			    </tr>
				<tr>
					<td class="SubTituloDireita" align="right">Cargo do Responsável:</td>
					<td><?php $acicargoresponsavel= $oAcao->acicargoresponsavel; echo campo_texto( 'acicargoresponsavel', 'S', "S", '', 95, 95, '', '','','','','title="O campo Cargo do Responsável é obrigatório."' );?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" align="right">Resultado Esperado:</td>
					<td><?php $aciresultadoesperado = $oAcao->aciresultadoesperado; echo campo_textarea( 'aciresultadoesperado', 'S', "S", '', 100, 5, 500, '', '', '', '', 'O campo Resultado Esperado é obrigatório.'  ); ?></td>
				</tr>
				<tr bgcolor="#C0C0C0">
					<td>&nbsp;</td>
					<td>						
						<input type='submit' name='incluir' value='Salvar' />
						<input type='button' value='Fechar' id='btFechar' name='btFechar' onclick='fecharJanela()' />
						<input type='hidden' value='<?php echo $dados['dimid'];?>' id='dimid' name='dimid'/>
						<input type='hidden' value='<?php echo $dados['areid'];?>' id='areid' name='areid'/>
						<input type='hidden' value='<?php echo $dados['indid'];?>' id='indid' name='indid'/>
						<input type='hidden' value='<?php echo $dados['crtid'];?>' id='crtid' name='crtid'/>
						<input type='hidden' value='<?php echo $_REQUEST['ptoid'];?>' id='ptoid' name='ptoid'/>
						<input type='hidden' value='acao' id='tipo' name='tipo'/>
					</td>			
				</tr>
			</table>
		</form>	
	</body>
</html>