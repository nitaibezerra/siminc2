<?php

$proid =$_SESSION["emenda"]["proid"];

$sql= "SELECT 
			pronome
		FROM 		
			emenda.programa
		WHERE
			proid = $proid";
$pronome = $db->pegaUm($sql);			

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( "Emenda do Programa", "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );

if($_POST){
	$sql = "UPDATE emenda.programaemenda set
					prestatus='I'
			WHERE proid = $_REQUEST[proid]";
	$dados = $db->carregar($sql);

	foreach ($_REQUEST['emenda'] as $item ){
		if($item<>''){
			$sql = "SELECT prestatus FROM emenda.programaemenda
					WHERE proid = $_REQUEST[proid] and emeid=$item";
			$registro = $db->pegaUm($sql);
			if($registro=='I'){
				$sql = "UPDATE emenda.programaemenda set
						prestatus='A'
						WHERE proid = $_REQUEST[proid] and emeid = $item";
				$db->executar($sql);
			}
			else if($registro['prestatus']==''){
				$sql = "INSERT INTO emenda.programaemenda
				(proid,emeid,prestatus)
				VALUES
				($_REQUEST[proid],$item,'A')";
				$db->executar($sql);
			}
		}

	}
	
	
	$db->commit();
	echo "<script>alert('Informações salvas com sucesso!');
				  location.href='emenda.php?modulo=principal/cadastroEmendaPrograma&acao=C&proid='+$proid;
		  </script>";
}


?>
<html>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

	
	
</script>
<body>


<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >
<input type="hidden" name="proid" id="proid" value="<?=$proid;?>">
<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome do Programa:</td>
		<td><?=$pronome;?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Emendas do Programa:</td>
		<td>
      	<?php
			$sql = "SELECT
						pre.emeid AS codigo,
						e.emecodigo AS descricao
					FROM 
						emenda.emenda e, emenda.programaemenda pre
					WHERE
						e.emeid = pre.emeid and
						pre.proid = $proid and
						pre.prestatus='A'
					ORDER BY
						e.emecodigo";
			$emenda = $db->carregar($sql);			
			$sql = " SELECT
						e.emeid AS codigo,
						e.emecodigo AS descricao
					FROM 
						emenda.emenda e
					ORDER BY
						e.emecodigo";
			combo_popup('emenda', $sql, 'Selecione a(s) Emenda(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400);
 
		?>
		</td>
	</tr>
	<tr>
		<th align="center" colspan="2">
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="submete();">
			<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="voltar(<?=$proid;?>);">
		</th>
	</tr>
</table>
</form>
<script type="text/javascript">
	function submete(){
		var form = document.getElementById("formulario");
		selectAllOptions(form.emenda);
		document.formulario.submit();
	}
	function voltar(proid){
		window.location.href = 'emenda.php?modulo=principal/cadastroPrograma&acao=C&proid='+proid;
	}
</script>
</body>
</html>