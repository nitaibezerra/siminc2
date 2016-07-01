<?php 
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
verificaPermissao();

$ptrid = ( !empty($_SESSION['emenda']['ptrid']) ? $_SESSION['emenda']['ptrid'] : $_SESSION['emenda']['ptridAnalise']);
$_SESSION['emenda']['ptrid'] = $ptrid;

validaSessionPTA( $_SESSION['emenda']['ptrid'] );

if($_POST['requisicao'] == 'salvar'){
	
	if( is_array($_FILES['arquivo']['name'])){		
		foreach ($_FILES['arquivo']['name'] as $key => $arquivo) {
				
			if( !empty($arquivo) ){
				$gdonome 	= $_POST['gdonome'][$key];
				$gdoid 		= ($_POST['gdoid'][$key] ? $_POST['gdoid'][$key] : 'null');
				$type 		= $_FILES['arquivo']['type'][$key];
				$size 		= $_FILES['arquivo']['size'][$key];
				$tmp_name	= $_FILES['arquivo']['tmp_name'][$key];
				
				$sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
						VALUES( '".current(explode(".", $arquivo))."', '".end(explode(".", $arquivo))."',
								'".$gdonome."', '".$type."', '".$size."', '".date('Y-m-d')."',
								'".date('H:i:s')."', '".$_SESSION["usucpf"]."', 57, 'A') RETURNING arqid";
				$arqid = $db->pegaUm($sql);
				
				$sql = "INSERT INTO emenda.anexo(arqid, ptrid, anxdsc, anxtipo, gdoid) 
						VALUES ($arqid, $ptrid, '$gdonome', 'P', $gdoid)";					
				$db->executar($sql);
				
				$caminhoEarquivo = APPRAIZ.'arquivos/emenda/'.floor($arqid/1000).'/'.$arqid;
				if(!is_dir(APPRAIZ."arquivos/emenda")) {
					mkdir(APPRAIZ."arquivos/emenda", 0777);
				}
				if(!is_dir(APPRAIZ."arquivos/emenda/".floor($arqid/1000))) {
					mkdir(APPRAIZ."arquivos/emenda/".floor($arqid/1000), 0777);
				}
				
				switch($type) {
					case 'image/jpeg':
						ini_set("memory_limit", "128M");
						list($width, $height) = getimagesize($tmp_name);
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
						$image   = imagecreatefromjpeg($tmp_name);
						imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);						
						imagejpeg($image_p, $caminhoEarquivo, 100);
						//Clean-up memory
						ImageDestroy($image_p);
						//Clean-up memory
						ImageDestroy($image);
						break;
					default:
						if ( !move_uploaded_file( $tmp_name, $caminhoEarquivo) ) {
							echo '<script type="text/javascript"> 
						    		alert("Problemas no envio do arquivo.");
						    		window.location.href = "emenda.php?modulo=principal/manterAnexos&acao=A";
						    	  </script>';
						    die;
						}
				}
			}
		}
	}
	if($db->commit()){
		$db->sucesso('principal/manterAnexos');		
	} else {
		echo '<script type="text/javascript"> 
	    		alert("Falha na Operação!");
	    		window.location.href = "emenda.php?modulo=principal/manterAnexos&acao=A";
	    	  </script>';
	    die;
	}
}

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'emenda.php?modulo=principal/manterAnexos&acao=A';</script>";
    exit;
} 
if( ($_FILES["arquivoobras"] || $_FILES["arquivo"]) && !$_POST['arqid']){
	
	if( $_FILES["arquivo"]["name"] ){
		$campos	= array("ptrid"  => $_SESSION['emenda']['ptrid'],
					"anxdsc" => "'{$_POST['anxdsc']}'"
					);
	} else {
		if( !empty($_POST['anxtermoref']) ){
			$campos	= array("ptrid"  => $_SESSION['emenda']['ptrid'],
						"anxdsc" => "'{$_POST['anxdscobras']}'",
						"anxtermoref" => "'{$_POST['anxtermoref']}'"
						);	
		} else {
			echo '<script type="text/javascript"> 
						alert("Falha na inserção do anexos!");
						window.location.href = "emenda.php?modulo=principal/manterAnexos&acao=A";
				  </script>';
			die;
		}
	}
	$file = new FilesSimec("anexo", $campos ,"emenda");
	$arquivoSalvo = $file->setUpload();
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
		echo"<script>window.location.href = 'emenda.php?modulo=principal/manterAnexos&acao=A';</script>";
		die;	
	}
} elseif( $_POST['arqid_obra'] || $_POST['arqid'] ) {
	$anxdsc = ( $_POST['tipo'] ? $_POST['anxdscobras'] : $_POST['anxdsc'] );
	$arqid = ( $_POST['arqid_obra'] ? $_POST['arqid_obra'] : $_POST['arqid'] );
	$anxtermoref = ( $_POST['anxtermoref'] ? "'".$_POST['anxtermoref']."'" : 'NULL' );
	
    $sql = "UPDATE emenda.anexo SET anxdsc = '{$anxdsc}', anxtermoref = {$anxtermoref} where arqid=".$arqid;
    $db->executar($sql);
    $db->commit();
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href = "emenda.php?modulo=principal/manterAnexos&acao=A";
    	  </script>';
    die;
}
 
if($_GET['arqidDel']){
    $sql = "DELETE FROM emenda.anexo where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $boPai = pegaPaiPTA($_SESSION['emenda']['ptrid']);
    if( empty($boPai) ){
	    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
	    $db->executar($sql);
    }
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href="emenda.php?modulo=principal/manterAnexos&acao=A";
    	  </script>';
    die;
}

//Chamada de programa
if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
print "<br/>";
exibeInstituicaoBenefiada();
print "<br/>";

//validaSessionPTA( $_SESSION['emenda']['federal'] );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

if( $_SESSION['emenda']['federal'] == 'S' ){
	$obrigatorio = 'S';
} else {
	$obrigatorio = 'N';
}
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );

monta_titulo( 'Anexos do PTA', 'Você pode inserir documentos que complementam seu plano de trabalho<br><img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
echo cabecalhoPlanoTrabalho($_SESSION['emenda']['ptrid']);

/**/

//background: rgb(238, 238, 238)
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<?
$boGuia = $db->pegaUm("SELECT 
							count(ggd.gdoid)
						FROM
							emenda.guia_planotrabalho gp
						    inner join emenda.guia g on g.guiid = gp.guiid
						    inner join emenda.guia_guiainiciativa ggi on ggi.guiid = g.guiid
						    inner join emenda.guia_guiadocumento ggd on ggd.ginid = ggi.ginid
						WHERE
							gp.ptrid = {$_SESSION['emenda']['ptrid']}");
//if( $_SESSION['exercicio'] != '2012' || (int)$boGuia == 0 ){

	$sql = "SELECT
				ini.iniobras
			FROM
				emenda.planotrabalho ptr
			    inner join emenda.ptiniciativa pti
			    	on pti.ptrid = ptr.ptrid
			    inner join emenda.iniciativa ini
			    	on ini.iniid = pti.iniid
			WHERE
				ptr.ptrid = {$_SESSION['emenda']['ptrid']}
			    and ptr.ptrstatus = 'A'
			    and ini.iniobras = 'S'";
	
	$iniobras = $db->pegaUm( $sql );
	
	if( $iniobras == 'S' ){
		$labelAnexo = 'Projeto Básico';
		$anxtermoref = 'PB';
	} else {
		$labelAnexo = 'Termo de referência';
		$anxtermoref = 'TR';
	}
	
	$sql = "select anx.anxid,
			   anx.arqid,
			   anx.ptrid,
			   anx.anxdsc,
			   arq.arqnome || '.' || arq.arqextensao as arquivo,
			   anx.anxtermoref,
			   arq.arqdata
			from emenda.anexo anx
				inner join public.arquivo arq on anx.arqid = arq.arqid
	 		where ptrid = {$_SESSION['emenda']['ptrid']} and anx.anxtipo = 'P' and anx.gdoid is null";
	$arDados = $db->carregar($sql);
	
	if( $_REQUEST['tipo'] ){
		$anxtermoref = $_REQUEST['tipo']; 
		$anxdscobras = $anxdsc;
	}
?>
<form name="form_obra" id="form_obra" method="post" enctype="multipart/form-data">
<input type="hidden" name="arqid_obra" id="arqid_obra" value="<?php echo $_GET['arqid_obra'] ?>" />
<input type="hidden" name="tipo" id="tipo" value="<?=$_REQUEST['tipo']; ?>" />
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
 <tbody>
 	<tr>
 		<td colspan="2" style="text-align: left;"><b>Anexos (<?=$labelAnexo; ?>)</b></td>
 	</tr>
 </tbody>
</table> 
<table class="Tabela" bgcolor="#eeeeee" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td>
		<table class="listagem" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
		    <tr>
			    <th>Arquivo</th>
			    <th>Data do Arquivo</th>
			    <th><?=$labelAnexo; ?></th>
			    <th>Tipo</th>
			    <th>Ação</th>
		    </tr>
		    <tr>
		        <td align="left">
		            <input type="file" name="arquivoobras" id="arquivoobras" />
		        </td>
		        <td align="left">&nbsp;</td>
		        <td align="left">
		        	<?
					if($_GET['arqid_obra'] && !empty($_GET['tipo']) ){
						$anxdscobras = $db->pegaUm("select anxdsc from emenda.anexo where arqid = {$_GET['arqid_obra']}");
					}
		        	echo campo_texto( 'anxdscobras', $obrigatorio, $habilitado, 'Descrição Anexo', 100, 100, '', '','','','','id="anxdscobras"'); ?>
		        </td>
		        <td>
		        	<input type="hidden" name="anxtermoref" id="anxtermoref" value="<?php echo $anxtermoref ?>" />
		        </td>
		        <td align="center">
		        	<input type="button" name="botao" value="Salvar" onclick="validaFormObras('<?=$labelAnexo; ?>');" <?=verificaPermissaoPerfil( 'pta', '', $estadoAtual, true ); ?> />
		        </td>
		    </tr>
		    <?php	    
		    $perfil = pegaPerfil($_SESSION['usucpf']);
		    
		    if($arDados) {
		    	$count = 1;
			    foreach($arDados as $dados){
			    	if( $dados['anxtermoref'] == $anxtermoref ){
			    	?>
					<tr>		    
				        <td align="left">
				        	<?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/manterAnexos&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>'"><?php echo $dados['arquivo'];?></a>
				        </td>
				        <td align="left">
				        	<?php echo formata_data($dados['arqdata']); ?>
				        </td>
				        <td align="left">
				        	<?php echo $dados['anxdsc']; ?>
				        </td>
				        <td align="center">
				        	<?php echo $dados['anxtermoref']; ?>
				        </td>
				        <td align="center">
				        <? if( verificaPermissaoPerfil( 'pta', 'boolean', $estadoAtual, true ) ) {?>
				        	<img src="../imagens/alterar.gif" onClick="window.location.href='emenda.php?modulo=principal/manterAnexos&acao=A&arqid_obra=<?php echo $dados['arqid'];?>&tipo=<?=$dados['anxtermoref'] ?>'" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
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
			    }
			}
		    ?>
		   </table>
	</td>
</tr>
</table><p>
</p></form>
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
 <tbody>
 	<tr>
 		<td colspan="2" style="text-align: left;"><b>Anexos adicionais</b></td>
 	</tr>
 </tbody>
</table>
<table class="Tabela" bgcolor="#eeeeee" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td>
		<table class="listagem" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
		    <tr>
			    <th>Arquivo</th>
			    <th>Data do Arquivo</th>
			    <th>Descrição</th>
			    <th>Ação</th>
		    </tr>
		    <tr>
		        <td align="left">
		            <input type="file" name="arquivo" id="arquivo" />
		        </td>
		        <td align="left">&nbsp;</td>
		        <td align="left">
		        	<?
					if($_GET['arqid'] && empty($_GET['tipo']) ){
						$anxdsc = $db->pegaUm("select anxdsc from emenda.anexo where arqid = {$_GET['arqid']}");
					}
		        	echo campo_texto( 'anxdsc', 'N', $habilitado, 'Descrição Anexo', 100, 100, '', '','','','','id="anxdsc"'); ?>
		        </td>
		        <td align="center">
		        	<input type="button" name="botao" value="Salvar" onclick="validaForm();" <?=verificaPermissaoPerfil( 'pta', '', $estadoAtual, true ); ?> />
		        </td>
		    </tr>
		    <?php
		    $perfil = pegaPerfil($_SESSION['usucpf']);
		    
		    if($arDados) {
		    	$count = 1;
			    foreach($arDados as $dados){
			    	if( empty( $dados['anxtermoref'] ) ){
			    	?>
					<tr>		    
				        <td align="left">
				        	<?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/manterAnexos&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>'"><?php echo $dados['arquivo'];?></a>
				        </td>
				        <td align="left">
				        	<?php echo formata_data($dados['arqdata']); ?>
				        </td>
				        <td align="left">
				        	<?php echo $dados['anxdsc']; ?>
				        </td>
				        <td align="center">
				        <? if( verificaPermissaoPerfil( 'pta', 'boolean', $estadoAtual, true ) ) {?>
				        	<img src="../imagens/alterar.gif" onClick="window.location.href='emenda.php?modulo=principal/manterAnexos&acao=A&arqid=<?php echo $dados['arqid'];?>&tipo='" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
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
			    }
			}
		    ?>
		</table>
	</td>
</tr>
</table>
</form>
<?/*}else{*/
if( $_SESSION['exercicio'] >= '2012' ){ ?>
<form name="formulario_guia" id="formulario_guia" method="post" enctype="multipart/form-data">
<input type="hidden" name="requisicao" id="requisicao" value="" />
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
 <tbody>
 	<tr>
 		<td colspan="2" style="text-align: left;"><b>Anexos adicionais - Guia</b></td>
 	</tr>
 </tbody>
</table>
<table class="Tabela" bgcolor="#eeeeee" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td>
		<table class="listagem" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
		    <tr>
			    <th width="15%">Arquivo</th>
			    <th width="05%">Data do Arquivo</th>
			    <th width="20%">Nome</th>
			    <th width="60%">Descrição</th>
		    </tr>
		    <?
			$sql = "SELECT DISTINCT
						gdo.gdoid, 
						gdo.gdonome, 
					    gdo.gdodescricao,
					    anx.arqid,
					    arq.arqnome || '.' || arq.arqextensao as arquivo,
					    arq.arqdata
					FROM
					    emenda.planotrabalho ptr
                        inner join emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid
                        inner join emenda.iniciativa_guiadocumento igd on igd.iniid = pti.iniid
					    inner join emenda.guiadocumento gdo on gdo.gdoid = igd.gdoid
					    left join emenda.anexo anx 
					    	inner join public.arquivo arq on anx.arqid = arq.arqid
					    on anx.ptrid = ptr.ptrid and anx.anxtipo = 'P' and anx.gdoid = gdo.gdoid and anx.gdoid is not null
					WHERE
					    ptr.ptrid = {$_SESSION['emenda']['ptrid']}";
			
			$arrDocumento = $db->carregar( $sql );
			$arrDocumento = $arrDocumento ? $arrDocumento : array();
			$count = 1;
			$boArquivo = true;
			foreach ($arrDocumento as $doc) {			
		    ?>
		    <tr>
		        <td align="left">
		        	<?if( $doc['arqid'] ){?>
		        		<? if( verificaPermissaoPerfil( 'pta', 'boolean', $estadoAtual, true ) ) {?>
				        	<img src="../imagens/excluir.gif" onClick="excluirAnexo('<?=$doc['arqid']; ?>');" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
				        <? } else { ?>
				        	<img src="../imagens/excluir_01.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
				        <? } ?>
		        		<?=$count.' - '; ?>
		        			<a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/manterAnexos&acao=A&download=S&arqid=<?=$doc['arqid'];?>'"><?=$doc['arquivo'];?></a>
		        	<?} else { 
		        		$boArquivo = false;?>
		            	<input type="file" name="arquivo[]" id="arquivo" />
		            	<input type="hidden" name="gdoid[]" id="gdoid" value="<?=$doc['gdoid']; ?>" />
		            	<input type="hidden" name="arqid[]" id="arqid" value="<?=$doc['arqid']; ?>" />
		            	<input type="hidden" name="gdonome[]" id="gdonome" value="<?=$doc['gdonome']; ?>" />
		            <?} ?>
		        </td>
		        <td align="left"><?=formata_data($doc['arqdata']); ?></td>
		        <td align="left">
		        	<?
					if($_GET['arqid']){
						$anxdsc = $db->pegaUm("select anxdsc from emenda.anexo where arqid = {$_GET['arqid']}");
					} else {
						$anxdsc = $doc['gdonome'];
					}
		        	echo $anxdsc; //campo_texto( 'anxdsc', 'N', 'N', 'Descrição Anexo', 100, 100, '', '','','','','id="anxdsc"'); ?>
		        </td>
		        <td><?=$doc['gdodescricao']; ?></td>
		    </tr>
		    <?$count++; 
			} ?>
		    <tr>
		    	<td align="center" colspan="3">
		        	<input type="button" name="botao" value="Salvar Documentos" <?=($boArquivo ? 'disabled="disabled"' : '') ?> onclick="validaForm2012();" <?=verificaPermissaoPerfil( 'pta', '', $estadoAtual, true ); ?> />
		        </td>
		    </tr>
		</table>
	</td>
</tr>
</table>
</form>
<?}
//} ?>
<script language="javascript" type="text/javascript">
var objFormObra = document.forms["form_obra"];
var objForm = document.forms["formulario"];

if( objFormObra.tipo.value ){
	if(objFormObra.arqid_obra.value != ''){
		objFormObra.arquivoobras.disabled = true;
	}
}else{
	if(objForm.arqid.value != ''){
		objForm.arquivo.disabled = true;
	}
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
function validaForm2012(){
	var alerta = "";
	$("[type~=file]").each(function() {
		if(this.value == ''){
			alerta = "Você deve escolher um arquivo.\n";
		}
	});
	
	if(alerta){
		//alert(alerta);
		//return false;
	} 
	$('#requisicao').val('salvar');
	document.getElementById('formulario_guia').submit();
}

function validaFormObras(label){
	var alerta = "";
	if(objFormObra.arquivoobras.value == '' && objFormObra.arqid_obra.value == ''){
		alerta += "Você deve escolher um arquivo.\n";
	}
	if(objFormObra.anxdscobras.value == ''){
		alerta += "O campo "+label+" é Obrigatório.";
	}
	
	if(alerta){
		alert(alerta);
		return false;
	} 
	objFormObra.submit();
}
	 
function excluirAnexo( arqid ){
 	if ( confirm( 'Deseja excluir o Documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid;
 	}
 }
</script>