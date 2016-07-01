<?php
	if($_REQUEST['consulta'] == '')
		$_REQUEST['consulta'] = 'html';
// REMOVER
if ( $_REQUEST['remover'] ) 
{
	$ctrid = (integer) $_REQUEST['remover'];  
	$sql = "update evento.ctcontrato SET ctrstatus = 'I' WHERE ctrid = " . $ctrid;
	$db->executar( $sql );
	$db->commit();
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/inicioContrato");
	die;
}

if( $_POST['ajaxsession'] ){
	$_SESSION['ctrid'] = $_POST['ajaxsession']; 
}

//$ungcod = getUnidadeByCpf();
$perfis = arrayPerfil();
						 
/*if( ( ( in_array(PERFIL_SEE, $perfis ) ) ||in_array(PERFIL_SAA, $perfis ) ) || ( in_array(PERFIL_SUPER_USUARIO, $perfis ) ) ){
	$condicao = '';
}elseif ( in_array(PERFIL_DEMANDANTE, $perfis) ){
	$condicao = " AND ( ( co.ungcod = '$ungcod' ) OR co.usucpf = '" . $_SESSION['usucpf'] . "' ) ";	
}else{
	$condicao = " AND (co.ungcod = '$ungcod' OR co.usucpf = '" . $_SESSION['usucpf'] . "' )";
}*/

if( $_POST['filtro_entnome']){
	$where = " AND UPPER(ent.entnome) LIKE UPPER('%{$_POST['filtro_entnome']}%')";
} 
if( $_POST['filtro_tpcid']){
	$where .= " AND ctr.tpcid= '{$_POST['filtro_tpcid']}' ";
} 

if( $_POST['filtro_modid']){
	$where .= " AND ctr.modid= '{$_POST['filtro_modid']}' ";
} 

if( $_POST['filtro_ctrobj']){
	$where .= " AND UPPER (ctr.ctrobj) LIKE UPPER('%{$_POST['filtro_ctrobj']}%')";
} 

if( $_POST['filtro_ctrnum']){
	$where .= " AND ctr.ctrnum = '{$_POST['filtro_ctrnum']}' ";
} 

if( $_POST['filtro_ctrano']){
	$where .= " AND ctr.ctrano = '{$_POST['filtro_ctrano']}' ";
} 

if( $_POST['filtro_sitid']){
	$where .= " AND ctr.sitid= '{$_POST['filtro_sitid']}' ";
} 

if( $_POST['filtro_ctrnummod']){
	$where .= " AND ctr.ctrnummod= '{$_POST['filtro_ctrnummod']}' ";
} 

if( $_POST['filtro_ctrproccontr']){
	$where .= " AND ctr.ctrproccontr LIKE '%{$_POST['filtro_ctrproccontr']}%' ";
} 

if( $_POST['filtro_ctrprocexecctr']){
	$where .= " AND ctr.ctrprocexecctr LIKE '%{$_POST['filtro_ctrprocexecctr']}%' ";
} 

if( $_POST['filtro_ctrprocexecfin']){
	$where .= " AND ctr.ctrprocexecfin LIKE '%{$_POST['filtro_ctrprocexecfin']}%' ";
} 


$camposSQL = " SELECT ";
	if($_REQUEST['consulta'] == 'html'){
		$camposSQL .= "
			 DISTINCT
			 	'<center><img align=\"absmiddle\" src=\"/imagens/alterar.gif\"  style=\"cursor: pointer\" onclick=\"javascript: selecionarContrato('|| ctr.ctrid ||' );\" title=\"Selecionar Contrato\"> 
				<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"removerContrato( '|| ctr.ctrid ||' );\" title=\"Excluir Contrato\"/></center>' as ACAO,
				 CASE
				  WHEN anc.ctrid IS NULL THEN ''
				  WHEN anc.ctrid IS NOT NULL THEN '<center><img src=\"/imagens/anexo.gif\" style=\"cursor: pointer\" onclick=\"selecionaContrato('||ctr.ctrid||');\"> </img></center>' 
				 END as doc,
		";
	}
	else 
		$camposSQL .= " distinct ctr.ctrid, ";
	$camposSQL .= " tpc.tpcdsc as tipo, ";
	if($_REQUEST['consulta'] == 'html')
		$camposSQL .= " '<div align=\"left\"> <a href=\"javascript:selecionarContrato(  ' ||ctr.ctrid || '  );\" title=\"Selecionar Contrato\" > ' ||ctr.ctrnum || ' </div>'  as numerocontrato, ";
	else 
		$camposSQL .= " ctr.ctrnum as numerocontrato, ";
	$camposSQL .= "
   	   ctr.ctrano as ano,
   	   ctr.ctrnummod as numeromodalidade,
	   sit.sitdsc as situacao,
	";
	if($_REQUEST['consulta'] == 'html')
		$camposSQL .= " '<div align=\"left\"> <a href=\"javascript:selecionarContrato(  ' ||ctr.ctrid || '  );\" title=\"Selecionar Contrato\" > ' ||ctr.ctrobj || ' </div>'  as objeto, ";
	else 
		$camposSQL .= " ctr.ctrobj as objeto, ";
	$camposSQL .= "
       ent.entnome as contratada, 
       mod.moddsc as modalidade ";
	
 $sqlAll = " 
 $camposSQL
    FROM
       evento.ctcontrato AS ctr
       LEFT JOIN entidade.entidade AS ent ON ent.entid = ctr.entidcontratada
       LEFT JOIN evento.cttipocontrato AS tpc ON tpc.tpcid = ctr.tpcid
       LEFT JOIN evento.ctanexo anc on anc.ctrid = ctr.ctrid
       LEFT JOIN evento.ctmodalidadecontrato mod on ctr.modid = mod.modid
       LEFT JOIN evento.ctsituacaocontrato sit on ctr.sitid = sit.sitid
    where ctr.ctrstatus = 'A' 
      $condicao 
     $where 
   ORDER BY ano desc, numerocontrato desc
";
     
	if($_REQUEST['consulta'] == 'xls'){
		header('content-type: text/html; charset=ISO-8859-1');
		$cabecalho = array("", "Tipo", "Número", "Ano", "Número Modalidade", "Situação", "Objeto", "Contratada", "Modalidade");
		$db->sql_to_excel($sqlAll, 'listaContratos', $cabecalho);
		exit;
	}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' ); 
     
     //dbg($sqlAll,1);
?>
<script src="../includes/prototype.js"></script>
<script type="text/javascript">
	function removerFiltro(){
		document.formulario.filtro.value = "";
		document.formulario.estuf.selectedIndex = 0;
		document.formulario.submit();
	}
</script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="POST" name="formulario">
					<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
					<input type="hidden" name="consulta" id="consulta" value="html">
					<div style="float: left;">
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Contratada:
									<br/> 
									<?= campo_texto( 'filtro_entnome', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
								<td valign="bottom">
									Tipo Contrato:
									<br/>
									<?php
									$filtro_tpcid = $_REQUEST['filtro_tpcid'];
									 $sql = "SELECT 
				                            tpcid AS codigo, 
				                            tpcdsc AS descricao
				                        FROM
				                            evento.cttipocontrato
				                       "; 
									$db->monta_combo( "filtro_tpcid", $sql, 'S', 'Selecione...', 'filtraTipo', '' );
									?>
								</td>
																<td valign="bottom">
									Modalidade:
									<br/>
									<?php
									$filtro_modid = $_REQUEST['filtro_modid'];
									 $sql = "SELECT 
				                            modid AS codigo, 
				                            moddsc AS descricao
				                        FROM
				                            evento.ctmodalidadecontrato
				                       "; 
									$db->monta_combo( "filtro_modid", $sql, 'S', 'Selecione...', 'filtraTipo', '' );
									?>
								</td>
								
							</tr>
							<tr>
								<td valign="bottom">
									Objeto:
									<br/> 
									<?= campo_texto( 'filtro_ctrobj', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
								<td valign="bottom">
									Número:
									<br/> 
									<?= campo_texto( 'filtro_ctrnum', 'N', 'S', '', 3, 200, '', '' ); ?>
								</td>
								<td valign="bottom">
									Ano:
									<br/> 
									<?= campo_texto( 'filtro_ctrano', 'N', 'S', '', 5, 200, '', '' ); ?>
								</td>
							
							</tr>						
							<tr>
								<td valign="bottom">
									Vigência:
									<br/> 
									<?php
									$filtro_sitid = $_REQUEST['filtro_sitid'];
									 $sql = "SELECT 
				                            sitid AS codigo, 
				                            sitdsc AS descricao
				                        FROM
				                            evento.ctsituacaocontrato
				                       "; 
									$db->monta_combo( "filtro_sitid", $sql, 'S', 'Selecione...', 'filtraTipo', '' );
									?>

								</td>
								<td valign="bottom">
									Número Modalidade:
									<br/> 
									<?= campo_texto( 'filtro_ctrnummod', 'N', 'S', '', 10, 200, '', '' ); ?>
								</td>
								<td valign="bottom">
									Processo de Contratação:
									<br/> 
									<?= campo_texto( 'filtro_ctrproccontr', 'N', 'S', '', 30, 50, '', '' ); ?>
								</td>
							
							</tr>						
							<tr>
								<td valign="bottom">
									Processo de Execução:
									<br/> 
									<?= campo_texto( 'filtro_ctrprocexecctr', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
								<td valign="bottom">
									Processo de Execução Financeira:
									<br/> 
									<?= campo_texto( 'filtro_ctrprocexecfin', 'N', 'S', '', 30, 50, '', '' ); ?>
								</td>
							</tr>						
							
						</table>
						<div style="float: left;">
						<br>
							<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
							<input type="button" name="" value="Visualizar XLS" onclick="return validaForm('xls');"/>
						</div>	
					</div>	
				</form>
			</td>
		</tr>
	</tbody>
</table>
<?php
if( ( ( in_array(PERFIL_CGCC, $perfis ) ) ||in_array(PERFIL_SAA, $perfis ) ) || ( in_array(PERFIL_SUPER_USUARIO, $perfis ) ) ){ 
?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:none;">
		<tr>
			<td style="padding: 10px;">
				<span
					style="cursor: pointer"
					onclick="cadastrarContrato();"
					title="Novo Contrato"
				>
					<img
						align="absmiddle"
						src="/imagens/gif_inclui.gif"
					/>
					Cadastrar Contrato
				</span>
			</td>
		</tr>
	</table>
<?}?>	 
<?php 
	$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "", "Tipo", "Número", "Ano","Número Modalidade" , "Situação",  "Objeto", "Contratada", "Modalidade" ); 
	$db->monta_lista( $sqlAll, $cabecalho, 25, 10, 'N', '', '','','');
?>
<script type="text/javascript"><!--
function removerContrato( id ) {
 
	if ( confirm( 'Deseja excluir o Contrato?' ) ) {
		window.location.href = 'evento.php?modulo=principal/inicioContrato&acao=A&remover='+id;
	}
}

function selecionarContrato( ctrid )
{   
	window.location.href = 'evento.php?modulo=principal/cadContrato&acao=A&ctrid='+ctrid; 
}

function cadastrarContrato(){ 
	var req = new Ajax.Request('?modulo=principal/cadContrato&acao=I', {
				        method:     'post',
				        parameters: '&ajaxunsetsession=1',							         
				        onComplete: function (res) { 
							window.location.href = '?modulo=principal/cadContrato&acao=I';
						}
	});  
}

function enviar_email( cpf ){
	var nome_janela = 'janela_enviar_emai_' + cpf;
	window.open(
		'/geral/envia_email.php?cpf=' + cpf,
		nome_janela,
		'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
	);
}
function validaForm(consulta){
	if(consulta == undefined)
		consulta = 'html';
	if(consulta == 'html')
		document.getElementById('consulta').value='html';
	else if(consulta == 'xls')
		document.getElementById('consulta').value='xls';
  	document.formulario.submit(); 
}

function pesquisaByFiltro( tipo ){
	 if(tipo){
	 	window.location.href="?modulo=inicio&acao=C&pesqAvancada=t&tipoPesq="+tipo;
	 }
}

function selecionaContrato( id ){
	var req = new Ajax.Request('evento.php?modulo=principal/inicioContrato&acao=A', {
					        method:     'post',
					        parameters: '&ajaxsession=' + id,							         
					        onComplete: function (res) {	
								window.location.href = '?modulo=principal/cadContratoAnexo&acao=A';
							}
		});
}

function filtraTipo(estuf)
{
	if( !estuf ){
		return false;
	}
	td 	   = document.getElementById('municipio');
	select = document.getElementsByName('muncod')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('evento.php?modulo=inicio&acao=C', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,
							        onComplete: function (res)
							        {			 
							        	var inner = 'Município<br/>';
										td.innerHTML = inner+res.responseText;
										td.style.visibility = 'visible';
							        }
							  });
    
}
</script>