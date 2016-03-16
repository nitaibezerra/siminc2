<?
require_once APPRAIZ . "evento/classes/CoObjeto.class.inc";

if($_POST['formulario']){
	extract($_POST);
	$obCoObjeto = new CoObjeto();
	$obCoObjeto->cooid    	= $cooid;
	$obCoObjeto->coodsc 	= $coodsc;
	$obCoObjeto->coostatus 	= 'A';
	$obCoObjeto->salvar();
	$obCoObjeto->commit();
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/cadCompraObjeto","&cooid=".$obCoObjeto->cooid);
	unset($obCoObjeto);
	die;
}
if($_GET['cooidExcluir']){
	$obCoObjeto = new CoObjeto();
	$obCoObjeto->excluir($_GET['cooidExcluir']);
	$obCoObjeto->commit();
	unset($_GET['cooidExcluir']);
	unset($obCoObjeto);
	$_REQUEST['acao'] = 'I';
	$db->sucesso("principal/cadCompraObjeto");
	die;
}

$obCoObjeto = new CoObjeto($_GET['cooid']);
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
?>
<form method="post" name="formulario" id="formulario" action="/evento/evento.php?modulo=principal/cadCompraObjeto&acao=A">
<input type="hidden" name="formulario" id="formulario" value="1" />
<input type="hidden" name="cooid" id="cooid" value="<? echo $obCoObjeto->cooid; ?>" />
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" >
					Descrição do Objeto:
			</td>
			<td><?$coodsc = $obCoObjeto->coodsc; ?>
				<?= campo_texto( 'coodsc', 'S', $permissao_formulario, 'Descrição do Objeto', 67, 100, '', '','','','','id="coodsc"'); ?>
			</td>
		</tr>
	</table>
	<div id=buttonAcao>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
			<tr>
				<td class="SubTituloDireita" colspan="2" style="text-align:center">
					<input type="button" value="Salvar" onclick="enviaForm();"  /> <input type="button" value="Incluir Novo" onclick="window.location.href='evento.php?modulo=principal/cadCompraObjeto&acao=I';" />
				</td>
			</tr>
	</table>
	</div>
</form>
<?
$sql = "select 
			'<a style=\"margin: 0 -5px 0 5px;\" href=\"evento.php?modulo=principal/cadCompraObjeto&acao=A&cooid='|| cooid ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
			&nbsp;<a style=\"margin: 0 -5px 0 5px;\" style=\"cursor:hand\" href=\"#\" onclick=\"excluirObjeto(' || cooid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>' as acao,
			'<a style=\"margin: 0 -5px 0 5px;\" href=\"evento.php?modulo=principal/cadCompraObjeto&acao=A&cooid='|| cooid ||'\">' || coodsc ||'</a>' as descricao
	     from evento.coobjeto";
	
$arObjetos = $db->carregar($sql);
$cabecalho = array("Ação","Descrição");
$db->monta_lista_array($arObjetos, $cabecalho, 50, 20, '', 'center','');
echo "<br />";
?>
<script type="text/javascript">
<!--
    function enviaForm(){
	 	var nomeform 		= 'formulario';
		var submeterForm 	= true;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		campos[0] 			= "coodsc";
						 
		tiposDeCampos[0] 	= "texto";
		
		validaForm(nomeform, campos, tiposDeCampos, submeterForm );
	}
    function excluirObjeto(cooid){
		if(confirm('Deseja excluir registro')){
			window.location.href = "/evento/evento.php?modulo=principal/cadCompraObjeto&acao=A&cooidExcluir="+cooid;
			return true;
		} else {
			return false;
		}
	}
-->
</script>