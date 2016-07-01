<?php

if($_POST['hdn_atualizar']):
	ini_set("memory_limit","250M");
	set_time_limit(0);
	$msg = atualizarDocidEscolasEMI();
endif;

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo("Atualização de WorkFlow", "Atualiza os identificadores (docid) das escolas vinculadas às Secretarias (emi.ementidade) do Exercício 2009.");

?>

<table class="tabela" height="100%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
	<?php if($msg): ?>
	<tr>
		<td bgcolor="#DCDCDC" align="center"><?php echo $msg ?></td>
	</tr>
	<?php endif; ?>
	<tr>
		<td align="center">
		<?php if($db->testa_superuser()): ?>
			<script>
				function carregandoWorkFlow(){
					document.getElementById('btn_atualizar').disabled = "disabled"
					document.getElementById('btn_atualizar').value="Carregando...";
					formulario.submit();
				}
				
			</script>
			<form name="formulario" method="post" action="">
			<input type="hidden" name="hdn_atualizar" value="1" />
			<input type="button" id="btn_atualizar" onclick="carregandoWorkFlow()" value="Atualizar" name="btn_atualizar"  >
			</form>
		<?php else: ?>
			Seu perfil não possui permissão para realizar esta operação!
		<?php endif; ?>
		</td>
	</tr>
</table>