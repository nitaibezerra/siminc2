<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";


$dmdid = $_SESSION['dmdid'];



$habil = 'N';
$mostraWorkflow = 'N';

//verifica se o usuário pode editar a demanda
$verificaEditaDemanda = verificaEditaDemanda();
if($verificaEditaDemanda == true){
	$habil = 'S';
	$mostraWorkflow = 'S';
}
else{
	$habil = 'N';
	$mostraWorkflow = 'N';
}

// Pega o "estado do documento", vinculado à demanda
$esdid = recuperaEstadoDocumento();
//verifica se o estado do documento é finalizada ou cancelada (se sim, bloqueia todos os campos).
if( in_array($esdid, array(DEMANDA_ESTADO_CANCELADO,DEMANDA_GENERICO_ESTADO_CANCELADO,DEMANDA_ESTADO_FINALIZADO,DEMANDA_GENERICO_ESTADO_FINALIZADO,DEMANDA_ESTADO_INVALIDADA,DEMANDA_GENERICO_ESTADO_INVALIDADA,DEMANDA_ESTADO_VALIDADA_FORA_PRAZO)) ){
	$habil = 'N';
}


//habilita e desabilita campos para o perfil gerente e analista fabrica de software
if ( ( in_array(DEMANDA_PERFIL_GERENTE_FSW, arrayPerfil()) || in_array(DEMANDA_PERFIL_ANALISTA_FSW, arrayPerfil()) ) && in_array($esdid, array(DEMANDA_GENERICO_ESTADO_EM_ANALISE, DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO, DEMANDA_GENERICO_ESTADO_AGUARDANDO_VALIDACAO)) ){
	$habil = 'S';
	$mostraWorkflow = 'S';
}



if($_GET['anxid'] && $_GET['arqid'] && $_GET['op5']){
	session_start();
	$_SESSION['anxid'] = $_GET['anxid'];
	$_SESSION['arqid'] = $_GET['arqid'];
	$_SESSION['op5'] = $_GET['op5'];
	header( "Location: demandas.php?modulo=principal/arquivosAnexados&acao=A" );
	exit();
}

if($_REQUEST['op5'] == 'download'){
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    exit;
}
/*
if($_REQUEST['op5'] == 'download'){

	$param = array();
	$param["arqid"] = $_REQUEST['arqid'];
	DownloadArquivo($param);
	exit;

}
*/

if($_SESSION['anxid'] && $_SESSION['arqid'] && $_SESSION['op5']){
	if($_SESSION['op5'] == 'delete'){
		
		$campos	= array("anxid" => $_SESSION['anxid']);
		$file   = new FilesSimec("anexos", $campos, "demandas");
		// Remove o registro do anexo antigo (pelo arqid) e o arquivo físico
		$file->setRemoveUpload( $_SESSION['arqid'] );
		
		unset($_SESSION['anxid']);
		unset($_SESSION['arqid']);
		unset($_SESSION['op5']);
		
		echo "<script>alert('Operação realizada com sucesso!');</script>";
		echo "<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
		
		die;		
		/*
		deletarAnexo();	
		unset($_SESSION['anxid']);
		unset($_SESSION['arqid']);
		unset($_SESSION['op5']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
		//header( "Location: demandas.php?modulo=principal/equipeSistema&acao=A" );
		 */
	}
	
}

/*
 * Executa a função que insere o arquivo
 * Retorna mensagem de sucesso e redireciona a página
 */
if ($_FILES['anexo']['size']){
 	 $dmdid = $_SESSION['dmdid'];
 	 $dados["arqdescricao"] = $_REQUEST["arqdescricao"];
	 if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], $dados, $dmdid)){
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href='demandas.php?modulo=principal/arquivosAnexados&acao=A';
			 </script>");
	 }else{
	 	die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");	
	 }
	
}

include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ ."includes/cabecalho.inc";

/*
 * Recupera o "docid"
 * Caso não exista "docid" cadastrado na demanda 
 * Insere o documento e vincula na demanda
 */
$docid = criarDocumento( $dmdid );

print '<br>';
$db->cria_aba( $abacod_tela, $url, '');

monta_titulo( 'Arquivos Anexados - Cód. # '.$dmdid, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<?=cabecalhoDemanda(); ?>
    
<script type="text/javascript" src="/includes/prototype.js"></script>
<script src="../fabrica/js/autocomplete/jquery-1.7.2.js"></script>
<script type="text/javascript">
jQuery.noConflict();
jQuery(document).ready(function(){
<?php 
    $estadoAtualDaDemanda =  wf_pegarEstadoAtual($docid);
    if( in_array($estadoAtualDaDemanda['esdid'], array(
    DEMANDA_ESTADO_EM_ANALISE
    ,DEMANDA_ESTADO_EM_ATENDIMENTO
    ,DEMANDA_GENERICO_ESTADO_EM_ANALISE	       				
    ,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO			   				
    ) ) ) { ?>
        jQuery('a[title^="Avaliação da demanda"]').removeAttr("href").css('color', '#B5B5B5').css('text-decoration', 'none');
<?php } ?>
});
</script>
<form id="formAnexos" name="formAnexos" method="post" enctype="multipart/form-data" onsubmit="return validaForm();" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<input type="hidden" name="dmdid" value="<?php echo $_SESSION['dmdid']; ?>" />
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>&nbsp;</td>		
		<td rowspan="6" width="1%">	
		<?php 
				if($mostraWorkflow == 'S'){
					// Barra de estado atual e ações e Historico
					wf_desenhaBarraNavegacao( $docid , array( 'unicod' => '' ) );	
					//Barra outras opções
					criarNovaDemanda(); //www/demandas/_funcoes.php
				}else{
					?>
						<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>estado atual</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<?php 
												if($esdid){
													$sqlesd = "SELECT esddsc
															FROM workflow.estadodocumento
															WHERE esdid = $esdid";
													
													echo $db->pegaUm($sqlesd);
												}
												else{
													echo "Em Processamento";
												}
												?>					
											</span>
										</td>
									</tr>
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>ações</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												você não possui permissão
											</span>
										</td>
									</tr>
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>histórico</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<img style="cursor: pointer;" src="http://<?=$_SERVER['SERVER_NAME']?>/imagens/fluxodoc.gif"	 onclick="exibeHistoricoDem( '<?=$docid?>' );">
										</td>
									</tr>
									
						</table>					
					<? 
				}
			?>	
		</td>			
	</tr>
	<tr>
		<td class="SubTituloDireita">Arquivo:</td>
		<td><input type="file" name="anexo" <?if($habil=='N') echo "disabled";?>><? echo obrigatorio(); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" >Descrição:</td>
		<td><?= campo_textarea( 'arqdescricao', 'N', $habil, '', 60, 2, 250 ); ?></td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
	    	<input type='submit' class='botao' value='Salvar' name='cadastrar' <?if($habil=='N') echo "disabled";?>>	    	
		</td>			
	</tr>
	<tr>
		<td colspan="2" height="40">&nbsp;</td>
	</tr>
</table>

<?

	$sql = "SELECT
			CASE WHEN p.usucpf = '{$_SESSION['usucpf']}' THEN 
				'<a href=\"demandas.php?modulo=principal/arquivosAnexados&acao=A&arqid=' || a.arqid || '&op5=download\">
				 	<img border=0 src=\"../imagens/anexo.gif\" />
				 </a> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=principal/arquivosAnexados&acao=A&arqid=' || a.arqid || '&anxid=' || a.anxid || '&op5=delete\');\" >
				 	<img border=0 src=\"../imagens/excluir.gif\" />
				 </a>'
			ELSE
				'<a href=\"demandas.php?modulo=principal/arquivosAnexados&acao=A&arqid=' || a.arqid || '&op5=download\">
				 	<img border=0 src=\"../imagens/anexo.gif\" />
				 </a>'
			END AS OPCOES,
			to_char(a.anxdtinclusao,'DD/MM/YYYY') AS dtinclusao,
			'<a style=\"cursor: pointer; color: blue;\" href=\"demandas.php?modulo=principal/arquivosAnexados&acao=A&arqid=' || a.arqid || '&op5=download\">' || p.arqnome || '</a>' AS arqnome,
			p.arqtamanho || ' kb' AS arqtamanho,
			p.arqdescricao,
			u.usunome
		
			FROM demandas.anexos AS a
		LEFT JOIN public.arquivo AS p ON a.arqid = p.arqid
		LEFT JOIN seguranca.usuario AS u ON p.usucpf = u.usucpf
		
			WHERE a.dmdid = {$_SESSION['dmdid']} AND a.anxstatus = 'A'";
	$cabecalho = array( "Opções","Data da Inclusão","Nome do Arquivo","Tamanho","Descrição do Arquivo","Responsável");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
?>
</form>		
			
			
<form name="formdp">
<?php
$sql = "SELECT
		 d.dmdid || ' ',
		 d.dmdtitulo, 
		 to_char(a.anxdtinclusao, 'DD-MM-YYYY HH24:MI:SS') AS data,
		 --p.arqdescricao,
		 '<a style=\"cursor: pointer; color: blue;\" href=\"demandas.php?modulo=principal/arquivosAnexados&acao=A&arqid=' || a.arqid || '&op5=download\">' || p.arqnome || '</a>' AS arqnome,  
		 p.arqdescricao,
		 u.usunome
		FROM
			demandas.anexos AS a 
		LEFT JOIN public.arquivo AS p ON a.arqid = p.arqid
		LEFT JOIN seguranca.usuario AS u ON u.usucpf = p.usucpf 
		LEFT JOIN demandas.demanda as d ON d.dmdid = a.dmdid 
	    WHERE
	     a.dmdid in (select de.dmdidorigem from demandas.demanda as de where de.dmdid = ".$dmdid.") AND 
	     a.anxstatus = 'A' 
	    ORDER BY
	     a.anxid DESC";
$aux = $db->PegaUm($sql);
if($aux){
	echo "<br><br><div><table width='95%' align='center'><tr><td><b>Anexos da Demanda Principal</b></td></tr></table></div>";
	$cabecalho = array("Cód. Demanda","Assunto","Postado","Nome do arquivo","Descrição do arquivo","Responsável");			
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
}
?>
</form>

<form name="formsd">
<?php
$sql = "SELECT
		 d.dmdid || ' ',
		 d.dmdtitulo, 
		 to_char(a.anxdtinclusao, 'DD-MM-YYYY HH24:MI:SS') AS data,
		 --p.arqdescricao,
		 '<a style=\"cursor: pointer; color: blue;\" href=\"demandas.php?modulo=principal/arquivosAnexados&acao=A&arqid=' || a.arqid || '&op5=download\">' || p.arqnome || '</a>' AS arqnome,  
		 p.arqdescricao,
		 u.usunome
		FROM
			demandas.anexos AS a 
		LEFT JOIN public.arquivo AS p ON a.arqid = p.arqid
		LEFT JOIN seguranca.usuario AS u ON u.usucpf = p.usucpf 
		LEFT JOIN demandas.demanda as d ON d.dmdid = a.dmdid 
	    WHERE
	     a.dmdid in (select de.dmdid from demandas.demanda as de where de.dmdidorigem = ".$dmdid.") AND 
	     a.anxstatus = 'A' 
	    ORDER BY
	     a.anxid DESC";
$aux = $db->PegaUm($sql);
if($aux){
	echo "<br><br><div><table width='95%' align='center'><tr><td><b>Anexos das SubDemandas</b></td></tr></table></div>";
	$cabecalho = array("Cód. Demanda","Assunto","Postado","Nome do arquivo","Descrição do arquivo","Responsável");			
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
}
?>
</form>
			
			
			
			
			
<script type="text/javascript">

function validaForm(){
	if(document.formAnexos.anexo.value == ''){
		alert ('Selecione o Arquivo.');
		document.formAnexos.anexo.focus();
		return false;
	}
	/*if(document.formAnexos.arqdescricao.value == ''){
		alert ('O campo Descrição deve ser preenchido.');
		document.formAnexos.arqdescricao.focus();
		return false;
	}*/
}

function exclusao(url) {
	if ('<?=$habil?>' == 'N'){
		alert('Seu perfil não lhe permite executar esta operação!');
		return;
	}	
	var questao = confirm("Tem Certeza?")
	if (questao){
		window.location = url;
	}
}

function exibeHistoricoDem( docid )
{
	var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}				

</script>


</body>

<?php 
######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$dmdid){
	global $db;
	
	if (!$arquivo || !$dmdid)
		return false;
		
	// obtém o arquivo
	#$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	
			(
				arqnome,
				arqextensao,
				arqdescricao,
				arqtipo,
				arqtamanho,
				arqdata,
				arqhora,
				usucpf,
				sisid
			)VALUES(
				'".current(explode(".", $arquivo["name"]))."',
				'".end(explode(".", $arquivo["name"]))."',
				'".$dados["arqdescricao"]."',
				'".$arquivo["type"]."',
				'".$arquivo["size"]."',
				'".date('Y/m/d')."',
				'".date('H:i:s')."',
				'".$_SESSION["usucpf"]."',
				". $_SESSION["sisid"] ."
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	//Insere o registro na tabela demandas.anexos
	$sql = "INSERT INTO demandas.anexos 
			(
				dmdid,
				arqid,
				anxdtinclusao,
				anxstatus
			)VALUES(
			    ". $dmdid .",
				". $arqid .",
				now(),
				'A'
			);";
	$db->executar($sql);
	
	if(!is_dir('../../arquivos/demandas/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/demandas/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				return false;
			}
	}
	
	
	$db->commit();
	return true;

}

/*
######## Fim funções de UPLOAD ########
function deletarAnexo(){
	global $db;
	$sql = "UPDATE public.arquivo SET
				arqstatus = 0 
			WHERE arqid = {$_SESSION['arqid']}";
	$db->executar($sql);
	$sql = "UPDATE demandas.anexos SET
				anxstatus = 'I' 
			WHERE anxid = {$_SESSION['anxid']}";
	$db->executar($sql);
	$db->commit();
	//header( "Location: demandas.php?modulo=principal/arquivosAnexados&acao=A" );
	//exit();
}

function DownloadArquivo($param){
		global $db;
		
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$param['arqid'];
		$arquivo = $db->carregar($sql);
        $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
		if ( !is_file( $caminho ) ) {
            $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
        }
        $filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
        header( 'Content-type: '. $arquivo[0]['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
}
*/
?>