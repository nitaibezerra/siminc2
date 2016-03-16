<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . 'includes/workflow.php';

if(!$_SESSION['elabrev_var']['lbrid']) {
	
	echo "<script>
			alert('Problemas nas variaveis enviadas');
			window.location='elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A';
		  </script>";
	exit;
	
}

if( $_REQUEST['submeter'] == '1' ){
	$data = formata_data_sql($_POST['anxdata']);
	
	if($_FILES["arquivo"] && !$_POST['arqid']){
		$campos	= array("lbrid"  => $_SESSION['elabrev_var']['lbrid'],
						"anxdsc" => "'{$_POST['anxdsc']}'"
						);	
		$file = new FilesSimec("anexo", $campos ,"elabrev");
		$arquivoSalvo = $file->setUpload();
		if($arquivoSalvo){
			echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
			echo"<script>window.location.href = 'elabrev.php?modulo=principal/anexos&acao=A';</script>";
			die;	
		}
	} elseif($_POST['arqid']) {
	    $sql = "UPDATE elabrev.anexo SET anxdsc = '{$_POST['anxdsc']}' where arqid=".$_POST['arqid'];
	    $db->executar($sql);
	    $db->commit();
	    echo '<script type="text/javascript"> 
	    		alert("Operação realizada com sucesso!");
	    		window.location.href = "elabrev.php?modulo=principal/anexos&acao=A";
	    	  </script>';
	    die;
	}
}

if($_GET['arqidDel']){
    $sql = "UPDATE elabrev.anexo SET anxstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href="elabrev.php?modulo=principal/anexos&acao=A";
    	  </script>';
    die;
}

#manipular anexos de arquivos para envio de email
if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'elabrev.php?modulo=principal/anexos&acao=A';</script>";
    exit;
}

include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev_var']['lbrid'] );

$abas = menuAbasLiberacao();
echo montarAbasArray($abas, $_SERVER['REQUEST_URI']);

cabecalhoLiberacao($_SESSION['elabrev_var']['lbrid']);
	
if( $_REQUEST['arqid'] ){
		$sql = "select anx.anxid,
				   anx.arqid,
				   anx.lbrid,
				   anx.anxdsc,
				   arq.arqnome || '.' || arq.arqextensao as arquivo				   
			from elabrev.anexo anx
				inner join public.arquivo arq on anx.arqid = arq.arqid
	   		where arq.arqid = {$_REQUEST['arqid']} and anxstatus = 'A'";
		$arReg = $db->pegaLinha( $sql );
		$arReg = $arReg ? $arReg : array();
		extract($arReg);
		
		//$anxdata = formata_data( $anxdata );
	}

?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td style="width: 95%">
	<form name=formulario id=formulario method=post enctype="multipart/form-data">
	<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />
	<input type="hidden" name="submeter" id="submeter" value="1">

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
		<tr>
		    <th>Arquivo</th>
		    <th>Descrição</th>
		    <th>Ações</th>
	    </tr>
		<tr>
		    <td><input type="file" name="arquivo" id="arquivo" /></td>
		    <td><?=campo_texto( 'anxdsc', 'S', 'S', 'Descrição Anexo', 80, 100, '', '','','','','id="anxdsc"'); ?> </td>
		    <td><input type="button" name="btnSalvar" id="btnSalvar" <?=disabled($estadoAtual); ?> onclick="validaFormAnexos();" value="Salvar"> </td>
	    </tr>
	    <?
	    $sql = "select anx.anxid,
				   anx.arqid,
				   anx.anxdsc,
				   arq.arqnome || '.' || arq.arqextensao as arquivo
				from elabrev.anexo anx
				inner join public.arquivo arq on anx.arqid = arq.arqid
		  		where lbrid = {$_SESSION['elabrev_var']['lbrid']} and anxstatus = 'A'";
		   
		$arDados = $db->carregar($sql);
	    $count = 1;
	    
		if($arDados) {
		    foreach($arDados as $dados){
		    ?>
			<tr>		    
		        <td align="left">
		        	<?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/anexos&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>'"><?php echo $dados['arquivo'];?></a>
		        </td>
		        <td align="left"><?php echo $dados['anxdsc']; ?></td>
		        <td align="center">
		        <? if( !disabled( $estadoAtual ) ) {?>
		        	<img src="../imagens/alterar.gif" onClick="window.location.href='elabrev.php?modulo=principal/anexos&acao=A&arqid=<?php echo $dados['arqid'];?>'" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
		        	<img src="../imagens/excluir.gif" onClick="excluirAnexo('<?php echo $dados['arqid']; ?>');" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
		        <? } else { ?>
		        	<img src="../imagens/alterar_01.gif" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
		        	<img src="../imagens/excluir_01.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
		        <? } ?>
		        </td>
			   	</tr>
		    <?php
		    $count++;
		    }
		} else {
			echo '<tr>
			    	<td style="color: red">Nenhum documento disponível!</td>
			    </tr>';
		}
	    ?>
	</table>
	</form>
	</td>
	<td valign="top"><?
		$docid = criarDocumento( $_SESSION['elabrev_var']['lbrid'] );
		wf_desenhaBarraNavegacao( $docid , array( 'lbrid' => $_SESSION['elabrev_var']['lbrid'], 'url' => $_SESSION['favurl'] ) );
	?></td>
</tr>
</table>
<script type="text/javascript">
var objForm = document.forms["formulario"];

if(objForm.arqid.value != ''){
	objForm.arquivo.disabled = true;
}

function validaFormAnexos(){
	var alerta = "";
	if(objForm.arquivo.value == '' && objForm.arqid.value == ''){
		alert("Você deve escolher um arquivo.");
		return false;
	}
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
		$("#formulario").submit();
	}
}
	 
function excluirAnexo( arqid, tipo ){
 	if ( confirm( 'Deseja excluir o Documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid;
 	}
}
</script>