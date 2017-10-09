<?php
include APPRAIZ ."includes/workflow.php";

$odsidpai = $_GET['odsidpai'];
$tosid = $_GET['tosid'];

if ($tosid == 2){
    $tipoOrdemServico = "Estimada";
} else {
    $tipoOrdemServico = "Detalhada";
    
    if( !validaDivergencia( $odsidpai, 'OS', 'Estimada' )  )
    {
        ?>
        <script type="text/javascript" >
            alert("Não é possível criar a OS Detalhada.\nAusência de contagem estimada ou divêrgencia de contagem");
            window.close();
        </script>
        <?php
        exit;
    }
    
}

if($_POST){
	
    $dataInicial = (int) DateTimeUtil::retiraMascaraRetornandoObjetoDateTime($_POST['odsdtprevinicio'])->format('Ymd');
    $dataFinal = (int) DateTimeUtil::retiraMascaraRetornandoObjetoDateTime($_POST['odsdtprevtermino'])->format('Ymd');
    $diferenca = $dataFinal - $dataInicial;
	
    
    
    $ordemServico = new OrdemServico();
    $ordemServico = $ordemServico->recuperePorId($odsidpai);
    
    if ( $diferenca <= 2 )
    {
        $conteudo  = '<p><strong>Listagem de Ordem de Serviço</strong><p>';
		$conteudo .= '<p>Prezado(a) Preposto(a),</p>';
		$conteudo .= '<p>As OS relacionada abaixo, possue data de encerramento previsto para os próximos 2(dois) dias.</p>';
		$conteudo .= "<p>Número da SS: <strong> {$ordemServico->getIdSolicitacaoServico()} </strong></p>";
		$conteudo .= "<p>Número da OS: <strong> {$_REQUEST['odsid']} </strong></p>";
		$conteudo .= "<p>Previsão de início: <strong> {$_POST['odsdtprevinicio']} </strong></p>";
		$conteudo .= "<p>Previsão de término: <strong> {$_POST['odsdtprevtermino']} </strong></p>";
		$conteudo .= "<p>Tipo de Ordem de Serviço: <strong> {$tipoOrdemServico} </strong></p>";
		
		$assunto = SIGLA_SISTEMA. " - Fábrica - Aviso de criação de Ordem de Serviço";
		
		$remetente          = array();
		$destinatarios      = array();
		$remetente['email'] = "noreply@mec.gov.br";
		$remetente['nome']  = SIGLA_SISTEMA;
		
		$sqlPrepostoSquadra = "SELECT usu.usuemail
                    FROM seguranca.usuario usu
                    INNER JOIN seguranca.perfilusuario pu
                        ON usu.usucpf = pu.usucpf	
                    INNER JOIN seguranca.perfil per
                        ON per.pflcod = pu.pflcod
                    WHERE per.pflcod = " . PERFIL_PREPOSTO . "  
                    ORDER BY pu.pflcod;";
		
		$arrPrepostoSquadra = $db->carregar( $sqlPrepostoSquadra );
		foreach ($arrPrepostoSquadra as $destinatario){
			$destinatarios[] = $destinatario['usuemail'];
		}
		
		$destinatarios[] = $_SESSION['email_sistema'];
		
		if( enviar_email( $remetente, $destinatarios, $assunto, $conteudo ) )
        {
            $msgRetorno = 'E-mail enviado para o preposto(a) responsável';
        }
    }
}

if( !empty($_REQUEST['alerta']) )
{?>
	<script>
		alert('E-mail enviado para o preposto(a) responsável');
		window.opener.location = window.opener.location;
	</script>
<?}

if($_POST['requisicao'])
{
	$arrRes = $_POST['requisicao']($_POST);
	$msg 	= $arrRes['msg'];
	$script = $arrRes['script'];
	$odsid 	= $arrRes['odsid'];

	if (isset($msgRetorno)) {
		header("Location: fabrica.php?modulo=principal/popup/contagemPontoFuncao&acao=A&odsidpai=$odsidpai&tosid=$tosid&alerta=1");
	} else {
	 	header("Location: fabrica.php?modulo=principal/popup/contagemPontoFuncao&acao=A&odsidpai=$odsidpai&tosid=$tosid");
	}
}

if($odsid && $odsidpai){
	$sql = "select * from fabrica.ordemservico where odsid = $odsid and odsidpai = $odsidpai and tosid = $tosid";
}elseif($odsid && !$odsidpai){
	$sql = "select * from fabrica.ordemservico where odsid = $odsid and tosid = $tosid";
}elseif(!$odsid && $odsidpai){
	$sql = "select * from fabrica.ordemservico where odsidpai = $odsidpai and tosid = $tosid";
}
if($sql){
	$dadosOS = $db->pegaLinha($sql);
	if($dadosOS){
		extract($dadosOS);
	}
}
if ($_REQUEST['tosid']== 2){
    $titulo_pf = 'ESTIMADA';
    $ctrtipoempresaitem = "AND ctrtipoempresaitem = 2";
}else{
    $titulo_pf = 'DETALHADA';
    $ctrtipoempresaitem = "AND ctrtipoempresaitem = 2";
}
$titulo = "Contagem de PF ".$titulo_pf." - Nº OS ".($odsidpai ? "<span style='cursor:pointer; color: #0066CC;' onclick=window.location.reload()>".$odsidpai."</span>" : "");
monta_titulo( $titulo, "Ator: Fiscal");
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

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
<script>
	function salvarOSContagem()
	{
		if(validaCampos()){
			$("#form_contagem_pf").submit();
		}
	}
	function validaCampos()
	{
		var erro = 0;
		$("[class~=obrigatorio]").each(function() {
			if(!this.value){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		if(erro == 0){
			return true;
		}else{
			return false;
		}
	}
</script>
<form name="form_contagem_pf" id="form_contagem_pf" method="post" action="" >
	<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td valign="top" >
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                                    
                                            <tr>
                                                <td class="SubtituloDireita" id="contrato" name="contrato" >Contrato:</td>
                                                <td>
                                                        <?php
                                                            if($ctrid != 0){
                                                                $sql = "SELECT ctrid FROM fabrica.contrato WHERE ctrid= $ctrid ";
                                                                $ctrid = $db->pegaUm($sql);
                                                            }

                                                                        $sql = " SELECT distinct 
                                                                        ctr.ctrid as codigo,
                                                                        (ent.entnome ||' - ' || to_char(ctrdtinicio,'dd/MM/YYYY') || ' à ' || to_char(ctrdtfim,'dd/MM/YYYY')) as descricao
                                                                FROM
                                                                        fabrica.contrato ctr
                                                                LEFT JOIN
                                                                        fabrica.contratotiposervico cts
                                                                        on cts.ctrid=ctr.ctrid 
                                                                LEFT JOIN
                                                                        fabrica.tiposervico tps
                                                                        on tps.tpsid=cts.tpsid
                                                                INNER JOIN
                                                                        fabrica.contratosituacao cs
                                                                        on cs.ctrid=ctr.ctrid and cs.ctsstatus='A'
                                                                INNER JOIN
                                                                        fabrica.tiposituacaocontrato tsc
                                                                        on tsc.tscid=cs.tscid and tsc.tscstatus='A'
                                                               INNER JOIN entidade.entidade ent
                                                                        on ent.entid = ctr.entidcontratado and ent.entstatus = 'A'
                                                                        and ctr.ctrstatus = 'A'
                                                                        $ctrtipoempresaitem
                                                                        ORDER BY codigo asc" ;

                                                            if (empty($ctrid) && !empty($_REQUEST['ctrid'])) {
                                                                //$ctrid = $EmAndamento; 
                                                                $ctrid = $_REQUEST['ctrid'];

                                                            }       

                                                                 $db->monta_combo("ctrid",$sql,$habil,"","","","","","S","ctrid","",$ctrid); 


                                                            ?>
                                                </td>
                                            </tr>

					<tr>
						<td class="SubTituloDireita">Descrição do Serviço:</td>
						<td>
							<? echo campo_textarea( 'odsdetalhamento', 'S', "S", '', '70', '4', '1000'); ?>
							<input type="hidden" name="requisicao" value="salvarOSContagemPF">
							<input type="hidden" name="odsid" value="<?php echo $odsid ?>">
							<input type="hidden" name="odsidpai" value="<?php echo $odsidpai ?>">
							<input type="hidden" name="tosid" value="<?php echo $tosid ?>">
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Previsão de início:</td>
						<td><? echo campo_data2('odsdtprevinicio','S', "S", 'Previsão de início', 'S' ); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Previsão de Término:</td>
						<td><? echo campo_data2('odsdtprevtermino','S', "S", 'Previsão de Término', 'S' ); ?></td>
					</tr>
					<tr>
						<td colspan="2" class="TituloTabela center">
							<?php $arrEstado = $docidpf ? wf_pegarEstadoAtual($docidpf) : false; ?>
							<?php if(!$docidpf || ($arrEstado && WF_ESTADO_CPF_PENDENTE == $arrEstado['esdid'])): ?>
								<input type="button" onclick="limparPaginaUnload();salvarOSContagem()" value="Salvar" >
							<?php else: ?>
								<input type="button" disabled="disabled" onclick="alert('Operação não disponível!')" value="Salvar" >
							<?php endif; ?>
						</td>
					</tr>
				</table>
			</td>
			<? if($docidpf): ?>
			<td valign="top" width="150" >
				<?php
					$dados_wf = array(
										'odsidpai' => $odsidpai,
										'odsid' => $odsid
									 );
					wf_desenhaBarraNavegacao($docidpf, $dados_wf);
				?>
			</td>
			<? endif; ?>
		</tr>
	</table>
</form>

<script>
function limparPaginaUnload() {
	window.onbeforeunload = function (){}
}

<? if($docidpf): ?>
$(document).ready(function() {
	window.onbeforeunload = function (){
		window.opener.location = window.opener.location;
		window.close();
	}
});
<? endif; ?>
</script>