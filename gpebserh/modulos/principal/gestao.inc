<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

unset($_SESSION['cpf']);
function listacol($where){
	global $db;
	$cabecalho = array("&nbsp;","CPF", "Nome do Colaborador", "Cargo", "Escolaridade");
	$colunaAcoes = "'<center>'|| '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarDado(\'' || dpecpf || '\');\" title=\"Editar os Dados\" border=\"0\">' || '&nbsp;<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirColaborador(\'' || dpecpf || '\');\"> ' || 
					   	' </center>' as acoes,";
		
	$sql = " SELECT DISTINCT ".$colunaAcoes." dpecpf as cpf, dpenome as nome,carid as cargo,nesid as escolaridade FROM gpebserh.dadospessoais WHERE dpestatus = 'A'".(($where)?"AND ". implode(" AND ", $where):"");
	ver(simec_htmlentities($sql),d);	
	$db->monta_lista($sql,$cabecalho,100,5,'N','center','N','listacolaborador');
}
//Chamada de programa
if ($_POST['filtraMunicipio'] && $_POST['estuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipioEBS($_POST['estuf']);
	exit;
}

if( $_POST['deletaCol'] && $_POST['dpeid']){
	$sql = "UPDATE gpebserh.dependente SET depstatus = 'I' WHERE depid IN (select depid from gpebserh.pessoaldependente where dpeid =(Select dpeid from gpebserh.dadospessoais where dpeid = '".$_POST['dpeid']."'));";
	$sql .= "DELETE FROM gpebserh.pessoaldependente WHERE depid in(select depid from gpebserh.pessoaldependente where dpeid =(Select dpeid from gpebserh.dadospessoais where dpeid = '".$_POST['dpeid']."'));";
	$sql .= "UPDATE gpebserh.dadospessoais SET dpestatus = 'I' WHERE dpeid = '".$_POST['dpeid']."';";
	
	
	$db->executar($sql);
	$db->commit();
//	$where = array();
	//echo listaCol($where);
	exit;
	
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );
//ver($abacod_tela, $url,$titulo_modulo,d);

?>
<style>
	#pesquisas thead tr td
	{
		background-color: #F7F7F7;
	}

	#pesquisas thead tr td.textosTabela
	{
		width: 200px;
	}
	#pesquisas thead tr td.camposTabela
	{
		width: 300px;
	}
	#pesquisas tbody tr td
	{
		background-color: #F7F7F7;
	}
	
	#pesquisas tbody tr td.SubTituloDireita
	{
		FONT-SIZE: 8pt;
    	COLOR: black;
    	FONT-FAMILY: Arial, Verdana;
    	TEXT-ALIGN: right;
		BACKGROUND-COLOR: #dcdcdc;
	}
	
	#pesquisas tbody tr td input.botao
	{
		border: 1px solid #757575;
		background-color: #DCDCDC;
		font-size: 11px;
		font-family: arial;
		font-weight: bold;
	}
	
	
	
	
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
jQuery.noConflict(); 
 
jQuery(document).ready(function(){

	jQuery('.novo').click(function(){
		window.location='gpebserh.php?modulo=principal/cadColaborador&acao=A';

	});
	
	
	jQuery('#estuf').change(function(){
			
		var estuf = jQuery(this).val();
		if(estuf!=''){
			
			jQuery.ajax({
				
				   type: "POST",
				   url: window.location,
				   data: "filtraMunicipio=true&estuf=" + estuf,
				   success: function(resp){
						jQuery('#td_municipio').html(resp);
					}
			});
		}
	});
	
});



function excluirColaborador(dpeid){
	//alert(dpecpf);
	if( confirm("Tem certeza que deseja excluir esse colaborador?") ){
		jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "deletaCol=true&dpeid=" + dpeid,
        	   success: function(resp){
			  		alert('Operação realizada com sucesso!');
			  		window.location='gpebserh.php?modulo=principal/gestao&acao=A'
			}
		});
	}
}
function alterarDado(dpeid){
	window.location='gpebserh.php?modulo=principal/cadColaborador&acao=E&dpeid='+dpeid;
}

</script>
<form method="post" name="formulario" id="formulario">
<table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<thead>
		<tr>
			<td class="textosTabela"></td>
			<td class="camposTabela"></td>
			<td class="textosTabela"></td>
			<td></td>
		</tr>
	</thead>
	<tbody>
	<tr>
		<td class="SubTituloDireita">CPF:</td>
		<td colspan="3">
			<?php
				$filtro = $_POST['cadcpf'];
				$cadcpf = $filtro;
				
				echo campo_texto( 'cadcpf', 'N', 'S', '', 20, 80, '###.###.###-##', '','left','',0,'id="cadcpf"','',$cadcpf,""); 
			?>
		</td>
				
	</tr>
    <tr>
		<td class ="SubTituloDireita" align="right">Nome: </td>
			<td>
				<?= campo_texto('dpenome', 'N', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="dpenome" onblur="MouseBlur(this);"' ); ?>  
			</td>
	</tr>
	<tr>
		<td class ="SubTituloDireita" align="right">Cargo: </td>
		<td>
			<?
				$carid = $_POST['carid'];
				$sqlCargo = "SELECT carid as codigo, cardsc as descricao FROM gpebserh.cargo";
				$db->monta_combo("carid", $sqlCargo, "S","Selecione","","","","","N","carid");
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Nível de Escolaridade:</td>
		<td colspan="3">
			<?
			$escolaridade = $_POST['nesid'];
			$sql = "SELECT nesid as codigo, nesdsc as descricao FROM gpebserh.nivelescolaridade";
			
			$db->monta_combo( "nesid", $sql, 'S', 'Selecione', '', '', '', '', 'N', "escolnesidaridade" );
			?>
		</td> 
	</tr>
	<tr>
		<td class="SubTituloDireita" >Estado Civil:</td>
		<td colspan="3">
		<?php
			$sql = "SELECT etcid as codigo, etcdsc as descricao FROM gpebserh.estado_civil";
			$db->monta_combo( "etcid", $sql, 'S', 'Selecione', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Estado:</td>
		<td colspan="3">
		<?php
			$estuf = $_POST['estuf'];
	        $sql = "SELECT estuf AS codigo, estdescricao AS descricao FROM territorios.estado ORDER BY estdescricao"; 
			$db->monta_combo( "estuf", $sql, "S", "Selecione...", '', "","","","N","estuf","",$estuf); 
			//$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
		?>
		</td>
	</tr>
		<tr>
		<td class="SubTituloDireita" >Cidade:</td>
		<td id = "td_municipio" colspan="3">
		
		<?php
			$sql = "SELECT ter.muncod AS codigo, ter.mundescricao AS descricao FROM territorios.municipio ter WHERE ter.estuf = '$estuf' ORDER BY ter.estuf, ter.mundescricao";
			$db->monta_combo("muncodlogradouro",$sql,"S","Selecione...",'',"","","","N","muncodlogradouro","",$muncodlogradouro);
			
			//$sql = "select 	muncod as codigo, mundescricao as descricao FROM territorios.municipio";
			//$db->monta_combo( "municipio", $sql, 'S', 'Selecione', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class ="SubTituloDireita" align="right">Tipo de Deficiência: </td>
		<td>
			<? 
				$sql = "SELECT tpdid as codigo, tpddsc as descricao FROM gpebserh.tipodeficiencia";
				$db->monta_combo("tpdid",$sql,"S","Selecione...",'','');?>
			
		</td>
	</tr> 
	<tr>
		<td class="SubTituloDireita"></td>
		<td colspan="3">
			<input class="botao" type="submit" name="pesquisar" id= "pesquisar" value="Pesquisar">
			<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location='gpebserh.php?modulo=principal/gestao&acao=A';" />
		</td>
	</tr>
	</tbody>
</table>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" >
	<tr>
		<td colspan="3">
			<a href="#" class="novo"> 
				<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" value = "novo" border="0" title="Incluir Novo Colaborador"> Incluir Novo Colaborador
			</a>
		</td>
	</tr>
</table>
</form>
<?php

	if($_REQUEST['pesquisar']) {
   
		if($_POST['cadcpf']) {
			$cpf = str_replace('.','',$_POST['cadcpf']);
			$cpf = str_replace('-','',$cpf);
			$where[] = "dpecpf = '".$cpf."'";
		}
		if($_POST['dpenome']) {
			$where[] .= "dpenome ILIKE removeacento('%".$_POST['dpenome']."%')";
		}
		
		if($_POST['estuf']) {
			$where[] .= "estuf = '".$_POST['estuf']."'";
		}
		
		if($_POST['carid']) {
			$where[] .= "carid = ".$_POST['carid'];
		}
		if($_POST['nesid']) {
			$where[] .= "nesid = ".$_POST['nesid'];
		}
		
		if($_POST['muncodlogradouro']) {
			$where[] .= "muncodlogradouro = '".$_POST['muncodlogradouro']."' ";
		}
		if($_POST['etcid']) {
			$where[] .= "etcid = '".$_POST['etcid']."' ";
		}
		
		if($_POST['tpdid']){
			$where[] = "tpdid = ".$_POST['tpdid'];
		}
	}
	?>
	<div id = "listaColaborador">
	<?
	//ver($where,d);
	//listaCol($where);
	$cabecalho = array("&nbsp;","CPF", "Nome do Colaborador", "Cargo", "Escolaridade");
	$colunaAcoes = "'<center>'|| '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarDado(\'' || dpeid || '\');\" title=\"Editar os Dados\" border=\"0\">' || '&nbsp;<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirColaborador(\'' || dpeid || '\');\"> ' || 
					   	' </center>' as acoes,";
		
	$sql = " SELECT DISTINCT ".$colunaAcoes." dp.dpecpf as cpf, dp.dpenome as dpenome, cg.cardsc as cargo,ne.nesdsc as escolaridade FROM gpebserh.dadospessoais dp join gpebserh.cargo cg ON cg.carid = dp.carid join gpebserh.nivelescolaridade ne ON ne.nesid = dp.nesid  WHERE dpestatus = 'A'".(($where)?"AND ". implode(" AND ", $where):"");
	//ver(simec_htmlentities($sql),d);	
	$db->monta_lista($sql,$cabecalho,100,5,'N','center','N','listacolaborador');
//	$cabecalho = array("&nbsp;","CPF", "Nome do Colaborador", "Cargo", "Escolaridade");
//	$colunaAcoes = "'<center>'|| '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarDado(\'' || dpecpf || '\');\" title=\"Editar os Dados\" border=\"0\">' || '&nbsp;<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirColaborador(\'' || dpecpf || '\');\"> ' || 
//					   	' </center>' as acoes,";
//		
//	$sql = " SELECT DISTINCT ".$colunaAcoes." dpecpf as cpf, dpenome as nome,carid as cargo,nesid as escolaridade FROM gpebserh.dadospessoais ".(($where)?"WHERE ".implode(" AND ", $where):"");
//	//ver(simec_htmlentities($sql),d);	
//		$db->monta_lista($sql,$cabecalho,100,5,'N','center','N','listacolaborador');
	?>
	</div>
 
<table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">

</table>