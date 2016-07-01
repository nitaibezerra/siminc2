<?php 
				
//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
if($_SESSION['par']['muncod']){
	
	$sql = "select mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
	$rsMunicipio = $db->pegaLinha($sql);
	$linha2 = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
	
}else{
	
	$linha2 = $_SESSION['par']['estuf'];
}
echo "<br/>";
monta_titulo('FINANCIAMENTO DE NOVAS TURMAS DE EJA', $linha2);
echo "<br>";
monta_titulo('Lista de Escolas', '');
?>
<style>
.escola{
cursor:pointer;
}
.escola:hover{
background-color:fffd77;
font-weight:bold;
}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		$('.ir').click(function(){
			window.location = "par.php?modulo=principal/programas/fin_nov_tur_eja/termoadesao&acao=A&cod_inep="+$(this).attr('id');
		});
	});
</script>
<center>
	<table bgcolor="#f5f5f5" align="center" class="tabela" >
		<?php 
		$sql = "SELECT DISTINCT
					no_entidade as escola,
					pk_cod_entidade as cod_inep
				FROM 
					".SCHEMAEDUCACENSO.".tab_entidade ent
				INNER JOIN ".SCHEMAEDUCACENSO.".tab_turma tur ON tur.fk_cod_entidade = ent.pk_cod_entidade
				WHERE
					id_dependencia_adm = 3 
					AND tur.fk_cod_etapa_ensino BETWEEN 3 AND 39
					AND fk_cod_municipio::integer = '".$_SESSION['par']['muncod']."'::integer";
		$escolas = $db->carregar($sql);
		if( is_array( $escolas ) ){
			foreach( $escolas as $escola ){
		?>
		<tr>
			<td width="90%" align="left">
				<div id="<?=$escola['cod_inep'] ?>" class="escola ir">
					<?=$escola['escola'] ?>
				</div>
			</td>
		</tr>
		<?php 
			}
		}else{
		?>
		<tr>
			<td width="90%" align="center" style="color:red;">
				<b>Não possui escolas.</b>
			</td>
		</tr>
		<?php 
		}
		?>
	</table>
	<input type="button" value="Sair" onclick="window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa'" />
</center>
