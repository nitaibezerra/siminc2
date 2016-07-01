<?
include APPRAIZ ."includes/workflow.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//permissao para edição da tela
$habil = 'S';
if(!verificaPermissaoEdicao()) $habil = 'N';

// resgatando o estado da solicitação
$estado_wf = wf_pegarEstadoAtual($db->pegaUm("SELECT docid FROM fabrica.solicitacaoservico WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'"));

// se o estado for finalizada desabilita campos
if($estado_wf['esdid'] == WF_ESTADO_FINALIZADA) {
	 $habil = 'N';
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

switch($_REQUEST['acao']) {
	case 'A':
		$menu = carregarMenuDetalhamentoSolicitacao();
		break;
	case 'C':
		$menu = carregarMenuAvaliacaoSolicitacao();
		break;
	case 'D':
		$menu = carregarMenuExecucaoSolicitacao();
		break;
}

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

$titulo_modulo = "Anexos da Ordem de Serviço";
monta_titulo($titulo_modulo, '');

?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>

<script language="javascript" type="text/javascript">

function submeterAnexoSolicitacaoServico() {


	if(document.getElementById('odsid').value == '') {
		alert('Selecione a OS');
		return false;
	}

	if(document.getElementById('arquivo').value.length == 0) {
		alert('Selecione um arquivo');
		return false;
	}


	if(document.getElementById('taoid').value == '') {
		alert('Selecione o tipo de arquivo');
		return false;
	}

	if(document.getElementById('aosdsc_').value.length == 0) {
		alert('Preencha a descrição');
		return false;
	}

	divCarregando();
	document.getElementById('formulario').submit();
}

</script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirAnexoDetalhamento">
<input type="hidden" name="redirecionamento" value="fabrica.php?modulo=principal/cadDetalhamentoAnexos&acao=<? echo $_REQUEST['acao']; ?>">
<input type="hidden" name="scsid" value="<? echo $_SESSION['fabrica_var']['scsid']; ?>">
<?

//telaCabecalhoSolicitacaoServico(array("scsid" => $_SESSION['fabrica_var']['scsid']));
//telaCabecalhoAnaliseSolicitacaoServico(array("ansid" => $_SESSION['fabrica_var']['ansid']));

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td width="100%" valign="top">
	<table class="listagem" width="100%">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Anexar arquivos na solicitação de serviço</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">OS:</td>
		<td>
		<?
		$sql = "SELECT odsid as codigo, 'Nº OS '||odsid as descricao FROM fabrica.ordemservico WHERE scsid='".$_SESSION['fabrica_var']['scsid']."' order by 1";
		$db->monta_combo('odsid', $sql, $habil, 'Selecione', '', '', '', '', 'S', 'odsid');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Arquivo:</td>
		<td><input type="file" name="arquivo" id="arquivo" <?php if($habil == 'N') echo 'disabled';?>></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de anexo:</td>
		<td><?  if($estado_wf['esdid'] == WF_ESTADO_EXECUCAO){
				$sql = "SELECT taoid as codigo,
							taodsc as descricao
						FROM
							fabrica.tipoanexoordem
						WHERE
							taoid not in(28)
						AND
							taostatus='A' order by 2";
				}else if($estado_wf['esdid'] == WF_ESTADO_DETALHAMENTO){
					$sql = "SELECT taoid as codigo,
								taodsc as descricao
							FROM
								fabrica.tipoanexoordem
							WHERE
								taoid not in(29)
							AND
								taostatus='A' order by 2";
				}else{
					$sql = "SELECT taoid as codigo, taodsc as descricao FROM fabrica.tipoanexoordem WHERE taostatus='A' order by 2";
				}
				$db->monta_combo('taoid', $sql, $habil, 'Selecione', '', '', '', '', 'S', 'taoid');
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição:</td>
		<td><? echo campo_texto('aosdsc_', 'S', $habil, 'Descrição', 50, 255, '', '', '', '', 0, 'id="aosdsc_"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda">&nbsp;</td>
		<td class="SubTituloEsquerda">
		<?php if($habil=='S'){?>
			<input type="button" name="inseriranalise" value="Enviar" onclick="submeterAnexoSolicitacaoServico();">
		<?php }?>

		</td>
	</tr>

	</table>
	
	</td>
</tr>
</table>
</form>

<?

    $pfls = arrayPerfil();

    $wh = "";
    //perfis
    if( in_array(PERFIL_PREPOSTO,$pfls) || in_array(PERFIL_ESPECIALISTA_SQUADRA,$pfls) ){
            $wh = " AND fa.tosid = ".TIPO_OS_GERAL."";
    }

    $btnExcluir = "<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"Excluir(\'fabrica.php?modulo=principal/cadDetalhamentoAnexos&acao=A&requisicao=removerAnexoDetalhamento&aosid='||an.aosid||'\',\'Deseja realmente excluir o anexo?\');\">";
    if($habil == 'N') $btnExcluir = "";

    $sql = "SELECT '<div align=center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=\'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&requisicao=downloadAnexoDetalhamentoServico&arqid='|| ar.arqid ||'\';\"> $btnExcluir</div>' as acoes,
                '<p title=\"Editar O.S.\" style=\"cursor:pointer; color: #0066CC;\" onclick=\"window.open(\'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='||fa.odsid||'\',\'Observacoes\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">'||fa.odsid||'</p>' as os,
                                    '<div align=center>'||to_char(an.aosdtinclusao,'dd/mm/YYYY HH24:MI') ||'</div>' as dataos,
                                    tp.taodsc,
                                    ar.arqnome||'.'||ar.arqextensao as nomearquivo,
                                    '<div align=center>'||ar.arqtamanho||'</div>' as tam,
                                    an.aosdsc
                    FROM fabrica.anexoordemservico an
                    LEFT JOIN fabrica.tipoanexoordem tp ON an.taoid=tp.taoid
                    LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid
                    LEFT JOIN fabrica.ordemservico fa ON fa.odsid=an.odsid
                    WHERE scsid='".$_SESSION['fabrica_var']['scsid']."' ".$wh." AND aosstatus='A'
                    ORDER BY fa.odsid DESC ,an.aosdtinclusao DESC";

    $cabecalho = array("Ações", "Nº OS", "Data inclusão", "Tipo Arquivo", "Nome arquivo", "Tamanho(bytes)", "Descrição");
    $tamanho       = array("5%","5%","5%","30%", "10%", "10%", "5%", "5%", "10%", "5%");
    $alinhamento   = array("center","center","center","left", "center", "center", "center", "center", "center", "center");

    //$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
    $db->monta_lista($sql,$cabecalho,50,5,'N','center',"","",$tamanho,$alinhamento);
?>
