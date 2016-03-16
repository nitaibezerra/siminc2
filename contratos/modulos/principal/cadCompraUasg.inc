<?php
require_once APPRAIZ . "evento/classes/CoUasg.class.inc";
if($_POST['formulario']){
	extract($_POST);
	
	//verifica se já existe
	if($usgcod != $usgcod_old){
		$sql = "select usgcod from evento.uasg where usgcod = '{$usgcod}' ";
		$e = $db->pegaUm($sql);
		if($e){
			$db->insucesso('Código UASG já existe. Favor digite outro código!','','principal/cadCompraUasg&acao=I');	
			die;
		}
	}
	
	$obCoUasg = new CoUasg();
	$obCoUasg->usgid    = $usgid;
	$obCoUasg->usgcod 	= $usgcod;
	$obCoUasg->usgdsc 	= $usgdsc;
	$obCoUasg->usggestaouasg = $usggestaouasg;
	$obCoUasg->salvar();
	
	$obCoUasg->deletaUasgContratacao( $obCoUasg->usgid );
	$arConid = $conid;
	$conid = "";
	if (count($arConid) > 0){
		foreach($arConid as $conid){
			$obCoUasg->insereUasgContratacao( $conid, $obCoUasg->usgid ); 
		}
	}
	$obCoUasg->commit();
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/cadCompraUasg","&usgid=".$obCoUasg->usgid);
	unset($obCoUasg);
	die;
}
if($_GET['usgidExcluir']){
	$obCoUasg = new CoUasg();
	$obCoUasg->deletaUasgContratacao( $_GET['usgidExcluir'] );
	$obCoUasg->excluir($_GET['usgidExcluir']);
	$obCoUasg->commit();
	unset($_GET['usgidExcluir']);
	unset($obCoUasg);
	$_REQUEST['acao'] = 'I';
	$db->sucesso("principal/cadCompraUasg");
	die;
}

$obCoUasg = new CoUasg($_GET['usgid']);
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
?>
<form method="post" name="form" id="form" action="/evento/evento.php?modulo=principal/cadCompraUasg&acao=A">
<input type="hidden" name="formulario" id="formulario" value="1" />
<input type="hidden" name="usgid" id="usgid" value="<? echo $obCoUasg->usgid; ?>" />
<input type="hidden" name="usgcod_old" id="usgcod_old" value="<? echo $obCoUasg->usgcod; ?>" />
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" >
					Código UASG / Entidade:
			</td>
			<td><?$usgcod = $obCoUasg->usgcod; ?>
				<?= campo_texto( 'usgcod', 'S', $permissao_formulario, 'Código da UASG', 18, 15, '###############', '','','','','id="usgcod"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Código Gestão:
			</td>
			<td><?$usggestaouasg = $obCoUasg->usggestaouasg; ?>
				<?= campo_texto( 'usggestaouasg', 'S', $permissao_formulario, 'Código Gestão', 10, 5, '#####', '','','','','id="usggestaouasg"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Descrição UASG:
			</td>
			<td><?$usgdsc = $obCoUasg->usgdsc; ?>
				<?= campo_texto( 'usgdsc', 'S', $permissao_formulario, 'Descrição da UASG', 59, 200, '', '','','','','id="usgdsc"'); ?>
			</td>
		</tr>
       <tr>
			<td class="SubTituloDireita" valign="top">Nível Contratação:</td>
			<td align="left">
			<?php
			$sql = "SELECT conid as codigo, condsc as descricao FROM evento.conivelcontratacao WHERE constatus = 'A'";                 
			
			$sqlMarcados = "SELECT 
								nc.conid as codigo,
								nc.condsc as descricao
							FROM 
								evento.couasgnivelcontratacao AS unc
							INNER JOIN evento.conivelcontratacao nc ON unc.conid = nc.conid
								WHERE unc.usgid =  '{$obCoUasg->usgid}'";
			if( $obCoUasg->usgid ){
				$conid = $db->carregar( $sqlMarcados );
			}
			
			combo_popup( "conid", $sql, "Selecione os Níveis Contratações", "192x400", 0, array(), "", "S", false, false, 5, 310 );
			?>
			<img src="../imagens/obrig.gif">
			</td>
		</tr>
	</table>
	<div id=buttonAcao>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
			<tr>
				<td class="SubTituloDireita" colspan="2" style="text-align:center">
					<input type="button" value="Salvar" onclick="enviaForm();"  /> <input type="button" value="Incluir Novo" onclick="window.location.href='evento.php?modulo=principal/cadCompraUasg&acao=I';" />
				</td>
			</tr>
	</table>
	</div>
</form>
<?php
$sql = "select
				'<center><a href=\"evento.php?modulo=principal/cadCompraUasg&acao=A&usgid='|| u.usgid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\"></a>
				<a style=\"margin: 0 -5px 0 5px;\" style=\"cursor:hand\" href=\"#\" onclick=\"excluirUasg(' || u.usgid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,	
				u.usgcod  as cod,
				u.usggestaouasg,
				u.usgdsc as unidade  
			from evento.uasg as u ";
	$cabecalho = array("Ação","Código UASG / Entidade","Código Gestão","Unidade");
	$db->monta_lista($sql,$cabecalho,200,10,'N','center','');
echo "<br />";
?>
<script type="text/javascript">
<!--
    function enviaForm(){
	 	var nomeform 		= 'form';
		var submeterForm 	= true;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		campos[0] 			= "usgdsc";
		campos[1] 			= "usgcod";
		campos[2] 			= "usggestaouasg";
						 
		tiposDeCampos[0] 	= "texto";
		tiposDeCampos[1] 	= "texto";
		tiposDeCampos[2] 	= "texto";
		
		if( !validaComboPopup() ){
			return false;
		}
		validaForm(nomeform, campos, tiposDeCampos, submeterForm );
	}
	
	function validaComboPopup(){ 
		var conid = document.getElementById('conid');
		
		selectAllOptions( conid );
		
		if( conid.value == '' ){
			alert('Você deve selecionar no mínimo um Nível de Contratação.');
			return false;
		}
		
		return true;
	} 
	
    function excluirUasg(usgid){
		if(confirm('Deseja excluir este registro')){
			window.location.href = "/evento/evento.php?modulo=principal/cadCompraUasg&acao=A&usgidExcluir="+usgid;
			return true;
		} else {
			return false;
		}
	}
	
-->
</script>