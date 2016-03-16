<?php

if( $_REQUEST["requisicao"] == "pesquisar" ){
	
	$filtros = array();
	
	!empty($_REQUEST["prodsc"]) 	 ? array_push( $filtros, " pro.prodsc ilike '%{$_REQUEST["prodsc"]}%' " )     : "";
	!empty($_REQUEST["unpid"])   	 ? array_push( $filtros, " pro.unpid = '{$_REQUEST["unpid"]}' " ) 			  : "";
	
	$dadosWhere = ( count($filtros) > 0 ) ? implode( " AND", $filtros ) : "";
	$whereProcedencia = !empty($dadosWhere) ? "WHERE {$dadosWhere}" : "";
	
}

if($_POST && ! $_REQUEST["requisicao"])

if($_POST["ex_proid"]){
	
	$sql = "SELECT count(*) FROM profeinep.processoprofeinep WHERE proid = ".$_POST["ex_proid"];
	$cont = $db->pegaUm($sql);
	
	if((integer)$cont == 0)
	{
		$sql = "DELETE FROM 
   					profeinep.procedencia
   				WHERE
		   			proid =	".$_POST["ex_proid"];
		$db->executar($sql);
		$db->commit();
		
		echo("<script>alert('A procedência foi excluída com sucesso.');</script>");
	}
	else
	{
		echo("<script>alert('A procedência não pode ser excluída pois possui referência em outras tabelas.');</script>");
	}
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
monta_titulo( 'PROFE/INEP', 'Lista de Procedências' ); 
 
?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script>
	function conPesquisaProcedencia(){
		document.getElementById('formPesquisaProcedencia').submit();
	}
</script>
<form method="post" action="" id="formProcedencia">
<input type="hidden" value="" id="ex_proid" name="ex_proid" />
</form>

<!-- Formulário de Pesquisa (soliticado por Luiz Fernando) -->
<form action="" method="post" name="formPesquisaProcedencia" id="formPesquisaProcedencia">
	<input type="hidden" name="requisicao" id="requisicao" value="pesquisar"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td colspan="2" class="SubTituloCentro">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td width="190px;" class="SubTituloDireita">Descrição:</td>
			<td>
				<?php
					$prodsc = $_REQUEST["prodsc"]; 
					print campo_texto('prodsc', 'N', 'S', '', 60, 60, '', '', 'left', '', 0, ''); 
				?>
			</td>
		</tr>
		<tr>
			<td width="190px;" class="SubTituloDireita">Unidade:</td>
			<td>
				<?php 
				
					$unpid = $_REQUEST["unpid"];
				
					$sql = "SELECT
								unpid as codigo,
								unpdsc as descricao
							FROM
								profeinep.unidadeprocedencia
							where 
								undpstatus='A'
							ORDER BY
								unpdsc";
				
					$db->monta_combo("unpid", $sql, "S", "Todas", '', '', '', '', 'N','unpid');
					
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<input type="button" value="Pesquisar" style="cursor: pointer;" onclick="conPesquisaProcedencia();"/>
			</td>
		</tr>
	</table>
</form>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<span style="cursor: pointer" onclick="cadastrarProcedencia(null);" title="Incluir Procedência">
					<img align="absmiddle" src="/imagens/gif_inclui.gif" /> <a href="#">Incluir Procedência</a>
				</span>
			</td>
		</tr>
	</tbody>
</table>

<? 
$sql = "SELECT
			'<img 
			align=\"absmiddle\" 
			src=\"/imagens/alterar.gif\" 
			style=\"cursor: pointer\" 
		 	onclick=\"javascript: cadastrarProcedencia(' || pro.proid || ');\" 
			title=\"Editar Procedência\" />
			
			<img
			align=\"absmiddle\"
			src=\"/imagens/excluir.gif\"
			style=\"cursor: pointer\"
		 	onclick=\"javascript: excluirProcedencia(' || pro.proid || ');\"
			title=\"Excluir Procedência\" /> 
			',
			
			pro.prodsc,
			unp.unpdsc,
			CASE WHEN pro.prodtstatus = 'A'
			THEN 'Ativo' ELSE 'Inativo' END as prodtstatus
		FROM
			profeinep.procedencia pro
		INNER JOIN 
			profeinep.unidadeprocedencia unp ON unp.unpid = pro.unpid
										 AND unp.undpstatus = 'A'
		{$whereProcedencia}
		ORDER BY
			pro.prodsc ASC";
		
$cabecalho = array("Ações", "Descrição", "Unidade", "Status" ); 

$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', ''); 

?>

<script>

function cadastrarProcedencia(proid)
{
	if(proid == null)
		return windowOpen( '?modulo=sistema/geral/cadDadosProcedencia&acao=A','blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	else
		return windowOpen( '?modulo=sistema/geral/cadDadosProcedencia&acao=A&proid='+proid,'blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function excluirProcedencia(proid)
{
	var form	=	document.getElementById("formProcedencia");
	var exProid	=	document.getElementById("ex_proid");

	if( confirm("Deseja excluir a Procedência ?") )
	{
		exProid.value = proid;
		form.submit();
	}
}

</script>