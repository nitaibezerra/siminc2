<?php 

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$campos	= array('slcid' 			=> $_SESSION['sic']['slcid'],
				'anedescricao'		=> "'".$_REQUEST['anedescricao']."'",
				'taaid'				=> $_REQUEST['taaid']);
	
$file = new FilesSimec("anexoatividade", $campos, 'sic');
if(is_file($_FILES["arquivo"]["tmp_name"])){
	$arquivoSalvo = $file->setUpload($_FILES["arquivo"]["name"], "arquivo");
	$db->sucesso('principal/anexoPedido');
}

if($_REQUEST['requisicao'] == 'downloadArquivo'){	
	$file->getDownloadArquivo($_REQUEST['arqid']);	
}

if($_REQUEST['requisicao'] == 'deletaArquivo'){	
	$file->setRemoveUpload($_REQUEST['arqid']);
	$db->sucesso('principal/anexoPedido');	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";

// Recupera dados do workflow
$docid = criaDocumento($_SESSION['sic']['slcid']);
$esdid = pegaEstadoAtual($docid);

// Funções de abas
$arMnuid = array();
if(!$docid || in_array($esdid, array(WF_ESDID_EM_CADASTRAMENTO,WF_ESDID_DEVOLVIDO_PELA_AREA_RESP))){	
	$arMnuid[] = 11631;
}

if(!in_array($esdid,array(WF_ESDID_RECURSO_1_INSTANCIA,WF_ESDID_RECURSO_2_INSTANCIA,WF_ESDID_ANALISE_1_RECURSO,WF_ESDID_ANALISE_2_RECURSO))){
	$arMnuid[] = 11052;
}

$abacod_tela = 57548;
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

monta_titulo('Anexos do pedido', $linha2);

// Recupera dados do workflow
$docid = criaDocumento($_SESSION['sic']['slcid']);
$esdid = pegaEstadoAtual($docid);

// Verifica se pode editar
$boAtivo = 'N';
if((!$docid || in_array($esdid, array(WF_ESDID_EM_CADASTRAMENTO, 
										WF_ESDID_ANALISE_AREA_RESPONSAVEL, 
										WF_ESDID_DEVOLVIDO_PELA_AREA_RESP,
										WF_ESDID_RECURSO_1_INSTANCIA,
										WF_ESDID_RECURSO_2_INSTANCIA,
										WF_ESDID_ANALISE_1_RECURSO,
										WF_ESDID_ANALISE_2_RECURSO,
										WF_ESDID_FINALIZADO_1_INSTANCIA,
										WF_ESDID_FINALIZADO_2_INSTANCIA)))){
	$boAtivo = 'S';
}

if( checkPerfil(array(SIC_PERFIL_ORGAO_CONTROLE)) ){
	$boAtivo = 'N';
}

?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script>
	$(function(){
		
		jQuery("[name=arquivo]").addClass("required");
		jQuery("[name=anedescricao]").addClass("required");
		
		jQuery("#formulario_id").validate();
	});

	function salvarDados()
	{
		$('#formulario_id').submit();
	}

	function downloadAnexo(arqid)
	{
		document.location.href = 'sic.php?modulo=principal/anexoPedido&acao=A&requisicao=downloadArquivo&arqid='+arqid;
	}

	function deletaAnexo(arqid)
	{
		if(confirm('Deseja deletar o anexo?')){
			document.location.href = 'sic.php?modulo=principal/anexoPedido&acao=A&requisicao=deletaArquivo&arqid='+arqid;
		}
	}
	
</script>
<center>
<form method="post" name="formulario" id="formulario_id" enctype="multipart/form-data">
	<input type="hidden" name="evento" value="cadastrar_anexo"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 95%;">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
				<input type="file" name="arquivo" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?>/>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			</td>
		</tr>		
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
			<td>
				<select style="width: 200px;" class="CampoEstilo" name="taaid" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?>>
					<optgroup label="Instrumento Legal">
						<?php
							$sql = sprintf( "select taaid, taadescricao from pde.tipoanexoatividade where taalegal = true order by taadescricao asc" );
						?>
						<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
						<option value="<?= $tipo['taaid'] ?>"><?= $tipo['taadescricao'] ?></option>
						<?php endforeach; ?>
					</optgroup>
					<optgroup label="Instrumento de Trabalho">
						<?php
							$sql = sprintf( "select taaid, taadescricao from pde.tipoanexoatividade where taalegal = false order by taadescricao asc" );
						?>
						<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
						<option value="<?= $tipo['taaid'] ?>"><?= $tipo['taadescricao'] ?></option>
						<?php endforeach; ?>
					</optgroup>
				</select>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?= campo_textarea( 'anedescricao', 'S', $boAtivo, '', 70, 2, 250 ); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<?php if($boAtivo == 'S'): ?>
					<input type="button" name="botao" value="Salvar" onclick="salvarDados();"/>
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>
</center>
<?php 

$stAcao = '';
if($boAtivo == 'S'){
	$stAcao .= '<img src="../imagens/excluir.gif" style=\"cursor:pointer\" onclick=\"deletaAnexo(\' || aq.arqid || \')\" />';
}

if(!$_SESSION['sic']['slcid']){
    echo "<script>
        alert('Sua sessão expirou. Por favor, efetue seu login novamente!');
        document.location.href = '../login.php';
        </script>";
    die;
}

$sql = "select 
			'<center>
				{$stAcao}	
				<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer\" onclick=\"downloadAnexo(' || aq.arqid || ')\"/>
			</center>' as acao,
			aq.arqnome || '.' || aq.arqextensao,
			to_char(aq.arqdata ,'dd/MM/yyyy'),
			us.usunome
		from
			sic.solicitacao sc
		inner join 
			sic.anexoatividade an on an.slcid = sc.slcid
		inner join
			public.arquivo aq on aq.arqid = an.arqid
		inner join
			seguranca.usuario us on us.usucpf = aq.usucpf
		where 
			sc.slcid = {$_SESSION['sic']['slcid']}
		and
			arqstatus = 'A'";

$cabecalho = array('Ação','Descrição','Data','Responsável');

$db->monta_lista($sql, $cabecalho, 10, 10, '', '', '', '', '');
?>