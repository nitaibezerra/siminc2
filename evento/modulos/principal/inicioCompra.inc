<?
// REMOVER
if ( $_REQUEST['remover'] ) 
{
	$copid = (integer) $_REQUEST['remover'];  
	$sql = "update evento.coprocesso SET copstatus = 'I' WHERE copid = " . $copid;
	$db->executar( $sql );
	$db->commit();
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/inicioCompra");
	die;
}

if( $_POST['ajaxsession'] ){
	$_SESSION['copid'] = $_POST['ajaxsession']; 
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

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );

if( $_POST['filtro_copnumprocesso']){
	$where = " AND co.copnumprocesso LIKE '%{$_POST['filtro_copnumprocesso']}%' ";
} 
if( $_POST['filtro_cooid']){
	$where .= " AND co.cooid= '{$_POST['filtro_cooid']}' ";
}
if( $_POST['filtro_copdataabertura']){
	$where .= " AND to_char(co.copdataabertura::date,'YYYY') = '{$_POST['filtro_copdataabertura']}' ";
} 
  
$camposSQL = "
SELECT
 DISTINCT
 	'<center><img align=\"absmiddle\" src=\"/imagens/alterar.gif\"  style=\"cursor: pointer\" onclick=\"javascript: selecionarProcesso('|| co.copid ||' );\" title=\"Selecionar Processo\"> 
	<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"removerProcesso( '|| co.copid ||' );\" title=\"Excluir Processo\"/></center>' as copid,
	 CASE
	  WHEN ax.copid IS NULL THEN ''
	  WHEN ax.copid IS NOT NULL THEN '<center><img src=\"/imagens/anexo.gif\" style=\"cursor: pointer\" onclick=\"selecionaProcesso('||co.copid||');\"> </img></center>' 
	 END as doc,		 
   	'<div align=\"left\"> <a href=\"javascript:selecionarProcesso(  ' ||co.copid || '  );\" title=\"Selecionar Processo\" > ' ||co.copnumprocesso||' </div>'  as copnumprocesso, 
	co.copdsc,
	to_char(co.copdatalimite,'dd/mm/YYYY') as copdatalimite ";
 $sqlAll = " 
 $camposSQL
    FROM
    evento.coprocesso AS co
    LEFT JOIN seguranca.usuario AS u ON u.usucpf = co.usucpf
    LEFT JOIN evento.coanexo ax on co.copid = ax.copid
    where co.copstatus = 'A'
     $condicao 
     $where 
   ORDER BY copid 
"; 
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
					<div style="float: left;">
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Nº Processo:
									<br/> 
									<?= campo_texto( 'filtro_copnumprocesso', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
								<!-- td valign="bottom">
									Objeto:
									<br/>
									<?php
									$filtro_cooid = $_REQUEST['filtro_cooid'];
									 $sql = "SELECT 
				                            cooid AS codigo, 
				                            coodsc AS descricao
				                        FROM
				                            evento.coobjeto
				                       "; 
									$db->monta_combo( "filtro_cooid", $sql, 'S', 'Selecione...', 'filtraTipo', '' );
									?>
								</td -->
							
								 <td valign="bottom">
									Ano de Cadastramento
									<br/>
									<?php
									$filtro_copdataabertura = $_POST['filtro_copdataabertura'];
									$sql = "select distinct (to_char(copdataabertura::date,'YYYY')) as codigo,
									(to_char(copdataabertura::date,'YYYY')) as descricao
									from evento.coprocesso
									where copdataabertura is not null
									order by descricao";
									$db->monta_combo( "filtro_copdataabertura", $sql, 'S', 'Selecione...', '', '', '', '', '', 'filtro_copdataabertura' );
									?>
								</td>							
							</tr>
						</table>
						<div style="float: left;">
						<br>
							<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
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
					onclick="cadastrarProcesso();"
					title="Novo Processo"
				>
					<img
						align="absmiddle"
						src="/imagens/gif_inclui.gif"
					/>
					Cadastrar Processo
				</span>
			</td>
		</tr>
	</table>
<?}?>	 
<?php
	$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "", "Nº Processo", "Título do Processo", "Data Limite" ); 
	$db->monta_lista( $sqlAll, $cabecalho, 25, 10, 'N', '', '');
?>
<script type="text/javascript"><!--
function removerProcesso( id ) {
 
	if ( confirm( 'Deseja excluir o Processo?' ) ) {
		window.location.href = 'evento.php?modulo=principal/inicioCompra&acao=A&remover='+id;
	}
}

function selecionarProcesso( copid )
{   
	window.location.href = 'evento.php?modulo=principal/cadProcesso&acao=A&copid='+copid; 
}

function cadastrarProcesso(){ 
	var req = new Ajax.Request('?modulo=principal/cadProcesso&acao=I', {
				        method:     'post',
				        parameters: '&ajaxunsetsession=1',							         
				        onComplete: function (res) { 
							window.location.href = '?modulo=principal/cadProcesso&acao=I';
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
function validaForm()
{ 
  	document.formulario.submit(); 
}

function pesquisaByFiltro( tipo ){
	 if(tipo){
	 	window.location.href="?modulo=inicio&acao=C&pesqAvancada=t&tipoPesq="+tipo;
	 }
}

function selecionaProcesso( id ){
	var req = new Ajax.Request('evento.php?modulo=principal/inicioCompra&acao=A', {
					        method:     'post',
					        parameters: '&ajaxsession=' + id,							         
					        onComplete: function (res) {	
								window.location.href = '?modulo=principal/cadCompraAnexo&acao=A';
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