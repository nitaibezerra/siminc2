<?php

 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Programador: Fernando Bagno (fernandobagno@gmail.com)
 */

switch ($_REQUEST["requisicao"]) {
	
	case "pesquisa":
		$filtro = $ies->filtraListaInstituicao( $_REQUEST );
	break;
	case "cancelarprojeto":
		$filtro = $ies->cancelaProjetoInstituicao( $_REQUEST["iesid"] );
	break;
	
}


if ( $_REQUEST["esdid"] || $_SESSION["ies"]["esdid"] ){
	
	$_SESSION["ies"]["esdid"] = !empty($_REQUEST["esdid"]) ? $_REQUEST["esdid"] : $_SESSION["ies"]["esdid"];  
	$esdid = $_SESSION["ies"]["esdid"];
	
	if( !iesVerificaEstado( $esdid )){
		
		echo '<script>
				alert("O Status informado não existe!");
				history.back(-1);
		  	  </script>';
		
		unset($_SESSION["ies"]["esdid"]);
		die;
		
	}
	
}

if ( !$esdid ){
	
	echo '<script>
				alert("O Status selecionado não existe ou não foi informado. Favor selecioná-lo novamente.");
				history.back(-1);
		  </script>';
	die;
	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

print "<br/>";
monta_titulo( "Lista de Instituições", "" );

?>

<form action="" method="post" name="formFiltro" id="formFiltro">
	<input type="hidden" name="requisicao" id="requisicao" value="pesquisa"/>
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td class="subtitulocentro" colspan="2">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="190px">Status</td>
			<td>
				<b><?php echo iesPegarNomeEstado( $esdid ); ?></b>
				<input type="hidden" name="esdid" id="esdid" value="<?=$esdid?>">
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="190px">Instituição</td>
			<td>
				<?php 
					
					$iesid = $_REQUEST["iesid"];
				
					$sql = "SELECT
								ie.iesid as codigo,
								iescodigo || ' - ' || iesnome as descricao
							FROM
								ies.ies ie
							ORDER BY
								iesnome";
					
					$db->monta_combo( "iesid", $sql, "S", "Todos", "", "", "", "350", "N", "iesid" );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">UF</td>
			<td>
				<?php 
					
					$estuf = $_REQUEST["estuf"];
				
					$sql = "SELECT DISTINCT
								iesuf as codigo,
								iesuf as descricao
							FROM
								ies.ies
							ORDER BY
								descricao";
					
					$db->monta_combo( "estuf", $sql, "S", "Todas", "", "", "", "", "N", "estuf" );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Situação</td>
			<td>
				<input type="radio" id="pbistatus" name="pbistatus" value="A" <?php if($_REQUEST["pbistatus"] == 'A' ){ echo 'checked';} ?>/> Ativo
				<input type="radio" id="pbistatus" name="pbistatus" value="I" <?php if($_REQUEST["pbistatus"] == 'I' ){ echo 'checked';} ?>/> Inativo
				<input type="radio" id="pbistatus" name="pbistatus" value=""  <?php if($_REQUEST["pbistatus"] == '' ){ echo 'checked';} ?>/> Todos
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<input type="button" value="Pesquisar" onclick="iesFiltrarLista();" style="cursor: pointer;">
			</td>
		</tr>
	</table>
</form>

<?php $ies->montaListaInstituicoes( $esdid, $filtro ); ?>

<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr bgcolor="#D0D0D0">
		<td>
			<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;">
		</td>
	</tr>
</table>