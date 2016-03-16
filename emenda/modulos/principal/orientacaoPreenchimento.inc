<?php 
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

verificaPermissao();

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'emenda.php?modulo=principal/orientacaoPreenchimento&acao=A';</script>";
    exit;
} 
if( $_FILES["arquivo"] && !$_POST['arqid']){
		$campos	= array(
					"anxdsc" => "'{$_POST['anxdsc']}'",
					"anxtipo" => "'O'"
					);
					
	$file = new FilesSimec("anexo", $campos ,"emenda");
	$arquivoSalvo = $file->setUpload();
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
		echo"<script>window.location.href = 'emenda.php?modulo=principal/orientacaoPreenchimento&acao=A';</script>";
		die;	
	}
} elseif( $_POST['arqid'] ) {	
    $sql = "UPDATE emenda.anexo SET anxdsc = '{$_POST['anxdsc']}' where arqid=".$_POST['arqid'];
    $db->executar($sql);
    $db->commit();
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href = "emenda.php?modulo=principal/orientacaoPreenchimento&acao=A";
    	  </script>';
    die;
}
 
if($_GET['arqidDel']){
    $sql = "DELETE FROM emenda.anexo where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href="emenda.php?modulo=principal/orientacaoPreenchimento&acao=A";
    	  </script>';
    die;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Orientações de Preenchimento', '' );

$sql = "select anx.anxid,
		   anx.arqid,
		   anx.anxdsc,
		   arq.arqnome || '.' || arq.arqextensao as arquivo
		from emenda.anexo anx
			inner join public.arquivo arq on anx.arqid = arq.arqid
 		where anx.anxtipo = 'O'";

$arDados = $db->carregar($sql);

$arPerfil = pegaPerfilArray($_SESSION['usucpf']);

if( in_array( ANALISTA_FNDE, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) || in_array( SUPER_USUARIO, $arPerfil ) ){
	$boHabilita = true;
	$acao = '<th>Ação</th>';
} else {
	$boHabilita = false;
	$acao = '';	
}

?>
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
 <tbody>
 	<tr>
 		<td colspan="2" style="text-align: left;"><b>Anexos</b></td>
 	</tr>
 </tbody>
</table>
<table class="Tabela" bgcolor="#eeeeee" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td>
		<table class="listagem" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
		    <tr>
			    <th>Arquivo</th>
			    <th>Descrição</th>
			    <?=$acao; ?>
		    </tr>
		    <?if( $boHabilita ){?>
		    <tr>
		        <td align="left">
		            <input type="file" name="arquivo" id="arquivo" />
		        </td>
		        <td align="left">
		        	<?
					if( $_GET['arqid'] ){
						$anxdsc = $db->pegaUm("select anxdsc from emenda.anexo where arqid = {$_GET['arqid']}");
					}
		        	echo campo_texto( 'anxdsc', 'N', $habilitado, 'Descrição Anexo', 100, 100, '', '','','','','id="anxdsc"'); ?>
		        </td>
		        <td align="center">
		        	<input type="button" name="botao" value="Salvar" onclick="validaForm();" />
		        </td>
		    </tr>
		    <?php
			}
		    
		    if($arDados) {
		    	$count = 1;
			    foreach($arDados as $dados){
			    	?>
					<tr>		    
				        <td align="left">
				        	<?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/orientacaoPreenchimento&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>'"><?php echo $dados['arquivo'];?></a>
				        </td>
				        <td align="left">
				        	<?php echo $dados['anxdsc']; ?>
				        </td>
				        <?
				        if( $boHabilita ){ ?>
				        <td align="center">
				        <? if( possuiPermissao() ) {?>
				        	<img src="../imagens/alterar.gif" onClick="window.location.href='emenda.php?modulo=principal/orientacaoPreenchimento&acao=A&arqid=<?php echo $dados['arqid'];?>&tipo='" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
				        	<img src="../imagens/excluir.gif" onClick="excluirAnexo('<?php echo $dados['arqid']; ?>');" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
				        <? } else { ?>
				        	<img src="../imagens/alterar_01.gif" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
				        	<img src="../imagens/excluir_01.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
				        <? } ?>
				        </td>
				        <?} ?>
				   	</tr>
			    	<?php
			    	$count++;
			    }
			}
		    ?>
		</table>
	</td>
</tr>
</table>
<script type="text/javascript">
var objForm = document.forms["formulario"];

if(objForm.arqid.value != ''){
	objForm.arquivo.disabled = true;
}

function validaForm(){
	var alerta = "";
	if(objForm.arquivo.value == '' && objForm.arqid.value == ''){
		alerta += "Você deve escolher um arquivo.\n";
	}
	if(objForm.anxdsc.value == ''){
		alerta += "O campo Descrição Anexo é Obrigatório.";
	}
	
	if(alerta){
		alert(alerta);
		return false;
	} 
	objForm.submit();
}

function excluirAnexo( arqid ){
 	if ( confirm( 'Deseja excluir o Documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid;
 	}
}
</script>
</form>