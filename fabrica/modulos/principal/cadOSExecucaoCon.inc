<?
include APPRAIZ ."includes/workflow.php";

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

require_once APPRAIZ . "fabrica/classes/HistoricoPausaServico.class.inc";
$oHistoricoPausaServico = new HistoricoPausaServico();

// Variável de requisição para executar as funções solicitadas
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

// validando o ID da ordem de serviço
if(!$_REQUEST['odsid']) {
	die("<script>alert('OS não reconhecida');window.close();</script>");
}

$sql = "    SELECT wkd.esdid
                FROM fabrica.ordemservico as fos

                    inner join  workflow.documento as wkd
                        on wkd.docid = fos.docid
                    inner join  workflow.estadodocumento as wed
                        on wed.esdid = wkd.esdid

            WHERE fos.odsid = {$_REQUEST["odsid"]}";

$SituacaoOrdem = $db->pegaLinha($sql);
$SituacaoAtual = $SituacaoOrdem["esdid"];

// se passar o ID da analise, inserir na sessão
if($_REQUEST['ansid']) {
	$_SESSION['fabrica_var']['ansid'] = $_REQUEST['ansid'];
}

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    exit;
}



function executaAcaoProf($dados){
	global $db;

	//print_r($dados);
	$odsid = $dados['odsid'];
	$acao = $dados['acao'];
	$usucpf = $dados['usucpf'];

	if($acao == 'I'){

		$sql = "INSERT INTO fabrica.profissionalos
					(
						odsid,
						usucpf
					)VALUES(
						".$odsid.",
						'".$usucpf."'
					)";

		$db->executar($sql);
		$db->commit();

		//recupera docid
		$sql = "SELECT docid FROM fabrica.ordemservico WHERE odsid=".$odsid;
		$docid = $db->pegaUm($sql);

		if($docid){
			$estadoOS = wf_pegarEstadoAtual($docid);
			switch($estadoOS['esdid']) {
				case WF_ESTADO_OS_EXECUCAO:
					$enviaEmail = "OK";
					break;
				default:
					$enviaEmail = "";
			}


			//cadastra a demanda e envia um email
			if($enviaEmail){
				$envia = envialEmailProfissionais($odsid, $usucpf);

				if(!$envia){
					$erro = "ERRO";
				}
			}
		}


	}
	elseif($acao == 'E'){

		//desvincula o profisional da OS
		$sql = " DELETE FROM fabrica.profissionalos
				 WHERE odsid = {$odsid}
				 and usucpf = '$usucpf'";
 	 	$db->executar($sql);
		$db->commit();

		//cancela demanda
		include_once APPRAIZ . 'includes/workflow.php';

		$sql = "SELECT d.dmdid, d.docid, doc.esdid FROM demandas.demanda d
				INNER JOIN workflow.documento doc ON doc.docid = d.docid
				WHERE d.odsid=$odsid and d.usucpfexecutor = '$usucpf'";

		$demandas = $db->pegaLinha($sql);

		if($demandas){

			$cmddsc = "Cancelado pelo Módulo de Fábrica.";
			$dadosVerificacao = (array) "a:1:{s:6:\"unicod\";s:0:\"\";}";

			$dmdid = $demandas['dmdid'];
			$esdid = $demandas['esdid'];
			$docid = $demandas['docid'];

			switch($esdid) {
				case '107': //em analise p/ cancelado - esdid=107&aedid=189
					$aedid = WF_DEMANDA_ACAO_ANALISE_PARA_CANCELADO;
					break;
				case '108': //em atendimento p/ cancelado - esdid=108&aedid=491
					$aedid = WF_DEMANDA_ACAO_ATENDIMENTO_PARA_CANCELADO;
					break;
				default:
					$aedid = 0;
			}

			if($aedid != 0){
				wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao );
			}

	 	 	$sql = " UPDATE demandas.demanda
					 SET  odsid = null
					 WHERE dmdid = $dmdid";
	 	 	$db->executar($sql);

	 	 	$db->commit();


		}




	}

	if($erro) echo "<script>alert('Erro.\nFavor entrar em contato com o administrador do sistema.');</script>";
	echo "<script>location.href = 'fabrica.php?modulo=principal/cadOSExecucaoCon&acao=A&odsid={$odsid}';</script>";
    exit;


}


//permissao para edição da tela
$habil = 'N';

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<script type="text/javascript">
function estimarContagemPF(odsid)
	{
		if(odsid){
			window.open('fabrica.php?modulo=principal/popup/contagemPontoFuncaoCon&acao=A&odsidpai=' + odsid + '&tosid=<?php echo TIPO_OS_CONTAGEM_DETALHADA ?>','Contagem_PF','scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no');
		}else{
			alert('Não é possível realizar a contagem desta OS!');
		}
	}



$(document).ready(function() {
	window.onbeforeunload = function (){
		if(document.getElementById('fecharTela').value == 'sim') {
			window.opener.location = window.opener.location;
			window.close();
		}
	}
});


function AcaoProf(acao, usucpf){

	document.getElementById('fecharTela').value='nao';

	odsid = "<?=$_REQUEST['odsid']?>";

	location.href="fabrica.php?modulo=principal/cadOSExecucaoCon&acao=A&odsid="+odsid+"&requisicao=executaAcaoProf&acao="+acao+"&usucpf="+usucpf;

}

function incluiProf(usucpf){
	if(document.formulario_.usucpf.value==""){
		alert("Selecione o Profissinal.");
		document.formulario_.usucpf.focus();
		return;
	}
	else{
		AcaoProf('I', document.formulario_.usucpf.value);
	}
}

function excluiProf(usucpf){
	if(confirm('Deseja realmente retirar este profissional desta OS?')){
		AcaoProf('E', usucpf);
	}
}


function salvarExec(){
    odssubtotalpfdetalhada
	if(document.getElementById('odssubtotalpfdetalhada').value ==''){
		alert("Preencha o Subtotal de PF Detalhado");
		document.getElementById('odssubtotalpfdetalhada').focus();
		return false;
	}
	if(document.getElementById('odsqtdpfdetalhada').value ==''){
		alert("Preencha a PF a Pagar");
		document.getElementById('odsqtdpfdetalhada').focus();
		return false;
	}

	document.getElementById('fecharTela').value='nao';
	salvar();
}

</script>

<?

$sql = "SELECT scsid, docid, odscontratada, odsdetalhamento, odsqtdpfestimada, odssubtotalpf, odssubtotalpfdetalhada, odsqtdpfdetalhada, odsid, to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio, to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino
		FROM fabrica.ordemservico os
		WHERE odsid='".$_REQUEST['odsid']."' ORDER BY odsid";

// buscando dados da OS
$oss = $db->pegaLinha($sql);

// validando se a ordem de serviço existe
if(!$oss) {
	die("<script>alert('OS não existe');window.close();</script>");
}

if($oss) $_SESSION['fabrica_var']['scsid'] = $oss['scsid'];


$estado_wf = wf_pegarEstadoAtual($oss['docid']);
$titulo_modulo = "OS Nº ".$oss['odsid'];

$menu = array(0 => array("id" => 1, "descricao" => "OS", 	"link" => "/fabrica/fabrica.php?modulo=principal/cadOSExecucaoCon&acao=A&odsid=".$_REQUEST['odsid']),
				  1 => array("id" => 2, "descricao" => "Observações",	"link" => "/fabrica/fabrica.php?modulo=principal/cadOSObservacaoCon&acao=A&odsid=".$_REQUEST['odsid']."&fecharTela=nao")
			  	  );
			  	  
echo montarAbasArray($menu);


//monta_titulo($titulo_modulo, 'Executando solicitação');

?>
<form method="post" name="formulario_" id="formulario_" enctype="multipart/form-data">
<?
// identificando qual requisição que o formulario deve executar ao ser submetido
switch($estado_wf['esdid']) {
	case WF_ESTADO_OS_VERIFICACAO:
		//$requisicao = "gravarAvaliacaoOS";
		$requisicao = "gravarVerificaoOS";
                $ator = "Fiscal Técnico";
		break;
	case WF_ESTADO_OS_EXECUCAO:
		$requisicao = "executarOS";
                $ator = "Empresa Item 1";
		break;
	case WF_ESTADO_OS_PENDENTE:
		$requisicao = "pendenteOS";
                $ator = "Empresa Item 1";
		break;
	case WF_ESTADO_OS_FINALIZADA:
		$requisicao = "finalizadaOS";
		break;
	case WF_ESTADO_OS_ATESTO_TECNICO:
		$requisicao = "gravarAtestoOS";
                $ator = "Requisitante";
		break;
	default:
		$requisicao = "inserirProfissionais";
                $ator = "Empresa Item 2";
}

monta_titulo($titulo_modulo, $ator);

//libera acesso para adicionar profissionais
$liberaAddProfissionais = false;

$pfls = arrayPerfil();

if( (in_array(PERFIL_SUPER_USUARIO, $pfls) ||
   in_array(PERFIL_FISCAL_CONTRATO,  $pfls) ||
   in_array(PERFIL_GESTOR_CONTRATO,  $pfls)) && !$oss['odscontratada']) {

   	$liberaAddProfissionais = true;

}

if( (in_array(PERFIL_SUPER_USUARIO, $pfls) ||
   in_array(PERFIL_PREPOSTO,  $pfls) ||
   in_array(PERFIL_ESPECIALISTA_SQUADRA,  $pfls)) && $oss['odscontratada']) {

   	$liberaAddProfissionais = true;

}

if($requisicao!="executarOS" && $requisicao!="pendenteOS") {
	$liberaAddProfissionais = false;
}

if($habil == 'N') $liberaAddProfissionais = false;
//fim libera acesso para adicionar profissionais


//controle para fechar popup
if(!$_SESSION['esdid_old']) $_SESSION['esdid_old'] = $estado_wf['esdid'];

if($_SESSION['esdid_old'] != $estado_wf['esdid']){
	$_REQUEST['fecharTela'] = "sim";
	$_SESSION['esdid_old'] = "";

if (($SituacaoAtual != 301)  && ($SituacaoAtual != 255)) {
  echo "<script>
          window.opener.location = window.opener.location;
          window.close();
       </script>";
}

}

?>
<input type="hidden" name="fecharTela" id="fecharTela" value="<?=$_REQUEST['fecharTela']?>">
<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td width="95%" valign="top">
	<table class="listagem" width="100%">
	<tr>
		<td class="SubTituloDireita">Número:</td>
		<td>
			<? echo $oss['odsid']; ?>
			<?php if((WF_ESTADO_OS_VERIFICACAO == $estado_wf['esdid']) && $habil == 'S'): ?>
			 	<input type="button" id="btn_enviar_contagem" name="btn_enviar_contagem" value="Detalhar Contagem de P.F." onclick="estimarContagemPF('<?  echo $oss['odsid']; ?>')" />
			 <?php endif; ?>
		</td>
	</tr>
	<tr>
		<td width="35%" class="SubTituloDireita">Prev. Início:</td>
		<td><? echo $oss['odsdtprevinicio']; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prev. Término:</td>
		<td>
                    <? echo $oss['odsdtprevtermino']; ?>
                    <?php if(!empty($_GET['odsid'])): ?>
                    <script type="text/javascript">
                        function alterarPrevisaoTermino(){

                            window.open('?modulo=principal/popup/alterarPrevisaoTerminoCon&acao=A&odsid=<?php echo $_GET['odsid']; ?>', 'alterarPrevisaoTermino', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');

                        }
                    </script>
                        <a href="javascript: alterarPrevisaoTermino()"
                           style="text-decoration: underline;">
                            Redefinir Previs&atilde;o de T&eacute;rmino
                        </a>
                    <?php endif; ?>
                </td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Subtotal de PF Estimado:</td>
		<td>
			<?php if($oss['odssubtotalpf']) $odsqtdpfestimada = number_format($oss['odssubtotalpf'],2,",",".");?>
			<? echo $odsqtdpfestimada; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Qtd. PF. estimado:</td>
		<td>
			<?php if($oss['odsqtdpfestimada']) $odsqtdpfestimada = number_format($oss['odsqtdpfestimada'],2,",",".");?>
			<? echo $odsqtdpfestimada; ?>
		</td>
	</tr>
        <? if($estado_wf['esdid'] == WF_ESTADO_OS_FINALIZADA){ ?>
	<tr>
		<td class="SubTituloDireita">Subtotal de PF Detalhado:</td>
		<td>
			<?php if($oss['odssubtotalpfdetalhada']) $odssubtotalpfdetalhada = number_format($oss['odssubtotalpfdetalhada'],2,",",".");?>
			<? echo $odssubtotalpfdetalhada; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">PF a Pagar:</td>
		<td>
			<?php if($oss['odsqtdpfdetalhada']) $odsqtdpfdetalhada = number_format($oss['odsqtdpfdetalhada'],2,",",".");?>
			<? echo $odsqtdpfdetalhada; ?>
		</td>
	</tr>
	<? }elseif($estado_wf['esdid'] == WF_ESTADO_OS_EXECUCAO || $estado_wf['esdid'] == WF_ESTADO_OS_VERIFICACAO){?>
	<tr>
		<td class="SubTituloDireita">Subtotal de PF Detalhado:</td>
		<td>
			<?php if($oss['odssubtotalpfdetalhada']) $odssubtotalpfdetalhada = number_format($oss['odssubtotalpfdetalhada'],2,",",".");?>
			<? echo campo_texto('odssubtotalpfdetalhada', 'S',  $habil, 'Subtotal de PF Detalhado', 15, 14, '###.###.###,##', '', '', '', 0, 'id="odssubtotalpfdetalhada"' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">PF a Pagar:</td>
		<td>
			<?php if($oss['odsqtdpfdetalhada']) $odsqtdpfdetalhada = number_format($oss['odsqtdpfdetalhada'],2,",",".");?>
			<? echo campo_texto('odsqtdpfdetalhada', 'S',  $habil, 'Qtd. de P.F. detalhado', 15, 14, '###.###.###,##', '', '', '', 0, 'id="odsqtdpfdetalhada"' ); ?>
		</td>
	</tr>
	<? }?>
	<tr>
		<td class="SubTituloDireita">Detalhamento:</td>
		<td><?  echo $oss['odsdetalhamento']; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Adicionar Profissionais:</td>
		<td >
		<?
		/*
		$sql = "SELECT u.usucpf as codigo, u.usunome as descricao
				FROM seguranca.usuario u
				LEFT JOIN fabrica.profissionalos p ON u.usucpf=p.usucpf
				WHERE odsid='".$oss['odsid']."'";

		$usucpf = $db->carregarColuna($sql);


		$sql = "SELECT osd.dspid, os.tpeid FROM fabrica.ordemservicodisciplina osd
				LEFT JOIN fabrica.ordemservico os ON osd.odsid=os.odsid
				LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid=os.scsid
				WHERE osd.odsid='".$oss['odsid']."'";

		$ds = $db->carregar($sql);
		if($ds[0]) {
			foreach($ds as $s) {
				$clausula_ur[] = "(ur.dspid='".$s['dspid']."' AND ur.tpeid='".$s['tpeid']."')";
			}
		}


		$sql = "SELECT distinct u.usucpf as codigo, u.usunome as descricao
				FROM seguranca.usuario u
				INNER JOIN seguranca.perfilusuario o ON u.usucpf=o.usucpf
				INNER JOIN seguranca.perfil p ON p.pflcod=o.pflcod
				INNER JOIN fabrica.usuarioresponsabilidade ur ON p.pflcod=ur.pflcod AND u.usucpf=ur.usucpf
				WHERE sisid='".$_SESSION['sisid']."' AND ur.rpustatus='A' " . ($usucpf ? " AND u.usucpf not in('".implode("','", $usucpf)."') " : "") . (($clausula_ur)?"AND (".implode(" OR ", $clausula_ur).")":"") . "ORDER BY 2";

		$db->monta_combo('usucpf', $sql, ($liberaAddProfissionais?"S":"N"), 'Selecione', '', '', '', '', 'N', 'usucpf', '');
		*/

		$sql = "SELECT u.usucpf as codigo, u.usunome as descricao
				FROM seguranca.usuario u
				LEFT JOIN fabrica.profissionalos p ON u.usucpf=p.usucpf
				WHERE odsid='".$oss['odsid']."'";
		$usucpf = $db->carregarColuna($sql);

		$sql = "SELECT DISTINCT dp.pflcod
				FROM fabrica.servicofaseproduto sfp
				INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid=sfp.fdpid
				INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid=fdp.fsdid
				INNER JOIN fabrica.disciplinaperfildemanda dp ON dp.dspid=fd.dspid
				INNER JOIN fabrica.analisesolicitacao ans ON ans.ansid=sfp.ansid
				INNER JOIN fabrica.ordemservico osd ON osd.scsid = ans.scsid
				WHERE osd.odsid='".$oss['odsid']."'";

		$ds = $db->carregar($sql);
		if($ds[0]) {
			foreach($ds as $s) {
				$clausula_ur[] = "(ur.pflcod='".$s['pflcod']."')";
			}
		}

		if($clausula_ur[0]){
			$sql = "SELECT distinct u.usucpf as codigo, u.usunome || ' - ' || p.pfldsc as descricao
					FROM seguranca.usuario u
					INNER JOIN seguranca.perfilusuario o ON u.usucpf=o.usucpf
					INNER JOIN seguranca.perfil p ON p.pflcod=o.pflcod
					INNER JOIN demandas.usuarioresponsabilidade ur ON p.pflcod=ur.pflcod AND u.usucpf=ur.usucpf
					WHERE sisid=".SISID_DEMANDAS." AND ur.celid IS NOT NULL AND ur.rpustatus='A' " . ($usucpf ? " AND u.usucpf not in('".implode("','", $usucpf)."') " : "") . (($clausula_ur)?"AND (".implode(" OR ", $clausula_ur).")":"") . "ORDER BY 2";
			$db->monta_combo('usucpf', $sql, ($liberaAddProfissionais?"S":"N"), 'Selecione', '', '', '', '', 'N', 'usucpf', '');

			echo "<br>";
			echo "<input type='button' name='btnadd' value='Adicionar' onclick='incluiProf();' ".(!$liberaAddProfissionais ? 'DISABLED' : '').">";
		}
		else{
			echo "Não existe(m) disciplina(s) para esta OS.";
		}
		//combo_popup( "usucpf", $sql, "Profissionais", "192x400", 0, array(), "", (($requisicao!="inserirProfissionais")?"N":"S"), false, false, 5, 400);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Profissionais Vinculados:</td>
		<td >
		<?
		/*
		$sql = "SELECT osd.dspid, os.tpeid FROM fabrica.ordemservicodisciplina osd
				LEFT JOIN fabrica.ordemservico os ON osd.odsid=os.odsid
				LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid=os.scsid
				WHERE osd.odsid='".$oss['odsid']."'";

		$ds = $db->carregar($sql);
		if($ds[0]) {
			foreach($ds as $s) {
				$clausula_ur[] = "(ur.dspid='".$s['dspid']."' AND ur.tpeid='".$s['tpeid']."')";
			}
		}


		$btnExcluir = "<a href=\"javascript:void(0);\" onclick=\"excluiProf(\'' || u.usucpf || '\')\"><img src=\"../imagens/excluir.gif\" border=\"0\"></a>";
		$btnDemanda = "<a href=\"javascript:void(0);\" onclick=\"window.close();window.opener.location=\'../demandas/demandas.php?modulo=principal/lista&acao=A&dmdid=".$db->pegaUm("SELECT dmdid FROM demandas.demanda WHERE odsid='".$oss['odsid']."'")."\';\"><img src=\"/imagens/consultar.gif\" border=\"0\"></a>";

		if(!$liberaAddProfissionais) {
			$btnExcluir = "";
			$btnDemanda = "";
		}

		$sql = "SELECT distinct
				'<center>".$btnExcluir." ".$btnDemanda."</center>' as acao,
				u.usunome as descricao
				FROM seguranca.usuario u
				INNER JOIN seguranca.perfilusuario o ON u.usucpf=o.usucpf
				INNER JOIN seguranca.perfil p ON p.pflcod=o.pflcod
				INNER JOIN fabrica.usuarioresponsabilidade ur ON p.pflcod=ur.pflcod AND u.usucpf=ur.usucpf
				INNER JOIN fabrica.profissionalos pr ON u.usucpf=pr.usucpf
				WHERE sisid='".$_SESSION['sisid']."' AND ur.rpustatus='A' AND pr.odsid='".$oss['odsid']."' ".(($clausula_ur)?"AND (".implode(" OR ", $clausula_ur).")":"") . "ORDER BY 2";
		//dbg($sql);
		$cabecalho = array("Ação","Nome");
		$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
		*/

		$btnExcluir = "<a href=\"javascript:void(0);\" onclick=\"excluiProf(\'' || u.usucpf || '\')\"><img src=\"../imagens/excluir.gif\" border=\"0\"></a>";
		$btnDemanda = "<a href=\"javascript:void(0);\" onclick=\"window.close();window.opener.location=\'../demandas/demandas.php?modulo=principal/lista&acao=A&dmdid=".$db->pegaUm("SELECT dmdid FROM demandas.demanda WHERE odsid='".$oss['odsid']."'")."\';\"><img src=\"/imagens/consultar.gif\" border=\"0\"></a>";

		if(!$liberaAddProfissionais) {
			$btnExcluir = "";
			$btnDemanda = "";
		}

		$sql = "SELECT distinct
				'<center>".$btnExcluir." ".$btnDemanda."</center>' as acao,
				u.usunome as descricao
				FROM seguranca.usuario u
				INNER JOIN fabrica.profissionalos pr ON u.usucpf=pr.usucpf
				WHERE pr.odsid=".$oss['odsid']." ORDER BY 2";
		//dbg($sql);
		$cabecalho = array("Ação","Nome");
		$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );

		?>
		</td>
	</tr>

	<?php
	switch($estado_wf['esdid']) {

		case WF_ESTADO_OS_VERIFICACAO:
			$verificacaoos = $db->pegaLinha("SELECT * FROM fabrica.verificacaoos WHERE odsid='".$oss['odsid']."'");
			$veoobs = $verificacaoos['veoobs'];
		?>
		<tr>
			<td colspan="2">
				<table class="listagem" width="100%">
					<tr>
						<td colspan="2" align="center" bgcolor="#c9c9c9"><b>AVALIAÇÃO</b></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">1. Os produtos solicitados estão nos padrões e requisitos técnicos especificados?</td>
						<td><input type="radio" name="veoprodutos" value="TRUE" <? echo (($verificacaoos['veoprodutos']=="t" || !$verificacaoos['veoprodutos'])?"checked":""); ?>> Sim <input type="radio" name="veoprodutos" value="FALSE" <? echo (($verificacaoos['veoprodutos']=="f")?"checked":""); ?>> Não</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">2. Os níveis de serviços especificados(disponibilidade, não conformidade, descumprimento de prazos) foram atendidos?</td>
						<td><input type="radio" name="veoniveis" value="TRUE" <? echo (($verificacaoos['veoniveis']=="t" || !$verificacaoos['veoniveis'])?"checked":""); ?>> Sim <input type="radio" name="veoniveis" value="FALSE" <? echo (($verificacaoos['veoniveis']=="f")?"checked":""); ?>> Não</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">3. As ocorrências e os riscos durante a execução do serviços foram monitorados e comunicados? Sim, Não, Nenhuma ocorrência.</td>
						<td><input type="radio" name="veoocorrencias" value="S" <? echo (($verificacaoos['veoocorrencias']=="S" || !$verificacaoos['veoocorrencias'])?"checked":""); ?>> Sim <input type="radio" name="veoocorrencias" value="N" <? echo (($verificacaoos['veoocorrencias']=="N")?"checked":""); ?>> Não <input type="radio" name="veoocorrencias" value="O" <? echo (($verificacaoos['veoocorrencias']=="O")?"checked":""); ?>> Nenhuma ocorrência</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Observação:</td>
						<td><? echo campo_textarea( 'veoobs', 'S',  $habil, '', '70', '4', '1000'); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2">
				<?php if($habil == 'S'){?>
				<input type="button" name="inserirAtesto_" value="Salvar" onclick="salvar();">
				<?php }?>
			</td>
		</tr>
		<?php
		break;
		case WF_ESTADO_OS_ATESTO_TECNICO:
			$atestoos = $db->pegaLinha("SELECT * FROM fabrica.atestoos WHERE odsid='".$oss['odsid']."'");
			$atoobs = $atestoos['atoobs'];
		?>
		<tr>
			<td colspan="2">
				<table class="listagem" width="100%">
					<tr>
						<td colspan="2" align="center" bgcolor="#c9c9c9"><b>ATESTO TÉCNICO</b></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">1. O tempo de atendimento do serviço foi de acordo com o esperado?</td>
						<td><input type="radio" name="atoatendimento" value="TRUE" <? echo (($atestoos['atoatendimento']=="t" || !$atestoos['atoatendimento'])?"checked":""); ?>> Sim <input type="radio" name="atoatendimento" value="FALSE" <? echo (($atestoos['atoatendimento']=="f")?"checked":""); ?>> Não</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">2. Questões como performance(tempo de resposta), disponibilidade, segurança, facilidade de uso, acessibilidade solicitadas foram atendidas?</td>
						<td><input type="radio" name="atoresposta" value="TRUE" <? echo (($atestoos['atoresposta']=="t" || !$atestoos['atoresposta'])?"checked":""); ?>> Sim <input type="radio" name="atoresposta" value="FALSE" <? echo (($atestoos['atoresposta']=="f")?"checked":""); ?>> Não</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">3. Os requisitos de negócio foram atendidos?</td>
						<td><input type="radio" name="atorequisito" value="TRUE" <? echo (($atestoos['atorequisito']=="t" || !$atestoos['atorequisito'])?"checked":""); ?>> Sim <input type="radio" name="atorequisito" value="FALSE" <? echo (($atestoos['atorequisito']=="f")?"checked":""); ?>> Não</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">4. O layout atende ao esperado?</td>
						<td><input type="radio" name="atolayout" value="TRUE" <? echo (($atestoos['atolayout']=="t" || !$atestoos['atolayout'])?"checked":""); ?>> Sim <input type="radio" name="atolayout" value="FALSE" <? echo (($atestoos['atolayout']=="f")?"checked":""); ?>> Não</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Observação:</td>
						<td><? echo campo_textarea( 'atoobs', 'S', $habil, '', '70', '4', '1000'); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2">
				<?php if($habil == 'S'){?>
				<input type="button" name="inserirAtesto_" value="Salvar" onclick="salvar();">
				<?php }?>
			</td>
		</tr>
		<?php
		break;
		case WF_ESTADO_AVALIACAO:
		?>
			<table class="listagem" width="100%">
				<tr>
					<td class="SubTituloEsquerda" colspan="2">Anexar arquivos na solicitação de serviço</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">OS:</td>
					<td>
					<?
					$sql = "SELECT odsid as codigo, 'OS #'||odsid as descricao FROM fabrica.ordemservico WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'";
					$db->monta_combo('odsid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'odsid');
					?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Arquivo:</td>
					<td><input type="file" name="arquivo" id="arquivo"></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Tipo de anexo:</td>
					<td><?
					$sql = "SELECT taoid as codigo, taodsc as descricao FROM fabrica.tipoanexoordem WHERE taostatus='A'";
					$db->monta_combo('taoid', $sql, $habil, 'Selecione', '', '', '', '', 'S', 'taoid');
					?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Descrição:</td>
					<td><? echo campo_texto('aosdsc_', 'S', $habil, 'Descrição', 50, 255, '', '', '', '', 0, 'id="aosdsc_"' ); ?></td>
				</tr>
			</table>
		<?php
		break;

		case WF_ESTADO_OS_EXECUCAO:
		?>
			<tr>
				<td class="SubTituloDireita" colspan="2">
					<?php if($habil == 'S'){?>
					<input type="button" name="inserirAtesto_" value="Salvar" onclick="salvarExec();">
					<?php }?>
				</td>
			</tr>
		<?php
		break;

		case WF_ESTADO_APROVACAO:
		?>

		<?php
		break;

		default:
		?>
		<tr>
			<td colspan="2" align="center" bgcolor="#c9c9c9"><br><b>Lista de Anexos</b></td>
		</tr>
		<tr>
			<td colspan="2">
			<?
			$sql = "SELECT to_char(an.aosdtinclusao,'dd/mm/YYYY'), tp.taodsc,
						'<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'{$_SERVER['REQUEST_URI']}&download=S&arqid=' || ar.arqid || '\';\" />' || ar.arqnome||'.'||ar.arqextensao ||'</a>',
						ar.arqtamanho, an.aosdsc
					FROM fabrica.anexoordemservico an
					LEFT JOIN fabrica.tipoanexoordem tp ON an.taoid=tp.taoid
					LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid
					WHERE odsid='".$oss['odsid']."' AND aosstatus='A'";

			$cabecalho = array("Data inclusão", "Tipo Arquivo", "Nome arquivo", "Tamanho(bytes)", "Descrição");
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
			?>
			</td>
		</tr>
		<?
	}
	?>
	</table>
	</td>

	<td width="5%" valign="top">

            <?php

            $dados_wf = array('scsid' => $_SESSION['fabrica_var']['scsid'], 'odsid' => $oss['odsid']);
            wf_desenhaBarraNavegacao($oss['docid'], $dados_wf);

            ?>

            <script type="text/javascript">

                function popPausarServico(){

                    window.open('?modulo=principal/popup/pausarServico&acao=A&odsid=<?php echo $_GET['odsid']; ?>', 'pausarServico', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');

                }

            </script>
            

	</td>
    </tr>
</table>
</form>



