<?php
	if($_REQUEST['consulta'] == '')
		$_REQUEST['consulta'] = 'html';

// REMOVER
if ( $_REQUEST['remover'] ) 
{
	$ctrid = (integer) $_REQUEST['remover'];  
	$sql = "update evento.ctcontratada SET ctastatus = 'I' WHERE ctaid = " . $ctaid;
	$db->executar( $sql );
	$db->commit();
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/listaContradada");
	die;
}

if( $_POST['ajaxsession'] ){
	$_SESSION['ctaid'] = $_POST['ajaxsession']; 
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

if( $_POST['filtro_ctanome']){
	$where = " AND UPPER(ent.entnome) LIKE UPPER('%{$_POST['filtro_ctanome']}%')";
} 
if( $_POST['filtro_estuf']){
	$where .= " AND ende.estuf = '{$_POST['filtro_estuf']}' ";
} 

/*if( $_POST['filtro_muncod']){
	$where .= " AND cta.muncod= '{$_POST['filtro_muncod']}' ";
} */


$camposSQL = " SELECT ";
if($_REQUEST['consulta'] == 'html'){
 $camposSQL .= "DISTINCT
 	'<center><img align=\"absmiddle\" src=\"/imagens/alterar.gif\"  style=\"cursor: pointer\" onclick=\"javascript: inserirEmpresa('|| ent.entid ||' );\" title=\"Selecionar Contratada\">' as ACAO,	 
   	'<div align=\"left\"> <a href=\"javascript:inserirEmpresa(  ' ||ent.entid || '  );\" title=\"Selecionar Contratada\" > ' || ent.entnome ||' </div>'  as nomecontratada, ";
}
elseif ($_REQUEST['consulta'] == 'xls'){
	$camposSQL .= " DISTINCT ent.entid,	 
	   	ent.entnome as nomecontratada, ";
}
$camposSQL .= "
   	   ent.entnumcpfcnpj || '&nbsp;' as cnpj,
   	   mun.mundescricao as municipio, 
 	   ende.estuf as uf ";
	
 $sqlAll = " 
 $camposSQL
    from entidade.entidade ent
    left join entidade.endereco ende on ende.entid = ent.entid	
    left join territorios.municipio as mun on mun.muncod = ende.muncod
	left join entidade.funcaoentidade fue on ent.entid = fue.entid
	where fue.funid = 46
      $condicao 
     $where 
   ORDER BY 5,4,2 
"; 
    if($_REQUEST['consulta'] == 'xls'){
		header('content-type: text/html; charset=ISO-8859-1');
		$cabecalho = array("Id", "Nome Contratada", "Número CNPJ", "Cidade", "UF");
		$db->sql_to_excel($sqlAll, 'listaContratadas', $cabecalho);
		exit;
	}
     
	//Chamada de programa
	include  APPRAIZ."includes/cabecalho.inc";
	echo'<br>';
	$db->cria_aba( $abacod_tela, $url, '' );
	monta_titulo( $titulo_modulo, '' );
?>

<form name="formulario" id="formulario" method="POST">
<div style="display:none" name="entnomeempresa" id="entnomeempresa" ></div>
<input type="hidden" name="entidempresa" id="entidempresa" value="">
<input type="hidden" name="consulta" id="consulta" value="html">
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
									Contratada:
									<br/> 
									<?= campo_texto( 'filtro_ctanome', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
								<!--  td valign="bottom">
									Município:
									<br/>
									<?php /*
									$filtro_tpcid = $_REQUEST['filtro_muncod'];
									 $sql = "SELECT 
				                            muncod AS codigo, 
				                            tpcdsc AS descricao
				                        FROM
				                            evento.cttipocontrato
				                       "; 
									$db->monta_combo( "filtro_tpcid", $sql, 'S', 'Selecione...', 'filtraTipo', '' );*/
									?>
								</td -->
								<td valign="bottom">
									UF:
									<br/>
									<?php
									$filtro_estuf = $_REQUEST['filtro_estuf'];
									 $sql = "SELECT 
				                            estuf AS codigo, 
				                            estdescricao AS descricao
				                        FROM
				                            territorios.estado order by 2
				                       "; 
									$db->monta_combo( "filtro_estuf", $sql, 'S', 'Selecione...', 'filtraTipo', '' );
									?>
								</td>
								
							</tr>
						</table>
						<div style="float: left;">
						<br>
							<input type="button" name="" value="Pesquisar" onclick="return validaFormx();"/>
							<input type="button" name="" value="Visualizar XLS" onclick="return validaFormx('xls');"/>
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
					onclick="inserirEmpresa();"
					title="Nova Contratada"
				>
					<img
						align="absmiddle"
						src="/imagens/gif_inclui.gif"
					/>
					Cadastrar Contratada
				</span>
			</td>
		</tr>
	</table>
<?}?>	
</form> 
<?php
	$cabecalho = array("<center>Ação</center>", "Nome Contratada", "Número CNPJ", "Cidade", "UF"); 
	$db->monta_lista( $sqlAll, $cabecalho, 25, 10, 'N', '', '');
?>
<script type="text/javascript"><!--






function validaFormx(consulta){
	if(consulta == undefined)
		consulta = 'html';
	if(consulta == 'html')
		document.getElementById('consulta').value='html';
	else if(consulta == 'xls')
		document.getElementById('consulta').value='xls';
  	document.formulario.submit();
}




var caminho_atual = "evento.php";
function inserirEmpresa(entidempresa){
	if (entidempresa){ 
			return windowOpen( caminho_atual + '?modulo=principal/inserir_empresa&acao=A&busca=entnumcpfcnpj&entid=' + entidempresa,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( caminho_atual + '?modulo=principal/inserir_empresa&acao=A','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}
</script>