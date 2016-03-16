<?php
verificaSessao();
// REMOVER
if ( $_REQUEST['remover'] )
{
	$copid = (integer) $_REQUEST['remover'];
	$sql = "update evento.coprocesso SET evestatus = 'I' WHERE copid = " . $copid;
	$db->executar( $sql );
	$db->commit();

	redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
}

$perfis = arrayPerfil();

if( $_POST['ajaxsession'] ){
	$_SESSION['copid'] = $_POST['ajaxsession']; 
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$arUnidadesVisiveis = verificaUnidadesPermitidadas();
$arMuiid = array();
if( $arUnidadesVisiveis[0] && !$arUnidadesVisiveis[1]  ){
	array_push( $arMuiid, '4942' );
}

$db->cria_aba( $abacod_tela, $url, '', $arMuiid );
monta_titulo( $titulo_modulo, '' );

if( $_POST['filtro_copnumprocesso']){
	$where = " AND co.copnumprocesso LIKE '%{$_POST['filtro_copnumprocesso']}%' ";
} 
if( $_POST['filtro_situacao']){
	$where .= " AND wed.esdid = '{$_POST['filtro_situacao']}' ";
}
if( $_POST['filtro_cooid']){
	$where .= " AND co.cooid= '{$_POST['filtro_cooid']}' ";
} 
if( $_POST['filtro_copdsc']){
	$where .= " AND co.copdsc LIKE '%{$_POST['filtro_copdsc']}%' ";
} 
if( $_POST['filtro_adesao']){
	$where .= " AND di.coaid is not null ";
} 

$camposSQL = "
	SELECT DISTINCT
		'<center><img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: selecionarProcessoUnidade('|| co.copid ||' );\" title=\"Selecionar Processo\"></center>' as copid,
		CASE
			WHEN ax.copid IS NULL THEN ''
			WHEN ax.copid IS NOT NULL THEN '<center><img src=\"/imagens/anexo.gif\" style=\"cursor: pointer\" onclick=\"abrirPopupAnexo('||co.copid||');\"></img></center>' 
		END as doc,		 
		'<div align=\"left\"><a href=\"javascript:selecionarProcessoUnidade(' ||co.copid || ');\" title=\"Selecionar Processo\">' ||co.copnumprocesso||' </div>'  as copnumprocesso,  
		co.copdsc,
		to_char(co.copdatalimite,'dd/mm/YYYY') as copdatalimite,
		case when count(aps.participante) > 0
			then 'Participante'
			else 'Não Participante'
		end as participante
";

$sqlAll = " $camposSQL
FROM
	evento.coprocesso AS co
LEFT JOIN seguranca.usuario 		 u ON u.usucpf = co.usucpf
LEFT JOIN evento.coadesao 			 a ON co.copid = a.copid
LEFT JOIN evento.codemandaitem      di ON  a.coaid = di.coaid
LEFT JOIN evento.coanexo 		    ax on co.copid = ax.copid
LEFT JOIN workflow.documento 	    wd ON  a.docid = wd.docid
LEFT JOIN workflow.estadodocumento wed ON wd.esdid = wed.esdid
INNER JOIN evento.couasgnivelcontratacao nc on nc.usgid = {$_SESSION['unidade']}
LEFT JOIN ( SELECT 
				case when sum(coalesce(di.coaid,0)) is not null 
					then 'Participante'
					else 'Não Participante'
				end as participante,
				a.usgid,
				co2.copid
            FROM evento.coprocesso AS co2
            LEFT JOIN evento.coadesao a on co2.copid = a.copid
            LEFT JOIN evento.codemandaitem di on a.coaid = di.coaid 
            GROUP BY a.usgid,co2.copid
           ) as aps ON aps.usgid = nc.usgid AND aps.copid = co.copid
WHERE co.copid is not null 
	AND co.conid = nc.conid 
	AND co.copstatus = 'A'
$condicao
$where
GROUP BY
	ax.copid,
	co.copid,
	co.copnumprocesso,
	co.copdsc,
	co.copdatalimite
ORDER BY 
	copid";
?>
<script src="../includes/prototype.js"></script>
<script src="../includes/calendario.js"></script>
<script type="text/javascript">
function removerFiltro(){
	document.formulario.filtro.value = "";
	document.formulario.estuf.selectedIndex = 0;
	document.formulario.submit();
}
</script>
<?php
montaCabecalhoUnidade($_SESSION['unidade']);
?>
<table align="center" border="0" class="tabela" cellpadding="3"
	cellspacing="1">
	<tbody>
		<tr>
			<td
				style="padding: 15px; background-color: #e9e9e9; color: #404040; vertical-align: top;"
				colspan="4">
			<form action="" method="POST" name="formulario"><input type="hidden"
				name="acao" value="<?= $_REQUEST['acao'] ?>" />
			<div style="float: left;">

			<table border="0" cellpadding="3" cellspacing="0">
				<tr>
					<td valign="bottom">
						Nº Processo:
						<br/> 
						<?= campo_texto( 'filtro_copnumprocesso', 'N', 'S', '', 50, 200, '', '' ); ?>
					</td>
					<td>
						Situação:
						<br>
						<?php
							$filtro_situacao = $_POST['filtro_situacao'];
							$sql = sprintf("SELECT
											 esdid AS codigo,
											 esddsc AS descricao
											FROM
											 workflow.estadodocumento et
											 where tpdid = 7
											  ");
							$db->monta_combo( "filtro_situacao", $sql, 'S', 'Selecione...', '', '', '', '', '', 'filtro_situacao' );
						?>				
					</td>
					<td valign="bottom">
						<?php 
						$check = false;
						if($filtro_adesao = $_POST['filtro_adesao']){
							$check = "checked='checked'";
						}
						?>
						Adesão: <input type="checkbox" name="filtro_adesao" <?php echo $check; ?> id="filtro_adesao" value="1" />	
					</td>
				</tr>
			</table>
			<div style="float: left;"><br>
			<input type="button" name="" value="Pesquisar"
				onclick="return validaForm();" /></div>
			</div>
			</form>
			</td>
		</tr>
	</tbody>
</table>
<?php
$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "", "Nº Processo", "Título do Processo", "Data Limite", "Adesão" );
$db->monta_lista( $sqlAll, $cabecalho, 25, 10, 'N', 'center', '');

?>
<script type="text/javascript"><!--
function selecionarProcessoUnidade( copid ){     
	window.location.href = '?modulo=principal/cadProcessoUnidade&acao=A&copid='+copid;  
}
 
function validaForm()
{ 
  	document.formulario.submit(); 
}

function selecionaProcesso( id ){
	var req = new Ajax.Request('evento.php?modulo=principal/listaProcessoUnidade&acao=A', {
					        method:     'post',
					        parameters: '&ajaxsession=' + id,							         
					        onComplete: function (res) {	
								window.location.href = '?modulo=principal/cadCompraAnexo&acao=A';
							}
		});
}
function abrirPopupAnexo( id ){
	var req = new Ajax.Request('evento.php?modulo=principal/listaProcessoUnidade&acao=A', {
					        method:     'post',
					        parameters: '&ajaxsession=' + id,							         
					        onComplete: function (res) {	
					        	window.open('evento.php?modulo=principal/popupCompraAnexo&acao=A','','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=1000,height=500');
					        	 
							}
		});
}
</script>