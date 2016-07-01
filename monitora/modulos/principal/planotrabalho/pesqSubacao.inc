<?php

/*
 * Verifica qual página sera redirecionado
 */
include "planotrabalho/_constantes.php";
include "planotrabalho/_funcoes.php";

redirecionaUnidadePerfil('gerenciarSubacao');

exit;

/*
 * Verifica se o usuário possui o perfil 'Gestor da Unidade de Planejamento' 
 * e possui a unidade do FNDE(26298) associada.
 */
$GESTOR_FNDE = false;

$sql = "SELECT
			trim(ur.unicod) as unicod
		FROM
			monitora.usuarioresponsabilidade ur
		WHERE
			ur.usucpf = '".$_SESSION["usucpf"]."' AND
			ur.pflcod = 112 AND
			ur.rpustatus = 'A'";

$unidades = $db->carregar($sql);
if($unidades) {
	for($i=0; $i<count($unidades); $i++) {
		if($unidades[$i]["unicod"] == "26298") {
			$GESTOR_FNDE = true;
		}
	}
}
/*
 * Constantes das unidades que utilizam regras especificas para o gerador
 * Inclusão de Unidade Gestora e Unidade Responsável na geração do número
 */
define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);
define("FIES" ,74902);

/*
 * Constantes das unidades que utilizam regras especificas para o gerador
 * Inclusão de Unidade Gestora e Unidade Responsável na geração do número 
 */

// controlador do numero sequencial do gerador
include APPRAIZ."/includes/controlegeradorsequenciapi.inc";

/*
 * SCRIPT que controle as regras de combobox
 */
include APPRAIZ."/includes/controlecombobox.inc";
if(is_array($_REQUEST['dados'])) {
	$parametroconstrutor = array_merge($_REQUEST['dados'], array());
}
$combo = new ControleComboBox($parametroconstrutor);

/*
 * Constantes das opções da combobox
 */
define('COMBO_REGRAGERAL', 5);
define('COMBO_REGRAPDEEPPA', 3);
/*
 * Constantes das opções da combobox
 */


/*
 * FIM SCRIPT que controle as regras de combobox
 */

// carrega as funções de integração
include "planotrabalho/unidade_atividade_funcoes.php";
// carrega as funções do módulo pde
include "planotrabalho/_constantes.php";
include "planotrabalho/_funcoes.php";
include "planotrabalho/_componentes.php";

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( "Gerenciar Subação", "");
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Código:</td>
    <td><?
    	$sbacod = $_REQUEST['sbacod'];
    	echo campo_texto('sbacod','N','S','',6,4,'',''); 
    	?>
    </td>
</tr>

<tr>
	<td align='right' class="SubTituloDireita">Título:</td>
    <td><? 
    	$sbatitulo = $_REQUEST['sbatitulo'];
    	echo campo_texto('sbatitulo','N','S','',50,255,'',''); 
    	?>
    </td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
    <td>
    	
		<?
		$eqdid = $_REQUEST['eqdid'];
		$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 5 order by cdtcod";  
		$db->monta_combo('eqdid', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
		?>    	
    </td>
</tr>

<tr>
	<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
    <td>
		<?
		if($GESTOR_FNDE) {
			echo "F - FNDE";
			$sql = "SELECT cdtid FROM public.combodadostabela WHERE ctbid = 3 and trim(upper(cdtdsc)) = 'FNDE'";
			$exeid = $db->pegaUm($sql);
			echo "<input type=\"hidden\" id=\"exeid\" name=\"exeid\" value=\"".$exeid."\">";
		} else {
			$exeid = $_REQUEST['exeid'];
			$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 3 order by cdtcod";  
			$db->monta_combo('exeid', $sql, 'S',  'Selecione', '', '', '', '', 'N');
		}
		?>    	
    </td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
    <td>
    	
		<?
		$gstid = $_REQUEST['gstid'];
		$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 4 order by cdtcod";  
		$db->monta_combo('gstid', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
		?>    	
    </td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Permite Obras?</td>
    <td>
    	<input type="radio" name="sbaobra" value="todos" <? echo ((!$_REQUEST["sbaobra"]||$_REQUEST["sbaobra"]=="todos")?"checked":""); ?>> Todos <input type="radio" name="sbaobra" value="sim" <? echo (($_REQUEST["sbaobra"]=="sim")?"checked":""); ?>> Sim <input type="radio" name="sbaobra" value="nao" <? echo (($_REQUEST["sbaobra"]=="nao")?"checked":""); ?>> Não
    </td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Ações:</td>
    <td>
		<?
		//ver($_POST['acaid'][0], d);
		if($_POST['acaid'][0] && !$_POST['ordemlista']) {
			$acaid = $db->carregar("SELECT
										aca.acaid as codigo,
										'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
									  FROM monitora.acao aca 
									  	inner join monitora.ptres pt on pt.acaid=aca.acaid  
									WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
									and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
									and aca.acaid in (".implode(",", $_POST['acaid']).")
									GROUP BY codigo , descricao
									ORDER BY 
										1");
		}
		$sqlComboAcoes = "SELECT
							aca.acaid as codigo,
							'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
						  FROM monitora.acao aca 
						  	inner join monitora.ptres pt on pt.acaid=aca.acaid
						  WHERE aca.prgano='".$_SESSION['exercicio']."' and aca.acasnrap = false
						  	and aca.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."')
						  GROUP BY codigo , descricao
						  ORDER BY 
							1";
		combo_popup( "acaid", $sqlComboAcoes, "Ações", "600x600", 0, array(), "", "S", false, false, 5, 400 );
		?>    	
    </td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Unidades Orçamentárias / Gestoras:</td>
	<td>
	<?php
	if($_POST['entid'][0] && !$_POST['ordemlista']){
			$entid = $db->carregar("SELECT ent.entid as codigo,
								 		   CASE fen.funid
										   WHEN 34 THEN ent.entungcod ||' - '|| ent.entnome	ELSE ent.entunicod ||' - '|| ent.entnome END as descricao
									FROM entidade.entidade ent 
									LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid  
									WHERE ent.entid in (".implode(",", $_POST['entid']).")");
	}
	else{
		$entid = "";
	}
	$sqlComboEntidade = "SELECT
					   ent.entid as codigo,
					   CASE fen.funid
					   	when 34 then 
					   		entungcod ||' - '|| entnome 
					   	else 
					   		entunicod ||' - '|| entnome 
					   	end as descricao
					   FROM entidade.entidade ent 
					   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
					   WHERE entunicod IN('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."') and fen.funid <> 33 and ent.entid not in(391293,391294) ORDER BY entunicod";

	combo_popup( "entid", $sqlComboEntidade, "Unidades Orçamentárias", "600x600", 0, array(), "", "S", false, false, 5, 400 );
	?>
	</td>
</tr>
<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td>
	  	  <input type="button" class="botao" name="bta" value="Pesquisar" onclick="submeter();">
	  	  <input type="button" class="botao" name="bt" value="Nova Subação" onclick="window.location='?modulo=principal/planotrabalho/programainstitucional&acao=A';"></td>
	  	  
	      </tr>
</table>
</form>
<?
if($_POST){
	$where = " WHERE sba.sbastatus = 'A' ";
	$where .= $_REQUEST['sbatitulo'] ? " AND sba.sbatitulo ilike '%".$_REQUEST['sbatitulo']."%' " : '';
	$where .= $_REQUEST['sbacod'] ? " AND upper(sba.sbacod) like '%".strtoupper($_REQUEST['sbacod'])."%' " : '';
	$where .= $_REQUEST['eqdid'] ? " AND sba.eqdid = ".$_REQUEST['eqdid']." " : '';
	$where .= $_REQUEST['exeid'] ? " AND sba.exeid = ".$_REQUEST['exeid']." " : '';
	$where .= $_REQUEST['gstid'] ? " AND sba.gstid = ".$_REQUEST['gstid']." " : '';
	
	if($_REQUEST['acaid'][0]) {
		$acoes = implode(",", $_REQUEST['acaid'] );
		$where .= " AND ptr.acaid in (".$acoes.") ";
	}
	
	$where .= ($_REQUEST['sbaobra']!="todos" && $_REQUEST['sbaobra']!="") ? " AND sba.sbaobra = ".(($_REQUEST['sbaobra']=="sim")?"true":"false")." " : '';
	if($_POST['entid'][0] && !$_POST['ordemlista']) {
		$where .= " AND pe.entid in (".implode(",", $_POST['entid']).")";
	}
	$sql = "SELECT '<center><a style=\"cursor:pointer;\" onclick=\"alterarsba(\''||sba.sbaid||'\');\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\"></a><a  style=\"cursor:pointer;\" onclick=\"removersba(\''||sba.sbaid||'\');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a></center>' as acao, 
				sba.sbacod,
				sba.sbatitulo,
				cdt.cdtcod ||' - '||cdt.cdtdsc as enquadramento,
				c.cdtcod ||' - '||c.cdtdsc as executor,
				cd.cdtcod ||' - '||cd.cdtdsc as gestor,
				u.usunome ||' - '||to_char(sba.sbadata, 'dd/mm/YYYY hh24:mi') as resp
				FROM monitora.subacao sba
				LEFT JOIN seguranca.usuario u ON u.usucpf = sba.usucpf
				LEFT JOIN public.combodadostabela c ON c.cdtid = sba.exeid
				LEFT JOIN public.combodadostabela cd ON cd.cdtid = sba.gstid
				LEFT JOIN public.combodadostabela cdt ON cdt.cdtid = sba.eqdid
				LEFT JOIN monitora.subacaounidade pe ON pe.sbaid = sba.sbaid
				LEFT JOIN monitora.subacaodotacao sdt ON sdt.sbaid = sba.sbaid
				LEFT JOIN monitora.ptres ptr ON ptr.ptrid = sdt.ptrid
				{$where}
				GROUP BY sba.sbaid,sba.sbacod,sba.sbatitulo,enquadramento,executor,gestor,resp
				ORDER BY sba.sbacod DESC";
	$cabecalho = array("","Código","Título","Enquadramento","Executor Orçamentário","Gestor da Subação","Atualizado por / Data");
	
	$db->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2);
}
?>
<script type="text/javascript">
function submeter(){
	if(document.formulario.acaid.options[0].value != "")
		selectAllOptions( document.formulario.acaid );
	if(document.formulario.entid.options[0].value != "")
 		selectAllOptions( document.formulario.entid );

	document.formulario.submit();	
}
function removersba(sbaid){
	var conf = confirm("Você realmente deseja excluir esta Subação?");	
	if(conf) {
		location.href="monitora.php?modulo=principal/planotrabalho/programainstitucional&acao=A&evento=E&sbaid="+sbaid;	
	}
}
function alterarsba(sbaid){
	location.href="monitora.php?modulo=principal/planotrabalho/programainstitucional&acao=A&evento=A&sbaid="+sbaid;
}
function abrir_lista() {
	janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalho/listarProgramaSubAcao&acao=A', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-120)+',height=680' ); janela.focus();
}
</script>