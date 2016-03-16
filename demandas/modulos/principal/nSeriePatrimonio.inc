<?php

if(!$_SESSION['dmdid']){
	unset($_SESSION);
	print "<script>
				alert('Sua sessão expirou. Acesse novamente o link para acessar a demanda!');
				window.close(); 
		   </script>";
	exit;
}

$dmdid = $_SESSION['dmdid'];


if($_POST){
	
	
	$sql = " DELETE FROM demandas.itemhardwaredemanda WHERE dmdid = $dmdid";
	$db->executar($sql);
	
	$dmdqtde = $_POST['dmdqtde'];
	
	for($i=1; $i<=$dmdqtde; $i++){

		$sql = " INSERT INTO demandas.itemhardwaredemanda
				 (
				 	dmdid, 
				 	ihdstatus,
				 	ihdnumserie,
				 	ihdnumpatrimonio
				 ) VALUES (
				 	".$dmdid.", 
				 	'A',
				 	'".$_POST['nserie'.$i]."',
				 	'".$_POST['npatrimonio'.$i]."'  
				 );";
		$db->executar($sql);
	}
	
	$db->commit();
		
	unset($_POST);
	
	die("<script>
			window.opener.location.reload();
			alert('Operação realizada com sucesso!');
			window.close();
		 </script>");
	
}
 	
	

	

//include APPRAIZ ."includes/cabecalho.inc";
global $db;
$db->cria_aba( $abacod_tela, $url, '' );


//pega dmdqtde para qtd de nº de series e patrimonio
$sql = "select d.dmdqtde, t.tipnumserie, t.tipnumpatr
		from demandas.demanda d
		LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		where dmdid = $dmdid
		";
$dados = $db->carregar($sql);
if($dados) extract($dados[0]);	

if(!$dmdqtde) $dmdqtde = 1;


//carrega para edição
$sql = "select ihdnumserie, ihdnumpatrimonio
		from demandas.itemhardwaredemanda
		where dmdid = $dmdid
		";
$da = $db->carregar($sql);

if($da){
	$y=1;
	$edita = array();
	foreach($da as $d){
		$edita['nserie'.$y] = $d['ihdnumserie'];
		$edita['npatrimonio'.$y] = $d['ihdnumpatrimonio'];
		$y++;
	}
	extract($edita);	
}



$habil = 'N';

//verifica se o usuário pode editar a demanda
$verificaEditaDemanda = verificaEditaDemanda();
if($verificaEditaDemanda == true){
	$habil = 'S';
}
else{
	$habil = 'N';
}

// Pega o "estado do documento", vinculado à demanda
$esdid = recuperaEstadoDocumento();
//verifica se o estado do documento é finalizada ou cancelada (se sim, bloqueia todos os campos).
if( in_array($esdid, array(DEMANDA_ESTADO_CANCELADO,DEMANDA_GENERICO_ESTADO_CANCELADO,DEMANDA_ESTADO_FINALIZADO,DEMANDA_GENERICO_ESTADO_FINALIZADO,DEMANDA_ESTADO_INVALIDADA,DEMANDA_GENERICO_ESTADO_INVALIDADA,DEMANDA_ESTADO_VALIDADA_FORA_PRAZO)) ){
	$habil = 'N';
}

?>
<html>
<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">


<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>

<form name="formulario" action="" method="post">

<input type="hidden" name="dmdqtde" value="<?=$dmdqtde?>">

<table width="99%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="background-color: #d0d0d0; text-align: center;" colspan="2">
			<b>CADASTRO DE Nº DE SERIE / PATRIMONIO</b>
		</td>
	</tr>
	<?if($tipnumserie == 't' || $tipnumpatr == 't'){
		
		for($i=1; $i<=$dmdqtde; $i++){
		?>
		<tr>
			<td align='right' class="SubTituloDireita">Item <?=$i?>:</td>
			<td>
				<?if($tipnumserie == 't'){?>
					Nº Serie: <?= campo_texto( 'nserie'.$i, 'S', $habil, '', 30, 30, '', ''); ?>
					&nbsp;&nbsp;&nbsp;
				<?}?>
				<?if($tipnumpatr == 't'){?>
					Nº Patrimonio: <?= campo_texto( 'npatrimonio'.$i, 'S', $habil, '', 30, 30, '', ''); ?>
				<?}?>
			</td>
		</tr>
		<?}?>
	<tr bgcolor="#C0C0C0">
		<td colspan="2" align="center">
			<input type="button" class="botao" name="btalterar" value="Salvar" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> onclick="validaForm();">
		</td>
	</tr>	
	<?}else{?>
	<tr>
		<td colspan="2" align="center">Não existe itens para serem cadastrados.</td>
	</tr>	
	<tr bgcolor="#C0C0C0">
		<td colspan="2" align="center">
			<input type="button" class="botao" name="btFECHAR" value="Fechar" onclick="self.close();">
		</td>
	</tr>	

	<?}?>
</table>
</form>
</body>
</html>

<script type="text/javascript">
d = document.formulario;


/*
* Function destinada a validação do formulário
*/
function validaForm(){


	<? 
	if($tipnumserie == 't' || $tipnumpatr == 't'){
		for($i=1; $i<=$dmdqtde; $i++){
			if($tipnumserie == 't'){
				?>
					if (d.nserie<?=$i?>.value == ''){
						alert('Item <?=$i?>: O campo Nº Serie  é obrigatório!');
						d.nserie<?=$i?>.focus();
						return false;
					}
				<? 
			}
			if($tipnumpatr == 't'){
				?>
					if (d.npatrimonio<?=$i?>.value == ''){
						alert('O campo Item <?=$i?>: Nº Patrimonio  é obrigatório!');
						d.npatrimonio<?=$i?>.focus();
						return false;
					}
				<? 
			}
			
		}
	}
	?>

	
	
	d.submit();
}
</script>
