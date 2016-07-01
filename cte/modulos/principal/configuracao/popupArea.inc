<?php
if (!$_GET[dimid])
	die("<script>window.close()</script>");
	
$_POST[dimid] ? $dimid = $_POST[dimid] : $dimid = $_GET[dimid];

if ($_POST):
	if ($_POST[ardid]):
		$sql = sprintf("UPDATE
							cte.areadimensao
						SET
							ardcod = %d,
							arddsc = '%s'
						WHERE
							ardid = %d",$_POST[ardcod], $_POST[arddsc], $_POST[ardid]);
	else:
		$sql = sprintf("INSERT INTO
							cte.areadimensao (
							dimid,
							arddsc,
							ardcod							
						)VALUES(
							%d,
							'%s',
							%d
						)", $_POST[dimid], $_POST[arddsc], $_POST[ardcod]);
	endif;
	
	$db->executar($sql);
	$db->commit();
	
	die ("<script>
			alert('Operação executada com sucesso!');
			window.opener.location.replace(window.opener.location);
			window.close();
		  </script>");
	
endif;


$sql = sprintf("SELECT
					dimdsc
				FROM
					cte.dimensao
				WHERE
					dimid = %d", $dimid);
$dimdsc = $db->pegaUm($sql);

if ($_GET[ardid]):
	$sql = sprintf("SELECT
						ardid,
						arddsc,
						ardcod
					FROM
						cte.areadimensao
					WHERE
						ardid = %d",$_GET[ardid]);
	$dados = $db->carregar($sql);
	
	foreach ($dados[0] as $k => $val)
		${$k} = $val;				
else:
	$sql = "SELECT
				MAX(ardcod)					
			FROM
				cte.areadimensao
			WHERE
				ardstatus = 'A' AND
				dimid = {$dimid}";
	$ardcod = $db->pegaUm($sql)+1;
endif;	
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formArd">
<div id="sp_acoes">
<?php
monta_titulo( 'Manutenção da Área', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Dimensão:</td>
		<td><?= campo_texto( 'dimdsc', 'S', 'N', '', 75, 500, '', '' ); ?></td>
	</tr>		
	<tr>
		<td class="SubTituloDireita" align="right">Área:</td>
		<td><?= campo_texto( 'arddsc', 'S', 'S', '', 75, 500, '', '' ); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right"> Ordem:</td>
		<td><?= campo_texto( 'ardcod', 'S', 'S', '', 10, 75, '', '' ); ?></td>
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
			<input type='hidden' value='<?php echo $dimid;?>' id='dimid' name='dimid'/>
			<input type='hidden' value='<?php echo $ardid;?>' id='ardid' name='ardid'/>
		</td>			
	</tr>
</table>
</form>
<script type="text/javascript">
function validar() {
	d = document.formArd;
	
	if (d.arddsc.value == ''){
		d.arddsc.onfocus();
		alert('O campo Área é obrigatório!');
		return false;
	}
	
	if (d.ardcod.value == '') {
		d.ardcod.onfocus();
		alert('O campo Ordem é obrigatório!');
		return false;		
	}
	d.submit();
}
</script>
</body>