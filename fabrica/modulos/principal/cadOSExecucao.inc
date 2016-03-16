<?php
include_once APPRAIZ ."includes/workflow.php";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

// Variável de requisição para executar as funçõeses solicitadas
if($_REQUEST['requisicao']) 
{
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

// validando o ID da ordem de serviço
if(!$_REQUEST['odsid']) {
	die("<script>alert('OS Não reconhecida');window.close();</script>");
}

$sql = "SELECT wkd.esdid
        FROM fabrica.ordemservico as fos
        INNER JOIN  workflow.documento as wkd
            ON wkd.docid = fos.docid
        INNER JOIN  workflow.estadodocumento as wed
            ON wed.esdid = wkd.esdid            
        INNER JOIN fabrica.analisesolicitacao ans
            ON fos.scsid = ans.scsid                      
        WHERE fos.odsid = {$_REQUEST["odsid"]}";

$SituacaoOrdem = $db->pegaLinha($sql);
$SituacaoAtual = $SituacaoOrdem["esdid"];

$ordemServico = new OrdemServico();
$ordemServico = $ordemServico->recuperePorId($_REQUEST['odsid']);

//PASSAGEM DO VALOR DO OBJETO PARA PEGA A SITUACAO ATUAL DA OS ESSE VALOR E UTILIZADO PARA SABER SE A OS E PASSIVEL DE GLOSA
$ordemServico->setStatusOrdemServico($SituacaoAtual);

if ($ordemServico->possuiGlosa()) {
	$glosa = new Glosa();
	$glosa = $glosa->recupereGlosaPeloId($ordemServico->getIdGlosa());
}

// se passar o ID da analise, inserir na sessão
if($_REQUEST['ansid']) {
	$_SESSION['fabrica_var']['ansid'] = $_REQUEST['ansid'];
}

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $file->getDownloadArquivo( $arqid );
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    exit;
}
$historicoPrevisaoTermino = new HistoricoPrevisaoTermino();
$arrDadosSituacaoStatusOS = $historicoPrevisaoTermino->retornarDataPrevisaoTerminoInicialEmpresaItem1( $_REQUEST['odsid'] );

$checkedAvalItem2 = false;

if ( strtoupper( $arrDadosSituacaoStatusOS['status_os'] ) == 'OS EM DIA')
{
    $colorStatusOS = 'green';
} else
{
    $colorStatusOS = 'red';
    
    $qtdPFDetalhadaOS       = $ordemServico->getQtdePfDetalhada();
    $valorIDP               = ( ( $arrDadosSituacaoStatusOS['dias_em_atraso'] * 100) / $arrDadosSituacaoStatusOS['dias_previstos'] );
    $porcentagemDisciplina  = $ordemServico->recuperarPorcentagemDisciplinasContratadas($_REQUEST["odsid"]);
    
    $percentualGlosa = 0;

    if ( $valorIDP > 81 )
    {
        $percentualGlosa = 20;
    } elseif ( $valorIDP >= 51 && $valorIDP <= 80 )
    {
        $percentualGlosa = 15;
    } elseif ( $valorIDP >= 31 && $valorIDP <= 50 )
    {
        $percentualGlosa = 10;
    } elseif ( $valorIDP >= 11 && $valorIDP <= 30 )
    {
        $percentualGlosa = 5;
    }

    $qtdPFGlosa = ( ( $qtdPFDetalhadaOS * ( $percentualGlosa / 100 )) * ( $porcentagemDisciplina / 100 ) );
    
    $checkedAvalItem2 = true;
                 
    $db->executar( $sql );
    $db->commit();
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
	echo "<script>location.href = 'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid={$odsid}';</script>";
    exit;


}
//permissao para edição da tela
 $habil = 'S';
if(!verificaPermissaoEdicao()) $habil = 'N';


//recupera sigla do item da metrica
$sigla = recuperaMetrica( $_SESSION['fabrica_var']['ansid'] );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link rel='stylesheet' type='text/css' href='./css/jquery-ui-1.8.16.custom.css'/>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />
<style type="text/css">
	.ui-widget{font-size: 9pt;}
</style>

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<script type="text/javascript">
function estimarContagemPF(odsid)
{
    if( odsid ) 
    {
        window.open('fabrica.php?modulo=principal/popup/contagemPontoFuncao&acao=A&odsidpai=' + odsid + '&tosid=<?php echo TIPO_OS_CONTAGEM_DETALHADA ?>','Contagem_PF','scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no');
    }else{
        alert('Não é possí­vel realizar a contagem desta OS!');
    }
}

function listaArtefatos( idSS )
{
	document.getElementById('fecharTela').value = '';
	var caminho = "fabrica.php?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao=" + idSS;
	window.opener.location = caminho;
}

$(document).ready(function() {
	window.onbeforeunload = function (){
		if(document.getElementById('fecharTela').value == 'sim') {
			
				window.opener.location = window.opener.location;
				window.close();
			
		}
	}
});

function validaContrato(){
	 <?php if ( WF_ESTADO_OS_VALIDACAO ){?>
	 	<?if($sigla == 'PF'){?>
		 if(document.getElementById('odssubtotalpfdetalhada').value ==''){
			alert("Preencha o Subtotal de PF Detalhado");
			document.getElementById('odssubtotalpfdetalhada').focus();
			return false;
		}
		<?}?>
		if(document.getElementById('odsqtdpfdetalhada').value ==''){
			alert("Preencha a PF a Pagar");
			document.getElementById('odsqtdpfdetalhada').focus();
			return false;
		}
        
        if( document.getElementById('ctrid').value == '' )
        {
			alert("Selecione o contrato");
			document.getElementById('ctrid').focus();
			return false;
        }
        
        document.getElementById('formulario_').submit();

	<?php }?>
}
function AcaoProf(acao, usucpf){

	document.getElementById('fecharTela').value='nao';

	odsid = "<?=$_REQUEST['odsid']?>";

	location.href="fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid="+odsid+"&requisicao=executaAcaoProf&acao="+acao+"&usucpf="+usucpf;

}

function incluiProf(usucpf){
	if(document.formulario_.usucpf.value==""){
		alert("Selecione o Profissional.");
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
    <?if($sigla == 'PF'){?>
	if(document.getElementById('odssubtotalpfdetalhada').value ==''){
		alert("Preencha o Subtotal de PF Detalhado");
		document.getElementById('odssubtotalpfdetalhada').focus();
		return false;
	}
	<?}?>
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

$sql = "SELECT os.scsid, os.docid, os.odscontratada, os.odsdetalhamento, os.odsqtdpfestimada, os.odssubtotalpf, os.odssubtotalpfdetalhada, os.odsqtdpfdetalhada,
               os.odsid, to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio, to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino, os.ctrid, ans.ansgarantia,
               os.tosid
		FROM fabrica.ordemservico os
        INNER JOIN fabrica.analisesolicitacao ans
            ON os.scsid = ans.scsid
		WHERE odsid='".$_REQUEST['odsid']."' ORDER BY odsid";

// buscando dados da OS
$oss = $db->pegaLinha($sql);

$habilitado = 'S';
if( $oss['ansgarantia'] == 't' )
{
    $habilitado = 'N';
    $oss['odssubtotalpfdetalhada']  = 0;
    $oss['odsqtdpfdetalhada']       = 0;
}


// validando se a ordem de serviço existe
if(!$oss) {
	die("<script>alert('OS Não existe');window.close();</script>");
}

if($oss) $_SESSION['fabrica_var']['scsid'] = $oss['scsid'];

/*
if(!$oss['docid'] && $oss['tosid']=='1'){
	enviar_execucao( $_SESSION['fabrica_var']['scsid'] );
	//envia_os_pausa_execucao( $scsid );
}
*/

$estado_wf = wf_pegarEstadoAtual( $oss['docid'] );

$titulo_modulo = "OS Nº <span style='cursor:pointer; color: #0066CC;' onclick=window.location.reload()>".$oss['odsid']."</span>";

echo montarAbasArray(carregarMenuOS());



/*
* Criado por: Rondomar França
* Data: 04-07-2012
* SS-982 - REQ001 - Ordem de Serviço - "Em execuação"
* */
if( WF_ESTADO_OS_EXECUCAO == $estado_wf['esdid'] ){

	$habilitarGerenciaDeConfiguracao = !$ordemServico->temUrlDosArtefatos( $ordemServico->odsid ); 
}else{
	$habilitarGerenciaDeConfiguracao = false;
}

//monta_titulo($titulo_modulo, 'Executando solicitação');
?>
<form method="post" name="formulario_" id="formulario_" enctype="multipart/form-data">
<?
// identificando qual requisição que o formulario deve executar ao ser submetido

switch( $estado_wf['esdid'] ) {
	case WF_ESTADO_OS_VERIFICACAO:
		//$requisicao = "gravarAvaliacaoOS";
		$requisicao = "gravarVerificaoOS";
                $ator = "Fiscal Técnico";
		break;
	case ( WF_ESTADO_OS_EXECUCAO == $estado_wf['esdid'] ) || ( WF_ESTADO_OS_DIVERGENCIA == $estado_wf['esdid'] ):
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
                $ator = "Empresa Item 1";
}

monta_titulo($titulo_modulo, $ator);

//libera acesso para adicionar profissionais
$liberaAddProfissionais = false;

$pfls = arrayPerfil();

if( (in_array(PERFIL_SUPER_USUARIO, $pfls) ||
   in_array(PERFIL_FISCAL_CONTRATO,  $pfls) ||
   in_array(PERFIL_GESTOR_CONTRATO,  $pfls) ||
   in_array(PERFIL_ADMINISTRADOR, $pfls)) && !$oss['odscontratada']) {

   	$liberaAddProfissionais = true;

}

if( (in_array(PERFIL_SUPER_USUARIO, $pfls) ||
   in_array(PERFIL_PREPOSTO,  $pfls) ||
   in_array(PERFIL_ESPECIALISTA_SQUADRA,  $pfls) ||
   in_array(PERFIL_ADMINISTRADOR, $pfls)) && $oss['odscontratada']) {

   	$liberaAddProfissionais = true;

}

$bloqueiaSubTotalDetalhada = false;
if( in_array(PERFIL_FISCAL_CONTRATO,  $pfls)  && $estado_wf['esdid'] == WF_ESTADO_OS_VERIFICACAO ) {

	$bloqueiaSubTotalDetalhada = true;

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


}

?>
<input type="hidden" name="fecharTela" id="fecharTela" value="<?=$_REQUEST['fecharTela']?>">
	<?php 
	if( $estado_wf['esdid'] == WF_ESTADO_OS_PENDENTE){
		?>
		<script>
			document.getElementById('fecharTela').value = 'sim';
		</script>
		<?php
	} 
	?>
<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
    <tr>
            <td width="95%" valign="top">
                <table class="listagem" width="100%" >
                    <tr>
                        <td width="35%"  class="SubTituloDireita">Status:</td>
                        <td style="color: <?php echo $colorStatusOS ?>; font-weight: bold">
                            <?php echo $arrDadosSituacaoStatusOS['status_os']; ?>
                        </td>
                    </tr>                    
                </table>
            </td>
        </tr>
<tr>
	<td width="95%" valign="top">
	<table class="listagem" width="100%">
	<tr>
		<td class="SubTituloDireita">N&uacute;mero:</td>
		<td>
			<? echo "<span style='cursor:pointer; color: #0066CC;' onclick=window.location.reload()>".$oss['odsid']."</span>"; ?>
			<?php if((WF_ESTADO_OS_VERIFICACAO == $estado_wf['esdid']) && $habil == 'S' && $sigla == 'PF'): ?>
			 	<input type="button" id="btn_enviar_contagem" name="btn_enviar_contagem" value="Detalhar Contagem de P.F." onclick="estimarContagemPF('<?  echo $oss['odsid']; ?>')" />
			 <?php endif; ?>
		</td>
	</tr>
	<tr>
        <td width="35%" class="SubTituloDireita">Prev. In&iacute;cio:</td>
		<td><? echo $oss['odsdtprevinicio']; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prev. Término:</td>
		<td>
                    <? echo $oss['odsdtprevtermino']; ?>
                    <?php if(!empty($_GET['odsid'])): ?>
                    <script type="text/javascript">
                        function alterarPrevisaoTermino(){

                            window.open('?modulo=principal/popup/alterarPrevisaoTermino&acao=A&odsid=<?php echo $_GET['odsid']; ?>&alteracao=OSitem1', 'alterarPrevisaoTermino', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');

                        }
                    </script>
                          <?php 
                          if($estado_wf['esdid'] != WF_ESTADO_OS_FINALIZADA): ?>
                                <a href="javascript: alterarPrevisaoTermino()"
                                   style="text-decoration: underline;">
                                    Redefinir Previs&atilde;o de T&eacute;rmino
                                </a>
                            <?php endif; ?>
                    <?php endif; ?>
                </td>
	</tr>
	<?if($sigla == 'PF'){?>
		<tr>
			<td class="SubTituloDireita">Subtotal de PF Estimado:</td>
			<td>
				<?php if($oss['odssubtotalpf']) $odsqtdpfestimada = number_format($oss['odssubtotalpf'],2,",",".");?>
				<? echo $odsqtdpfestimada; ?>
			</td>
		</tr>
	<?}?>
	<tr>
		<td class="SubTituloDireita">Qtd. estimada de <?=$sigla?>:</td>
		<td>
			<?php if($oss['odsqtdpfestimada']) $odsqtdpfestimada = number_format($oss['odsqtdpfestimada'],2,",",".");?>
			<? echo $odsqtdpfestimada; ?>
		</td>
	</tr>
    <? if( $estado_wf['esdid'] == WF_ESTADO_OS_FINALIZADA || $estado_wf['esdid'] == WF_ESTADO_OS_APROVACAO || $estado_wf['esdid'] == WF_ESTADO_OS_ATESTO_TECNICO || $bloqueiaSubTotalDetalhada==true){ ?>
		<?if($sigla == 'PF'){?>
			<tr>
				<td class="SubTituloDireita">Subtotal de PF Detalhado:</td>
				<td>
					<?php 
		            if($oss['odssubtotalpfdetalhada']) 
		            	$odssubtotalpfdetalhada = number_format($oss['odssubtotalpfdetalhada'],2,",",".");
		            else
		            	$odssubtotalpfdetalhada = '0,00';
					
		            echo $odssubtotalpfdetalhada; ?>
					<input type="hidden" value="<? echo $odssubtotalpfdetalhada; ?>" id="odssubtotalpfdetalhada" />
				</td>
			</tr>
		<?}?>
		
		<tr>
			<td class="SubTituloDireita"><?=$sigla?> a Pagar:</td>
			<td>
				<?php 
				if($oss['odsqtdpfdetalhada'])
					$odsqtdpfdetalhada = number_format($oss['odsqtdpfdetalhada'],2,",",".");
				else 
					$odsqtdpfdetalhada = '0,00';
				
				echo $odsqtdpfdetalhada; ?>
				<input type="hidden" value="<? echo $odsqtdpfdetalhada; ?>" id="odsqtdpfdetalhada" />
			</td>
		</tr>
	
	<? }elseif( $estado_wf['esdid'] == WF_ESTADO_OS_EXECUCAO || $estado_wf['esdid'] == WF_ESTADO_OS_VERIFICACAO  || $estado_wf['esdid'] ==  WF_ESTADO_OS_DIVERGENCIA ){?>
		
		<?if($sigla == 'PF'){?>
			<tr>
				<td class="SubTituloDireita">Subtotal de PF Detalhado:</td>
				<td>
					<?php if(  $oss['odssubtotalpfdetalhada'] || $oss['odssubtotalpfdetalhada'] == 0 ) $odssubtotalpfdetalhada = number_format($oss['odssubtotalpfdetalhada'],2,",",".");?>
					<? echo campo_texto('odssubtotalpfdetalhada', 'S',  $habilitado, 'Subtotal de PF Detalhado', 15, 14, '###.###.###,##', '', '', '', 0, 'id="odssubtotalpfdetalhada"' ); ?>
				</td>
			</tr>
		<?}?>
		<tr>
			<td class="SubTituloDireita"><?=$sigla?> a Pagar:</td>
			<td>
				<?php if($oss['odsqtdpfdetalhada'] || $oss['odsqtdpfdetalhada'] == 0) $odsqtdpfdetalhada = number_format($oss['odsqtdpfdetalhada'],2,",",".");?>
				<?php echo campo_texto('odsqtdpfdetalhada', 'S',  $habilitado, 'Qtd. detalhado', 15, 14, '###.###.###,##', '', '', '', 0, 'id="odsqtdpfdetalhada"' ); ?>
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

		$btnExcluir = "<a title=\"Excluir\" href=\"javascript:void(0);\" onclick=\"excluiProf(\'' || u.usucpf || '\')\"><img src=\"../imagens/excluir.gif\" border=\"0\"></a>";
		$btnDemanda = "<a title=\"Consultar\" href=\"javascript:void(0);\" onclick=\"window.close();window.opener.location=\'../demandas/demandas.php?modulo=principal/lista&acao=A&dmdid=".$db->pegaUm("SELECT dmdid FROM demandas.demanda WHERE odsid='".$oss['odsid']."'")."\';\"><img src=\"/imagens/consultar.gif\" border=\"0\"></a>";

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
	<?php if ($ordemServico->possuiGlosa()) { ?>
		<tr>
			<td class="SubTituloDireita">Quantidade de ponto de função glosado:</td>
			<td><?php echo $glosa->getValorEmPfComMascara();?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Justificativa da glosa:</td>
			<td><?php echo $glosa->getJustificativa();?></td>
		</tr>
	<?php } 
	
	/*
	 * Criado por: Rondomar França
	 * SS831 - REQ004 - Detalhe da Ordem de Serviço(OS) - Finalizada
	 * */
	if( $estado_wf['esdid'] == WF_ESTADO_FINALIZADA || WF_ESTADO_OS_FINALIZADA == $estado_wf['esdid'])
	{
		$sqlMemoLink = "select 
							a.memoid, b.memonumero || '/' || date_part('year', b.memodata) as memonumero_ano
						from fabrica.ordemservico a
						inner join fabrica.memorando b on a.memoid = b.memoid
						where a.odsid = " . $oss['odsid'];
		
		$rsMemoId = $db->pegaLinha( $sqlMemoLink );
		?>
		<tr>
			<td class="SubTituloDireita">Memorando:</td>
			<td><a href="javascript:abrirMemorando( <?php echo($rsMemoId['memoid']); ?> )"><?php echo( $rsMemoId['memonumero_ano'] );?></a></td>
		</tr>
		<?php
	}
	
	
	switch($estado_wf['esdid']) {

		case WF_ESTADO_OS_VERIFICACAO:
			$verificacaoos              = $db->pegaLinha("SELECT * FROM fabrica.verificacaoos WHERE odsid='".$oss['odsid']."'");
			$veoobs                     = $verificacaoos['veoobs'];
            $verificacaoos['veoniveis'] = $checkedAvalItem2 == true ? 'f' : $verificacaoos['veoniveis'];
            if( $checkedAvalItem2 == true && empty($veoobs) ){
                $veoobs                     = 'Demanda entregue fora do prazo';
            }
            
		?>
        
        
		<tr>
			<td colspan="2">
				<table class="listagem" width="100%">
					<tr>
						<td colspan="2" align="center" bgcolor="#c9c9c9"><b>AVALIAÇÃO</b></td>
					</tr>
                                         
					<tr>
            <td class="SubtituloDireita" id="contrato" name="contrato" >Contrato:</td>
            <td>
                    <?php 
                    $ctrid = $oss['ctrid'];
                    if($ctrid != 0){
                        $sql = "SELECT ctrid FROM fabrica.contrato WHERE ctrid= $ctrid ";
                        $ctrid = $db->pegaUm($sql);
                    }

				$sql = " SELECT distinct 
                                ctr.ctrid as codigo,
                                (ent.entnome ||' - ' || to_char(ctrdtinicio,'dd/MM/YYYY') || ' à  ' || to_char(ctrdtfim,'dd/MM/YYYY')) as descricao
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
                                and ctr.ctrtipoempresaitem = 1
                                ORDER BY codigo desc" ;
                    
                    if (empty($ctrid) && !empty($_REQUEST['ctrid'])) {
                        //$ctrid = $EmAndamento; 
                        $ctrid = $_REQUEST['ctrid'];
                        
                    }       
                                
                         $db->monta_combo("ctrid",$sql,$habil,"Selecione...","","","","","S","ctrid","",$ctrid); 
				                  
                    
                    ?>

                   
            </td>
        </tr>
					<tr>
						<td class="SubTituloDireita">1. Os produtos solicitados estão nos padrões e requisitos técnicos especificados?</td>
						<td><input type="radio" name="veoprodutos" value="TRUE" <? echo (($verificacaoos['veoprodutos']=="t" || !$verificacaoos['veoprodutos'])?"checked":""); ?>> Sim <input type="radio" name="veoprodutos" value="FALSE" <? echo (($verificacaoos['veoprodutos']=="f")?"checked":""); ?>> Não</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">2. Os ní­veis de serviços especificados(disponibilidade, Não conformidade, descumprimento de prazos) foram atendidos?</td>
						<td><input type="radio" name="veoniveis" value="TRUE" <? echo (($verificacaoos['veoniveis']=="t" || !$verificacaoos['veoniveis'])?"checked":""); ?>> Sim <input type="radio" name="veoniveis" value="FALSE" <? echo (($verificacaoos['veoniveis']=="f")?"checked":""); ?>> Não</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">3. As ocorrências e os riscos durante a execução do serviços foram monitorados e comunicados? Sim, Não, Nenhuma ocorrência.</td>
						<td><input type="radio" name="veoocorrencias" value="S" <? echo (($verificacaoos['veoocorrencias']=="S" || !$verificacaoos['veoocorrencias'])?"checked":""); ?>> Sim <input type="radio" name="veoocorrencias" value="N" <? echo (($verificacaoos['veoocorrencias']=="N")?"checked":""); ?>> Não <input type="radio" name="veoocorrencias" value="O" <? echo (($verificacaoos['veoocorrencias']=="O")?"checked":""); ?>> Nenhuma ocorrência</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Observação:</td>
						<td><? echo campo_textarea( 'veoobs', 'N',  $habil, '', '70', '4', '1000'); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2">
                            <?php
                                    $parametroSistema = new ParametrosSistema();
                                     
									$dataAtualEhMaiorOuIgualADataDeTramitacao = true;
									$dataAtual = new DateTime();
									if ($oss['odssubtotalpf'] > 50){
	                                   	 
	                                    $sqlDataTramitacao = "SELECT to_char(htddata, 'mm/dd/YYYY') as datatramitacao
	                                        from fabrica.solicitacaoservico ss 
	                                        join fabrica.ordemservico os
	                                                on os.scsid = ss.scsid
	                                        join workflow.documento d 
	                                                on d.docid = ss.docid
	                                        join workflow.historicodocumento hsd
	                                                on d.docid = hsd.docid
	                                        join workflow.acaoestadodoc aed
	                                                on hsd.aedid = aed.aedid
	                                        where 
	                                                os.odsid = ".$_REQUEST['odsid']."
	                                                and aed.esdidorigem = 247 and  aed.esdiddestino = 248
	                                        order by htddata desc limit 1";
	                                    $dataHoraRecuperada = $db->pegaLinha($sqlDataTramitacao);  
	                                    $dataTramitacao = new DateTime($dataHoraRecuperada['datatramitacao']);
	                                    $diasTramitacao = $parametroSistema->recuperaValorPeloNome(ParametrosSistema::PERIODO_DETALHAMENTO_AVALIACAO);
	                                    
	                                    $dataTramitacao->modify("+ $diasTramitacao days");
	                                    
	                                    $dataAtualEhMaiorOuIgualADataDeTramitacao = $dataAtual>=$dataTramitacao;
	  								}
                                ?>
				<?php if($habil == 'S' and $dataAtualEhMaiorOuIgualADataDeTramitacao) {?>
					<input type="button" name="inserirAtesto_" value="Salvar" onclick="validaContrato();">
				<?php }else{?>
						<span style="color:#C00">De acordo com a parametrização do sistema (<?php echo$diasTramitacao;?>  dias) não é possível tramitar a OS.</span>
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
						<td><? echo campo_textarea( 'atoobs', 'N', $habil, '', '70', '4', '1000'); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2">
			 <?php
                                    $parametroSistema = new ParametrosSistema();
									$dataAtualEhMaiorOuIgualADataDeTramitacao = true;
									
									if ($oss['odssubtotalpf'] > 50){
	                                   	 
	                                    $sqlDataTramitacao = "SELECT to_char(htddata, 'mm/dd/YYYY') as datatramitacao
	                                        from fabrica.solicitacaoservico ss 
	                                        join fabrica.ordemservico os
	                                                on os.scsid = ss.scsid
	                                        join workflow.documento d 
	                                                on d.docid = ss.docid
	                                        join workflow.historicodocumento hsd
	                                                on d.docid = hsd.docid
	                                        join workflow.acaoestadodoc aed
	                                                on hsd.aedid = aed.aedid
	                                        where 
	                                                os.odsid = ".$_REQUEST['odsid']."
	                                                and aed.esdidorigem = 247 and  aed.esdiddestino = 248
	                                        order by htddata desc limit 1";
	                                    $dataHoraRecuperada = $db->pegaLinha($sqlDataTramitacao);  
	                                    $dataTramitacao = new DateTime($dataHoraRecuperada['datatramitacao']);
	                                    $diasTramitacao = $parametroSistema->recuperaValorPeloNome(ParametrosSistema::PERIODO_DETALHAMENTO_AVALIACAO);
	                                    
	                                    $dataTramitacao->modify("+ $diasTramitacao days");
	                                    
	                                    $dataAtual = new DateTime();
	                                    
	                                    $dataAtualEhMaiorOuIgualADataDeTramitacao = $dataAtual>=$dataTramitacao;
	  								}
                                ?>
				<?php if($habil == 'S' and $dataAtualEhMaiorOuIgualADataDeTramitacao) {?>
					<input type="button" name="inserirAtesto_" value="Salvar" onclick="salvar();">
				<?php }else{?>
						<span style="color:#C00">De acordo com a parametrização do sistema (<?php echo$diasTramitacao;?>  dias) não é possível tramitar a OS.</span>
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

		case (WF_ESTADO_OS_EXECUCAO == $estado_wf['esdid']) || (WF_ESTADO_OS_DIVERGENCIA == $estado_wf['esdid']):
            if( $checkedAvalItem2 == true ){
                $veoobs                     = 'Demanda entregue fora do prazo';
            }
		?>
			<tr>
				<td class="SubTituloDireita" colspan="2">
							 <?php
                                    $parametroSistema = new ParametrosSistema();
                                    $dataAtualEhMaiorOuIgualADataDeTramitacao = true;
									
									if ($oss['odssubtotalpf'] > 50){
	                                   	 
	                                    $sqlDataTramitacao = "SELECT to_char(htddata, 'mm/dd/YYYY') as datatramitacao
	                                        from fabrica.solicitacaoservico ss 
	                                        join fabrica.ordemservico os
	                                                on os.scsid = ss.scsid
	                                        join workflow.documento d 
	                                                on d.docid = ss.docid
	                                        join workflow.historicodocumento hsd
	                                                on d.docid = hsd.docid
	                                        join workflow.acaoestadodoc aed
	                                                on hsd.aedid = aed.aedid
	                                        where 
	                                                os.odsid = ".$_REQUEST['odsid']."
	                                                and aed.esdidorigem = 247 and  aed.esdiddestino = 248
	                                        order by htddata desc limit 1";
	                                    $dataHoraRecuperada = $db->pegaLinha($sqlDataTramitacao);  
	                                    $dataTramitacao     = new DateTime($dataHoraRecuperada['datatramitacao']);
	                                    $diasTramitacao     = $parametroSistema->recuperaValorPeloNome(ParametrosSistema::PERIODO_DETALHAMENTO_AVALIACAO);
	                                    
	                                    $dataTramitacao->modify("+ $diasTramitacao days");
	                                    
	                                    $dataAtual = new DateTime();
	                                    
	                                    $dataAtualEhMaiorOuIgualADataDeTramitacao = $dataAtual>=$dataTramitacao;
	  								}
                                ?>
					<?php if($habil == 'S' and $dataAtualEhMaiorOuIgualADataDeTramitacao) {?>
                        <input type="button" name="inserirAtesto_" value="Salvar" onclick="salvarExec();" />
					<?php }else{?>
							<span style="color:#C00">De acordo com a parametrização do sistema (<?php echo$diasTramitacao;?>  dias) não é possível tramitar a OS.</span>
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
			$sql = "SELECT to_char(an.aosdtinclusao,'dd/mm/YYYY HH24:MI'), tp.taodsc,
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
            	<?php if( $habilitarGerenciaDeConfiguracao ) {?>
					$(document).ready(function(){
						if( $('table').find('a[title="Enviar para Avaliação"]').size() > 0 )
						{
							$('table').find( 'a[title="Enviar para Avaliação"]' )
							.css( 'color' , '#666' )
							.css( 'text-decoration' , 'none' )
							.attr( 'onclick', '')
							.click( function(){
								alert("Artefatos não informados na funcionalidade de GC(SVN)"); 
							});
						}else{
							$('#estadoConsultarGC').hide();
						}		
					});
				<?php }?>

                function popPausarServico()
                {
                    window.open('?modulo=principal/popup/pausarServico&acao=A&odsid=<?php echo $_GET['odsid']; ?>', 'pausarServico', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');
                }

            </script>
            
            <table border="0" cellpadding="3" cellspacing="0"
                style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
                <tr style="background-color: #c9c9c9; text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <span title="estado atual">
                            <b>outras ações</b>
                        </span>
                    </td>
                </tr>

                    <?php
                  if ( strtoupper( $arrDadosSituacaoStatusOS['status_os'] ) == 'OS EM ATRASO' && (!$ordemServico->isCancelada())){ ?>
                        <tr>
                            <td id="inserirGlosa" style="font-size: 7pt; text-align: center; border-bottom: 2px solid #c9c9c9; cursor: pointer;" onmouseover="this.style.backgroundColor='#ffffdd';" onmouseout="this.style.backgroundColor='';">
                            <span style="color: #133368;">Glosa</span>
                            </td>
                        </tr>    
                    <?php
                    }
                        
                        if( ($estado_wf['esdid'] == WF_ESTADO_OS_EXECUCAO && !in_array( PERFIL_GERENTE_PROJETO, $pfls ) ) 
                                || ($estado_wf['esdid']  == WF_ESTADO_OS_EXECUCAO && in_array(PERFIL_SUPER_USUARIO, $pfls) )
 								|| ($estado_wf['esdid'] ==  WF_ESTADO_OS_EXECUCAO && in_array(PERFIL_ADMINISTRADOR, $pfls) ) )
                        {
                            
                            $sqlAnexoDetalhada = " SELECT 
                                count(an.aosid) as codigo
                            FROM fabrica.anexoordemservico an
                            INNER JOIN fabrica.tipoanexoordem tp 
                                ON an.taoid=tp.taoid
                            INNER JOIN public.arquivo ar 
                                ON ar.arqid=an.arqid
                            INNER JOIN fabrica.ordemservico os 
                                ON os.odsid=an.odsid
                            WHERE 
                                os.odsid= ". $_REQUEST['odsid'] ."
                                AND aosstatus='A' 
                                AND tp.taoid = 29";

                            $resultadoAnexoDetalhada = $db->pegaLinha( $sqlAnexoDetalhada );

                            //if( $resultadoAnexoDetalhada['codigo'] > 0 ) {
                                ?>
                                <td style="font-size: 7pt; text-align: center; border-bottom: 2px solid #c9c9c9;"
                                    onmouseover="this.style.backgroundColor='#ffffdd';"
                                    onmouseout="this.style.backgroundColor='';">
                                    <a href="javascript: popPausarServico();">
                                    Enviar para Pausa
                                    </a>
                                </td>
                                <?php
                            /*}else{
                                ?>
                                <td onmouseover="return escape('Relatório de contagem de PF Detalhada não anexado');" 
                                    onclick="alert( 'Relatório de contagem de PF Detalhada não anexado' )"
                                            style="font-size: 7pt; color: #909090; border-top: 2px solid #d0d0d0; text-align: center;">
                                    Enviar para Pausa
                                </td>
                            </tr>
                                <?php
                            } */
			
                        if ( $habilitarGerenciaDeConfiguracao ){
                        ?>
                            <tr id="estadoConsultarGC" id="estadoConsultarGC">
                                <td style="font-size: 7pt; text-align: center; border-bottom: 2px solid #c9c9c9; cursor: pointer;" 
                                    onmouseover="this.style.backgroundColor='#ffffdd';"
                                    onmouseout="this.style.backgroundColor='';" 
                                    onclick="listaArtefatos( <?php echo( $ordemServico->scsid )?>);">
                                <span style="color: #133368;">Consultar GC</span>
                                </td>
                            </tr>
                        <?php } 
                        }?>
            </table>
	</td>
    </tr>
</table>
</form>

<div id="modalFormularioSalvarGlosa" style="display: none;">
	<form id="formularioSalvarGlosa" method="post" action="geral/cadastro_glosa_em_cadOsExecucao.php">
		<p class="paragrafo_glosa">
			<label style="display: block;">Qtde em dias de atraso:</label>
			<span class="campos"><?php echo $arrDadosSituacaoStatusOS['dias_em_atraso'];?></span>
		</p>
		<p class="paragrafo_glosa">
			<label style="display: block;">Qtde em PF da glosa:</label>
			<span class="campos"><?php echo $qtdPFGlosa;?></span>
		</p>
		<p class="paragrafo_glosa">
			<label style="display: block;">Data de inicio:</label>
			<span class="campos"><?php echo date_create($arrDadosSituacaoStatusOS['odsdtprevinicio'])->format('d/m/Y');?></span>
		</p>
		<p class="paragrafo_glosa">
			<label style="display: block;">Data de término planejada:</label>
			<span class="campos"><?php echo $arrDadosSituacaoStatusOS['data_termino_planejada'];?></span>
		</p>
		<p class="paragrafo_glosa">
			<label style="display: block;">Data de entrega:</label>
			<span class="campos"><?php echo $arrDadosSituacaoStatusOS['datatramitacao'];?></span>
		</p>
		<p class="paragrafo_textarea">
			<label>Motivo da glosa</label>
			<?php
                if( empty($textoPadrao) )
                {
                    $textoPadrao= $ordemServico->possuiGlosa() ? $glosa->getJustificativa() : $veoobs;
                }
			?>
			<span class="campos"><?php echo $textoPadrao;?></span>
		</p>
		<p>
			<?php if($ordemServico->possuiGlosa()) { ?>
				<input type="hidden" name="glosaid" value="<?php echo $ordemServico->getIdGlosa()?>" />
			<?php } ?>
			<input type="hidden" name="idOrdemServico" value="<?php echo $_REQUEST['odsid']?>"/>
			<input type="hidden" name="cpfUsuarioResponsavel" value="<?php echo $_SESSION['usucpf'] ?>"/>
			<input type="hidden" name="dataInclusao" value="<?php echo date('Y-m-d')?>" />
		</p>
	</form>
</div>
