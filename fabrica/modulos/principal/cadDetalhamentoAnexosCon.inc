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
		$menu = array(0 => array("id" => 1, "descricao" => "Detalhar Solicitação",		 "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoCon&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 				 "link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacaoCon&acao=A&tipoobs=cadDetalhamento"),
				  2 => array("id" => 3, "descricao" => "Anexos da Solicitação",		 "link" => "/fabrica/fabrica.php?modulo=principal/analiseDemandaAnexosCon&acao=C"),
				  3 => array("id" => 4, "descricao" => "Anexos da Ordem de Serviço", "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexosCon&acao=A"),
				  4 => array("id" => 5, "descricao" => "Providências", 		 		 "link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=cadDetalhamento")
			  	  );
		break;
	case 'C':
		$menu = array(0 => array("id" => 1, "descricao" => "Avaliar / Aprovar Solicitação",      "link" => "/fabrica/fabrica.php?modulo=principal/cadAvaliacaoAprovacaoCon&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 						 "link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacao&acao=A&tipoobs=cadAvaliacaoAprovacao"),
				  2 => array("id" => 3, "descricao" => "Anexos da ordem de serviço",         "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexosCon&acao=C"),
				  3 => array("id" => 4, "descricao" => "Providências", 		 		 		 "link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=cadAvaliacaoAprovacao")
			  	  );
		break;
	case 'D':
		$menu = array(0 => array("id" => 1, "descricao" => "Executar Solicitação",      		 "link" => "/fabrica/fabrica.php?modulo=principal/cadExecucaoCon&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 						 "link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacaoCon&acao=A&tipoobs=cadExecucao"),
				  2 => array("id" => 3, "descricao" => "Anexos da ordem de serviço",         "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexosCon&acao=D"),
				  3 => array("id" => 4, "descricao" => "Monitoramento / Controle de riscos", "link" => "/fabrica/fabrica.php?modulo=principal/monitoramentoRiscosCon&acao=C"),
				  4 => array("id" => 5, "descricao" => "Providências", 		 		 	 	 "link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=cadExecucao")
			  	  );
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
<input type="hidden" name="redirecionamento" value="fabrica.php?modulo=principal/cadDetalhamentoAnexosCon&acao=<? echo $_REQUEST['acao']; ?>">
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
		<td class="SubTituloEsquerda" colspan="2">Arquivos na solicitação de serviço</td>
	</tr>
	<tr style="display: none">
		<td class="SubTituloDireita">OS:</td>
		<td>
		<?
		$sql = "SELECT odsid as codigo, 'Nº OS '||odsid as descricao FROM fabrica.ordemservico WHERE scsid='".$_SESSION['fabrica_var']['scsid']."' order by 1";
		$db->monta_combo('odsid', $sql, $habil, 'Selecione', '', '', '', '', 'S', 'odsid');
		?>
		</td>
	</tr>
	<tr style="display: none">
		<td class="SubTituloDireita">Arquivo:</td>
		<td><input type="file" name="arquivo" id="arquivo" <?php if($habil == 'N') echo 'disabled';?>></td>
	</tr>
	<tr style="display: none">
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
	<tr style="display: none">
		<td class="SubTituloDireita">Descrição:</td>
		<td><? echo campo_texto('aosdsc_', 'S', $habil, 'Descrição', 50, 255, '', '', '', '', 0, 'id="aosdsc_"' ); ?></td>
	</tr>
	<tr style="display: none">
		<td class="SubTituloEsquerda">&nbsp;</td>
		<td class="SubTituloEsquerda">
		<?php if($habil=='S'){?>
			<input type="button" name="inseriranalise" value="Enviar" onclick="submeterAnexoSolicitacaoServico();">	
		<?php }?>
		
		</td>
	</tr>

	</table>
	<?

	$pfls = arrayPerfil();

	$wh = "";
	//perfis
	if( in_array(PERFIL_PREPOSTO,$pfls) || in_array(PERFIL_ESPECIALISTA_SQUADRA,$pfls) ){
		$wh = " AND fa.tosid = ".TIPO_OS_GERAL."";
	}

	$btnExcluir = "<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"Excluir(\'fabrica.php?modulo=principal/cadDetalhamentoAnexos&acao=A&requisicao=removerAnexoDetalhamento&aosid='||an.aosid||'\',\'Deseja realmente excluir o anexo?\');\">";
	if($habil == 'N') $btnExcluir = "";
	$btnExcluir = "";
	
	$sql = "SELECT '<div align=center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=\'fabrica.php?modulo=principal/cadDetalhamentoCon&acao=A&requisicao=downloadAnexoDetalhamentoServico&arqid='|| ar.arqid ||'\';\"> $btnExcluir</div>' as acoes, 
					'<div align=center>'||fa.odsid ||'</div>' as os,
					'<div align=center>'||to_char(an.aosdtinclusao,'dd/mm/YYYY') ||'</div>' as dataos, 
					tp.taodsc, 
					ar.arqnome||'.'||ar.arqextensao as nomearquivo,
					'<div align=center>'||ar.arqtamanho||'</div>' as tam, 
					an.aosdsc 
			FROM fabrica.anexoordemservico an 
			LEFT JOIN fabrica.tipoanexoordem tp ON an.taoid=tp.taoid 
			LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid 
			LEFT JOIN fabrica.ordemservico fa ON fa.odsid=an.odsid
			WHERE scsid='".$_SESSION['fabrica_var']['scsid']."' ".$wh." AND aosstatus='A'";

	$cabecalho = array("Ações", "Nº OS", "Data inclusão", "Tipo Arquivo", "Nome arquivo", "Tamanho(bytes)", "Descrição");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	?>
	</td>
</tr>
</table>
</form>
