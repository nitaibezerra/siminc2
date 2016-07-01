<?php
set_time_limit(0);

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");


include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

//echo montarAbasArray(carregaAbasEmendas('listaPrograma'), "/emenda/emenda.php?modulo=principal/listaPrograma&acao=A");
monta_titulo( 'Listar Entidade(s) Beneficiada(s)', "Filtro de Pesquisa" );
?>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>

<script>
	function pesquisar()
	{
		document.formulario.submit();	
	}
</script>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/entidadeBeneficiada.js"></script>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">CNPJ:</td>
		<td><?=campo_texto( 'entnumcpfcnpj', 'N', 'S', '', 40, 20, '', '','','','','id="entnumcpfcnpj"', ""); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome da Entidade:</td>
		<td><?=campo_texto( 'entnome', 'N', 'S', '', 40, 100, '', '','','','','id="entnome"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisar();"></th>
	</tr>
</table>
</form>
<div id="lista">
<? 
		$filtro = array();
		
	
		if($_POST['entnumcpfcnpj']){
			$filtro[] = " e.entnumcpfcnpj = '".$_POST['entnumcpfcnpj']."'";
		}
		if($_POST['entnome']){
			$filtro[] = " upper(e.entnome) ilike '%".strtoupper($_POST['entnome'])."%'";
		}
		
		$sql = "SELECT DISTINCT
				  e.entnumcpfcnpj,
				  e.entnome,
				  ende.estuf,
				  eb.enbid,
				  eb.enbsituacaohabilita,
				  to_char(eb.enbdataalteracao, 'DD/MM/YYYY HH24:MI:SS') as enbdataalteracao
				FROM 
					emenda.entidadebeneficiada eb 
					inner join entidade.entidade e
						ON (eb.entid = e.entid)
				    inner join entidade.endereco ende 
						ON (ende.entid = e.entid) 			  
				WHERE 
					eb.enbstatus = 'A'
					AND  e.entstatus = 'A'
					AND eb.enbano >= (select min(enbano) from emenda.entidadebeneficiada limit 1)
					". ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' )."
				order by 2";
		$cabecalho = array("CNPJ", "Nome", "UF", "Total CPFs Autorizados", "Situação Habilita", "Data Atualização");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
?>
</div>
	
</body>
</html>