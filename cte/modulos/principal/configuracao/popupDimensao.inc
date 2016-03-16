<?php
if (!$_GET[itrid])
	die("<script>window.close();</script>");	

$itrid = $_GET[itrid];	

if ($_POST[dimdsc] && $_POST[dimcod]):
	if ($_POST[dimid]):
		$sql = sprintf("UPDATE
							cte.dimensao
						SET
							dimcod = %d,	
							dimdsc = '%s'							
						WHERE
							dimid = %d", $_POST[dimcod], $_POST[dimdsc], $_POST[dimid]);
	else:
		$sql = sprintf("INSERT INTO
							cte.dimensao (
							dimdsc,
							dimcod,
							itrid
						)VALUES(
							'%s',
							%d,
							%d
						)
						",$_POST[dimdsc], $_POST[dimcod],$itrid);
	endif;

	$db->executar($sql);
	$db->commit();
	
	die ("<script>
			alert('Operação executada com sucesso!');
			window.opener.location.replace(window.opener.location);
			window.close();
		  </script>");
endif;

# Carrega dados para edição
if ($_GET[dimid]):
	$sql = sprintf("SELECT
						*
					FROM
						cte.dimensao
					WHERE
						dimid = %d",$_GET[dimid]);

	$dados = $db->carregar($sql);
	
	foreach ($dados[0] as $k => $val)
		${$k} = $val;				
else:
	$sql = sprintf("SELECT
				MAX(dimcod) + 1
			FROM
				cte.dimensao
			WHERE
				dimstatus = 'A' AND
				itrid=%d", $itrid);		
	$dimcod = $db->pegaUm($sql);
endif;
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formDim">
<div id="sp_acoes">
<?php
monta_titulo( 'Manutenção da Dimensão', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Dimensão:</td>
		<td><?= campo_texto( 'dimdsc', 'S', 'S', '', 75, 500, '', '' ); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right"> Ordem:</td>
		<td><?= campo_texto( 'dimcod', 'S', 'S', '', 10, 75, '', '' ); ?></td>
	</tr>		
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<?
			if($aedid){
				echo("<td><input type='button' class='botao' value='Alterar' id='btalterarestado' name='btalterarestado' onclick=\"validar()\" />");
			}else{
				echo("<td><input type='button' class='botao' name='consultar' value='Salvar' onclick=\"validar()\" />");
			}
		?>
			<input type='button' class='botao' value='Voltar' id='btFechar' name='btFechar' onclick='window.opener.location.replace(window.opener.location); window.close();' />
			<input type='hidden' value='<?php echo $itrid;?>' id='itrid' name='itrid'/>
			<input type='hidden' value='<?php echo $_GET[dimid];?>' id='dimid' name='dimid'/>
		</td>			
	</tr>
</table>
</form>
<script type="text/javascript">
function validar() {
	d = document.formDim;
	
	if (d.dimdsc.value == ''){
		d.dimdsc.onfocus();
		alert('O campo Dimensão é obrigatório!');
		return false;
	}
	
	if (d.dimcod.value == '') {
		d.dimcod.onfocus();
		alert('O campo Ordem é obrigatório!');
		return false;		
	}
	d.submit();
}
</script>
</body>