<?php

switch ($_REQUEST["requisicao"]) {
	case 'pesquisa':
		$filtros = geraFiltroContrapartida( $_REQUEST );
	break;
	case 'cadastra':
		insereContrapartidaMinima( $_REQUEST );
	break;
}


include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Contrapartida Mínima', '');

?>

<style>
	#div_rolagem table {
		width: 100%;                
	}
</style>

<script type="text/javascript" src="js/emenda.js"></script>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" value="pesquisa" name="requisicao"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td colspan="2" class="subtitulocentro">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td width="190px" class="subtitulodireita">Exercício:</td>
			<td>
				<?php echo $_SESSION["exercicio"]; ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">CNPJ:</td>
			<td>
				<?php
					$enbcnpj = $_REQUEST["enbcnpj"]; 
					echo campo_texto( 'enbcnpj', 'N', 'S', '', 22, 18, '##.###.###/####-##', '', 'left', '', 0, 'id="enbcnpj"');  
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nome Entidade:</td>
			<td>
				<?php
					
					$entnome = $_REQUEST["enbnome"];
					echo campo_texto( 'enbnome', 'N', 'S', '', 60, 150, '', '', 'left', '', 0, 'id="enbnome"');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Contrapartida Mínima Definida:</td>
			<td>
				<input type="radio" value="S" id="contrapartida" name="contrapartida" <?=$_REQUEST["contrapartida"] == 'S' ? 'checked=checked' : '' ?> /> Sim
				<input type="radio" value="N" id="contrapartida" name="contrapartida" <?=$_REQUEST["contrapartida"] == 'N' ? 'checked=checked' : '' ?> /> Não
				<input type="radio" value=""  id="contrapartida" name="contrapartida" <?=$_REQUEST["contrapartida"] == ''  ? 'checked=checked' : '' ?> /> Todas
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<input type="button" style="cursor: pointer;" onclick="document.getElementById('formulario').submit();" value="Pesquisar"/>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subtitulocentro">Lista de Entidades Beneficiadas</td>
		</tr>
	</table>
</form>

<center>
	<div id="div_rolagem" style="overflow-x: auto; overflow-y: auto; width:95%; height:300px;">
		<form id="forContra" name="forContra" method="post" action="">
			<input type="hidden" value="cadastra" name="requisicao"/>
			<?php listaEntidadesContrapartida( $filtros ); ?>
			
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr bgcolor="#D0D0D0">
					<td>
						<input type="button" value="Salvar" onclick="document.getElementById('forContra').submit();" style="cursor: pointer;" <?=disabled();?> />
						<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>
		</form>
	</div>
</center>