<?php 
include APPRAIZ ."includes/workflow.php";

if($_REQUEST['requisicao']) {
	$arrRest = $_REQUEST['requisicao']($_REQUEST);
	if($arrRest['msg']){
		echo "<script>alert('{$arrRest['msg']}')</script>";
		echo "<script>window.location.href = 'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}'</script>";
		exit;
	}else{
		header("Location: fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}");
	}
}

if($_REQUEST['download'] == 'S'){
	$arqid = $_REQUEST['arqid'];
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    exit;
}

$sql = "SELECT os.docid, 
			   os.tosid,
			   os.docidpf,
			   scs.scsid,
			   ans.ansid,
			   odsdetalhamento,
			   odssubtotalpf,
			   odsqtdpfdetalhada,
			   odsqtdpfestimada,
			   odsid, 
			   to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio, to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino 
		FROM fabrica.ordemservico os
		inner join fabrica.solicitacaoservico scs ON scs.scsid = os.scsid 
		inner join fabrica.analisesolicitacao ans ON ans.scsid = scs.scsid
		WHERE odsid = {$_REQUEST['odsid']} ORDER BY odsid";

// buscando dados da OS
$oss = $db->pegaLinha($sql);

$arrEstado = wf_pegarEstadoAtual( $oss['docidpf']);
if($arrEstado['esdid'] != WF_ESTADO_CPF_AGUARDANDO_CONTAGEM){
	$permissaoCampos = false;
        $ator = "Empresa Item 2";
}else{
	$permissaoCampos = true;
        $ator = "Fiscal Técnico";
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

//abas
$menu[0] = array("descricao" => "Listar OS", "link"=> "fabrica.php?modulo=principal/listarOrdemServicoCon&acao=A");
$menu[1] = array("descricao" => "Dados OS de Origem", "link"=> "fabrica.php?modulo=principal/contagemOSCon&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}");
$menu[2] = array("descricao" => "Contagem P.F.", "link"=> "fabrica.php?modulo=principal/cadContagemOSCon&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}");
echo montarAbasArray($menu, "fabrica.php?modulo=principal/cadContagemOSCon&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}");


$titulo = "Nº OS {$oss['odsid']}";
monta_titulo( $titulo, "Gestor do Contrato / Fiscal Técnico");

$habil = 'N';
$permissaoCampos = false;
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<script type="text/javascript">
function salvarOSPF()
{
	if(validaFormulario()){

            if(document.getElementById('odssubtotalpf').value == ''){
                    alert("Preencha o campo Subtotal de PF.");
                    document.getElementById('odssubtotalpf').focus();
                    return false;
            }


            <?php if($oss['tosid'] == TIPO_OS_CONTAGEM_DETALHADA): ?>
                    if(document.getElementById('odsqtdpfdetalhada').value == ''){
                            alert("Preencha o campo Qtde P.F. Detalhada.");
                            document.getElementById('odsqtdpfdetalhada').focus();
                            return false;
                    }
            <?php else: ?>
                    if(document.getElementById('odsqtdpfestimada').value == ''){
                            alert("Preencha o campo Qtde P.F. Estimada.");
                            document.getElementById('odsqtdpfestimada').focus();
                            return false;
                    }
            <?php endif; ?>

            $('#form_cad_contagem').submit();
            
	}
}

function validaFormulario()
{
	if($('[name=arquivo]').val()){
		if($('[name=taoid]').val() && $('[name=aosdsc_]').val()){
			return true;
		}else{
			alert('Favor informar todos os dados para anexar o arquivo!');
			return false;
		}
	}else{
		return true;
	}
}

function excluirAnexo(arqid)
{
	if(confirm("Deseja realmente excluir o anexo?")){
		$('[name=requisicao]').val("excluirAnexo");
		$('[name=arqid]').val(arqid);
		if($('[name=requisicao]').val() == "excluirAnexo" && $('[name=arqid]').val()){
			$('#form_cad_contagem').submit();
		}
	}
}
</script>
<form name="form_cad_contagem" id="form_cad_contagem" method="post" action="" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" value="salvarContagemPFOS" />
	<input type="hidden" name="odsid" value="<?php echo $_GET['odsid'] ?>" />
	<input type="hidden" name="arqid" value="" />
	<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td valign="top" >
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td width="25%" class="SubTituloDireita">Número:</td>
						<td><? echo $oss['odsid']; ?></td>
					</tr>
					<tr>
					<td class="SubTituloDireita">Disciplinas e Produtos:</td>
					<td>
					<?php $arrDisciplina = carregarDisciplinasProdutosPorAnalise($oss['ansid']); ?>
						<?php if(is_array($arrDisciplina)): ?>
							<?php foreach($arrDisciplina as $disc): ?>
									<?php
									$arrContratada[$disc['disciplina']]['dpedsc'] = $disc['disciplina'];
									$arrContratada[$disc['disciplina']]['produtos'][] = $disc['produto'];
									?>
							<?php endforeach; ?>
						<?php endif; ?>
						<?php if(is_array($arrContratada)): ?>
							<?php $n=1; ?>
							<?php foreach($arrContratada as $d): ?>
									<b><?php echo $n.") ".$d['dpedsc'] ?></b><?php echo $d['produtos'][0] ? ": ".implode(", ",$d['produtos']).";" : ";"  ?><br />
									<?php $n++; ?>
							<?php endforeach; ?>
						<?php else: ?>
							N/A
						<?php endif; ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Previsão Início:</td>
						<td><? echo $oss['odsdtprevinicio']; ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Previsão Término:</td>
						<td><? echo $oss['odsdtprevtermino']; ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Detalhamento:</td>
						<td><?  echo $oss['odsdetalhamento']; ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Subtotal de PF:</td>
						<td>
							<?php if($oss['odssubtotalpf']) $odssubtotalpf = number_format($oss['odssubtotalpf'],2,",",".");?>
							<input type="hidden" name="odssubtotalpf_" value="<? echo $odssubtotalpf; ?>" />
							<? echo campo_texto('odssubtotalpf', 'S', $habil, 'Subtotal de PF', 15, 14, '###.###.###,##', '', '', '', 0, 'id="odssubtotalpf"' ); ?>
						</td>
					</tr>
					<?php if($oss['tosid'] == TIPO_OS_CONTAGEM_DETALHADA): ?>
						<tr>
							<td class="SubTituloDireita">Qtde P.F. Detalhada:</td>
							<td>
								<?php if($oss['odsqtdpfdetalhada']) $odsqtdpfdetalhada = number_format($oss['odsqtdpfdetalhada'],2,",",".");?>
								<input type="hidden" name="odsqtdpfdetalhada_" value="<? echo $odsqtdpfdetalhada; ?>" />
								<? echo campo_texto('odsqtdpfdetalhada', 'S', ($permissaoCampos ? "S" : "N") , 'Qtde P.F. Detalhada', 15, 14, '###.###.###,##', '', '', '', 0, 'id="odsqtdpfdetalhada"' ); ?>
							</td>
						</tr>
					<?php else: ?>
						<tr>
							<td class="SubTituloDireita">Qtde P.F. Estimada:</td>
							<td>
								<?php if($oss['odsqtdpfestimada']) $odsqtdpfestimada = number_format($oss['odsqtdpfestimada'],2,",",".");?>
								<input type="hidden" name="odsqtdpfestimada_" value="<? echo $odsqtdpfestimada; ?>" />
								<? echo campo_texto('odsqtdpfestimada', 'S', ($permissaoCampos ? "S" : "N") , 'Qtd. de P.F. Estimado', 15, 14, '###.###.###,##', '', '', '', 0, 'id="odsqtdpfestimada"' ); ?>
							</td>
						</tr>
					<?php endif; ?>
					<?php if($permissaoCampos): ?>
						<tr>
							<td class="TituloTabela center" colspan="2">Anexar arquivos</td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Arquivo:</td>
							<td><input type="file" name="arquivo" id="arquivo"></td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Tipo de anexo:</td>
							<td>
								<? if($oss['tosid'] == TIPO_OS_CONTAGEM_DETALHADA) {
									echo "Relatório de Contagem de PF Detalhada";
									echo "<input type='hidden' name='taoid' value='29'";
								}
								elseif($oss['tosid'] == TIPO_OS_CONTAGEM_ESTIMADA){
									echo "Relatório de Contagem de PF Estimada";
									echo "<input type='hidden' name='taoid' value='28'";
								}
								else{
									$sql = "SELECT taoid as codigo, taodsc as descricao FROM fabrica.tipoanexoordem WHERE taostatus='A' order by 2";
									$db->monta_combo('taoid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'taoid');
								}
								?>
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Descrição:</td>
							<td><? echo campo_texto('aosdsc_', 'S', 'S', 'Descrição', 50, 255, '', '', '', '', 0, 'id="aosdsc_"' ); ?></td>
						</tr>
						<tr>
							<td colspan="2" class="SubTituloDireita center">
								<input type="button" name="btn_salvar" value="Salvar" onclick="salvarOSPF()" />
								&nbsp;
								<input type="button" name="btn_solicitacao" value="Voltar" onclick="history.back();" />
							</td>
						</tr>
					<?php endif; ?>
					<tr>
						<td colspan="2" class="TituloTabela center">Anexos</td>
					</tr>
					<tr>
						<td colspan="2">
						<?php 
						$sql = "SELECT
									".($permissaoCampos ? "'<center><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer\" title=\"Excluir Anexo\" onclick=\"excluirAnexo(\'' || an.arqid || '\')\" /></center>' as acao," : "")." 
									to_char(an.aosdtinclusao,'dd/mm/YYYY'), tp.taodsc, 
									'<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'{$_SERVER['REQUEST_URI']}&download=S&arqid=' || ar.arqid || '\';\" />' || ar.arqnome||'.'||ar.arqextensao ||'</a>',
									ar.arqtamanho, an.aosdsc 
								FROM fabrica.anexoordemservico an 
								LEFT JOIN fabrica.tipoanexoordem tp ON an.taoid=tp.taoid 
								LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid 
								WHERE odsid='".$oss['odsid']."' AND aosstatus='A'";
					
						if($permissaoCampos){
							$cabecalho = array("Ação","Data inclusão", "Tipo Arquivo", "Nome arquivo", "Tamanho(bytes)", "Descrição");	
						}else{
							$cabecalho = array("Data inclusão", "Tipo Arquivo", "Nome arquivo", "Tamanho(bytes)", "Descrição");
						}
						$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
						?>
						</td>
					</tr>
				</table>
			</td>
			<td valign="top" width="150" >
				<?php
					$dados_wf = array(
										'odsid' => $oss['odsid'],
										'tosid' => $oss['tosid']
									 );
					wf_desenhaBarraNavegacao($oss['docidpf'], $dados_wf);
				?>
			</td>
		</tr>
	</table>
</form>
<script>
<?php if($arrRest['msg']): ?>
	alert('<?php echo $arrRest['msg'] ?>');
<?php endif; ?>
</script>