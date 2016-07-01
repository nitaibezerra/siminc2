<?

if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}

// Verificando a segurança
$permissoes = verificaPerfilSig();
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['sig_var']['orgid']);
// Fim segurança


?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/sig.js"></script>
<?

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
echo montarAbasArray(carregardadosmenusig(), "/sig/sig.php?modulo=principal/notatecnica&acao=A");

//montando cabeçalho
monta_cabecalho_sig($_SESSION['sig_var']['entid']);

$sql = "SELECT (CASE usu.usucpf WHEN '".$_SESSION['usucpf']."' 
					THEN '<center><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=principal/notatecnica&acao=A&exec_function=removernotatecnica&ntcid=' || ntc.ntcid || '\',\'Deseja realmente excluir esta nota técnica?\');\"></center>' 
					ELSE '&nbsp' 
				END) as acao0, 
				(CASE WHEN arqid IS NOT NULL 
					THEN '<center><img src=\"../imagens/pdf.gif\" width=20 height=20 border=0 title=\"Visualizar\" style=\"cursor:pointer;\"  onclick=\"window.open(\'?modulo=principal/visualizarnotatecnicapdf&acao=A&ntcid='|| ntc.ntcid ||'\',\'Nota\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\"></center>'
				 	ELSE '&nbsp' 
				 END) as acao2, 
				'<center><img src=\"../imagens/ico_html.gif\" width=20 height=20 border=0 title=\"Visualizar\" style=\"cursor:pointer;\"  onclick=\"window.open(\'?modulo=principal/visualizarnotatecnica&acao=A&ntcid='|| ntc.ntcid ||'\',\'Nota\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acao, to_char(ntc.ntcid, '9999999000') AS ntcid, to_char(ntcdatageracao,'dd/mm/yyyy HH24:MI:SS'), 
				usunome 
		FROM 
			academico.notatecnica ntc
		LEFT JOIN 
			seguranca.usuario usu ON usu.usucpf = ntc.usucpf 
		WHERE 
			ntc.entid = '". $_SESSION['sig_var']['entid'] ."' 
		ORDER BY 
			ntcdatageracao DESC";
$cabecalho = array("Ações","Nota técnica (PDF)","Nota técnica (HTML)","Número","Emitido em","Emitido por");
$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'N', '95%','N');

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td colspan="2">
		<div style="float: left;">
		&nbsp;
		<? if($permissoes['gravar']) { ?>
			<input type="button" name="nt" id="notatecnica" value="Gerar nota técnica" onclick="window.open('?modulo=principal/visualizarnotatecnica&acao=A','Nota','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=yes,location=no');"/>
		<? } ?>
		</div>
	</td>
</tr>
</table>
