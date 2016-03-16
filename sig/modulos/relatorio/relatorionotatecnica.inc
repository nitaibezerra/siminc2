<?
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
echo "<br/>";
$titulo_modulo = "Sistema de Informações Gerenciais";
monta_titulo( $titulo_modulo,'Relatório Nota Técnica');
?>

<script>

function abrepopupUnidade(){
	janela = window.open('combo_unidades.php','Unidades','width=400,height=400,toolbar=yes,status=yes,scrollbars=1,top=200,left=480');
	janela.focus();
}
function exibirRelatorio() {
	var formulario = document.formulario;
	//Filtros
	selectAllOptions(document.formulario.estuf);
	selectAllOptions(document.formulario.unidades);
	
	return true;	
}

</script>

<form method="post" name="formulario">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
	<tr><td colspan="2">Filtros</td></tr>
	<tr><td class="SubTituloDireita" valign="top">Unidades da Federação</td>
		<td>
		<?
		$sqlComboEstado = "select estuf as codigo, estdescricao as descricao
						   from territorios.estado
						   order by	estdescricao";
		$estuf = array(0 => array("codigo" => "", "descricao" => "Todas"));
		
		combo_popup("estuf", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400);
		?>
		</td>
	</tr>
    <tr>
        <td class="SubTituloDireita" valign="top">Unidades</td>
        <td>
        <select multiple="multiple" size="5" name="unidades[]" id="unidades" ondblclick="abrepopupUnidade();" class="CampoEstilo" style="width:400px;">
        <option value="">Todas</option>
        </select>
        </td>
   </tr>
   <tr bgcolor="#C0C0C0">
   		<td>&nbsp;</td>
		<td><input type="submit" value="Listar" onclick="exibirRelatorio();"><input type="hidden" name="exibir" value="relatorio"></td>
   </tr>

<?
if($_POST['exibir'] == "relatorio") { 
?>
    </tr><td colspan="2">
<?
	// Analisando o filtro geral
	if($_POST['estuf'][0]) {
		$filtro['geral'][] = "ende.estuf IN('".implode("','",$_POST['estuf'])."')"; 
	}
	if($_POST['unidades'][0]) {
		foreach($_POST['unidades'] as $unidade) {
			$detalhesunidade = $unidade;
			$filuni[] = "(ent.entidassociado = '".$detalhesunidade."' OR ent.entid='".$detalhesunidade."')";
		}
		$filtro['geral'][] = "(". implode(" OR ", $filuni) .")";
	}
	$sql = "SELECT '<center><img src=\"../imagens/pdf.gif\" width=20 height=20 border=0 title=\"Visualizar\" style=\"cursor:pointer;\"  onclick=\"window.open(\'?modulo=principal/visualizarnotatecnicapdf&acao=A&ntcid='|| ntc.ntcid ||'\',\'Nota\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acao2, '<center><img src=\"../imagens/ico_html.gif\" width=20 height=20 border=0 title=\"Visualizar\" style=\"cursor:pointer;\"  onclick=\"window.open(\'?modulo=principal/visualizarnotatecnica&acao=A&ntcid='|| ntc.ntcid ||'\',\'Nota\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acao, to_char(ntc.ntcid, '9999999000') AS ntcid, to_char(ntcdatageracao,'dd/mm/yyyy HH24:MI:SS'), usunome, entnome 
			FROM academico.notatecnica ntc
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = ntc.usucpf 
			LEFT JOIN entidade.entidade ent ON ent.entid = ntc.entid 
			LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid  
			WHERE ntcstatus = 'A'".(($filtro['geral'])?" AND (".implode(" OR ",$filtro['geral']).")":"")." ORDER BY ntcdatageracao DESC";
	$cabecalho = array("Nota técnica (PDF)","Nota técnica (HTML)","Número","Emitido em","Emitido por","Entidade");
	$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'N', '100%','N');
?>	</td></tr> <?
}
?>
</table>
</form>