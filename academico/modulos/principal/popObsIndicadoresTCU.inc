<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

monta_titulo("Observação", "");

$perfilNotBloq = array( //PERFIL_ADMINISTRADOR,
						PERFIL_IFESCADTCU);

// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança

$bloqueado = $permissoes['gravar']==1 ? "" :  "disabled";

?>

<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form action="">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
			<tr>
				<td class="subtitulodireita">Observação:</td>
				<td>
				<?=campo_textarea( 'lciobs', 'N', 'S', '', 80, 15, 2000, '', 0, '' );?>
				</td>
			</tr>
			<tr bgcolor="#C0C0C0">
				<td colspan="2">
					<?php 
					/*$usucpf = $_SESSION['usucpf'];
					$pflcod = academico_array_perfil();
					foreach ($pflcod as $perfil){
						if($perfil == PERFIL_IFESCONTCU)
							$habilita = "disabled";
						else
							$habilita = "";	
					}*/
					?>
					
					<input type="button" id="bt_salvar_obs" <?= $bloqueado; ?> value="Salvar" onclick="salvarObservacao();" style="cursor: pointer;" />
					<input type="button" id="bt_fechar_obs" value="Fechar" onclick="self.close();" style="cursor: pointer;" />
				</td>
			</tr>
		</table>
		</form>
	</body>
</html>

<script type="text/javascript">

var tcuid 			=	<?=$_REQUEST["tcuid"]?>;
var lciobs 			=	document.getElementsByName("lciobs")[0];
var obsindicador	=	window.opener.document.getElementById("obs_indicador_" + tcuid);

lciobs.value = obsindicador.value;

function salvarObservacao() {
	var btSalvar = document.getElementById('bt_salvar_obs');
	var btFechar = document.getElementById('bt_fechar_obs');

	btSalvar.disabled = true;
	btFechar.disabled = true;
	
	if (lciobs.value.length < 2000){		
		obsindicador.value = lciobs.value;
		alert('Dados gravados com sucesso.');
		self.close();
	}else{
		alert('Quantidade de caracteres excede o limite!');
	}
}

</script>