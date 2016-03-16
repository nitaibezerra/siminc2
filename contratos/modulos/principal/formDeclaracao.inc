<?php
if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}

verificaSessao(true);

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
montaCabecalhoUnidade($_SESSION['unidade']); 
montaCabecalhoProcesso($_SESSION['copid'], false);

/*(CASE WHEN arqid IS NOT NULL 
					THEN '<center><img src=\"../imagens/pdf.gif\" width=20 height=20 border=0 title=\"Visualizar\" style=\"cursor:pointer;\"  onclick=\"window.open(\'?modulo=principal/visualizarnotatecnicapdf&acao=A&decid='|| dec.decid ||'\',\'Declaração\',\'toolbar=no,location=no,status=no,menubar=no,scrollbars=yes,resizable=no,width=800,height=600\');\"></center>'
				 	ELSE '&nbsp' 
				 END) as acao2,*/
$boAlterar = true;

$arPerfisGeraDeclaracao = array(EVENTO_PERFIL_SUPER_USUARIO,
					 	  	    EVENTO_PERFIL_ORDENADOR_DESPESA_COMPRAS
						  		);
$perfilGeraDeclaracao = possuiPerfil($arPerfisGeraDeclaracao);

$cdiid = $coaid = "";
$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade'], false);
if($coaid){
	$sql = "SELECT cdiid FROM evento.codemandaitem WHERE coaid = $coaid ";
	$cdiid = $db->pegaUm($sql);
	$boAlterar = permissaoAlterar($coaid);
}

if($boAlterar || !$boAlterar && $perfilGeraDeclaracao){
	$linhaExcluir = "<center><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirDeclaracao(\'?modulo=principal/formDeclaracao&acao=A&exec_function=removerdeclaracao&decid=' || dec.decid || '\',\'Deseja realmente excluir esta declaração?\');\"></center>";	
} else {
	$linhaExcluir = "<center><img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\"></center>";
}

$sql = "SELECT (CASE usu.usucpf WHEN '".$_SESSION['usucpf']."' 
					THEN '$linhaExcluir'
					ELSE '<center><img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\"></center>'
				END) as acao0, 
				'<center><img src=\"../imagens/ico_html.gif\" width=20 height=20 border=0 title=\"Visualizar\" style=\"cursor:pointer;\"  onclick=\"window.open(\'?modulo=principal/visualizarDeclaracao&acao=A&decid='|| dec.decid ||'\',\'Declaração\',\'toolbar=no,location=no,status=no,menubar=no,scrollbars=yes,resizable=no,width=800,height=600\');\"></center>' as acao, to_char(dec.decid, '9999999000') AS decid, to_char(dec.decdatageracao,'dd/mm/yyyy HH24:MI:SS'), 
				usu.usunome 
		FROM 
			evento.declaracao dec
		LEFT JOIN 
			seguranca.usuario usu ON usu.usucpf = dec.usucpf 
		WHERE 
			dec.usgid = '". $_SESSION['unidade']."'
		AND dec.decstatus = 'A'
		AND dec.copid = '". $_SESSION['copid'] ."'
		ORDER BY 
			dec.decdatageracao DESC";

$cabecalho = array("Ações","Declaração (HTML)","Número","Emitido em","Emitido por");
$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'N', '95%','N');

if($cdiid && $boAlterar || !$boAlterar && $perfilGeraDeclaracao){
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td colspan="2">
		<div style="float: left;">
		&nbsp;
		<input type="button" name="declaracao" id="declaracao" value="Gerar declaração" onclick="window.open('evento.php?modulo=principal/visualizarDeclaracao&acao=A','','toolbar=no,location=no,status=no,menubar=no,scrollbars=yes,resizable=no,width=800,height=600');"/>
		</div>
	</td>
</tr>
<?php
}
?>
</table>
<script type="text/javascript">
function excluirDeclaracao(url, msg){
	if(confirm(msg)){
		window.location.href = url;
		return true;
	} else {
		return false;
	}
}
</script>