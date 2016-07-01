<?
include APPRAIZ ."includes/workflow.php";

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

function atualizar_odsqtdpffinal($dados) {
	global $db;
	
	$sql = "UPDATE fabrica.ordemservico SET odsqtdpffinal='".$dados['odsqtdpffinal']."' WHERE odsid='".$dados['odsid']."'";
	$db->executar($sql);
	
	$db->commit();
	
	die("<script>
			alert('Qtd.de PF da OS foi definida');
			window.location='fabrica.php?modulo=principal/verDadosOs&acao=A&odsid=".$dados['odsid']."';
		 </script>");
	

}

// Variável de requisição para executar as funções solicitadas
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

// validando o ID da ordem de serviço
if(!$_REQUEST['odsid']) {
	die("<script>alert('OS não reconhecida');window.close();</script>");
}

// se passar o ID da solicitação, inserir na sessão
if($_REQUEST['scsid']) {
	$_SESSION['fabrica_var']['scsid'] = $_REQUEST['scsid']; 
}
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
			window.open('fabrica.php?modulo=principal/popup/contagemPontoFuncao&acao=A&odsidpai=' + odsid + '&tosid=<?php echo TIPO_OS_CONTAGEM_DETALHADA ?>','Contagem PF','scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no');
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

	location.href="fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid="+odsid+"&requisicao=executaAcaoProf&acao="+acao+"&usucpf="+usucpf;

}

</script>

<?

$sql = "SELECT docid, odscontratada, odsqtdpffinal, odsdetalhamento, odsqtdpfestimada, odsqtdpfdetalhada, odsid, to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio, to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino 
		FROM fabrica.ordemservico os 
		WHERE odsid='".$_REQUEST['odsid']."' ORDER BY odsid";

// buscando dados da OS
$oss = $db->pegaLinha($sql);

// validando se a ordem de serviço existe
if(!$oss) {
	die("<script>alert('OS não existe');window.close();</script>");
}



$estado_wf = wf_pegarEstadoAtual($oss['docid']);
$titulo_modulo = "OS Nº ".$oss['odsid'];

//monta_titulo($titulo_modulo, 'Executando solicitação');
monta_titulo($titulo_modulo, $estado_wf['esddsc']);

?>
<form method="post" name="formulario_" id="formulario_" enctype="multipart/form-data">
<?

//libera acesso para adicionar profissionais
$liberaAddProfissionais = false;

$pfls = arrayPerfil();
//fim libera acesso para adicionar profissionais

if(!$_REQUEST['fecharTela']) $_REQUEST['fecharTela'] = "nao";


?>
<input type="hidden" name="fecharTela" id="fecharTela" value="<?=$_REQUEST['fecharTela']?>">
<input type="hidden" name="requisicao" value="atualizar_odsqtdpffinal">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td width="95%" valign="top">
	<table class="listagem" width="100%">
	<tr>
		<td class="SubTituloDireita">Número:</td>
		<td>
			<? echo $oss['odsid']; ?>
		</td>
	</tr>
	<tr>
		<td width="35%" class="SubTituloDireita">Prev. Início:</td>
		<td><? echo $oss['odsdtprevinicio']; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prev. Término:</td>
		<td><? echo $oss['odsdtprevtermino']; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Detalhamento:</td>
		<td><?  echo $oss['odsdetalhamento']; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Profissionais Vinculados:</td>
		<td >
		<?
		
		$sql = "SELECT distinct 
				u.usunome as descricao 
				FROM seguranca.usuario u
				INNER JOIN fabrica.profissionalos pr ON u.usucpf=pr.usucpf
				WHERE pr.odsid=".$oss['odsid']." ORDER BY 1";
		//dbg($sql);
		$cabecalho = array("Nome");
		$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
		
		?>
		</td>
	</tr>
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
	<tr>
		<td colspan="2">
		<table class="listagem" width="100%">
			<tr>
				<td class="SubTituloCentro">&nbsp;</td>
				<td class="SubTituloCentro">Qtd. PF Estimado</td>
				<td class="SubTituloCentro">Qtd. PF Detalhada</td>
			</tr>
			<?
			$os = $db->pegaLinha("SELECT * FROM fabrica.ordemservico WHERE odsid='".$_REQUEST['odsid']."'");
			
			if($os['tosid'] == TIPO_OS_GERAL) {
				
				$empresa1 = array('qtd_estimado' => $os['odsqtdpfestimada'], 'qtd_detalhada' => $os['odsqtdpfdetalhada']);
				
				$os2 = $db->pegaLinha("SELECT odsqtdpfestimada FROM fabrica.ordemservico WHERE odsidpai='".$_REQUEST['odsid']."' AND tosid='".TIPO_OS_CONTAGEM_ESTIMADA."'");
				$empresa2['qtd_estimado'] = $os2['odsqtdpfestimada'];
				
				$os3 = $db->pegaLinha("SELECT odsqtdpfdetalhada FROM fabrica.ordemservico WHERE odsidpai='".$_REQUEST['odsid']."' AND tosid='".TIPO_OS_CONTAGEM_DETALHADA."'");
				$empresa2['qtd_detalhada'] = $os3['odsqtdpfdetalhada'];
				
			}
			
			$habilita= false;			
			if($empresa1['qtd_estimado'] > 0 && $empresa2['qtd_estimado'] > 0) {
				$num = round($empresa1['qtd_estimado']/$empresa2['qtd_estimado']*100);
				if($num > 100) {
					$porc = $num-100;
					
					if(($porc) > 5) $habilita = true;
					
				} else {
					$porc = 100-$num;
					
					if(($porc) > 5) $habilita = true;
					
				}
			}

			
			if($empresa1['qtd_detalhada'] > 0 && $empresa2['qtd_detalhada'] > 0) {
				$num = round($empresa1['qtd_detalhada']/$empresa2['qtd_detalhada']*100);
				if($num > 100) {
					$porc2 = $num-100;
					
					if(($porc2) > 5) $habilita = true;
					
				} else {
					$porc2 = 100-$num;
					
					if(($porc2) > 5) $habilita = true;
					
				}
			}
			
			?>
			<tr>
				<td class="SubTituloDireita">Empresa 1:</td>
				<td><? echo (($empresa1['qtd_estimado'])?$empresa1['qtd_estimado']:"0"); ?></td>
				<td><? echo (($empresa1['qtd_detalhada'])?$empresa1['qtd_detalhada']:"0"); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Empresa 2:</td>
				<td><? echo (($empresa2['qtd_estimado'])?$empresa2['qtd_estimado']:"0"); ?></td>
				<td><? echo (($empresa2['qtd_detalhada'])?$empresa2['qtd_detalhada']:"0"); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">%:</td>
				<td><? echo ((isset($porc))?$porc:"-"); ?></td>
				<td><? echo ((isset($porc2))?$porc2:"-"); ?></td>
			</tr>
			<? if($habilita): ?>
			
			<? $odsqtdpffinal = $oss['odsqtdpffinal']; ?>
			<tr>
				<td class="SubTituloDireita" colspan=2>Qtd.de PF da OS:</td>
				<td colspan=2><? echo campo_texto('odsqtdpffinal', 'N', 'S', 'Qtd. de P.F. Estimado', 10, 11, '##########', '', '', '', 0, 'id="odsqtdpffinal"' ); ?></td>

			</tr>
			<tr>
				<td colspan=4><input type="submit" name="btn_gravar" value="Gravar"></td>

			</tr>

			<? endif; ?>
		</table>
		</td>
	</tr>
	</table>
	</td>
</tr>
</table>
</form>



